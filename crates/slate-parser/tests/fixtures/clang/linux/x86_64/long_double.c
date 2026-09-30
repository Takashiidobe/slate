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
// DEFAULT-NEXT:     type @type[[TYPE___int8_t:[0-9]+]] __int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE___uint8_t:[0-9]+]] __uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE___int16_t:[0-9]+]] __int16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE___uint16_t:[0-9]+]] __uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE___int32_t:[0-9]+]] __int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE___uint32_t:[0-9]+]] __uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE___int64_t:[0-9]+]] __int64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE___uint64_t:[0-9]+]] __uint64_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_int8_t:[0-9]+]] int8_t = i8;
// DEFAULT-NEXT:     type @type[[TYPE_int16_t:[0-9]+]] int16_t = i16;
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE_int64_t:[0-9]+]] int64_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_uint8_t:[0-9]+]] uint8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_uint16_t:[0-9]+]] uint16_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_uint32_t:[0-9]+]] uint32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_uint64_t:[0-9]+]] uint64_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 115, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 59> [storage=static] = code_units<array<i8, 59>>([105, 56, 61, 37, 100, 32, 117, 56, 61, 37, 117, 32, 105, 49, 54, 61, 37, 100, 32, 117, 49, 54, 61, 37, 117, 32, 105, 51, 50, 61, 37, 100, 32, 117, 51, 50, 61, 37, 117, 32, 105, 54, 52, 61, 37, 108, 108, 100, 32, 117, 54, 52, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([105, 49, 50, 56, 61, 37, 108, 108, 100, 32, 117, 49, 50, 56, 95, 104, 105, 61, 37, 108, 108, 117, 32, 117, 49, 50, 56, 95, 108, 111, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 77> [storage=static] = code_units<array<i8, 77>>([98, 105, 116, 105, 110, 116, 95, 98, 49, 48, 49, 61, 37, 108, 108, 100, 32, 98, 105, 116, 105, 110, 116, 95, 117, 98, 49, 53, 48, 61, 37, 108, 108, 117, 32, 98, 105, 116, 105, 110, 116, 95, 98, 50, 53, 54, 95, 108, 111, 61, 37, 108, 108, 100, 32, 98, 105, 116, 105, 110, 116, 95, 117, 98, 51, 48, 48, 95, 108, 111, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([115, 113, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 98, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 116, 97, 110, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_16:[0-9]+]] .str[[VALUE_str_16]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_17:[0-9]+]] .str[[VALUE_str_17]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_18:[0-9]+]] .str[[VALUE_str_18]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([101, 120, 112, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_19:[0-9]+]] .str[[VALUE_str_19]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([108, 111, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_20:[0-9]+]] .str[[VALUE_str_20]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 103, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_21:[0-9]+]] .str[[VALUE_str_21]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([108, 111, 103, 49, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_22:[0-9]+]] .str[[VALUE_str_22]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([112, 111, 119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_23:[0-9]+]] .str[[VALUE_str_23]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 108, 111, 111, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_24:[0-9]+]] .str[[VALUE_str_24]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 101, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_25:[0-9]+]] .str[[VALUE_str_25]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 111, 117, 110, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_26:[0-9]+]] .str[[VALUE_str_26]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([116, 114, 117, 110, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_27:[0-9]+]] .str[[VALUE_str_27]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 97, 98, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_28:[0-9]+]] .str[[VALUE_str_28]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 111, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_29:[0-9]+]] .str[[VALUE_str_29]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 121, 112, 111, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_30:[0-9]+]] .str[[VALUE_str_30]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([99, 111, 112, 121, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_31:[0-9]+]] .str[[VALUE_str_31]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 97, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_32:[0-9]+]] .str[[VALUE_str_32]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_33:[0-9]+]] .str[[VALUE_str_33]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_34:[0-9]+]] .str[[VALUE_str_34]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([108, 100, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_35:[0-9]+]] .str[[VALUE_str_35]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 114, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_36:[0-9]+]] .str[[VALUE_str_36]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 114, 101, 120, 112, 95, 101, 120, 112, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_37:[0-9]+]] .str[[VALUE_str_37]]: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([105, 115, 110, 97, 110, 61, 37, 100, 32, 105, 115, 105, 110, 102, 61, 37, 100, 32, 115, 105, 103, 110, 98, 105, 116, 95, 110, 101, 103, 61, 37, 100, 32, 115, 105, 103, 110, 98, 105, 116, 95, 112, 111, 115, 61, 37, 100, 32, 105, 115, 102, 105, 110, 105, 116, 101, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_38:[0-9]+]] .str[[VALUE_str_38]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_39:[0-9]+]] .str[[VALUE_str_39]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([101, 112, 115, 105, 108, 111, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_40:[0-9]+]] .str[[VALUE_str_40]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([109, 111, 100, 102, 95, 105, 112, 97, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_41:[0-9]+]] .str[[VALUE_str_41]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([109, 111, 100, 102, 95, 102, 114, 97, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_42:[0-9]+]] .str[[VALUE_str_42]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([114, 101, 109, 97, 105, 110, 100, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_43:[0-9]+]] .str[[VALUE_str_43]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([114, 101, 109, 113, 117, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_44:[0-9]+]] .str[[VALUE_str_44]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([114, 101, 109, 113, 117, 111, 95, 113, 117, 111, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_45:[0-9]+]] .str[[VALUE_str_45]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([115, 99, 97, 108, 98, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_46:[0-9]+]] .str[[VALUE_str_46]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([115, 99, 97, 108, 98, 108, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_47:[0-9]+]] .str[[VALUE_str_47]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([110, 101, 120, 116, 97, 102, 116, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_48:[0-9]+]] .str[[VALUE_str_48]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 120, 116, 116, 111, 119, 97, 114, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_49:[0-9]+]] .str[[VALUE_str_49]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 100, 105, 109, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_50:[0-9]+]] .str[[VALUE_str_50]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([114, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_51:[0-9]+]] .str[[VALUE_str_51]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([110, 101, 97, 114, 98, 121, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_52:[0-9]+]] .str[[VALUE_str_52]]: array<i8, 47> [storage=static] = code_units<array<i8, 47>>([108, 114, 105, 110, 116, 61, 37, 108, 100, 32, 108, 108, 114, 105, 110, 116, 61, 37, 108, 108, 100, 32, 108, 114, 111, 117, 110, 100, 61, 37, 108, 100, 32, 108, 108, 114, 111, 117, 110, 100, 61, 37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_53:[0-9]+]] .str[[VALUE_str_53]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([105, 108, 111, 103, 98, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_54:[0-9]+]] .str[[VALUE_str_54]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 103, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_55:[0-9]+]] .str[[VALUE_str_55]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([101, 114, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_56:[0-9]+]] .str[[VALUE_str_56]]: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([101, 114, 102, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_57:[0-9]+]] .str[[VALUE_str_57]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([116, 103, 97, 109, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_58:[0-9]+]] .str[[VALUE_str_58]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([108, 103, 97, 109, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_59:[0-9]+]] .str[[VALUE_str_59]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_60:[0-9]+]] .str[[VALUE_str_60]]: array<i8, 86> [storage=static] = code_units<array<i8, 86>>([105, 115, 110, 97, 110, 95, 118, 61, 37, 100, 32, 105, 115, 105, 110, 102, 95, 118, 61, 37, 100, 32, 105, 115, 102, 105, 110, 105, 116, 101, 95, 118, 61, 37, 100, 32, 105, 115, 110, 111, 114, 109, 97, 108, 95, 118, 61, 37, 100, 32, 105, 115, 117, 110, 111, 114, 100, 101, 114, 101, 100, 95, 118, 61, 37, 100, 32, 105, 115, 117, 110, 111, 114, 100, 101, 114, 101, 100, 95, 111, 107, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_61:[0-9]+]] .str[[VALUE_str_61]]: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 117, 98, 110, 111, 114, 109, 97, 108, 95, 105, 115, 110, 111, 114, 109, 97, 108, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_62:[0-9]+]] .str[[VALUE_str_62]]: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 108, 116, 61, 37, 100, 32, 105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 101, 113, 61, 37, 100, 32, 105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 110, 97, 110, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_63:[0-9]+]] .str[[VALUE_str_63]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([99, 97, 110, 111, 110, 105, 99, 97, 108, 105, 122, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_64:[0-9]+]] .str[[VALUE_str_64]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([99, 97, 110, 111, 110, 105, 99, 97, 108, 105, 122, 101, 95, 114, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_65:[0-9]+]] .str[[VALUE_str_65]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([108, 100, 98, 108, 95, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_66:[0-9]+]] .str[[VALUE_str_66]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([108, 100, 98, 108, 95, 116, 114, 117, 101, 95, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_67:[0-9]+]] .str[[VALUE_str_67]]: array<i8, 100> [storage=static] = code_units<array<i8, 100>>([108, 100, 98, 108, 95, 109, 97, 110, 116, 95, 100, 105, 103, 61, 37, 100, 32, 108, 100, 98, 108, 95, 100, 105, 103, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 105, 110, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 97, 120, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 105, 110, 95, 49, 48, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 97, 120, 95, 49, 48, 95, 101, 120, 112, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_68:[0-9]+]] .str[[VALUE_str_68]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_69:[0-9]+]] .str[[VALUE_str_69]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_70:[0-9]+]] .str[[VALUE_str_70]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_exp:[0-9]+]] @exp(%[[VALUE___x:[0-9]+]] __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acosl:[0-9]+]] @acosl(%[[VALUE___x_2:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinl:[0-9]+]] @asinl(%[[VALUE___x_3:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanl:[0-9]+]] @atanl(%[[VALUE___x_4:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan2l:[0-9]+]] @atan2l(%[[VALUE___y:[0-9]+]] __y: f80, %[[VALUE___x_5:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosl:[0-9]+]] @cosl(%[[VALUE___x_6:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinl:[0-9]+]] @sinl(%[[VALUE___x_7:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanl:[0-9]+]] @tanl(%[[VALUE___x_8:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_coshl:[0-9]+]] @coshl(%[[VALUE___x_9:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinhl:[0-9]+]] @sinhl(%[[VALUE___x_10:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanhl:[0-9]+]] @tanhl(%[[VALUE___x_11:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expl:[0-9]+]] @expl(%[[VALUE___x_12:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frexpl:[0-9]+]] @frexpl(%[[VALUE___x_13:[0-9]+]] __x: f80, %[[VALUE___exponent:[0-9]+]] __exponent: ptr<i32>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ldexpl:[0-9]+]] @ldexpl(%[[VALUE___x_14:[0-9]+]] __x: f80, %[[VALUE___exponent_2:[0-9]+]] __exponent: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logl:[0-9]+]] @logl(%[[VALUE___x_15:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log10l:[0-9]+]] @log10l(%[[VALUE___x_16:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_modfl:[0-9]+]] @modfl(%[[VALUE___x_17:[0-9]+]] __x: f80, %[[VALUE___iptr:[0-9]+]] __iptr: ptr<f80>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logbl:[0-9]+]] @logbl(%[[VALUE___x_18:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp2l:[0-9]+]] @exp2l(%[[VALUE___x_19:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log2l:[0-9]+]] @log2l(%[[VALUE___x_20:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_powl:[0-9]+]] @powl(%[[VALUE___x_21:[0-9]+]] __x: f80, %[[VALUE___y_2:[0-9]+]] __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrtl:[0-9]+]] @sqrtl(%[[VALUE___x_22:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hypotl:[0-9]+]] @hypotl(%[[VALUE___x_23:[0-9]+]] __x: f80, %[[VALUE___y_3:[0-9]+]] __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cbrtl:[0-9]+]] @cbrtl(%[[VALUE___x_24:[0-9]+]] __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceill:[0-9]+]] @ceill(%[[VALUE___x_25:[0-9]+]] __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsl:[0-9]+]] @fabsl(%[[VALUE___x_26:[0-9]+]] __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_floorl:[0-9]+]] @floorl(%[[VALUE___x_27:[0-9]+]] __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmodl:[0-9]+]] @fmodl(%[[VALUE___x_28:[0-9]+]] __x: f80, %[[VALUE___y_4:[0-9]+]] __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_copysignl:[0-9]+]] @copysignl(%[[VALUE___x_29:[0-9]+]] __x: f80, %[[VALUE___y_5:[0-9]+]] __y: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nanl:[0-9]+]] @nanl(%[[VALUE___tagb:[0-9]+]] __tagb: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_erfl:[0-9]+]] @erfl(%[[VALUE0:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erfcl:[0-9]+]] @erfcl(%[[VALUE1:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lgammal:[0-9]+]] @lgammal(%[[VALUE2:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tgammal:[0-9]+]] @tgammal(%[[VALUE3:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rintl:[0-9]+]] @rintl(%[[VALUE___x_30:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nextafterl:[0-9]+]] @nextafterl(%[[VALUE___x_31:[0-9]+]] __x: f80, %[[VALUE___y_6:[0-9]+]] __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nexttowardl:[0-9]+]] @nexttowardl(%[[VALUE___x_32:[0-9]+]] __x: f80, %[[VALUE___y_7:[0-9]+]] __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remainderl:[0-9]+]] @remainderl(%[[VALUE___x_33:[0-9]+]] __x: f80, %[[VALUE___y_8:[0-9]+]] __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbnl:[0-9]+]] @scalbnl(%[[VALUE___x_34:[0-9]+]] __x: f80, %[[VALUE___n:[0-9]+]] __n: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ilogbl:[0-9]+]] @ilogbl(%[[VALUE___x_35:[0-9]+]] __x: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalblnl:[0-9]+]] @scalblnl(%[[VALUE___x_36:[0-9]+]] __x: f80, %[[VALUE___n_2:[0-9]+]] __n: i64) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nearbyintl:[0-9]+]] @nearbyintl(%[[VALUE___x_37:[0-9]+]] __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_roundl:[0-9]+]] @roundl(%[[VALUE___x_38:[0-9]+]] __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_truncl:[0-9]+]] @truncl(%[[VALUE___x_39:[0-9]+]] __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_remquol:[0-9]+]] @remquol(%[[VALUE___x_40:[0-9]+]] __x: f80, %[[VALUE___y_9:[0-9]+]] __y: f80, %[[VALUE___quo:[0-9]+]] __quo: ptr<i32>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lrintl:[0-9]+]] @lrintl(%[[VALUE___x_41:[0-9]+]] __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llrintl:[0-9]+]] @llrintl(%[[VALUE___x_42:[0-9]+]] __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lroundl:[0-9]+]] @lroundl(%[[VALUE___x_43:[0-9]+]] __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llroundl:[0-9]+]] @llroundl(%[[VALUE___x_44:[0-9]+]] __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fdiml:[0-9]+]] @fdiml(%[[VALUE___x_45:[0-9]+]] __x: f80, %[[VALUE___y_10:[0-9]+]] __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmaxl:[0-9]+]] @fmaxl(%[[VALUE___x_46:[0-9]+]] __x: f80, %[[VALUE___y_11:[0-9]+]] __y: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fminl:[0-9]+]] @fminl(%[[VALUE___x_47:[0-9]+]] __x: f80, %[[VALUE___y_12:[0-9]+]] __y: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmal:[0-9]+]] @fmal(%[[VALUE___x_48:[0-9]+]] __x: f80, %[[VALUE___y_13:[0-9]+]] __y: f80, %[[VALUE___z:[0-9]+]] __z: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_canonicalizel:[0-9]+]] @canonicalizel(%[[VALUE___cx:[0-9]+]] __cx: ptr<f80>, %[[VALUE___x_49:[0-9]+]] __x: ptr<const f80>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_mix_long_double:[0-9]+]] @mix_long_double(%[[VALUE_a:[0-9]+]] a: f80, %[[VALUE_b:[0-9]+]] b: f80) -> f80 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: f80 [storage=automatic] = div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_a]]), read<f80>(%[[VALUE_b]])), const<f80>(2));
// DEFAULT-NEXT:         return mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_c]]), const<f80>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_truncate_long_double:[0-9]+]] @truncate_long_double(%[[VALUE_value:[0-9]+]] value: f80) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f80>(%[[VALUE_value]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_print_ld:[0-9]+]] @print_ld(%[[VALUE_name:[0-9]+]] name: ptr<const i8>, %[[VALUE_v:[0-9]+]] v: f80) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str]])), read<ptr<const i8>>(%[[VALUE_name]]), read<f80>(%[[VALUE_v]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_int_casts:[0-9]+]] @check_int_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i8:[0-9]+]] i8: i8 [storage=automatic] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(100)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(%[[VALUE_i8]])), neg<f80>(const<f80>(100)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_u8:[0-9]+]] u8: u8 [storage=automatic] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(200));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(%[[VALUE_u8]])), const<f80>(200))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_i16:[0-9]+]] i16: i16 [storage=automatic] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(12345)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(%[[VALUE_i16]])), neg<f80>(const<f80>(12345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_u16:[0-9]+]] u16: u16 [storage=automatic] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(54321));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(%[[VALUE_u16]])), const<f80>(54321))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_i32:[0-9]+]] i32: i32 [storage=automatic] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(1234567890)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i32]])), neg<f80>(const<f80>(1234567890)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_u32:[0-9]+]] u32: u32 [storage=automatic] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(3456789012));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u32>(%[[VALUE_u32]])), const<f80>(3456789012))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_i64:[0-9]+]] i64: i64 [storage=automatic] = neg<i64, overflow=ub>(const<i64>(123456789012345));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i64>(%[[VALUE_i64]])), neg<f80>(const<f80>(123456789012345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_u64:[0-9]+]] u64: u64 [storage=automatic] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(12345678901234567890));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u64>(%[[VALUE_u64]])), const<f80>(12345678901234567890))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(59)>(%[[VALUE_str_2]])), widen<i32, reason=vararg>(read<i8>(%[[VALUE_i8]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%[[VALUE_u8]]))), widen<i32, reason=vararg>(read<i16>(%[[VALUE_i16]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u16>(%[[VALUE_u16]]))), read<i32>(%[[VALUE_i32]]), read<u32>(%[[VALUE_u32]]), read<i64>(%[[VALUE_i64]]), read<u64>(%[[VALUE_u64]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_i128_casts:[0-9]+]] @check_i128_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i128:[0-9]+]] i128: i128 [storage=automatic] = float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(9223372036854775807)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i128>(%[[VALUE_i128]])), neg<f80>(const<f80>(9223372036854775807)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_u128:[0-9]+]] u128: u128 [storage=automatic] = float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(18446744073709551615));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u128>(%[[VALUE_u128]])), const<f80>(18446744073709551615))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(37)>(%[[VALUE_str_3]])), truncate<i64, reason=explicit, fits=unknown>(read<i128>(%[[VALUE_i128]])), truncate<u64, reason=explicit, fits=unknown>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%[[VALUE_u128]]), const<i32>(64))), truncate<u64, reason=explicit, fits=unknown>(and<u128>(read<u128>(%[[VALUE_u128]]), widen<u128, reason=usual_arith>(const<u64>(18446744073709551615)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_bitint_casts:[0-9]+]] @check_bitint_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_b9:[0-9]+]] b9: i9b [storage=automatic] = float_to_int<i9b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(100)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i9b>(%[[VALUE_b9]])), neg<f80>(const<f80>(100)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_ub9:[0-9]+]] ub9: u9b [storage=automatic] = float_to_int<u9b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(200));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u9b>(%[[VALUE_ub9]])), const<f80>(200))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_b40:[0-9]+]] b40: i40b [storage=automatic] = float_to_int<i40b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(123456789)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i40b>(%[[VALUE_b40]])), neg<f80>(const<f80>(123456789)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_ub40:[0-9]+]] ub40: u40b [storage=automatic] = float_to_int<u40b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(987654321));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u40b>(%[[VALUE_ub40]])), const<f80>(987654321))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_b101:[0-9]+]] b101: i101b [storage=automatic] = float_to_int<i101b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(123456789012345)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i101b>(%[[VALUE_b101]])), neg<f80>(const<f80>(123456789012345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i101b>(float_to_int<i101b, reason=explicit, out_of_range=ub, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i101b>(%[[VALUE_b101]]))), read<i101b>(%[[VALUE_b101]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_ub150:[0-9]+]] ub150: u150b [storage=automatic] = float_to_int<u150b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(987654321098765));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u150b>(%[[VALUE_ub150]])), const<f80>(987654321098765))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u150b>(float_to_int<u150b, reason=explicit, out_of_range=ub, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u150b>(%[[VALUE_ub150]]))), read<u150b>(%[[VALUE_ub150]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_b256:[0-9]+]] b256: i256b [storage=automatic] = float_to_int<i256b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(9999999999));
// DEFAULT-NEXT:         if ne<i256b>(read<i256b>(%[[VALUE_b256]]), widen<i256b, reason=usual_arith>(const<i64>(9999999999)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i256b>(%[[VALUE_b256]])), const<f80>(9999999999))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_ub300:[0-9]+]] ub300: u300b [storage=automatic] = float_to_int<u300b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(4.2E+9));
// DEFAULT-NEXT:         if ne<u300b>(read<u300b>(%[[VALUE_ub300]]), widen<u300b, reason=usual_arith>(const<u32>(4200000000)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u300b>(%[[VALUE_ub300]])), const<f80>(4.2E+9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE_b129:[0-9]+]] b129: i129b [storage=automatic] = float_to_int<i129b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(123));
// DEFAULT-NEXT:         if ne<i32>(truncate<i32, reason=explicit, fits=unknown>(read<i129b>(%[[VALUE_b129]])), const<i32>(123))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(widen<i129b, reason=explicit>(const<i32>(123))), const<f80>(123))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(77)>(%[[VALUE_str_4]])), truncate<i64, reason=explicit, fits=unknown>(read<i101b>(%[[VALUE_b101]])), truncate<u64, reason=explicit, fits=unknown>(read<u150b>(%[[VALUE_ub150]])), truncate<i64, reason=explicit, fits=unknown>(read<i256b>(%[[VALUE_b256]])), truncate<u64, reason=explicit, fits=unknown>(read<u300b>(%[[VALUE_ub300]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_vall:[0-9]+]] @__builtin_huge_vall() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_check_math_functions:[0-9]+]] @check_math_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_5]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_sqrtl]], const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_6]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_cbrtl]], const<f80>(27)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_7]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_sinl]], const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_8]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_cosl]], const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_9]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_tanl]], const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_10]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_asinl]], const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_11]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_acosl]], const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_12]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_atanl]], const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_13]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_atan2l]], const<f80>(1), const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_14]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_sinhl]], const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_15]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_coshl]], const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_16]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_tanhl]], const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_17]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_18]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_exp2l]], const<f80>(10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_19]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_logl]], call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], const<f80>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_20]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_log2l]], const<f80>(8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_21]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_log10l]], const<f80>(1000)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_22]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_powl]], const<f80>(2), const<f80>(10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_23]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_floorl]], const<f80>(2.70000000000000000004)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_24]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_ceill]], const<f80>(2.09999999999999999991)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_25]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_roundl]], const<f80>(2.5)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_26]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_truncl]], neg<f80>(const<f80>(2.70000000000000000004))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_27]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], neg<f80>(const<f80>(3.5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_28]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fmodl]], const<f80>(10), const<f80>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_29]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_hypotl]], const<f80>(3), const<f80>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_30]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_copysignl]], const<f80>(3), neg<f80>(const<f80>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_31]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fmaxl]], const<f80>(1), const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_32]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fminl]], const<f80>(1), const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_33]])), call<f80, signature=fn(f80, f80, f80) -> f80>(%[[VALUE_fmal]], const<f80>(2), const<f80>(3), const<f80>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_34]])), call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE_ldexpl]], const<f80>(1), const<i32>(4)));
// DEFAULT-NEXT:         let %[[VALUE_exp_2:[0-9]+]] exp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_35]])), call<f80, signature=fn(f80, ptr<i32>) -> f80>(%[[VALUE_frexpl]], const<f80>(100), addr_of<ptr<i32>>(%[[VALUE_exp_2]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_36]])), read<i32>(%[[VALUE_exp_2]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(61)>(%[[VALUE_str_37]])), from_bool<i32, reason=vararg>(float_class<bool, test=nan>(call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_38]]))))), conditional<i32>(float_class<bool, test=infinite>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_huge_vall]])), conditional<i32>(float_class<bool, test=sign_bit>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_huge_vall]])), const<i32>(-1), const<i32>(1)), const<i32>(0)), from_bool<i32, reason=vararg>(float_class<bool, test=sign_bit>(neg<f80>(const<f80>(1)))), from_bool<i32, reason=vararg>(float_class<bool, test=sign_bit>(const<f80>(1))), from_bool<i32, reason=vararg>(float_class<bool, test=finite>(const<f80>(1.18973149535723176502E+4932))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_39]])), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_check_remaining_math_functions:[0-9]+]] @check_remaining_math_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ten:[0-9]+]] ten: volatile f80 [storage=automatic] = const<f80>(10);
// DEFAULT-NEXT:         let %[[VALUE_three:[0-9]+]] three: volatile f80 [storage=automatic] = const<f80>(3);
// DEFAULT-NEXT:         let %[[VALUE_ipart:[0-9]+]] ipart: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %[[VALUE_frac:[0-9]+]] frac: f80 [storage=automatic] = call<f80, signature=fn(f80, ptr<f80>) -> f80>(%[[VALUE_modfl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]])), addr_of<ptr<f80>>(%[[VALUE_ipart]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_40]])), read<f80>(%[[VALUE_ipart]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_41]])), read<f80>(%[[VALUE_frac]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_42]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_remainderl]], read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]])));
// DEFAULT-NEXT:         let %[[VALUE_quo:[0-9]+]] quo: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_43]])), call<f80, signature=fn(f80, f80, ptr<i32>) -> f80>(%[[VALUE_remquol]], read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]), addr_of<ptr<i32>>(%[[VALUE_quo]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_44]])), read<i32>(%[[VALUE_quo]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_45]])), call<f80, signature=fn(f80, i32) -> f80>(%[[VALUE_scalbnl]], read<f80, volatile>(%[[VALUE_ten]]), const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str_46]])), call<f80, signature=fn(f80, i64) -> f80>(%[[VALUE_scalblnl]], read<f80, volatile>(%[[VALUE_ten]]), const<i64>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_47]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_nextafterl]], read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_48]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_nexttowardl]], read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_49]])), call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_fdiml]], read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_50]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_rintl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_51]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_nearbyintl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(47)>(%[[VALUE_str_52]])), call<i64, signature=fn(f80) -> i64>(%[[VALUE_lrintl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]))), call<i64, signature=fn(f80) -> i64>(%[[VALUE_llrintl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]))), call<i64, signature=fn(f80) -> i64>(%[[VALUE_lroundl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]))), call<i64, signature=fn(f80) -> i64>(%[[VALUE_llroundl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_53]])), call<i32, signature=fn(f80) -> i32>(%[[VALUE_ilogbl]], read<f80, volatile>(%[[VALUE_ten]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_54]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_logbl]], read<f80, volatile>(%[[VALUE_ten]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_55]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_erfl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%[[VALUE_str_56]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_erfcl]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%[[VALUE_ten]]), read<f80, volatile>(%[[VALUE_three]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_57]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_tgammal]], read<f80, volatile>(%[[VALUE_three]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_58]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE_lgammal]], read<f80, volatile>(%[[VALUE_ten]])));
// DEFAULT-NEXT:         let %[[VALUE_vnan:[0-9]+]] vnan: volatile f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_59]])));
// DEFAULT-NEXT:         let %[[VALUE_vinf:[0-9]+]] vinf: volatile f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%[[VALUE___builtin_huge_vall]]);
// DEFAULT-NEXT:         let %[[VALUE_vzero:[0-9]+]] vzero: volatile f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %[[VALUE_vone:[0-9]+]] vone: volatile f80 [storage=automatic] = const<f80>(1);
// DEFAULT-NEXT:         let %[[VALUE_vsub:[0-9]+]] vsub: volatile f80 [storage=automatic] = const<f80>(3.64519953188247460253E-4951);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___u:[0-9]+]] __u: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vnan]]);
// DEFAULT-NEXT:             let %[[VALUE___v:[0-9]+]] __v: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vone]]);
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u]]), read<f80, volatile>(%[[VALUE___v]])), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u]]), read<f80, volatile>(%[[VALUE___u]])), ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___v]]), read<f80, volatile>(%[[VALUE___v]])))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___u_2:[0-9]+]] __u: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vone]]);
// DEFAULT-NEXT:             let %[[VALUE___v_2:[0-9]+]] __v: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vzero]]);
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u_2]]), read<f80, volatile>(%[[VALUE___v_2]])), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u_2]]), read<f80, volatile>(%[[VALUE___u_2]])), ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___v_2]]), read<f80, volatile>(%[[VALUE___v_2]])))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(86)>(%[[VALUE_str_60]])), from_bool<i32, reason=vararg>(float_class<bool, test=nan>(read<f80, volatile>(%[[VALUE_vnan]]))), conditional<i32>(float_class<bool, test=infinite>(read<f80, volatile>(%[[VALUE_vinf]])), conditional<i32>(float_class<bool, test=sign_bit>(read<f80, volatile>(%[[VALUE_vinf]])), const<i32>(-1), const<i32>(1)), const<i32>(0)), from_bool<i32, reason=vararg>(float_class<bool, test=finite>(read<f80, volatile>(%[[VALUE_vone]]))), from_bool<i32, reason=vararg>(float_class<bool, test=normal>(read<f80, volatile>(%[[VALUE_vone]]))), from_bool<i32, reason=vararg>(read<bool>(%[[VALUE4]])), from_bool<i32, reason=vararg>(read<bool>(%[[VALUE5]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%[[VALUE_str_61]])), from_bool<i32, reason=vararg>(float_class<bool, test=normal>(read<f80, volatile>(%[[VALUE_vsub]]))));
// DEFAULT-NEXT:         let %[[VALUE_vtwo:[0-9]+]] vtwo: volatile f80 [storage=automatic] = const<f80>(2);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___x_50:[0-9]+]] __x: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vone]]);
// DEFAULT-NEXT:             let %[[VALUE___y_14:[0-9]+]] __y: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vtwo]]);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___u_3:[0-9]+]] __u: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE___x_50]]);
// DEFAULT-NEXT:                 let %[[VALUE___v_3:[0-9]+]] __v: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE___y_14]]);
// DEFAULT-NEXT:                 write<bool>(%[[VALUE7]], logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u_3]]), read<f80, volatile>(%[[VALUE___v_3]])), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u_3]]), read<f80, volatile>(%[[VALUE___u_3]])), ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___v_3]]), read<f80, volatile>(%[[VALUE___v_3]])))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], logical_and<bool>(not<bool>(read<bool>(%[[VALUE7]])), ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___x_50]]), read<f80, volatile>(%[[VALUE___y_14]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___x_51:[0-9]+]] __x: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vone]]);
// DEFAULT-NEXT:             let %[[VALUE___y_15:[0-9]+]] __y: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vone]]);
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___u_4:[0-9]+]] __u: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE___x_51]]);
// DEFAULT-NEXT:                 let %[[VALUE___v_4:[0-9]+]] __v: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE___y_15]]);
// DEFAULT-NEXT:                 write<bool>(%[[VALUE9]], logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u_4]]), read<f80, volatile>(%[[VALUE___v_4]])), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u_4]]), read<f80, volatile>(%[[VALUE___u_4]])), ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___v_4]]), read<f80, volatile>(%[[VALUE___v_4]])))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], logical_and<bool>(not<bool>(read<bool>(%[[VALUE9]])), ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___x_51]]), read<f80, volatile>(%[[VALUE___y_15]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___x_52:[0-9]+]] __x: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vnan]]);
// DEFAULT-NEXT:             let %[[VALUE___y_16:[0-9]+]] __y: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_vone]]);
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE___u_5:[0-9]+]] __u: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE___x_52]]);
// DEFAULT-NEXT:                 let %[[VALUE___v_5:[0-9]+]] __v: volatile f80 [storage=automatic] = read<f80, volatile>(%[[VALUE___y_16]]);
// DEFAULT-NEXT:                 write<bool>(%[[VALUE11]], logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u_5]]), read<f80, volatile>(%[[VALUE___v_5]])), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___u_5]]), read<f80, volatile>(%[[VALUE___u_5]])), ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___v_5]]), read<f80, volatile>(%[[VALUE___v_5]])))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], logical_and<bool>(not<bool>(read<bool>(%[[VALUE11]])), ne<f80, exceptions=ignore>(read<f80, volatile>(%[[VALUE___x_52]]), read<f80, volatile>(%[[VALUE___y_16]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%[[VALUE_str_62]])), from_bool<i32, reason=vararg>(read<bool>(%[[VALUE6]])), from_bool<i32, reason=vararg>(read<bool>(%[[VALUE8]])), from_bool<i32, reason=vararg>(read<bool>(%[[VALUE10]])));
// DEFAULT-NEXT:         let %[[VALUE_ten_plain:[0-9]+]] ten_plain: f80 [storage=automatic] = read<f80, volatile>(%[[VALUE_ten]]);
// DEFAULT-NEXT:         let %[[VALUE_canon:[0-9]+]] canon: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %[[VALUE_canon_r:[0-9]+]] canon_r: i32 [storage=automatic] = call<i32, signature=fn(ptr<f80>, ptr<const f80>) -> i32>(%[[VALUE_canonicalizel]], addr_of<ptr<f80>>(%[[VALUE_canon]]), pointer_cast<ptr<const f80>, reason=arg>(addr_of<ptr<f80>>(%[[VALUE_ten_plain]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_63]])), read<f80>(%[[VALUE_canon]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str_64]])), read<i32>(%[[VALUE_canon_r]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_65]])), const<f80>(3.36210314311209350626E-4932));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%[[VALUE_print_ld]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_66]])), const<f80>(3.64519953188247460253E-4951));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%[[VALUE_str_67]])), const<i32>(64), const<i32>(18), neg<i32, overflow=ub>(const<i32>(16381)), const<i32>(16384), neg<i32, overflow=ub>(const<i32>(4931)), const<i32>(4932));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: f80 [storage=automatic] = const<f80>(1.5);
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: f80 [storage=automatic] = const<f80>(4.5);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_68]])), call<i32, signature=fn(f80) -> i32>(%[[VALUE_truncate_long_double]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_x]]), read<f80>(%[[VALUE_y]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_69]])), call<i32, signature=fn(f80) -> i32>(%[[VALUE_truncate_long_double]], call<f80, signature=fn(f80, f80) -> f80>(%[[VALUE_mix_long_double]], const<f80>(3), const<f80>(5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_70]])), call<i32, signature=fn(f80) -> i32>(%[[VALUE_truncate_long_double]], div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(7)), const<f80>(2))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check_int_casts]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check_i128_casts]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check_bitint_casts]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check_math_functions]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check_remaining_math_functions]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
