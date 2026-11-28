# Usage

Slate translates C files into Rust source and C projects into Cargo crates. Try
a single file in the [Playground](../) or install Slate to run it locally.

## Install Slate

```sh
cargo +nightly install slate-c2rust --locked
```

Use `cargo binstall slate-c2rust` if a prebuilt binary is available for your
platform. The package is named `slate-c2rust` and the command is `slate`.

## Translate a file

Create a small C file and write the translation to `add.rs`.

```sh
printf 'int add(int a, int b) { return a + b; }\n' > add.c
slate translate add.c > add.rs
```

Put compiler arguments such as `-std=gnu17` and `-Iinclude` before the input
file. Slate defaults to Clang mode. Use `--flavor=gcc` or `--flavor=msvc` to
match those compilers.

## Translate a project

Start with the [zlib tutorial](tutorials/zlib.md) to generate a compilation
database, translate a library, and link a C program to the resulting archive.
For other project types, see [Project Translation](project/index.md).

- [Translate zlib for two platforms](tutorials/cross-platform.md) to install
  target headers and build a merged Cargo crate.
- [Translate a freestanding library](tutorials/freestanding.md) to build a
  `no_std` archive.
- [Debug a translation](tutorials/debugging.md) to diagnose header, translation,
  or build errors.

See [Command Line](reference/cli.md) for command options and
[Sysroots and Headers](reference/sysroots.md) for header setup.
