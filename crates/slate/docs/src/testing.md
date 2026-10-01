# Testing

Run release nextest profiles from the workspace root:

```bash
cargo nextest r --release --profile slate
cargo nextest r --release --profile parser
```

The Slate profile contains differential fixtures, c-testsuite, GCC torture,
GCC dg and chibicc. It compares C and raw lowered Rust stdout and exit status.
The parser profile covers slate-parser fixtures. Use each profile whose crate
changed. Passing raw lowering does not establish backend rewrite coverage.

## Ratchet and triage

Supported buckets must pass; unsupported buckets must still fail. When a case
starts passing, promote it with the `git mv` printed by the ratchet.

```bash
SLATE_DIFF_FIXTURE=<stem> cargo nextest r --release --profile slate \
  --test differential -E 'test(generated_differential)' --nocapture
SLATE_DIFF_FIXTURE=<stem> cargo nextest r --release --profile slate \
  --test differential -E 'test(fixtures_unsupported_triage_report)' \
  --run-ignored ignored-only --nocapture
```

Corpus selectors are `SLATE_GCC_TORTURE_FIXTURE`, `SLATE_GCC_DG_FIXTURE` and
`SLATE_C_TESTSUITE_FIXTURE`. Chibicc exercises project translation with two-TU
fixtures. FileCheck shape tests are suspended.

The [fixture guide](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/differential-fixtures.md)
and corpus READMEs define admission, selectors and ignored cases. Suite
restoration is tracked in `slate-p58o.6.7` through `slate-p58o.6.17`.
