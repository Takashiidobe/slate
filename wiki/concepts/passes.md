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

The engine builds a `FunctionOptimizer` containing an `Arena` of body nodes and
function-signature facts, then applies `NodeRule`s while rescheduling affected
nodes until a fixed point. The current registry
(`engine/rules/mod.rs`, in order):

- `ZeroInitFold` (`zero_init.rs`) — fuses a zero-init `let` with the assignment that overwrites it.
- `ParamSpillFold` (`param_spills.rs`) — renames a parameter to its same-typed top-level mutable spill and deletes the spill when the parameter's only read is that initialization.
- `RawPtrAliasElide` (`raw_ptr_alias.rs`) — collapses redundant raw-pointer alias locals.
- `ScopeFlatten` (`singleton_scopes.rs`) — splices any `{ }` scope's statements into its parent's statement list in place, since `cir.scope` always lowers to a plain `Stmt::Scope` regardless of what it wraps (a for-loop's induction-variable scope included).
- `ForRangeRecover` (`for_range.rs`) — recognizes the canonical desugared for-loop shape (`let mut i; i = start; loop { if !(i<end){break} body; i=i+1; }`) and rewrites it to `for i in start..end { body }`, requiring the increment to be exactly `i = i + 1`, `body` to never reassign `i`, and `i` to have no reads outside the recognized region.
- `ForArrayIterRecover` (`array_iter.rs`) — follow-on to `ForRangeRecover`: when a `for i in 0..N { body }`'s only use of `i` is as `arr[cast(i)]` for a single `Prim`-element array `arr` of length `N` not otherwise referenced in `body`, rewrites to `for i in arr.iter().copied() { ...i... }` (`.copied()` rather than `into_iter()` since `arr` is typically wrapped in `aligned::Aligned<_, [T; N]>`, whose `Deref` only ever yields `&[T; N]` under method-call autoderef, never an owned array).
- `LoopToWhile` (`loop_to_while.rs`) — fallback for the general lowerer-emitted head-tested `loop { if !cond { break } body }` shape (any `while`/`for` the lowerer built, not just ones `ForRangeRecover` can further recover into a range) into idiomatic `while cond { body }`. Priority 20, run after `ForRangeRecover`/`ForArrayIterRecover` (13/14) so those get first claim on the raw `Loop` shape. Only fires on unlabeled loops — `Stmt::While` has no label field, so a loop kept alive by a real `continue`/labeled `break` (e.g. `continue_while.c`) stays in `loop` form until that's added.
- `ReturnCleanup` (`return_cleanup.rs`) — collapses an adjacent synthetic `__retval = value; return __retval;` pair after proving every slot use is a direct store, return, or single-use return-forwarding temp; removes the dead slot through `DeadStore`; and renders a final top-level return of a proven `Copy` type as a Rust tail expression.
- `LateInlineTemps` (`inline_temps.rs`) — inlines single-use temps into their sole use (pure temps generally; effectful/atomic ones into an adjacent use).
- `EffectfulTempForward` (`inline_temps.rs`) — sinks a single-use effectful temp (chiefly a call result) forward into its one argument position.
- `InlineConstArgTemps` (`inline_temps.rs`) — inlines a non-type-anchored numeric-constant temp into its sole call/macro argument.
- `PeelCasts` (`peel_casts.rs`) — drops a redundant outer cast in an adjacent pair (`(e as T) as T`, or `(e as A) as B` where `A`,`B` are thin raw pointers and `e` provably yields a pointer/integer), never dropping a float or narrowing intermediate or touching a reference operand.
- `DeadStore` (`dead_store.rs`) — deletes a `let` with no def-use readers when its initializer is side-effect-free.
- `libc_call::rules()` (`libc_call.rs`) — the libc call-rewrite table (`memcpy`/`memmove`/`memset`/`str*`/… → native Rust or `Box`/slice ops), matched by call anchor.

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
