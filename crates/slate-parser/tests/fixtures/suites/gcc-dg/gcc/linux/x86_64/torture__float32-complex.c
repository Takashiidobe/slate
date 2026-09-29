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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: volatile f32 [storage=static] = const<f32>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(2.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: volatile complex<f32> [storage=static] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(2.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: volatile complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(2.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_fn:[0-9]+]] @fn(%[[VALUE_arg:[0-9]+]] arg: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_arg]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: volatile complex<f32> [storage=automatic];
// DEFAULT-NEXT:         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile>(%[[VALUE_b]]), read<complex<f32>, volatile>(%[[VALUE_c]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile>(%[[VALUE_b]]), read<complex<f32>, volatile>(%[[VALUE_d]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32, volatile>(%[[VALUE_a]]), read<complex<f32>, volatile>(%[[VALUE_b]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%[[VALUE_r]])), const<f32>(3.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%[[VALUE_r]])), const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE1]]), read<complex<f32>, volatile>(%[[VALUE_d]]));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE2]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%[[VALUE_r]])), const<f32>(5.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%[[VALUE_r]])), const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE3]]), read<f32, volatile>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE4]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%[[VALUE_r]])), const<f32>(4.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%[[VALUE_r]])), const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: complex<f32> [synthetic] = div<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE5]]), add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32, volatile>(%[[VALUE_a]]), read<f32, volatile>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE6]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%[[VALUE_r]])), const<f32>(2.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%[[VALUE_r]])), const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: complex<f32> [synthetic] = mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE7]]), add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32, volatile>(%[[VALUE_a]]), read<f32, volatile>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE8]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%[[VALUE_r]])), const<f32>(4.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%[[VALUE_r]])), const<f32>(6.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE9]]), read<complex<f32>, volatile>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE10]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%[[VALUE_r]])), const<f32>(2.0)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%[[VALUE_r]])), const<f32>(3.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: complex<f32> [synthetic] = mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE11]]), read<complex<f32>, volatile>(%[[VALUE_r]]));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE12]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%[[VALUE_r]])), neg<f32>(const<f32>(5.0))), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%[[VALUE_r]])), const<f32>(12.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: complex<f32> [synthetic] = div<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE13]]), read<complex<f32>, volatile>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE15]]), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(100.0), index1 = const<f32>(100.0)));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE16]]));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>, volatile>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE17]]), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(100.0), index1 = const<f32>(100.0)));
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], read<complex<f32>>(%[[VALUE18]]));
// DEFAULT-NEXT:         if ne<complex<f32>, exceptions=observable>(read<complex<f32>, volatile>(%[[VALUE_r]]), read<complex<f32>, volatile>(%[[VALUE_b]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_r]], call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%[[VALUE_fn]], read<complex<f32>, volatile>(%[[VALUE_r]])));
// DEFAULT-NEXT:         call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%[[VALUE_fn]], read<complex<f32>, volatile>(%[[VALUE_r]]));
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32, volatile>(real(%[[VALUE_r]])), const<f32>(0.5)), ne<f32, exceptions=observable>(read<f32, volatile>(imag(%[[VALUE_r]])), const<f32>(0.75)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
