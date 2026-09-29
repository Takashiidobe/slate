/* Test C2Y complex increment and decrement: allowed for C23 with
   -Wno-c23-c2y-compat.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors -Wno-c23-c2y-compat" } */

_Complex float a;

void
f (void)
{
  a++;
  ++a;
  a--;
  --a;
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE0]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE2]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE4]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: complex<f32> [synthetic] = read<complex<f32>>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE6]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_a]], read<complex<f32>>(%[[VALUE7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
