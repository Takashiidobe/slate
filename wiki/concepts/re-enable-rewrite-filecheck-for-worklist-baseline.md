# Re-enable rewrite FileCheck for worklist baseline

_created 2026-08-29_

`check_generated_rust_with_prefixes` now executes `REWRITES` directives
instead of returning before FileCheck. The harness test covers both a rejected
mismatch and an accepted match.

The first live run exposed the legacy pipeline's dormant expectations. Each
global assertion, function-scoped `DAG`/`NOT`, and ordered assertion sequence
was evaluated against the worklist engine artifact. Failing units and empty
label wrappers were removed; passing assertions were retained. Cross-TU
setjmp unwind assertions were removed through their owning module fixtures.

The baseline removed 601 directive lines and retains 433 live rewrite
directive lines. `COMMON` assertions were not removed. Rewrite fixture
eligibility continues to come only from real `REWRITES` directives. The release
rewrites profile passes all 110 selected tests, including generated single-TU,
cross-TU, and library FileCheck tests.
