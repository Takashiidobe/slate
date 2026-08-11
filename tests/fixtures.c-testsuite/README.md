Vendored from https://github.com/c-testsuite/c-testsuite (`tests/single-exec`,
MIT-licensed harness; individual test provenance is in each upstream `.otags`
file, not carried over here). Files are renamed `case_<NNNNN>.c` from the
upstream `<NNNNN>.c` numbering — a bare leading digit isn't a legal Rust/Cargo
crate-name character, which the generated per-case Cargo projects need.

Not wired into `cargo nextest r --release` by default; run explicitly with
`cargo nextest r --release --test c_testsuite_suite` (see the `test = false`
entry for it in `Cargo.toml`).

`unsupported/TRIAGE.md` records why each case in that bucket currently fails.
