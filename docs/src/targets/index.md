# Target Support

Slate translates C for x86-64, i686, AArch64, and 32-bit Arm. It supports Linux,
macOS, and Windows targets, with tier-2 support for Android and FreeBSD. Choose
the target triple for the platform where the generated code will run and the
compiler flavor that built the original C.

For example, translate a source file built with Clang for AArch64 Linux.

```sh
slate sysroot install aarch64-unknown-linux-gnu
slate sysroot install compiler-headers clang
slate translate --target=aarch64-unknown-linux-gnu --flavor=clang input.c > input.rs
```

For project translation, use the original compilation database so Slate can read
the compiler, flags, and include paths for each file. See [Usage](../users.md)
and [Translation Unit Configuration](../parsing/configuration.md).

- [Architectures](architectures.md) covers data layout, calling conventions, and
  instruction-set options.
- [Operating Systems](operating-systems.md) lists target triples, system
  headers, and test coverage.
- [Compilers](compilers.md) describes flavor selection and differences in flags
  and language extensions.
