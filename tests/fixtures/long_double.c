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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(f128)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, align(16))]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct LongDouble([u8; 10]);
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_add(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_div(self, __o)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-NEXT:         {
// LOWERING-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-NEXT:     type Output = LongDouble;
// LOWERING-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-NEXT:         __slate_f80_neg(self)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-NEXT:         } else {
// LOWERING-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-NEXT:             } else {
// LOWERING-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     None
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
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
// LOWERING-NEXT:     fn nanl(_0: *const core::ffi::c_char) -> LongDouble;
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
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = truncate_long_double({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = mix_long_double({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = truncate_long_double({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = truncate_long_double({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     check_int_casts();
// LOWERING-NEXT:     check_i128_casts();
// LOWERING-NEXT:     check_bitint_casts();
// LOWERING-NEXT:     check_math_functions();
// LOWERING-NEXT:     check_remaining_math_functions();
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn truncate_long_double({{arg[0-9]+}}: LongDouble) -> i32 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __slate_f80_to_i32({{arg[0-9]+}});
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn mix_long_double({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = __slate_f80_to_i8({{__v[0-9]+}});
// LOWERING-NEXT:     i8 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i8 = i8;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i8({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = __slate_f80_to_u8({{__v[0-9]+}});
// LOWERING-NEXT:     u8 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: u8 = u8;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u8({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i16 = __slate_f80_to_i16({{__v[0-9]+}});
// LOWERING-NEXT:     i16 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i16 = i16;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i16({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = __slate_f80_to_u16({{__v[0-9]+}});
// LOWERING-NEXT:     u16 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: u16 = u16;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u16({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __slate_f80_to_i32({{__v[0-9]+}});
// LOWERING-NEXT:     i32 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = i32;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = __slate_f80_to_u32({{__v[0-9]+}});
// LOWERING-NEXT:     u32 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: u32 = u32;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u32({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = -123456789012345i64;
// LOWERING-NEXT:     i64 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i64 = i64;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i64({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = __slate_f80_to_u64({{__v[0-9]+}});
// LOWERING-NEXT:     u64 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = u64;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u64({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-NEXT:         b"i8=%d u8=%u i16=%d u16=%u i32=%d u32=%u i64=%lld u64=%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i16 = i16;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u16 = u16;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u32 = u32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_i128_casts() {
// LOWERING-NEXT:     let mut i128: i128 = 0;
// LOWERING-NEXT:     let mut u128: u128 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = __slate_f80_to_i128({{__v[0-9]+}});
// LOWERING-NEXT:     i128 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: i128 = i128;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = __slate_f80_to_u128({{__v[0-9]+}});
// LOWERING-NEXT:     u128 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: u128 = u128;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}});
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"i128=%lld u128_hi=%llu u128_lo=%llu\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i128 = i128;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = u128;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} >> {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = u128;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = 18446744073709551615u128;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u128 = {{__v[0-9]+}} & {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_bitint_casts() {
// LOWERING-NEXT:     let mut b9: aligned::Aligned<aligned::A2, bitint::BInt<9, 1, 2>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BInt::<9, 1, 2>::ZERO);
// LOWERING-NEXT:     let mut ub9: aligned::Aligned<aligned::A2, bitint::BUint<9, 1, 2>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BUint::<9, 1, 2>::ZERO);
// LOWERING-NEXT:     let mut b40: aligned::Aligned<aligned::A8, bitint::BInt<40, 1, 8>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BInt::<40, 1, 8>::ZERO);
// LOWERING-NEXT:     let mut ub40: aligned::Aligned<aligned::A8, bitint::BUint<40, 1, 8>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BUint::<40, 1, 8>::ZERO);
// LOWERING-NEXT:     let mut b101: aligned::Aligned<aligned::A8, bitint::BInt<101, 2, 16>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BInt::<101, 2, 16>::ZERO);
// LOWERING-NEXT:     let mut ub150: aligned::Aligned<aligned::A8, bitint::BUint<150, 3, 24>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BUint::<150, 3, 24>::ZERO);
// LOWERING-NEXT:     let mut b256: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// LOWERING-NEXT:     let mut ub300: aligned::Aligned<aligned::A8, bitint::BUint<300, 5, 40>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BUint::<300, 5, 40>::ZERO);
// LOWERING-NEXT:     let mut b129: aligned::Aligned<aligned::A8, bitint::BInt<129, 3, 24>> =
// LOWERING-NEXT:         aligned::Aligned(bitint::BInt::<129, 3, 24>::ZERO);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<9, 1, 2> =
// LOWERING-NEXT:         bitint::BInt::<9, 1, 2>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// LOWERING-NEXT:     *b9 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<9, 1, 2> = *b9;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BUint<9, 1, 2> =
// LOWERING-NEXT:         bitint::BUint::<9, 1, 2>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// LOWERING-NEXT:     *ub9 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<9, 1, 2> = *ub9;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<40, 1, 8> =
// LOWERING-NEXT:         bitint::BInt::<40, 1, 8>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// LOWERING-NEXT:     *b40 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<40, 1, 8> = *b40;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BUint<40, 1, 8> =
// LOWERING-NEXT:         bitint::BUint::<40, 1, 8>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// LOWERING-NEXT:     *ub40 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<40, 1, 8> = *ub40;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> =
// LOWERING-NEXT:         bitint::BInt::<101, 2, 16>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// LOWERING-NEXT:     *b101 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> = *b101;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> = *b101;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> =
// LOWERING-NEXT:             bitint::BInt::<101, 2, 16>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> = *b101;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> =
// LOWERING-NEXT:         bitint::BUint::<150, 3, 24>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// LOWERING-NEXT:     *ub150 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> = *ub150;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> = *ub150;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> =
// LOWERING-NEXT:             bitint::BUint::<150, 3, 24>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> = *ub150;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// LOWERING-NEXT:         bitint::BInt::<256, 4, 32>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// LOWERING-NEXT:     *b256 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *b256;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// LOWERING-NEXT:             bitint::BInt::<256, 4, 32>::from_decimal_str("9999999999");
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *b256;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> =
// LOWERING-NEXT:         bitint::BUint::<300, 5, 40>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// LOWERING-NEXT:     *ub300 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> = *ub300;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> =
// LOWERING-NEXT:             bitint::BUint::<300, 5, 40>::from_decimal_str("4200000000");
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> = *ub300;
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<129, 3, 24> =
// LOWERING-NEXT:         bitint::BInt::<129, 3, 24>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// LOWERING-NEXT:     *b129 = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<129, 3, 24> = *b129;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 123;
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{__v[0-9]+}}: bitint::BInt<129, 3, 24> = bitint::BInt::<129, 3, 24>::from_decimal_str("123");
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// LOWERING-NEXT:         let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]);
// LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:         if {{__v[0-9]+}} {
// LOWERING-NEXT:             unsafe { abort() };
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-NEXT:         b"bitint_b101=%lld bitint_ub150=%llu bitint_b256_lo=%lld bitint_ub300_lo=%llu\n\0".as_ptr()
// LOWERING-NEXT:             as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> = *b101;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.to_i128() as i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> = *ub150;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}}.to_u128() as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *b256;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.to_i128() as i64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> = *ub300;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}}.to_u128() as u64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn check_math_functions() {
// LOWERING-NEXT:     let mut exp: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sqrt\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_sqrtl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cbrt\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 216, 3, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_cbrtl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_sinl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cos\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_cosl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"tan\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_tanl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"asin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_asinl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"acos\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_acosl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"atan\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_atanl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"atan2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_atan2l__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"sinh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_sinhl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"cosh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_coshl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"tanh\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_tanhl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"exp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_expl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"exp2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_exp2l__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"log\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_expl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_logl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"log2\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_log2l__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"log10\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 250, 8, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_log10l__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"pow\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_powl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"floor\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_floor({{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ceil\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([102, 102, 102, 102, 102, 102, 102, 134, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_ceil({{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"round\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_round({{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"trunc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_trunc({{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"fabs\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 192]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_abs({{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"fmod\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_fmodl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"hypot\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_hypotl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"copysign\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_copysign({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"fmax\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_fmax({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"fmin\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_fmin({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"fma\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_fma({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ldexp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_ldexpl__rf80_f80_i32({{__v[0-9]+}}, {{__v[0-9]+}} as i32) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     exp = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"frexp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble =
// LOWERING-NEXT:         unsafe { __slate_frexpl__rf80_f80_pi32({{__v[0-9]+}}, std::ptr::addr_of_mut!(exp) as *mut i32) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"frexp_exp=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = exp;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-NEXT:         b"isnan=%d isinf=%d signbit_neg=%d signbit_pos=%d isfinite=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_nanl__rf80_pc({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"epsilon\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
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
// LOWERING-NEXT:     let mut quo: i32 = 0;
// LOWERING-NEXT:     let mut vnan: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vinf: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vzero: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vone: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vsub: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut vtwo: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut ten_plain: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let mut canon: LongDouble = LongDouble([0; 10]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(ten), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(three), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_fract({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_trunc({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"modf_ipart\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"modf_frac\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"remainder\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_remainderl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     quo = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"remquo\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe {
// LOWERING-NEXT:         __slate_remquol__rf80_f80_f80_pi32({{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(quo) as *mut i32)
// LOWERING-NEXT:     };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"remquo_quo=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = quo;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"scalbn\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_scalbnl__rf80_f80_i32({{__v[0-9]+}}, {{__v[0-9]+}} as i32) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"scalbln\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_scalblnl__rf80_f80_i64({{__v[0-9]+}}, {{__v[0-9]+}} as i64) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"nextafter\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_nextafterl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"nexttoward\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_nexttowardl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"fdim\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_fdiml__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"rint\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_rint({{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"nearbyint\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_rint({{__v[0-9]+}});
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"lrint=%ld llrint=%lld lround=%ld llround=%lld\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { __slate_lrintl__ri64_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { __slate_llrintl__ri64_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { __slate_lroundl__ri64_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { __slate_llroundl__ri64_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ilogb=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { __slate_ilogbl__ri32_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"logb\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_logbl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"erf\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_erfl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"erfc\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_erfcl__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"tgamma\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_tgammal__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"lgamma\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_lgammal__rf80_f80({{__v[0-9]+}}) };
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_nanl__rf80_pc({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vnan), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vinf), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vzero), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vone), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vsub), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-NEXT:         b"isnan_v=%d isinf_v=%d isfinite_v=%d isnormal_v=%d isunordered_v=%d isunordered_ok=%d\n\0"
// LOWERING-NEXT:             .as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vinf)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 516);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 504);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 264);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}} || {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vzero)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}} || {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"subnormal_isnormal=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vsub)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 264);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vtwo), {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// LOWERING-NEXT:         b"islessgreater_lt=%d islessgreater_eq=%d islessgreater_nan=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vtwo)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}} || {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}} || {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}} || {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// LOWERING-NEXT:     ten_plain = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     canon = {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         __slate_canonicalizel__ri32_pf80_pf80(
// LOWERING-NEXT:             std::ptr::addr_of_mut!(canon),
// LOWERING-NEXT:             std::ptr::addr_of_mut!(ten_plain),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"canonicalize\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = canon;
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"canonicalize_r=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ldbl_min\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 0]);
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ldbl_true_min\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-NEXT:     print_ld({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"ldbl_mant_dig=%d ldbl_dig=%d ldbl_min_exp=%d ldbl_max_exp=%d ldbl_min_10_exp=%d ldbl_max_10_exp=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 64;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 18;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -16381;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 16384;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -4931;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4932;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn print_ld({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: LongDouble) {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%s=%La\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 =
// LOWERING-NEXT:         unsafe { __slate_printf__ri32_pi8_pi8_f80({{__v[0-9]+}} as *mut i8, {{arg[0-9]+}} as *mut i8, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn __slate_acosl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_asinl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_atan2l__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_atanl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_canonicalizel__ri32_pf80_pf80(_0: *mut LongDouble, _1: *const LongDouble) -> i32;
// LOWERING-NEXT:     fn __slate_cbrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_cf80_div(
// LOWERING-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-NEXT:     fn __slate_coshl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_erfcl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_erfl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_exp2l__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_expl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// LOWERING-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// LOWERING-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
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
// LOWERING-NEXT:     fn __slate_nanl__rf80_pc(_0: *const core::ffi::c_char) -> LongDouble;
// LOWERING-NEXT:     fn __slate_nextafterl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_nexttowardl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_powl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80(_0: *mut i8, _1: *mut i8, _2: LongDouble) -> i32;
// LOWERING-NEXT:     fn __slate_remainderl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-NEXT:     fn __slate_remquol__rf80_f80_f80_pi32(
// LOWERING-NEXT:         _0: LongDouble,
// LOWERING-NEXT:         _1: LongDouble,
// LOWERING-NEXT:         _2: *mut i32,
// LOWERING-NEXT:     ) -> LongDouble;
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
// REWRITES: #![feature(f128)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, align(16))]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct LongDouble([u8; 10]);
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_add(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_div(self, __o)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-NEXT:     type Output = LongDouble;
// REWRITES-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-NEXT:         __slate_f80_neg(self)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-NEXT:             } else {
// REWRITES-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     None
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
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
// REWRITES-NEXT:     fn nanl(_0: *const core::ffi::c_char) -> LongDouble;
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
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d\n".as_ptr(),
// REWRITES-NEXT:             truncate_long_double(
// REWRITES-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63])
// REWRITES-NEXT:                     + LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 1, 64]),
// REWRITES-NEXT:             ),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 1, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = mix_long_double({{__v[0-9]+}}, {{__v[0-9]+}});
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             truncate_long_double({{__v[0-9]+}}),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(7 as i32);
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             truncate_long_double({{__v[0-9]+}} / LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64])),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     check_int_casts();
// REWRITES-NEXT:     check_i128_casts();
// REWRITES-NEXT:     check_bitint_casts();
// REWRITES-NEXT:     check_math_functions();
// REWRITES-NEXT:     check_remaining_math_functions();
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn truncate_long_double({{arg[0-9]+}}: LongDouble) -> i32 {
// REWRITES-NEXT:     __slate_f80_to_i32({{arg[0-9]+}})
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn mix_long_double({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-NEXT:     ({{arg[0-9]+}} + {{arg[0-9]+}}) / LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64])
// REWRITES-NEXT:         * LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64])
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_int_casts() {
// REWRITES-NEXT:     let mut i8: i8 = 0;
// REWRITES-NEXT:     let mut u8: u8 = 0;
// REWRITES-NEXT:     let mut i16: i16 = 0;
// REWRITES-NEXT:     let mut u16: u16 = 0;
// REWRITES-NEXT:     let mut i32: i32 = 0;
// REWRITES-NEXT:     let mut u32: u32 = 0;
// REWRITES-NEXT:     let mut i64: i64 = -123456789012345i64;
// REWRITES-NEXT:     let mut u64: u64 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// REWRITES-NEXT:     i8 = __slate_f80_to_i8({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i8(i8);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// REWRITES-NEXT:     u8 = __slate_f80_to_u8({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u8(u8);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// REWRITES-NEXT:     i16 = __slate_f80_to_i16({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i16(i16);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]);
// REWRITES-NEXT:     u16 = __slate_f80_to_u16({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u16(u16);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 0, 0, 49, 212, 14, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]);
// REWRITES-NEXT:     i32 = __slate_f80_to_i32({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i32(i32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 164, 5, 44, 147, 29, 192]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]);
// REWRITES-NEXT:     u32 = __slate_f80_to_u32({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u32(u32);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 20, 106, 10, 206, 30, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i64(i64);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]);
// REWRITES-NEXT:     u64 = __slate_f80_to_u64({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u64(u64);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([210, 10, 31, 235, 140, 169, 84, 171, 62, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"i8=%d u8=%u i16=%d u16=%u i32=%d u32=%u i64=%lld u64=%llu\n".as_ptr(),
// REWRITES-NEXT:             i8 as i32,
// REWRITES-NEXT:             u8 as i32,
// REWRITES-NEXT:             i16 as i32,
// REWRITES-NEXT:             u16 as i32,
// REWRITES-NEXT:             i32,
// REWRITES-NEXT:             u32,
// REWRITES-NEXT:             i64,
// REWRITES-NEXT:             u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_i128_casts() {
// REWRITES-NEXT:     let mut i128: i128 = 0;
// REWRITES-NEXT:     let mut u128: u128 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192]);
// REWRITES-NEXT:     i128 = __slate_f80_to_i128({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128(i128);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([254, 255, 255, 255, 255, 255, 255, 255, 61, 192]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]);
// REWRITES-NEXT:     u128 = __slate_f80_to_u128({{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128(u128);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 62, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"i128=%lld u128_hi=%llu u128_lo=%llu\n".as_ptr(),
// REWRITES-NEXT:             i128 as i64,
// REWRITES-NEXT:             (u128 >> (64 as i32)) as u64,
// REWRITES-NEXT:             (u128 & 18446744073709551615u128) as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_bitint_casts() {
// REWRITES-NEXT:     let mut b9: aligned::Aligned<aligned::A2, bitint::BInt<9, 1, 2>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BInt::<9, 1, 2>::ZERO);
// REWRITES-NEXT:     let mut ub9: aligned::Aligned<aligned::A2, bitint::BUint<9, 1, 2>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BUint::<9, 1, 2>::ZERO);
// REWRITES-NEXT:     let mut b40: aligned::Aligned<aligned::A8, bitint::BInt<40, 1, 8>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BInt::<40, 1, 8>::ZERO);
// REWRITES-NEXT:     let mut ub40: aligned::Aligned<aligned::A8, bitint::BUint<40, 1, 8>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BUint::<40, 1, 8>::ZERO);
// REWRITES-NEXT:     let mut b101: aligned::Aligned<aligned::A8, bitint::BInt<101, 2, 16>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BInt::<101, 2, 16>::ZERO);
// REWRITES-NEXT:     let mut ub150: aligned::Aligned<aligned::A8, bitint::BUint<150, 3, 24>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BUint::<150, 3, 24>::ZERO);
// REWRITES-NEXT:     let mut b256: aligned::Aligned<aligned::A8, bitint::BInt<256, 4, 32>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BInt::<256, 4, 32>::ZERO);
// REWRITES-NEXT:     let mut ub300: aligned::Aligned<aligned::A8, bitint::BUint<300, 5, 40>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BUint::<300, 5, 40>::ZERO);
// REWRITES-NEXT:     let mut b129: aligned::Aligned<aligned::A8, bitint::BInt<129, 3, 24>> =
// REWRITES-NEXT:         aligned::Aligned(bitint::BInt::<129, 3, 24>::ZERO);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<9, 1, 2> =
// REWRITES-NEXT:         bitint::BInt::<9, 1, 2>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// REWRITES-NEXT:     *b9 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<9, 1, 2> = *b9;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 192]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<9, 1, 2> =
// REWRITES-NEXT:         bitint::BUint::<9, 1, 2>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// REWRITES-NEXT:     *ub9 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<9, 1, 2> = *ub9;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 6, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<40, 1, 8> =
// REWRITES-NEXT:         bitint::BInt::<40, 1, 8>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// REWRITES-NEXT:     *b40 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<40, 1, 8> = *b40;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 160, 162, 121, 235, 25, 192]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<40, 1, 8> =
// REWRITES-NEXT:         bitint::BUint::<40, 1, 8>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// REWRITES-NEXT:     *ub40 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<40, 1, 8> = *ub40;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 196, 162, 121, 235, 28, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> =
// REWRITES-NEXT:         bitint::BInt::<101, 2, 16>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// REWRITES-NEXT:     *b101 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> = *b101;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 242, 190, 27, 12, 145, 224, 45, 192]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> = *b101;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> =
// REWRITES-NEXT:         bitint::BInt::<101, 2, 16>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != *b101;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> =
// REWRITES-NEXT:         bitint::BUint::<150, 3, 24>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// REWRITES-NEXT:     *ub150 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> = *ub150;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 64, 3, 20, 62, 12, 145, 224, 48, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> = *ub150;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> =
// REWRITES-NEXT:         bitint::BUint::<150, 3, 24>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != *ub150;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// REWRITES-NEXT:         bitint::BInt::<256, 4, 32>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// REWRITES-NEXT:     *b256 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *b256;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> =
// REWRITES-NEXT:         bitint::BInt::<256, 4, 32>::from_decimal_str("9999999999");
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *b256;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 192, 255, 248, 2, 149, 32, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> =
// REWRITES-NEXT:         bitint::BUint::<300, 5, 40>::from_u128(__slate_f80_to_u128({{__v[0-9]+}}) as u128);
// REWRITES-NEXT:     *ub300 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> = *ub300;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> =
// REWRITES-NEXT:         bitint::BUint::<300, 5, 40>::from_decimal_str("4200000000");
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> = *ub300;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_u128({{__v[0-9]+}}.to_u128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 0, 234, 86, 250, 30, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<129, 3, 24> =
// REWRITES-NEXT:         bitint::BInt::<129, 3, 24>::from_i128(__slate_f80_to_i128({{__v[0-9]+}}) as i128);
// REWRITES-NEXT:     *b129 = {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<129, 3, 24> = *b129;
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.to_i128() as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 123;
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<129, 3, 24> = bitint::BInt::<129, 3, 24>::from_decimal_str("123");
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_from_i128({{__v[0-9]+}}.to_i128());
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != LongDouble([0, 0, 0, 0, 0, 0, 0, 246, 5, 64]);
// REWRITES-NEXT:     if {{__v[0-9]+}} {
// REWRITES-NEXT:         unsafe { abort() };
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<101, 2, 16> = *b101;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<150, 3, 24> = *ub150;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BInt<256, 4, 32> = *b256;
// REWRITES-NEXT:     let {{__v[0-9]+}}: bitint::BUint<300, 5, 40> = *ub300;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"bitint_b101=%lld bitint_ub150=%llu bitint_b256_lo=%lld bitint_ub300_lo=%llu\n"
// REWRITES-NEXT:                 .as_ptr(),
// REWRITES-NEXT:             {{__v[0-9]+}}.to_i128() as i64,
// REWRITES-NEXT:             {{__v[0-9]+}}.to_u128() as u64,
// REWRITES-NEXT:             {{__v[0-9]+}}.to_i128() as i64,
// REWRITES-NEXT:             {{__v[0-9]+}}.to_u128() as u64,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn check_math_functions() {
// REWRITES-NEXT:     let mut exp: i32 = 0;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT:     print_ld(c"sqrt".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_sqrtl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 216, 3, 64]);
// REWRITES-NEXT:     print_ld(c"cbrt".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_cbrtl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     print_ld(c"sin".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_sinl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     print_ld(c"cos".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_cosl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     print_ld(c"tan".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_tanl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"asin".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_asinl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"acos".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_acosl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"atan".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_atanl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"atan2".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_atan2l__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"sinh".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_sinhl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"cosh".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_coshl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"tanh".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_tanhl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"exp".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_expl__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-NEXT:     print_ld(c"exp2".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_exp2l__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"log".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_expl__rf80_f80({{__v[0-9]+}}) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe { __slate_logl__rf80_f80({{__v[0-9]+}}) });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// REWRITES-NEXT:     print_ld(c"log2".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_log2l__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 250, 8, 64]);
// REWRITES-NEXT:     print_ld(c"log10".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_log10l__rf80_f80({{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-NEXT:     print_ld(c"pow".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_powl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 64]);
// REWRITES-NEXT:     print_ld(c"floor".as_ptr() as *mut i8, __slate_f80_floor({{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([102, 102, 102, 102, 102, 102, 102, 134, 0, 64]);
// REWRITES-NEXT:     print_ld(c"ceil".as_ptr() as *mut i8, __slate_f80_ceil({{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]);
// REWRITES-NEXT:     print_ld(c"round".as_ptr() as *mut i8, __slate_f80_round({{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([205, 204, 204, 204, 204, 204, 204, 172, 0, 192]);
// REWRITES-NEXT:     print_ld(c"trunc".as_ptr() as *mut i8, __slate_f80_trunc({{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 192]);
// REWRITES-NEXT:     print_ld(c"fabs".as_ptr() as *mut i8, __slate_f80_abs({{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:     print_ld(c"fmod".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_fmodl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// REWRITES-NEXT:     print_ld(c"hypot".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_hypotl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// REWRITES-NEXT:     print_ld(
// REWRITES-NEXT:         c"copysign".as_ptr() as *mut i8,
// REWRITES-NEXT:         __slate_f80_copysign({{__v[0-9]+}}, {{__v[0-9]+}}),
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT:     print_ld(c"fmax".as_ptr() as *mut i8, __slate_f80_fmax({{__v[0-9]+}}, {{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT:     print_ld(c"fmin".as_ptr() as *mut i8, __slate_f80_fmin({{__v[0-9]+}}, {{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// REWRITES-NEXT:     print_ld(
// REWRITES-NEXT:         c"fma".as_ptr() as *mut i8,
// REWRITES-NEXT:         __slate_f80_fma({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}),
// REWRITES-NEXT:     );
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     print_ld(c"ldexp".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_ldexpl__rf80_f80_i32({{__v[0-9]+}}, 4 as i32)
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 5, 64]);
// REWRITES-NEXT:     print_ld(c"frexp".as_ptr() as *mut i8, unsafe {
// REWRITES-NEXT:         __slate_frexpl__rf80_f80_pi32({{__v[0-9]+}}, std::ptr::addr_of_mut!(exp) as *mut i32)
// REWRITES-NEXT:     });
// REWRITES-NEXT:     unsafe { printf(c"frexp_exp=%d\n".as_ptr(), exp) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-NEXT:         c"isnan=%d isinf=%d signbit_neg=%d signbit_pos=%d isfinite=%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { __slate_nanl__rf80_pc(c"".as_ptr()) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             __slate_f80_is_fp_class({{__v[0-9]+}}, 3) as i32,
// REWRITES-NEXT:             1 as i32,
// REWRITES-NEXT:             1 as i32,
// REWRITES-NEXT:             0 as i32,
// REWRITES-NEXT:             1 as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// REWRITES-NEXT:     print_ld(c"epsilon".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: /// The functions above all round-trip through the generic call-shim (any
// REWRITES-NEXT: /// known extern function with a long double arg/return links straight to
// REWRITES-NEXT: /// libm), which check_math_functions already exercises. This covers the
// REWRITES-NEXT: /// remaining libm entry points -- pointer out-params, integer-returning
// REWRITES-NEXT: /// variants, and the classification family -- with volatile operands so
// REWRITES-NEXT: /// they can't constant-fold away and skip the real runtime path.
// REWRITES-NEXT: fn check_remaining_math_functions() {
// REWRITES-NEXT:     let mut ten: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut three: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut quo: i32 = 0;
// REWRITES-NEXT:     let mut vnan: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut vinf: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut vzero: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut vone: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut vsub: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut vtwo: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut ten_plain: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let mut canon: LongDouble = LongDouble([0; 10]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(ten), {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(three), {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = {{__v[0-9]+}} / {{__v[0-9]+}};
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = __slate_f80_fract({{__v[0-9]+}});
// REWRITES-NEXT:     print_ld(c"modf_ipart".as_ptr() as *mut i8, __slate_f80_trunc({{__v[0-9]+}}));
// REWRITES-NEXT:     print_ld(c"modf_frac".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"remainder".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe {
// REWRITES-NEXT:         __slate_remainderl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"remquo".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe {
// REWRITES-NEXT:         __slate_remquol__rf80_f80_f80_pi32({{__v[0-9]+}}, {{__v[0-9]+}}, std::ptr::addr_of_mut!(quo) as *mut i32)
// REWRITES-NEXT:     });
// REWRITES-NEXT:     unsafe { printf(c"remquo_quo=%d\n".as_ptr(), quo) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"scalbn".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe {
// REWRITES-NEXT:         __slate_scalbnl__rf80_f80_i32({{__v[0-9]+}}, 3 as i32)
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"scalbln".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe {
// REWRITES-NEXT:         __slate_scalblnl__rf80_f80_i64({{__v[0-9]+}}, 3 as i64)
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"nextafter".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe {
// REWRITES-NEXT:         __slate_nextafterl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"nexttoward".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe {
// REWRITES-NEXT:         __slate_nexttowardl__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"fdim".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe { __slate_fdiml__rf80_f80_f80({{__v[0-9]+}}, {{__v[0-9]+}}) });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"rint".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, __slate_f80_rint({{__v[0-9]+}} / {{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"nearbyint".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, __slate_f80_rint({{__v[0-9]+}} / {{__v[0-9]+}}));
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"lrint=%ld llrint=%lld lround=%ld llround=%lld\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { __slate_lrintl__ri64_f80({{__v[0-9]+}} / {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { __slate_llrintl__ri64_f80({{__v[0-9]+}} / {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i64 = unsafe { __slate_lroundl__ri64_f80({{__v[0-9]+}} / {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             unsafe { __slate_llroundl__ri64_f80({{__v[0-9]+}} / {{__v[0-9]+}}) },
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"ilogb=%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf({{__v[0-9]+}} as *const core::ffi::c_char, unsafe {
// REWRITES-NEXT:             __slate_ilogbl__ri32_f80({{__v[0-9]+}})
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"logb".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe { __slate_logbl__rf80_f80({{__v[0-9]+}}) });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"erf".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe { __slate_erfl__rf80_f80({{__v[0-9]+}} / {{__v[0-9]+}}) });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"erfc".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe { __slate_erfcl__rf80_f80({{__v[0-9]+}} / {{__v[0-9]+}}) });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"tgamma".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(three)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe { __slate_tgammal__rf80_f80({{__v[0-9]+}}) });
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"lgamma".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     print_ld({{__v[0-9]+}}, unsafe { __slate_lgammal__rf80_f80({{__v[0-9]+}}) });
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(vnan), unsafe {
// REWRITES-NEXT:             __slate_nanl__rf80_pc(c"".as_ptr())
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 127]);
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vinf), {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vzero), {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vone), {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vsub), {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-NEXT:         c"isnan_v=%d isinf_v=%d isfinite_v=%d isnormal_v=%d isunordered_v=%d isunordered_ok=%d\n"
// REWRITES-NEXT:             .as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vinf)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 516);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 504);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 264);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} != {{__v[0-9]+}} || {{__v[0-9]+}} != {{__v[0-9]+}}) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vzero)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             ({{__v[0-9]+}} != {{__v[0-9]+}} || {{__v[0-9]+}} != {{__v[0-9]+}}) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"subnormal_isnormal=%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vsub)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             __slate_f80_is_fp_class({{__v[0-9]+}}, 264) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(vtwo), {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: *mut i8 =
// REWRITES-NEXT:         c"islessgreater_lt=%d islessgreater_eq=%d islessgreater_nan=%d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vtwo)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} < {{__v[0-9]+}} || {{__v[0-9]+}} > {{__v[0-9]+}}) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} < {{__v[0-9]+}} || {{__v[0-9]+}} > {{__v[0-9]+}}) as i32;
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vnan)) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(vone)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             {{__v[0-9]+}},
// REWRITES-NEXT:             ({{__v[0-9]+}} < {{__v[0-9]+}} || {{__v[0-9]+}} > {{__v[0-9]+}}) as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     ten_plain = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) };
// REWRITES-NEXT:     canon = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// REWRITES-NEXT:         __slate_canonicalizel__ri32_pf80_pf80(
// REWRITES-NEXT:             std::ptr::addr_of_mut!(canon),
// REWRITES-NEXT:             std::ptr::addr_of_mut!(ten_plain),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     print_ld(c"canonicalize".as_ptr() as *mut i8, canon);
// REWRITES-NEXT:     unsafe { printf(c"canonicalize_r=%d\n".as_ptr(), {{__v[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 0]);
// REWRITES-NEXT:     print_ld(c"ldbl_min".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-NEXT:     let {{__v[0-9]+}}: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-NEXT:     print_ld(c"ldbl_true_min".as_ptr() as *mut i8, {{__v[0-9]+}});
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(c"ldbl_mant_dig=%d ldbl_dig=%d ldbl_min_exp=%d ldbl_max_exp=%d ldbl_min_10_exp=%d ldbl_max_10_exp=%d\n".as_ptr(), 64 as i32, 18 as i32, -16381 as i32, 16384 as i32, -4931 as i32, 4932 as i32)
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn print_ld({{arg[0-9]+}}: *mut i8, {{arg[0-9]+}}: LongDouble) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         __slate_printf__ri32_pi8_pi8_f80(c"%s=%La\n".as_ptr() as *mut i8, {{arg[0-9]+}} as *mut i8, {{arg[0-9]+}})
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn __slate_acosl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_asinl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_atan2l__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_atanl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_canonicalizel__ri32_pf80_pf80(_0: *mut LongDouble, _1: *const LongDouble) -> i32;
// REWRITES-NEXT:     fn __slate_cbrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_cf80_div(
// REWRITES-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-NEXT:     fn __slate_coshl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_cosl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_erfcl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_erfl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_exp2l__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_expl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f128_nexttoward(__from: f128, __toward: f128) -> f128;
// REWRITES-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// REWRITES-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
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
// REWRITES-NEXT:     fn __slate_nanl__rf80_pc(_0: *const core::ffi::c_char) -> LongDouble;
// REWRITES-NEXT:     fn __slate_nextafterl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_nexttowardl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_powl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_printf__ri32_pi8_pi8_f80(_0: *mut i8, _1: *mut i8, _2: LongDouble) -> i32;
// REWRITES-NEXT:     fn __slate_remainderl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-NEXT:     fn __slate_remquol__rf80_f80_f80_pi32(
// REWRITES-NEXT:         _0: LongDouble,
// REWRITES-NEXT:         _1: LongDouble,
// REWRITES-NEXT:         _2: *mut i32,
// REWRITES-NEXT:     ) -> LongDouble;
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
