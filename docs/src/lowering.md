# Lowering

`frontend::lowerer` translates slate-parser's typed module into the Rust AST.
Type conversions, source bindings, places and evaluation order are already
explicit in the IR.

```bash
cargo run --release -p slate-c2rust -- lowering-barriers input.c
cargo run --release -p slate-c2rust -- translate input.c
```

The lowerer collects unsupported constructs as barriers. Invalid IR aborts with
an invariant diagnostic. Strict translation reports the first barrier;
`lowering-barriers` lists all collected barriers without generating Rust.

Raw Rust may use `unsafe`, `libc`, raw pointers and explicit temporaries. The
supported differential buckets are the coverage contract.

## Lowering from analysis facts

- [remove_mut](lowering/remove_mut.md) explains when bindings need `mut`.
- [Interprocedural Lifts](lowering/interprocedural.md) covers pointer borrows,
  slices, and C strings across function signatures and call sites.
