# Differential fixtures

<!-- toc -->
- [Suites](#suites)
- [Supported/unsupported ratchet](#supportedunsupported-ratchet)
- [Triage](#triage)
- [Workflow](#workflow)
- [Per-fixture compiler flags](#per-fixture-compiler-flags)
- [FileCheck (suspended)](#filecheck-suspended)
<!-- /toc -->

Fixtures are C programs. Slate compiles and runs the C source with
`clang` on PATH (the oracle) and the generated Rust, then requires identical
stdout and exit status. Runtime parity is the only correctness gate.

## Suites

`generated_differential`, `gcc_torture_suite`, `gcc_dg_suite`, and
`c_testsuite_suite` translate every fixture through the slate-parser frontend
(`support::translate_slate`, i.e. `slate translate-lowered`).
That emits raw lowered Rust. Headers come from slate-sysroots, and fixtures
run for the host target only. Together with `chibicc_suite`, these suites make
up the `slate` nextest profile:

```bash
cargo nextest r --release --profile slate
```

Run it from the workspace root. `chibicc_suite` uses `translate-project` for
two-TU fixtures. Additional suite coverage is tracked for restoration in
`slate-p58o.6.7` through `slate-p58o.6.17`; the fixture directories remain.

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
translation (`translate-lowered`) still fails on the first
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
signal rather than noise. Differential suites do not run FileCheck; do not add
`SLATE-FILECHECK` blocks or `@lowering`, `@rewrite`, or `@slate-lowerer` markers.
Earlier shape-check workflows and tools are documented in the
[historical index](../historical/index.md).
