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
// DEFAULT-NEXT:     global %215 .str215: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([37, 115, 61, 37, 76, 97, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %216 .str216: array<i8, 59> [storage=static] = code_units<array<i8, 59>>([105, 56, 61, 37, 100, 32, 117, 56, 61, 37, 117, 32, 105, 49, 54, 61, 37, 100, 32, 117, 49, 54, 61, 37, 117, 32, 105, 51, 50, 61, 37, 100, 32, 117, 51, 50, 61, 37, 117, 32, 105, 54, 52, 61, 37, 108, 108, 100, 32, 117, 54, 52, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %217 .str217: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([105, 49, 50, 56, 61, 37, 108, 108, 100, 32, 117, 49, 50, 56, 95, 104, 105, 61, 37, 108, 108, 117, 32, 117, 49, 50, 56, 95, 108, 111, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %218 .str218: array<i8, 77> [storage=static] = code_units<array<i8, 77>>([98, 105, 116, 105, 110, 116, 95, 98, 49, 48, 49, 61, 37, 108, 108, 100, 32, 98, 105, 116, 105, 110, 116, 95, 117, 98, 49, 53, 48, 61, 37, 108, 108, 117, 32, 98, 105, 116, 105, 110, 116, 95, 98, 50, 53, 54, 95, 108, 111, 61, 37, 108, 108, 100, 32, 98, 105, 116, 105, 110, 116, 95, 117, 98, 51, 48, 48, 95, 108, 111, 61, 37, 108, 108, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %219 .str219: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([115, 113, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %220 .str220: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 98, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %221 .str221: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %222 .str222: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %223 .str223: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %224 .str224: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 115, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %225 .str225: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 99, 111, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %226 .str226: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([97, 116, 97, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %227 .str227: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 116, 97, 110, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %228 .str228: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([115, 105, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %229 .str229: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 111, 115, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %230 .str230: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([116, 97, 110, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %231 .str231: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %232 .str232: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([101, 120, 112, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %233 .str233: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([108, 111, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %234 .str234: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 103, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %235 .str235: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([108, 111, 103, 49, 48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %236 .str236: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([112, 111, 119, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %237 .str237: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 108, 111, 111, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %238 .str238: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([99, 101, 105, 108, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %239 .str239: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([114, 111, 117, 110, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %240 .str240: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([116, 114, 117, 110, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %241 .str241: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 97, 98, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %242 .str242: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 111, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %243 .str243: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 121, 112, 111, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %244 .str244: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([99, 111, 112, 121, 115, 105, 103, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %245 .str245: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 97, 120, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %246 .str246: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %247 .str247: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %248 .str248: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([108, 100, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %249 .str249: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 114, 101, 120, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %250 .str250: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 114, 101, 120, 112, 95, 101, 120, 112, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %251 .str251: array<i8, 61> [storage=static] = code_units<array<i8, 61>>([105, 115, 110, 97, 110, 61, 37, 100, 32, 105, 115, 105, 110, 102, 61, 37, 100, 32, 115, 105, 103, 110, 98, 105, 116, 95, 110, 101, 103, 61, 37, 100, 32, 115, 105, 103, 110, 98, 105, 116, 95, 112, 111, 115, 61, 37, 100, 32, 105, 115, 102, 105, 110, 105, 116, 101, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %252 .str252: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %253 .str253: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([101, 112, 115, 105, 108, 111, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %254 .str254: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([109, 111, 100, 102, 95, 105, 112, 97, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %255 .str255: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([109, 111, 100, 102, 95, 102, 114, 97, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %256 .str256: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([114, 101, 109, 97, 105, 110, 100, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %257 .str257: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([114, 101, 109, 113, 117, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %258 .str258: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([114, 101, 109, 113, 117, 111, 95, 113, 117, 111, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %259 .str259: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([115, 99, 97, 108, 98, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %260 .str260: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([115, 99, 97, 108, 98, 108, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %261 .str261: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([110, 101, 120, 116, 97, 102, 116, 101, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %262 .str262: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([110, 101, 120, 116, 116, 111, 119, 97, 114, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %263 .str263: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([102, 100, 105, 109, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %264 .str264: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([114, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %265 .str265: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([110, 101, 97, 114, 98, 121, 105, 110, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %266 .str266: array<i8, 47> [storage=static] = code_units<array<i8, 47>>([108, 114, 105, 110, 116, 61, 37, 108, 100, 32, 108, 108, 114, 105, 110, 116, 61, 37, 108, 108, 100, 32, 108, 114, 111, 117, 110, 100, 61, 37, 108, 100, 32, 108, 108, 114, 111, 117, 110, 100, 61, 37, 108, 108, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %267 .str267: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([105, 108, 111, 103, 98, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %268 .str268: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([108, 111, 103, 98, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %269 .str269: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([101, 114, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %270 .str270: array<i8, 5> [storage=static] = code_units<array<i8, 5>>([101, 114, 102, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %271 .str271: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([116, 103, 97, 109, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %272 .str272: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([108, 103, 97, 109, 109, 97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %273 .str273: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %274 .str274: array<i8, 86> [storage=static] = code_units<array<i8, 86>>([105, 115, 110, 97, 110, 95, 118, 61, 37, 100, 32, 105, 115, 105, 110, 102, 95, 118, 61, 37, 100, 32, 105, 115, 102, 105, 110, 105, 116, 101, 95, 118, 61, 37, 100, 32, 105, 115, 110, 111, 114, 109, 97, 108, 95, 118, 61, 37, 100, 32, 105, 115, 117, 110, 111, 114, 100, 101, 114, 101, 100, 95, 118, 61, 37, 100, 32, 105, 115, 117, 110, 111, 114, 100, 101, 114, 101, 100, 95, 111, 107, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %275 .str275: array<i8, 23> [storage=static] = code_units<array<i8, 23>>([115, 117, 98, 110, 111, 114, 109, 97, 108, 95, 105, 115, 110, 111, 114, 109, 97, 108, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %276 .str276: array<i8, 62> [storage=static] = code_units<array<i8, 62>>([105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 108, 116, 61, 37, 100, 32, 105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 101, 113, 61, 37, 100, 32, 105, 115, 108, 101, 115, 115, 103, 114, 101, 97, 116, 101, 114, 95, 110, 97, 110, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %277 .str277: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([99, 97, 110, 111, 110, 105, 99, 97, 108, 105, 122, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %278 .str278: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([99, 97, 110, 111, 110, 105, 99, 97, 108, 105, 122, 101, 95, 114, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %279 .str279: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([108, 100, 98, 108, 95, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %280 .str280: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([108, 100, 98, 108, 95, 116, 114, 117, 101, 95, 109, 105, 110, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %281 .str281: array<i8, 100> [storage=static] = code_units<array<i8, 100>>([108, 100, 98, 108, 95, 109, 97, 110, 116, 95, 100, 105, 103, 61, 37, 100, 32, 108, 100, 98, 108, 95, 100, 105, 103, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 105, 110, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 97, 120, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 105, 110, 95, 49, 48, 95, 101, 120, 112, 61, 37, 100, 32, 108, 100, 98, 108, 95, 109, 97, 120, 95, 49, 48, 95, 101, 120, 112, 61, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %282 .str282: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %283 .str283: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %284 .str284: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @exp(%139 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %1 @acosl(%140 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %2 @asinl(%141 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %3 @atanl(%142 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %4 @atan2l(%143 __y: f80, %144 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %5 @cosl(%145 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %6 @sinl(%146 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %7 @tanl(%147 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %8 @coshl(%148 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %9 @sinhl(%149 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %10 @tanhl(%150 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %11 @expl(%151 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %12 @frexpl(%152 __x: f80, %153 __exponent: ptr<i32>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %13 @ldexpl(%154 __x: f80, %155 __exponent: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %14 @logl(%156 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %15 @log10l(%157 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %16 @modfl(%158 __x: f80, %159 __iptr: ptr<f80>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %17 @logbl(%160 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %18 @exp2l(%161 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %19 @log2l(%162 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %20 @powl(%163 __x: f80, %164 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %21 @sqrtl(%165 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %22 @hypotl(%166 __x: f80, %167 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %23 @cbrtl(%168 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %24 @ceill(%169 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %25 @fabsl(%170 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %26 @floorl(%171 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %27 @fmodl(%172 __x: f80, %173 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %28 @copysignl(%174 __x: f80, %175 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %29 @nanl(%176 __tagb: ptr<const i8>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %30 @erfl(%177 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %31 @erfcl(%178 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %32 @lgammal(%179 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %33 @tgammal(%180 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %34 @rintl(%181 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %35 @nextafterl(%182 __x: f80, %183 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %36 @nexttowardl(%184 __x: f80, %185 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %37 @remainderl(%186 __x: f80, %187 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %38 @scalbnl(%188 __x: f80, %189 __n: i32) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %39 @ilogbl(%190 __x: f80) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %40 @scalblnl(%191 __x: f80, %192 __n: i64) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %41 @nearbyintl(%193 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %42 @roundl(%194 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %43 @truncl(%195 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %44 @remquol(%196 __x: f80, %197 __y: f80, %198 __quo: ptr<i32>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %45 @lrintl(%199 __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %46 @llrintl(%200 __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %47 @lroundl(%201 __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %48 @llroundl(%202 __x: f80) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %49 @fdiml(%203 __x: f80, %204 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %50 @fmaxl(%205 __x: f80, %206 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %51 @fminl(%207 __x: f80, %208 __y: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %52 @fmal(%209 __x: f80, %210 __y: f80, %211 __z: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %53 @canonicalizel(%212 __cx: ptr<f80>, %213 __x: ptr<const f80>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %70 @printf(%214 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %71 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %72 @mix_long_double(%73 a: f80, %74 b: f80) -> f80 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %75 c: f80 [storage=automatic] = div<f80, rounding=nearest_even, exceptions=ignore>(add<f80, rounding=nearest_even, exceptions=ignore>(read<f80>(%73), read<f80>(%74)), const<f80>(2));
// DEFAULT-NEXT:         return mul<f80, rounding=nearest_even, exceptions=ignore>(read<f80>(%75), const<f80>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @truncate_long_double(%77 value: f80) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f80>(%77));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @print_ld(%79 name: ptr<const i8>, %80 v: f80) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%215)), read<ptr<const i8>>(%79), read<f80>(%80));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @check_int_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %82 i8: i8 [storage=automatic] = float_to_int<i8, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(100)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(%82)), neg<f80>(const<f80>(100)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %83 u8: u8 [storage=automatic] = float_to_int<u8, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(200));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(%83)), const<f80>(200))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %84 i16: i16 [storage=automatic] = float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(12345)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i16>(%84)), neg<f80>(const<f80>(12345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %85 u16: u16 [storage=automatic] = float_to_int<u16, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(54321));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u16>(%85)), const<f80>(54321))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %86 i32: i32 [storage=automatic] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(1234567890)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%86)), neg<f80>(const<f80>(1234567890)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %87 u32: u32 [storage=automatic] = float_to_int<u32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(3456789012));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u32>(%87)), const<f80>(3456789012))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %88 i64: i64 [storage=automatic] = neg<i64, overflow=ub>(const<i64>(123456789012345));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i64>(%88)), neg<f80>(const<f80>(123456789012345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %89 u64: u64 [storage=automatic] = float_to_int<u64, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(12345678901234567890));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u64>(%89)), const<f80>(12345678901234567890))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(59)>(%216)), widen<i32, reason=vararg>(read<i8>(%82)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%83))), widen<i32, reason=vararg>(read<i16>(%84)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u16>(%85))), read<i32>(%86), read<u32>(%87), read<i64>(%88), read<u64>(%89));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %90 @check_i128_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %91 i128: i128 [storage=automatic] = float_to_int<i128, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(9223372036854775807)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i128>(%91)), neg<f80>(const<f80>(9223372036854775807)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %92 u128: u128 [storage=automatic] = float_to_int<u128, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(18446744073709551615));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u128>(%92)), const<f80>(18446744073709551615))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(37)>(%217)), truncate<i64, reason=explicit, fits=unknown>(read<i128>(%91)), truncate<u64, reason=explicit, fits=unknown>(shr<u128, amount_out_of_range=ub, fill=zero_extend>(read<u128>(%92), const<i32>(64))), truncate<u64, reason=explicit, fits=unknown>(and<u128>(read<u128>(%92), widen<u128, reason=usual_arith>(const<u64>(18446744073709551615)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %93 @check_bitint_casts() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %94 b9: i9b [storage=automatic] = float_to_int<i9b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(100)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i9b>(%94)), neg<f80>(const<f80>(100)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %95 ub9: u9b [storage=automatic] = float_to_int<u9b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(200));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u9b>(%95)), const<f80>(200))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %96 b40: i40b [storage=automatic] = float_to_int<i40b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(123456789)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i40b>(%96)), neg<f80>(const<f80>(123456789)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %97 ub40: u40b [storage=automatic] = float_to_int<u40b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(987654321));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<u40b>(%97)), const<f80>(987654321))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %98 b101: i101b [storage=automatic] = float_to_int<i101b, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f80>(const<f80>(123456789012345)));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i101b>(%98)), neg<f80>(const<f80>(123456789012345)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         if ne<i101b>(float_to_int<i101b, reason=explicit, out_of_range=ub, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i101b>(%98))), read<i101b>(%98))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %99 ub150: u150b [storage=automatic] = float_to_int<u150b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(987654321098765));
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u150b>(%99)), const<f80>(987654321098765))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         if ne<u150b>(float_to_int<u150b, reason=explicit, out_of_range=ub, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u150b>(%99))), read<u150b>(%99))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %100 b256: i256b [storage=automatic] = float_to_int<i256b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(9999999999));
// DEFAULT-NEXT:         if ne<i256b>(read<i256b>(%100), widen<i256b, reason=usual_arith>(const<i64>(9999999999)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<i256b>(%100)), const<f80>(9999999999))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %101 ub300: u300b [storage=automatic] = float_to_int<u300b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(4.2E+9));
// DEFAULT-NEXT:         if ne<u300b>(read<u300b>(%101), widen<u300b, reason=usual_arith>(const<u32>(4200000000)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u300b>(%101)), const<f80>(4.2E+9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         let %102 b129: i129b [storage=automatic] = float_to_int<i129b, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f80>(123));
// DEFAULT-NEXT:         if ne<i32>(truncate<i32, reason=explicit, fits=unknown>(read<i129b>(%102)), const<i32>(123))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(widen<i129b, reason=explicit>(const<i32>(123))), const<f80>(123))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%71);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(77)>(%218)), truncate<i64, reason=explicit, fits=unknown>(read<i101b>(%98)), truncate<u64, reason=explicit, fits=unknown>(read<u150b>(%99)), truncate<i64, reason=explicit, fits=unknown>(read<i256b>(%100)), truncate<u64, reason=explicit, fits=unknown>(read<u300b>(%101)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %103 @check_math_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%219)), call<f80, signature=fn(f80) -> f80>(%21, const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%220)), call<f80, signature=fn(f80) -> f80>(%23, const<f80>(27)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%221)), call<f80, signature=fn(f80) -> f80>(%6, const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%222)), call<f80, signature=fn(f80) -> f80>(%5, const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%223)), call<f80, signature=fn(f80) -> f80>(%7, const<f80>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%224)), call<f80, signature=fn(f80) -> f80>(%2, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%225)), call<f80, signature=fn(f80) -> f80>(%1, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%226)), call<f80, signature=fn(f80) -> f80>(%3, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%227)), call<f80, signature=fn(f80, f80) -> f80>(%4, const<f80>(1), const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%228)), call<f80, signature=fn(f80) -> f80>(%9, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%229)), call<f80, signature=fn(f80) -> f80>(%8, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%230)), call<f80, signature=fn(f80) -> f80>(%10, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%231)), call<f80, signature=fn(f80) -> f80>(%11, const<f80>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%232)), call<f80, signature=fn(f80) -> f80>(%18, const<f80>(10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%233)), call<f80, signature=fn(f80) -> f80>(%14, call<f80, signature=fn(f80) -> f80>(%11, const<f80>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%234)), call<f80, signature=fn(f80) -> f80>(%19, const<f80>(8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%235)), call<f80, signature=fn(f80) -> f80>(%15, const<f80>(1000)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%236)), call<f80, signature=fn(f80, f80) -> f80>(%20, const<f80>(2), const<f80>(10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%237)), call<f80, signature=fn(f80) -> f80>(%26, const<f80>(2.70000000000000000004)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%238)), call<f80, signature=fn(f80) -> f80>(%24, const<f80>(2.09999999999999999991)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%239)), call<f80, signature=fn(f80) -> f80>(%42, const<f80>(2.5)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%240)), call<f80, signature=fn(f80) -> f80>(%43, neg<f80>(const<f80>(2.70000000000000000004))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%241)), call<f80, signature=fn(f80) -> f80>(%25, neg<f80>(const<f80>(3.5))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%242)), call<f80, signature=fn(f80, f80) -> f80>(%27, const<f80>(10), const<f80>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%243)), call<f80, signature=fn(f80, f80) -> f80>(%22, const<f80>(3), const<f80>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%244)), call<f80, signature=fn(f80, f80) -> f80>(%28, const<f80>(3), neg<f80>(const<f80>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%245)), call<f80, signature=fn(f80, f80) -> f80>(%50, const<f80>(1), const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%246)), call<f80, signature=fn(f80, f80) -> f80>(%51, const<f80>(1), const<f80>(2)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%247)), call<f80, signature=fn(f80, f80, f80) -> f80>(%52, const<f80>(2), const<f80>(3), const<f80>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%248)), call<f80, signature=fn(f80, i32) -> f80>(%13, const<f80>(1), const<i32>(4)));
// DEFAULT-NEXT:         let %104 exp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%249)), call<f80, signature=fn(f80, ptr<i32>) -> f80>(%12, const<f80>(100), addr_of<ptr<i32>>(%104)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%250)), read<i32>(%104));
// DEFAULT-NEXT:         let %285: i32 [synthetic];
// DEFAULT-NEXT:         if float_class<bool, test=infinite>(call<f80, signature=fn() -> f80>(__builtin_huge_vall))
// DEFAULT-NEXT:             write<i32>(%285, conditional<i32>(float_class<bool, test=sign_bit>(call<f80, signature=fn() -> f80>(__builtin_huge_vall)), const<i32>(-1), const<i32>(1)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%285, const<i32>(0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(61)>(%251)), from_bool<i32, reason=vararg>(float_class<bool, test=nan>(call<f80, signature=fn(ptr<const i8>) -> f80>(%29, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%252))))), read<i32>(%285), from_bool<i32, reason=vararg>(float_class<bool, test=sign_bit>(neg<f80>(const<f80>(1)))), from_bool<i32, reason=vararg>(float_class<bool, test=sign_bit>(const<f80>(1))), from_bool<i32, reason=vararg>(float_class<bool, test=finite>(const<f80>(1.18973149535723176502E+4932))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%253)), const<f80>(1.08420217248550443401E-19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %105 @check_remaining_math_functions() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %106 ten: volatile f80 [storage=automatic] = const<f80>(10);
// DEFAULT-NEXT:         let %107 three: volatile f80 [storage=automatic] = const<f80>(3);
// DEFAULT-NEXT:         let %108 ipart: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %109 frac: f80 [storage=automatic] = call<f80, signature=fn(f80, ptr<f80>) -> f80>(%16, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107)), addr_of<ptr<f80>>(%108));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%254)), read<f80>(%108));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%255)), read<f80>(%109));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%256)), call<f80, signature=fn(f80, f80) -> f80>(%37, read<f80, volatile>(%106), read<f80, volatile>(%107)));
// DEFAULT-NEXT:         let %110 quo: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%257)), call<f80, signature=fn(f80, f80, ptr<i32>) -> f80>(%44, read<f80, volatile>(%106), read<f80, volatile>(%107), addr_of<ptr<i32>>(%110)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%258)), read<i32>(%110));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%259)), call<f80, signature=fn(f80, i32) -> f80>(%38, read<f80, volatile>(%106), const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%260)), call<f80, signature=fn(f80, i64) -> f80>(%40, read<f80, volatile>(%106), const<i64>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%261)), call<f80, signature=fn(f80, f80) -> f80>(%35, read<f80, volatile>(%106), read<f80, volatile>(%107)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%262)), call<f80, signature=fn(f80, f80) -> f80>(%36, read<f80, volatile>(%106), read<f80, volatile>(%107)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%263)), call<f80, signature=fn(f80, f80) -> f80>(%49, read<f80, volatile>(%106), read<f80, volatile>(%107)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%264)), call<f80, signature=fn(f80) -> f80>(%34, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%265)), call<f80, signature=fn(f80) -> f80>(%41, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(47)>(%266)), call<i64, signature=fn(f80) -> i64>(%45, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107))), call<i64, signature=fn(f80) -> i64>(%46, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107))), call<i64, signature=fn(f80) -> i64>(%47, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107))), call<i64, signature=fn(f80) -> i64>(%48, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%267)), call<i32, signature=fn(f80) -> i32>(%39, read<f80, volatile>(%106)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%268)), call<f80, signature=fn(f80) -> f80>(%17, read<f80, volatile>(%106)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%269)), call<f80, signature=fn(f80) -> f80>(%30, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(5)>(%270)), call<f80, signature=fn(f80) -> f80>(%31, div<f80, rounding=nearest_even, exceptions=ignore>(read<f80, volatile>(%106), read<f80, volatile>(%107))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%271)), call<f80, signature=fn(f80) -> f80>(%33, read<f80, volatile>(%107)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%272)), call<f80, signature=fn(f80) -> f80>(%32, read<f80, volatile>(%106)));
// DEFAULT-NEXT:         let %111 vnan: volatile f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%29, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%273)));
// DEFAULT-NEXT:         let %112 vinf: volatile f80 [storage=automatic] = call<f80, signature=fn() -> f80>(__builtin_huge_vall);
// DEFAULT-NEXT:         let %113 vzero: volatile f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %114 vone: volatile f80 [storage=automatic] = const<f80>(1);
// DEFAULT-NEXT:         let %115 vsub: volatile f80 [storage=automatic] = const<f80>(3.64519953188247460253E-4951);
// DEFAULT-NEXT:         let %286: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %116 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%111);
// DEFAULT-NEXT:             let %117 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%114);
// DEFAULT-NEXT:             write<bool>(%286, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%116), read<f80, volatile>(%117)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%116), read<f80, volatile>(%116)), ne<f80, exceptions=ignore>(read<f80, volatile>(%117), read<f80, volatile>(%117)))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %287: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %118 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%114);
// DEFAULT-NEXT:             let %119 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%113);
// DEFAULT-NEXT:             write<bool>(%287, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%118), read<f80, volatile>(%119)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%118), read<f80, volatile>(%118)), ne<f80, exceptions=ignore>(read<f80, volatile>(%119), read<f80, volatile>(%119)))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(86)>(%274)), from_bool<i32, reason=vararg>(float_class<bool, test=nan>(read<f80, volatile>(%111))), conditional<i32>(float_class<bool, test=infinite>(read<f80, volatile>(%112)), conditional<i32>(float_class<bool, test=sign_bit>(read<f80, volatile>(%112)), const<i32>(-1), const<i32>(1)), const<i32>(0)), from_bool<i32, reason=vararg>(float_class<bool, test=finite>(read<f80, volatile>(%114))), from_bool<i32, reason=vararg>(float_class<bool, test=normal>(read<f80, volatile>(%114))), from_bool<i32, reason=vararg>(read<bool>(%286)), from_bool<i32, reason=vararg>(read<bool>(%287)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(23)>(%275)), from_bool<i32, reason=vararg>(float_class<bool, test=normal>(read<f80, volatile>(%115))));
// DEFAULT-NEXT:         let %120 vtwo: volatile f80 [storage=automatic] = const<f80>(2);
// DEFAULT-NEXT:         let %288: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %121 __x: volatile f80 [storage=automatic] = read<f80, volatile>(%114);
// DEFAULT-NEXT:             let %122 __y: volatile f80 [storage=automatic] = read<f80, volatile>(%120);
// DEFAULT-NEXT:             let %289: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %123 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%121);
// DEFAULT-NEXT:                 let %124 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%122);
// DEFAULT-NEXT:                 write<bool>(%289, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%123), read<f80, volatile>(%124)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%123), read<f80, volatile>(%123)), ne<f80, exceptions=ignore>(read<f80, volatile>(%124), read<f80, volatile>(%124)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%288, logical_and<bool>(not<bool>(read<bool>(%289)), ne<f80, exceptions=ignore>(read<f80, volatile>(%121), read<f80, volatile>(%122))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %290: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %125 __x: volatile f80 [storage=automatic] = read<f80, volatile>(%114);
// DEFAULT-NEXT:             let %126 __y: volatile f80 [storage=automatic] = read<f80, volatile>(%114);
// DEFAULT-NEXT:             let %291: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %127 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%125);
// DEFAULT-NEXT:                 let %128 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%126);
// DEFAULT-NEXT:                 write<bool>(%291, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%127), read<f80, volatile>(%128)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%127), read<f80, volatile>(%127)), ne<f80, exceptions=ignore>(read<f80, volatile>(%128), read<f80, volatile>(%128)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%290, logical_and<bool>(not<bool>(read<bool>(%291)), ne<f80, exceptions=ignore>(read<f80, volatile>(%125), read<f80, volatile>(%126))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %292: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %129 __x: volatile f80 [storage=automatic] = read<f80, volatile>(%111);
// DEFAULT-NEXT:             let %130 __y: volatile f80 [storage=automatic] = read<f80, volatile>(%114);
// DEFAULT-NEXT:             let %293: bool [synthetic];
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %131 __u: volatile f80 [storage=automatic] = read<f80, volatile>(%129);
// DEFAULT-NEXT:                 let %132 __v: volatile f80 [storage=automatic] = read<f80, volatile>(%130);
// DEFAULT-NEXT:                 write<bool>(%293, logical_and<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%131), read<f80, volatile>(%132)), logical_or<bool>(ne<f80, exceptions=ignore>(read<f80, volatile>(%131), read<f80, volatile>(%131)), ne<f80, exceptions=ignore>(read<f80, volatile>(%132), read<f80, volatile>(%132)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             write<bool>(%292, logical_and<bool>(not<bool>(read<bool>(%293)), ne<f80, exceptions=ignore>(read<f80, volatile>(%129), read<f80, volatile>(%130))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(62)>(%276)), from_bool<i32, reason=vararg>(read<bool>(%288)), from_bool<i32, reason=vararg>(read<bool>(%290)), from_bool<i32, reason=vararg>(read<bool>(%292)));
// DEFAULT-NEXT:         let %133 ten_plain: f80 [storage=automatic] = read<f80, volatile>(%106);
// DEFAULT-NEXT:         let %134 canon: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %135 canon_r: i32 [storage=automatic] = call<i32, signature=fn(ptr<f80>, ptr<const f80>) -> i32>(%53, addr_of<ptr<f80>>(%134), pointer_cast<ptr<const f80>, reason=arg>(addr_of<ptr<f80>>(%133)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%277)), read<f80>(%134));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%278)), read<i32>(%135));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%279)), const<f80>(3.36210314311209350626E-4932));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, f80) -> void>(%78, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%280)), const<f80>(3.64519953188247460253E-4951));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(100)>(%281)), const<i32>(64), const<i32>(18), neg<i32, overflow=ub>(const<i32>(16381)), const<i32>(16384), neg<i32, overflow=ub>(const<i32>(4931)), const<i32>(4932));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %136 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %137 x: f80 [storage=automatic] = const<f80>(1.5);
// DEFAULT-NEXT:         let %138 y: f80 [storage=automatic] = const<f80>(4.5);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%282)), call<i32, signature=fn(f80) -> i32>(%76, add<f80, rounding=nearest_even, exceptions=ignore>(read<f80>(%137), read<f80>(%138))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%283)), call<i32, signature=fn(f80) -> i32>(%76, call<f80, signature=fn(f80, f80) -> f80>(%72, const<f80>(3), const<f80>(5))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%70, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%284)), call<i32, signature=fn(f80) -> i32>(%76, div<f80, rounding=nearest_even, exceptions=ignore>(int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(7)), const<f80>(2))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%81);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%90);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%93);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%103);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%105);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
