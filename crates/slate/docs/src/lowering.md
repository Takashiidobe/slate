# Lowering

`frontend::lowerer` translates slate-parser's typed module into the Rust AST.
Type conversions, source bindings, places and evaluation order are already
explicit in the IR.

```bash
cargo run --release -p slate -- lowering-barriers input.c
cargo run --release -p slate -- translate-lowered input.c
```

The lowerer collects unsupported constructs as barriers. Invalid IR aborts
with an invariant diagnostic. Strict translation reports the first barrier;
`lowering-barriers` lists all collected barriers without generating Rust.

Raw Rust may use `unsafe`, `libc`, raw pointers and explicit temporaries. The
supported differential buckets are the coverage contract.
