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
// DEFAULT-NEXT:     type @type0 __int8_t = i8;
// DEFAULT-NEXT:     type @type1 __uint8_t = u8;
// DEFAULT-NEXT:     type @type2 __int16_t = i16;
// DEFAULT-NEXT:     type @type3 __uint16_t = u16;
// DEFAULT-NEXT:     type @type4 __int32_t = i32;
// DEFAULT-NEXT:     type @type5 __uint32_t = u32;
// DEFAULT-NEXT:     type @type6 __int64_t = i64;
// DEFAULT-NEXT:     type @type7 __uint64_t = u64;
// DEFAULT-NEXT:     type @type8 int8_t = i8;
// DEFAULT-NEXT:     type @type9 int16_t = i16;
// DEFAULT-NEXT:     type @type10 int32_t = i32;
// DEFAULT-NEXT:     type @type11 int64_t = i64;
// DEFAULT-NEXT:     type @type12 uint8_t = u8;
// DEFAULT-NEXT:     type @type13 uint16_t = u16;
// DEFAULT-NEXT:     type @type14 uint32_t = u32;
// DEFAULT-NEXT:     type @type15 uint64_t = u64;
// DEFAULT-NEXT:     global %287 .str287: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 115, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %288 .str288: array<i8, 59> [storage=static] = code_units<array<i8, 59>>([105, 56, 61, 37, 100, 32, 117, 56, 61, 37, 117, 32, 105, 49, 54, 61, 37, 100, 32, 117, 49, 54, 61, 37, 117, 32, 105, 51, 50, 61, 37, 100, 32, 117, 51, 50, 61, 37, 117, 32, 105, 54, 52, 61, 37, 108, 108, 100, 32, 117, 54, 52, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %289 .str289: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([105, 49, 50, 56, 61, 37, 108, 108, 100, 32, 117, 49, 50, 56, 95, 104, 105, 61, 37, 108, 108, 117, 32, 117, 49, 50, 56, 95, 108, 111, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %290 .str290: array<i8, 77> [storage=static] = code_units<array<i8, 77>>([98, 105, 116, 105, 110, 116, 95, 98, 49, 48, 49, 61, 37, 108, 108, 100, 32, 98, 105, 116, 105, 110, 116, 95, 117, 98, 49, 53, 48, 61, 37, 108, 108, 117, 32, 98, 105, 116, 105, 110, 116, 95, 98, 50, 53, 54, 95, 108, 111, 61, 37, 108, 108, 100, 32, 98, 105, 116, 105, 110, 116, 95, 117, 98, 51, 48, 48, 95, 108, 111, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %291 .str291: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([115, 113, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %292 .str292: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 98, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %293 .str293: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %294 .str294: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %295 .str295: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %296 .str296: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %297 .str297: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %298 .str298: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %299 .str299: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 116, 97, 110, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %300 .str300: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %301 .str301: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %302 .str302: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %303 .str303: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %304 .str304: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([101, 120, 112, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %305 .str305: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([108, 111, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %306 .str306: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 103, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %307 .str307: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([108, 111, 103, 49, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %308 .str308: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([112, 111, 119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %309 .str309: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 108, 111, 111, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %310 .str310: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 101, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %311 .str311: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 111, 117, 110, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %312 .str312: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([116, 114, 117, 110, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %313 .str313: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 97, 98, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %314 .str314: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 111, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %315 .str315: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 121, 112, 111, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %316 .str316: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([99, 111, 112, 121, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %317 .str317: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 97, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %318 .str318: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %319 .str319: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %320 .str320: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([108, 100, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %321 .str321: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 114, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %322 .str322: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 114, 101, 120, 112, 95, 101, 120, 112, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %323 .str323: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([105, 115, 110, 97, 110, 61, 37, 100, 32, 105, 115, 105, 110, 102, 61, 37, 100, 32, 115, 105, 103, 110, 98, 105, 116, 95, 110, 101, 103, 61, 37, 100, 32, 115, 105, 103, 110, 98, 105, 116, 95, 112, 111, 115, 61, 37, 100, 32, 105, 115, 102, 105, 110, 105, 116, 101, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %324 .str324: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %326 .str326: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([101, 112, 115, 105, 108, 111, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %327 .str327: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([109, 111, 100, 102, 95, 105, 112, 97, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %328 .str328: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([109, 111, 100, 102, 95, 102, 114, 97, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %329 .str329: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([114, 101, 109, 97, 105, 110, 100, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %330 .str330: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([114, 101, 109, 113, 117, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %331 .str331: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([114, 101, 109, 113, 117, 111, 95, 113, 117, 111, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %332 .str332: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([115, 99, 97, 108, 98, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %333 .str333: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([115, 99, 97, 108, 98, 108, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %334 .str334: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([110, 101, 120, 116, 97, 102, 116, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %335 .str335: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 120, 116, 116, 111, 119, 97, 114, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %336 .str336: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 100, 105, 109, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %337 .str337: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([114, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %338 .str338: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([110, 101, 97, 114, 98, 121, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %339 .str339: array<i8, 47> [storage=static] = code_units<array<i8, 47>>([108, 114, 105, 110, 116, 61, 37, 108, 100, 32, 108, 108, 114, 105, 110, 116, 61, 37, 108, 108, 100, 32, 108, 114, 111, 117, 110, 100, 61, 37, 108, 100, 32, 108, 108, 114, 111, 117, 110, 100, 61, 37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %340 .str340: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([105, 108, 111, 103, 98, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %341 .str341: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 103, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %342 .str342: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([101, 114, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %343 .str343: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([101, 114, 102, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %344 .str344: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([116, 103, 97, 109, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %345 .str345: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([108, 103, 97, 109, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %346 .str346: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %347 .str347: array<i8, 86> [storage=static] = code_units<array<i8, 86>>([105, 115, 110, 97, 110, 95, 118, 61, 37, 100, 32, 105, 115, 105, 110, 102, 95, 118, 61, 37, 100, 32, 105, 115, 102, 105, 110, 105, 116, 101, 95, 118, 61, 37, 100, 32, 105, 115, 110, 111, 114, 109, 97, 108, 95, 118, 61, 37, 100, 32, 105, 115, 117, 110, 111, 114, 100, 101, 114, 101, 100, 95, 118, 61, 37, 100, 32, 105, 115, 117, 110, 111, 114, 100, 101, 114, 101, 100, 95, 111, 107, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %348 .str348: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 117, 98, 110, 111, 114, 109, 97, 108, 95, 105, 115, 110, 111, 114, 109, 97, 108, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %349 .str349: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 108, 116, 61, 37, 100, 32, 105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 101, 113, 61, 37, 100, 32, 105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 110, 97, 110, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %350 .str350: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([99, 97, 110, 111, 110, 105, 99, 97, 108, 105, 122, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %351 .str351: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([99, 97, 110, 111, 110, 105, 99, 97, 108, 105, 122, 101, 95, 114, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %352 .str352: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([108, 100, 98, 108, 95, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %353 .str353: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([108, 100, 98, 108, 95, 116, 114, 117, 101, 95, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %354 .str354: array<i8, 100> [storage=static] = code_units<array<i8, 100>>([108, 100, 98, 108, 95, 109, 97, 110, 116, 95, 100, 105, 103, 61, 37, 100, 32, 108, 100, 98, 108, 95, 100, 105, 103, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 105, 110, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 97, 120, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 105, 110, 95, 49, 48, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 97, 120, 95, 49, 48, 95, 101, 120, 112, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %355 .str355: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %356 .str356: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %357 .str357: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @exp(%211 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @acosl(%212 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %5 @asinl(%213 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %7 @atanl(%214 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %10 @atan2l(%215 __y: f80, %216 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %12 @cosl(%217 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %14 @sinl(%218 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %16 @tanl(%219 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %18 @coshl(%220 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %20 @sinhl(%221 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %22 @tanhl(%222 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %24 @expl(%223 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %27 @frexpl(%224 __x: f80, %225 __exponent: ptr<i32>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %30 @ldexpl(%226 __x: f80, %227 __exponent: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %32 @logl(%228 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %34 @log10l(%229 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %37 @modfl(%230 __x: f80, %231 __iptr: ptr<f80>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %39 @logbl(%232 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %41 @exp2l(%233 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %43 @log2l(%234 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %46 @powl(%235 __x: f80, %236 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %48 @sqrtl(%237 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %51 @hypotl(%238 __x: f80, %239 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %53 @cbrtl(%240 __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %55 @ceill(%241 __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %57 @fabsl(%242 __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %59 @floorl(%243 __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %62 @fmodl(%244 __x: f80, %245 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %65 @copysignl(%246 __x: f80, %247 __y: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %67 @nanl(%248 __tagb: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %68 @erfl(%249 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %69 @erfcl(%250 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %70 @lgammal(%251 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %71 @tgammal(%252 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %73 @rintl(%253 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %76 @nextafterl(%254 __x: f80, %255 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %79 @nexttowardl(%256 __x: f80, %257 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %82 @remainderl(%258 __x: f80, %259 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %85 @scalbnl(%260 __x: f80, %261 __n: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %87 @ilogbl(%262 __x: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %90 @scalblnl(%263 __x: f80, %264 __n: i64) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %92 @nearbyintl(%265 __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %94 @roundl(%266 __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %96 @truncl(%267 __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %100 @remquol(%268 __x: f80, %269 __y: f80, %270 __quo: ptr<i32>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %102 @lrintl(%271 __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %104 @llrintl(%272 __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %106 @lroundl(%273 __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %108 @llroundl(%274 __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %111 @fdiml(%275 __x: f80, %276 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %114 @fmaxl(%277 __x: f80, %278 __y: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %117 @fminl(%279 __x: f80, %280 __y: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %121 @fmal(%281 __x: f80, %282 __y: f80, %283 __z: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %124 @canonicalizel(%284 __cx: ptr<f80>, %285 __x: ptr<const f80>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %142 @printf(%286 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %143 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %144 @mix_long_double(%145 a: f80, %146 b: f80) -> f80 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %147 c: f80 [storage=automatic] = div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%145), read<f80>(%146)), const<f80>(2));
// DEFAULT-NEXT:         return mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%147), const<f80>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %148 @truncate_long_double(%149 value: f80) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f80>(%149));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %150 @print_ld(%151 name: ptr<const i8>, %152 v: f80) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%287)), read<ptr<const i8>>(%151), read<f80>(%152));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %153 @check_int_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %154 i8: i8 [storage=automatic] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(100)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(%154)), neg<f80>(const<f80>(100)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %155 u8: u8 [storage=automatic] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(200));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(%155)), const<f80>(200))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %156 i16: i16 [storage=automatic] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(12345)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(%156)), neg<f80>(const<f80>(12345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %157 u16: u16 [storage=automatic] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(54321));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(%157)), const<f80>(54321))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %158 i32: i32 [storage=automatic] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(1234567890)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%158)), neg<f80>(const<f80>(1234567890)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %159 u32: u32 [storage=automatic] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(3456789012));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u32>(%159)), const<f80>(3456789012))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %160 i64: i64 [storage=automatic] = neg<i64, overflow=ub>(const<i64>(123456789012345));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i64>(%160)), neg<f80>(const<f80>(123456789012345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %161 u64: u64 [storage=automatic] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(12345678901234567890));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u64>(%161)), const<f80>(12345678901234567890))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(59)>(%288)), widen<i32, reason=vararg>(read<i8>(%154)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%155))), widen<i32, reason=vararg>(read<i16>(%156)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u16>(%157))), read<i32>(%158), read<u32>(%159), read<i64>(%160), read<u64>(%161));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %162 @check_i128_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %163 i128: i128 [storage=automatic] = float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(9223372036854775807)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i128>(%163)), neg<f80>(const<f80>(9223372036854775807)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %164 u128: u128 [storage=automatic] = float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(18446744073709551615));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u128>(%164)), const<f80>(18446744073709551615))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(37)>(%289)), truncate<i64, reason=explicit, fits=unknown>(read<i128>(%163)), truncate<u64, reason=explicit, fits=unknown>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%164), const<i32>(64))), truncate<u64, reason=explicit, fits=unknown>(and<u128>(read<u128>(%164), widen<u128, reason=usual_arith>(const<u64>(18446744073709551615)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %165 @check_bitint_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %166 b9: i9b [storage=automatic] = float_to_int<i9b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(100)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i9b>(%166)), neg<f80>(const<f80>(100)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %167 ub9: u9b [storage=automatic] = float_to_int<u9b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(200));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u9b>(%167)), const<f80>(200))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %168 b40: i40b [storage=automatic] = float_to_int<i40b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(123456789)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i40b>(%168)), neg<f80>(const<f80>(123456789)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %169 ub40: u40b [storage=automatic] = float_to_int<u40b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(987654321));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u40b>(%169)), const<f80>(987654321))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %170 b101: i101b [storage=automatic] = float_to_int<i101b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(123456789012345)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i101b>(%170)), neg<f80>(const<f80>(123456789012345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         if ne<i101b>(float_to_int<i101b, reason=explicit, out_of_range=ub, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i101b>(%170))), read<i101b>(%170))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %171 ub150: u150b [storage=automatic] = float_to_int<u150b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(987654321098765));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u150b>(%171)), const<f80>(987654321098765))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         if ne<u150b>(float_to_int<u150b, reason=explicit, out_of_range=ub, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u150b>(%171))), read<u150b>(%171))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %172 b256: i256b [storage=automatic] = float_to_int<i256b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(9999999999));
// DEFAULT-NEXT:         if ne<i256b>(read<i256b>(%172), widen<i256b, reason=usual_arith>(const<i64>(9999999999)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i256b>(%172)), const<f80>(9999999999))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %173 ub300: u300b [storage=automatic] = float_to_int<u300b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(4.2E+9));
// DEFAULT-NEXT:         if ne<u300b>(read<u300b>(%173), widen<u300b, reason=usual_arith>(const<u32>(4200000000)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u300b>(%173)), const<f80>(4.2E+9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         let %174 b129: i129b [storage=automatic] = float_to_int<i129b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(123));
// DEFAULT-NEXT:         if ne<i32>(truncate<i32, reason=explicit, fits=unknown>(read<i129b>(%174)), const<i32>(123))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(widen<i129b, reason=explicit>(const<i32>(123))), const<f80>(123))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%143);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(77)>(%290)), truncate<i64, reason=explicit, fits=unknown>(read<i101b>(%170)), truncate<u64, reason=explicit, fits=unknown>(read<u150b>(%171)), truncate<i64, reason=explicit, fits=unknown>(read<i256b>(%172)), truncate<u64, reason=explicit, fits=unknown>(read<u300b>(%173)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %325 @__builtin_huge_vall() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %175 @check_math_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%291)), call<f80, signature=fn(f80) -> f80>(%48, const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%292)), call<f80, signature=fn(f80) -> f80>(%53, const<f80>(27)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%293)), call<f80, signature=fn(f80) -> f80>(%14, const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%294)), call<f80, signature=fn(f80) -> f80>(%12, const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%295)), call<f80, signature=fn(f80) -> f80>(%16, const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%296)), call<f80, signature=fn(f80) -> f80>(%5, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%297)), call<f80, signature=fn(f80) -> f80>(%3, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%298)), call<f80, signature=fn(f80) -> f80>(%7, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%299)), call<f80, signature=fn(f80, f80) -> f80>(%10, const<f80>(1), const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%300)), call<f80, signature=fn(f80) -> f80>(%20, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%301)), call<f80, signature=fn(f80) -> f80>(%18, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%302)), call<f80, signature=fn(f80) -> f80>(%22, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%303)), call<f80, signature=fn(f80) -> f80>(%24, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%304)), call<f80, signature=fn(f80) -> f80>(%41, const<f80>(10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%305)), call<f80, signature=fn(f80) -> f80>(%32, call<f80, signature=fn(f80) -> f80>(%24, const<f80>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%306)), call<f80, signature=fn(f80) -> f80>(%43, const<f80>(8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%307)), call<f80, signature=fn(f80) -> f80>(%34, const<f80>(1000)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%308)), call<f80, signature=fn(f80, f80) -> f80>(%46, const<f80>(2), const<f80>(10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%309)), call<f80, signature=fn(f80) -> f80>(%59, const<f80>(2.70000000000000000004)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%310)), call<f80, signature=fn(f80) -> f80>(%55, const<f80>(2.09999999999999999991)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%311)), call<f80, signature=fn(f80) -> f80>(%94, const<f80>(2.5)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%312)), call<f80, signature=fn(f80) -> f80>(%96, neg<f80>(const<f80>(2.70000000000000000004))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%313)), call<f80, signature=fn(f80) -> f80>(%57, neg<f80>(const<f80>(3.5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%314)), call<f80, signature=fn(f80, f80) -> f80>(%62, const<f80>(10), const<f80>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%315)), call<f80, signature=fn(f80, f80) -> f80>(%51, const<f80>(3), const<f80>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%316)), call<f80, signature=fn(f80, f80) -> f80>(%65, const<f80>(3), neg<f80>(const<f80>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%317)), call<f80, signature=fn(f80, f80) -> f80>(%114, const<f80>(1), const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%318)), call<f80, signature=fn(f80, f80) -> f80>(%117, const<f80>(1), const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%319)), call<f80, signature=fn(f80, f80, f80) -> f80>(%121, const<f80>(2), const<f80>(3), const<f80>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%320)), call<f80, signature=fn(f80, i32) -> f80>(%30, const<f80>(1), const<i32>(4)));
// DEFAULT-NEXT:         let %176 exp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%321)), call<f80, signature=fn(f80, ptr<i32>) -> f80>(%27, const<f80>(100), addr_of<ptr<i32>>(%176)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%322)), read<i32>(%176));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(61)>(%323)), from_bool<i32, reason=vararg>(float_class<bool, test=nan>(call<f80, signature=fn(ptr<const i8>) -> f80>(%67, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%324))))), conditional<i32>(float_class<bool, test=infinite>(call<f80, signature=fn() -> f80>(%325)), conditional<i32>(float_class<bool, test=sign_bit>(call<f80, signature=fn() -> f80>(%325)), const<i32>(-1), const<i32>(1)), const<i32>(0)), from_bool<i32, reason=vararg>(float_class<bool, test=sign_bit>(neg<f80>(const<f80>(1)))), from_bool<i32, reason=vararg>(float_class<bool, test=sign_bit>(const<f80>(1))), from_bool<i32, reason=vararg>(float_class<bool, test=finite>(const<f80>(1.18973149535723176502E+4932))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%326)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %177 @check_remaining_math_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %178 ten: volatile f80 [storage=automatic] = const<f80>(10);
// DEFAULT-NEXT:         let %179 three: volatile f80 [storage=automatic] = const<f80>(3);
// DEFAULT-NEXT:         let %180 ipart: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %181 frac: f80 [storage=automatic] = call<f80, signature=fn(f80, ptr<f80>) -> f80>(%37, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179)), addr_of<ptr<f80>>(%180));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%327)), read<f80>(%180));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%328)), read<f80>(%181));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%329)), call<f80, signature=fn(f80, f80) -> f80>(%82, read<f80, volatile>(%178), read<f80, volatile>(%179)));
// DEFAULT-NEXT:         let %182 quo: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%330)), call<f80, signature=fn(f80, f80, ptr<i32>) -> f80>(%100, read<f80, volatile>(%178), read<f80, volatile>(%179), addr_of<ptr<i32>>(%182)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%331)), read<i32>(%182));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%332)), call<f80, signature=fn(f80, i32) -> f80>(%85, read<f80, volatile>(%178), const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%333)), call<f80, signature=fn(f80, i64) -> f80>(%90, read<f80, volatile>(%178), const<i64>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%334)), call<f80, signature=fn(f80, f80) -> f80>(%76, read<f80, volatile>(%178), read<f80, volatile>(%179)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%335)), call<f80, signature=fn(f80, f80) -> f80>(%79, read<f80, volatile>(%178), read<f80, volatile>(%179)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%336)), call<f80, signature=fn(f80, f80) -> f80>(%111, read<f80, volatile>(%178), read<f80, volatile>(%179)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%337)), call<f80, signature=fn(f80) -> f80>(%73, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%338)), call<f80, signature=fn(f80) -> f80>(%92, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(47)>(%339)), call<i64, signature=fn(f80) -> i64>(%102, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179))), call<i64, signature=fn(f80) -> i64>(%104, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179))), call<i64, signature=fn(f80) -> i64>(%106, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179))), call<i64, signature=fn(f80) -> i64>(%108, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%340)), call<i32, signature=fn(f80) -> i32>(%87, read<f80, volatile>(%178)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%341)), call<f80, signature=fn(f80) -> f80>(%39, read<f80, volatile>(%178)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%342)), call<f80, signature=fn(f80) -> f80>(%68, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%343)), call<f80, signature=fn(f80) -> f80>(%69, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80, volatile>(%178), read<f80, volatile>(%179))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%344)), call<f80, signature=fn(f80) -> f80>(%71, read<f80, volatile>(%179)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%345)), call<f80, signature=fn(f80) -> f80>(%70, read<f80, volatile>(%178)));
// DEFAULT-NEXT:         let %183 vnan: volatile f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%67, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%346)));
// DEFAULT-NEXT:         let %184 vinf: volatile f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%325);
// DEFAULT-NEXT:         let %185 vzero: volatile f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %186 vone: volatile f80 [storage=automatic] = const<f80>(1);
// DEFAULT-NEXT:         let %187 vsub: volatile f80 [storage=automatic] = const<f80>(3.64519953188247460253E-4951);
// DEFAULT-NEXT:         let %358: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %188 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%183);
// DEFAULT-NEXT:             let %189 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%186);
// DEFAULT-NEXT:             write<bool>(%358, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%188), read<f80, volatile>(%189)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%188), read<f80, volatile>(%188)), ne<f80, exceptions=ignore>(read<f80, volatile>(%189), read<f80, volatile>(%189)))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %359: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %190 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%186);
// DEFAULT-NEXT:             let %191 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%185);
// DEFAULT-NEXT:             write<bool>(%359, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%190), read<f80, volatile>(%191)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%190), read<f80, volatile>(%190)), ne<f80, exceptions=ignore>(read<f80, volatile>(%191), read<f80, volatile>(%191)))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(86)>(%347)), from_bool<i32, reason=vararg>(float_class<bool, test=nan>(read<f80, volatile>(%183))), conditional<i32>(float_class<bool, test=infinite>(read<f80, volatile>(%184)), conditional<i32>(float_class<bool, test=sign_bit>(read<f80, volatile>(%184)), const<i32>(-1), const<i32>(1)), const<i32>(0)), from_bool<i32, reason=vararg>(float_class<bool, test=finite>(read<f80, volatile>(%186))), from_bool<i32, reason=vararg>(float_class<bool, test=normal>(read<f80, volatile>(%186))), from_bool<i32, reason=vararg>(read<bool>(%358)), from_bool<i32, reason=vararg>(read<bool>(%359)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%348)), from_bool<i32, reason=vararg>(float_class<bool, test=normal>(read<f80, volatile>(%187))));
// DEFAULT-NEXT:         let %192 vtwo: volatile f80 [storage=automatic] = const<f80>(2);
// DEFAULT-NEXT:         let %360: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %193 __x: volatile f80 [storage=automatic] = read<f80, volatile>(%186);
// DEFAULT-NEXT:             let %194 __y: volatile f80 [storage=automatic] = read<f80, volatile>(%192);
// DEFAULT-NEXT:             let %361: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %195 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%193);
// DEFAULT-NEXT:                 let %196 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%194);
// DEFAULT-NEXT:                 write<bool>(%361, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%195), read<f80, volatile>(%196)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%195), read<f80, volatile>(%195)), ne<f80, exceptions=ignore>(read<f80, volatile>(%196), read<f80, volatile>(%196)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%360, logical_and<bool>(not<bool>(read<bool>(%361)), ne<f80, exceptions=ignore>(read<f80, volatile>(%193), read<f80, volatile>(%194))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %362: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %197 __x: volatile f80 [storage=automatic] = read<f80, volatile>(%186);
// DEFAULT-NEXT:             let %198 __y: volatile f80 [storage=automatic] = read<f80, volatile>(%186);
// DEFAULT-NEXT:             let %363: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %199 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%197);
// DEFAULT-NEXT:                 let %200 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%198);
// DEFAULT-NEXT:                 write<bool>(%363, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%199), read<f80, volatile>(%200)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%199), read<f80, volatile>(%199)), ne<f80, exceptions=ignore>(read<f80, volatile>(%200), read<f80, volatile>(%200)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%362, logical_and<bool>(not<bool>(read<bool>(%363)), ne<f80, exceptions=ignore>(read<f80, volatile>(%197), read<f80, volatile>(%198))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %364: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %201 __x: volatile f80 [storage=automatic] = read<f80, volatile>(%183);
// DEFAULT-NEXT:             let %202 __y: volatile f80 [storage=automatic] = read<f80, volatile>(%186);
// DEFAULT-NEXT:             let %365: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %203 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%201);
// DEFAULT-NEXT:                 let %204 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%202);
// DEFAULT-NEXT:                 write<bool>(%365, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%203), read<f80, volatile>(%204)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%203), read<f80, volatile>(%203)), ne<f80, exceptions=ignore>(read<f80, volatile>(%204), read<f80, volatile>(%204)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%364, logical_and<bool>(not<bool>(read<bool>(%365)), ne<f80, exceptions=ignore>(read<f80, volatile>(%201), read<f80, volatile>(%202))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%349)), from_bool<i32, reason=vararg>(read<bool>(%360)), from_bool<i32, reason=vararg>(read<bool>(%362)), from_bool<i32, reason=vararg>(read<bool>(%364)));
// DEFAULT-NEXT:         let %205 ten_plain: f80 [storage=automatic] = read<f80, volatile>(%178);
// DEFAULT-NEXT:         let %206 canon: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %207 canon_r: i32 [storage=automatic] = call<i32, signature=fn(ptr<f80>, ptr<const f80>) -> i32>(%124, addr_of<ptr<f80>>(%206), pointer_cast<ptr<const f80>, reason=arg>(addr_of<ptr<f80>>(%205)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%350)), read<f80>(%206));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%351)), read<i32>(%207));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%352)), const<f80>(3.36210314311209350626E-4932));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%150, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%353)), const<f80>(3.64519953188247460253E-4951));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%354)), const<i32>(64), const<i32>(18), neg<i32, overflow=ub>(const<i32>(16381)), const<i32>(16384), neg<i32, overflow=ub>(const<i32>(4931)), const<i32>(4932));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %208 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %209 x: f80 [storage=automatic] = const<f80>(1.5);
// DEFAULT-NEXT:         let %210 y: f80 [storage=automatic] = const<f80>(4.5);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%355)), call<i32, signature=fn(f80) -> i32>(%148, add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%209), read<f80>(%210))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%356)), call<i32, signature=fn(f80) -> i32>(%148, call<f80, signature=fn(f80, f80) -> f80>(%144, const<f80>(3), const<f80>(5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%142, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%357)), call<i32, signature=fn(f80) -> i32>(%148, div<f80, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(7)), const<f80>(2))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%153);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%162);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%165);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%175);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%177);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
