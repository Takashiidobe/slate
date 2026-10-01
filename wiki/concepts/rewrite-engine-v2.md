# Rust rewrite engine

<!-- toc -->
- [Ownership](#ownership)
- [Scheduling and facts](#scheduling-and-facts)
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

## Validation

```bash
cargo clippy -p slate --allow-dirty --fix
cargo fmt
cargo nextest r --release --profile slate
```

Run from the workspace root. The Slate profile exercises raw lowering;
`translate` invokes backend rewriting, while `translate-lowered` and project
translation emit raw lowered programs. Rewrite changes need differential
fixtures that execute the backend path. FileCheck shape checks are suspended.

## Design history

- [Historical index](../historical/index.md): prior worklist, query and Salsa designs, pass catalogs and measurements.
- [Pass-porting workflow](pass-porting-workflow.md): current workflow for adapting retained algorithms.
- Earlier corpus timings used a different frontend and are not current performance claims.
