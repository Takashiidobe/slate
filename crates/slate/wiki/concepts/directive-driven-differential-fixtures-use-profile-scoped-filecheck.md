# Directive-driven differential fixtures use profile-scoped FileCheck

_created 2026-08-24_

The differential harness is migrating one C fixture at a time from handwritten
Rust `contains` assertions to LLVM FileCheck directives embedded in the fixture.
Only migrated fixtures run by default; `SLATE_DIFF_FIXTURE` can still force any
legacy fixture through the runtime differential path.

`COMMON` checks run in both profiles. `LOWERING` and `REWRITES` checks run only
under their matching nextest profile. Position-independent checks use `DAG`.
Each function `LABEL` block is invoked independently, and every assertion is
bounded by the function's column-zero closing brace, so neither function order
nor a match in a later function can satisfy it.

Runtime C-versus-Rust stdout and exit-status comparison remains the correctness
oracle. See [differential fixtures](differential-fixtures.md) for syntax.
