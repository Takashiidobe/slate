# Translate C to Rust

Slate translates a C source file to Rust or a C project to a Cargo crate. Try a
single file in the [Playground](../) or install the CLI.

## Install and translate a file

```sh
cargo +nightly install slate-c2rust --locked
printf 'int add(int a, int b) { return a + b; }\n' > add.c
slate translate add.c > add.rs
```

Use `cargo binstall slate-c2rust` if a prebuilt binary is available for your
platform. The package is named `slate-c2rust` and the command is `slate`.

Pass compiler flags before the input file. Slate defaults to Clang mode. Use
`--flavor=gcc` or `--flavor=msvc` for source built with those compilers.

```sh
slate translate --flavor=gcc -std=gnu17 -Iinclude -DNDEBUG add.c > add.rs
```

Single-file output can use [support crates](vendored-crates.md) or require C
runtime code. Use project translation to generate the Cargo manifest and the
support files the project needs.

## Translate an executable

Use your build's `compile_commands.json` so Slate gets the compiler flags,
defines, and include paths for each source file. For a CMake project, configure
with a generator that exports the database, such as Ninja.

```sh
cmake -S project -B project/build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
slate translate-project \
  --compile-commands project/build/compile_commands.json project project-rs
cargo +nightly build --manifest-path project-rs/Cargo.toml
```

The database for an executable should include its `main` and the source files
that link into that executable.

## Translate a library

Use a compilation database for the library's source files and select the Cargo
crate type. Use `staticlib` for a static C-compatible library or `cdylib` for a
shared one.

```sh
slate translate-project \
  --compile-commands library/compile_commands.json \
  --crate-type staticlib library library-rs
cargo +nightly build --manifest-path library-rs/Cargo.toml
```

See [cross compilation](compilation.md) to translate for another platform or
combine several target configurations into one crate.

## Diagnose a translation failure

If Slate cannot find system headers for your host, run
`slate sysroot doctor native` to see which compiler and include directories it
found. For another target, install its headers as described in
[cross compilation](compilation.md).

If Slate reports unsupported C, use `slate lowering-barriers` with the same
compiler flags and input to locate the first lowering barrier. See
[feature coverage](features.md) for examples and known gaps.
