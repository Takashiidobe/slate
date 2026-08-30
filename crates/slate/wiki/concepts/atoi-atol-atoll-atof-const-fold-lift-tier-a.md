# atoi/atol/atoll/atof const-fold lift (Tier A)

_created 2026-08-29_

Tier A of the numeric-parse lift (slate-y0qs.6.6): when the argument resolves to
a compile-time string literal, evaluate the C conversion at rewrite time and emit
a numeric literal. Provably correct for all inputs (junk suffix, sign, whitespace,
empty; integer overflow saturates — UB in C).

- `function_identity.rs`: added `Known::Atoll`, `Known::Atof`.
- `backend/engine/rules/libc_call.rs`: `CallCtx::const_str_arg` chases casts/`as_ptr`
  and non-reassigned raw-ptr `Let` inits back to a literal; `fold_atoi` (i32/i64)
  via `c_atoi_prefix`; `fold_atof` folds only finite full-string `f64::parse`, else bails.
- Non-literal / separately-assigned pointer args bail to libc (still correct).
- atof inf/nan and hex-float bail (codegen renders non-finite as invalid Rust; strtod-only).

Fixture: `tests/fixtures/atoi_atof_const_fold.c`. rewrites profile 110/110.

Deferred: Tier B (`__slate_ato*` prelude helper for dynamic args) needs a
backend→prelude wiring path (memchr injects at the frontend lowerer). errno tiering
only matters for strtol/strtoul/strtod, not these zero-error conversions.
