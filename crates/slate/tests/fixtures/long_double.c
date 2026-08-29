#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

void abort(void);

static long double mix_long_double(long double a, long double b) {
  long double c = (a + b) / 2.0L;
  return c * 3.0L;
}

static int truncate_long_double(long double value) { return (int)value; }

static void print_ld(const char *name, long double v) {
  printf("%s=%La\n", name, v);
}

static void check_int_casts(void) {
  int8_t i8 = (int8_t)-100.0L;
  if ((long double)i8 != -100.0L)
    abort();
  uint8_t u8 = (uint8_t)200.0L;
  if ((long double)u8 != 200.0L)
    abort();

  int16_t i16 = (int16_t)-12345.0L;
  if ((long double)i16 != -12345.0L)
    abort();
  uint16_t u16 = (uint16_t)54321.0L;
  if ((long double)u16 != 54321.0L)
    abort();

  int32_t i32 = (int32_t)-1234567890.0L;
  if ((long double)i32 != -1234567890.0L)
    abort();
  uint32_t u32 = (uint32_t)3456789012.0L;
  if ((long double)u32 != 3456789012.0L)
    abort();

  int64_t i64 = (int64_t)-123456789012345LL;
  if ((long double)i64 != -123456789012345.0L)
    abort();
  uint64_t u64 = (uint64_t)12345678901234567890.0L;
  if ((long double)u64 != 12345678901234567890.0L)
    abort();

  printf("i8=%d u8=%u i16=%d u16=%u i32=%d u32=%u i64=%lld u64=%llu\n", i8, u8,
         i16, u16, i32, u32, (long long)i64, (unsigned long long)u64);
}

static void check_i128_casts(void) {
  __int128 i128 = (__int128)(-9223372036854775807.0L);
  if ((long double)i128 != -9223372036854775807.0L)
    abort();

  unsigned __int128 u128 = (unsigned __int128)18446744073709551615.0L;
  if ((long double)u128 != 18446744073709551615.0L)
    abort();

  printf("i128=%lld u128_hi=%llu u128_lo=%llu\n", (long long)i128,
         (unsigned long long)(u128 >> 64),
         (unsigned long long)(u128 & 0xFFFFFFFFFFFFFFFFULL));
}

static void check_bitint_casts(void) {
  _BitInt(9) b9 = (_BitInt(9))(-100.0L);
  if ((long double)b9 != -100.0L)
    abort();

  unsigned _BitInt(9) ub9 = (unsigned _BitInt(9))200.0L;
  if ((long double)ub9 != 200.0L)
    abort();

  _BitInt(40) b40 = (_BitInt(40))(-123456789.0L);
  if ((long double)b40 != -123456789.0L)
    abort();

  unsigned _BitInt(40) ub40 = (unsigned _BitInt(40))987654321.0L;
  if ((long double)ub40 != 987654321.0L)
    abort();

  _BitInt(101) b101 = (_BitInt(101))(-123456789012345.0L);
  if ((long double)b101 != -123456789012345.0L)
    abort();
  if ((_BitInt(101))(long double)b101 != b101)
    abort();

  unsigned _BitInt(150) ub150 = (unsigned _BitInt(150))987654321098765.0L;
  if ((long double)ub150 != 987654321098765.0L)
    abort();
  if ((unsigned _BitInt(150))(long double)ub150 != ub150)
    abort();

  _BitInt(256) b256 = (_BitInt(256))9999999999.0L;
  if (b256 != 9999999999)
    abort();
  if ((long double)b256 != 9999999999.0L)
    abort();

  unsigned _BitInt(300) ub300 = (unsigned _BitInt(300))4200000000.0L;
  if (ub300 != 4200000000U)
    abort();
  if ((long double)ub300 != 4200000000.0L)
    abort();

  _BitInt(129) b129 = (_BitInt(129))123.0L;
  if ((int)b129 != 123)
    abort();
  if ((long double)(_BitInt(129))123 != 123.0L)
    abort();

  printf("bitint_b101=%lld bitint_ub150=%llu bitint_b256_lo=%lld "
         "bitint_ub300_lo=%llu\n",
         (long long)b101, (unsigned long long)ub150, (long long)b256,
         (unsigned long long)ub300);
}

static void check_math_functions(void) {
  print_ld("sqrt", sqrtl(2.0L));
  print_ld("cbrt", cbrtl(27.0L));
  print_ld("sin", sinl(0.0L));
  print_ld("cos", cosl(0.0L));
  print_ld("tan", tanl(0.0L));
  print_ld("asin", asinl(1.0L));
  print_ld("acos", acosl(1.0L));
  print_ld("atan", atanl(1.0L));
  print_ld("atan2", atan2l(1.0L, 1.0L));
  print_ld("sinh", sinhl(1.0L));
  print_ld("cosh", coshl(1.0L));
  print_ld("tanh", tanhl(1.0L));
  print_ld("exp", expl(1.0L));
  print_ld("exp2", exp2l(10.0L));
  print_ld("log", logl(expl(1.0L)));
  print_ld("log2", log2l(8.0L));
  print_ld("log10", log10l(1000.0L));
  print_ld("pow", powl(2.0L, 10.0L));
  print_ld("floor", floorl(2.7L));
  print_ld("ceil", ceill(2.1L));
  print_ld("round", roundl(2.5L));
  print_ld("trunc", truncl(-2.7L));
  print_ld("fabs", fabsl(-3.5L));
  print_ld("fmod", fmodl(10.0L, 3.0L));
  print_ld("hypot", hypotl(3.0L, 4.0L));
  print_ld("copysign", copysignl(3.0L, -1.0L));
  print_ld("fmax", fmaxl(1.0L, 2.0L));
  print_ld("fmin", fminl(1.0L, 2.0L));
  print_ld("fma", fmal(2.0L, 3.0L, 4.0L));
  print_ld("ldexp", ldexpl(1.0L, 4));

  int exp = 0;
  print_ld("frexp", frexpl(100.0L, &exp));
  printf("frexp_exp=%d\n", exp);

  printf("isnan=%d isinf=%d signbit_neg=%d signbit_pos=%d isfinite=%d\n",
         isnan(nanl("")), isinf(HUGE_VALL), signbit(-1.0L), signbit(1.0L),
         isfinite(LDBL_MAX));

  print_ld("epsilon", LDBL_EPSILON);
}

/* The functions above all round-trip through the generic call-shim (any
 * known extern function with a long double arg/return links straight to
 * libm), which check_math_functions already exercises. This covers the
 * remaining libm entry points -- pointer out-params, integer-returning
 * variants, and the classification family -- with volatile operands so
 * they can't constant-fold away and skip the real runtime path. */
static void check_remaining_math_functions(void) {
  volatile long double ten = 10.0L;
  volatile long double three = 3.0L;

  long double ipart = 0.0L;
  long double frac = modfl(ten / three, &ipart);
  print_ld("modf_ipart", ipart);
  print_ld("modf_frac", frac);

  print_ld("remainder", remainderl(ten, three));

  int quo = 0;
  print_ld("remquo", remquol(ten, three, &quo));
  printf("remquo_quo=%d\n", quo);

  print_ld("scalbn", scalbnl(ten, 3));
  print_ld("scalbln", scalblnl(ten, 3L));
  print_ld("nextafter", nextafterl(ten, three));
  print_ld("nexttoward", nexttowardl(ten, three));
  print_ld("fdim", fdiml(ten, three));
  print_ld("rint", rintl(ten / three));
  print_ld("nearbyint", nearbyintl(ten / three));

  printf("lrint=%ld llrint=%lld lround=%ld llround=%lld\n",
         lrintl(ten / three), llrintl(ten / three), lroundl(ten / three),
         llroundl(ten / three));

  printf("ilogb=%d\n", ilogbl(ten));
  print_ld("logb", logbl(ten));

  print_ld("erf", erfl(ten / three));
  print_ld("erfc", erfcl(ten / three));
  print_ld("tgamma", tgammal(three));
  print_ld("lgamma", lgammal(ten));

  volatile long double vnan  = nanl("");
  volatile long double vinf  = HUGE_VALL;
  volatile long double vzero = 0.0L;
  volatile long double vone  = 1.0L;
  volatile long double vsub  = LDBL_TRUE_MIN;
  printf("isnan_v=%d isinf_v=%d isfinite_v=%d isnormal_v=%d "
         "isunordered_v=%d isunordered_ok=%d\n",
         isnan(vnan), isinf(vinf), isfinite(vone), isnormal(vone),
         isunordered(vnan, vone), isunordered(vone, vzero));
  printf("subnormal_isnormal=%d\n", isnormal(vsub));

  volatile long double vtwo = 2.0L;
  printf("islessgreater_lt=%d islessgreater_eq=%d islessgreater_nan=%d\n",
         islessgreater(vone, vtwo), islessgreater(vone, vone),
         islessgreater(vnan, vone));

  long double ten_plain = ten;
  long double canon     = 0.0L;
  int canon_r           = canonicalizel(&canon, &ten_plain);
  print_ld("canonicalize", canon);
  printf("canonicalize_r=%d\n", canon_r);

  print_ld("ldbl_min", LDBL_MIN);
  print_ld("ldbl_true_min", LDBL_TRUE_MIN);
  printf("ldbl_mant_dig=%d ldbl_dig=%d ldbl_min_exp=%d ldbl_max_exp=%d "
         "ldbl_min_10_exp=%d ldbl_max_10_exp=%d\n",
         LDBL_MANT_DIG, LDBL_DIG, LDBL_MIN_EXP, LDBL_MAX_EXP, LDBL_MIN_10_EXP,
         LDBL_MAX_10_EXP);
}

int main(void) {
  long double x = 1.5L;
  long double y = 4.5L;
  printf("%d\n", truncate_long_double(x + y));
  printf("%d\n", truncate_long_double(mix_long_double(3.0L, 5.0L)));
  printf("%d\n", truncate_long_double((long double)7 / 2.0L));

  check_int_casts();
  check_i128_casts();
  check_bitint_casts();
  check_math_functions();
  check_remaining_math_functions();

  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct LongDouble([u8; 10]);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn add(self, o: LongDouble) -> LongDouble { __slate_f80_add(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn sub(self, o: LongDouble) -> LongDouble { __slate_f80_sub(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn mul(self, o: LongDouble) -> LongDouble { __slate_f80_mul(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn div(self, o: LongDouble) -> LongDouble { __slate_f80_div(self, o) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-NEXT:     fn add_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_add(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-NEXT:     fn sub_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_sub(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-NEXT:     fn mul_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_mul(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-NEXT:     fn div_assign(&mut self, o: LongDouble) { {
// LOWERING-NEXT:     *self = __slate_f80_div(*self, o);
// LOWERING-NEXT: } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn neg(self) -> LongDouble { __slate_f80_neg(self) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-NEXT:     fn eq(&self, other: &LongDouble) -> bool { __slate_f80_eq(*self, *other) }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> { if __slate_f80_lt(*self, *other) { Some(std::cmp::Ordering::Less) } else { if __slate_f80_gt(*self, *other) { Some(std::cmp::Ordering::Greater) } else { if __slate_f80_eq(*self, *other) { Some(std::cmp::Ordering::Equal) } else { None } } } }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn abort() -> !;
// LOWERING-NEXT:     fn sqrtl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn cbrtl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn sinl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn cosl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn tanl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn asinl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn acosl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn atanl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn atan2l(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn sinhl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn coshl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn tanhl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn expl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn exp2l(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn logl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn log2l(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn log10l(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn powl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn fmodl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn hypotl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn ldexpl(_0: LongDouble, _1: i32) -> LongDouble;
// LOWERING-NEXT:     fn frexpl(_0: LongDouble, _1: *mut i32) -> LongDouble;
// LOWERING-NEXT:     fn nanl(_0: *const i8) -> LongDouble;
// LOWERING-NEXT:     fn remainderl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn remquol(_0: LongDouble, _1: LongDouble, _2: *mut i32) -> LongDouble;
// LOWERING-NEXT:     fn scalbnl(_0: LongDouble, _1: i32) -> LongDouble;
// LOWERING-NEXT:     fn scalblnl(_0: LongDouble, _1: i64) -> LongDouble;
// LOWERING-NEXT:     fn nextafterl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn nexttowardl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn fdiml(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn lrintl(_0: LongDouble) -> i64;
// LOWERING-NEXT:     fn llrintl(_0: LongDouble) -> i64;
// LOWERING-NEXT:     fn lroundl(_0: LongDouble) -> i64;
// LOWERING-NEXT:     fn llroundl(_0: LongDouble) -> i64;
// LOWERING-NEXT:     fn ilogbl(_0: LongDouble) -> i32;
// LOWERING-NEXT:     fn logbl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn erfl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn erfcl(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn tgammal(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn lgammal(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn canonicalizel(_0: *mut LongDouble, _1: *const LongDouble) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn truncate_long_double(arg4: LongDouble) -> i32 {
// LOWERING-NEXT:     let mut value: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     value = arg4;
// LOWERING-NEXT:     let _v0: LongDouble = value;
// LOWERING-NEXT:     let _v1: i32 = __slate_f80_to_i32(_v0);
// LOWERING-NEXT:     __retval = _v1;
// LOWERING-NEXT:     let _v2: i32 = __retval;
// LOWERING-NEXT:     return _v2;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mix_long_double(arg2: LongDouble, arg3: LongDouble) -> LongDouble {
// LOWERING-NEXT:     let mut a: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut b: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut __retval: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut c: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     a = arg2;
// LOWERING-NEXT:     b = arg3;
// LOWERING-NEXT:     let _v0: LongDouble = a;
// LOWERING-NEXT:     let _v1: LongDouble = b;
// LOWERING-NEXT:     let _v2: LongDouble = _v0 + _v1;
// LOWERING-NEXT:     let _v3: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let _v4: LongDouble = _v2 / _v3;
// LOWERING-NEXT:     c = _v4;
// LOWERING-NEXT:     let _v5: LongDouble = c;
// LOWERING-NEXT:     let _v6: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let _v7: LongDouble = _v5 * _v6;
// LOWERING-NEXT:     __retval = _v7;
// LOWERING-NEXT:     let _v8: LongDouble = __retval;
// LOWERING-NEXT:     return _v8;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_int_casts() {
// LOWERING-NEXT:     let mut i8: i8 = 0;
// LOWERING-NEXT:     let mut u8: u8 = 0;
// LOWERING-NEXT:     let mut i16: i16 = 0;
// LOWERING-NEXT:     let mut u16: u16 = 0;
// LOWERING-NEXT:     let mut i32: i32 = 0;
// LOWERING-NEXT:     let mut u32: u32 = 0;
// LOWERING-NEXT:     let mut i64: i64 = 0;
// LOWERING-NEXT:     let mut u64: u64 = 0;
// LOWERING-NEXT:     let _v0: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// LOWERING-NEXT:     let _v1: i8 = __slate_f80_to_i8(_v0);
// LOWERING-NEXT:     i8 = _v1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v2: i8 = i8;
// LOWERING-NEXT:         let _v3: LongDouble = __slate_f80_from_i8(_v2);
// LOWERING-NEXT:         let _v4: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// LOWERING-NEXT:         let _v5: bool = _v3 != _v4;
// LOWERING-NEXT:         if _v5 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// LOWERING-NEXT:     let _v7: u8 = __slate_f80_to_u8(_v6);
// LOWERING-NEXT:     u8 = _v7;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v8: u8 = u8;
// LOWERING-NEXT:         let _v9: LongDouble = __slate_f80_from_u8(_v8);
// LOWERING-NEXT:         let _v10: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// LOWERING-NEXT:         let _v11: bool = _v9 != _v10;
// LOWERING-NEXT:         if _v11 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// LOWERING-NEXT:     let _v13: i16 = __slate_f80_to_i16(_v12);
// LOWERING-NEXT:     i16 = _v13;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v14: i16 = i16;
// LOWERING-NEXT:         let _v15: LongDouble = __slate_f80_from_i16(_v14);
// LOWERING-NEXT:         let _v16: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// LOWERING-NEXT:         let _v17: bool = _v15 != _v16;
// LOWERING-NEXT:         if _v17 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v18: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]);
// LOWERING-NEXT:     let _v19: u16 = __slate_f80_to_u16(_v18);
// LOWERING-NEXT:     u16 = _v19;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v20: u16 = u16;
// LOWERING-NEXT:         let _v21: LongDouble = __slate_f80_from_u16(_v20);
// LOWERING-NEXT:         let _v22: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]);
// LOWERING-NEXT:         let _v23: bool = _v21 != _v22;
// LOWERING-NEXT:         if _v23 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v24: LongDouble = LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]);
// LOWERING-NEXT:     let _v25: i32 = __slate_f80_to_i32(_v24);
// LOWERING-NEXT:     i32 = _v25;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v26: i32 = i32;
// LOWERING-NEXT:         let _v27: LongDouble = __slate_f80_from_i32(_v26);
// LOWERING-NEXT:         let _v28: LongDouble = LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]);
// LOWERING-NEXT:         let _v29: bool = _v27 != _v28;
// LOWERING-NEXT:         if _v29 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v30: LongDouble = LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]);
// LOWERING-NEXT:     let _v31: u32 = __slate_f80_to_u32(_v30);
// LOWERING-NEXT:     u32 = _v31;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v32: u32 = u32;
// LOWERING-NEXT:         let _v33: LongDouble = __slate_f80_from_u32(_v32);
// LOWERING-NEXT:         let _v34: LongDouble = LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]);
// LOWERING-NEXT:         let _v35: bool = _v33 != _v34;
// LOWERING-NEXT:         if _v35 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v36: i64 = -123456789012345i64;
// LOWERING-NEXT:     i64 = _v36;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v37: i64 = i64;
// LOWERING-NEXT:         let _v38: LongDouble = __slate_f80_from_i64(_v37);
// LOWERING-NEXT:         let _v39: LongDouble = LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// LOWERING-NEXT:         let _v40: bool = _v38 != _v39;
// LOWERING-NEXT:         if _v40 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v41: LongDouble = LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]);
// LOWERING-NEXT:     let _v42: u64 = __slate_f80_to_u64(_v41);
// LOWERING-NEXT:     u64 = _v42;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v43: u64 = u64;
// LOWERING-NEXT:         let _v44: LongDouble = __slate_f80_from_u64(_v43);
// LOWERING-NEXT:         let _v45: LongDouble = LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]);
// LOWERING-NEXT:         let _v46: bool = _v44 != _v45;
// LOWERING-NEXT:         if _v46 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v47: *mut i8 = b"i8=%d u8=%u i16=%d u16=%u i32=%d u32=%u i64=%lld u64=%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v48: i8 = i8;
// LOWERING-NEXT:     let _v49: i32 = _v48 as i32;
// LOWERING-NEXT:     let _v50: u8 = u8;
// LOWERING-NEXT:     let _v51: i32 = _v50 as i32;
// LOWERING-NEXT:     let _v52: i16 = i16;
// LOWERING-NEXT:     let _v53: i32 = _v52 as i32;
// LOWERING-NEXT:     let _v54: u16 = u16;
// LOWERING-NEXT:     let _v55: i32 = _v54 as i32;
// LOWERING-NEXT:     let _v56: i32 = i32;
// LOWERING-NEXT:     let _v57: u32 = u32;
// LOWERING-NEXT:     let _v58: i64 = i64;
// LOWERING-NEXT:     let _v59: u64 = u64;
// LOWERING-NEXT:     let _v60: i32 = unsafe { printf(_v47 as *const i8, _v49, _v51, _v53, _v55, _v56, _v57, _v58, _v59) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_i128_casts() {
// LOWERING-NEXT:     let mut i128: i128 = 0;
// LOWERING-NEXT:     let mut u128: u128 = 0;
// LOWERING-NEXT:     let _v0: LongDouble = LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192]);
// LOWERING-NEXT:     let _v1: i128 = __slate_f80_to_i128(_v0);
// LOWERING-NEXT:     i128 = _v1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v2: i128 = i128;
// LOWERING-NEXT:         let _v3: LongDouble = __slate_f80_from_i128(_v2);
// LOWERING-NEXT:         let _v4: LongDouble = LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192]);
// LOWERING-NEXT:         let _v5: bool = _v3 != _v4;
// LOWERING-NEXT:         if _v5 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]);
// LOWERING-NEXT:     let _v7: u128 = __slate_f80_to_u128(_v6);
// LOWERING-NEXT:     u128 = _v7;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v8: u128 = u128;
// LOWERING-NEXT:         let _v9: LongDouble = __slate_f80_from_u128(_v8);
// LOWERING-NEXT:         let _v10: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]);
// LOWERING-NEXT:         let _v11: bool = _v9 != _v10;
// LOWERING-NEXT:         if _v11 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: *mut i8 = b"i128=%lld u128_hi=%llu u128_lo=%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: i128 = i128;
// LOWERING-NEXT:     let _v14: i64 = _v13 as i64;
// LOWERING-NEXT:     let _v15: u128 = u128;
// LOWERING-NEXT:     let _v16: i32 = 64;
// LOWERING-NEXT:     let _v17: u128 = _v15 >> _v16;
// LOWERING-NEXT:     let _v18: u64 = _v17 as u64;
// LOWERING-NEXT:     let _v19: u128 = u128;
// LOWERING-NEXT:     let _v20: u128 = 18446744073709551615u128;
// LOWERING-NEXT:     let _v21: u128 = _v19 & _v20;
// LOWERING-NEXT:     let _v22: u64 = _v21 as u64;
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v12 as *const i8, _v14, _v18, _v22) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_bitint_casts() {
// LOWERING-NEXT:     let mut b9: aligned::Aligned<aligned::A2, bitint::BInt<9, 1, 2>> = aligned::Aligned(bitint::BInt::<9, 1, 2>::ZERO);
// LOWERING-NEXT:     let mut ub9: aligned::Aligned<aligned::A2, bitint::BUint<9, 1, 2>> = aligned::Aligned(bitint::BUint::<9, 1, 2>::ZERO);
// LOWERING-NEXT:     let mut b40: aligned::Aligned<aligned::A8, bitint::BInt<40, 1, 8>> = aligned::Aligned(bitint::BInt::<40, 1, 8>::ZERO);
// LOWERING-NEXT:     let mut ub40: aligned::Aligned<aligned::A8, bitint::BUint<40, 1, 8>> = aligned::Aligned(bitint::BUint::<40, 1, 8>::ZERO);
// LOWERING-NEXT:     let mut b101: aligned::Aligned<aligned::A8, bitint::BInt<101, 2, 16>> = aligned::Aligned(bitint::BInt::<101, 2, 16>::ZERO);
// LOWERING-NEXT:     let mut ub150: aligned::Aligned<aligned::A8, bitint::BUint<150, 3, 24>> = aligned::Aligned(bitint::BUint::<150, 3, 24>::ZERO);
// LOWERING-NEXT:     let mut b256: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> = aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut ub300: aligned::Aligned<aligned::A8, bitint::BUint<300, 5, 40>> = aligned::Aligned(bitint::BUint::<300, 5, 40>::ZERO);
// LOWERING-NEXT:     let mut b129: aligned::Aligned<aligned::A8, bitint::BInt<129, 3, 24>> = aligned::Aligned(bitint::BInt::<129, 3, 24>::ZERO);
// LOWERING-NEXT:     let _v0: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// LOWERING-NEXT:     let _v1: bitint::BInt<9, 1, 2> = bitint::BInt::<9, 1, 2>::from_i128(__slate_f80_to_i128(_v0) as i128);
// LOWERING-NEXT:     *b9 = _v1;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v2: bitint::BInt<9, 1, 2> = *b9;
// LOWERING-NEXT:         let _v3: LongDouble = __slate_f80_from_i128(_v2.to_i128());
// LOWERING-NEXT:         let _v4: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// LOWERING-NEXT:         let _v5: bool = _v3 != _v4;
// LOWERING-NEXT:         if _v5 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v6: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// LOWERING-NEXT:     let _v7: bitint::BUint<9, 1, 2> = bitint::BUint::<9, 1, 2>::from_u128(__slate_f80_to_u128(_v6) as u128);
// LOWERING-NEXT:     *ub9 = _v7;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v8: bitint::BUint<9, 1, 2> = *ub9;
// LOWERING-NEXT:         let _v9: LongDouble = __slate_f80_from_u128(_v8.to_u128());
// LOWERING-NEXT:         let _v10: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// LOWERING-NEXT:         let _v11: bool = _v9 != _v10;
// LOWERING-NEXT:         if _v11 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v12: LongDouble = LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]);
// LOWERING-NEXT:     let _v13: bitint::BInt<40, 1, 8> = bitint::BInt::<40, 1, 8>::from_i128(__slate_f80_to_i128(_v12) as i128);
// LOWERING-NEXT:     *b40 = _v13;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v14: bitint::BInt<40, 1, 8> = *b40;
// LOWERING-NEXT:         let _v15: LongDouble = __slate_f80_from_i128(_v14.to_i128());
// LOWERING-NEXT:         let _v16: LongDouble = LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]);
// LOWERING-NEXT:         let _v17: bool = _v15 != _v16;
// LOWERING-NEXT:         if _v17 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v18: LongDouble = LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]);
// LOWERING-NEXT:     let _v19: bitint::BUint<40, 1, 8> = bitint::BUint::<40, 1, 8>::from_u128(__slate_f80_to_u128(_v18) as u128);
// LOWERING-NEXT:     *ub40 = _v19;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v20: bitint::BUint<40, 1, 8> = *ub40;
// LOWERING-NEXT:         let _v21: LongDouble = __slate_f80_from_u128(_v20.to_u128());
// LOWERING-NEXT:         let _v22: LongDouble = LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]);
// LOWERING-NEXT:         let _v23: bool = _v21 != _v22;
// LOWERING-NEXT:         if _v23 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v24: LongDouble = LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// LOWERING-NEXT:     let _v25: bitint::BInt<101, 2, 16> = bitint::BInt::<101, 2, 16>::from_i128(__slate_f80_to_i128(_v24) as i128);
// LOWERING-NEXT:     *b101 = _v25;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v26: bitint::BInt<101, 2, 16> = *b101;
// LOWERING-NEXT:         let _v27: LongDouble = __slate_f80_from_i128(_v26.to_i128());
// LOWERING-NEXT:         let _v28: LongDouble = LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// LOWERING-NEXT:         let _v29: bool = _v27 != _v28;
// LOWERING-NEXT:         if _v29 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v30: bitint::BInt<101, 2, 16> = *b101;
// LOWERING-NEXT:         let _v31: LongDouble = __slate_f80_from_i128(_v30.to_i128());
// LOWERING-NEXT:         let _v32: bitint::BInt<101, 2, 16> = bitint::BInt::<101, 2, 16>::from_i128(__slate_f80_to_i128(_v31) as i128);
// LOWERING-NEXT:         let _v33: bitint::BInt<101, 2, 16> = *b101;
// LOWERING-NEXT:         let _v34: bool = _v32 != _v33;
// LOWERING-NEXT:         if _v34 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v35: LongDouble = LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]);
// LOWERING-NEXT:     let _v36: bitint::BUint<150, 3, 24> = bitint::BUint::<150, 3, 24>::from_u128(__slate_f80_to_u128(_v35) as u128);
// LOWERING-NEXT:     *ub150 = _v36;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v37: bitint::BUint<150, 3, 24> = *ub150;
// LOWERING-NEXT:         let _v38: LongDouble = __slate_f80_from_u128(_v37.to_u128());
// LOWERING-NEXT:         let _v39: LongDouble = LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]);
// LOWERING-NEXT:         let _v40: bool = _v38 != _v39;
// LOWERING-NEXT:         if _v40 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v41: bitint::BUint<150, 3, 24> = *ub150;
// LOWERING-NEXT:         let _v42: LongDouble = __slate_f80_from_u128(_v41.to_u128());
// LOWERING-NEXT:         let _v43: bitint::BUint<150, 3, 24> = bitint::BUint::<150, 3, 24>::from_u128(__slate_f80_to_u128(_v42) as u128);
// LOWERING-NEXT:         let _v44: bitint::BUint<150, 3, 24> = *ub150;
// LOWERING-NEXT:         let _v45: bool = _v43 != _v44;
// LOWERING-NEXT:         if _v45 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v46: LongDouble = LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]);
// LOWERING-NEXT:     let _v47: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_i128(__slate_f80_to_i128(_v46) as i128);
// LOWERING-NEXT:     *b256 = _v47;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v48: bitint::BInt<256, 4, 32> = *b256;
// LOWERING-NEXT:         let _v49: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_decimal_str("9999999999");
// LOWERING-NEXT:         let _v50: bool = _v48 != _v49;
// LOWERING-NEXT:         if _v50 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v51: bitint::BInt<256, 4, 32> = *b256;
// LOWERING-NEXT:         let _v52: LongDouble = __slate_f80_from_i128(_v51.to_i128());
// LOWERING-NEXT:         let _v53: LongDouble = LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]);
// LOWERING-NEXT:         let _v54: bool = _v52 != _v53;
// LOWERING-NEXT:         if _v54 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v55: LongDouble = LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]);
// LOWERING-NEXT:     let _v56: bitint::BUint<300, 5, 40> = bitint::BUint::<300, 5, 40>::from_u128(__slate_f80_to_u128(_v55) as u128);
// LOWERING-NEXT:     *ub300 = _v56;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v57: bitint::BUint<300, 5, 40> = *ub300;
// LOWERING-NEXT:         let _v58: bitint::BUint<300, 5, 40> = bitint::BUint::<300, 5, 40>::from_decimal_str("4200000000");
// LOWERING-NEXT:         let _v59: bool = _v57 != _v58;
// LOWERING-NEXT:         if _v59 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v60: bitint::BUint<300, 5, 40> = *ub300;
// LOWERING-NEXT:         let _v61: LongDouble = __slate_f80_from_u128(_v60.to_u128());
// LOWERING-NEXT:         let _v62: LongDouble = LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]);
// LOWERING-NEXT:         let _v63: bool = _v61 != _v62;
// LOWERING-NEXT:         if _v63 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v64: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]);
// LOWERING-NEXT:     let _v65: bitint::BInt<129, 3, 24> = bitint::BInt::<129, 3, 24>::from_i128(__slate_f80_to_i128(_v64) as i128);
// LOWERING-NEXT:     *b129 = _v65;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v66: bitint::BInt<129, 3, 24> = *b129;
// LOWERING-NEXT:         let _v67: i32 = _v66.to_i128() as i32;
// LOWERING-NEXT:         let _v68: i32 = 123;
// LOWERING-NEXT:         let _v69: bool = _v67 != _v68;
// LOWERING-NEXT:         if _v69 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v70: bitint::BInt<129, 3, 24> = bitint::BInt::<129, 3, 24>::from_decimal_str("123");
// LOWERING-NEXT:         let _v71: LongDouble = __slate_f80_from_i128(_v70.to_i128());
// LOWERING-NEXT:         let _v72: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]);
// LOWERING-NEXT:         let _v73: bool = _v71 != _v72;
// LOWERING-NEXT:         if _v73 {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v74: *mut i8 = b"bitint_b101=%lld bitint_ub150=%llu bitint_b256_lo=%lld bitint_ub300_lo=%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v75: bitint::BInt<101, 2, 16> = *b101;
// LOWERING-NEXT:     let _v76: i64 = _v75.to_i128() as i64;
// LOWERING-NEXT:     let _v77: bitint::BUint<150, 3, 24> = *ub150;
// LOWERING-NEXT:     let _v78: u64 = _v77.to_u128() as u64;
// LOWERING-NEXT:     let _v79: bitint::BInt<256, 4, 32> = *b256;
// LOWERING-NEXT:     let _v80: i64 = _v79.to_i128() as i64;
// LOWERING-NEXT:     let _v81: bitint::BUint<300, 5, 40> = *ub300;
// LOWERING-NEXT:     let _v82: u64 = _v81.to_u128() as u64;
// LOWERING-NEXT:     let _v83: i32 = unsafe { printf(_v74 as *const i8, _v76, _v78, _v80, _v82) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn print_ld(arg0: *mut i8, arg1: LongDouble) {
// LOWERING-NEXT:     let mut name: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut v: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     name = arg0;
// LOWERING-NEXT:     v = arg1;
// LOWERING-NEXT:     let _v0: *mut i8 = b"%s=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v1: *mut i8 = name;
// LOWERING-NEXT:     let _v2: LongDouble = v;
// LOWERING-NEXT:     let _v3: i32 = unsafe { __slate_printf__ri32_pi8_pi8_f80(_v0 as *mut i8, _v1 as *mut i8, _v2) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_math_functions() {
// LOWERING-NEXT:     let mut exp: i32 = 0;
// LOWERING-NEXT:     let _v0: *mut i8 = b"sqrt\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v1: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let _v2: LongDouble = unsafe { __slate_sqrtl__rf80_f80(_v1) };
// LOWERING-NEXT:     print_ld(_v0, _v2);
// LOWERING-NEXT:     let _v3: *mut i8 = b"cbrt\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 216, 3, 64]);
// LOWERING-NEXT:     let _v5: LongDouble = unsafe { __slate_cbrtl__rf80_f80(_v4) };
// LOWERING-NEXT:     print_ld(_v3, _v5);
// LOWERING-NEXT:     let _v6: *mut i8 = b"sin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let _v8: LongDouble = unsafe { __slate_sinl__rf80_f80(_v7) };
// LOWERING-NEXT:     print_ld(_v6, _v8);
// LOWERING-NEXT:     let _v9: *mut i8 = b"cos\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let _v11: LongDouble = unsafe { __slate_cosl__rf80_f80(_v10) };
// LOWERING-NEXT:     print_ld(_v9, _v11);
// LOWERING-NEXT:     let _v12: *mut i8 = b"tan\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let _v14: LongDouble = unsafe { __slate_tanl__rf80_f80(_v13) };
// LOWERING-NEXT:     print_ld(_v12, _v14);
// LOWERING-NEXT:     let _v15: *mut i8 = b"asin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v17: LongDouble = unsafe { __slate_asinl__rf80_f80(_v16) };
// LOWERING-NEXT:     print_ld(_v15, _v17);
// LOWERING-NEXT:     let _v18: *mut i8 = b"acos\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v20: LongDouble = unsafe { __slate_acosl__rf80_f80(_v19) };
// LOWERING-NEXT:     print_ld(_v18, _v20);
// LOWERING-NEXT:     let _v21: *mut i8 = b"atan\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v22: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v23: LongDouble = unsafe { __slate_atanl__rf80_f80(_v22) };
// LOWERING-NEXT:     print_ld(_v21, _v23);
// LOWERING-NEXT:     let _v24: *mut i8 = b"atan2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v26: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v27: LongDouble = unsafe { __slate_atan2l__rf80_f80_f80(_v25, _v26) };
// LOWERING-NEXT:     print_ld(_v24, _v27);
// LOWERING-NEXT:     let _v28: *mut i8 = b"sinh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v29: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v30: LongDouble = unsafe { __slate_sinhl__rf80_f80(_v29) };
// LOWERING-NEXT:     print_ld(_v28, _v30);
// LOWERING-NEXT:     let _v31: *mut i8 = b"cosh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v32: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v33: LongDouble = unsafe { __slate_coshl__rf80_f80(_v32) };
// LOWERING-NEXT:     print_ld(_v31, _v33);
// LOWERING-NEXT:     let _v34: *mut i8 = b"tanh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v35: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v36: LongDouble = unsafe { __slate_tanhl__rf80_f80(_v35) };
// LOWERING-NEXT:     print_ld(_v34, _v36);
// LOWERING-NEXT:     let _v37: *mut i8 = b"exp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v38: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v39: LongDouble = unsafe { __slate_expl__rf80_f80(_v38) };
// LOWERING-NEXT:     print_ld(_v37, _v39);
// LOWERING-NEXT:     let _v40: *mut i8 = b"exp2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v41: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-NEXT:     let _v42: LongDouble = unsafe { __slate_exp2l__rf80_f80(_v41) };
// LOWERING-NEXT:     print_ld(_v40, _v42);
// LOWERING-NEXT:     let _v43: *mut i8 = b"log\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v44: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v45: LongDouble = unsafe { __slate_expl__rf80_f80(_v44) };
// LOWERING-NEXT:     let _v46: LongDouble = unsafe { __slate_logl__rf80_f80(_v45) };
// LOWERING-NEXT:     print_ld(_v43, _v46);
// LOWERING-NEXT:     let _v47: *mut i8 = b"log2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v48: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// LOWERING-NEXT:     let _v49: LongDouble = unsafe { __slate_log2l__rf80_f80(_v48) };
// LOWERING-NEXT:     print_ld(_v47, _v49);
// LOWERING-NEXT:     let _v50: *mut i8 = b"log10\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v51: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 250, 8, 64]);
// LOWERING-NEXT:     let _v52: LongDouble = unsafe { __slate_log10l__rf80_f80(_v51) };
// LOWERING-NEXT:     print_ld(_v50, _v52);
// LOWERING-NEXT:     let _v53: *mut i8 = b"pow\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v54: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let _v55: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-NEXT:     let _v56: LongDouble = unsafe { __slate_powl__rf80_f80_f80(_v54, _v55) };
// LOWERING-NEXT:     print_ld(_v53, _v56);
// LOWERING-NEXT:     let _v57: *mut i8 = b"floor\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v58: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 64]);
// LOWERING-NEXT:     let _v59: LongDouble = __slate_f80_floor(_v58);
// LOWERING-NEXT:     print_ld(_v57, _v59);
// LOWERING-NEXT:     let _v60: *mut i8 = b"ceil\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v61: LongDouble = LongDouble([102, 102, 102, 102, 102, 102, 102, 134, 0, 64]);
// LOWERING-NEXT:     let _v62: LongDouble = __slate_f80_ceil(_v61);
// LOWERING-NEXT:     print_ld(_v60, _v62);
// LOWERING-NEXT:     let _v63: *mut i8 = b"round\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v64: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]);
// LOWERING-NEXT:     let _v65: LongDouble = __slate_f80_round(_v64);
// LOWERING-NEXT:     print_ld(_v63, _v65);
// LOWERING-NEXT:     let _v66: *mut i8 = b"trunc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v67: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 192]);
// LOWERING-NEXT:     let _v68: LongDouble = __slate_f80_trunc(_v67);
// LOWERING-NEXT:     print_ld(_v66, _v68);
// LOWERING-NEXT:     let _v69: *mut i8 = b"fabs\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v70: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 192]);
// LOWERING-NEXT:     let _v71: LongDouble = __slate_f80_abs(_v70);
// LOWERING-NEXT:     print_ld(_v69, _v71);
// LOWERING-NEXT:     let _v72: *mut i8 = b"fmod\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v73: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-NEXT:     let _v74: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let _v75: LongDouble = unsafe { __slate_fmodl__rf80_f80_f80(_v73, _v74) };
// LOWERING-NEXT:     print_ld(_v72, _v75);
// LOWERING-NEXT:     let _v76: *mut i8 = b"hypot\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v77: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let _v78: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// LOWERING-NEXT:     let _v79: LongDouble = unsafe { __slate_hypotl__rf80_f80_f80(_v77, _v78) };
// LOWERING-NEXT:     print_ld(_v76, _v79);
// LOWERING-NEXT:     let _v80: *mut i8 = b"copysign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v81: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let _v82: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// LOWERING-NEXT:     let _v83: LongDouble = __slate_f80_copysign(_v81, _v82);
// LOWERING-NEXT:     print_ld(_v80, _v83);
// LOWERING-NEXT:     let _v84: *mut i8 = b"fmax\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v85: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v86: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let _v87: LongDouble = __slate_f80_fmax(_v85, _v86);
// LOWERING-NEXT:     print_ld(_v84, _v87);
// LOWERING-NEXT:     let _v88: *mut i8 = b"fmin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v89: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v90: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let _v91: LongDouble = __slate_f80_fmin(_v89, _v90);
// LOWERING-NEXT:     print_ld(_v88, _v91);
// LOWERING-NEXT:     let _v92: *mut i8 = b"fma\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v93: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let _v94: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let _v95: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// LOWERING-NEXT:     let _v96: LongDouble = __slate_f80_fma(_v93, _v94, _v95);
// LOWERING-NEXT:     print_ld(_v92, _v96);
// LOWERING-NEXT:     let _v97: *mut i8 = b"ldexp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v98: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let _v99: i32 = 4;
// LOWERING-NEXT:     let _v100: LongDouble = unsafe { __slate_ldexpl__rf80_f80_i32(_v98, _v99 as i32) };
// LOWERING-NEXT:     print_ld(_v97, _v100);
// LOWERING-NEXT:     let _v101: i32 = 0;
// LOWERING-NEXT:     exp = _v101;
// LOWERING-NEXT:     let _v102: *mut i8 = b"frexp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v103: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64]);
// LOWERING-NEXT:     let _v104: LongDouble = unsafe { __slate_frexpl__rf80_f80_pi32(_v103, std::ptr::addr_of_mut!(exp) as *mut i32) };
// LOWERING-NEXT:     print_ld(_v102, _v104);
// LOWERING-NEXT:     let _v105: *mut i8 = b"frexp_exp=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v106: i32 = exp;
// LOWERING-NEXT:     let _v107: i32 = unsafe { printf(_v105 as *const i8, _v106) };
// LOWERING-NEXT:     let _v108: *mut i8 = b"isnan=%d isinf=%d signbit_neg=%d signbit_pos=%d isfinite=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v109: *mut i8 = b"\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v110: LongDouble = unsafe { __slate_nanl__rf80_pi8(_v109 as *const i8) };
// LOWERING-NEXT:     let _v111: bool = __slate_f80_is_fp_class(_v110, 3);
// LOWERING-NEXT:     let _v112: i32 = _v111 as i32;
// LOWERING-NEXT:     let _v113: i32 = 1;
// LOWERING-NEXT:     let _v114: i32 = 1;
// LOWERING-NEXT:     let _v115: i32 = 0;
// LOWERING-NEXT:     let _v116: i32 = 1;
// LOWERING-NEXT:     let _v117: i32 = unsafe { printf(_v108 as *const i8, _v112, _v113, _v114, _v115, _v116) };
// LOWERING-NEXT:     let _v118: *mut i8 = b"epsilon\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v119: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// LOWERING-NEXT:     print_ld(_v118, _v119);
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: /// The functions above all round-trip through the generic call-shim (any
// LOWERING-NEXT: /// known extern function with a long double arg/return links straight to
// LOWERING-NEXT: /// libm), which check_math_functions already exercises. This covers the
// LOWERING-NEXT: /// remaining libm entry points -- pointer out-params, integer-returning
// LOWERING-NEXT: /// variants, and the classification family -- with volatile operands so
// LOWERING-NEXT: /// they can't constant-fold away and skip the real runtime path.
// LOWERING-NEXT: fn check_remaining_math_functions() {
// LOWERING-NEXT:     let mut ten: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut three: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut ipart: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut frac: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut quo: i32 = 0;
// LOWERING-NEXT:     let mut vnan: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vinf: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vzero: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vone: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vsub: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vtwo: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut ten_plain: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut canon: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut canon_r: i32 = 0;
// LOWERING-NEXT:     let _v0: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(ten), _v0) };
// LOWERING-NEXT:     let _v1: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(three), _v1) };
// LOWERING-NEXT:     let _v2: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     ipart = _v2;
// LOWERING-NEXT:     let _v3: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v4: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v5: LongDouble = _v3 / _v4;
// LOWERING-NEXT:     let _v6: LongDouble = __slate_f80_fract(_v5);
// LOWERING-NEXT:     let _v7: LongDouble = __slate_f80_trunc(_v5);
// LOWERING-NEXT:     ipart = _v7;
// LOWERING-NEXT:     frac = _v6;
// LOWERING-NEXT:     let _v8: *mut i8 = b"modf_ipart\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v9: LongDouble = ipart;
// LOWERING-NEXT:     print_ld(_v8, _v9);
// LOWERING-NEXT:     let _v10: *mut i8 = b"modf_frac\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: LongDouble = frac;
// LOWERING-NEXT:     print_ld(_v10, _v11);
// LOWERING-NEXT:     let _v12: *mut i8 = b"remainder\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v13: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v14: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v15: LongDouble = unsafe { __slate_remainderl__rf80_f80_f80(_v13, _v14) };
// LOWERING-NEXT:     print_ld(_v12, _v15);
// LOWERING-NEXT:     let _v16: i32 = 0;
// LOWERING-NEXT:     quo = _v16;
// LOWERING-NEXT:     let _v17: *mut i8 = b"remquo\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v18: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v19: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v20: LongDouble = unsafe { __slate_remquol__rf80_f80_f80_pi32(_v18, _v19, std::ptr::addr_of_mut!(quo) as *mut i32) };
// LOWERING-NEXT:     print_ld(_v17, _v20);
// LOWERING-NEXT:     let _v21: *mut i8 = b"remquo_quo=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v22: i32 = quo;
// LOWERING-NEXT:     let _v23: i32 = unsafe { printf(_v21 as *const i8, _v22) };
// LOWERING-NEXT:     let _v24: *mut i8 = b"scalbn\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v25: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v26: i32 = 3;
// LOWERING-NEXT:     let _v27: LongDouble = unsafe { __slate_scalbnl__rf80_f80_i32(_v25, _v26 as i32) };
// LOWERING-NEXT:     print_ld(_v24, _v27);
// LOWERING-NEXT:     let _v28: *mut i8 = b"scalbln\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v29: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v30: i64 = 3;
// LOWERING-NEXT:     let _v31: LongDouble = unsafe { __slate_scalblnl__rf80_f80_i64(_v29, _v30 as i64) };
// LOWERING-NEXT:     print_ld(_v28, _v31);
// LOWERING-NEXT:     let _v32: *mut i8 = b"nextafter\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v33: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v34: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v35: LongDouble = unsafe { __slate_nextafterl__rf80_f80_f80(_v33, _v34) };
// LOWERING-NEXT:     print_ld(_v32, _v35);
// LOWERING-NEXT:     let _v36: *mut i8 = b"nexttoward\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v37: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v38: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v39: LongDouble = unsafe { __slate_nexttowardl__rf80_f80_f80(_v37, _v38) };
// LOWERING-NEXT:     print_ld(_v36, _v39);
// LOWERING-NEXT:     let _v40: *mut i8 = b"fdim\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v41: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v42: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v43: LongDouble = unsafe { __slate_fdiml__rf80_f80_f80(_v41, _v42) };
// LOWERING-NEXT:     print_ld(_v40, _v43);
// LOWERING-NEXT:     let _v44: *mut i8 = b"rint\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v45: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v46: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v47: LongDouble = _v45 / _v46;
// LOWERING-NEXT:     let _v48: LongDouble = __slate_f80_rint(_v47);
// LOWERING-NEXT:     print_ld(_v44, _v48);
// LOWERING-NEXT:     let _v49: *mut i8 = b"nearbyint\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v50: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v51: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v52: LongDouble = _v50 / _v51;
// LOWERING-NEXT:     let _v53: LongDouble = __slate_f80_rint(_v52);
// LOWERING-NEXT:     print_ld(_v49, _v53);
// LOWERING-NEXT:     let _v54: *mut i8 = b"lrint=%ld llrint=%lld lround=%ld llround=%lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v55: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v56: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v57: LongDouble = _v55 / _v56;
// LOWERING-NEXT:     let _v58: i64 = unsafe { __slate_lrintl__ri64_f80(_v57) };
// LOWERING-NEXT:     let _v59: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v60: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v61: LongDouble = _v59 / _v60;
// LOWERING-NEXT:     let _v62: i64 = unsafe { __slate_llrintl__ri64_f80(_v61) };
// LOWERING-NEXT:     let _v63: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v64: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v65: LongDouble = _v63 / _v64;
// LOWERING-NEXT:     let _v66: i64 = unsafe { __slate_lroundl__ri64_f80(_v65) };
// LOWERING-NEXT:     let _v67: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v68: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v69: LongDouble = _v67 / _v68;
// LOWERING-NEXT:     let _v70: i64 = unsafe { __slate_llroundl__ri64_f80(_v69) };
// LOWERING-NEXT:     let _v71: i32 = unsafe { printf(_v54 as *const i8, _v58, _v62, _v66, _v70) };
// LOWERING-NEXT:     let _v72: *mut i8 = b"ilogb=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v73: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v74: i32 = unsafe { __slate_ilogbl__ri32_f80(_v73) };
// LOWERING-NEXT:     let _v75: i32 = unsafe { printf(_v72 as *const i8, _v74) };
// LOWERING-NEXT:     let _v76: *mut i8 = b"logb\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v77: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v78: LongDouble = unsafe { __slate_logbl__rf80_f80(_v77) };
// LOWERING-NEXT:     print_ld(_v76, _v78);
// LOWERING-NEXT:     let _v79: *mut i8 = b"erf\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v80: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v81: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v82: LongDouble = _v80 / _v81;
// LOWERING-NEXT:     let _v83: LongDouble = unsafe { __slate_erfl__rf80_f80(_v82) };
// LOWERING-NEXT:     print_ld(_v79, _v83);
// LOWERING-NEXT:     let _v84: *mut i8 = b"erfc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v85: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v86: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v87: LongDouble = _v85 / _v86;
// LOWERING-NEXT:     let _v88: LongDouble = unsafe { __slate_erfcl__rf80_f80(_v87) };
// LOWERING-NEXT:     print_ld(_v84, _v88);
// LOWERING-NEXT:     let _v89: *mut i8 = b"tgamma\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v90: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let _v91: LongDouble = unsafe { __slate_tgammal__rf80_f80(_v90) };
// LOWERING-NEXT:     print_ld(_v89, _v91);
// LOWERING-NEXT:     let _v92: *mut i8 = b"lgamma\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v93: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let _v94: LongDouble = unsafe { __slate_lgammal__rf80_f80(_v93) };
// LOWERING-NEXT:     print_ld(_v92, _v94);
// LOWERING-NEXT:     let _v95: *mut i8 = b"\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v96: LongDouble = unsafe { __slate_nanl__rf80_pi8(_v95 as *const i8) };
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vnan), _v96) };
// LOWERING-NEXT:     let _v97: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vinf), _v97) };
// LOWERING-NEXT:     let _v98: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vzero), _v98) };
// LOWERING-NEXT:     let _v99: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vone), _v99) };
// LOWERING-NEXT:     let _v100: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vsub), _v100) };
// LOWERING-NEXT:     let _v101: *mut i8 = b"isnan_v=%d isinf_v=%d isfinite_v=%d isnormal_v=%d isunordered_v=%d isunordered_ok=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v102: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// LOWERING-NEXT:     let _v103: bool = __slate_f80_is_fp_class(_v102, 3);
// LOWERING-NEXT:     let _v104: i32 = _v103 as i32;
// LOWERING-NEXT:     let _v105: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vinf)) };
// LOWERING-NEXT:     let _v106: bool = __slate_f80_is_fp_class(_v105, 516);
// LOWERING-NEXT:     let _v107: i32 = _v106 as i32;
// LOWERING-NEXT:     let _v108: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let _v109: bool = __slate_f80_is_fp_class(_v108, 504);
// LOWERING-NEXT:     let _v110: i32 = _v109 as i32;
// LOWERING-NEXT:     let _v111: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let _v112: bool = __slate_f80_is_fp_class(_v111, 264);
// LOWERING-NEXT:     let _v113: i32 = _v112 as i32;
// LOWERING-NEXT:     let _v114: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// LOWERING-NEXT:     let _v115: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let _v116: bool = _v114 != _v114 || _v115 != _v115;
// LOWERING-NEXT:     let _v117: i32 = _v116 as i32;
// LOWERING-NEXT:     let _v118: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let _v119: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vzero)) };
// LOWERING-NEXT:     let _v120: bool = _v118 != _v118 || _v119 != _v119;
// LOWERING-NEXT:     let _v121: i32 = _v120 as i32;
// LOWERING-NEXT:     let _v122: i32 = unsafe { printf(_v101 as *const i8, _v104, _v107, _v110, _v113, _v117, _v121) };
// LOWERING-NEXT:     let _v123: *mut i8 = b"subnormal_isnormal=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v124: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vsub)) };
// LOWERING-NEXT:     let _v125: bool = __slate_f80_is_fp_class(_v124, 264);
// LOWERING-NEXT:     let _v126: i32 = _v125 as i32;
// LOWERING-NEXT:     let _v127: i32 = unsafe { printf(_v123 as *const i8, _v126) };
// LOWERING-NEXT:     let _v128: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vtwo), _v128) };
// LOWERING-NEXT:     let _v129: *mut i8 = b"islessgreater_lt=%d islessgreater_eq=%d islessgreater_nan=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v130: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let _v131: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vtwo)) };
// LOWERING-NEXT:     let _v132: bool = _v130 < _v131 || _v130 > _v131;
// LOWERING-NEXT:     let _v133: i32 = _v132 as i32;
// LOWERING-NEXT:     let _v134: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let _v135: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let _v136: bool = _v134 < _v135 || _v134 > _v135;
// LOWERING-NEXT:     let _v137: i32 = _v136 as i32;
// LOWERING-NEXT:     let _v138: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// LOWERING-NEXT:     let _v139: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let _v140: bool = _v138 < _v139 || _v138 > _v139;
// LOWERING-NEXT:     let _v141: i32 = _v140 as i32;
// LOWERING-NEXT:     let _v142: i32 = unsafe { printf(_v129 as *const i8, _v133, _v137, _v141) };
// LOWERING-NEXT:     let _v143: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     ten_plain = _v143;
// LOWERING-NEXT:     let _v144: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     canon = _v144;
// LOWERING-NEXT:     let _v145: i32 = unsafe { __slate_canonicalizel__ri32_pf80_pf80(std::ptr::addr_of_mut!(canon), std::ptr::addr_of_mut!(ten_plain)) };
// LOWERING-NEXT:     canon_r = _v145;
// LOWERING-NEXT:     let _v146: *mut i8 = b"canonicalize\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v147: LongDouble = canon;
// LOWERING-NEXT:     print_ld(_v146, _v147);
// LOWERING-NEXT:     let _v148: *mut i8 = b"canonicalize_r=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v149: i32 = canon_r;
// LOWERING-NEXT:     let _v150: i32 = unsafe { printf(_v148 as *const i8, _v149) };
// LOWERING-NEXT:     let _v151: *mut i8 = b"ldbl_min\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v152: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 0]);
// LOWERING-NEXT:     print_ld(_v151, _v152);
// LOWERING-NEXT:     let _v153: *mut i8 = b"ldbl_true_min\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v154: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     print_ld(_v153, _v154);
// LOWERING-NEXT:     let _v155: *mut i8 = b"ldbl_mant_dig=%d ldbl_dig=%d ldbl_min_exp=%d ldbl_max_exp=%d ldbl_min_10_exp=%d ldbl_max_10_exp=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v156: i32 = 64;
// LOWERING-NEXT:     let _v157: i32 = 18;
// LOWERING-NEXT:     let _v158: i32 = -16381;
// LOWERING-NEXT:     let _v159: i32 = 16384;
// LOWERING-NEXT:     let _v160: i32 = -4931;
// LOWERING-NEXT:     let _v161: i32 = 4932;
// LOWERING-NEXT:     let _v162: i32 = unsafe { printf(_v155 as *const i8, _v156, _v157, _v158, _v159, _v160, _v161) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut y: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]);
// LOWERING-NEXT:     x = _v1;
// LOWERING-NEXT:     let _v2: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// LOWERING-NEXT:     y = _v2;
// LOWERING-NEXT:     let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: LongDouble = x;
// LOWERING-NEXT:     let _v5: LongDouble = y;
// LOWERING-NEXT:     let _v6: LongDouble = _v4 + _v5;
// LOWERING-NEXT:     let _v7: i32 = truncate_long_double(_v6);
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v3 as *const i8, _v7) };
// LOWERING-NEXT:     let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v10: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let _v11: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:     let _v12: LongDouble = mix_long_double(_v10, _v11);
// LOWERING-NEXT:     let _v13: i32 = truncate_long_double(_v12);
// LOWERING-NEXT:     let _v14: i32 = unsafe { printf(_v9 as *const i8, _v13) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: i32 = 7;
// LOWERING-NEXT:     let _v17: LongDouble = __slate_f80_from_i32(_v16);
// LOWERING-NEXT:     let _v18: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let _v19: LongDouble = _v17 / _v18;
// LOWERING-NEXT:     let _v20: i32 = truncate_long_double(_v19);
// LOWERING-NEXT:     let _v21: i32 = unsafe { printf(_v15 as *const i8, _v20) };
// LOWERING-NEXT:     check_int_casts();
// LOWERING-NEXT:     check_i128_casts();
// LOWERING-NEXT:     check_bitint_casts();
// LOWERING-NEXT:     check_math_functions();
// LOWERING-NEXT:     check_remaining_math_functions();
// LOWERING-NEXT:     let _v22: i32 = 0;
// LOWERING-NEXT:     __retval = _v22;
// LOWERING-NEXT:     let _v23: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v23 as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __slate_acosl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_asinl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_atan2l__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_atanl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_canonicalizel__ri32_pf80_pf80(_0: *mut LongDouble, _1: *const LongDouble) -> i32;
// LOWERING-NEXT:     fn __slate_cbrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_cf80_div(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_cf80_mul(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn __slate_coshl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_erfcl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_erfl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_exp2l__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_expl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_abs(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_add(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ceil(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_copysign(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_div(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_eq(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_floor(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fma(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmax(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmin(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fract(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_bool(a: bool) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f32(a: f32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f64(a: f64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i128(a: i128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i16(a: i16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i32(a: i32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i64(a: i64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i8(a: i8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u128(a: u128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u16(a: u16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u32(a: u32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u64(a: u64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u8(a: u8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ge(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_gt(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_is_fp_class(a: LongDouble, flags: i32) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_le(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_lt(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_mul(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ne(a: LongDouble, b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_neg(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_rint(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_round(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_signbit(a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_sub(a: LongDouble, b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_to_bool(a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_to_f32(a: LongDouble) -> f32;
// LOWERING-NEXT:     safe fn __slate_f80_to_f64(a: LongDouble) -> f64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i128(a: LongDouble) -> i128;
// LOWERING-NEXT:     safe fn __slate_f80_to_i16(a: LongDouble) -> i16;
// LOWERING-NEXT:     safe fn __slate_f80_to_i32(a: LongDouble) -> i32;
// LOWERING-NEXT:     safe fn __slate_f80_to_i64(a: LongDouble) -> i64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i8(a: LongDouble) -> i8;
// LOWERING-NEXT:     safe fn __slate_f80_to_u128(a: LongDouble) -> u128;
// LOWERING-NEXT:     safe fn __slate_f80_to_u16(a: LongDouble) -> u16;
// LOWERING-NEXT:     safe fn __slate_f80_to_u32(a: LongDouble) -> u32;
// LOWERING-NEXT:     safe fn __slate_f80_to_u64(a: LongDouble) -> u64;
// LOWERING-NEXT:     safe fn __slate_f80_to_u8(a: LongDouble) -> u8;
// LOWERING-NEXT:     safe fn __slate_f80_trunc(a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_fdiml__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_fmodl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_frexpl__rf80_f80_pi32(_0: LongDouble, _1: *mut i32) -> LongDouble;
// LOWERING-NEXT:     fn __slate_hypotl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_ilogbl__ri32_f80(_0: LongDouble) -> i32;
// LOWERING-NEXT:     fn __slate_ldexpl__rf80_f80_i32(_0: LongDouble, _1: i32) -> LongDouble;
// LOWERING-NEXT:     fn __slate_lgammal__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_llrintl__ri64_f80(_0: LongDouble) -> i64;
// LOWERING-NEXT:     fn __slate_llroundl__ri64_f80(_0: LongDouble) -> i64;
// LOWERING-NEXT:     fn __slate_log10l__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_log2l__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_logbl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_logl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_lrintl__ri64_f80(_0: LongDouble) -> i64;
// LOWERING-NEXT:     fn __slate_lroundl__ri64_f80(_0: LongDouble) -> i64;
// LOWERING-NEXT:     fn __slate_nanl__rf80_pi8(_0: *const i8) -> LongDouble;
// LOWERING-NEXT:     fn __slate_nextafterl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_nexttowardl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_powl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80(_0: *mut i8, _1: *mut i8, _2: LongDouble) -> i32;
// LOWERING-NEXT:     fn __slate_remainderl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_remquol__rf80_f80_f80_pi32(_0: LongDouble, _1: LongDouble, _2: *mut i32) -> LongDouble;
// LOWERING-NEXT:     fn __slate_scalblnl__rf80_f80_i64(_0: LongDouble, _1: i64) -> LongDouble;
// LOWERING-NEXT:     fn __slate_scalbnl__rf80_f80_i32(_0: LongDouble, _1: i32) -> LongDouble;
// LOWERING-NEXT:     fn __slate_sinhl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_sinl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_sqrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_tanhl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_tanl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_tgammal__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(16))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct LongDouble([u8; 10]);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn add(self, o: LongDouble) -> LongDouble { __slate_f80_add(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn sub(self, o: LongDouble) -> LongDouble { __slate_f80_sub(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn mul(self, o: LongDouble) -> LongDouble { __slate_f80_mul(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn div(self, o: LongDouble) -> LongDouble { __slate_f80_div(self, o) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-NEXT:     fn add_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_add(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-NEXT:     fn sub_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_sub(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-NEXT:     fn mul_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_mul(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-NEXT:     fn div_assign(&mut self, o: LongDouble) { {
// REWRITES-NEXT:     *self = __slate_f80_div(*self, o);
// REWRITES-NEXT: } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn neg(self) -> LongDouble { __slate_f80_neg(self) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-NEXT:     fn eq(&self, other: &LongDouble) -> bool { __slate_f80_eq(*self, *other) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-NEXT:     fn partial_cmp(&self, other: &LongDouble) -> Option<std::cmp::Ordering> { if __slate_f80_lt(*self, *other) { Some(std::cmp::Ordering::Less) } else { if __slate_f80_gt(*self, *other) { Some(std::cmp::Ordering::Greater) } else { if __slate_f80_eq(*self, *other) { Some(std::cmp::Ordering::Equal) } else { None } } } }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn abort() -> !;
// REWRITES-NEXT:     fn sqrtl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn cbrtl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn sinl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn cosl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn tanl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn asinl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn acosl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn atanl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn atan2l(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn sinhl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn coshl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn tanhl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn expl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn exp2l(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn logl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn log2l(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn log10l(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn powl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn fmodl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn hypotl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn ldexpl(_0: LongDouble, _1: i32) -> LongDouble;
// REWRITES-NEXT:     fn frexpl(_0: LongDouble, _1: *mut i32) -> LongDouble;
// REWRITES-NEXT:     fn nanl(_0: *const i8) -> LongDouble;
// REWRITES-NEXT:     fn remainderl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn remquol(_0: LongDouble, _1: LongDouble, _2: *mut i32) -> LongDouble;
// REWRITES-NEXT:     fn scalbnl(_0: LongDouble, _1: i32) -> LongDouble;
// REWRITES-NEXT:     fn scalblnl(_0: LongDouble, _1: i64) -> LongDouble;
// REWRITES-NEXT:     fn nextafterl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn nexttowardl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn fdiml(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn lrintl(_0: LongDouble) -> i64;
// REWRITES-NEXT:     fn llrintl(_0: LongDouble) -> i64;
// REWRITES-NEXT:     fn lroundl(_0: LongDouble) -> i64;
// REWRITES-NEXT:     fn llroundl(_0: LongDouble) -> i64;
// REWRITES-NEXT:     fn ilogbl(_0: LongDouble) -> i32;
// REWRITES-NEXT:     fn logbl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn erfl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn erfcl(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn tgammal(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn lgammal(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn canonicalizel(_0: *mut LongDouble, _1: *const LongDouble) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn truncate_long_double(arg4: LongDouble) -> i32 {
// REWRITES-NEXT: let mut value: LongDouble = arg4;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = __slate_f80_to_i32(value);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mix_long_double(arg2: LongDouble, arg3: LongDouble) -> LongDouble {
// REWRITES-NEXT: let mut a: LongDouble = arg2;
// REWRITES-NEXT: let mut b: LongDouble = arg3;
// REWRITES-NEXT: let mut __retval: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut c: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: c = (a + b) / LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT: __retval = c * LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_int_casts() {
// REWRITES-NEXT: let mut i8: i8 = 0;
// REWRITES-NEXT: let mut u8: u8 = 0;
// REWRITES-NEXT: let mut i16: i16 = 0;
// REWRITES-NEXT: let mut u16: u16 = 0;
// REWRITES-NEXT: let mut i32: i32 = 0;
// REWRITES-NEXT: let mut u32: u32 = 0;
// REWRITES-NEXT: let mut i64: i64 = 0;
// REWRITES-NEXT: let mut u64: u64 = 0;
// REWRITES-NEXT: let _v0: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// REWRITES-NEXT: i8 = __slate_f80_to_i8(_v0);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v3: LongDouble = __slate_f80_from_i8(i8);
// REWRITES-NEXT:         let _v5: bool = _v3 != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// REWRITES-NEXT:         if _v5 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// REWRITES-NEXT: u8 = __slate_f80_to_u8(_v6);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v9: LongDouble = __slate_f80_from_u8(u8);
// REWRITES-NEXT:         let _v11: bool = _v9 != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// REWRITES-NEXT:         if _v11 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v12: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// REWRITES-NEXT: i16 = __slate_f80_to_i16(_v12);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v15: LongDouble = __slate_f80_from_i16(i16);
// REWRITES-NEXT:         let _v17: bool = _v15 != LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// REWRITES-NEXT:         if _v17 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v18: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]);
// REWRITES-NEXT: u16 = __slate_f80_to_u16(_v18);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v21: LongDouble = __slate_f80_from_u16(u16);
// REWRITES-NEXT:         let _v23: bool = _v21 != LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]);
// REWRITES-NEXT:         if _v23 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v24: LongDouble = LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]);
// REWRITES-NEXT: i32 = __slate_f80_to_i32(_v24);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v27: LongDouble = __slate_f80_from_i32(i32);
// REWRITES-NEXT:         let _v29: bool = _v27 != LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]);
// REWRITES-NEXT:         if _v29 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v30: LongDouble = LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]);
// REWRITES-NEXT: u32 = __slate_f80_to_u32(_v30);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v33: LongDouble = __slate_f80_from_u32(u32);
// REWRITES-NEXT:         let _v35: bool = _v33 != LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]);
// REWRITES-NEXT:         if _v35 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: i64 = -123456789012345i64;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v38: LongDouble = __slate_f80_from_i64(i64);
// REWRITES-NEXT:         let _v40: bool = _v38 != LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// REWRITES-NEXT:         if _v40 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v41: LongDouble = LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]);
// REWRITES-NEXT: u64 = __slate_f80_to_u64(_v41);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v44: LongDouble = __slate_f80_from_u64(u64);
// REWRITES-NEXT:         let _v46: bool = _v44 != LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]);
// REWRITES-NEXT:         if _v46 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v47: *mut i8 = b"i8=%d u8=%u i16=%d u16=%u i32=%d u32=%u i64=%lld u64=%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v60: i32 = unsafe { printf(_v47 as *const i8, i8 as i32, u8 as i32, i16 as i32, u16 as i32, i32, u32, i64, u64) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_i128_casts() {
// REWRITES-NEXT: let mut i128: i128 = 0;
// REWRITES-NEXT: let mut u128: u128 = 0;
// REWRITES-NEXT: let _v0: LongDouble = LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192]);
// REWRITES-NEXT: i128 = __slate_f80_to_i128(_v0);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v3: LongDouble = __slate_f80_from_i128(i128);
// REWRITES-NEXT:         let _v5: bool = _v3 != LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192]);
// REWRITES-NEXT:         if _v5 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]);
// REWRITES-NEXT: u128 = __slate_f80_to_u128(_v6);
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v9: LongDouble = __slate_f80_from_u128(u128);
// REWRITES-NEXT:         let _v11: bool = _v9 != LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]);
// REWRITES-NEXT:         if _v11 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v12: *mut i8 = b"i128=%lld u128_hi=%llu u128_lo=%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = 64;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v12 as *const i8, i128 as i64, (u128 >> _v16) as u64, (u128 & 18446744073709551615u128) as u64) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_bitint_casts() {
// REWRITES-NEXT: let mut b9: aligned::Aligned<aligned::A2, bitint::BInt<9, 1, 2>> = aligned::Aligned(bitint::BInt::<9, 1, 2>::ZERO);
// REWRITES-NEXT: let mut ub9: aligned::Aligned<aligned::A2, bitint::BUint<9, 1, 2>> = aligned::Aligned(bitint::BUint::<9, 1, 2>::ZERO);
// REWRITES-NEXT: let mut b40: aligned::Aligned<aligned::A8, bitint::BInt<40, 1, 8>> = aligned::Aligned(bitint::BInt::<40, 1, 8>::ZERO);
// REWRITES-NEXT: let mut ub40: aligned::Aligned<aligned::A8, bitint::BUint<40, 1, 8>> = aligned::Aligned(bitint::BUint::<40, 1, 8>::ZERO);
// REWRITES-NEXT: let mut b101: aligned::Aligned<aligned::A8, bitint::BInt<101, 2, 16>> = aligned::Aligned(bitint::BInt::<101, 2, 16>::ZERO);
// REWRITES-NEXT: let mut ub150: aligned::Aligned<aligned::A8, bitint::BUint<150, 3, 24>> = aligned::Aligned(bitint::BUint::<150, 3, 24>::ZERO);
// REWRITES-NEXT: let mut b256: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> = aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// REWRITES-NEXT: let mut ub300: aligned::Aligned<aligned::A8, bitint::BUint<300, 5, 40>> = aligned::Aligned(bitint::BUint::<300, 5, 40>::ZERO);
// REWRITES-NEXT: let mut b129: aligned::Aligned<aligned::A8, bitint::BInt<129, 3, 24>> = aligned::Aligned(bitint::BInt::<129, 3, 24>::ZERO);
// REWRITES-NEXT: let _v0: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// REWRITES-NEXT: let _v1: bitint::BInt<9, 1, 2> = bitint::BInt::<9, 1, 2>::from_i128(__slate_f80_to_i128(_v0) as i128);
// REWRITES-NEXT: *b9 = _v1;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v2: bitint::BInt<9, 1, 2> = *b9;
// REWRITES-NEXT:         let _v3: LongDouble = __slate_f80_from_i128(_v2.to_i128());
// REWRITES-NEXT:         let _v5: bool = _v3 != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// REWRITES-NEXT:         if _v5 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v6: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// REWRITES-NEXT: let _v7: bitint::BUint<9, 1, 2> = bitint::BUint::<9, 1, 2>::from_u128(__slate_f80_to_u128(_v6) as u128);
// REWRITES-NEXT: *ub9 = _v7;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v8: bitint::BUint<9, 1, 2> = *ub9;
// REWRITES-NEXT:         let _v9: LongDouble = __slate_f80_from_u128(_v8.to_u128());
// REWRITES-NEXT:         let _v11: bool = _v9 != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// REWRITES-NEXT:         if _v11 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v12: LongDouble = LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]);
// REWRITES-NEXT: let _v13: bitint::BInt<40, 1, 8> = bitint::BInt::<40, 1, 8>::from_i128(__slate_f80_to_i128(_v12) as i128);
// REWRITES-NEXT: *b40 = _v13;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v14: bitint::BInt<40, 1, 8> = *b40;
// REWRITES-NEXT:         let _v15: LongDouble = __slate_f80_from_i128(_v14.to_i128());
// REWRITES-NEXT:         let _v17: bool = _v15 != LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]);
// REWRITES-NEXT:         if _v17 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v18: LongDouble = LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]);
// REWRITES-NEXT: let _v19: bitint::BUint<40, 1, 8> = bitint::BUint::<40, 1, 8>::from_u128(__slate_f80_to_u128(_v18) as u128);
// REWRITES-NEXT: *ub40 = _v19;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v20: bitint::BUint<40, 1, 8> = *ub40;
// REWRITES-NEXT:         let _v21: LongDouble = __slate_f80_from_u128(_v20.to_u128());
// REWRITES-NEXT:         let _v23: bool = _v21 != LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]);
// REWRITES-NEXT:         if _v23 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v24: LongDouble = LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// REWRITES-NEXT: let _v25: bitint::BInt<101, 2, 16> = bitint::BInt::<101, 2, 16>::from_i128(__slate_f80_to_i128(_v24) as i128);
// REWRITES-NEXT: *b101 = _v25;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v26: bitint::BInt<101, 2, 16> = *b101;
// REWRITES-NEXT:         let _v27: LongDouble = __slate_f80_from_i128(_v26.to_i128());
// REWRITES-NEXT:         let _v29: bool = _v27 != LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// REWRITES-NEXT:         if _v29 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v30: bitint::BInt<101, 2, 16> = *b101;
// REWRITES-NEXT:         let _v31: LongDouble = __slate_f80_from_i128(_v30.to_i128());
// REWRITES-NEXT:         let _v32: bitint::BInt<101, 2, 16> = bitint::BInt::<101, 2, 16>::from_i128(__slate_f80_to_i128(_v31) as i128);
// REWRITES-NEXT:         let _v34: bool = _v32 != *b101;
// REWRITES-NEXT:         if _v34 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v35: LongDouble = LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]);
// REWRITES-NEXT: let _v36: bitint::BUint<150, 3, 24> = bitint::BUint::<150, 3, 24>::from_u128(__slate_f80_to_u128(_v35) as u128);
// REWRITES-NEXT: *ub150 = _v36;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v37: bitint::BUint<150, 3, 24> = *ub150;
// REWRITES-NEXT:         let _v38: LongDouble = __slate_f80_from_u128(_v37.to_u128());
// REWRITES-NEXT:         let _v40: bool = _v38 != LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]);
// REWRITES-NEXT:         if _v40 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v41: bitint::BUint<150, 3, 24> = *ub150;
// REWRITES-NEXT:         let _v42: LongDouble = __slate_f80_from_u128(_v41.to_u128());
// REWRITES-NEXT:         let _v43: bitint::BUint<150, 3, 24> = bitint::BUint::<150, 3, 24>::from_u128(__slate_f80_to_u128(_v42) as u128);
// REWRITES-NEXT:         let _v45: bool = _v43 != *ub150;
// REWRITES-NEXT:         if _v45 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v46: LongDouble = LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]);
// REWRITES-NEXT: let _v47: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_i128(__slate_f80_to_i128(_v46) as i128);
// REWRITES-NEXT: *b256 = _v47;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v48: bitint::BInt<256, 4, 32> = *b256;
// REWRITES-NEXT:         let _v49: bitint::BInt<256, 4, 32> = bitint::BInt::<256, 4, 32>::from_decimal_str("9999999999");
// REWRITES-NEXT:         let _v50: bool = _v48 != _v49;
// REWRITES-NEXT:         if _v50 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v51: bitint::BInt<256, 4, 32> = *b256;
// REWRITES-NEXT:         let _v52: LongDouble = __slate_f80_from_i128(_v51.to_i128());
// REWRITES-NEXT:         let _v54: bool = _v52 != LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]);
// REWRITES-NEXT:         if _v54 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v55: LongDouble = LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]);
// REWRITES-NEXT: let _v56: bitint::BUint<300, 5, 40> = bitint::BUint::<300, 5, 40>::from_u128(__slate_f80_to_u128(_v55) as u128);
// REWRITES-NEXT: *ub300 = _v56;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v57: bitint::BUint<300, 5, 40> = *ub300;
// REWRITES-NEXT:         let _v58: bitint::BUint<300, 5, 40> = bitint::BUint::<300, 5, 40>::from_decimal_str("4200000000");
// REWRITES-NEXT:         let _v59: bool = _v57 != _v58;
// REWRITES-NEXT:         if _v59 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v60: bitint::BUint<300, 5, 40> = *ub300;
// REWRITES-NEXT:         let _v61: LongDouble = __slate_f80_from_u128(_v60.to_u128());
// REWRITES-NEXT:         let _v63: bool = _v61 != LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]);
// REWRITES-NEXT:         if _v63 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v64: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]);
// REWRITES-NEXT: let _v65: bitint::BInt<129, 3, 24> = bitint::BInt::<129, 3, 24>::from_i128(__slate_f80_to_i128(_v64) as i128);
// REWRITES-NEXT: *b129 = _v65;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v66: bitint::BInt<129, 3, 24> = *b129;
// REWRITES-NEXT:         let _v67: i32 = _v66.to_i128() as i32;
// REWRITES-NEXT:         let _v68: i32 = 123;
// REWRITES-NEXT:         let _v69: bool = _v67 != _v68;
// REWRITES-NEXT:         if _v69 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v70: bitint::BInt<129, 3, 24> = bitint::BInt::<129, 3, 24>::from_decimal_str("123");
// REWRITES-NEXT:         let _v71: LongDouble = __slate_f80_from_i128(_v70.to_i128());
// REWRITES-NEXT:         let _v73: bool = _v71 != LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]);
// REWRITES-NEXT:         if _v73 {
// REWRITES-NEXT:                     unsafe { abort() };
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v74: *mut i8 = b"bitint_b101=%lld bitint_ub150=%llu bitint_b256_lo=%lld bitint_ub300_lo=%llu\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v75: bitint::BInt<101, 2, 16> = *b101;
// REWRITES-NEXT: let _v76: i64 = _v75.to_i128() as i64;
// REWRITES-NEXT: let _v77: bitint::BUint<150, 3, 24> = *ub150;
// REWRITES-NEXT: let _v78: u64 = _v77.to_u128() as u64;
// REWRITES-NEXT: let _v79: bitint::BInt<256, 4, 32> = *b256;
// REWRITES-NEXT: let _v80: i64 = _v79.to_i128() as i64;
// REWRITES-NEXT: let _v81: bitint::BUint<300, 5, 40> = *ub300;
// REWRITES-NEXT: let _v82: u64 = _v81.to_u128() as u64;
// REWRITES-NEXT: let _v83: i32 = unsafe { printf(_v74 as *const i8, _v76, _v78, _v80, _v82) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn print_ld(arg0: *mut i8, arg1: LongDouble) {
// REWRITES-NEXT: let mut name: *mut i8 = arg0;
// REWRITES-NEXT: let mut v: LongDouble = arg1;
// REWRITES-NEXT: let _v0: *mut i8 = b"%s=%La\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = unsafe { __slate_printf__ri32_pi8_pi8_f80(_v0 as *mut i8, name as *mut i8, v) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_math_functions() {
// REWRITES-NEXT: let mut exp: i32 = 0;
// REWRITES-NEXT: let _v0: *mut i8 = b"sqrt\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v1: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT: let _v2: LongDouble = unsafe { __slate_sqrtl__rf80_f80(_v1) };
// REWRITES-NEXT: print_ld(_v0, _v2);
// REWRITES-NEXT: let _v3: *mut i8 = b"cbrt\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 216, 3, 64]);
// REWRITES-NEXT: let _v5: LongDouble = unsafe { __slate_cbrtl__rf80_f80(_v4) };
// REWRITES-NEXT: print_ld(_v3, _v5);
// REWRITES-NEXT: let _v6: *mut i8 = b"sin\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT: let _v8: LongDouble = unsafe { __slate_sinl__rf80_f80(_v7) };
// REWRITES-NEXT: print_ld(_v6, _v8);
// REWRITES-NEXT: let _v9: *mut i8 = b"cos\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT: let _v11: LongDouble = unsafe { __slate_cosl__rf80_f80(_v10) };
// REWRITES-NEXT: print_ld(_v9, _v11);
// REWRITES-NEXT: let _v12: *mut i8 = b"tan\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT: let _v14: LongDouble = unsafe { __slate_tanl__rf80_f80(_v13) };
// REWRITES-NEXT: print_ld(_v12, _v14);
// REWRITES-NEXT: let _v15: *mut i8 = b"asin\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v17: LongDouble = unsafe { __slate_asinl__rf80_f80(_v16) };
// REWRITES-NEXT: print_ld(_v15, _v17);
// REWRITES-NEXT: let _v18: *mut i8 = b"acos\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v20: LongDouble = unsafe { __slate_acosl__rf80_f80(_v19) };
// REWRITES-NEXT: print_ld(_v18, _v20);
// REWRITES-NEXT: let _v21: *mut i8 = b"atan\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v22: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v23: LongDouble = unsafe { __slate_atanl__rf80_f80(_v22) };
// REWRITES-NEXT: print_ld(_v21, _v23);
// REWRITES-NEXT: let _v24: *mut i8 = b"atan2\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v25: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v26: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v27: LongDouble = unsafe { __slate_atan2l__rf80_f80_f80(_v25, _v26) };
// REWRITES-NEXT: print_ld(_v24, _v27);
// REWRITES-NEXT: let _v28: *mut i8 = b"sinh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v29: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v30: LongDouble = unsafe { __slate_sinhl__rf80_f80(_v29) };
// REWRITES-NEXT: print_ld(_v28, _v30);
// REWRITES-NEXT: let _v31: *mut i8 = b"cosh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v32: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v33: LongDouble = unsafe { __slate_coshl__rf80_f80(_v32) };
// REWRITES-NEXT: print_ld(_v31, _v33);
// REWRITES-NEXT: let _v34: *mut i8 = b"tanh\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v35: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v36: LongDouble = unsafe { __slate_tanhl__rf80_f80(_v35) };
// REWRITES-NEXT: print_ld(_v34, _v36);
// REWRITES-NEXT: let _v37: *mut i8 = b"exp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v38: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v39: LongDouble = unsafe { __slate_expl__rf80_f80(_v38) };
// REWRITES-NEXT: print_ld(_v37, _v39);
// REWRITES-NEXT: let _v40: *mut i8 = b"exp2\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v41: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-NEXT: let _v42: LongDouble = unsafe { __slate_exp2l__rf80_f80(_v41) };
// REWRITES-NEXT: print_ld(_v40, _v42);
// REWRITES-NEXT: let _v43: *mut i8 = b"log\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v44: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v45: LongDouble = unsafe { __slate_expl__rf80_f80(_v44) };
// REWRITES-NEXT: let _v46: LongDouble = unsafe { __slate_logl__rf80_f80(_v45) };
// REWRITES-NEXT: print_ld(_v43, _v46);
// REWRITES-NEXT: let _v47: *mut i8 = b"log2\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v48: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// REWRITES-NEXT: let _v49: LongDouble = unsafe { __slate_log2l__rf80_f80(_v48) };
// REWRITES-NEXT: print_ld(_v47, _v49);
// REWRITES-NEXT: let _v50: *mut i8 = b"log10\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v51: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 250, 8, 64]);
// REWRITES-NEXT: let _v52: LongDouble = unsafe { __slate_log10l__rf80_f80(_v51) };
// REWRITES-NEXT: print_ld(_v50, _v52);
// REWRITES-NEXT: let _v53: *mut i8 = b"pow\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v54: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT: let _v55: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-NEXT: let _v56: LongDouble = unsafe { __slate_powl__rf80_f80_f80(_v54, _v55) };
// REWRITES-NEXT: print_ld(_v53, _v56);
// REWRITES-NEXT: let _v57: *mut i8 = b"floor\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v58: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 64]);
// REWRITES-NEXT: let _v59: LongDouble = __slate_f80_floor(_v58);
// REWRITES-NEXT: print_ld(_v57, _v59);
// REWRITES-NEXT: let _v60: *mut i8 = b"ceil\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v61: LongDouble = LongDouble([102, 102, 102, 102, 102, 102, 102, 134, 0, 64]);
// REWRITES-NEXT: let _v62: LongDouble = __slate_f80_ceil(_v61);
// REWRITES-NEXT: print_ld(_v60, _v62);
// REWRITES-NEXT: let _v63: *mut i8 = b"round\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v64: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]);
// REWRITES-NEXT: let _v65: LongDouble = __slate_f80_round(_v64);
// REWRITES-NEXT: print_ld(_v63, _v65);
// REWRITES-NEXT: let _v66: *mut i8 = b"trunc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v67: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 192]);
// REWRITES-NEXT: let _v68: LongDouble = __slate_f80_trunc(_v67);
// REWRITES-NEXT: print_ld(_v66, _v68);
// REWRITES-NEXT: let _v69: *mut i8 = b"fabs\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v70: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 192]);
// REWRITES-NEXT: let _v71: LongDouble = __slate_f80_abs(_v70);
// REWRITES-NEXT: print_ld(_v69, _v71);
// REWRITES-NEXT: let _v72: *mut i8 = b"fmod\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v73: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-NEXT: let _v74: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT: let _v75: LongDouble = unsafe { __slate_fmodl__rf80_f80_f80(_v73, _v74) };
// REWRITES-NEXT: print_ld(_v72, _v75);
// REWRITES-NEXT: let _v76: *mut i8 = b"hypot\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v77: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT: let _v78: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// REWRITES-NEXT: let _v79: LongDouble = unsafe { __slate_hypotl__rf80_f80_f80(_v77, _v78) };
// REWRITES-NEXT: print_ld(_v76, _v79);
// REWRITES-NEXT: let _v80: *mut i8 = b"copysign\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v81: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT: let _v82: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// REWRITES-NEXT: let _v83: LongDouble = __slate_f80_copysign(_v81, _v82);
// REWRITES-NEXT: print_ld(_v80, _v83);
// REWRITES-NEXT: let _v84: *mut i8 = b"fmax\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v85: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v86: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT: let _v87: LongDouble = __slate_f80_fmax(_v85, _v86);
// REWRITES-NEXT: print_ld(_v84, _v87);
// REWRITES-NEXT: let _v88: *mut i8 = b"fmin\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v89: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v90: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT: let _v91: LongDouble = __slate_f80_fmin(_v89, _v90);
// REWRITES-NEXT: print_ld(_v88, _v91);
// REWRITES-NEXT: let _v92: *mut i8 = b"fma\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v93: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT: let _v94: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT: let _v95: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// REWRITES-NEXT: let _v96: LongDouble = __slate_f80_fma(_v93, _v94, _v95);
// REWRITES-NEXT: print_ld(_v92, _v96);
// REWRITES-NEXT: let _v97: *mut i8 = b"ldexp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v98: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: let _v99: i32 = 4;
// REWRITES-NEXT: let _v100: LongDouble = unsafe { __slate_ldexpl__rf80_f80_i32(_v98, _v99 as i32) };
// REWRITES-NEXT: print_ld(_v97, _v100);
// REWRITES-NEXT: exp = 0;
// REWRITES-NEXT: let _v102: *mut i8 = b"frexp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v103: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64]);
// REWRITES-NEXT: let _v104: LongDouble = unsafe { __slate_frexpl__rf80_f80_pi32(_v103, std::ptr::addr_of_mut!(exp) as *mut i32) };
// REWRITES-NEXT: print_ld(_v102, _v104);
// REWRITES-NEXT: let _v105: *mut i8 = b"frexp_exp=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v107: i32 = unsafe { printf(_v105 as *const i8, exp) };
// REWRITES-NEXT: let _v108: *mut i8 = b"isnan=%d isinf=%d signbit_neg=%d signbit_pos=%d isfinite=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v109: *mut i8 = b"\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v110: LongDouble = unsafe { __slate_nanl__rf80_pi8(_v109 as *const i8) };
// REWRITES-NEXT: let _v111: bool = __slate_f80_is_fp_class(_v110, 3);
// REWRITES-NEXT: let _v113: i32 = 1;
// REWRITES-NEXT: let _v114: i32 = 1;
// REWRITES-NEXT: let _v115: i32 = 0;
// REWRITES-NEXT: let _v116: i32 = 1;
// REWRITES-NEXT: let _v117: i32 = unsafe { printf(_v108 as *const i8, _v111 as i32, _v113, _v114, _v115, _v116) };
// REWRITES-NEXT: let _v118: *mut i8 = b"epsilon\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v119: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// REWRITES-NEXT: print_ld(_v118, _v119);
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// The functions above all round-trip through the generic call-shim (any
// REWRITES-NEXT: /// known extern function with a long double arg/return links straight to
// REWRITES-NEXT: /// libm), which check_math_functions already exercises. This covers the
// REWRITES-NEXT: /// remaining libm entry points -- pointer out-params, integer-returning
// REWRITES-NEXT: /// variants, and the classification family -- with volatile operands so
// REWRITES-NEXT: /// they can't constant-fold away and skip the real runtime path.
// REWRITES-NEXT: fn check_remaining_math_functions() {
// REWRITES-NEXT: let mut ten: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut three: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut ipart: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut frac: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut quo: i32 = 0;
// REWRITES-NEXT: let mut vnan: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut vinf: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut vzero: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut vone: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut vsub: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut vtwo: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut ten_plain: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut canon: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut canon_r: i32 = 0;
// REWRITES-NEXT: let _v0: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(ten), _v0) };
// REWRITES-NEXT: let _v1: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(three), _v1) };
// REWRITES-NEXT: ipart = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT: let _v3: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v4: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v5: LongDouble = _v3 / _v4;
// REWRITES-NEXT: let _v6: LongDouble = __slate_f80_fract(_v5);
// REWRITES-NEXT: ipart = __slate_f80_trunc(_v5);
// REWRITES-NEXT: frac = _v6;
// REWRITES-NEXT: let _v8: *mut i8 = b"modf_ipart\0".as_ptr() as *mut i8;
// REWRITES-NEXT: print_ld(_v8, ipart);
// REWRITES-NEXT: let _v10: *mut i8 = b"modf_frac\0".as_ptr() as *mut i8;
// REWRITES-NEXT: print_ld(_v10, frac);
// REWRITES-NEXT: let _v12: *mut i8 = b"remainder\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v13: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v14: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v15: LongDouble = unsafe { __slate_remainderl__rf80_f80_f80(_v13, _v14) };
// REWRITES-NEXT: print_ld(_v12, _v15);
// REWRITES-NEXT: quo = 0;
// REWRITES-NEXT: let _v17: *mut i8 = b"remquo\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v18: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v19: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v20: LongDouble = unsafe { __slate_remquol__rf80_f80_f80_pi32(_v18, _v19, std::ptr::addr_of_mut!(quo) as *mut i32) };
// REWRITES-NEXT: print_ld(_v17, _v20);
// REWRITES-NEXT: let _v21: *mut i8 = b"remquo_quo=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: i32 = unsafe { printf(_v21 as *const i8, quo) };
// REWRITES-NEXT: let _v24: *mut i8 = b"scalbn\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v25: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v26: i32 = 3;
// REWRITES-NEXT: let _v27: LongDouble = unsafe { __slate_scalbnl__rf80_f80_i32(_v25, _v26 as i32) };
// REWRITES-NEXT: print_ld(_v24, _v27);
// REWRITES-NEXT: let _v28: *mut i8 = b"scalbln\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v29: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v30: i64 = 3;
// REWRITES-NEXT: let _v31: LongDouble = unsafe { __slate_scalblnl__rf80_f80_i64(_v29, _v30 as i64) };
// REWRITES-NEXT: print_ld(_v28, _v31);
// REWRITES-NEXT: let _v32: *mut i8 = b"nextafter\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v33: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v34: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v35: LongDouble = unsafe { __slate_nextafterl__rf80_f80_f80(_v33, _v34) };
// REWRITES-NEXT: print_ld(_v32, _v35);
// REWRITES-NEXT: let _v36: *mut i8 = b"nexttoward\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v37: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v38: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v39: LongDouble = unsafe { __slate_nexttowardl__rf80_f80_f80(_v37, _v38) };
// REWRITES-NEXT: print_ld(_v36, _v39);
// REWRITES-NEXT: let _v40: *mut i8 = b"fdim\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v41: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v42: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v43: LongDouble = unsafe { __slate_fdiml__rf80_f80_f80(_v41, _v42) };
// REWRITES-NEXT: print_ld(_v40, _v43);
// REWRITES-NEXT: let _v44: *mut i8 = b"rint\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v45: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v46: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v48: LongDouble = __slate_f80_rint(_v45 / _v46);
// REWRITES-NEXT: print_ld(_v44, _v48);
// REWRITES-NEXT: let _v49: *mut i8 = b"nearbyint\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v50: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v51: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v53: LongDouble = __slate_f80_rint(_v50 / _v51);
// REWRITES-NEXT: print_ld(_v49, _v53);
// REWRITES-NEXT: let _v54: *mut i8 = b"lrint=%ld llrint=%lld lround=%ld llround=%lld\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v55: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v56: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v58: i64 = unsafe { __slate_lrintl__ri64_f80(_v55 / _v56) };
// REWRITES-NEXT: let _v59: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v60: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v62: i64 = unsafe { __slate_llrintl__ri64_f80(_v59 / _v60) };
// REWRITES-NEXT: let _v63: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v64: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v66: i64 = unsafe { __slate_lroundl__ri64_f80(_v63 / _v64) };
// REWRITES-NEXT: let _v67: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v68: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v70: i64 = unsafe { __slate_llroundl__ri64_f80(_v67 / _v68) };
// REWRITES-NEXT: let _v71: i32 = unsafe { printf(_v54 as *const i8, _v58, _v62, _v66, _v70) };
// REWRITES-NEXT: let _v72: *mut i8 = b"ilogb=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v73: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v74: i32 = unsafe { __slate_ilogbl__ri32_f80(_v73) };
// REWRITES-NEXT: let _v75: i32 = unsafe { printf(_v72 as *const i8, _v74) };
// REWRITES-NEXT: let _v76: *mut i8 = b"logb\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v77: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v78: LongDouble = unsafe { __slate_logbl__rf80_f80(_v77) };
// REWRITES-NEXT: print_ld(_v76, _v78);
// REWRITES-NEXT: let _v79: *mut i8 = b"erf\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v80: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v81: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v83: LongDouble = unsafe { __slate_erfl__rf80_f80(_v80 / _v81) };
// REWRITES-NEXT: print_ld(_v79, _v83);
// REWRITES-NEXT: let _v84: *mut i8 = b"erfc\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v85: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v86: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v88: LongDouble = unsafe { __slate_erfcl__rf80_f80(_v85 / _v86) };
// REWRITES-NEXT: print_ld(_v84, _v88);
// REWRITES-NEXT: let _v89: *mut i8 = b"tgamma\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v90: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT: let _v91: LongDouble = unsafe { __slate_tgammal__rf80_f80(_v90) };
// REWRITES-NEXT: print_ld(_v89, _v91);
// REWRITES-NEXT: let _v92: *mut i8 = b"lgamma\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v93: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: let _v94: LongDouble = unsafe { __slate_lgammal__rf80_f80(_v93) };
// REWRITES-NEXT: print_ld(_v92, _v94);
// REWRITES-NEXT: let _v95: *mut i8 = b"\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v96: LongDouble = unsafe { __slate_nanl__rf80_pi8(_v95 as *const i8) };
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vnan), _v96) };
// REWRITES-NEXT: let _v97: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vinf), _v97) };
// REWRITES-NEXT: let _v98: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vzero), _v98) };
// REWRITES-NEXT: let _v99: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vone), _v99) };
// REWRITES-NEXT: let _v100: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vsub), _v100) };
// REWRITES-NEXT: let _v101: *mut i8 = b"isnan_v=%d isinf_v=%d isfinite_v=%d isnormal_v=%d isunordered_v=%d isunordered_ok=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v102: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// REWRITES-NEXT: let _v103: bool = __slate_f80_is_fp_class(_v102, 3);
// REWRITES-NEXT: let _v104: i32 = _v103 as i32;
// REWRITES-NEXT: let _v105: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vinf)) };
// REWRITES-NEXT: let _v106: bool = __slate_f80_is_fp_class(_v105, 516);
// REWRITES-NEXT: let _v107: i32 = _v106 as i32;
// REWRITES-NEXT: let _v108: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT: let _v109: bool = __slate_f80_is_fp_class(_v108, 504);
// REWRITES-NEXT: let _v110: i32 = _v109 as i32;
// REWRITES-NEXT: let _v111: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT: let _v112: bool = __slate_f80_is_fp_class(_v111, 264);
// REWRITES-NEXT: let _v113: i32 = _v112 as i32;
// REWRITES-NEXT: let _v114: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// REWRITES-NEXT: let _v115: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT: let _v117: i32 = (_v114 != _v114 || _v115 != _v115) as i32;
// REWRITES-NEXT: let _v118: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT: let _v119: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vzero)) };
// REWRITES-NEXT: let _v122: i32 = unsafe { printf(_v101 as *const i8, _v104, _v107, _v110, _v113, _v117, (_v118 != _v118 || _v119 != _v119) as i32) };
// REWRITES-NEXT: let _v123: *mut i8 = b"subnormal_isnormal=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v124: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vsub)) };
// REWRITES-NEXT: let _v125: bool = __slate_f80_is_fp_class(_v124, 264);
// REWRITES-NEXT: let _v127: i32 = unsafe { printf(_v123 as *const i8, _v125 as i32) };
// REWRITES-NEXT: let _v128: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT: unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vtwo), _v128) };
// REWRITES-NEXT: let _v129: *mut i8 = b"islessgreater_lt=%d islessgreater_eq=%d islessgreater_nan=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v130: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT: let _v131: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vtwo)) };
// REWRITES-NEXT: let _v133: i32 = (_v130 < _v131 || _v130 > _v131) as i32;
// REWRITES-NEXT: let _v134: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT: let _v135: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT: let _v137: i32 = (_v134 < _v135 || _v134 > _v135) as i32;
// REWRITES-NEXT: let _v138: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// REWRITES-NEXT: let _v139: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT: let _v142: i32 = unsafe { printf(_v129 as *const i8, _v133, _v137, (_v138 < _v139 || _v138 > _v139) as i32) };
// REWRITES-NEXT: ten_plain = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT: canon = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT: canon_r = unsafe { __slate_canonicalizel__ri32_pf80_pf80(std::ptr::addr_of_mut!(canon), std::ptr::addr_of_mut!(ten_plain)) };
// REWRITES-NEXT: let _v146: *mut i8 = b"canonicalize\0".as_ptr() as *mut i8;
// REWRITES-NEXT: print_ld(_v146, canon);
// REWRITES-NEXT: let _v148: *mut i8 = b"canonicalize_r=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v150: i32 = unsafe { printf(_v148 as *const i8, canon_r) };
// REWRITES-NEXT: let _v151: *mut i8 = b"ldbl_min\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v152: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 0]);
// REWRITES-NEXT: print_ld(_v151, _v152);
// REWRITES-NEXT: let _v153: *mut i8 = b"ldbl_true_min\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v154: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT: print_ld(_v153, _v154);
// REWRITES-NEXT: let _v155: *mut i8 = b"ldbl_mant_dig=%d ldbl_dig=%d ldbl_min_exp=%d ldbl_max_exp=%d ldbl_min_10_exp=%d ldbl_max_10_exp=%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v156: i32 = 64;
// REWRITES-NEXT: let _v157: i32 = 18;
// REWRITES-NEXT: let _v158: i32 = -16381;
// REWRITES-NEXT: let _v159: i32 = 16384;
// REWRITES-NEXT: let _v160: i32 = -4931;
// REWRITES-NEXT: let _v161: i32 = 4932;
// REWRITES-NEXT: let _v162: i32 = unsafe { printf(_v155 as *const i8, _v156, _v157, _v158, _v159, _v160, _v161) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut x: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: let mut y: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: x = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]);
// REWRITES-NEXT: y = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// REWRITES-NEXT: let _v3: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v7: i32 = truncate_long_double(x + y);
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v3 as *const i8, _v7) };
// REWRITES-NEXT: let _v9: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v10: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT: let _v11: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT: let _v12: LongDouble = mix_long_double(_v10, _v11);
// REWRITES-NEXT: let _v13: i32 = truncate_long_double(_v12);
// REWRITES-NEXT: let _v14: i32 = unsafe { printf(_v9 as *const i8, _v13) };
// REWRITES-NEXT: let _v15: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = 7;
// REWRITES-NEXT: let _v17: LongDouble = __slate_f80_from_i32(_v16);
// REWRITES-NEXT: let _v20: i32 = truncate_long_double(_v17 / LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]));
// REWRITES-NEXT: let _v21: i32 = unsafe { printf(_v15 as *const i8, _v20) };
// REWRITES-NEXT: check_int_casts();
// REWRITES-NEXT: check_i128_casts();
// REWRITES-NEXT: check_bitint_casts();
// REWRITES-NEXT: check_math_functions();
// REWRITES-NEXT: check_remaining_math_functions();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __slate_acosl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_asinl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_atan2l__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_atanl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_canonicalizel__ri32_pf80_pf80(_0: *mut LongDouble, _1: *const LongDouble) -> i32;
// REWRITES-NEXT:     fn __slate_cbrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_cf80_div(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_cf80_mul(a: num_complex::Complex<LongDouble>, b: num_complex::Complex<LongDouble>) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn __slate_coshl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_erfcl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_erfl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_exp2l__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_expl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_abs(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_add(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ceil(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_copysign(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_div(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_eq(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_floor(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fma(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmax(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmin(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fract(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_bool(a: bool) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f32(a: f32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f64(a: f64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i128(a: i128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i16(a: i16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i32(a: i32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i64(a: i64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i8(a: i8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u128(a: u128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u16(a: u16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u32(a: u32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u64(a: u64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u8(a: u8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ge(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_gt(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_is_fp_class(a: LongDouble, flags: i32) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_le(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_lt(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_mul(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ne(a: LongDouble, b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_neg(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_rint(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_round(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_signbit(a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_sub(a: LongDouble, b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_to_bool(a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_to_f32(a: LongDouble) -> f32;
// REWRITES-NEXT:     safe fn __slate_f80_to_f64(a: LongDouble) -> f64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i128(a: LongDouble) -> i128;
// REWRITES-NEXT:     safe fn __slate_f80_to_i16(a: LongDouble) -> i16;
// REWRITES-NEXT:     safe fn __slate_f80_to_i32(a: LongDouble) -> i32;
// REWRITES-NEXT:     safe fn __slate_f80_to_i64(a: LongDouble) -> i64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i8(a: LongDouble) -> i8;
// REWRITES-NEXT:     safe fn __slate_f80_to_u128(a: LongDouble) -> u128;
// REWRITES-NEXT:     safe fn __slate_f80_to_u16(a: LongDouble) -> u16;
// REWRITES-NEXT:     safe fn __slate_f80_to_u32(a: LongDouble) -> u32;
// REWRITES-NEXT:     safe fn __slate_f80_to_u64(a: LongDouble) -> u64;
// REWRITES-NEXT:     safe fn __slate_f80_to_u8(a: LongDouble) -> u8;
// REWRITES-NEXT:     safe fn __slate_f80_trunc(a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_fdiml__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_fmodl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_frexpl__rf80_f80_pi32(_0: LongDouble, _1: *mut i32) -> LongDouble;
// REWRITES-NEXT:     fn __slate_hypotl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_ilogbl__ri32_f80(_0: LongDouble) -> i32;
// REWRITES-NEXT:     fn __slate_ldexpl__rf80_f80_i32(_0: LongDouble, _1: i32) -> LongDouble;
// REWRITES-NEXT:     fn __slate_lgammal__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_llrintl__ri64_f80(_0: LongDouble) -> i64;
// REWRITES-NEXT:     fn __slate_llroundl__ri64_f80(_0: LongDouble) -> i64;
// REWRITES-NEXT:     fn __slate_log10l__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_log2l__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_logbl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_logl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_lrintl__ri64_f80(_0: LongDouble) -> i64;
// REWRITES-NEXT:     fn __slate_lroundl__ri64_f80(_0: LongDouble) -> i64;
// REWRITES-NEXT:     fn __slate_nanl__rf80_pi8(_0: *const i8) -> LongDouble;
// REWRITES-NEXT:     fn __slate_nextafterl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_nexttowardl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_powl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80(_0: *mut i8, _1: *mut i8, _2: LongDouble) -> i32;
// REWRITES-NEXT:     fn __slate_remainderl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_remquol__rf80_f80_f80_pi32(_0: LongDouble, _1: LongDouble, _2: *mut i32) -> LongDouble;
// REWRITES-NEXT:     fn __slate_scalblnl__rf80_f80_i64(_0: LongDouble, _1: i64) -> LongDouble;
// REWRITES-NEXT:     fn __slate_scalbnl__rf80_f80_i32(_0: LongDouble, _1: i32) -> LongDouble;
// REWRITES-NEXT:     fn __slate_sinhl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_sinl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_sqrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_tanhl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_tanl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_tgammal__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
