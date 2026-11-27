# Lowering

Lowering translates IR into Rust. It preserves control flow, evaluation
order, and the target's data layout and calling conventions where supported. The
IR already records C type conversions and evaluation order. Lowering uses that
information to choose Rust types, expressions, and runtime calls.

```bash
cargo run --release -p slate-c2rust -- lowering-barriers input.c
cargo run --release -p slate-c2rust -- translate input.c
```

The lowerer collects unsupported constructs as barriers. Invalid IR aborts with
an invariant diagnostic. Strict translation reports the first barrier;
`lowering-barriers` lists all collected barriers without generating Rust.

## C features

- [Switch and Goto](goto.md) covers switch fallthrough, labeled exits, and
  dispatch loops for cycles with multiple entry points.
- [Dynamic Stack Allocation](lowering/alloca.md) covers `alloca` lifetimes,
  alignment, and allocation spacing.
- [setjmp and longjmp](setjmp.md) covers nonlocal control transfers through a
  saved C execution context.
- [Long Double](long-double.md) covers target-dependent precision and native ABI
  calls through C bridges.
- [Floating-point Environment](floating-point-environment.md) covers operations
  whose results depend on the active rounding mode.
- [Packed Records and Bit-fields](lowering/packed-records.md) covers unaligned
  fields and shared bit-field storage.
- [Variadic Calls and `va_arg`](lowering/variadics.md) covers default promotions
  and reconstruction of arguments passed in multiple registers.
- [Clang Blocks](lowering/clang-blocks.md) covers captures, shared mutation, and
  copied blocks that outlive the declaring scope.

See [Vendored Crates](vendored-crates.md) for the dependencies of generated
Cargo projects and [Rewrites](writing-a-rewrite.md) for the changes Slate makes
while choosing Rust representations and after building the Rust AST.
