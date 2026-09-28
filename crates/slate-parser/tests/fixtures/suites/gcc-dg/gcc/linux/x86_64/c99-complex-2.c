/* Test for _Complex: in C99 only.  Test for increment and decrement.  */
/* Origin: Joseph Myers <jsm28@cam.ac.uk> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -pedantic-errors" } */

/* Use of ++ and -- on complex types (both prefix and postfix) is a
   C99 constraint violation (6.5.2.4p1, 6.5.3.1p1).
*/

_Complex double
foo (_Complex double z)
{
  z++; /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "complex" "postinc" { target *-*-* } .-1 } */
  ++z; /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "complex" "preinc" { target *-*-* } .-1 } */
  z--; /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "complex" "postdec" { target *-*-* } .-1 } */
  --z; /* { dg-bogus "warning" "warning in place of error" } */
  /* { dg-error "complex" "predec" { target *-*-* } .-1 } */
  return z;
}

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     fn %0 @foo(%1 z: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2: complex<f64> [synthetic] = read<complex<f64>>(%1);
// DEFAULT-NEXT:         let %3: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%2), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%1, read<complex<f64>>(%3));
// DEFAULT-NEXT:         let %4: complex<f64> [synthetic] = read<complex<f64>>(%1);
// DEFAULT-NEXT:         let %5: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%4), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%1, read<complex<f64>>(%5));
// DEFAULT-NEXT:         let %6: complex<f64> [synthetic] = read<complex<f64>>(%1);
// DEFAULT-NEXT:         let %7: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%6), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%1, read<complex<f64>>(%7));
// DEFAULT-NEXT:         let %8: complex<f64> [synthetic] = read<complex<f64>>(%1);
// DEFAULT-NEXT:         let %9: complex<f64> [synthetic] = sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%8), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:         write<complex<f64>>(%1, read<complex<f64>>(%9));
// DEFAULT-NEXT:         return read<complex<f64>>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
