# For users

Slate translates C to Rust through its built-in slate-parser frontend. It aims
for modern C support and deterministic translation, with baseline correctness
checked by running both the C and generated Rust.

## Build and headers

From the workspace root:

```bash
cargo build --release -p slate
cargo run --release -p slate -- sysroot install x86_64-unknown-linux-gnu
```

Release binaries live in `target/test-cache/release/`. See [setup](setup.md)
for the Rust toolchain and test dependencies.

## Translate a file

```bash
target/test-cache/release/slate translate input.c
target/test-cache/release/slate translate -std=gnu17 input.c
```

`translate` runs the backend pipeline. Single-file output may need supporting crates or C runtime bridges.

## Translate a project

```bash
target/test-cache/release/slate translate-project \
  --compile-commands project/compile_commands.json project output-crate
```

The compilation database supplies per-unit targets, defines and include paths.
The project must have exactly one unit defining `main`, one configuration per
unit and unique module stems. Library projects are not yet supported.
Generated crates include required support dependencies and runtime bridges.

See [cross compilation](compilation.md) and [feature coverage](features.md)
for current limits.
