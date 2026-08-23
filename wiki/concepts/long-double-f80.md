# long double (f80) representation

> See [lowerer-internals.md](lowerer-internals.md)'s submodule table:
> `frontend/lowerer/runtime_support.rs` owns the generated `f80`/long-double
> shim, `frontend/lowerer/types.rs` handles the type-string parsing.

## Why not just `f64`

C `long double` and `double` are not interchangeable on the targets slate
cares about most: on Linux x86_64, `long double` is genuinely 80-bit x87
extended precision (10 bytes of value, padded to 16-byte alignment), with
different rounding/precision behavior than `f64`. Silently widening it to
`f64` would pass slate's differential tests on trivial cases and diverge on
anything precision-sensitive. So slate models it as its own type,
`LongDouble` (`frontend/lowerer/runtime_support.rs::LONG_DOUBLE_TY`), backed
by a `[u8; 10]` byte representation, with arithmetic implemented via
`rustc_apfloat` (the same target-independent arbitrary-precision float crate
rustc itself uses internally) since Rust has no native 80-bit float type.

## ABI varies by target — `uses_f64_long_double_abi()`

`long double`'s size/alignment is target-dependent, not just a slate
implementation detail:

- Linux x86_64: 80-bit value, 16-byte size, 16-byte alignment (x87 extended).
- macOS and MSVC targets: `long double` is ABI-identical to `double` — 8-byte
  size and alignment.

`cir::emit::uses_f64_long_double_abi()` reports which regime the current
target is in, and every layout/lowering decision that touches long double
(`c_layout` in the lowerer, the record-field `uses_long_double` flag) checks
it rather than assuming the x87 80-bit shape unconditionally.

## Integration points

- **Casts to/from arbitrary-width integers**: `_BitInt(N)`/unsigned
  `_BitInt(N)` values cast to/from `LongDouble` by routing through `i128`/
  `u128` as an intermediate width (`bitint_to_int_expr`,
  `f80_cast_from_name`/`f80_cast_to_name` in `frontend/lowerer/memory.rs`),
  rather than special-casing every bit-width pairing directly.
- **libc functions**: f80-returning/accepting libc functions (`strtold`,
  `fabsl`, `copysignl`, etc.) route through the same shim table as other
  known-libc calls in `frontend/c_shim.rs` — no bespoke special-casing per
  function.
- **`_Complex long double`**: composes with slate's general `_Complex`
  support, which is implemented via the `num-complex` crate rather than a
  hand-rolled complex type.
- **variadics**: `long double` arguments passed through `va_list` (e.g. a
  `scanf`-family call) are covered by dedicated test fixtures
  (`tests/fixtures/bionic/long_double_pointer.c`) since va_list argument
  promotion/passing for an oversized non-native type is an easy place for
  ABI bugs to hide.
- **Records**: a per-`Lowerer` `uses_long_double` flag is set whenever any
  lowered record field's type needs it, gating whether the generated program
  needs the `LongDouble` support code emitted at all.

## History

Implemented 2026-08-12 through 2026-08-14 (`wiki/log/2026-08-13-00-00.md`);
closed slate's "raw lowering failure" epic once the long double test suite
passed end to end.
