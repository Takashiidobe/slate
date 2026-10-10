/* Test _Decimal64x in C23 mode.  */
/* { dg-options "-std=c23 -pedantic-errors" } */

int a = (int) 1.1D64x;
int b = (int) 2.d64x;
_Decimal64x c = 1;

#define expr_has_type(e, t) _Generic (e, default : 0, t : 1)
static_assert (expr_has_type (1.1D64x, _Decimal64x));
static_assert (expr_has_type (2.d64x, _Decimal64x));
static_assert (expr_has_type (.33D64x, _Decimal64x));
static_assert (expr_has_type (2e1d64x, _Decimal64x));
static_assert (expr_has_type (.3e2D64x, _Decimal64x));
static_assert (expr_has_type (4.5e3d64x, _Decimal64x));
static_assert (expr_has_type (5.e0D64x, _Decimal64x));
static_assert (expr_has_type (1e+2d64x, _Decimal64x));
static_assert (expr_has_type (1000e-3D64x, _Decimal64x));
static_assert (expr_has_type (__DEC64X_MIN__, _Decimal64x));
static_assert (expr_has_type (__DEC64X_MAX__, _Decimal64x));
static_assert (expr_has_type (__DEC64X_EPSILON__, _Decimal64x));
static_assert (expr_has_type (__DEC64X_SUBNORMAL_MIN__, _Decimal64x));
#if __DEC64X_MANT_DIG__ == __DEC128_MANT_DIG__ \
    && __DEC64X_MAX_EXP__ == __DEC128_MAX_EXP__
static_assert (expr_has_type (1.1D32 + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1.1D64 + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1.1D128 + 1.1D64x, _Decimal128));
static_assert (expr_has_type (1 + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1U + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1L + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1UL + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1LL + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1ULL + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1.1D32, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1.1D64, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1.1D64x, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1.1D128, _Decimal128));
static_assert (expr_has_type (1.1D64x + 1, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1U, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1L, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1UL, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1LL, _Decimal64x));
static_assert (expr_has_type (1.1D64x + 1ULL, _Decimal64x));
#endif

_Decimal64x
foo (_Decimal64x x, _Decimal64x y)
{
  return x + y;
}

int
main ()
{
#if __DEC64X_MANT_DIG__ == __DEC128_MANT_DIG__ \
    && __DEC64X_MAX_EXP__ == __DEC128_MAX_EXP__
  if (1.1D64x != 1.1dl
      || 2.d64x != 2.dl
      || .33D64x != .33dl
      || 2e1d64x != 2e1dl
      || .3e2D64x != .3e2dl
      || 4.5e3d64x != 4.5e3dl
      || 5.e0D64x != 5.e0dl
      || 1e+2d64x != 1e+2dl
      || 1000e-3D64x != 1000e-3dl
      || __DEC64X_MIN__ != __DEC128_MIN__
      || __DEC64X_MAX__ != __DEC128_MAX__
      || __DEC64X_EPSILON__ != __DEC128_EPSILON__
      || __DEC64X_SUBNORMAL_MIN__ != __DEC128_SUBNORMAL_MIN__
      || foo (0.5d64x, 0.5D64x) != 1.D64x)
    __builtin_abort ();
#endif
}

// SLATE-FILECHECK-STD DEFAULT c23
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d128>(1.1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d128>(2.)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: d128 [storage=static] = int_to_float<d128, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: d128, %[[VALUE_y:[0-9]+]] y: d128) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<d128, rounding=nearest_even, exceptions=observable, contract=off>(read<d128>(%[[VALUE_x]]), read<d128>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<d128, exceptions=observable>(const<d128>(1.1), const<d128>(1.1)), ne<d128, exceptions=observable>(const<d128>(2.), const<d128>(2.))), ne<d128, exceptions=observable>(const<d128>(.33), const<d128>(.33))), ne<d128, exceptions=observable>(const<d128>(2e1), const<d128>(2e1))), ne<d128, exceptions=observable>(const<d128>(.3e2), const<d128>(.3e2))), ne<d128, exceptions=observable>(const<d128>(4.5e3), const<d128>(4.5e3))), ne<d128, exceptions=observable>(const<d128>(5.e0), const<d128>(5.e0))), ne<d128, exceptions=observable>(const<d128>(1e+2), const<d128>(1e+2))), ne<d128, exceptions=observable>(const<d128>(1000e-3), const<d128>(1000e-3))), ne<d128, exceptions=observable>(const<d128>(1e-6143), const<d128>(1e-6143))), ne<d128, exceptions=observable>(const<d128>(9.999999999999999999999999999999999e6144), const<d128>(9.999999999999999999999999999999999e6144))), ne<d128, exceptions=observable>(const<d128>(1e-33), const<d128>(1e-33))), ne<d128, exceptions=observable>(const<d128>(0.000000000000000000000000000000001e-6143), const<d128>(0.000000000000000000000000000000001e-6143)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], ne<d128, exceptions=observable>(call<d128, signature=fn(d128, d128) -> d128>(%[[VALUE_foo]], const<d128>(0.5), const<d128>(0.5)), const<d128>(1.)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
