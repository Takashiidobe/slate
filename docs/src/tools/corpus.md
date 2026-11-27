# Sweep and Test the C Corpus

The corpus tools use real project build flags and headers. Start with a native
build and compilation database, then choose parser triage or an end-to-end
translation run. Both workflows use the project sources under `~/c-corpus` by
default. Pass `--corpus <directory>` to use another root.

## Build and sweep one project

```sh
cargo build --release -p slate-parser
python3 crates/slate-parser/tools/c_corpus_setup.py --flavor clang zlib
python3 crates/slate-parser/tools/c_corpus_sweep.py --flavor clang zlib
```

Setup uses the project's recipe to build the native sources and write
`build-clang/compile_commands.json`. The sweep reads the commands, runs the
selected compiler as an oracle, and asks `slate-parser ir` to handle each
translation unit. Match `--flavor` to the native build you prepared.

Start with `target/c-corpus-sweep.md` for failure clusters and slow units, then
use `target/c-corpus-sweep.json` for individual results. `oracle-rejects` points
to an input the native compiler did not accept. For a Slate failure, start with
the first detailed diagnostic and reproduce it with that unit's flags and
working directory.

## Translate, build, and compare

```sh
cargo build --release -p slate-c2rust
python3 tools/corpus/e2e.py zlib --mode clang --until barriers
python3 tools/corpus/e2e.py zlib --mode clang
```

The first run stops after checking lowering barriers. A full run picks the
translation units for the recipe, translates the project, builds it, and runs
the available runtime comparisons. Recipes can add stages, such as booting the
Linux kernel and running its selftests. Use `--setup` when the native build and
compilation database need to be rebuilt first.

For zlib, look in `target/corpus/zlib/clang/`. `summary.json` identifies the
first failing stage. Read `barriers.md` for unsupported constructs and the stage
log for translation, build, or test failures. A runtime comparison uses the same
inputs on both sides and checks exit status and comparable output.

Use `tools/corpus/validate_merge.py --crate <merged-crate> --work <directory>`
to validate a merged zlib crate across configurations. This is useful when an
individual target works and the merged output differs.

## Sweep compiler test suites

`crates/slate-parser/tools/corpus_sweep.py` runs existing parser fixtures
against Slate or a selected compiler. Pass `--fixtures <path>` to focus on a
subtree. `gcc_dg_sweep.py` and `llvm_lit_sweep.py` in the same directory compare
tests from compiler source checkouts and record JSON Lines results. For an
imported case already under `tests/fixtures/`, use the fixture harness in
[Testing](../testing.md).
