# Project Translation

`slate translate-project` reads a C project's compilation database and writes a
Cargo crate. Use the compile commands from the application or library you want
to translate.

For a CMake application, configure it with Ninja and export its compile
commands. Run these commands from the parent of `project/`.

```sh
cmake -S project -B project/build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
slate translate-project \
  --compile-commands project/build/compile_commands.json project project-rs
cargo +nightly run --manifest-path project-rs/Cargo.toml
```

## Select the C build

The compilation database supplies the flags and working directory for each
translation unit. Slate uses those settings to resolve relative include paths,
select a [compiler flavor](../targets/compilers.md), and see the same macros and
declarations as the original compiler. Run the original configure or build steps
that produce any generated headers or sources before translating.

A database can contain commands for multiple build targets, such as a static
library, a shared library, and its tests. Pass `--build-target NAME` to select
the one you want. When the same source has different compile commands and you
leave the option unset, Slate selects a target and prints its choice. For
example, CMake object paths under `CMakeFiles/zlibstatic.dir/` identify
`zlibstatic` as a build target.

## Build the generated crate

Slate translates each selected C file into a Rust module and resolves shared
symbols across the project. The output includes `Cargo.toml`, the generated
Rust, and [vendored support crates](../vendored-crates.md). Some translations
include a `build.rs` that compiles C shims with the target C toolchain.

For a hosted application with `main`, run the generated binary with Cargo. For a
library, choose `--crate-type staticlib` for a C-compatible static archive or
`--crate-type cdylib` for a shared library, then use `cargo +nightly build`.
When every translation unit is freestanding, Slate emits a `#![no_std]` crate.

The compilation database records compile commands. Keep any external libraries,
startup code, or linker scripts required by the original build available for the
generated crate or its final link. For cross-compilation, see
[Target Support](../targets/index.md). To build one crate for several platforms
or preprocessor configurations, use [Merge](merge.md).
