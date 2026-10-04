# Cross compilation

Install the target headers, then pass compiler arguments before the input:

```bash
cargo run --release -p slate-c2rust -- sysroot install aarch64-unknown-linux-gnu
cargo run --release -p slate-c2rust -- translate \
  --target=aarch64-unknown-linux-gnu input.c
```

slate-parser supplies target layout and predefines. Translation uses target
headers from slate-sysroots. Building and running generated Rust also requires
the matching Rust target, linker, C libraries and, when runtime bridges are
used, a C cross compiler.

Single-file `translate --targets=<triple>,<triple> input.c` can merge supported
whole-item configuration branches into Rust cfg items. Project translation
rejects multiple configurations of one translation unit; it does not merge
several target builds of a project into one crate.

The current differential profile executes host-target tests. Successful
cross-target translation alone is not runtime validation.
