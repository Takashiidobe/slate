Supported runnable `gcc.dg` differential fixtures live here. The matching
`fixtures.gcc-dg.unsupported` directory contains cases that compile and run
with Clang but still fail Slate differential testing.

The corpus currently includes atomic, complex, and long-double run cases.

Run the supported suite with:

```bash
cargo nextest r --release --profile lowering --test gcc_dg_suite \
  --ignore-default-filter -E 'test(gcc_dg_supported_tests_match_c)'
```

The runner extracts applicable `dg-options` from each fixture for translation
and the C reference build. Atomic references link `libatomic`; target-only
GCC dump and architecture options are ignored by the shared option parser.
