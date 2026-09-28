/* Test that imaginary constants are diagnosed in C23 mode: -pedantic.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic" } */
/* { dg-add-options float128 } */
/* { dg-require-effective-target float128 } */

_Complex _Float128 a = 1.if128;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float128 b = 2.F128j;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float128 c = 3.f128i;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float128 d = 4.JF128;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
__extension__ _Complex _Float128 e = 1.if128;
__extension__ _Complex _Float128 f = 2.F128j;
__extension__ _Complex _Float128 g = 3.f128i;
__extension__ _Complex _Float128 h = 4.JF128;

int
main ()
{
  if (a * a != -1.f128
      || b * b != -4.f128
      || c * c != -9.f128
      || d * d != -16.f128
      || e * e != -1.f128
      || f * f != -4.f128
      || g * g != -9.f128
      || h * h != -16.f128)
    __builtin_abort ();
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
// DEFAULT-NEXT:     global %0 a: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(1)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(2)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(3)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(4)) [linkage=external];
// DEFAULT-NEXT:     global %4 e: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(1)) [linkage=external];
// DEFAULT-NEXT:     global %5 f: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(2)) [linkage=external];
// DEFAULT-NEXT:     global %6 g: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(3)) [linkage=external];
// DEFAULT-NEXT:     global %7 h: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(4)) [linkage=external];
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f128>, exceptions=observable>(mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%0), read<complex<f128>>(%0)), neg<f128>(const<f128>(1))), ne<complex<f128>, exceptions=observable>(mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%1), read<complex<f128>>(%1)), neg<f128>(const<f128>(4)))), ne<complex<f128>, exceptions=observable>(mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%2), read<complex<f128>>(%2)), neg<f128>(const<f128>(9)))), ne<complex<f128>, exceptions=observable>(mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%3), read<complex<f128>>(%3)), neg<f128>(const<f128>(16)))), ne<complex<f128>, exceptions=observable>(mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%4), read<complex<f128>>(%4)), neg<f128>(const<f128>(1)))), ne<complex<f128>, exceptions=observable>(mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%5), read<complex<f128>>(%5)), neg<f128>(const<f128>(4)))), ne<complex<f128>, exceptions=observable>(mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%6), read<complex<f128>>(%6)), neg<f128>(const<f128>(9)))), ne<complex<f128>, exceptions=observable>(mul<complex<f128>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f128>>(%7), read<complex<f128>>(%7)), neg<f128>(const<f128>(16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
