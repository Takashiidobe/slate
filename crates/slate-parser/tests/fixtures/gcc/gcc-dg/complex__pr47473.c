/* PR c/47473 */
/* { dg-do run } */
/* { dg-options "-std=c99" } */

int main(void) {
  long double _Complex w = 0.2L - 0.3iL;
  w                      = w * (0.3L - (0.0F + 1.0iF) * 0.9L);
  if (__builtin_fabsl(__real__ w + 0.21L) > 0.001L ||
      __builtin_fabsl(__imag__ w + 0.27L) > 0.001L)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %3 @__builtin_fabsl(%2 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %4 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %1 w: complex<f80> [storage=automatic] = sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(0.200000000000000000003), aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(0.300000000000000000011)));
// DEFAULT-NEXT:         write<complex<f80>>(%1, mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%1), sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f80>(0.300000000000000000011), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(complex_convert<complex<f80>, reason=usual_arith>(add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f32>(0.0), aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))), const<f80>(0.899999999999999999978)))));
// DEFAULT-NEXT:         if logical_or<bool>(gt<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%3, add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(real(%1)), const<f80>(0.209999999999999999994))), const<f80>(9.99999999999999999958E-4)), gt<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%3, add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(imag(%1)), const<f80>(0.27000000000000000001))), const<f80>(9.99999999999999999958E-4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
