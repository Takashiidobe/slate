# SQLite differential fuzzing

Run from the workspace root. Requires the SQLite corpus at `~/c-corpus/sqlite`
with its Clang compilation database, Clang with libFuzzer, and the Rust toolchain.

```bash
python3 tools/fuzz/sqlite.py libraries   # native + translated libsqlite3 with extensions
python3 tools/fuzz/sqlite.py build       # instrumented native SQLite + harness
python3 tools/fuzz/sqlite.py seeds       # seeds from test/fuzzdata*.db
python3 tools/fuzz/sqlite.py fuzz --seconds 600
python3 tools/fuzz/sqlite.py fuzz target/sqlite-fuzz/divergences/<file>   # replay one input
```

- `libraries` builds `target/sqlite-benchmark/fuzz/` through
  `tools/benchmark/sqlite.py --build-only`, adding FTS3/4/5, RTREE, geopoly,
  dbstat, dbpage, bytecode/stmt vtabs, carray, STAT4, and ordered-set aggregates.
- The fuzzer links native SQLite, compiled with `-fsanitize=fuzzer-no-link`, and
  `dlopen`s the translated `rust/libsqlite3.so` with `RTLD_LOCAL|RTLD_DEEPBIND`.
  Coverage feedback comes from the native side only.
- An input is SQL, or a dbsqlfuzz input: a hex database image, a `\n--\n`
  divider, then SQL. Images load with `sqlite3_deserialize`; header bytes 72..79
  toggle db_config settings and optimizations, as in `test/fuzzcheck.c`.
- Both libraries run each input on an in-memory database. The harness compares
  a trace of open, prepare, step, finalize, and close codes, error messages, change
  counts, and typed column values (doubles as `%a`, text and blobs as hex).
- Determinism: a fixed PRNG seed per input, a VFS with a fixed clock and
  randomness that refuses named files, a step-count progress limit, heap limits
  reset per input, zero-filled allocations, and an authorizer that denies global
  pragmas and `compile_options` (slate-parser reports its own clang version).
- `fuzz` runs `-jobs`/`-workers` with one per CPU, writing `fuzz-N.log` under
  `target/sqlite-fuzz/`. Divergent inputs go to `target/sqlite-fuzz/divergences/`
  and fuzzing continues. Without `SLATE_SQLITE_DIVERGENCES` set, a divergence aborts.

## Known noise

- Corrupt database images occasionally diverge even native against native
  (point `SLATE_SQLITE_TRANSLATED` at `target/sqlite-benchmark/fuzz/clang/libsqlite3.so`).
  About one in 46k seeds per replay, never the same input twice, and none
  reproduce when replayed alone: SQLite's results there depend on state left by
  earlier inputs, likely uninitialized stack reads. Replay a divergence on its
  own before treating it as a translation bug.
- Unnamed temp files use the shared temp directory, and both libraries draw the
  same names from the fixed PRNG seed, so parallel jobs can collide on them.
