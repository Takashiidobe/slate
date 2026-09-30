# Cross Compilation

Slate can translate a C project once and produce Rust that still runs
correctly on every target you ask for, even targets whose libc headers
aren't installed on the machine doing the translation. Target headers
come from slate-sysroots, so there's no dependency on the host's system
libc. Supply one compilation database per target to `translate-project`:

```sh
slate translate-project \
  --compile-commands x86_64/compile_commands.json \
  --compile-commands aarch64/compile_commands.json \
  ./project ./project-rs
```

C code that branches on the target with `#ifdef`/`#if defined(...)`
(architecture, OS, libc, endianness, ...) gets translated once per target and
merged into a single crate, with each variant gated behind the matching Rust
`#[cfg(...)]` — `target_arch`, `target_os`, `target_endian`, so
the output crate cross-compiles from `cargo build --target <triple>` the same
way the C project would have from a cross toolchain. See
[translate directives](./translate-directives.md) for the mechanism.
