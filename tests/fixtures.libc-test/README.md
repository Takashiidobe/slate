Vendored from `https://repo.or.cz/libc-test.git` (Rich Felker's musl
conformance suite) at commit `123433158bf985d7eb3b4072e32121b9e32a1a1a`.
MIT-licensed; `COPYRIGHT` is retained beside the corpus.

Two buckets, both under `api/` and `functional/` split into `supported/` and
`unsupported/`:

- `api/*.c` -- upstream's compile-only declaration/type conformance checks
  (`src/api`, minus the empty `main.c` stub every other api/\*.o links
  against). These never run; a case "passes" by compiling cleanly against
  `libc-shim` with the same flags upstream uses
  (`-pedantic-errors -Werror -Wno-unused -D_XOPEN_SOURCE=700`, plus
  `-Wno-strict-prototypes` -- every file uses the pre-C23 `static void f()`
  form upstream doesn't consider a bug). Each case is checked against both
  the glibc and musl x86-64 shim profiles (`tests/support/libc_shim.rs`),
  since the point of this bucket is catching declarations libc-shim gets
  right for one libc and wrong for the other, not exercising Slate's
  translation pipeline. `tests/libc_test_api_suite.rs` runs it.
- `functional/*.c` -- upstream's runtime behavior tests (`src/functional`).
  Differential: compiled and run both natively and through Slate's
  translation, stdout/exit compared, the same as `fixtures.c-testsuite`/
  `fixtures.gcc-torture`. `tests/libc_test_functional_suite.rs` runs it.

`tools/filter-libc-test.sh <libc-test checkout> <empty output dir>` (re)vendors
both buckets from a local clone. api files are copied unfiltered. Each
functional case is upstream's `<name>.c` merged with `src/common/{test.h,
print.c}` (`t_status`/`t_printf` -- the only `common/` helper any admitted
case needs) into one self-contained file, `#define _GNU_SOURCE 1` first so it
wins regardless of where in the original file a conditional
`#ifndef _GNU_SOURCE` guard appeared. Merging into one file, rather than
vendoring a 3-file `main.c`+`common.c`+`test.h` cross-TU fixture, lets this
bucket use Slate's plain single-file `translate` path (in-process, batched
like `gcc_torture_suite.rs`) instead of the heavier per-case
`translate-project` Cargo-crate machinery -- faster, and it keeps this bucket
scoped to libc conformance rather than also exercising Slate's cross-TU
handling (already covered by `tests/cross_tu.rs`). A case is admitted only if
the merged file compiles, links, and runs to completion (no crash/timeout)
under the CIR-enabled `SLATE_CLANG` with `-D_POSIX_C_SOURCE=200809L -lm -lrt`;
`FILTER.tsv` records the disposition of every upstream functional file (api
files are all `admitted` unconditionally). Rejected functional cases mostly
need a different `common/*.c` helper this harness doesn't vendor (`rand.c` for
randomized cases, `vmfill.c`/`fdfill.c`/`setrlim.c` for resource-limit cases,
`path.c` for argv-relative paths) or a companion DSO (the `dlopen*`/`tls_*dso`
`.mk` cases) -- expanding the vendored `common/` set is follow-up work, not
done here.

Of 79 upstream api files (78 after dropping `main.c`), all 78 are admitted;
50 currently pass through libc-shim, 28 don't. Of 77 upstream functional
files, 68 are admitted; 37 currently pass end to end through Slate, 31 don't.

Both suites' supported buckets run under `cargo nextest r --release`. Both
unsupported regression guards (`*_unsupported_tests_still_fail`) are
`#[ignore]`d by default to keep that gate fast; run them explicitly to look
for cases ready to promote:

```bash
cargo nextest r --release --test libc_test_api_suite -E 'test(libc_test_api_unsupported_tests_still_fail)' --run-ignored ignored-only
cargo nextest r --release --test libc_test_functional_suite -E 'test(libc_test_functional_unsupported_tests_still_fail)' --run-ignored ignored-only
```

To inspect one unsupported case's failure directly:

```bash
SLATE_LIBC_TEST_FIXTURE=<case-stem> cargo nextest r --release --test libc_test_api_suite -E 'test(libc_test_api_unsupported_triage_report)' --run-ignored ignored-only --nocapture
SLATE_LIBC_TEST_FIXTURE=<case-stem> cargo nextest r --release --test libc_test_functional_suite -E 'test(libc_test_functional_unsupported_triage_report)' --run-ignored ignored-only --nocapture
```

Both suites parallelize translation and compilation across every available
CPU through one shared work queue and cached `src/bin` batch crate, same as
`gcc_torture_suite.rs`. Set `SLATE_LIBC_TEST_JOBS` (or `SLATE_TEST_JOBS`) to
limit it.

Editing `libc-shim/` or these corpora doesn't usually touch CIR lowering, and
vice versa, so both suites (plus `libc_shim_suite.rs` and
`header_compilation.rs`) are grouped into a `libc` nextest profile, separate
from the C-language lowering corpora's `lang` profile (see
`.config/nextest.toml`):

```bash
cargo nextest r --release --profile libc  # libc-shim + libc-test only
cargo nextest r --release --profile lang  # c-testsuite/chibicc/gcc-torture/cross-TU/... only
```
