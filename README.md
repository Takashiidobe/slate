# Slate - an idiomatic C23 to Rust Transpiler

Slate is an idiomatic C to Rust transpiler inspired by
[C2Rust](https://c2rust.com/).

Slate aims to follow the path set by other C to Rust transpilers like
C2Rust. First, by lowering to unidiomatic Rust, and then run a set of passes
on the generated Rust in order to refine it to safer Rust with only
static analyses and rewrites.

Slate supports modern C (C23 support), cross-compilation, and a simple
user experience.

## Installation

Install [`slate-c2rust`](https://crates.io/crates/slate-c2rust) from crates.io
with Rust nightly:

```sh
cargo +nightly install slate-c2rust --locked
```

The package is named `slate-c2rust`; the executable is named `slate`.
To install the standalone C parser and typed IR CLI:

```sh
cargo +nightly install slate-parser --locked
```

To avoid compilation from source, use
[cargo-binstall](https://github.com/cargo-bins/cargo-binstall) to install without
compiling. Prebuilt binaries are built for x86_64 Linux, x86_64 and aarch64 mac,
and x86_64 Windows:

```sh
cargo binstall slate-c2rust slate-parser
```

This installs both `slate` and `slate-parser`; omit either package to install
just one. The release workflow publishes these archives on
[GitHub Releases](https://github.com/takashiidobe/slate/releases).
See [publishing](RELEASING.md) for release setup and tag conventions.

To build from source:

```sh
git clone https://github.com/takashiidobe/slate
cd slate
```

Build the workspace:

```sh
cargo +nightly build --release
```

## Setup

Before translating C code, install a sysroot for your target. This is required
to fetch the C standard library and platform headers that Slate uses to parse
your code. Run the command for your target:

| Target                        | Command                                          |
| ----------------------------- | ------------------------------------------------ |
| macOS aarch64 (Apple Silicon) | `slate sysroot install aarch64-apple-darwin`     |
| Linux x86_64 (glibc)          | `slate sysroot install x86_64-unknown-linux-gnu` |
| Windows x86_64 (MSVC)         | `slate sysroot install x86_64-pc-windows-msvc`   |

Linux and Windows sysroots download the headers. On macOS, Slate uses the local
Apple SDK instead; install it with `xcode-select --install` if needed, then run
the sysroot command above.

See [sysroot documentation](crates/slate-sysroots/README.md) for other targets
and SDK configuration.

## Usage

Slate uses its built-in slate-parser frontend. Use `translate-project` with one
or more compilation databases to generate a Cargo crate: an executable when one
unit defines `main`, otherwise a library (`src/lib.rs`) whose `--crate-type` is
any comma-separated mix of `rlib` (default), `staticlib`, and `cdylib`. Multiple
configurations of the same translation unit are not yet supported. Project
output always applies control-flow rewrites. Each command is parsed as the
compiler it names (`gcc` and `*-gcc` as gcc, `cl` as MSVC, anything else as
clang); `--flavor gcc|clang|msvc` overrides that for every command.

```text
translate [compiler args...] <file.c>                         C -> Rust
emit-slate-ir [compiler args...] <file.c>                     typed parser IR
translate-project [--crate-type <types>] [--flavor <flavor>] --compile-commands <file>... <dir> <crate_dir>
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

C23 support is a goal. Current end-to-end coverage is defined by the supported
C differential fixtures and corpus buckets; parser acceptance alone does not
establish Rust translation support. Remaining cases live in the corresponding
unsupported buckets and are tracked with beads.

See [feature coverage](crates/slate/docs/src/features.md) and
[testing](wiki/concepts/differential-fixtures.md) for the runtime contract.
Target ABI details, runtime bridges and nonlocal jumps require coverage for the
specific input and target.

## Trophy Case

Projects that we've tested e2e that translate to Rust:

- [SQLite](https://sqlite.org)
- [Redis](https://redis.io)
- [chibicc](https://github.com/rui314/chibicc)
- [cJSON](https://github.com/DaveGamble/cJSON)
- [libyaml](https://github.com/yaml/libyaml)
- [Lua](https://www.lua.org)
- [zlib](https://zlib.net)
- [utf8proc](https://github.com/JuliaStrings/utf8proc)
- [libexpat](https://github.com/libexpat/libexpat)
- [giflib](https://giflib.sourceforge.net)
- [LMDB](https://www.symas.com/mdb)
- [c-ares](https://c-ares.org)
- [libevent](https://libevent.org)
- [yyjson](https://github.com/ibireme/yyjson)
- [PCRE2](https://github.com/PCRE2Project/pcre2)

### In Progress

- [ ] Passing the GCC Torture Test Suite
- [ ] Support for other architectures
  - Currently only x86_64, no testing done for x86_32, arm32, arm64
- [ ] Support for other compiler flavors, like gcc and msvc

## Acknowledgements

- [C2Rust](https://c2rust.com/) the original C to Rust transpiler
- [Musl](https://musl.libc.org/) for target libc headers
- [Glibc](https://ftp.gnu.org/gnu/glibc/) for extra headers that are
  supported on most linux-likes
- [Aligned](https://crates.io/crates/aligned) for the aligned type to
  handle pointer offsets for types that follow the Sys-V ABI
- [bit-int](https://crates.io/crates/bit-int) for the basics of
  how to implement the `_Bitint(N)` type from C
- [num-complex](https://crates.io/crates/num-complex) for complex types
  in rust
- [chibicc](https://github.com/rui314/chibicc) for a readable c
  compiler and test-suite that I learned about compiler supported
  headers from
- [GCC Torture Test
  Suite](https://gcc.gnu.org/onlinedocs/gccint/Torture-Tests.html) for a
  comprehensive list of extremely difficult C tests to translate to Rust
