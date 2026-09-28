/* Test C2Y complex increment and decrement: disallowed for C23.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

_Complex float a;

void
f (void)
{
  a++; /* { dg-error "does not support" } */
  ++a; /* { dg-error "does not support" } */
  a--; /* { dg-error "does not support" } */
  --a; /* { dg-error "does not support" } */
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
// DEFAULT-NEXT:     global %0 a: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2: complex<f32> [synthetic] = read<complex<f32>>(%0);
// DEFAULT-NEXT:         let %3: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%2), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%0, read<complex<f32>>(%3));
// DEFAULT-NEXT:         let %4: complex<f32> [synthetic] = read<complex<f32>>(%0);
// DEFAULT-NEXT:         let %5: complex<f32> [synthetic] = add<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%4), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%0, read<complex<f32>>(%5));
// DEFAULT-NEXT:         let %6: complex<f32> [synthetic] = read<complex<f32>>(%0);
// DEFAULT-NEXT:         let %7: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%6), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%0, read<complex<f32>>(%7));
// DEFAULT-NEXT:         let %8: complex<f32> [synthetic] = read<complex<f32>>(%0);
// DEFAULT-NEXT:         let %9: complex<f32> [synthetic] = sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%8), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f32>>(%0, read<complex<f32>>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
