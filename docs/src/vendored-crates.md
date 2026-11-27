# Vendored Crates

`translate-project` writes local Cargo dependencies for C representations,
platform bindings, and runtime code. The generated manifest includes a base set
of support crates even for projects that use only plain scalar types. Some
crates appear only in the manifests of the crates that use them.

## C representations

| Crate            | What uses it                                                                                                                                           |
| ---------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `aligned`        | Provides a wrapper for explicitly aligned storage. Slate can also emit an alignment wrapper directly, for example `__SlateAlign64` for `_Alignas(64)`. |
| `bitint`         | `BInt` and `BUint` provide signed and unsigned `_BitInt` storage and operations where lowering uses a bit-precise type.                                |
| `num-complex`    | `Complex<T>` represents the real and imaginary parts of C complex values.                                                                              |
| `num-traits`     | Numeric traits used by `num-complex`.                                                                                                                  |
| `bitfields`      | The `bitfield` attribute provides field accessors over the backing storage Slate chooses for C bit-fields.                                             |
| `bitfields-impl` | The procedural macro implementation used by `bitfields`.                                                                                               |

## Platform bindings and runtime

| Crate           | What uses it                                                                                                                                      |
| --------------- | ------------------------------------------------------------------------------------------------------------------------------------------------- |
| `slate-support` | Generated Rust uses it for recognized C declarations and helpers, such as `slate_support::printf` and the `FILE` type.                            |
| `libc`          | Platform C runtime bindings used by the hosted parts of `slate-support`.                                                                          |
| `mach2`         | Apple Mach bindings exposed through `slate_support::mach`.                                                                                        |
| `slate-blocks`  | Supplies the compiler-rt Blocks runtime when the output uses translated Clang Blocks on a non-Apple target. Apple targets use the system runtime. |

`slate-support` has a `std` feature for helpers that need allocation or OS
services and for its `libc` dependency. A generated project whose units all use
the core-only runtime leaves the feature off. Helpers that use only `core`
remain available.

## How the files reach a generated project

The translator packages these sources under `crates/slate/vendor/`. Their
manifests are named `Cargo.toml.template` so Cargo includes these directories in
the translator's published archive. `translate-project` writes the templates as
`Cargo.toml` in the generated dependency directories.

Some features also need a C shim compiled with the target C toolchain. For
example, the [Long Double](long-double.md) bridge is generated as C source and
built by the generated project's `build.rs`.
