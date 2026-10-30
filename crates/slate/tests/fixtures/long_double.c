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
  volatile long double ten   = 10.0L;
  volatile long double three = 3.0L;

  long double ipart = 0.0L;
  long double frac  = modfl(ten / three, &ipart);
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

  printf("lrint=%ld llrint=%lld lround=%ld llround=%lld\n", lrintl(ten / three),
         llrintl(ten / three), lroundl(ten / three), llroundl(ten / three));

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
  int         canon_r   = canonicalizel(&canon, &ten_plain);
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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exp([[ARG_0:arg[0-9]+]]: f64) -> f64;
// CHECK-NEXT:     fn canonicalizel([[ARG_0]]: *mut LongDouble, [[ARG_1:arg[0-9]+]]: *const LongDouble) -> i32;
// CHECK-NEXT:     fn printf([[ARG_0]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT:     fn __slate_acosl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_asinl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_atan2l__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_atanl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_cbrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_ceill__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_copysignl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_coshl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_erfcl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_erfl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_exp2l__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_expl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_fabsl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_fdiml__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_floorl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_fmal__rf80_f80_f80_f80(_0: LongDouble, _1: LongDouble, _2: LongDouble)
// CHECK-NEXT:     -> LongDouble;
// CHECK-NEXT:     fn __slate_fmaxl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_fminl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_fmodl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_frexpl__rf80_f80_pi32(_0: LongDouble, _1: *mut i32) -> LongDouble;
// CHECK-NEXT:     fn __slate_hypotl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_ilogbl__ri32_f80(_0: LongDouble) -> i32;
// CHECK-NEXT:     fn __slate_ldexpl__rf80_f80_i32(_0: LongDouble, _1: i32) -> LongDouble;
// CHECK-NEXT:     fn __slate_lgammal__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_llrintl__ri64_f80(_0: LongDouble) -> i64;
// CHECK-NEXT:     fn __slate_llroundl__ri64_f80(_0: LongDouble) -> i64;
// CHECK-NEXT:     fn __slate_log10l__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_log2l__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_logbl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_logl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_lrintl__ri64_f80(_0: LongDouble) -> i64;
// CHECK-NEXT:     fn __slate_lroundl__ri64_f80(_0: LongDouble) -> i64;
// CHECK-NEXT:     fn __slate_modfl__rf80_f80_pf80(_0: LongDouble, _1: *mut LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_nearbyintl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_nextafterl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_nexttowardl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_powl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80(_0: *const i8, _1: *const i8, _2: LongDouble) -> i32;
// CHECK-NEXT:     fn __slate_remainderl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_remquol__rf80_f80_f80_pi32(
// CHECK-NEXT:         _0: LongDouble,
// CHECK-NEXT:         _1: LongDouble,
// CHECK-NEXT:         _2: *mut i32,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     fn __slate_rintl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_roundl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_scalblnl__rf80_f80_i64(_0: LongDouble, _1: i64) -> LongDouble;
// CHECK-NEXT:     fn __slate_scalbnl__rf80_f80_i32(_0: LongDouble, _1: i32) -> LongDouble;
// CHECK-NEXT:     fn __slate_sinhl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_sinl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_sqrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_tanhl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_tanl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_tgammal__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_truncl__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mix_long_double(a: LongDouble, b: LongDouble) -> LongDouble {
// CHECK-NEXT:     let c: LongDouble = (a + b) / LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// CHECK-NEXT:     c * LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64])
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn truncate_long_double(value: LongDouble) -> i32 {
// CHECK-NEXT:     __slate_f80_to_i32(value)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn print_ld(name: *const i8, v: LongDouble) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { __slate_printf__ri32_pi8_pi8_f80(c"%s=%La\n".as_ptr(), name, v) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_int_casts() {
// CHECK-NEXT:     let i8: i8 = __slate_f80_to_i8(LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]));
// CHECK-NEXT:     if __slate_f80_from_i8(i8) != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let u8: u8 = __slate_f80_to_u8(LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]));
// CHECK-NEXT:     if __slate_f80_from_u8(u8) != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let i16: i16 = __slate_f80_to_i16(LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]));
// CHECK-NEXT:     if __slate_f80_from_i16(i16) != LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let u16: u16 = __slate_f80_to_u16(LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]));
// CHECK-NEXT:     if __slate_f80_from_u16(u16) != LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let i32: i32 = __slate_f80_to_i32(LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]));
// CHECK-NEXT:     if __slate_f80_from_i32(i32) != LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let u32: u32 = __slate_f80_to_u32(LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]));
// CHECK-NEXT:     if __slate_f80_from_u32(u32) != LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let i64: i64 = -123456789012345;
// CHECK-NEXT:     if __slate_f80_from_i64(i64) != LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let u64: u64 = __slate_f80_to_u64(LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]));
// CHECK-NEXT:     if __slate_f80_from_u64(u64) != LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"i8=%d u8=%u i16=%d u16=%u i32=%d u32=%u i64=%lld u64=%llu\n".as_ptr(),
// CHECK-NEXT:             i8 as i32,
// CHECK-NEXT:             (u8 as u32) as i32,
// CHECK-NEXT:             i16 as i32,
// CHECK-NEXT:             (u16 as u32) as i32,
// CHECK-NEXT:             i32,
// CHECK-NEXT:             u32,
// CHECK-NEXT:             i64,
// CHECK-NEXT:             u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_i128_casts() {
// CHECK-NEXT:     let i128: i128 = __slate_f80_to_i128(LongDouble([
// CHECK-NEXT:         254, 255, 255, 255, 255, 255, 255, 255, 61, 192,
// CHECK-NEXT:     ]));
// CHECK-NEXT:     if __slate_f80_from_i128(i128) != LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192])
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let u128: u128 =
// CHECK-NEXT:         __slate_f80_to_u128(LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]));
// CHECK-NEXT:     if __slate_f80_from_u128(u128) != LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"i128=%lld u128_hi=%llu u128_lo=%llu\n".as_ptr(),
// CHECK-NEXT:             i128 as i64,
// CHECK-NEXT:             (u128 >> 64i32) as u64,
// CHECK-NEXT:             (u128 & 18446744073709551615) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_bitint_casts() {
// CHECK-NEXT:     let b9: i16 = __slate_f80_to_i16(LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]));
// CHECK-NEXT:     if __slate_f80_from_i16(b9) != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let ub9: u16 = __slate_f80_to_u16(LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]));
// CHECK-NEXT:     if __slate_f80_from_u16(ub9) != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let b40: i64 = __slate_f80_to_i64(LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]));
// CHECK-NEXT:     if __slate_f80_from_i64(b40) != LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let ub40: u64 = __slate_f80_to_u64(LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]));
// CHECK-NEXT:     if __slate_f80_from_u64(ub40) != LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut b101: bitint::BInt<101, 2, 16, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<101, 2, 16, bitint::A8>>::from_f80_bytes(
// CHECK-NEXT:             LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]).0,
// CHECK-NEXT:         );
// CHECK-NEXT:     if LongDouble(b101.to_f80_bytes()) != LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if <bitint::BInt<101, 2, 16, bitint::A8>>::from_f80_bytes(LongDouble(b101.to_f80_bytes()).0)
// CHECK-NEXT:         .compare(b101)
// CHECK-NEXT:         != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut ub150: bitint::BUint<150, 3, 24, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<150, 3, 24, bitint::A8>>::from_f80_bytes(
// CHECK-NEXT:             LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]).0,
// CHECK-NEXT:         );
// CHECK-NEXT:     if LongDouble(ub150.to_f80_bytes()) != LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if <bitint::BUint<150, 3, 24, bitint::A8>>::from_f80_bytes(LongDouble(ub150.to_f80_bytes()).0)
// CHECK-NEXT:         .compare(ub150)
// CHECK-NEXT:         != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut b256: bitint::BInt<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<256, 4, 32, bitint::A8>>::from_f80_bytes(
// CHECK-NEXT:             LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]).0,
// CHECK-NEXT:         );
// CHECK-NEXT:     if b256.compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(
// CHECK-NEXT:         9999999999 as i128,
// CHECK-NEXT:     )) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if LongDouble(b256.to_f80_bytes()) != LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut ub300: bitint::BUint<300, 5, 40, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<300, 5, 40, bitint::A8>>::from_f80_bytes(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]).0,
// CHECK-NEXT:         );
// CHECK-NEXT:     if ub300.compare(<bitint::BUint<300, 5, 40, bitint::A8>>::from_u128(
// CHECK-NEXT:         4200000000 as u128,
// CHECK-NEXT:     )) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if LongDouble(ub300.to_f80_bytes()) != LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut b129: bitint::BInt<129, 3, 24, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<129, 3, 24, bitint::A8>>::from_f80_bytes(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]).0,
// CHECK-NEXT:         );
// CHECK-NEXT:     if (b129.to_i128() as i32) != 123 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if LongDouble(<bitint::BInt<129, 3, 24, bitint::A8>>::from_i128(123 as i128).to_f80_bytes())
// CHECK-NEXT:         != LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64])
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"bitint_b101=%lld bitint_ub150=%llu bitint_b256_lo=%lld bitint_ub300_lo=%llu\n"
// CHECK-NEXT:                 .as_ptr(),
// CHECK-NEXT:             b101.to_i128() as i64,
// CHECK-NEXT:             ub150.to_u128() as u64,
// CHECK-NEXT:             b256.to_i128() as i64,
// CHECK-NEXT:             ub300.to_u128() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_math_functions() {
// CHECK-NEXT:     print_ld(c"sqrt".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_sqrtl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"cbrt".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_cbrtl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 216, 3, 64]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"sin".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_sinl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"cos".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_cosl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"tan".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_tanl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"asin".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_asinl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"acos".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_acosl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"atan".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_atanl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"atan2".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_atan2l__rf80_f80_f80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"sinh".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_sinhl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"cosh".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_coshl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"tanh".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_tanhl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"exp".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_expl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"exp2".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_exp2l__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"log".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_logl__rf80_f80(__slate_expl__rf80_f80(LongDouble([
// CHECK-NEXT:             0, 0, 0, 0, 0, 0, 0, 128, 255, 63,
// CHECK-NEXT:         ])))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"log2".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_log2l__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"log10".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_log10l__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 250, 8, 64]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"pow".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_powl__rf80_f80_f80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"floor".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_floorl__rf80_f80(LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 64]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"ceil".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_ceill__rf80_f80(LongDouble([102, 102, 102, 102, 102, 102, 102, 134, 0, 64]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"round".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_roundl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"trunc".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_truncl__rf80_f80(LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 192]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"fabs".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_fabsl__rf80_f80(LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 192]))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"fmod".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_fmodl__rf80_f80_f80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"hypot".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_hypotl__rf80_f80_f80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"copysign".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_copysignl__rf80_f80_f80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"fmax".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_fmaxl__rf80_f80_f80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"fmin".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_fminl__rf80_f80_f80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"fma".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_fmal__rf80_f80_f80_f80(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"ldexp".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_ldexpl__rf80_f80_i32(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), 4i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut exp: i32 = 0;
// CHECK-NEXT:     print_ld(c"frexp".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_frexpl__rf80_f80_pi32(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64]),
// CHECK-NEXT:             std::ptr::addr_of_mut!(exp),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("frexp_exp={}", exp);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "isnan={} isinf={} signbit_neg={} signbit_pos={} isfinite={}",
// CHECK-NEXT:         __slate_f80_is_fp_class(LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]), 3) as i32,
// CHECK-NEXT:         if __slate_f80_is_fp_class(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]), 516) {
// CHECK-NEXT:             if __slate_f80_signbit(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127])) {
// CHECK-NEXT:                 -1i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 1i32
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         },
// CHECK-NEXT:         __slate_f80_signbit(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191])) as i32,
// CHECK-NEXT:         __slate_f80_signbit(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63])) as i32,
// CHECK-NEXT:         __slate_f80_is_fp_class(
// CHECK-NEXT:             LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 254, 127]),
// CHECK-NEXT:             504
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     print_ld(
// CHECK-NEXT:         c"epsilon".as_ptr(),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]),
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// The functions above all round-trip through the generic call-shim (any
// CHECK-NEXT: /// known extern function with a long double arg/return links straight to
// CHECK-NEXT: /// libm), which check_math_functions already exercises. This covers the
// CHECK-NEXT: /// remaining libm entry points -- pointer out-params, integer-returning
// CHECK-NEXT: /// variants, and the classification family -- with volatile operands so
// CHECK-NEXT: /// they can't constant-fold away and skip the real runtime path.
// CHECK-NEXT: fn check_remaining_math_functions() {
// CHECK-NEXT:     let ten: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// CHECK-NEXT:     let three: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// CHECK-NEXT:     let mut ipart: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     let frac: LongDouble = unsafe {
// CHECK-NEXT:         __slate_modfl__rf80_f80_pf80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                 / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:             std::ptr::addr_of_mut!(ipart),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     print_ld(c"modf_ipart".as_ptr(), ipart);
// CHECK-NEXT:     print_ld(c"modf_frac".as_ptr(), frac);
// CHECK-NEXT:     print_ld(c"remainder".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_remainderl__rf80_f80_f80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten)),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     let mut quo: i32 = 0;
// CHECK-NEXT:     print_ld(c"remquo".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_remquol__rf80_f80_f80_pi32(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten)),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:             std::ptr::addr_of_mut!(quo),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("remquo_quo={}", quo);
// CHECK-NEXT:     print_ld(c"scalbn".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_scalbnl__rf80_f80_i32(std::ptr::read_volatile(std::ptr::addr_of!(ten)), 3i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"scalbln".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_scalblnl__rf80_f80_i64(std::ptr::read_volatile(std::ptr::addr_of!(ten)), 3i64)
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"nextafter".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_nextafterl__rf80_f80_f80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten)),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"nexttoward".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_nexttowardl__rf80_f80_f80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten)),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"fdim".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_fdiml__rf80_f80_f80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten)),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"rint".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_rintl__rf80_f80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                 / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"nearbyint".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_nearbyintl__rf80_f80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                 / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"lrint=%ld llrint=%lld lround=%ld llround=%lld\n".as_ptr(),
// CHECK-NEXT:             __slate_lrintl__ri64_f80(
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                     / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_llrintl__ri64_f80(
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                     / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_lroundl__ri64_f80(
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                     / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_llroundl__ri64_f80(
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                     / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("ilogb={}", unsafe {
// CHECK-NEXT:         __slate_ilogbl__ri32_f80(std::ptr::read_volatile(std::ptr::addr_of!(ten)))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"logb".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_logbl__rf80_f80(std::ptr::read_volatile(std::ptr::addr_of!(ten)))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"erf".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_erfl__rf80_f80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                 / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"erfc".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_erfcl__rf80_f80(
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                 / std::ptr::read_volatile(std::ptr::addr_of!(three)),
// CHECK-NEXT:         )
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"tgamma".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_tgammal__rf80_f80(std::ptr::read_volatile(std::ptr::addr_of!(three)))
// CHECK-NEXT:     });
// CHECK-NEXT:     print_ld(c"lgamma".as_ptr(), unsafe {
// CHECK-NEXT:         __slate_lgammal__rf80_f80(std::ptr::read_volatile(std::ptr::addr_of!(ten)))
// CHECK-NEXT:     });
// CHECK-NEXT:     let vnan: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 127]);
// CHECK-NEXT:     let vinf: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// CHECK-NEXT:     let vzero: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     let vone: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// CHECK-NEXT:     let vsub: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __u: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// CHECK-NEXT:         let __v: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// CHECK-NEXT:         [[V_0]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:             != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) }
// CHECK-NEXT:             && ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:                 != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) }
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) })
// CHECK-NEXT:                     != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) });
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __u: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// CHECK-NEXT:         let __v: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vzero)) };
// CHECK-NEXT:         [[V_1]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:             != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) }
// CHECK-NEXT:             && ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:                 != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) }
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) })
// CHECK-NEXT:                     != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) });
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "isnan_v={} isinf_v={} isfinite_v={} isnormal_v={} isunordered_v={} isunordered_ok={}",
// CHECK-NEXT:         __slate_f80_is_fp_class(
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) },
// CHECK-NEXT:             3
// CHECK-NEXT:         ) as i32,
// CHECK-NEXT:         if __slate_f80_is_fp_class(
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vinf)) },
// CHECK-NEXT:             516
// CHECK-NEXT:         ) {
// CHECK-NEXT:             if __slate_f80_signbit(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vinf)) }) {
// CHECK-NEXT:                 -1i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 1i32
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         },
// CHECK-NEXT:         __slate_f80_is_fp_class(
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) },
// CHECK-NEXT:             504
// CHECK-NEXT:         ) as i32,
// CHECK-NEXT:         __slate_f80_is_fp_class(
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) },
// CHECK-NEXT:             264
// CHECK-NEXT:         ) as i32,
// CHECK-NEXT:         [[V_0]] as i32,
// CHECK-NEXT:         [[V_1]] as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "subnormal_isnormal={}",
// CHECK-NEXT:         __slate_f80_is_fp_class(
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vsub)) },
// CHECK-NEXT:             264
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let vtwo: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __x: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// CHECK-NEXT:         let __y: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vtwo)) };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:         let __u: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__x)) };
// CHECK-NEXT:         let __v: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__y)) };
// CHECK-NEXT:         [[V_3]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:             != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) }
// CHECK-NEXT:             && ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:                 != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) }
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) })
// CHECK-NEXT:                     != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) });
// CHECK-NEXT:         [[V_2]] = ![[V_3]]
// CHECK-NEXT:             && (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__x)) })
// CHECK-NEXT:                 != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__y)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __x: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// CHECK-NEXT:         let __y: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: bool;
// CHECK-NEXT:         let __u: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__x)) };
// CHECK-NEXT:         let __v: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__y)) };
// CHECK-NEXT:         [[V_5]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:             != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) }
// CHECK-NEXT:             && ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:                 != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) }
// CHECK-NEXT:                 || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) })
// CHECK-NEXT:                     != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) });
// CHECK-NEXT:         [[V_4]] = ![[V_5]]
// CHECK-NEXT:             && (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__x)) })
// CHECK-NEXT:                 != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__y)) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bool;
// CHECK-NEXT:     let __x: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// CHECK-NEXT:     let __y: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     let __u: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__x)) };
// CHECK-NEXT:     let __v: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__y)) };
// CHECK-NEXT:     [[V_7]] = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:         != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) }
// CHECK-NEXT:         && ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) })
// CHECK-NEXT:             != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__u)) }
// CHECK-NEXT:             || (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) })
// CHECK-NEXT:                 != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__v)) });
// CHECK-NEXT:     [[V_6]] = ![[V_7]]
// CHECK-NEXT:         && (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__x)) })
// CHECK-NEXT:             != unsafe { std::ptr::read_volatile(std::ptr::addr_of!(__y)) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "islessgreater_lt={} islessgreater_eq={} islessgreater_nan={}",
// CHECK-NEXT:         [[V_2]] as i32, [[V_4]] as i32, [[V_6]] as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut ten_plain: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// CHECK-NEXT:     let mut canon: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// CHECK-NEXT:     let canon_r: i32 = unsafe {
// CHECK-NEXT:         canonicalizel(
// CHECK-NEXT:             std::ptr::addr_of_mut!(canon),
// CHECK-NEXT:             std::ptr::addr_of_mut!(ten_plain) as *const LongDouble,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     print_ld(c"canonicalize".as_ptr(), canon);
// CHECK-NEXT:     println!("canonicalize_r={}", canon_r);
// CHECK-NEXT:     print_ld(
// CHECK-NEXT:         c"ldbl_min".as_ptr(),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 0]),
// CHECK-NEXT:     );
// CHECK-NEXT:     print_ld(
// CHECK-NEXT:         c"ldbl_true_min".as_ptr(),
// CHECK-NEXT:         LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "ldbl_mant_dig={} ldbl_dig={} ldbl_min_exp={} ldbl_max_exp={} ldbl_min_10_exp={} ldbl_max_10_exp={}",
// CHECK-NEXT:         64i32, 18i32, -16381i32, 16384i32, -4931i32, 4932i32
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]);
// CHECK-NEXT:     let y: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// CHECK-NEXT:     println!("{}", truncate_long_double(x + y));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         truncate_long_double(mix_long_double(
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// CHECK-NEXT:             LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64])
// CHECK-NEXT:         ))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}",
// CHECK-NEXT:         truncate_long_double(LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]))
// CHECK-NEXT:     );
// CHECK-NEXT:     check_int_casts();
// CHECK-NEXT:     check_i128_casts();
// CHECK-NEXT:     check_bitint_casts();
// CHECK-NEXT:     check_math_functions();
// CHECK-NEXT:     check_remaining_math_functions();
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
