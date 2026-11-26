# Testing

Run release nextest profiles from the workspace root:

```bash
cargo nextest r --release --profile slate
cargo nextest r --release --profile parser
```

The Slate profile compares stdout and exit status from native C and generated
Rust. It runs the differential fixtures, c-testsuite, GCC torture, GCC dg, and
chibicc. The parser profile checks slate-parser fixtures. Run the profile for
each crate you change. Run the `merge` profile for changes to `--merge` or
`tests/fixtures.merge/`.

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

Corpus selectors are `SLATE_GCC_TORTURE_FIXTURE`, `SLATE_GCC_DG_FIXTURE`, and
`SLATE_C_TESTSUITE_FIXTURE`. FileCheck verifies the shape of generated Rust for
fixtures with check blocks after they pass the runtime comparison.

The
[fixture guide](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/differential-fixtures.md)
and corpus READMEs cover fixture admission, selectors, and ignored cases.
