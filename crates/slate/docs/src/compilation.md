# Cross Compilation

Slate can translate a C project once and produce Rust that still runs
correctly on every target you ask for, even targets whose libc headers
aren't installed on the machine doing the translation — `libc-shim` ships
its own portable headers, so there's no dependency on the host's system
libc. Pass `--target` (repeatable) to `translate-project`:

```sh
slate translate-project --compile-commands compile_commands.json \
  --target x86_64-unknown-linux-gnu --target aarch64-apple-darwin \
  ./project ./project-rs
```

C code that branches on the target with `#ifdef`/`#if defined(...)`
(architecture, OS, libc, endianness, ...) gets translated once per target and
merged into a single crate, with each variant gated behind the matching Rust
`#[cfg(...)]` — `target_arch`, `target_os`, `target_endian`, and so on — so
the output crate cross-compiles from `cargo build --target <triple>` the same
way the C project would have from a cross toolchain. See
[translate directives](./translate-directives.md) for the mechanism, and
[Libc](./libc.md) for why `libc-shim` exists in the first place.
