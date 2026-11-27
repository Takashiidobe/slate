# Compiler Flags

Use the flags from the original C build so Slate sees the same headers, active
code, types, and calling conventions. This reference lists common accepted flags
and the parts of their behavior Slate models.

```sh
slate translate --flavor=gcc -std=gnu17 -Iinclude -DFEATURE=1 input.c > input.rs
```

- [GCC Flags](gcc.md)
- [Clang Flags](clang.md)
- [MSVC Flags](msvc.md)

For single files, select the compiler with `--flavor`. Clang is the default.
[Project Translation](../../project/index.md) reads the compiler and flags from
each compilation database entry. Use a supported target triple from
[Target Support](../../targets/index.md).

Slate rejects recognized options used with the wrong flavor or unsupported
values. It warns and continues when it encounters an unrecognized compiler flag.
Some recognized driver options, such as output paths, are accepted and ignored.
Reproduce any build settings you still need when you build the generated crate.

See [Translation Unit Configuration](../../parsing/configuration.md) for how
options affect the pipeline and [Preprocessor Reference](../preprocessor.md) for
how headers and macros are resolved.

See [Calling Conventions](../calling-conventions.md) for per-function ABI
annotations and flags that change the default calling convention.
