# Differential fixtures

Fixtures are C programs. Slate compiles and runs the C source with
`SLATE_CLANG` (the oracle) and the generated Rust, then requires identical
stdout and exit status. Runtime parity is the only correctness gate.

## Suites

`generated_differential`, `gcc_torture_suite`, `gcc_dg_suite`, and
`c_testsuite_suite` translate every fixture through the slate-parser frontend
(`support::translate_slate`, i.e. `slate translate-lowered --frontend=slate`).
That emits raw lowered Rust: rewrites are not enabled for the slate frontend,
and there is no CIR fallback. Headers come from slate-sysroots, and fixtures
run for the host target only. These four suites make up the `slate` nextest
profile:

```bash
cargo nextest r --release --profile slate
```

Run it from the workspace root. `chibicc_suite`, `cross_tu`, `link`, `syslink`,
`yarpgen`, `directive_translate`, and the `differential_{arm,aarch64,i686}`
binaries still exercise the legacy CIR pipeline and are not gates.

## Supported/unsupported ratchet

Each suite has a supported and an unsupported bucket. Supported fixtures must
pass; unsupported fixtures must still fail. When an unsupported fixture starts
passing, `*_unsupported_tests_still_fail` fails and prints the `git mv` that
promotes it.

| Suite       | Supported                    | Unsupported                              | Selector                    |
| ----------- | ---------------------------- | ---------------------------------------- | --------------------------- |
| fixtures    | `tests/fixtures`             | `tests/fixtures.unsupported`             | `SLATE_DIFF_FIXTURE`        |
| gcc-torture | `tests/fixtures.gcc-torture` | `tests/fixtures.gcc-torture.unsupported` | `SLATE_GCC_TORTURE_FIXTURE` |
| gcc-dg      | `tests/fixtures.gcc-dg`      | `tests/fixtures.gcc-dg.unsupported`      | `SLATE_GCC_DG_FIXTURE`      |
| c-testsuite | `tests/fixtures.c-testsuite` | `tests/fixtures.c-testsuite.unsupported` | `SLATE_C_TESTSUITE_FIXTURE` |

Paths are relative to `crates/slate`. `tests/fixtures/x86_64/` (and its
unsupported twin) runs only on x86_64 hosts. `*.ignored` buckets hold only
features Slate will never support. Selectors take a fixture stem.

## Triage

`fixtures_unsupported_triage_report`, `gcc_torture_unsupported_triage_report`,
and `c_testsuite_unsupported_triage_report` are ignored tests. Each prints
`PASS`/`FAIL` per unsupported fixture, with the first barrier classified as
`parse/sema`, `unsupported lowering`, `rustc`, or `runtime mismatch`:

```bash
SLATE_DIFF_FIXTURE=<stem> cargo nextest r --release -p slate --test differential \
  -E 'test(fixtures_unsupported_triage_report)' --run-ignored ignored-only --nocapture
```

`slate lowering-barriers [compiler args] <file.c>` lists every function
defined in one translation unit with `ok` or its first lowering barrier, plus
`<module>` lines for top-level barriers (unsupported globals, top-level asm).
It never emits Rust and exits non-zero when any barrier exists. Strict
translation (`translate-lowered --frontend=slate`) still fails on the first
barrier. Record and enum definitions only block the functions that use them;
sema already resolves typedefs, so the module's type list is not a gate.

## Workflow

1. Put the fixture in the unsupported bucket and confirm the triage report
   shows the expected barrier.
2. Implement the lowering until the report prints `PASS <stem>`.
3. `git mv` the fixture into the supported bucket.
4. Run the full `slate` profile and promote anything else the change fixed.

## Per-fixture compiler flags

`dg-options "..."` comments in a fixture add compiler flags to both
slate-parser and the C oracle; `dg-additional-options "..."` adds them to
slate-parser only. Only semantic flags are kept (`-std=`, `-O0`..`-O3`, and
the list in `is_semantic_dg_flag` in `tests/support/mod.rs`), and a
`{ target ... }` clause that does not match the host triple drops the
directive.

## FileCheck (suspended)

FileCheck shape assertions are suspended until lowering coverage makes them
signal rather than noise. All `SLATE-FILECHECK` blocks and `@lowering`,
`@rewrite`, and `@slate-lowerer` markers were stripped from single-file
fixtures, and the differential suites do not run FileCheck. Do not add
markers or blocks, and do not run `tools/update_filecheck.py` for
single-file fixtures.

The tooling is still in the tree (`tools/update_filecheck.py`,
`tests/support/filecheck.rs`, the `justfile` regen recipes). Its full
documentation, covering region markers, `-fn-` markers, cross-target
prefixes, and project mode, is in this page's history before commit
`c6c92aef4`, for when shape checks return.
