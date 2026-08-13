Vendored from https://github.com/c-testsuite/c-testsuite (`tests/single-exec`,
MIT-licensed harness; individual test provenance is in each upstream `.otags`
file, not carried over here). Files are renamed `case_<NNNNN>.c` from the
upstream `<NNNNN>.c` numbering — a bare leading digit isn't a legal Rust/Cargo
crate-name character, which the generated per-case Cargo projects need.

The supported cases in this directory run under
`cargo nextest r --release --profile lowering`
through `tests/c_testsuite_suite.rs`.

`../fixtures.c-testsuite.unsupported/TRIAGE.md` records why each unsupported
case currently fails. The unsupported regression guard also runs by default;
run its ignored triage report with
`cargo nextest r --release --test c_testsuite_suite -E 'test(c_testsuite_unsupported_triage_report)' --run-ignored ignored-only --nocapture`.
