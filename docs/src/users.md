# Usage

These simple examples show how to translate a C file or build a Rust crate from
a C project. Try a single file in the [Playground](../) or install Slate to run
the examples locally.

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

## Install headers

Slate uses your native compiler's headers for host translation when available.
For cross-target translation, install the target and compiler headers. For
example, install x86-64 Linux headers for Clang.

```sh
slate sysroot install x86_64-unknown-linux-gnu
slate sysroot install compiler-headers clang
```

## Translate an application

Slate reads a compilation database, `compile_commands.json`, to get each C
file's compiler flags and include paths. For an application, use the compile
commands for its `main` and the files that link into the same executable. CMake
can generate the database with Ninja.

```sh
cmake -S project -B project/build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
slate translate-project \
  --compile-commands project/build/compile_commands.json project project-rs
cargo +nightly run --manifest-path project-rs/Cargo.toml
```

See [Project Translation](project/index.md) for build target selection and
what Slate writes to the generated crate.

## Translate a library

Use the compile commands for the library's source files. Select `staticlib` for
a static C-compatible library or `cdylib` for a shared library.

```sh
slate translate-project \
  --compile-commands library/compile_commands.json \
  --crate-type staticlib library library-rs
cargo +nightly build --manifest-path library-rs/Cargo.toml
```

## Translate zlib for multiple platforms

This matrix translates zlib for x86-64 Linux and Arm64 FreeBSD with `FASTEST`
enabled and disabled on each. Install the Linux and Clang headers above, then
run these commands from a zlib checkout.

```sh
slate sysroot install aarch64-unknown-freebsd
cmake -S . -B build-clang -G Ninja -DCMAKE_C_COMPILER=clang \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
slate project target add zlib.json linux-x86_64 build-clang/compile_commands.json \
  --triple x86_64-unknown-linux-gnu --build-target zlibstatic
slate project target add zlib.json freebsd-aarch64 build-clang/compile_commands.json \
  --triple aarch64-unknown-freebsd --build-target zlibstatic
slate project feature add zlib.json FASTEST
slate translate-project --manifest zlib.json --merge \
  --manifest-target linux-x86_64,freebsd-aarch64 --crate-type staticlib \
  . ../zlib-rs
```

The result is one crate with a `fastest` Cargo feature. Slate uses Rust `#[cfg]`
where the translations differ by target or feature. This example reuses the same
compile commands across both targets. If your build generates different sources
or configuration headers for each target, use that target's own compilation
database.

See [Merge](project/merge.md) for how Slate combines translations and how to
check the result for each configuration.

## Translate a freestanding project

Keep `-ffreestanding` in the compile commands for the project. When the
project's C files are all freestanding, Slate generates a `#![no_std]` crate.
For a CMake project that builds a freestanding library, you can generate the
database and translate it like this.

```sh
cmake -S kernel -B kernel/build -G Ninja -DCMAKE_C_COMPILER=clang \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_FLAGS=-ffreestanding
slate translate-project \
  --compile-commands kernel/build/compile_commands.json \
  --crate-type staticlib kernel kernel-rs
cargo +nightly build --manifest-path kernel-rs/Cargo.toml
```

For a kernel or firmware image, link the static library with your project's
startup code and linker script using the original target's build settings.
