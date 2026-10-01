Supported runnable `gcc.dg` differential fixtures live here. The matching
`fixtures.gcc-dg.unsupported` directory contains cases that compile and run
with Clang but still fail Slate differential testing.

The corpus includes atomic, complex, long-double, and C23 run cases.
`dg-shouldfail` cases are excluded: sanitizer tests that intentionally trigger
undefined behavior have no deterministic result to compare without instrumentation.

Run the supported suite from the workspace root:

```bash
cargo nextest r --release --profile slate --test gcc_dg_suite \
  -E 'test(gcc_dg_supported_tests_match_c)'
```

The runner extracts applicable `dg-options` from each fixture for translation
and the C reference build. Atomic references link `libatomic`; target-only
GCC dump and architecture options are ignored by the shared option parser.
The C oracle is `clang` on PATH.
