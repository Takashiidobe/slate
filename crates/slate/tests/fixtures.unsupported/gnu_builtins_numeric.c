#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

static int gnu_builtin_bits(void) {
  int total = 0;
  total += __builtin_ffs(16);
  total += __builtin_ffsl(32L);
  total += __builtin_ffsll(64LL);
  total += __builtin_clz(1U) == (int)(sizeof(unsigned int) * CHAR_BIT - 1);
  total += __builtin_clzl(1UL) == (int)(sizeof(unsigned long) * CHAR_BIT - 1);
  total +=
      __builtin_clzll(1ULL) == (int)(sizeof(unsigned long long) * CHAR_BIT - 1);
  total += __builtin_ctz(16U);
  total += __builtin_ctzl(32UL);
  total += __builtin_ctzll(64ULL);
  total += __builtin_clrsb(-1);
  total += __builtin_clrsbl(-2L);
  total += __builtin_clrsbll(-4LL);
  total += __builtin_popcount(0xf0U);
  total += __builtin_popcountl(0xffUL);
  total += __builtin_popcountll(0xff00ULL);
  total += __builtin_parity(7U);
  total += __builtin_parityl(15UL);
  total += __builtin_parityll(31ULL);
  return total;
}

static unsigned long long gnu_builtin_reordering(void) {
  unsigned long long total = 0;
  total += __builtin_bswap16(0x1234U);
  total += __builtin_bswap32(0x01020304U);
  total += __builtin_bswap64(0x0102030405060708ULL);
  total += __builtin_bitreverse8(0x12U);
  total += __builtin_bitreverse16(0x1234U);
  total += __builtin_bitreverse32(0x12345678U);
  total += __builtin_bitreverse64(0x0123456789abcdefULL);
  total += __builtin_rotateleft32(0x12345678U, 8);
  total += __builtin_rotateright32(0x12345678U, 8);
  total += __builtin_clzg(0U, 77);
  total += __builtin_ctzg(0U, 79);
  return total;
}

static int gnu_builtin_overflow(void) {
  int signed_result;
  unsigned int unsigned_result;
  long long long_result;
  int total = 0;
  total +=
      !__builtin_add_overflow(20, 22, &signed_result) && signed_result == 42;
  total += __builtin_add_overflow(INT_MAX, 1, &signed_result);
  total += !__builtin_sub_overflow(50U, 8U, &unsigned_result) &&
           unsigned_result == 42U;
  total += __builtin_sub_overflow(0U, 1U, &unsigned_result);
  total +=
      !__builtin_mul_overflow(6LL, 7LL, &long_result) && long_result == 42LL;
  total += __builtin_mul_overflow(LLONG_MAX, 2LL, &long_result);
  total +=
      !__builtin_sadd_overflow(17, 25, &signed_result) && signed_result == 42;
  total += !__builtin_uadd_overflow(19U, 23U, &unsigned_result) &&
           unsigned_result == 42U;
  total += __builtin_saddll_overflow(LLONG_MAX, 1LL, &long_result);
  return total;
}

static int gnu_builtin_floating(void) {
  double _Complex value = __builtin_complex(3.0, 4.0);
  double _Complex conjugate = __builtin_conj(value);
  int total = 0;
  total += __builtin_abs(-5);
  total += (int)__builtin_labs(-7L);
  total += (int)__builtin_llabs(-11LL);
  total += (int)__builtin_fabs(-13.0);
  total += (int)__builtin_fabsf(-17.0F);
  total += (int)__builtin_fabsl(-19.0L);
  total += __builtin_isinf(__builtin_inf());
  total += __builtin_isinf(__builtin_inff());
  total += __builtin_isinf(__builtin_infl());
  total += __builtin_isnan(__builtin_nan(""));
  total += __builtin_isnan(__builtin_nanf(""));
  total += __builtin_isnan(__builtin_nanl(""));
  total += __builtin_isfinite(23.0);
  total += __builtin_isnormal(29.0);
  total += __builtin_signbit(-31.0);
  total += __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL,
                                FP_ZERO, 0.0) == FP_ZERO;
  total += __builtin_isgreater(37.0, 31.0);
  total += __builtin_isgreaterequal(37.0, 37.0);
  total += __builtin_isless(31.0, 37.0);
  total += __builtin_islessequal(37.0, 37.0);
  total += __builtin_islessgreater(31.0, 37.0);
  total += !__builtin_isunordered(31.0, 37.0);
  total += (int)__builtin_creal(value);
  total += (int)__builtin_cimag(value);
  total += (int)__builtin_creal(conjugate);
  total += (int)-__builtin_cimag(conjugate);
  return total;
}

int main(void) {
  printf("%d %llu %d %d\n", gnu_builtin_bits(), gnu_builtin_reordering(),
         gnu_builtin_overflow(), gnu_builtin_floating());
  return 0;
}
