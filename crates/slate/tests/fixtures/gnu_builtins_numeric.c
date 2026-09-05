#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

// @lowering-fn-begin
// @rewrite-fn-begin
static int gnu_builtin_bits(void) {
  int total  = 0;
  total     += __builtin_ffs(16);
  total     += __builtin_ffsl(32L);
  total     += __builtin_ffsll(64LL);
  total     += __builtin_clz(1U) == (int)(sizeof(unsigned int) * CHAR_BIT - 1);
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
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static unsigned long long gnu_builtin_reordering(void) {
  unsigned long long total  = 0;
  total                    += __builtin_bswap16(0x1234U);
  total                    += __builtin_bswap32(0x01020304U);
  total                    += __builtin_bswap64(0x0102030405060708ULL);
  total                    += __builtin_bitreverse8(0x12U);
  total                    += __builtin_bitreverse16(0x1234U);
  total                    += __builtin_bitreverse32(0x12345678U);
  total                    += __builtin_bitreverse64(0x0123456789abcdefULL);
  total                    += __builtin_rotateleft32(0x12345678U, 8);
  total                    += __builtin_rotateright32(0x12345678U, 8);
  total                    += __builtin_clzg(0U, 77);
  total                    += __builtin_ctzg(0U, 79);
  return total;
}
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int gnu_builtin_overflow(void) {
  int          signed_result;
  unsigned int unsigned_result;
  long long    long_result;
  int          total = 0;
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
// @rewrite-fn-end
// @lowering-fn-end

// @lowering-fn-begin
// @rewrite-fn-begin
static int gnu_builtin_floating(void) {
  double _Complex value      = __builtin_complex(3.0, 4.0);
  double _Complex conjugate  = __builtin_conj(value);
  int total                  = 0;
  total                     += __builtin_abs(-5);
  total                     += (int)__builtin_labs(-7L);
  total                     += (int)__builtin_llabs(-11LL);
  total                     += (int)__builtin_fabs(-13.0);
  total                     += (int)__builtin_fabsf(-17.0F);
  total                     += (int)__builtin_fabsl(-19.0L);
  total                     += __builtin_isinf(__builtin_inf());
  total                     += __builtin_isinf(__builtin_inff());
  total                     += __builtin_isinf(__builtin_infl());
  total                     += __builtin_isnan(__builtin_nan(""));
  total                     += __builtin_isnan(__builtin_nanf(""));
  total                     += __builtin_isnan(__builtin_nanl(""));
  total                     += __builtin_isfinite(23.0);
  total                     += __builtin_isnormal(29.0);
  total                     += __builtin_signbit(-31.0);
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
// @rewrite-fn-end
// @lowering-fn-end

int main(void) {
  printf("%d %llu %d %d\n", gnu_builtin_bits(), gnu_builtin_reordering(),
         gnu_builtin_overflow(), gnu_builtin_floating());
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: fn gnu_builtin_bits() -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 31;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = u8::BITS as u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 63;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = u8::BITS as u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = u8::BITS as u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 63;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} - {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 31;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 62;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 61;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn gnu_builtin_reordering() -> u64 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 13330;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 67305985;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 578437695752307201u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 72;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 11336;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 510274632;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 17848844570815808640u64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 878082066;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 2014458966;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 77;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = 79;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn gnu_builtin_overflow() -> i32 {
// COMMON-LOWERING-DAG:     let mut signed_result: i32 = 0;
// COMMON-LOWERING-DAG:     let mut unsigned_result: u32 = 0;
// COMMON-LOWERING-DAG:     let mut long_result: i64 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 22;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-LOWERING-DAG:     signed_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = signed_result;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = i32::MAX as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-LOWERING-DAG:     signed_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 50;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 8;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-LOWERING-DAG:     unsigned_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u32 = unsigned_result;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u32 = 42;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-LOWERING-DAG:     unsigned_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 6;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 7;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.0 as i64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -9223372036854775808 || {{__v[0-9]+}}.0 > 9223372036854775807);
// COMMON-LOWERING-DAG:     long_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i64 = long_result;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i64 = 42;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = i64::MAX as i64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 2;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.0 as i64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -9223372036854775808 || {{__v[0-9]+}}.0 > 9223372036854775807);
// COMMON-LOWERING-DAG:     long_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 17;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 25;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}.0 as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-LOWERING-DAG:     signed_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = signed_result;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 19;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = 23;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: u32 = {{__v[0-9]+}}.0 as u32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-LOWERING-DAG:     unsigned_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u32 = unsigned_result;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: u32 = 42;
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     } else {
// COMMON-LOWERING-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-LOWERING-DAG:         {{__v[0-9]+}}
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = i64::MAX as i64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add({{__v[0-9]+}});
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i64 = {{__v[0-9]+}}.0 as i64;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -9223372036854775808 || {{__v[0-9]+}}.0 > 9223372036854775807);
// COMMON-LOWERING-DAG:     long_result = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// COMMON-LOWERING-DAG: fn gnu_builtin_floating() -> i32 {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 3.0, im: 4.0 };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-LOWERING-DAG:         re: {{__v[0-9]+}}.re,
// COMMON-LOWERING-DAG:         im: -{{__v[0-9]+}}.im,
// COMMON-LOWERING-DAG:     };
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = 13.0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f32 = 17.0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.re;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}}.im;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: f64 = -{{__v[0-9]+}};
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:     return {{__v[0-9]+}};
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 152, 3, 64]);
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = __slate_f80_to_i32({{__v[0-9]+}});
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.900000e+01f128;
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: fn gnu_builtin_bits() -> i32 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 8;
// COMMON-REWRITES-DAG:     0 + {{__v[0-9]+}}
// COMMON-REWRITES-DAG:         + 6
// COMMON-REWRITES-DAG:         + 7
// COMMON-REWRITES-DAG:         + ((31 == ((4 * (u8::BITS as u64) - 1) as i32)) as i32)
// COMMON-REWRITES-DAG:         + ((63 == (((u8::BITS as u64) * (u8::BITS as u64) - 1) as i32)) as i32)
// COMMON-REWRITES-DAG:         + ((63 == ((8 * {{__v[0-9]+}} - 1) as i32)) as i32)
// COMMON-REWRITES-DAG:         + 4
// COMMON-REWRITES-DAG:         + 5
// COMMON-REWRITES-DAG:         + 6
// COMMON-REWRITES-DAG:         + 31
// COMMON-REWRITES-DAG:         + 62
// COMMON-REWRITES-DAG:         + 61
// COMMON-REWRITES-DAG:         + 4
// COMMON-REWRITES-DAG:         + 8
// COMMON-REWRITES-DAG:         + 8
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 0
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn gnu_builtin_reordering() -> u64 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u64 = 13330;
// COMMON-REWRITES-DAG:     0 + {{__v[0-9]+}}
// COMMON-REWRITES-DAG:         + 67305985
// COMMON-REWRITES-DAG:         + 578437695752307201u64
// COMMON-REWRITES-DAG:         + 72
// COMMON-REWRITES-DAG:         + 11336
// COMMON-REWRITES-DAG:         + 510274632
// COMMON-REWRITES-DAG:         + 17848844570815808640u64
// COMMON-REWRITES-DAG:         + 878082066
// COMMON-REWRITES-DAG:         + 2014458966
// COMMON-REWRITES-DAG:         + 77
// COMMON-REWRITES-DAG:         + 79
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn gnu_builtin_overflow() -> i32 {
// COMMON-REWRITES-DAG:     let mut signed_result: i32 = 0;
// COMMON-REWRITES-DAG:     let mut unsigned_result: u32 = 0;
// COMMON-REWRITES-DAG:     let mut long_result: i64 = 0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 20;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(22 as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-REWRITES-DAG:     signed_result = {{__v[0-9]+}}.0 as i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = signed_result == 42;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = i32::MAX as i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(1 as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-REWRITES-DAG:     signed_result = {{__v[0-9]+}}.0 as i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 50;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub(8 as u32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-REWRITES-DAG:     unsigned_result = {{__v[0-9]+}}.0 as u32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = unsigned_result == 42;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_sub(1 as u32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-REWRITES-DAG:     unsigned_result = {{__v[0-9]+}}.0 as u32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i64 = 6;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul(7 as i64);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -9223372036854775808 || {{__v[0-9]+}}.0 > 9223372036854775807);
// COMMON-REWRITES-DAG:     long_result = {{__v[0-9]+}}.0 as i64;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = long_result == 42;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i64 = i64::MAX as i64;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_mul(2 as i64);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -9223372036854775808 || {{__v[0-9]+}}.0 > 9223372036854775807);
// COMMON-REWRITES-DAG:     long_result = {{__v[0-9]+}}.0 as i64;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 17;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(25 as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -2147483648 || {{__v[0-9]+}}.0 > 2147483647);
// COMMON-REWRITES-DAG:     signed_result = {{__v[0-9]+}}.0 as i32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = signed_result == 42;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: u32 = 19;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(23 as u32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || {{__v[0-9]+}}.0 > 4294967295;
// COMMON-REWRITES-DAG:     unsigned_result = {{__v[0-9]+}}.0 as u32;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = if {{__v[0-9]+}} {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = unsigned_result == 42;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     } else {
// COMMON-REWRITES-DAG:         let {{__v[0-9]+}}: bool = false;
// COMMON-REWRITES-DAG:         {{__v[0-9]+}}
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + ({{__v[0-9]+}} as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i64 = i64::MAX as i64;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}} = {{__v[0-9]+}}.overflowing_add(1 as i64);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.1 || ({{__v[0-9]+}}.0 < -9223372036854775808 || {{__v[0-9]+}}.0 > 9223372036854775807);
// COMMON-REWRITES-DAG:     long_result = {{__v[0-9]+}}.0 as i64;
// COMMON-REWRITES-DAG:     {{__v[0-9]+}} + ({{__v[0-9]+}} as i32)
// COMMON-REWRITES-DAG: }
// COMMON-REWRITES-DAG: fn gnu_builtin_floating() -> i32 {
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex { re: 3.0, im: 4.0 };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: num_complex::Complex<f64> = num_complex::Complex {
// COMMON-REWRITES-DAG:         re: {{__v[0-9]+}}.re,
// COMMON-REWRITES-DAG:         im: -{{__v[0-9]+}}.im,
// COMMON-REWRITES-DAG:     };
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0 + {{__v[0-9]+}} + 7 + 11 + ((13.0 as f64) as i32) + ((17.0 as f32) as i32);
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}}
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + ((2 == {{__v[0-9]+}}) as i32)
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1
// COMMON-REWRITES-DAG:         + 1;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-DAG:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// COMMON-REWRITES-DAG:     {{__v[0-9]+}}
// COMMON-REWRITES-DAG:         + ({{__v[0-9]+}} as i32)
// COMMON-REWRITES-DAG:         + ({{__v[0-9]+}}.re as i32)
// COMMON-REWRITES-DAG:         + ({{__v[0-9]+}}.im as i32)
// COMMON-REWRITES-DAG:         + ({{__v[0-9]+}}.re as i32)
// COMMON-REWRITES-DAG:         + (-{{__v[0-9]+}}.im as i32)
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 152, 3, 64]);
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = __slate_f80_to_i32({{__v[0-9]+}});
// REWRITES-X86_64-GNU-DAG:         + {{__v[0-9]+}}
// SLATE-FILECHECK-END rewrites-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-aarch64-gnu
// REWRITES-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: f128 = 1.900000e+01f128;
// REWRITES-AARCH64-GNU-DAG:         + ({{__v[0-9]+}} as i32)
// SLATE-FILECHECK-END rewrites-aarch64-gnu
