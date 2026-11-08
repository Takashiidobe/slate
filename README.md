# <img src="assets/pumpkin.png" width="40" alt="Slate icon"> Slate - an idiomatic C23 to Rust Transpiler

Slate is an idiomatic C to Rust transpiler inspired by
[C2Rust](https://c2rust.com/).

Slate follows the path set by other C to Rust transpilers like C2Rust; First, by
lowering to Rust, and then running a set of "rewrite" passes on the generated
Rust in order to produce better Rust with only information known statically.
This generates better rust than machine translation.

Slate supports all versions of C, from C89 to C2y, as well as non-standard
features like inline asm, long double, bitsets, nested functions, and many more.

## Installation

Install [`slate-c2rust`](https://crates.io/crates/slate-c2rust) from crates.io
with Rust nightly:

```sh
cargo +nightly install slate-c2rust --locked
```

The package is named `slate-c2rust`; the executable is named `slate`. To install
the standalone C parser and typed IR CLI:

```sh
cargo +nightly install slate-parser --locked
```

To avoid compilation from source, use
[cargo-binstall](https://github.com/cargo-bins/cargo-binstall) to install
binaries directly. Prebuilt binaries are available for x86_64 Linux, x86_64 and
Aarch64 mac, and x86_64 Windows:

```sh
cargo binstall slate-c2rust slate-parser
```

This installs both `slate` and `slate-parser`; omit either package to install
just one. The release workflow publishes these with github actions and puts them
in [GitHub Releases](https://github.com/takashiidobe/slate/releases).

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

When you translate for the machine you are on, Slate asks the native `clang` for
its system include directories, so no setup is needed beyond having `clang` on
`PATH`. Check what it finds with `slate sysroot doctor native`.

To translate for another target, or to pin headers for reproducible output,
install a sysroot. Setting `SLATE_SYSROOTS` makes Slate use installed sysroots
only, even for the host. Some common ones include:

Slate supports Android and Freebsd targets as well.

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

### Single-File Translation

For simple one file usage, there's also `translate`, which translates one C file
into Rust. Note that this won't work for all files (because some files will
require shims, like long doubles as f80s for x86_64 linux).

```sh
slate translate add.c # translates add.c, prints to stdout
```

You can also use `--flavor=msvc|clang|gcc` to have slate compile your code while
emulating another compiler. Without a flavor, slate emulates `clang`.

### Multi-target/feature translation

For now, slate has it's own "compile commands" esque format to handle
multi-target builds. I plan to add a cli option to allow merging multiple
compile_commands.json files for multiple targets into one json file to feed to
slate to build a project that is multitarget.

```sh
cmake -S ~/zlib -B ~/zlib/build-clang # build the sources

slate translate-project \
  --matrix tools/corpus/matrices/zlib.json \
  --merge \
  --matrix-target linux-x86_64,linux-i686,linux-aarch64 \
  --crate-type staticlib \
  ~/zlib ./zlib-rs
```

`--merge` produces one crate with `#[cfg]`'d code for each target and feature.

The format allows mapping C options to Cargo features. For example, for zlib’s
FASTEST configuration is added to the Cargo.toml as a `fastest` feature, and any
code that is multi-target is `#[cfg]`'d away. Keep in mind that each feature
adds an exponential amount of compiles (2^n, where n is the amount of
features/targets in total) so be careful if your build adds too many features.

For zlib with 9 features, on a single target my machine can compile to rust in
3s, but for all 16 targets that slate supports, it's closer to 30s.

Slate supports folding of `cfg` branches and simplification of `cfg` conditions,
to keep `cfgs` more readable.

### Features

C23 (+ C2y) support is a goal. Slate also supports non-standard features like
inline asm in all three compiler dialects, attributes like `__attribute__` or
MSVC's `__declspec`, nested functions, binary constants, vector extensions, and
many more. Basically, if any of the three major compilers (GCC, Clang, MSVC) can
compile your C, and Slate can't, it's a bug.

See [feature coverage](crates/slate/docs/src/features.md) and
[testing](wiki/concepts/differential-fixtures.md) for how we maintain regression
testing.

Compared to other tools like c2rust, slate also preserves and attaches comments
to nodes. This allows comments to travel along with the conversion, and be
attached to the generated rust, so you don't have to recomment your code after
translating it.

As well, because slate has its own parser stack, it has macro provenance -- so
it can find out where definitions come from and replace them with higher level
rust constructs. This allows us to replace macro constants from libc with
similar rust.

## Trophy Case

### Clang/GCC

- [SQLite](https://sqlite.org)
  - [Rust Translation](https://github.com/takashiidobe/slate-sqlite). Passes
    `select1.test` and `main.test` using `testfixture`, plus `mptest` and
    `threadtest3`. Benchmarks for the translated run ~5% faster to 8% slower,
    depending on run/benchmark for the generated rust.
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
- [curl](https://curl.se)
  - Translates libcurl and the `curl` tool. Passes all 1690 `runtests.pl` tests,
    and benchmarks within 5% of native over file:// and HTTP transfers.
- [jq](https://jqlang.org)

### MSVC

- [] [MinHook](https://github.com/TsudaKageyu/minhook)
- [x] [PuTTY](https://www.chiark.greenend.org.uk/~sgtatham/putty/)
  - Translates and links `plink`, `pscp` and `psftp` for
    `x86_64-pc-windows-msvc`. The translated `testcrypt` passes all 90
    `cryptsuite.py` tests, including the AES-NI, SHA-NI and CLMUL
    implementations.
- [] [ReactOS](https://github.com/reactos/reactos)

### In Progress

- [ ] Passing the GCC Torture Test Suite (Currently ~1400 out of 1697). On every
      commit, each test is differential tested with a binary compiled by
      clang/gcc for exit code/stdout/stderr equality.
- [ ] Cleaning up multi-target translated zlib. Lots of work to do to make cross
      target code more readable.

## Acknowledgements

- [C2Rust](https://c2rust.com/) the original C to Rust transpiler
- [Musl](https://musl.libc.org/) for target libc headers
- [Glibc](https://ftp.gnu.org/gnu/glibc/) for headers that are supported on most
  unix-likes
- [Aligned](https://crates.io/crates/aligned) for the aligned type to handle
  pointer offsets for types that follow the Sys-V ABI
- [bit-int](https://crates.io/crates/bit-int) for the basics of how to implement
  the `_Bitint(N)` type from C
- [num-complex](https://crates.io/crates/num-complex) for complex types in rust
- [chibicc](https://github.com/rui314/chibicc) for a readable c compiler and
  test-suite that I learned about compiler supported headers from
- [GCC Torture Test Suite](https://gcc.gnu.org/onlinedocs/gccint/Torture-Tests.html)
  for a comprehensive list of extremely difficult C tests to translate to Rust
