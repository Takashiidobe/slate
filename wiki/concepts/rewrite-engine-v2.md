# Rust rewrite engine

<!-- toc -->
- [Ownership](#ownership)
- [Scheduling and facts](#scheduling-and-facts)
- [Control-flow rewrites](#control-flow-rewrites)
- [Validation](#validation)
- [Design history](#design-history)
<!-- /toc -->

## Ownership

| Component under `crates/slate/src/backend` | Responsibility |
| --- | --- |
| `rust_ast`, `codegen` | Rust representation and emission |
| `interproc` | Call-graph, pointer, length and string analyses |
| `engine` | Arena, rule dispatch, caches and worklist scheduling |
| `engine/rules` | Registered local rewrites |

The backend consumes the Rust AST and the IR module's target facts. C typing,
conversions and layout remain slate-parser responsibilities.

## Scheduling and facts

- Run whole-program analyses before local worklist rewrites.
- Dispatch registered rules by node kind and call anchors.
- Reschedule affected neighbors and parents after an edit; bound oscillation with the edit budget.
- Maintain facts against the edited arena; cached facts must not outlive the changes that invalidate them.
- Recover safer representations only from conservative evidence; uncertain aliases or escapes retain raw pointers.
- The rule registry and implementation are authoritative for current ordering and coverage.
- [Pointer capability lattice](pointer-capability-lattice.md) defines the interprocedural representation choices.

## Control-flow rewrites

| CFG shape | Rust-to-Rust rewrite |
| --- | --- |
| Straight-line chain / forwarding states | Coalesce basic blocks / thread jumps |
| Acyclic region | `if` / `match` and labeled join blocks |
| Natural loop | Labeled `loop`; existing loop/while/for peepholes refine it |
| Multi-entry cyclic region | Existing SCC-local dispatch; explicit-tail-call helpers are an optimization candidate |

- Retained rules: `engine/rules/structure_goto.rs` and
  `structure_goto/reducible.rs` provide normalization, dominators, natural loops,
  and irreducible-SCC handling. `structure_dispatch.rs` handles switch recovery.
- Reuse the retained algorithms and registry; the removed frontend was CIR-specific,
  but these passes consume Rust AST. See [pass porting](pass-porting-workflow.md).
- Project translation runs the control-flow registry after raw basic-block coalescing.
  It preserves exported C signatures and skips interprocedural representation rewrites.
  `translate-project --raw` retains the baseline; `translate-lowered` stays raw.
- Goto recognition accepts legacy and `__slate_dispatch` / `__slate_state` names,
  integer / `usize` state values, and transfers inside conditional arms.
  Nested decisions stay in their original blocks; dispatcher exits remain labeled block exits.
- Keep loop-transfer labels when crossing labeled join blocks.
- Switch recovery accepts the current two-statement case-selector / dispatch scope.
- [SQLite runtime case study](sqlite-runtime-performance.md) measures the effects of these generic passes.
- Coalesce before structuring. Choose structured output for reducible regions;
  consider tail calls at the irreducible-SCC decision point before fallback localization.

### Explicit-tail-call candidate

- Example: `entry -> A or B`, with `A -> A or B` and `B -> A or B`.
  Extract each basic block into an internal helper; successor edges become
  `become a(...)` / `become b(...)`, removing the shared state dispatcher.
- Use guaranteed tail transfers through experimental
  [`explicit_tail_calls`](https://doc.rust-lang.org/unstable-book/language-features/explicit-tail-calls.html),
  rather than assuming an ordinary call will be optimized into a jump.
- Pass required live scalar values with compatible helper signatures/ABIs;
  passing a large state aggregate can retain the original copy overhead.
- Keep address-escaping C locals in stable storage owned by the outer wrapper.
  Helpers must not tail-transfer pointers into their disappearing local frames.
- Preserve the original exported function and C ABI, evaluation order, skipped
  initializers, uninitialized storage, and all region exits.
- Helper extraction affects function items and call-graph facts; integrate it at
  that boundary and invalidate/recompute affected analyses.
- Compare against structured output and SCC-local dispatch: executed C/Rust parity,
  tail-jump assembly, bounded stack use, live-state copies, code size, and runtime.
  Keep dispatch when eligibility or profitability is not established.
- Work: `slate-cgqg.3.1` coalescing, `slate-cgqg.3.3` retained-pass connection,
  `slate-cgqg.3.4` tail-call evaluation; retained structuring history: `slate-04q.85`.

## Validation

```bash
cargo clippy -p slate --allow-dirty --fix
cargo fmt
cargo nextest r --release --profile slate
```

Run from the workspace root. The Slate profile covers raw lowering, optimized
project control-flow fixtures, and multi-unit project linkage. `translate` runs
the full backend; project translation runs its control-flow subset. FileCheck
shape checks are suspended.

## Design history

- [Historical index](../historical/index.md): prior worklist, query and Salsa designs, pass catalogs and measurements.
- [Pass-porting workflow](pass-porting-workflow.md): current workflow for adapting retained algorithms.
- Earlier corpus timings used a different frontend and are not current performance claims.
