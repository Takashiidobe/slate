/* Test that imaginary constants are diagnosed in C23 mode: -pedantic.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic" } */
/* { dg-add-options float16 } */
/* { dg-require-effective-target float16 } */

_Complex _Float16 a =
    1.if16; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float16 b =
    2.F16j; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float16 c =
    3.f16i; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float16 d =
    4.JF16; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
__extension__ _Complex _Float16 e = 1.if16;
__extension__ _Complex _Float16 f = 2.F16j;
__extension__ _Complex _Float16 g = 3.f16i;
__extension__ _Complex _Float16 h = 4.JF16;

int main() {
  if (a * a != -1.f16 || b * b != -4.f16 || c * c != -9.f16 ||
      d * d != -16.f16 || e * e != -1.f16 || f * f != -4.f16 ||
      g * g != -9.f16 || h * h != -16.f16)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 a: complex<f16> [storage=static] = aggregate<complex<f16>, zero_fill=false>(index0 = const<f16>(0), index1 = const<f16>(1)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: complex<f16> [storage=static] = aggregate<complex<f16>, zero_fill=false>(index0 = const<f16>(0), index1 = const<f16>(2)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: complex<f16> [storage=static] = aggregate<complex<f16>, zero_fill=false>(index0 = const<f16>(0), index1 = const<f16>(3)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: complex<f16> [storage=static] = aggregate<complex<f16>, zero_fill=false>(index0 = const<f16>(0), index1 = const<f16>(4)) [linkage=external];
// DEFAULT-NEXT:     global %4 e: complex<f16> [storage=static] = aggregate<complex<f16>, zero_fill=false>(index0 = const<f16>(0), index1 = const<f16>(1)) [linkage=external];
// DEFAULT-NEXT:     global %5 f: complex<f16> [storage=static] = aggregate<complex<f16>, zero_fill=false>(index0 = const<f16>(0), index1 = const<f16>(2)) [linkage=external];
// DEFAULT-NEXT:     global %6 g: complex<f16> [storage=static] = aggregate<complex<f16>, zero_fill=false>(index0 = const<f16>(0), index1 = const<f16>(3)) [linkage=external];
// DEFAULT-NEXT:     global %7 h: complex<f16> [storage=static] = aggregate<complex<f16>, zero_fill=false>(index0 = const<f16>(0), index1 = const<f16>(4)) [linkage=external];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f16>, exceptions=ignore>(mul<complex<f16>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f16>>(%0), read<complex<f16>>(%0)), neg<f16>(const<f16>(1))), ne<complex<f16>, exceptions=ignore>(mul<complex<f16>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f16>>(%1), read<complex<f16>>(%1)), neg<f16>(const<f16>(4)))), ne<complex<f16>, exceptions=ignore>(mul<complex<f16>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f16>>(%2), read<complex<f16>>(%2)), neg<f16>(const<f16>(9)))), ne<complex<f16>, exceptions=ignore>(mul<complex<f16>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f16>>(%3), read<complex<f16>>(%3)), neg<f16>(const<f16>(16)))), ne<complex<f16>, exceptions=ignore>(mul<complex<f16>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f16>>(%4), read<complex<f16>>(%4)), neg<f16>(const<f16>(1)))), ne<complex<f16>, exceptions=ignore>(mul<complex<f16>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f16>>(%5), read<complex<f16>>(%5)), neg<f16>(const<f16>(4)))), ne<complex<f16>, exceptions=ignore>(mul<complex<f16>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f16>>(%6), read<complex<f16>>(%6)), neg<f16>(const<f16>(9)))), ne<complex<f16>, exceptions=ignore>(mul<complex<f16>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f16>>(%7), read<complex<f16>>(%7)), neg<f16>(const<f16>(16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
