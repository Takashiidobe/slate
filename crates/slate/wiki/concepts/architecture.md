# Architecture

## Why CIR, not LLVM IR

Translation quality is bounded by how much structure the source IR retains.
LLVM IR has already destroyed what Rust needs:

- **Control flow** becomes a CFG of basic blocks — no `for`/`while`/`if`.
  Recovering structure (the "relooper" problem) is hard and, for irreducible
  graphs, impossible without node duplication or dispatch variables.
- **SSA + phi nodes** replace source variables.
- **Signedness and source types** are largely gone (`add` is neither signed nor
  unsigned; structs are scalarized).

ClangIR (CIR) sits between the Clang AST and LLVM IR. In its high-level,
pre-`cir-flatten-cfg` form it keeps exactly what we need:

- Structured control flow as region-carrying ops (`cir.for`, `cir.scope`,
  `cir.if`, `cir.switch`).
- Named local variables as `cir.alloca "x"` memory slots (not SSA) — these map
  directly to Rust `let mut`, sidestepping phi reconstruction.
- Integer signedness in the type (`!cir.int<s, 32>` vs `<u, 32>`).

We consume the **MLIR generic form** (`cir-opt --mlir-print-op-generic`) because
it is fully regular — `"op"(operands) <{attrs}> ({regions}) : type` — which makes
a stable, op-agnostic parser possible. clang-ir parses this into a generic
`Operation` tree and converts function bodies to its generated `Op` model (see
[Two IRs](#two-irs) below). Slate dispatches directly on that generated enum;
it has no separate operation-kind taxonomy.

**Escape hatch for `goto`**: arbitrary `goto` (including Duff's-device-shaped
jumps into nested scopes) doesn't fit the structured region model above.
`frontend::toolchain::emit_generic_with_args_flattened` is a second, opt-in emission
path that runs `cir-opt --cir-flatten-cfg --cir-goto-solver`, producing a
plain multi-block CFG (`cir.switch.flat`/`cir.brcond`/real `cir.br` edges)
instead of the nested structured form, which `frontend::cir_input` then turns
into a `loop { match state { .. } }` dispatch. This is deliberately only
invoked for functions that actually contain a `goto` — running it
unconditionally would still be correct but would degrade every goto-free
function from native Rust `if`/`loop` control flow into the uglier dispatch
form.

## The three sources

Every input is available in three forms, all keyed by **source location**:

| Source    | How obtained                                                     | Role                                     |
| --------- | ---------------------------------------------------------------- | ----------------------------------------- |
| CIR       | `clang -fclangir -emit-cir` \| `cir-opt --mlir-print-op-generic` | primary lowering input                   |
| Clang AST | `clang -Xclang -ast-dump=json -fsyntax-only`                     | structured source context and raw oracle |
| C text    | read the file                                                    | comments / naming (final polish)         |

**Location is the join key.** CIR ops carry `loc("f.c":4:13)`; AST nodes carry
source ranges; C text is addressable by line:col. "Consult the AST" means: take
a CIR op's `loc`, look up the AST node covering that range, read the
disambiguating fact. The parser retains locations for exactly this reason —
they are structural, not noise.

`src/frontend/c_ast.rs` loads Clang's JSON AST during `translate`. It filters
the dump down to source-file function definitions, extracts a compact `Unit`
(`Function`, `Decl`, `Stmt`, `Expr`, `CType`, `Loc`), and preserves each
function's raw Clang JSON node. The compact AST is the common path; the raw
node is the escape hatch when a new feature needs more source facts before the
compact model has grown.

### Plugin provenance events

The Clang plugin (`SLATE_MACRO_DUMP_PLUGIN`, built by
`tools/macro-dump-plugin/build.sh`) emits three line-oriented JSON event types
on stderr:

- `MACRO_EXPANSION` records macro names at main-file source offsets.
- `INCLUDE_PROVENANCE` records the written include name, angle-versus-quote
  form, physical resolved file, includer location, and Clang system-header
  characteristic.
- `FUNCTION_PROVENANCE` records each main-file call site, its direct Clang
  declaration binding when one exists, canonical type, redeclaration evidence,
  trusted system-header ancestry, source macro spelling, resolved assembler or
  alias name, weak-import state, availability versions, and conservative
  rejection reasons.

`trusted_header` means that the declaration chain reaches a physical header
resolved by an angled include from a Clang system search path and has no
untrusted definition or symbol-changing declaration. It does not by itself mean
that the function is a particular libc API. Consumers must additionally match a
known function identity, required header, and canonical signature. `unknown` is
the default for indirect calls, project-only declarations, project definitions,
aliases, asm labels, weak declarations, and ifunc declarations.

Header provenance describes source-level declaration binding under the exact
Clang invocation. It does not prove which implementation a static or dynamic
linker selects, and it does not rule out runtime symbol interposition.

## Two IRs

The pipeline flows through two internal representations on the frontend side:

1. **clang-ir CIR model** — its parser first builds generic `Operation` nodes,
   then its generated `Op` model provides checked operation structs with named
   operands, results, attributes, successors, and regions. Slate lowers the
   generated model directly while retaining generic operations for module-level
   collection and analysis.
2. **Clang source context** (`src/frontend/c_ast.rs`) — a compact AST plus raw
   JSON, keyed by source locations and function names. This is not a
   handwritten C parser; it is Clang's semantic AST reduced to the facts Slate
   currently needs.

`src/backend/rust_ast.rs` is the output-side tree and printer. The lowerer
(`src/frontend/lowerer.rs` and `src/frontend/lowerer/`) builds it directly:
every handler emits structured `Item`/`Stmt`/`Expr` nodes that
`src/backend/codegen.rs` renders once, never Rust source strings. Keep that
output as strongly typed as possible — a new enum variant is preferred over a
`String` bridge, so the compiler enforces exhaustiveness and fixups can
pattern-match the shape. Fixups (`src/backend/`) operate on the same tree
(AST-to-AST only, see [fixups.md](fixups.md)). AST nodes can also carry typed,
non-rendered metadata populated by lowering when CIR or the Clang AST provides
semantic contracts that Rust syntax cannot express; fact analysis
(`src/backend/facts/`, [facts.md](facts.md)) imports that metadata without
coupling the lowerer to the fixup fact API.

**See [lowerer internals](lowerer-internals.md) for the lowerer's internal module split** — the
`Lowerer`/`FunctionLowerer` two-tier design, the submodule-by-concern table,
op dispatch, and how to wire in a new `cir.*` handler.

## Shared context

`Ctx` (`src/ctx.rs`) is threaded through lowering:

```rust
pub struct Ctx {
    pub diagnostics: Diagnostics, // unsupported-construct reports
}
```

`Diagnostics` is what keeps the pipeline runnable as coverage grows: an
unhandled op lowers to a marked fallback (a `todo!()`, an `unsafe` `libc` call,
or a comment) and records a diagnostic instead of crashing or silently
dropping code. Translation-unit-wide lowering state itself (globals, records,
enums, known function signatures) lives on `Lowerer`, not `Ctx` — see
[lowerer internals](lowerer-internals.md).

## Pipeline shape

The pipeline is fixed and explicit in the CLI (`src/main.rs`): emit CIR, parse
CIR, load Clang AST JSON, lower to Rust, apply fixups, and verify with
generated differential tests. Do not add pass scheduling machinery until a
feature needs conditional ordering.

See [passes.md](passes.md) for the current stages, the fixup pass sequence,
and the extension workflow.

## Adding a feature

- **Lowering feature** (teach Slate to preserve more C semantics in baseline
  Rust — structs, arrays, pointer arithmetic, a new arithmetic operator, a new
  `cir.*` op): see [lowerer internals](lowerer-internals.md#adding-a-new-cir-handler).
- **Rust fixup** (turn already-correct baseline Rust into more idiomatic or
  safer Rust): see [writing a query-driven fixup](writing-a-query-driven-fixup.md).

Both start from a fixture in `tests/fixtures/` and a failing differential test
— never hand-verify output by eye. Keep baseline lowering conservative
(`#[repr(C)]`, raw pointers, explicit temps, `libc`, `unsafe` are all
acceptable); recover idiom only in a separate, independently-verified fixup.
