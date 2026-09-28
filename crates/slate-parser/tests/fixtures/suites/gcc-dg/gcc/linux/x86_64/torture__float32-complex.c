/* Test _Float32 complex arithmetic.  */
/* { dg-do run } */
/* { dg-options "" } */
/* { dg-add-options float32 } */
/* { dg-require-effective-target float32_runtime } */

#define WIDTH 32
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
// DEFAULT-NEXT:     global %2 a: volatile f32 [storage=static] = const<f32>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %3 b: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(2.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0))) [linkage=external];
// DEFAULT-NEXT:     global %4 c: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(2.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0))) [linkage=external];
// DEFAULT-NEXT:     global %5 d: volatile complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(2.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%10 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @fn(%7 arg: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%7), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 r: volatile complex<f32> [storage=automatic];
// DEFAULT-NEXT:         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile>(%3), read<complex<f32>, volatile>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile>(%3), read<complex<f32>, volatile>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32, volatile>(%2), read<complex<f32>, volatile>(%3)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%9)), const<f32>(3.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%9)), const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %11: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %12: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%11), read<complex<f32>, volatile>(%5));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%12));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%9)), const<f32>(5.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%9)), const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %13: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %14: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%13), read<f32, volatile>(%2));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%14));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%9)), const<f32>(4.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%9)), const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %15: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %16: complex<f32> [synthetic] = div<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%15), add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32, volatile>(%2), read<f32, volatile>(%2)));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%16));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%9)), const<f32>(2.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%9)), const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %17: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %18: complex<f32> [synthetic] = mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%17), add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32, volatile>(%2), read<f32, volatile>(%2)));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%18));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%9)), const<f32>(4.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%9)), const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %19: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %20: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%19), read<complex<f32>, volatile>(%3));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%20));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%9)), const<f32>(2.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%9)), const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %21: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %22: complex<f32> [synthetic] = mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%21), read<complex<f32>, volatile>(%9));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%22));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%9)), neg<f32>(const<f32>(5.0))), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%9)), const<f32>(12.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         let %23: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %24: complex<f32> [synthetic] = div<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%23), read<complex<f32>, volatile>(%3));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%24));
// DEFAULT-NEXT:         let %25: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %26: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%25), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(100.0), index1 = const<f32>(100.0)));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%26));
// DEFAULT-NEXT:         let %27: complex<f32> [synthetic] = read<complex<f32>, volatile>(%9);
// DEFAULT-NEXT:         let %28: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%27), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(100.0), index1 = const<f32>(100.0)));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, read<complex<f32>>(%28));
// DEFAULT-NEXT:         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile>(%9), read<complex<f32>, volatile>(%3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%9, call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%6, read<complex<f32>, volatile>(%9)));
// DEFAULT-NEXT:         call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%6, read<complex<f32>, volatile>(%9));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%9)), const<f32>(0.5)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%9)), const<f32>(0.75)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
