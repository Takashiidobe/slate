# Landing a change

- C semantics and IR construction belong in slate-parser.
- Rust lowering belongs in `crates/slate/src/frontend/lowerer/`.
- Rust analyses and rewrites belong in `crates/slate/src/backend/`.
- Every feature starts with a failing C differential fixture.

## Lowering workflow

1. Place or find the fixture in an unsupported bucket and inspect its first barrier.
2. Fix the owning layer; use `emit-slate-ir`, `lowering-barriers` and `translate` to inspect the result.
3. Promote fixtures that now pass differential execution into the supported bucket.
4. Run `cargo clippy -p slate-c2rust --allow-dirty --fix`, `cargo fmt`, and `cargo nextest r --release --profile slate` from the workspace root. Run the parser gates too if its Rust changed.
5. Update the bead, log the change with `llog new`, and commit.

FileCheck is suspended. For rewrite work, use fixtures that execute the backend
path; the raw lowering suite alone cannot verify a rewrite.
