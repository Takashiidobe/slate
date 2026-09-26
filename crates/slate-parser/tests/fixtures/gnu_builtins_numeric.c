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
// DEFAULT-NEXT:     global %109 .str109: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %112 .str112: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %115 .str115: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %120 .str120: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([37, 100, 32, 37, 108, 108, 117, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%15 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_ffs(%16 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @__builtin_ffsl(%18 <unnamed>: i64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @__builtin_ffsll(%20 <unnamed>: i64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @__builtin_clz(%22 <unnamed>: u32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @__builtin_clzl(%24 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_clzll(%26 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %29 @__builtin_ctz(%28 <unnamed>: u32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %31 @__builtin_ctzl(%30 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %33 @__builtin_ctzll(%32 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %35 @__builtin_clrsb(%34 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %37 @__builtin_clrsbl(%36 <unnamed>: i64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %39 @__builtin_clrsbll(%38 <unnamed>: i64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %41 @__builtin_popcount(%40 <unnamed>: u32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %43 @__builtin_popcountl(%42 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %45 @__builtin_popcountll(%44 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %47 @__builtin_parity(%46 <unnamed>: u32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %49 @__builtin_parityl(%48 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %51 @__builtin_parityll(%50 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @gnu_builtin_bits() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %121: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %122: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%121), call<i32, signature=fn(i32) -> i32>(%17, const<i32>(16)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%122));
// DEFAULT-NEXT:         let %123: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %124: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%123), call<i32, signature=fn(i64) -> i32>(%19, const<i64>(32)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%124));
// DEFAULT-NEXT:         let %125: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %126: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%125), call<i32, signature=fn(i64) -> i32>(%21, const<i64>(64)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%126));
// DEFAULT-NEXT:         let %127: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %128: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%127), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u32) -> i32>(%23, const<u32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%128));
// DEFAULT-NEXT:         let %129: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %130: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%129), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u64) -> i32>(%25, const<u64>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%130));
// DEFAULT-NEXT:         let %131: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %132: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%131), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u64) -> i32>(%27, const<u64>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%132));
// DEFAULT-NEXT:         let %133: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %134: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%133), call<i32, signature=fn(u32) -> i32>(%29, const<u32>(16)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%134));
// DEFAULT-NEXT:         let %135: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %136: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%135), call<i32, signature=fn(u64) -> i32>(%31, const<u64>(32)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%136));
// DEFAULT-NEXT:         let %137: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %138: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%137), call<i32, signature=fn(u64) -> i32>(%33, const<u64>(64)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%138));
// DEFAULT-NEXT:         let %139: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %140: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%139), call<i32, signature=fn(i32) -> i32>(%35, neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%140));
// DEFAULT-NEXT:         let %141: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %142: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%141), call<i32, signature=fn(i64) -> i32>(%37, neg<i64, overflow=ub>(const<i64>(2))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%142));
// DEFAULT-NEXT:         let %143: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %144: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%143), call<i32, signature=fn(i64) -> i32>(%39, neg<i64, overflow=ub>(const<i64>(4))));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%144));
// DEFAULT-NEXT:         let %145: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %146: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%145), call<i32, signature=fn(u32) -> i32>(%41, const<u32>(240)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%146));
// DEFAULT-NEXT:         let %147: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %148: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%147), call<i32, signature=fn(u64) -> i32>(%43, const<u64>(255)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%148));
// DEFAULT-NEXT:         let %149: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %150: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%149), call<i32, signature=fn(u64) -> i32>(%45, const<u64>(65280)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%150));
// DEFAULT-NEXT:         let %151: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %152: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%151), call<i32, signature=fn(u32) -> i32>(%47, const<u32>(7)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%152));
// DEFAULT-NEXT:         let %153: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %154: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%153), call<i32, signature=fn(u64) -> i32>(%49, const<u64>(15)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%154));
// DEFAULT-NEXT:         let %155: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %156: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%155), call<i32, signature=fn(u64) -> i32>(%51, const<u64>(31)));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%156));
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @__builtin_bswap16(%52 <unnamed>: u16) -> u16 [linkage=external];
// DEFAULT-NEXT:     fn %55 @__builtin_bswap32(%54 <unnamed>: u32) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %57 @__builtin_bswap64(%56 <unnamed>: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %59 @__builtin_bitreverse8(%58 <unnamed>: u8) -> u8 [linkage=external];
// DEFAULT-NEXT:     fn %61 @__builtin_bitreverse16(%60 <unnamed>: u16) -> u16 [linkage=external];
// DEFAULT-NEXT:     fn %63 @__builtin_bitreverse32(%62 <unnamed>: u32) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %65 @__builtin_bitreverse64(%64 <unnamed>: u64) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %68 @__builtin_rotateleft32(%66 <unnamed>: u32, %67 <unnamed>: u32) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %71 @__builtin_rotateright32(%69 <unnamed>: u32, %70 <unnamed>: u32) -> u32 [linkage=external];
// DEFAULT-NEXT:     fn %74 @__builtin_clzg(%72 <unnamed>: u32, %73 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %77 @__builtin_ctzg(%75 <unnamed>: u32, %76 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @gnu_builtin_reordering() -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 total: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %157: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %158: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%157), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%53, truncate<u16, reason=arg, fits=always>(const<u32>(4660))))))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%158));
// DEFAULT-NEXT:         let %159: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %160: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%159), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%55, const<u32>(16909060))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%160));
// DEFAULT-NEXT:         let %161: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %162: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%161), call<u64, signature=fn(u64) -> u64>(%57, const<u64>(72623859790382856)));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%162));
// DEFAULT-NEXT:         let %163: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %164: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%163), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%59, truncate<u8, reason=arg, fits=always>(const<u32>(18))))))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%164));
// DEFAULT-NEXT:         let %165: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %166: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%165), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%61, truncate<u16, reason=arg, fits=always>(const<u32>(4660))))))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%166));
// DEFAULT-NEXT:         let %167: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %168: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%167), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%63, const<u32>(305419896))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%168));
// DEFAULT-NEXT:         let %169: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %170: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%169), call<u64, signature=fn(u64) -> u64>(%65, const<u64>(81985529216486895)));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%170));
// DEFAULT-NEXT:         let %171: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %172: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%171), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32, u32) -> u32>(%68, const<u32>(305419896), reinterpret<u32, reason=arg, fits=always>(const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%172));
// DEFAULT-NEXT:         let %173: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %174: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%173), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32, u32) -> u32>(%71, const<u32>(305419896), reinterpret<u32, reason=arg, fits=always>(const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%174));
// DEFAULT-NEXT:         let %175: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %176: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%175), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32, i32) -> i32>(%74, const<u32>(0), const<i32>(77)))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%176));
// DEFAULT-NEXT:         let %177: u64 [synthetic] = read<u64>(%4);
// DEFAULT-NEXT:         let %178: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%177), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32, i32) -> i32>(%77, const<u32>(0), const<i32>(79)))));
// DEFAULT-NEXT:         write<u64>(%4, read<u64>(%178));
// DEFAULT-NEXT:         return read<u64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @__builtin_sadd_overflow(%78 <unnamed>: i32, %79 <unnamed>: i32, %80 <unnamed>: ptr<i32>) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %85 @__builtin_uadd_overflow(%82 <unnamed>: u32, %83 <unnamed>: u32, %84 <unnamed>: ptr<u32>) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %89 @__builtin_saddll_overflow(%86 <unnamed>: i64, %87 <unnamed>: i64, %88 <unnamed>: ptr<i64>) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %5 @gnu_builtin_overflow() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 signed_result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 unsigned_result: u32 [storage=automatic];
// DEFAULT-NEXT:         let %8 long_result: i64 [storage=automatic];
// DEFAULT-NEXT:         let %9 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %179: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %180: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%179), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_add<bool>(const<i32>(20), const<i32>(22), deref(addr_of<ptr<i32>>(%6)))), eq<i32>(read<i32>(%6), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%180));
// DEFAULT-NEXT:         let %181: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %182: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%181), from_bool<i32, reason=promotion>(overflow_add<bool>(const<i32>(2147483647), const<i32>(1), deref(addr_of<ptr<i32>>(%6)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%182));
// DEFAULT-NEXT:         let %183: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %184: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%183), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_sub<bool>(const<u32>(50), const<u32>(8), deref(addr_of<ptr<u32>>(%7)))), eq<u32>(read<u32>(%7), const<u32>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%184));
// DEFAULT-NEXT:         let %185: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %186: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%185), from_bool<i32, reason=promotion>(overflow_sub<bool>(const<u32>(0), const<u32>(1), deref(addr_of<ptr<u32>>(%7)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%186));
// DEFAULT-NEXT:         let %187: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %188: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%187), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_mul<bool>(const<i64>(6), const<i64>(7), deref(addr_of<ptr<i64>>(%8)))), eq<i64>(read<i64>(%8), const<i64>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%188));
// DEFAULT-NEXT:         let %189: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %190: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%189), from_bool<i32, reason=promotion>(overflow_mul<bool>(const<i64>(9223372036854775807), const<i64>(2), deref(addr_of<ptr<i64>>(%8)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%190));
// DEFAULT-NEXT:         let %191: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %192: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%191), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(call<bool, signature=fn(i32, i32, ptr<i32>) -> bool>(%81, const<i32>(17), const<i32>(25), addr_of<ptr<i32>>(%6))), eq<i32>(read<i32>(%6), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%192));
// DEFAULT-NEXT:         let %193: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %194: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%193), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(call<bool, signature=fn(u32, u32, ptr<u32>) -> bool>(%85, const<u32>(19), const<u32>(23), addr_of<ptr<u32>>(%7))), eq<u32>(read<u32>(%7), const<u32>(42)))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%194));
// DEFAULT-NEXT:         let %195: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %196: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%195), from_bool<i32, reason=promotion>(call<bool, signature=fn(i64, i64, ptr<i64>) -> bool>(%89, const<i64>(9223372036854775807), const<i64>(1), addr_of<ptr<i64>>(%8))));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%196));
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @__builtin_conj(%90 <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>];
// DEFAULT-NEXT:     fn %93 @__builtin_abs(%92 <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %95 @__builtin_labs(%94 <unnamed>: i64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %97 @__builtin_llabs(%96 <unnamed>: i64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %99 @__builtin_fabs(%98 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %101 @__builtin_fabsf(%100 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %103 @__builtin_fabsl(%102 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %104 @__builtin_inf() -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %105 @__builtin_inff() -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %106 @__builtin_infl() -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %108 @__builtin_nan(%107 <unnamed>: ptr<const i8>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %111 @__builtin_nanf(%110 <unnamed>: ptr<const i8>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %114 @__builtin_nanl(%113 <unnamed>: ptr<const i8>) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %117 @__builtin_creal(%116 <unnamed>: complex<f64>) -> f64 [linkage=external] [abi=sysv64(coerce<f64, f64>) -> scalar];
// DEFAULT-NEXT:     fn %119 @__builtin_cimag(%118 <unnamed>: complex<f64>) -> f64 [linkage=external] [abi=sysv64(coerce<f64, f64>) -> scalar];
// DEFAULT-NEXT:     fn %10 @gnu_builtin_floating() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 value: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(3.0), index1 = const<f64>(4.0));
// DEFAULT-NEXT:         let %12 conjugate: complex<f64> [storage=automatic] = call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%91, read<complex<f64>>(%11));
// DEFAULT-NEXT:         let %13 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %197: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %198: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%197), call<i32, signature=fn(i32) -> i32>(%93, neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%198));
// DEFAULT-NEXT:         let %199: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %200: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%199), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64) -> i64>(%95, neg<i64, overflow=ub>(const<i64>(7)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%200));
// DEFAULT-NEXT:         let %201: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %202: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%201), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64) -> i64>(%97, neg<i64, overflow=ub>(const<i64>(11)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%202));
// DEFAULT-NEXT:         let %203: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %204: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%203), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%99, neg<f64>(const<f64>(13.0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%204));
// DEFAULT-NEXT:         let %205: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %206: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%205), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%101, neg<f32>(const<f32>(17.0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%206));
// DEFAULT-NEXT:         let %207: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %208: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%207), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%103, neg<f80>(const<f80>(19)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%208));
// DEFAULT-NEXT:         let %209: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %210: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%209), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f64, signature=fn() -> f64>(%104))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%210));
// DEFAULT-NEXT:         let %211: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %212: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%211), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f32, signature=fn() -> f32>(%105))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%212));
// DEFAULT-NEXT:         let %213: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %214: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%213), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f80, signature=fn() -> f80>(%106))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%214));
// DEFAULT-NEXT:         let %215: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %216: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%215), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f64, signature=fn(ptr<const i8>) -> f64>(%108, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%109))))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%216));
// DEFAULT-NEXT:         let %217: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %218: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%217), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f32, signature=fn(ptr<const i8>) -> f32>(%111, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%112))))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%218));
// DEFAULT-NEXT:         let %219: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %220: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%219), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f80, signature=fn(ptr<const i8>) -> f80>(%114, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%115))))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%220));
// DEFAULT-NEXT:         let %221: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %222: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%221), from_bool<i32, reason=promotion>(float_class<bool, test=finite>(const<f64>(23.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%222));
// DEFAULT-NEXT:         let %223: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %224: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%223), from_bool<i32, reason=promotion>(float_class<bool, test=normal>(const<f64>(29.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%224));
// DEFAULT-NEXT:         let %225: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %226: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%225), from_bool<i32, reason=promotion>(float_class<bool, test=sign_bit>(neg<f64>(const<f64>(31.0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%226));
// DEFAULT-NEXT:         let %227: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %228: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%227), from_bool<i32, reason=promotion>(eq<i32>(conditional<i32>(float_class<bool, test=nan>(const<f64>(0.0)), const<i32>(0), conditional<i32>(float_class<bool, test=infinite>(const<f64>(0.0)), const<i32>(1), conditional<i32>(float_class<bool, test=normal>(const<f64>(0.0)), const<i32>(4), conditional<i32>(float_class<bool, test=subnormal>(const<f64>(0.0)), const<i32>(3), const<i32>(2))))), const<i32>(2))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%228));
// DEFAULT-NEXT:         let %229: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %230: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%229), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(31.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%230));
// DEFAULT-NEXT:         let %231: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %232: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%231), from_bool<i32, reason=promotion>(ge<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%232));
// DEFAULT-NEXT:         let %233: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %234: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%233), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%234));
// DEFAULT-NEXT:         let %235: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %236: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%235), from_bool<i32, reason=promotion>(le<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%236));
// DEFAULT-NEXT:         let %237: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %238: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%237), or<i32>(from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%238));
// DEFAULT-NEXT:         let %239: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %240: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%239), from_bool<i32, reason=promotion>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(const<f64>(31.0))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(const<f64>(37.0)))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%240));
// DEFAULT-NEXT:         let %241: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %242: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%241), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%117, read<complex<f64>>(%11))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%242));
// DEFAULT-NEXT:         let %243: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %244: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%243), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%119, read<complex<f64>>(%11))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%244));
// DEFAULT-NEXT:         let %245: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %246: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%245), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%117, read<complex<f64>>(%12))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%246));
// DEFAULT-NEXT:         let %247: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:         let %248: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%247), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f64>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%119, read<complex<f64>>(%12)))));
// DEFAULT-NEXT:         write<i32>(%13, read<i32>(%248));
// DEFAULT-NEXT:         return read<i32>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%120)), call<i32, signature=fn() -> i32>(%1), call<u64, signature=fn() -> u64>(%3), call<i32, signature=fn() -> i32>(%5), call<i32, signature=fn() -> i32>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
