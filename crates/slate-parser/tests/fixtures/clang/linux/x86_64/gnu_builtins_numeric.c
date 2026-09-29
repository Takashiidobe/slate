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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([37, 100, 32, 37, 108, 108, 117, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffs:[0-9]+]] @__builtin_ffs(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffsl:[0-9]+]] @__builtin_ffsl(%[[VALUE1:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ffsll:[0-9]+]] @__builtin_ffsll(%[[VALUE2:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clz:[0-9]+]] @__builtin_clz(%[[VALUE3:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clzl:[0-9]+]] @__builtin_clzl(%[[VALUE4:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clzll:[0-9]+]] @__builtin_clzll(%[[VALUE5:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctz:[0-9]+]] @__builtin_ctz(%[[VALUE6:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctzl:[0-9]+]] @__builtin_ctzl(%[[VALUE7:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctzll:[0-9]+]] @__builtin_ctzll(%[[VALUE8:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsb:[0-9]+]] @__builtin_clrsb(%[[VALUE9:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsbl:[0-9]+]] @__builtin_clrsbl(%[[VALUE10:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clrsbll:[0-9]+]] @__builtin_clrsbll(%[[VALUE11:[0-9]+]] <unnamed>: i64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcount:[0-9]+]] @__builtin_popcount(%[[VALUE12:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcountl:[0-9]+]] @__builtin_popcountl(%[[VALUE13:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_popcountll:[0-9]+]] @__builtin_popcountll(%[[VALUE14:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parity:[0-9]+]] @__builtin_parity(%[[VALUE15:[0-9]+]] <unnamed>: u32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parityl:[0-9]+]] @__builtin_parityl(%[[VALUE16:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_parityll:[0-9]+]] @__builtin_parityll(%[[VALUE17:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_builtin_bits:[0-9]+]] @gnu_builtin_bits() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_ffs]], const<i32>(16)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsl]], const<i64>(32)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_ffsll]], const<i64>(64)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_clz]], const<u32>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzl]], const<u64>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), from_bool<i32, reason=promotion>(eq<i32>(call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_clzll]], const<u64>(1)), reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=unknown>(sub<u64, overflow=wrap>(mul<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_ctz]], const<u32>(16)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzl]], const<u64>(32)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_ctzll]], const<u64>(64)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_clrsb]], neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbl]], neg<i64, overflow=ub>(const<i64>(2))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), call<i32, signature=fn(i64) -> i32>(%[[VALUE___builtin_clrsbll]], neg<i64, overflow=ub>(const<i64>(4))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE42]]), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_popcount]], const<u32>(240)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountl]], const<u64>(255)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_popcountll]], const<u64>(65280)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE48]]), call<i32, signature=fn(u32) -> i32>(%[[VALUE___builtin_parity]], const<u32>(7)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE49]]));
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE50]]), call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityl]], const<u64>(15)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE51]]));
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE52]]), call<i32, signature=fn(u64) -> i32>(%[[VALUE___builtin_parityll]], const<u64>(31)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE53]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap16:[0-9]+]] @__builtin_bswap16(%[[VALUE54:[0-9]+]] <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap32:[0-9]+]] @__builtin_bswap32(%[[VALUE55:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bswap64:[0-9]+]] @__builtin_bswap64(%[[VALUE56:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bitreverse8:[0-9]+]] @__builtin_bitreverse8(%[[VALUE57:[0-9]+]] <unnamed>: u8) -> u8 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bitreverse16:[0-9]+]] @__builtin_bitreverse16(%[[VALUE58:[0-9]+]] <unnamed>: u16) -> u16 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bitreverse32:[0-9]+]] @__builtin_bitreverse32(%[[VALUE59:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_bitreverse64:[0-9]+]] @__builtin_bitreverse64(%[[VALUE60:[0-9]+]] <unnamed>: u64) -> u64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rotateleft32:[0-9]+]] @__builtin_rotateleft32(%[[VALUE61:[0-9]+]] <unnamed>: u32, %[[VALUE62:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rotateright32:[0-9]+]] @__builtin_rotateright32(%[[VALUE63:[0-9]+]] <unnamed>: u32, %[[VALUE64:[0-9]+]] <unnamed>: u32) -> u32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_clzg:[0-9]+]] @__builtin_clzg(%[[VALUE65:[0-9]+]] <unnamed>: u32, %[[VALUE66:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ctzg:[0-9]+]] @__builtin_ctzg(%[[VALUE67:[0-9]+]] <unnamed>: u32, %[[VALUE68:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_builtin_reordering:[0-9]+]] @gnu_builtin_reordering() -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_total_2:[0-9]+]] total: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE69]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bswap16]], truncate<u16, reason=arg, fits=always>(const<u32>(4660))))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE70]]));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE71]]), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bswap32]], const<u32>(16909060))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE72]]));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE73]]), call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bswap64]], const<u64>(72623859790382856)));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE74]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE75]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8) -> u8>(%[[VALUE___builtin_bitreverse8]], truncate<u8, reason=arg, fits=always>(const<u32>(18))))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE76]]));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE77]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn(u16) -> u16>(%[[VALUE___builtin_bitreverse16]], truncate<u16, reason=arg, fits=always>(const<u32>(4660))))))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE78]]));
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE79]]), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32) -> u32>(%[[VALUE___builtin_bitreverse32]], const<u32>(305419896))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE80]]));
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE81]]), call<u64, signature=fn(u64) -> u64>(%[[VALUE___builtin_bitreverse64]], const<u64>(81985529216486895)));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE82]]));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE83]]), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE___builtin_rotateleft32]], const<u32>(305419896), reinterpret<u32, reason=arg, fits=always>(const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE84]]));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE85]]), widen<u64, reason=usual_arith>(call<u32, signature=fn(u32, u32) -> u32>(%[[VALUE___builtin_rotateright32]], const<u32>(305419896), reinterpret<u32, reason=arg, fits=always>(const<i32>(8)))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE86]]));
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE87]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32, i32) -> i32>(%[[VALUE___builtin_clzg]], const<u32>(0), const<i32>(77)))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE88]]));
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE89]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(u32, i32) -> i32>(%[[VALUE___builtin_ctzg]], const<u32>(0), const<i32>(79)))));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_total_2]], read<u64>(%[[VALUE90]]));
// DEFAULT-NEXT:         return read<u64>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sadd_overflow:[0-9]+]] @__builtin_sadd_overflow(%[[VALUE91:[0-9]+]] <unnamed>: i32, %[[VALUE92:[0-9]+]] <unnamed>: i32, %[[VALUE93:[0-9]+]] <unnamed>: ptr<i32>) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_uadd_overflow:[0-9]+]] @__builtin_uadd_overflow(%[[VALUE94:[0-9]+]] <unnamed>: u32, %[[VALUE95:[0-9]+]] <unnamed>: u32, %[[VALUE96:[0-9]+]] <unnamed>: ptr<u32>) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_saddll_overflow:[0-9]+]] @__builtin_saddll_overflow(%[[VALUE97:[0-9]+]] <unnamed>: i64, %[[VALUE98:[0-9]+]] <unnamed>: i64, %[[VALUE99:[0-9]+]] <unnamed>: ptr<i64>) -> bool [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_builtin_overflow:[0-9]+]] @gnu_builtin_overflow() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_signed_result:[0-9]+]] signed_result: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_unsigned_result:[0-9]+]] unsigned_result: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_long_result:[0-9]+]] long_result: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_total_3:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE100]]), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_add<bool>(const<i32>(20), const<i32>(22), deref(addr_of<ptr<i32>>(%[[VALUE_signed_result]])))), eq<i32>(read<i32>(%[[VALUE_signed_result]]), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE101]]));
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE102]]), from_bool<i32, reason=promotion>(overflow_add<bool>(const<i32>(2147483647), const<i32>(1), deref(addr_of<ptr<i32>>(%[[VALUE_signed_result]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE103]]));
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE104]]), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_sub<bool>(const<u32>(50), const<u32>(8), deref(addr_of<ptr<u32>>(%[[VALUE_unsigned_result]])))), eq<u32>(read<u32>(%[[VALUE_unsigned_result]]), const<u32>(42)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE105]]));
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE106]]), from_bool<i32, reason=promotion>(overflow_sub<bool>(const<u32>(0), const<u32>(1), deref(addr_of<ptr<u32>>(%[[VALUE_unsigned_result]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE107]]));
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE108]]), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(overflow_mul<bool>(const<i64>(6), const<i64>(7), deref(addr_of<ptr<i64>>(%[[VALUE_long_result]])))), eq<i64>(read<i64>(%[[VALUE_long_result]]), const<i64>(42)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE109]]));
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE110]]), from_bool<i32, reason=promotion>(overflow_mul<bool>(const<i64>(9223372036854775807), const<i64>(2), deref(addr_of<ptr<i64>>(%[[VALUE_long_result]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE111]]));
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE112]]), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(call<bool, signature=fn(i32, i32, ptr<i32>) -> bool>(%[[VALUE___builtin_sadd_overflow]], const<i32>(17), const<i32>(25), addr_of<ptr<i32>>(%[[VALUE_signed_result]]))), eq<i32>(read<i32>(%[[VALUE_signed_result]]), const<i32>(42)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE113]]));
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE114]]), from_bool<i32, reason=promotion>(logical_and<bool>(not<bool>(call<bool, signature=fn(u32, u32, ptr<u32>) -> bool>(%[[VALUE___builtin_uadd_overflow]], const<u32>(19), const<u32>(23), addr_of<ptr<u32>>(%[[VALUE_unsigned_result]]))), eq<u32>(read<u32>(%[[VALUE_unsigned_result]]), const<u32>(42)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE115]]));
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE116]]), from_bool<i32, reason=promotion>(call<bool, signature=fn(i64, i64, ptr<i64>) -> bool>(%[[VALUE___builtin_saddll_overflow]], const<i64>(9223372036854775807), const<i64>(1), addr_of<ptr<i64>>(%[[VALUE_long_result]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE117]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_conj:[0-9]+]] @__builtin_conj(%[[VALUE118:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [memory=none] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abs:[0-9]+]] @__builtin_abs(%[[VALUE119:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_labs:[0-9]+]] @__builtin_labs(%[[VALUE120:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llabs:[0-9]+]] @__builtin_llabs(%[[VALUE121:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabs:[0-9]+]] @__builtin_fabs(%[[VALUE122:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsf:[0-9]+]] @__builtin_fabsf(%[[VALUE123:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsl:[0-9]+]] @__builtin_fabsl(%[[VALUE124:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff:[0-9]+]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infl:[0-9]+]] @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan:[0-9]+]] @__builtin_nan(%[[VALUE125:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf:[0-9]+]] @__builtin_nanf(%[[VALUE126:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanl:[0-9]+]] @__builtin_nanl(%[[VALUE127:[0-9]+]] <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_creal:[0-9]+]] @__builtin_creal(%[[VALUE128:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cimag:[0-9]+]] @__builtin_cimag(%[[VALUE129:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [memory=none] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_builtin_floating:[0-9]+]] @gnu_builtin_floating() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_value:[0-9]+]] value: complex<f64> [storage=automatic] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(3.0), index1 = const<f64>(4.0));
// DEFAULT-NEXT:         let %[[VALUE_conjugate:[0-9]+]] conjugate: complex<f64> [storage=automatic] = call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE___builtin_conj]], read<complex<f64>>(%[[VALUE_value]]));
// DEFAULT-NEXT:         let %[[VALUE_total_4:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE130]]), call<i32, signature=fn(i32) -> i32>(%[[VALUE___builtin_abs]], neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE131]]));
// DEFAULT-NEXT:         let %[[VALUE132:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE133:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE132]]), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64) -> i64>(%[[VALUE___builtin_labs]], neg<i64, overflow=ub>(const<i64>(7)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE133]]));
// DEFAULT-NEXT:         let %[[VALUE134:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE134]]), truncate<i32, reason=explicit, fits=unknown>(call<i64, signature=fn(i64) -> i64>(%[[VALUE___builtin_llabs]], neg<i64, overflow=ub>(const<i64>(11)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE135]]));
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE137:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE136]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_fabs]], neg<f64>(const<f64>(13.0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE137]]));
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE139:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE138]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_fabsf]], neg<f32>(const<f32>(17.0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE139]]));
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE140]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_fabsl]], neg<f80>(const<f80>(19)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE141]]));
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE143:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE142]]), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE143]]));
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE145:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE144]]), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE145]]));
// DEFAULT-NEXT:         let %[[VALUE146:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE147:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE146]]), from_bool<i32, reason=promotion>(float_class<bool, test=infinite>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE147]]));
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE148]]), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE149]]));
// DEFAULT-NEXT:         let %[[VALUE150:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE151:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE150]]), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]]))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE151]]));
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE153:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE152]]), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]]))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE153]]));
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE155:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE154]]), from_bool<i32, reason=promotion>(float_class<bool, test=finite>(const<f64>(23.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE155]]));
// DEFAULT-NEXT:         let %[[VALUE156:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE157:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE156]]), from_bool<i32, reason=promotion>(float_class<bool, test=normal>(const<f64>(29.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE157]]));
// DEFAULT-NEXT:         let %[[VALUE158:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE159:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE158]]), from_bool<i32, reason=promotion>(float_class<bool, test=sign_bit>(neg<f64>(const<f64>(31.0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE159]]));
// DEFAULT-NEXT:         let %[[VALUE160:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE161:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE160]]), from_bool<i32, reason=promotion>(eq<i32>(conditional<i32>(float_class<bool, test=nan>(const<f64>(0.0)), const<i32>(0), conditional<i32>(float_class<bool, test=infinite>(const<f64>(0.0)), const<i32>(1), conditional<i32>(float_class<bool, test=normal>(const<f64>(0.0)), const<i32>(4), conditional<i32>(float_class<bool, test=subnormal>(const<f64>(0.0)), const<i32>(3), const<i32>(2))))), const<i32>(2))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE161]]));
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE163:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE162]]), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(31.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE163]]));
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE165:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE164]]), from_bool<i32, reason=promotion>(ge<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE165]]));
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE167:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE166]]), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE167]]));
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE169:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE168]]), from_bool<i32, reason=promotion>(le<f64, exceptions=ignore>(const<f64>(37.0), const<f64>(37.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE169]]));
// DEFAULT-NEXT:         let %[[VALUE170:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE171:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE170]]), or<i32>(from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0))), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(const<f64>(31.0), const<f64>(37.0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE171]]));
// DEFAULT-NEXT:         let %[[VALUE172:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE173:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE172]]), from_bool<i32, reason=promotion>(not<bool>(ne<i32>(or<i32>(from_bool<i32, reason=promotion>(float_class<bool, test=nan>(const<f64>(31.0))), from_bool<i32, reason=promotion>(float_class<bool, test=nan>(const<f64>(37.0)))), const<i32>(0)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE173]]));
// DEFAULT-NEXT:         let %[[VALUE174:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE175:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE174]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_value]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE175]]));
// DEFAULT-NEXT:         let %[[VALUE176:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE177:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE176]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_value]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE177]]));
// DEFAULT-NEXT:         let %[[VALUE178:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE179:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE178]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_creal]], read<complex<f64>>(%[[VALUE_conjugate]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE179]]));
// DEFAULT-NEXT:         let %[[VALUE180:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE181:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE180]]), float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(neg<f64>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE___builtin_cimag]], read<complex<f64>>(%[[VALUE_conjugate]])))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE181]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_4]])), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_builtin_bits]]), call<u64, signature=fn() -> u64>(%[[VALUE_gnu_builtin_reordering]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_builtin_overflow]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_builtin_floating]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
