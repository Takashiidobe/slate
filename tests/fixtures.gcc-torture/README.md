Vendored from `gcc/testsuite/gcc.c-torture/execute` at GCC commit
`1dec3355162851ab3cf991fbf8c7fa9cd08331f3`.

The source snapshot is filtered with:

```bash
git clone --depth 1 --filter=blob:none --sparse https://github.com/gcc-mirror/gcc.git
git -C gcc sparse-checkout set gcc/testsuite/gcc.c-torture/execute
tools/filter-gcc-torture.sh gcc/gcc/testsuite/gcc.c-torture/execute /tmp/gcc-torture-filtered
```

The filter copies each C file without auxiliary sources or headers, compiles it
with the CIR-enabled `SLATE_CLANG` using `-O0 -std=c23`, and admits it only when
the resulting executable exits 0 within five seconds. Nested upstream paths
are flattened with `__`. `FILTER.tsv` records the disposition of every
upstream C file.

Supported cases live here. Cases that pass the Clang gate but do not yet match
after Slate translation live in `../fixtures.gcc-torture.unsupported`.

Of 1,918 upstream C files, the reference filter admitted 1,443, rejected 468 at
compile time, and rejected 7 that aborted. Differential classification placed
1,207 admitted cases here and 236 in the unsupported tree. The complete triage
output is preserved as `../fixtures.gcc-torture.unsupported/TRIAGE.log.gz` and
follow-up classification is tracked by `slate-os0h.3.1`.

Both bucket guards run under `cargo nextest r --release`. To inspect one
unsupported case through the shared cached `src/bin` batch crate, run:

```bash
SLATE_GCC_TORTURE_FIXTURE=<case-stem> cargo nextest r --release --test gcc_torture_suite -E 'test(gcc_torture_unsupported_triage_report)' --run-ignored ignored-only --nocapture
```

The vendored sources remain under GCC's GPLv3 terms; `COPYING3` is retained
beside the corpus.
