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
cargo run --release -p slate-c2rust -- sysroot install aarch64-unknown-linux-gnu
cargo run --release -p slate-c2rust -- translate \
  --target=aarch64-unknown-linux-gnu input.c
```

- `translate --targets=<triple>,<triple> input.c` can merge supported whole-item configuration variants into Rust cfg items.
- `translate-project` consumes compile commands, but rejects multiple configurations of the same translation unit. It does not generate a merged multi-target project.

## Building generated Rust

- Install the matching Rust target, linker and target C libraries.
- Generated crates with C runtime bridges also need a C cross compiler configured for the `cc` build dependency.
- Run cross-built binaries on the target or with a suitable user-mode emulator.

## Coverage

- The `slate` nextest profile executes x86_64 Linux differential suites.
- `slate-i686`, `slate-aarch64` and `slate-targets` run `tests/fixtures` for other Linux targets, under `qemu-aarch64-static` for aarch64 ([targets](differential-fixtures.md#targets)). aarch64 needs `aarch64-linux-gnu-gcc` and its sysroot in `/usr/aarch64-linux-gnu`.
- Other environments and OSes are tracked in `slate-5vj0.8`; corpus suites remain host-only.
- Successful translation alone is not cross-target runtime validation.
