# For Users

Slate is an idiomatic C to Rust Transpiler inspired by
[C2Rust](https://c2rust.com/) and
[TRACTOR](https://www.darpa.mil/research/programs/translating-all-c-to-rust).

How Slate aims to differentiate itself is through breadth and usability.

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
the hood and with support for modern C (C23 support), cross-compilation,
and with a simpler user experience.

## Installation

Clone the repo:

```sh
git clone https://github.com/takashiidobe/slate
```

You'll need an up to date build of clang with Clang IR enabled.

For example, with a checkout of llvm-project in `$HOME`:

```sh
cmake -G Ninja -S ~/llvm-project/llvm -B ~/llvm-project/build-cir
  -DLLVM_ENABLE_PROJECTS="clang;mlir" -DCLANG_ENABLE_CIR=ON
  -DLLVM_CCACHE_BUILD=ON -DCMAKE_BUILD_TYPE=Release
  -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
  -DLLVM_ENABLE_LLD=OFF -DLLVM_ENABLE_LTO=OFF -DLLVM_ENABLE_ASSERTIONS=OFF
  -DCMAKE_INSTALL_PREFIX=/usr/local
```

And a build of the MacroDumpPlugin in `./tools/macro-dump-plugin/MacroDump.cpp`.

Building requires running the build script:

```sh
$ ../../tools/macro-dump-plugin/build.sh
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
one C file into Rust.

```sh
slate translate add.c # translates add.c, prints to stdout
```
