# Landing a Change

Every feature and fixup starts with a C fixture. There are no unit tests
for Slate, so if you want to fix a regression, it should be a C file to
start.

## Where changes go

```
C -> CIR -> parse -> lower -> Rust source -> Fixup Rust
```

- `src/frontend/`, `src/cir/` parsing CIR/the Clang AST and lowering
  to baseline Rust. Baseline is unsafe, and unidiomatic to start.
  Test changes with the `lowering` profile.
- `src/backend/` fixup/idiomatization passes that run after baseline
  lowering to recover idiom (safe references, `Vec`/`Box`, `for x in ..`,
  compound assignment, ...) without changing behavior. Each pass is a
  `QueryRule` in `src/backend/query/rules/` that selects candidates, checks
  preconditions against read-only analysis in `src/backend/facts/`, and
  returns an edit set; `src/backend/mod.rs` runs a fixed, hand-written
  sequence of these passes. See [Rewriting](./writing-a-rewrite.md).
  Covered by the `rewrites` profile.
- `libc-shim/` the freestanding libc headers and implementations
  Slate compiles C against (`-nostdlib`). Covered by the `libc` profile.
- `vendor/` crates Slate ships fixed/adapted versions of
  (`bitint`, `num-complex`, `aligned`, `triplers`); see
  [Vendored Crates](./vendored-crates.md). These have their own unit
  tests, but Slate should also have e2e tests using them.

See [Setup](./setup.md) for how to build the CIR-enabled Clang and macro
dump plugin.

## Landing a lowering change

1. Write or extend a fixture in `tests/fixtures/*.c` that reproduces the gap
   a construct that fails to lower, or lowers to Rust that diverges from
   the C at runtime.
2. Isolate it while iterating, instead of running all the tests
   ```sh
   SLATE_DIFF_FIXTURE=<name> cargo nextest r --release --profile lowering \
     --nocapture
   ```
3. Use `cargo run -- translate-lowered <file.c>` to see baseline output
   before any fixups run, so you can tell whether a failure belongs in
   lowering or in a fixup pass.
4. Implement the change in `src/frontend/`.
   Every op inside a function body goes through
   `FunctionLowerer::lower_op` (`lowerer.rs`), which matches on
   a `self.lower_xxx(op)` handler per op.
   New ops get a new `Op::X(v) => self.lower_x(&v)` arm
   plus a `lower_x` implementation in the matching file.
5. Run the full `lowering` profile it also covers the chibicc,
   gcc-torture, c-testsuite, and libc-test suite, since a change can
   regress other fixtures.
6. `cargo fmt` and `cargo clippy` before finishing.

## Landing a libc-shim change

1. Add or extend a probe under `tests/stdlib/<header>/*.c` (one probe per
   libc function) or a fixture that exercises the header end to end.
2. Add the implementation/declaration to the relevant `libc-shim/include/`
   header. Gate anything that varies by target behind the `__SLATE_*`
   macros from `<features.h>` (see [Vendored Crates](./vendored-crates.md)
   for the full macro list).
3. Run the `libc` profile for header-only compilation, API
   compilation, and functional behavior across the targets Slate supports.
4. `cargo fmt` and `cargo clippy` before finishing.
