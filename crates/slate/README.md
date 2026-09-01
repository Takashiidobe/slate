# Slate - an idiomatic C23 to Rust Transpiler

Slate is an idiomatic C to Rust transpiler inspired by
[C2Rust](https://c2rust.com/).

C2Rust has three phases to translate code from C to Rust:

- Transpile
  - Translation of C to (mainly) unsafe Rust
- Refactor
  - Translating unsafe rust programs to safe rust programs (where
    possible)
- Postprocess
  - an LLM harness to aid in refactoring code that isn't statically
    known as safe into safe code

Slate aims to do the first two, but with a different architecture under
the hood and support for modern C (C23 support), cross-compilation,
and a simpler user experience.

## Installation

Clone this repo:

```sh
git clone https://github.com/takashiidobe/slate
```

You'll need a fork of LLVM with two patches I added:

```
git clone -b takashiidobe/slate-cir-loc-patches https://github.com/Takashiidobe/llvm-project
```

For example, with a checkout of llvm-project in `$HOME`:

```sh
$ cmake -G Ninja -S ~/llvm-project/llvm -B ~/llvm-project/build-cir
  -DLLVM_ENABLE_PROJECTS="clang;mlir" -DCLANG_ENABLE_CIR=ON
  -DLLVM_CCACHE_BUILD=ON -DCMAKE_BUILD_TYPE=Release
  -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
  -DLLVM_ENABLE_LLD=OFF -DLLVM_ENABLE_LTO=OFF -DLLVM_ENABLE_ASSERTIONS=OFF
  -DCMAKE_INSTALL_PREFIX=/usr/local
```

And a build of the MacroDumpPlugin in `./tools/macro-dump-plugin/MacroDump.cpp`.

Building requires running the build script:

```sh
$ tools/macro-dump-plugin/build.sh
```

## Usage

The main flag to use is `translate-project` which can either be
followed by --lib for a library or plain for an application with a main
function.

```
translate   [clang args...] <file.c>  C -> Rust");
translate-project --compile-commands <file>... <dir> <crate_dir>  cross-TU C dir -> Cargo binary crate, driven by a compile commands database"
translate-project --lib [--source-manifest <file>|--compile-commands <file>...] <project_dir> <crate_dir>  cross-TU C library -> Cargo crate"
```

For example, to translate a library, say `libexpat`:

```sh
slate translate-project --lib \
--compile-commands ~/c-corpus/libexpat/expat/build/compile_commands.json \
~/libexpat ./libexpat-rs
```

Or to translate a binary (in this case, `chibicc`):

```sh
slate translate-project --compile-commands ~/chibicc/compile_commands.json \
~/chibicc/ ./chibicc-rs
```

Since we're passing in `compile_commands.json` and both of these
projects are `make` driven, use a tool like
[bear](https://github.com/rizsotto/bear) to generate one first. This can
be as simple as running:

```sh
bear -- make
```

Which will generate a compile_commands.json with the commands required
to produce it.

### Single-File Translation

For simple one file usage, there's also `translate`, which translates
one C file into Rust. Note that this won't work for all files (because
some files will require shims, like long doubles as f80s for x86_64
linux).

```sh
slate translate add.c # translates add.c, prints to stdout
```

### C Feature Support

- Features up to C23
  - long double (f64, f80, f128) (double-double is unsupported)
  - floating-point environment
  - Complex numbers
  - Atomics
  - Attribute support
  - `_BitInt(N)` to arbitrary size
  - `#embed`
  - C17 style attributes like `[[deprecated]]`
  - `alignof`, `alignas`, `thread_local`
  - `_Decimal32/64/128`
- Support for most gnuisms too, like
  - gnu style attributes like `__attribute__(...)`

### Caveats

- setjmp/longjmp: because Rust doesn't have
  [`#[ffi_returns_twice]`](https://github.com/rust-lang/rust/issues/58314)
  removed in (https://github.com/rust-lang/rust/pull/120502)
  anymore, slate will translate your code that uses setjmp/longjmp, but
  there's no way to guarantee llvm won't break your code by over
  optimizing.
- protected visibility. In Rust there's no way to set visibility as
  protected.
- no support for naked asm. CIR doesn't lower naked asm yet.
- your C and Rust may link to different compiler runtimes, which can
  cause differences in precise mathematical operations that run in
  software (like for complex or large numbers)

### In Progress

- [ ] Passing the GCC Torture Test Suite
  - ~50 tests left
- [ ] Re-enabling rewriting to generate better code
- [ ] Generating target specific headers for intrinsics
- [ ] Support for other architectures
  - Currently only x86_64, no testing done for x86_32, arm32, arm64,
    riscv, risc32
- [ ] Support for other targets
  - Only linux w/ musl or glibc is supported for now, need to add libc
    support for windows, mac, android, and the BSDs

## Acknowledgements

- [C2Rust](https://c2rust.com/) the original C to Rust transpiler
- [Clang IR](https://llvm.github.io/clangir/) for providing an easy
  interface to translate C to Rust
- [Musl](https://musl.libc.org/) the libc that most of the headers from
  libc-shim are based on
- [Glibc](https://ftp.gnu.org/gnu/glibc/) for extra headers that are
  supported on most linux-likes
- [Aligned](https://crates.io/crates/aligned) for the aligned type to
  handle pointer offsets for types that follow the Sys-V ABI
- [bit-int](https://crates.io/crates/bit-int) for the basics of
  how to implement the `_Bitint(N)` type from C
- [num-complex](https://crates.io/crates/num-complex) for complex types
  in rust
- [triplers](https://crates.io/crates/triplers) for rust target
  detection
- [chibicc](https://github.com/rui314/chibicc) for a readable c
  compiler and test-suite that I learned about compiler supported
  headers from
- [libc-test](https://wiki.musl-libc.org/libc-test) A set of tests that
  validate your libc implementation
- [GCC Torture Test
  Suite](https://gcc.gnu.org/onlinedocs/gccint/Torture-Tests.html) for a
  comprehensive list of extremely difficult C tests to translate to Rust
