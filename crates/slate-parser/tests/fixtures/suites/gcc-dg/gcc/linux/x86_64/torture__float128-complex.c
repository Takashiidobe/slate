/* Test _Float128 complex arithmetic.  */
/* { dg-do run } */
/* { dg-options "" } */
/* { dg-add-options float128 } */
/* { dg-require-effective-target float128_runtime } */

#define WIDTH 128
#define EXT 0
#include "floatn-complex.h"

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: volatile f128 [storage=static] = const<f128>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: volatile complex<f128> [storage=static] = add<complex<f128>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f128>(2), aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: volatile complex<f128> [storage=static] = add<complex<f128>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f128>(2), aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: volatile complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(2), index1 = const<f128>(3)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fn:[0-9]+]] @fn(%[[VALUE_arg:[0-9]+]] arg: complex<f128>) -> complex<f128> [linkage=external] [abi=sysv64(byval<align=16>) -> sret<align=16>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE_arg]]), int_to_float<f128, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: volatile complex<f128> [storage=automatic];
// DEFAULT-NEXT:         if ne<complex<f128>, exceptions=observable>(read<complex<f128>, volatile>(%[[VALUE_b]]), read<complex<f128>, volatile>(%[[VALUE_c]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<complex<f128>, exceptions=observable>(read<complex<f128>, volatile>(%[[VALUE_b]]), read<complex<f128>, volatile>(%[[VALUE_d]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], add<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f128, volatile>(%[[VALUE_a]]), read<complex<f128>, volatile>(%[[VALUE_b]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%[[VALUE_r]])), const<f128>(3)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%[[VALUE_r]])), const<f128>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: complex<f128> [synthetic] = add<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE1]]), read<complex<f128>, volatile>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE2]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%[[VALUE_r]])), const<f128>(5)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%[[VALUE_r]])), const<f128>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: complex<f128> [synthetic] = sub<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE3]]), read<f128, volatile>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE4]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%[[VALUE_r]])), const<f128>(4)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%[[VALUE_r]])), const<f128>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: complex<f128> [synthetic] = div<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE5]]), add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(read<f128, volatile>(%[[VALUE_a]]), read<f128, volatile>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE6]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%[[VALUE_r]])), const<f128>(2)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%[[VALUE_r]])), const<f128>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: complex<f128> [synthetic] = mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE7]]), add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(read<f128, volatile>(%[[VALUE_a]]), read<f128, volatile>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE8]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%[[VALUE_r]])), const<f128>(4)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%[[VALUE_r]])), const<f128>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: complex<f128> [synthetic] = sub<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE9]]), read<complex<f128>, volatile>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE10]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%[[VALUE_r]])), const<f128>(2)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%[[VALUE_r]])), const<f128>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: complex<f128> [synthetic] = mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE11]]), read<complex<f128>, volatile>(%[[VALUE_r]]));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE12]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%[[VALUE_r]])), neg<f128>(const<f128>(5))), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%[[VALUE_r]])), const<f128>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: complex<f128> [synthetic] = div<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE13]]), read<complex<f128>, volatile>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: complex<f128> [synthetic] = add<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE15]]), aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(100), index1 = const<f128>(100)));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE16]]));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: complex<f128> [synthetic] = read<complex<f128>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: complex<f128> [synthetic] = sub<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%[[VALUE17]]), aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(100), index1 = const<f128>(100)));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], read<complex<f128>>(%[[VALUE18]]));
// DEFAULT-NEXT:         if ne<complex<f128>, exceptions=observable>(read<complex<f128>, volatile>(%[[VALUE_r]]), read<complex<f128>, volatile>(%[[VALUE_b]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%[[VALUE_r]], call<complex<f128>, signature=fn(complex<f128>) -> complex<f128>, abi=sysv64(byval<align=16>) -> sret<align=16>>(%[[VALUE_fn]], read<complex<f128>, volatile>(%[[VALUE_r]])));
// DEFAULT-NEXT:         call<complex<f128>, signature=fn(complex<f128>) -> complex<f128>, abi=sysv64(byval<align=16>) -> sret<align=16>>(%[[VALUE_fn]], read<complex<f128>, volatile>(%[[VALUE_r]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%[[VALUE_r]])), const<f128>(0.5)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%[[VALUE_r]])), const<f128>(0.75)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
