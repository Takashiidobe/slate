# Passes

Baseline lowering is deliberately ugly (`#[repr(C)]`, raw pointers, explicit
temps, `libc`, `unsafe`); readability is recovered afterward by rewrites, never
during lowering. This page catalogs the stages and the **current** rewrite
passes. For the rewrite engine's mechanics see
[rewrite-engine-v2.md](rewrite-engine-v2.md); for the whole-pipeline map see
[architecture.md](architecture.md).

## Pipeline

| Stage              | In → Out                               | How                                                                    |
| ------------------ | -------------------------------------- | ---------------------------------------------------------------------- |
| **emit-cir**       | C → CIR text                           | `clang -fclangir -emit-cir` piped to `cir-opt --mlir-print-op-generic` |
| **parse-cir**      | CIR text → clang-ir `Op` model         | clang-ir parser + generated operation conversion                       |
| **load-ast**       | C → compact source context + raw JSON  | `clang -Xclang -ast-dump=json -fsyntax-only`                           |
| **lower**          | CIR + AST → baseline Rust AST          | match generated `Op`; materialize temps; `libc`/`unsafe` fallbacks     |
| **rewrite**        | baseline Rust AST → cleaner Rust AST   | `backend::apply` → `engine::apply` (interproc analyses, then worklist) |
| **generated-diff** | C + generated Rust → output comparison | build generated Rust with Cargo, compare stdout + exit code            |

```text
emit-cir → parse-cir → load-ast → lower(libc/unsafe) → rewrite → generated-diff
```

`NEXTEST_PROFILE=lowering` and `SLATE_RAW_LOWER` short-circuit `backend::apply`
to return the baseline unchanged — the `lowering` profile tests baseline Rust,
the `rewrites` profile the rewritten output. Parsing and lowering detail: the
parser is generic (produces `Op { results, name, operands, attrs, regions, ty,
loc }` and does not know op semantics); the lowerer is the only stage that knows
CIR op semantics and emits typed `rust_ast` nodes, never Rust source strings
(see [lowerer-internals.md](lowerer-internals.md)).

## The rewrite stage

`engine::apply` (`src/backend/engine/mod.rs`) runs in two phases:

```text
engine::apply(program):
  1. interproc analyses      (whole-Program, run first)
       string_params::run          — lift C-string params toward &str
       pointer_lattice::apply       — resolve each pointer param's Rust shape;
                                       internally runs length_lattice for
                                       pointer+length → slice bridging
  2. per-function worklist   (build an Arena per function, run NodeRules to a
                              fixed point; EDIT_BUDGET guards oscillation)
  3. prelude::inject         — add helper preludes the rewrites referenced
```

### Interproc analyses (phase 1)

Whole-`Program` fixpoints over the call graph, run before any per-function arena
exists. Each is monotone and bounded (`interproc::run_worklist`).

- **`string_params`** (`interproc/string_params.rs`) — lifts NUL-terminated
  `char*` parameters to `&str`, flowing eligibility caller→callee.
- **`pointer_lattice`** (`interproc/pointer_lattice.rs`) — the c2rust-derived
  capability lattice that chooses each pointer's Rust representation
  (`&T`/`&mut T`/`&[T]`/`Box<T>`/`Vec<T>`/`*const`/`*mut`, with `Option<…>` and
  string specializations) from observed write/unique/free/offset/escape/
  nullable/string bits, propagated bidirectionally across call sites.
  **[pointer-capability-lattice.md](pointer-capability-lattice.md) is the
  canonical reference** for the lattice tables, evaluation order, and what is /
  isn't wired up.
- **`length_lattice`** (`interproc/length_lattice.rs`, invoked from
  `pointer_lattice::apply`) — pairs a pointer parameter with its length
  parameter into a single slice, and proves UTF-8 for the `Vec → String` row.

### Per-function worklist rules (phase 2)

The engine builds an `Arena` of AST nodes per function and applies `NodeRule`s,
rescheduling affected nodes until a fixed point. The current registry
(`engine/rules/mod.rs`, in order):

1. `ZeroInitFold` (`zero_init.rs`) — fuse a zero-init `let` with the assignment
   that overwrites it.
2. `RawPtrAliasElide` (`raw_ptr_alias.rs`) — collapse redundant raw-pointer
   alias locals.
3. `WhileLoopUnwrap` / `DoWhileLoopUnwrap` / `SingletonUnwrap`
   (`singleton_scopes.rs`) — unwrap a loop's redundant body scope around its
   negated-break guard, and one-statement `{ }` scopes.
4. `LateInlineTemps` (`inline_temps.rs`) — inline single-use temps into their
   sole use (pure temps generally; effectful/atomic ones into an adjacent use).
5. `EffectfulTempForward` (`inline_temps.rs`) — sink a single-use effectful
   temp (chiefly a call result) forward into its one argument position.
6. `InlineConstArgTemps` (`inline_temps.rs`) — inline a non-type-anchored
   numeric-constant temp into its sole call/macro argument.
7. `PeelCasts` (`peel_casts.rs`) — drop a redundant outer cast in an adjacent
   pair: `(e as T) as T`, and `(e as A) as B` where `A`,`B` are thin raw
   pointers and `e` provably yields a pointer/integer. Never drops a float or
   narrowing intermediate; never touches a reference operand.
8. `DeadStore` (`dead_store.rs`) — delete a `let` with no def-use readers when
   its initializer is side-effect-free.
9. `libc_call::rules()` (`libc_call.rs`) — the libc call-rewrite table
   (`memcpy`/`memmove`/`memset`/`str*`/… → native Rust or `Box`/slice ops),
   matched by call anchor.

This is a much smaller set than the retired straight-line engine (~65 passes at
`src/backend/query/rules/*`, now under `wiki/historical/`). The v2 worklist
engine is a mid-flight port — [rewrite-engine-v2.md](rewrite-engine-v2.md) is
the handoff spec and tracks which passes have landed. When you port or add a
rule, register it in `engine/rules/mod.rs` and start from a failing differential
fixture.

## Debugging the rewrite stage

The engine does not currently emit a per-pass trace, so `fixup-debug`'s
`--up-to-pass`/`--only-pass`/`--debug-only-pass` options are inert (it emits the
fully-rewritten output). The working comparison workflow is baseline-vs-rewritten:

```bash
cargo run -- translate-lowered tests/fixtures/<name>.c   # baseline, no rewrites
cargo run -- translate         tests/fixtures/<name>.c   # after rewrites
```

`translate-lowered` (equivalently `SLATE_RAW_LOWER=1 translate`) is the
canonical way to tell a lowering bug from a rewrite bug: if the baseline is
already wrong, it's a lowering issue; if only the rewritten output is wrong, it's
a rule.

## Adding a feature vs. a rewrite

- **Feature** (teach baseline lowering to preserve more C — a new `cir.*` op,
  structs, pointer arithmetic): implement conservative lowering, see
  [lowerer-internals.md](lowerer-internals.md#adding-a-new-cir-handler).
- **Rewrite** (turn correct baseline Rust into idiomatic Rust): add a `NodeRule`
  or extend the libc table, see [rewrite-engine-v2.md](rewrite-engine-v2.md).

Both start from a C fixture in `tests/fixtures/` and a failing differential test
— never hand-verify output by eye. See
[differential-fixtures.md](differential-fixtures.md) for the fixture + FileCheck
workflow.
