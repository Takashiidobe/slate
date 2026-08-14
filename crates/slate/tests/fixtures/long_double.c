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

  return 0;
}
