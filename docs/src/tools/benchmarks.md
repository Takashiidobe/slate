# Measure Performance

Use the SQLite benchmark for generated program runtime and the interprocedural
benchmark for the cost of running Slate's translation passes.

## Compare native and translated SQLite

Prepare a SQLite source checkout with a native build and compilation database as
described in [Sweep and Test the C Corpus](corpus.md).

```sh
python3 tools/benchmark/sqlite.py --label baseline --build-only
python3 tools/benchmark/sqlite.py --label baseline --skip-build
python3 tools/benchmark/sqlite.py --label candidate --compare baseline
```

The script builds SQLite with Clang and translates and builds the same inputs
with Slate. Both builds use the `speedtest1` driver for runtime measurement.
`--slate-root <worktree>` lets you build the translator from another checkout.
Use `--compare-only` to compare saved runs in the same output directory.

Each label has its own directory under `target/sqlite-benchmark/`. Start with
`build.json` for compiler versions, build settings, and input identity. Compare
timings only for compatible inputs and settings. Use `--profile` to collect
profiles of the native and translated binaries when investigating a slowdown.

## Measure interprocedural scaling

```sh
python3 tools/benchmark/interproc_scaling.py \
  --shapes fanout,scc_ring --sizes 100,200,400,800 \
  --json target/interproc-scaling.json
```

The script generates C inputs with a controlled call graph. The fanout shape
stresses many callees, while the ring stresses propagation through a cycle. The
report separates total translation time from individual passes and phases, such
as `ir_pointer` joins and summarization. The fitted exponent summarizes how a
phase grows over the largest tested sizes. A slope near one suggests linear
growth over that range. A slope near two suggests quadratic growth.

Use `--keep <directory>` to retain the generated C inputs, or `--sqlite` to also
time interprocedural passes during translation of the SQLite corpus.
