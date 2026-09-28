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
// DEFAULT-NEXT:     global %2 a: volatile f128 [storage=static] = const<f128>(1) [linkage=external];
// DEFAULT-NEXT:     global %3 b: volatile complex<f128> [storage=static] = add<complex<f128>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f128>(2), aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(3))) [linkage=external];
// DEFAULT-NEXT:     global %4 c: volatile complex<f128> [storage=static] = add<complex<f128>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f128>(2), aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(3))) [linkage=external];
// DEFAULT-NEXT:     global %5 d: volatile complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(2), index1 = const<f128>(3)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%10 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @fn(%7 arg: complex<f128>) -> complex<f128> [linkage=external] [abi=sysv64(byval<align=16>) -> sret<align=16>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%7), int_to_float<f128, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 r: volatile complex<f128> [storage=automatic];
// DEFAULT-NEXT:         if ne<complex<f128>, exceptions=observable>(read<complex<f128>, volatile>(%3), read<complex<f128>, volatile>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<complex<f128>, exceptions=observable>(read<complex<f128>, volatile>(%3), read<complex<f128>, volatile>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, add<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f128, volatile>(%2), read<complex<f128>, volatile>(%3)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%9)), const<f128>(3)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%9)), const<f128>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %11: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %12: complex<f128> [synthetic] = add<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%11), read<complex<f128>, volatile>(%5));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%12));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%9)), const<f128>(5)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%9)), const<f128>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %13: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %14: complex<f128> [synthetic] = sub<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%13), read<f128, volatile>(%2));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%14));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%9)), const<f128>(4)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%9)), const<f128>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %15: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %16: complex<f128> [synthetic] = div<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%15), add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(read<f128, volatile>(%2), read<f128, volatile>(%2)));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%16));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%9)), const<f128>(2)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%9)), const<f128>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %17: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %18: complex<f128> [synthetic] = mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%17), add<f128, rounding=nearest_even, exceptions=observable, contract=fast>(read<f128, volatile>(%2), read<f128, volatile>(%2)));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%18));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%9)), const<f128>(4)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%9)), const<f128>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %19: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %20: complex<f128> [synthetic] = sub<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%19), read<complex<f128>, volatile>(%3));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%20));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%9)), const<f128>(2)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%9)), const<f128>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %21: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %22: complex<f128> [synthetic] = mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%21), read<complex<f128>, volatile>(%9));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%22));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%9)), neg<f128>(const<f128>(5))), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%9)), const<f128>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %23: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %24: complex<f128> [synthetic] = div<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%23), read<complex<f128>, volatile>(%3));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%24));
// DEFAULT-NEXT:         let %25: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %26: complex<f128> [synthetic] = add<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%25), aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(100), index1 = const<f128>(100)));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%26));
// DEFAULT-NEXT:         let %27: complex<f128> [synthetic] = read<complex<f128>, volatile>(%9);
// DEFAULT-NEXT:         let %28: complex<f128> [synthetic] = sub<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%27), aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(100), index1 = const<f128>(100)));
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, read<complex<f128>>(%28));
// DEFAULT-NEXT:         if ne<complex<f128>, exceptions=observable>(read<complex<f128>, volatile>(%9), read<complex<f128>, volatile>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         write<complex<f128>, volatile>(%9, call<complex<f128>, signature=fn(complex<f128>) -> complex<f128>, abi=sysv64(byval<align=16>) -> sret<align=16>>(%6, read<complex<f128>, volatile>(%9)));
// DEFAULT-NEXT:         call<complex<f128>, signature=fn(complex<f128>) -> complex<f128>, abi=sysv64(byval<align=16>) -> sret<align=16>>(%6, read<complex<f128>, volatile>(%9));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f128, exceptions=observable>(read<f128, volatile>(real(%9)), const<f128>(0.5)), ne<f128, exceptions=observable>(read<f128, volatile>(imag(%9)), const<f128>(0.75)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
