# long double (f80) representation

<!-- toc -->
- [Slate frontend](#slate-frontend)
- [Why not just `f64`](#why-not-just-f64)
- [ABI varies by target —
  `uses_f64_long_double_abi()`](#abi-varies-by-target--uses_f64_long_double_abi)
- [Integration points](#integration-points)
- [History](#history)
<!-- /toc -->

> The `LongDouble` prelude, the `__slate_f80_*` shim declarations, and the
> bridge type tags live in `slate_parser_frontend/long_double.rs`, shared by
> the slate frontend and the legacy CIR lowerer.

## Slate frontend

`slate_parser_frontend/lowerer.rs` lowers IR `f80` to `LongDouble` and emits
the prelude when any f80 type is lowered. Constants become
`LongDouble([10 bytes])` straight from `Number::FloatBits`. Arithmetic,
negation, and comparisons use the prelude's operator impls. Every `Convert`
with an f80 side calls `__slate_f80_from_<t>` / `__slate_f80_to_<t>`.

A call to a body-less function whose return or argument holds f80 by value
(including inside a struct) becomes a call to a C bridge named
`__slate_<callee>__r<ret>_<arg tags>`. The test harness renders it from the
name (`slate_parser_frontend::c_shim::render_shim_c_source_for_names`) and links it with
`shims/long_double.c`, and the direct extern declaration is dropped. Barriers
remain for variadic callees missing from `function_identity::Known` (the
bridge needs their header), and for non-pointer aggregate arguments to a
bridge. Non-variadic bridged callees depend on `long_double.c` including
`<math.h>`.

Variadic `long double` arguments must reach the callee as real x87 values in
memory; Rust would pass `LongDouble` as an INTEGER-class struct. A call to a
Rust-defined variadic function with f80 variadic arguments becomes a call to
`__slate_vcall__r<ret>_<fixed tags>__<variadic tags>(callee as *const (), ...)`.
The C trampoline (`c_shim::render_variadic_trampoline`) calls the pointer as
`<ret> (*)(<fixed>, ...)` with fixed f80 parameters and the return kept as
`__slate_f80`, and loads only the variadic f80 arguments into `long double`.
The callee is passed by pointer rather than exported by symbol, so static
functions with the same name in different TUs, or named like a libc or Rust
runtime symbol (`exit`), never collide, and trampolines are shared per
signature. On the callee side, `va_arg(ap, long double)` lowers to
`__slate_f80_va_arg(&mut ap)`, which relies on x86_64 `VaList` sharing C's
`__va_list_tag` layout so C advances the Rust list in place.

- Runtime and name-based bridges: `slate_parser_frontend/c_shim.rs` and
  `slate_parser_frontend/shims/{long_double,fenv}.c`.
- Legacy typed bridges: `frontend/c_shim.rs`; retained for CIR callers.
- Generated crates compile `src/slate_long_double.c` with `cc` in `build.rs`.

## Why not just `f64`

C `long double` and `double` are not interchangeable on the targets slate
cares about most: on Linux x86_64, `long double` is genuinely 80-bit x87
extended precision (10 bytes of value, padded to 16-byte alignment), with
different rounding/precision behavior than `f64`. Silently widening it to
`f64` would pass slate's differential tests on trivial cases and diverge on
anything precision-sensitive. So slate models it as its own type,
`LongDouble` (`slate_parser_frontend/long_double.rs::LONG_DOUBLE_TY`), backed
by a `[u8; 10]` byte representation, with every x87 operation delegated to C
helpers in `slate_parser_frontend/shims/long_double.c` since Rust has no native 80-bit
float type.

## ABI varies by target — `uses_f64_long_double_abi()`

`long double`'s size/alignment is target-dependent, not just a slate
implementation detail:

- Linux x86_64: 80-bit value, 16-byte size, 16-byte alignment (x87 extended).
- 32-bit ARM Linux: ABI-identical to `double` — 8-byte size and alignment.
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
  known-libc calls in `slate_parser_frontend/c_shim.rs` — no bespoke special-casing per
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
