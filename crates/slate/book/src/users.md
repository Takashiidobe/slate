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

```

```

And a build of the MacroDumpPlugin in `./tools/macro-dump-plugin/MacroDump.cpp`.

Building requires running the build script:

```
$ ../../tools/macro-dump-plugin/build.sh
```

## Usage

The main flag we want to use is `translate-project` which can either be
a lib or

```
emit-cir    print ClangIR (generic form)");
translate   [clang args...] <file.c>  C -> Rust");
translate-project [--target <triple>]... <dir> <crate_dir>  cross-TU C dir -> Cargo binary crate"
translate-project --compile-commands <file>... <dir> <crate_dir>  cross-TU C dir -> Cargo binary crate, driven by a compile commands database"
translate-project --lib [--source-manifest <file>|--compile-commands <file>...] <project_dir> <crate_dir>  cross-TU C library -> Cargo crate"
```
