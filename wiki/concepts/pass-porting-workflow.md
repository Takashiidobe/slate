# Porting a backend pass

<!-- toc -->
- [Choose and implement](#choose-and-implement)
- [Verify](#verify)
- [Measure](#measure)
<!-- /toc -->

## Choose and implement

- Use `crates/slate/src/backend/engine/rules/mod.rs` and current failures to choose work; track it with `bd`.
- Read the nearest registered sibling rule before adapting an algorithm.
- Match current Rust AST nodes and preserve conservative preconditions, edit shape and fact invalidation.
- Register the rule with an order consistent with its dependencies.
- [Rewrite engine](rewrite-engine-v2.md) defines the current boundary; the [historical index](../historical/index.md) preserves earlier implementations and rationale.

## Verify

- Start changed behavior with a failing C differential fixture.
- Exercise `translate` for backend work. The current Slate profile runs raw lowering and cannot alone establish rewrite parity.
- Run `cargo clippy -p slate-c2rust --allow-dirty --fix`, `cargo fmt` and `cargo nextest r --release --profile slate` from the workspace root.
- FileCheck is suspended. Judge semantic correctness by executed C/Rust parity.

## Measure

- Compare binaries with and without the rule on the same input and flags.
- Use isolated worktrees when both implementations need to remain available.
- Project benchmarks must use a supported executable, unique module stems and one configuration per translation unit.
- Historical timings and pass cost tables are context, not a baseline for the current frontend.
