# Cross-target toolchains

<!-- toc -->
- [Translation](#translation)
- [Building generated Rust](#building-generated-rust)
- [Coverage](#coverage)
<!-- /toc -->

## Translation

- slate-parser models target facts from its registry and reads target headers from slate-sysroots.
- [Target configuration](slate-target-queries.md) defines defaults and compiler argument precedence.
- Install headers for each target before translating:

```bash
cargo run --release -p slate -- sysroot install aarch64-unknown-linux-gnu
cargo run --release -p slate -- translate \
  --target=aarch64-unknown-linux-gnu input.c
```

- `translate --targets=<triple>,<triple> input.c` can merge supported whole-item configuration variants into Rust cfg items.
- `translate-project` consumes compile commands, but rejects multiple configurations of the same translation unit. It does not generate a merged multi-target project.

## Building generated Rust

- Install the matching Rust target, linker and target C libraries.
- Generated crates with C runtime bridges also need a C cross compiler configured for the `cc` build dependency.
- Run cross-built binaries on the target or with a suitable user-mode emulator.

## Coverage

- The current Slate nextest profile executes host-target differential suites.
- Architecture-specific runtime suites are tracked for restoration under `slate-p58o.6.11` through `slate-p58o.6.13`.
- Successful translation alone is not cross-target runtime validation.
