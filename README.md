# Slate - an idiomatic C23 to Rust Transpiler

Slate is an idiomatic C to Rust transpiler inspired by
[C2Rust](https://c2rust.com/).

Slate aims to follow the path set by other C to Rust transpilers like
C2Rust. First, by lowering to unidiomatic Rust, and then run a set of passes
on the generated Rust in order to refine it to safer Rust with only
static analyses and rewrites.

Slate support for modern C (C23 support), cross-compilation,
and a simple user experience.

## Installation

Slate is still source only and not on crates.io, so for now:

```sh
git clone https://github.com/takashiidobe/slate
```

and build it, to build `slate`, `slate-parser`, and `slate-sysroots`.

```sh
cargo build --release
```

I plan to merge `slate-sysroots` into plain slate, possibly leaving
`slate-parser` as a separate crate if it's useful as a multi-dialect C
parser + IR generator.

## Usage

Slate uses its built-in slate-parser frontend. Use `translate-project` with one
or more compilation databases to generate an executable Cargo crate. Library
projects and multiple configurations of the same translation unit are not yet
supported.

```text
translate [compiler args...] <file.c>                         C -> Rust
translate-lowered [compiler args...] <file.c>                 raw Rust output
emit-slate-ir [compiler args...] <file.c>                     typed parser IR
translate-project --compile-commands <file>... <dir> <crate_dir>
```

For example, to translate `chibicc`:

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
- no support for naked asm.
- your C and Rust may link to different compiler runtimes, which can
  cause differences in precise mathematical operations that run in
  software (like for complex or large numbers)

### In Progress

- [ ] Passing the GCC Torture Test Suite
- [ ] Support for other architectures
  - Currently only x86_64, no testing done for x86_32, arm32, arm64
- [ ] Support for other compiler flavors, like gcc and msvc

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
