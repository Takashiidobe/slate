# Setup

## Prerequisite: a CIR-enabled Clang

Slate lowers ClangIR, not LLVM IR, so it needs a Clang built with
`CLANG_ENABLE_CIR=ON`. No distro or upstream binary build ships this, so you
have to build it yourself:

```bash
git clone https://github.com/llvm/llvm-project
cmake -S llvm-project/llvm -B llvm-project/build-cir -G Ninja \
  -DLLVM_ENABLE_PROJECTS=clang -DCLANG_ENABLE_CIR=ON \
  -DCMAKE_BUILD_TYPE=Release
ninja -C llvm-project/build-cir clang
```

Slate defaults to `~/llvm-project/build-cir/bin/clang`. If yours lives
elsewhere, every tool that needs it reads these environment variables
instead (all optional, all overriding a local-build default):

| Var                                 | Default                                      | Role                                                                                              |
| ----------------------------------- | -------------------------------------------- | ------------------------------------------------------------------------------------------------- |
| `SLATE_CLANG`                       | `~/llvm-project/build-cir/bin/clang`         | emit CIR + Clang AST JSON                                                                         |
| `SLATE_CIR_OPT`                     | `~/llvm-project/build-cir/bin/cir-opt`       | CIR -> MLIR generic form                                                                          |
| `SLATE_CC`                          | `clang` (from `PATH`)                        | compile the C side of differential tests                                                          |
| `SLATE_CARGO`                       | `cargo`                                      | compile the generated Rust                                                                        |
| `SLATE_TARGET` / `SLATE_CLANG_ARGS` | N/A                                          | shared target triple / extra clang flags                                                          |
| `SLATE_MACRO_DUMP_PLUGIN`           | `<$SLATE_CLANG build>/lib/SlateMacroDump.so` | the macro dump plugin binary (see below)                                                          |
| `SLATE_LIBC_SHIM`                   | `libc-shim/include`                          | headers `SLATE_CLANG` parses with `-nostdlibinc -isystem <dir>` instead of the host's system libc |

## Build the macro dump plugin

Slate always loads a Clang plugin
([Macro Dump Plugin](./macro-dump-plugin.md)) when it parses C, and that
plugin links against the exact Clang tree `SLATE_CLANG` points at:

```bash
SLATE_CLANG=~/llvm-project/build-cir/bin/clang ./tools/macro-dump-plugin/build.sh
```

Rerun this every time you rebuild `SLATE_CLANG`, a stale plugin built
against a different llvm-project could crash or silently misbehave, since it links
against that build's internal headers rather than a stable ABI.

## Build Slate itself

```bash
cargo build --release
cargo run -- translate tests/fixtures/add.c   # prints Rust to stdout
```

## Running tests

**Always use a release nextest profile**, never plain `cargo test` — the
suites are slow enough uncompiled that iterating on them is impractical, and
some fixtures assume optimized codegen.

```bash
cargo nextest r --release --profile lowering # frontend/lowering runtime differential, no fixups
cargo nextest r --release --profile rewrites # backend/fixups and every non-libc test
cargo nextest r --release --profile libc     # libc shim, headers, API, and functional tests
```

- `lowering` for `src/frontend/`, `src/cir/`, and the lowerer
  against differential fixtures (`tests/fixtures/*.c`) plus the chibicc,
  gcc-torture, c-testsuite, and libc-test suite, run through only lowering
  Run for changes in `src/frontend/`, `src/cir/`, or the CIR/AST parsing layer.
- `libc` for `libc-shim/` headers and implementations: API
  compilation, functional behavior, and header-only compilation across
  targets. Run for changes in `libc-shim/`.
- `rewrites` for fixup/idiomatization passes
  (`src/backend/query/rules/`) Run for changes in `src/backend/`.

Only the profile matching what you changed needs to pass; run more than one
only when a change genuinely crosses those boundaries (e.g. a shared type
used by both the lowerer and a fixup pass).

To run just one fixture, run `SLATE_DIFF_FIXTURE`:

```bash
SLATE_DIFF_FIXTURE=<name> cargo nextest r --release --profile lowering
```

## Cleanup

Make sure to run `cargo fmt --all-targets` and `cargo clippy --all-targets` and fix any violations.
