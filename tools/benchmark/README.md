# SQLite runtime benchmarks

Run from the workspace root. Requires the SQLite corpus's Clang compilation
database, Clang, the Rust toolchain, zlib, readline, and ncurses. `--profile`
also requires permission to collect `perf` counters.

Capture `before`, apply the optimization, then run `after`:

```bash
python3 tools/benchmark/sqlite.py --label before
python3 tools/benchmark/sqlite.py --label after --compare before
```

- Corpus defaults to `~/c-corpus/sqlite`; pass another directory as the positional argument.
- Compilation database defaults to `<corpus>/build-clang/compile_commands.json`;
  override with `--compile-commands`.
- Builds matching Clang `-O2` and Rust release libraries and translated shells
  under `target/sqlite-benchmark/<label>/`. Slate is rebuilt before translation.
- Project translation applies control-flow rewrites; `--raw` builds the raw baseline.
  `build.json` records the selected mode.
- Retains per-unit defines/includes. Clang uses PIC; translation receives the
  matching `__PIC__`/`__pic__` defines because slate-parser rejects `-fPIC`.
- Runs SQL, error, persistence, CSV, C API, WAL concurrency, and interrupt checks.
- Benchmarks `speedtest1` main, CTE, and JSON: in-memory, size 50, one warmup,
  five alternating native/generated samples; verification hashes must match.
- `--size`, `--runs`, and `--testsets` select workloads. Avoid overlapping
  benchmarks with builds, tests, or other CPU workloads.
- `build.json` records compilers, CPU, source/driver hashes, flags, build timings, VDBE
  state/transfer counts, and optimized VDBE code sizes. `source.patch` records
  uncommitted translator changes.
- Each build retains VDBE disassembly as `clang/vdbe.asm` and `rust/vdbe.asm`.
- `runtime/runtime.json` retains samples, medians, spread, and hashes.
  `--compare` checks configuration parity and writes `comparison.json` and
  `comparison.md`. The table uses one Clang median from the baseline run,
  alongside Rust before/after, optimization speedup, and the remaining Clang gap.
- Report saved results without rebuilding or benchmarking:
  `python3 tools/benchmark/sqlite.py --label after --compare before --compare-only`.
- `--build-only` builds without timing workloads; `--skip-build` reruns existing
  artifacts. `--profile` collects verified CPU profiles and hardware counters.
