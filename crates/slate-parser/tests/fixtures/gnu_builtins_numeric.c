#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

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

int main(void) {
  printf("%d %llu %d %d\n", gnu_builtin_bits(), gnu_builtin_reordering(),
         gnu_builtin_overflow(), gnu_builtin_floating());
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
// DEFAULT-NEXT:     global %16 .str16: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([37, 100, 32, 37, 108, 108, 117, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%15 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @gnu_builtin_bits() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %20: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), call<i32, signature=fn(i32) -> i32>(__builtin_ffs, const<i32>(16)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%21));
// DEFAULT-NEXT:         let %22: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), call<i32, signature=fn(i64) -> i32>(__builtin_ffsl, const<i64>(32)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%23));
// DEFAULT-NEXT:         let %24: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), call<i32, signature=fn(i64) -> i32>(__builtin_ffsll, const<i64>(64)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%25));
// DEFAULT-NEXT:         let %26: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u32) -> i32>(__builtin_clz, const<u32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%27));
// DEFAULT-NEXT:         let %28: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u64) -> i32>(__builtin_clzl, const<u64>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%29));
// DEFAULT-NEXT:         let %30: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u64) -> i32>(__builtin_clzll, const<u64>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%31));
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%32), call<i32, signature=fn(u32) -> i32>(__builtin_ctz, const<u32>(16)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%33));
// DEFAULT-NEXT:         let %34: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), call<i32, signature=fn(u64) -> i32>(__builtin_ctzl, const<u64>(32)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%35));
// DEFAULT-NEXT:         let %36: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), call<i32, signature=fn(u64) -> i32>(__builtin_ctzll, const<u64>(64)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%37));
// DEFAULT-NEXT:         let %38: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), call<i32, signature=fn(i32) -> i32>(__builtin_clrsb, neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%39));
// DEFAULT-NEXT:         let %40: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), call<i32, signature=fn(i64) -> i32>(__builtin_clrsbl, neg<i64, overflow=ub>(const<i64>(2))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%41));
// DEFAULT-NEXT:         let %42: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), call<i32, signature=fn(i64) -> i32>(__builtin_clrsbll, neg<i64, overflow=ub>(const<i64>(4))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%43));
// DEFAULT-NEXT:         let %44: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), call<i32, signature=fn(u32) -> i32>(__builtin_popcount, const<u32>(240)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%45));
// DEFAULT-NEXT:         let %46: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), call<i32, signature=fn(u64) -> i32>(__builtin_popcountl, const<u64>(255)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%47));
// DEFAULT-NEXT:         let %48: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), call<i32, signature=fn(u64) -> i32>(__builtin_popcountll, const<u64>(65280)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%49));
// DEFAULT-NEXT:         let %50: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), call<i32, signature=fn(u32) -> i32>(__builtin_parity, const<u32>(7)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%51));
// DEFAULT-NEXT:         let %52: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), call<i32, signature=fn(u64) -> i32>(__builtin_parityl, const<u64>(15)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%53));
// DEFAULT-NEXT:         let %54: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), call<i32, signature=fn(u64) -> i32>(__builtin_parityll, const<u64>(31)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%55));
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @gnu_builtin_reordering() -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 total: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %56: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %57: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%56), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(__builtin_bswap16, truncate<u16, reason=arg, fits=always>(const<u32>(4660))))))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%57));
// DEFAULT-NEXT:         let %58: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %59: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%58), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(__builtin_bswap32, const<u32>(16909060))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%59));
// DEFAULT-NEXT:         let %60: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %61: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%60), call<u64, signature=fn(u64) -> u64>(__builtin_bswap64, const<u64>(72623859790382856)));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%61));
// DEFAULT-NEXT:         let %62: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %63: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%62), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(__builtin_bitreverse8, truncate<u8, reason=arg, fits=always>(const<u32>(18))))))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%63));
// DEFAULT-NEXT:         let %64: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %65: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%64), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(__builtin_bitreverse16, truncate<u16, reason=arg, fits=always>(const<u32>(4660))))))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%65));
// DEFAULT-NEXT:         let %66: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %67: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%66), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(__builtin_bitreverse32, const<u32>(305419896))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%67));
// DEFAULT-NEXT:         let %68: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %69: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%68), call<u64, signature=fn(u64) -> u64>(__builtin_bitreverse64, const<u64>(81985529216486895)));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%69));
// DEFAULT-NEXT:         let %70: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %71: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%70), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32, u32) -> u32>(__builtin_rotateleft32, const<u32>(305419896), reinterpret<u32, reason=arg, fits=always>(const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%71));
// DEFAULT-NEXT:         let %72: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %73: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%72), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32, u32) -> u32>(__builtin_rotateright32, const<u32>(305419896), reinterpret<u32, reason=arg, fits=always>(const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%73));
// DEFAULT-NEXT:         let %74: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %75: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%74), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32, i32) -> i32>(__builtin_clzg, const<u32>(0), const<i32>(77)))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%75));
// DEFAULT-NEXT:         let %76: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %77: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%76), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32, i32) -> i32>(__builtin_ctzg, const<u32>(0), const<i32>(79)))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%77));
// DEFAULT-NEXT:         return read<u64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @gnu_builtin_overflow() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 signed_result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 unsigned_result: u32 [storage=automatic];
// DEFAULT-NEXT:         let %8 long_result: i64 [storage=automatic];
// DEFAULT-NEXT:         let %9 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %78: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %79: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%78), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_add<bool>(const<i32>(20), const<i32>(22), deref(addr_of<ptr<i32>>(%6)))), eq<i32>(read<i32>(%6), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%79));
// DEFAULT-NEXT:         let %80: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %81: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%80), from_bool<i32, reason=promotion>(overflow_add<bool>(const<i32>(2147483647), const<i32>(1), deref(addr_of<ptr<i32>>(%6)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%81));
// DEFAULT-NEXT:         let %82: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %83: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%82), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_sub<bool>(const<u32>(50), const<u32>(8), deref(addr_of<ptr<u32>>(%7)))), eq<u32>(read<u32>(%7), const<u32>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%83));
// DEFAULT-NEXT:         let %84: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %85: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%84), from_bool<i32, reason=promotion>(overflow_sub<bool>(const<u32>(0), const<u32>(1), deref(addr_of<ptr<u32>>(%7)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%85));
// DEFAULT-NEXT:         let %86: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %87: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%86), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_mul<bool>(const<i64>(6), const<i64>(7), deref(addr_of<ptr<i64>>(%8)))), eq<i64>(read<i64>(%8), const<i64>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%87));
// DEFAULT-NEXT:         let %88: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %89: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%88), from_bool<i32, reason=promotion>(overflow_mul<bool>(const<i64>(9223372036854775807), const<i64>(2), deref(addr_of<ptr<i64>>(%8)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%89));
// DEFAULT-NEXT:         let %90: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %91: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%90), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(call<bool, signature=fn(i32, i32, ptr<i32>) -> bool>(__builtin_sadd_overflow, const<i32>(17), const<i32>(25), addr_of<ptr<i32>>(%6))), eq<i32>(read<i32>(%6), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%91));
// DEFAULT-NEXT:         let %92: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %93: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%92), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(call<bool, signature=fn(u32, u32, ptr<u32>) -> bool>(__builtin_uadd_overflow, const<u32>(19), const<u32>(23), addr_of<ptr<u32>>(%7))), eq<u32>(read<u32>(%7), const<u32>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%93));
// DEFAULT-NEXT:         let %94: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %95: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%94), from_bool<i32, reason=promotion>(call<bool, signature=fn(i64, i64, ptr<i64>) -> bool>(__builtin_saddll_overflow, const<i64>(9223372036854775807), const<i64>(1), addr_of<ptr<i64>>(%8))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%95));
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @gnu_builtin_floating() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 value: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(3.0), index1 = const<f64>(4.0));
// DEFAULT-NEXT:         let %12 conjugate: complex<f64> [storage=automatic] = call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(__builtin_conj, read<complex<f64>>(%11));
// DEFAULT-NEXT:         let %13 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %96: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %97: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%96), call<i32, signature=fn(i32) -> i32>(__builtin_abs, neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%97));
// DEFAULT-NEXT:         let %98: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %99: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%98), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64) -> i64>(__builtin_labs, neg<i64, overflow=ub>(const<i64>(7)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%99));
// DEFAULT-NEXT:         let %100: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %101: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%100), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64) -> i64>(__builtin_llabs, neg<i64, overflow=ub>(const<i64>(11)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%101));
// DEFAULT-NEXT:         let %102: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %103: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%102), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(__builtin_fabs, neg<f64>(const<f64>(13.0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%103));
// DEFAULT-NEXT:         let %104: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %105: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%104), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(__builtin_fabsf, neg<f32>(const<f32>(17.0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%105));
// DEFAULT-NEXT:         let %106: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %107: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%106), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(__builtin_fabsl, neg<f80>(const<f80>(19)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%107));
// DEFAULT-NEXT:         let %108: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %109: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%108), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f64, signature=fn() -> f64>(__builtin_inf))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%109));
// DEFAULT-NEXT:         let %110: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %111: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%110), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f32, signature=fn() -> f32>(__builtin_inff))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%111));
// DEFAULT-NEXT:         let %112: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %113: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%112), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f80, signature=fn() -> f80>(__builtin_infl))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%113));
// DEFAULT-NEXT:         let %114: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %115: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%114), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f64, signature=fn(ptr<const i8>) -> f64>(__builtin_nan, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%16))))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%115));
// DEFAULT-NEXT:         let %116: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %117: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%116), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f32, signature=fn(ptr<const i8>) -> f32>(__builtin_nanf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%17))))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%117));
// DEFAULT-NEXT:         let %118: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %119: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%118), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f80, signature=fn(ptr<const i8>) -> f80>(__builtin_nanl, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%18))))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%119));
// DEFAULT-NEXT:         let %120: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %121: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%120), from_bool<i32, reason=promotion>(float_class<bool, test=finite>(const<f64>(23.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%121));
// DEFAULT-NEXT:         let %122: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %123: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%122), from_bool<i32, reason=promotion>(float_class<bool, test=normal>(const<f64>(29.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%123));
// DEFAULT-NEXT:         let %124: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %125: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%124), from_bool<i32, reason=promotion>(float_class<bool, test=sign_bit>(neg<f64>(const<f64>(31.0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%125));
// DEFAULT-NEXT:         let %126: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %127: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%126), from_bool<i32, reason=promotion>(eq<i32>(conditional<i32>(float_class<bool, test=nan>(const<f64>(0.0)), const<i32>(0), conditional<i32>(float_class<bool, test=infinite>(const<f64>(0.0)), const<i32>(1), conditional<i32>(float_class<bool, test=normal>(const<f64>(0.0)), const<i32>(4), conditional<i32>(float_class<bool, test=subnormal>(const<f64>(0.0)), const<i32>(3), const<i32>(2))))), const<i32>(2))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%127));
// DEFAULT-NEXT:         let %128: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %129: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%128), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(31.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%129));
// DEFAULT-NEXT:         let %130: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %131: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%130), from_bool<i32, reason=promotion>(ge<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%131));
// DEFAULT-NEXT:         let %132: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %133: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%132), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%133));
// DEFAULT-NEXT:         let %134: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %135: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%134), from_bool<i32, reason=promotion>(le<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%135));
// DEFAULT-NEXT:         let %136: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %137: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%136), or<i32>(from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%137));
// DEFAULT-NEXT:         let %138: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %139: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%138), from_bool<i32, reason=promotion>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(const<f64>(31.0))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(const<f64>(37.0)))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%139));
// DEFAULT-NEXT:         let %140: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %141: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%140), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%11))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%141));
// DEFAULT-NEXT:         let %142: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %143: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%142), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%11))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%143));
// DEFAULT-NEXT:         let %144: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %145: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%144), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_creal, read<complex<f64>>(%12))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%145));
// DEFAULT-NEXT:         let %146: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %147: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%146), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f64>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(__builtin_cimag, read<complex<f64>>(%12)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%147));
// DEFAULT-NEXT:         return read<i32>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%19)), call<i32, signature=fn() -> i32>(%1), call<u64, signature=fn() -> u64>(%3), call<i32, signature=fn() -> i32>(%5), call<i32, signature=fn() -> i32>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
