Vendored from `gcc/testsuite/gcc.dg` at GCC commit
`76466425366d0f30effcdc1cfe729be28db4b88e`.

Unlike `gcc.c-torture/execute`, `gcc.dg` is GCC's general test directory: most
of it is `dg-do compile` (checking accepted/rejected diagnostics, not runtime
behavior) and has nothing to differential-test. This corpus is scoped to the
`c23-*.c` cases (GCC's C23-conformance tests, renamed from `c2x-*` once C23
was ratified) that are `dg-do run`.

The source snapshot is filtered with:

```bash
git clone --depth 1 --filter=blob:none --sparse https://github.com/gcc-mirror/gcc.git
git -C gcc sparse-checkout set gcc/testsuite/gcc.dg
tools/filter-gcc-dg-c23.sh gcc/gcc/testsuite/gcc.dg /tmp/gcc-dg-c23-filtered
```

`filter-gcc-dg-c23.sh` selects `gcc.dg/c23-*.c` files declaring `dg-do run`,
then reuses `filter-gcc-torture.sh`'s gate unchanged: each candidate is copied
without auxiliary sources, compiled with the CIR-enabled `SLATE_CLANG` using
`-O0 -std=c23`, and admitted only when the resulting executable exits 0
within five seconds. `FILTER.tsv` records the disposition of every candidate.

Supported cases live here. Cases that pass the Clang gate but do not yet match
after Slate translation live in `../fixtures.gcc-dg-c23.unsupported`.

Of 70 `dg-do run` candidates, the reference filter admitted 42 and rejected 28
at compile time (3 need a second translation unit via `dg-additional-sources`
that the single-file gate can't link; several `#include` a sibling test file
from `gcc.dg` that wasn't vendored; the rest are genuine clang/GCC divergences
on corner cases -- e.g. `nullptr_t` assigned to `_Atomic _Bool`, or GCC
warning-but-allowing what clang treats as a hard error -- unrelated to Slate).
Differential classification placed 22 admitted cases here and 20 in the
unsupported tree.

The supported bucket runs under `cargo nextest r --release --profile lowering`.
The unsupported guard is ignored during the normal gate. To check the full
unsupported bucket for cases ready to promote, run:

```bash
cargo nextest r --release --test gcc_dg_c23_suite -E 'test(gcc_dg_c23_unsupported_tests_still_fail)' --run-ignored ignored-only
```

To inspect one unsupported case, run:

```bash
SLATE_GCC_DG_C23_FIXTURE=<case-stem> cargo nextest r --release --test gcc_dg_c23_suite -E 'test(gcc_dg_c23_unsupported_triage_report)' --run-ignored ignored-only --nocapture
```

The vendored sources remain under GCC's GPLv3 terms; `COPYING3` is retained
beside the corpus.
