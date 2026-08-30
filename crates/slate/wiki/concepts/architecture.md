# Architecture

A one-stop map of the pipeline: what each stage consumes and produces, which
crate or module owns it, and **where to target a given fix**. Deep-dives live in
[lowerer-internals.md](lowerer-internals.md), [rewrite-engine-v2.md](rewrite-engine-v2.md),
[passes.md](passes.md), and [differential-fixtures.md](differential-fixtures.md); this
page is the index that gets you to the right one.

## The pipeline at a glance

```
                          ┌─────────────────────── clang-ir crate ───────────────────────┐
  C source                │                                                              │
     │                    │ clang -fclangir -emit-cir ─▶ cir-opt --mlir-print-op-generic │
     ├── preprocess ──────┤                                                              │
     │   (#if/#cfg,       │                     │                                        │
     │    diagnostics)    │            parse ─▶ Operation tree ─▶ generated Op model     │
     │                    └─────────────────────────────────────────────┬────────────────┘
     │                                                                   │ Module (CIR)
     └── clang -ast-dump=json ───────────────▶ src/frontend/c_ast.rs ────┤ Unit (compact AST)
         + SLATE_MACRO_DUMP_PLUGIN provenance                            │
                                                                         ▼
                                        src/frontend/lowerer/*  ── LOWERING ──▶ Program
                                        (Module + Unit, joined by source location)   │ (rust_ast)
                                                                                     ▼
                                        src/backend/{interproc,engine}/* ── REWRITING ──▶ Program
                                        (lattices, then AST-to-AST worklist rules)    │
                                                                                      ▼
                                                     src/backend/codegen.rs ── EMIT ──▶ Rust source
```

Two independent knobs turn stages off: `NEXTEST_PROFILE=lowering` or
`SLATE_RAW_LOWER` skips **rewriting** entirely (baseline Rust only); the
`rewrites` profile runs the whole thing. The pipeline order is fixed and
explicit in `src/api.rs` (`lowered_program_with_args`) and `src/main.rs` — there
is no pass-scheduling machinery to configure.

## Why CIR, not LLVM IR

Translation quality is bounded by how much structure the source IR retains, and
LLVM IR has already destroyed what Rust needs: control flow is a CFG of basic
blocks (no `for`/`while`/`if`; irreducible graphs need node duplication or
dispatch variables to re-structure), source variables are gone into SSA + phi
nodes, and signedness and source types are largely erased.

ClangIR (CIR) sits between the Clang AST and LLVM IR. In its high-level,
pre-`cir-flatten-cfg` form it keeps what we need:

- Structured control flow as region-carrying ops (`cir.for`, `cir.scope`,
  `cir.if`, `cir.switch`).
- Named locals as `cir.alloca "x"` memory slots (not SSA) — these map directly
  to Rust `let mut`, sidestepping phi reconstruction.
- Integer signedness in the type (`!cir.int<s, 32>` vs `<u, 32>`).

**Escape hatch for `goto`**: arbitrary `goto` doesn't fit the structured region
model. For functions that contain a `goto`, Slate opts into a second emission
path that runs `cir-opt --cir-flatten-cfg --cir-goto-solver`, producing a plain
multi-block CFG, which lowering turns into a `loop { match state { .. } }`
dispatch. It is invoked only when needed — running it unconditionally would be
correct but would degrade every goto-free function into the uglier dispatch
form. (This flattening is why a value defined in one flattened block and used in
another must be recognized as cross-block-live during lowering.)

## The clang-ir crate (CIR ingestion)

Slate does not parse C or CIR by hand. The sibling `clang-ir` workspace
(`../clang-ir`, path dependency in `Cargo.toml`) owns everything about turning
CIR text into typed Rust values. Keep the split in mind but treat it as a
black box unless you're adding a CIR op:

- **`clang-ir-types-gen`** is a code generator that emits the **op / type /
  attribute / enum model** — the `Op` enum and per-op structs like `GetMember {
addr, result, .. }` that lowering matches on. When a CIR op Slate needs isn't
  in the model yet, that generator is what grows to add it (regenerate, don't
  hand-edit the generated `clang-ir-types` output).
- **`clang-ir`** holds the parsing helpers and `toolchain.rs`: it runs
  `clang`/`cir-opt`, lexes the MLIR generic form, builds a generic `Operation`
  tree, and converts function bodies into the generated `Op` model. Slate
  dispatches directly on that generated enum; it has no separate operation-kind
  taxonomy.

Slate consumes the **MLIR generic form** (`"op"(operands) <{attrs}> ({regions})
: type`) precisely because it is fully regular, which makes a stable,
op-agnostic parser possible. `src/frontend/cir_input.rs` drives emission and
hands lowering a `Module`.

## The three sources, joined by location

Every input is available in three forms, all keyed by **source location**:

| Source    | How obtained                                                     | Role                                     |
| --------- | ---------------------------------------------------------------- | ---------------------------------------- |
| CIR       | `clang -fclangir -emit-cir` \| `cir-opt --mlir-print-op-generic` | primary lowering input                   |
| Clang AST | `clang -Xclang -ast-dump=json -fsyntax-only`                     | structured source context and raw oracle |
| C text    | read the file                                                    | comments / naming (final polish)         |

**Location is the join key.** CIR ops carry `loc("f.c":4:13)`; AST nodes carry
source ranges. "Consult the AST" means: take a CIR op's `loc`, look up the AST
node covering that range, read the disambiguating fact (signedness of a literal,
a field's declared type, a macro spelling). `src/frontend/c_ast.rs` loads
Clang's JSON AST, filters it to source-file function definitions, extracts a
compact `Unit` (`Function`, `Decl`, `Stmt`, `Expr`, `CType`, `Loc`), and keeps
each function's raw Clang JSON node as an escape hatch for facts the compact
model hasn't grown yet.

### Provenance from the plugin

The Clang plugin (`SLATE_MACRO_DUMP_PLUGIN`, built by
`tools/macro-dump-plugin/build.sh` against the same clang tree as `SLATE_CLANG`)
emits line-oriented JSON on stderr: `MACRO_EXPANSION` (macro names at source
offsets), `INCLUDE_PROVENANCE` (written include, quote/angle form, resolved
file, system-header characteristic), and `FUNCTION_PROVENANCE` (each call site's
declaration binding, canonical type, trusted-system-header ancestry, asm/alias
name, weak-import and availability). `trusted_header` means the declaration
chain reaches an angled system header with no untrusted redefinition — it does
**not** by itself identify a libc API; consumers must additionally match a known
function identity, required header, and canonical signature.

## libc-shim (what "the system headers" are)

Slate never parses the host's real libc headers. `SLATE_CLANG` runs with
`-nostdlibinc -isystem libc-shim/include`, so `libc-shim/` **is** the C standard
library as far as CIR emission is concerned (clang's own freestanding builtin
headers — `stddef.h`, `stdint.h`, `stdatomic.h` — stay available). This pins the
ABI and declaration surface to something Slate controls and can model per
target, instead of whatever the build host happens to ship. Target variants live
alongside (`bionic-*`, `macos-*`, `msvc-*` header lists and `bits/`).

The reverse direction is `src/frontend/c_shim.rs`: when the generated Rust calls
into libc, it renders a small **shim C source** declaring exactly those externs
so the differential harness can compile and link the C side consistently. Shim
and header work is exercised by the `libc` nextest profile — see
[libc-shim.md](libc-shim.md).

## Directive translate (`#if`/`#cfg` reconstruction)

Preprocessing is its own producer, kept separate from lowering. `src/frontend/
preprocess.rs` records the translation unit's directive structure — `#if` /
`#ifdef` / `#elif` chains, `#error`/`#warning`, pragmas — as `Branch` /
`CondChain` records, and maps each condition to a Rust `Cfg`
(`src/backend/rust_ast.rs`). Active `#error`/`#warning` on the selected config
become `compile_error!` items so the failure survives translation. The
experimental `translate-directives` command
(`src/frontend/directive_translate.rs`) uses these records to reconstruct **one
Rust source carrying multiple `#[cfg]` configurations** instead of collapsing to
the single config clang happened to preprocess. High-level for now; the checks
run under `DIRECTIVES` FileCheck prefixes.

## Lowering (CIR + AST → baseline Rust AST)

This is the core, in `src/frontend/lowerer.rs` and `src/frontend/lowerer/`.
`frontend::lower(&module, &unit, &mut ctx)` returns a `Program` (a
`src/backend/rust_ast.rs` tree), which `src/backend/codegen.rs` renders — the
lowerer never builds Rust _source strings_, only typed `Item`/`Stmt`/`Expr`
nodes, so the compiler enforces exhaustiveness and rewrites can pattern-match
shape. Keep output strongly typed: prefer a new enum variant over a `String`
bridge, and attach typed, non-rendered metadata to nodes when CIR or the AST
proves a contract Rust syntax can't express.

Two tiers (full map in [lowerer-internals.md](lowerer-internals.md)):

```
Lowerer                     translation-unit state: globals, records, enums,
  │                         known signatures, aliases, string table
  └─ FunctionLowerer        per-function: SSA value map, alloca slots,
       │                    member_ptrs/element_ptrs, dispatch context
       └─ lower_op(op)  ──▶ one handler per cir.* op, split by concern:
```

| Concern           | Submodule(s)                                                        |
| ----------------- | ------------------------------------------------------------------- |
| op dispatch       | `dispatch.rs` (the big `match` in `lower_op`)                       |
| control flow      | `control_flow.rs` (`if`/loops/scope, goto dispatch)                 |
| memory / places   | `memory.rs`, `storage.rs` (load/store, get_member/element, allocas) |
| values / temps    | `values.rs` (`materialize_expr`, SSA value map)                     |
| arithmetic, calls | `arithmetic.rs`, `calls.rs`, `builtins.rs`, `intrinsics*.rs`        |
| types / records   | `types.rs`, `record_analysis.rs`, `bitfields.rs`                    |
| whole-fn analysis | `analysis.rs` (e.g. cross-block liveness), `function_setup.rs`      |

Key mechanics a fix usually touches: **op dispatch** (`lower_op` routes each
`Op::*` to a handler; a new op = a new arm + handler), the **SSA value map**
(`materialize_expr` decides `let _vN = ..` vs a hoisted `let mut`), **place
inlining** (`get_member`/`get_element` are stashed as `member_ptr`/`element_ptr`
and expanded at each use rather than bound), **goto dispatch** (`lower_dispatch`
turns flattened blocks into a `loop { match state }`, with `cross_block_live_
values` deciding which SSA temps must be hoisted across states), and the
**diagnostics fallback** — an unhandled op lowers to a marked `todo!()` / `unsafe`
libc call / comment and records a `Ctx` diagnostic instead of crashing, which is
what keeps the pipeline runnable as coverage grows.

## Rewriting (baseline Rust AST → idiomatic Rust AST)

Baseline lowering is allowed to be ugly (`#[repr(C)]`, raw pointers, explicit
temps, `unsafe`, `libc`) because correctness is the only bar. Rewrites recover
idiom — safe references, `Vec`/`Box`, `for x in ..`, compound assignment — as
**separately verified, AST-to-AST** passes, in `src/backend/`. Entry point is
`engine::apply(program)`:

```
engine::apply(program):
  1. interproc analyses  (src/backend/interproc/*)  — whole-program, run first
       string_params ▸ pointer_lattice ▸ length_lattice
       decide signatures/provenance: *mut vs &mut vs &[T], Box, string lifting
  2. per-function worklist  (src/backend/engine/*)
       build an Arena of nodes ─▶ run_worklist:
         pop node ▸ RuleRegistry.candidates(kind) ▸ rule.apply ▸ reschedule
         neighbors/parents; EDIT_BUDGET guards oscillation
       rules: inline_temps (early/late), zero_init, raw_ptr_alias,
              singleton_scopes (loop/scope unwrap), libc_call table
  3. prelude::inject  — add helper preludes the rewrites referenced
```

The interproc **lattices** are the cross-function part: `pointer_lattice`
resolves each pointer parameter to its safest Rust shape (`&T`/`&mut T`/`&[T]`/
`Box`/`*const`/`*mut`) by fixed-point over observed writes, offsets, escapes,
and call-site propagation; `length_lattice` pairs pointer+length params into
slices; `string_params` lifts C strings. A rule is a `NodeRule` matched by node
kind (and optional call anchor); the worklist reruns only affected nodes to a
fixed point. Full authoring contract and the pass catalog:
[rewrite-engine-v2.md](rewrite-engine-v2.md) and [passes.md](passes.md); the
pointer-representation lattice has its own canonical reference,
[pointer-capability-lattice.md](pointer-capability-lattice.md). (The older
`src/backend/query/` + salsa engine these replaced is documented under
`wiki/historical/`.)

## Verification

Correctness is checked by **differential testing**: compile and run both the C
source and the generated Rust, require identical stdout and exit code. FileCheck
directives add generated-Rust _shape_ assertions on top. Every feature and fixup
starts from a fixture in `tests/fixtures/` and a failing differential test —
never hand-verify output by eye. See
[differential-fixtures.md](differential-fixtures.md) for markers, the
`update_filecheck.py` scaffolder, and per-fixture clang-arg overrides.

## Where to target a fix

| Symptom                                                         | Subsystem → start here                                                     |
| --------------------------------------------------------------- | -------------------------------------------------------------------------- |
| A `cir.*` op emits `todo!()` / is unhandled                     | lowering op dispatch — `lowerer/dispatch.rs` + a handler                   |
| Wrong control-flow shape, goto/`match state`, out-of-scope temp | lowering control flow — `control_flow.rs`, `analysis.rs`                   |
| Wrong pointer type (`&` vs `&mut` vs slice), missing `Box`      | rewriting interproc — `interproc/pointer_lattice.rs` / `length_lattice.rs` |
| Ugly-but-correct Rust that should be idiomatic                  | rewriting rule — `engine/rules/*`                                          |
| A CIR op/type/attr Slate can't represent                        | `../clang-ir/clang-ir-types-gen` (regenerate the model)                    |
| Wrong libc declaration / ABI / missing header                   | `libc-shim/` (+ `c_shim.rs`); run the `libc` profile                       |
| `#if`/`#cfg` handling, `compile_error!` from directives         | `frontend/preprocess.rs`, `directive_translate.rs`                         |
| Wrong final Rust text for a correct AST                         | output side — `backend/rust_ast.rs`, `backend/codegen.rs`                  |
| Need a source fact CIR lacks (signedness, field type, macro)    | AST oracle — `frontend/c_ast.rs` (compact `Unit` or raw JSON)              |
