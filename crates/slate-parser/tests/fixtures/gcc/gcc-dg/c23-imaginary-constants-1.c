/* Test that imaginary constants are diagnosed in C23 mode: -pedantic.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic" } */

// SLATE-FILECHECK-FLAVOR gcc

_Complex float a =
    1.if; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float b =
    2.Fj; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float c =
    3.fI; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float d =
    4.JF; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double e =
    1.i; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double f =
    2.j; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double g =
    3.I; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double h =
    4.J; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double i =
    1.il; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double j =
    2.Lj; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double k =
    3.lI; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double l =
    4.JL; /* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
__extension__ _Complex float       m = 1.if;
__extension__ _Complex float       n = 2.Fj;
__extension__ _Complex float       o = 3.fI;
__extension__ _Complex float       p = 4.JF;
__extension__ _Complex double      q = 1.i;
__extension__ _Complex double      r = 2.j;
__extension__ _Complex double      s = 3.I;
__extension__ _Complex double      t = 4.J;
__extension__ _Complex long double u = 1.il;
__extension__ _Complex long double v = 2.Lj;
__extension__ _Complex long double w = 3.lI;
__extension__ _Complex long double x = 4.JL;

int main() {
  if (a * a != -1.f || b * b != -4.f || c * c != -9.f || d * d != -16.f ||
      e * e != -1. || f * f != -4. || g * g != -9. || h * h != -16. ||
      i * i != -1.L || j * j != -4.L || k * k != -9.L || l * l != -16.L ||
      m * m != -1.f || n * n != -4.f || o * o != -9.f || p * p != -16.f ||
      q * q != -1. || r * r != -4. || s * s != -9. || t * t != -16. ||
      u * u != -1.L || v * v != -4.L || w * w != -9.L || x * x != -16.L)
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
// DEFAULT-NEXT:     global %0 a: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %4 e: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %5 f: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %6 g: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %7 h: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %8 i: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1)) [linkage=external];
// DEFAULT-NEXT:     global %9 j: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2)) [linkage=external];
// DEFAULT-NEXT:     global %10 k: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(3)) [linkage=external];
// DEFAULT-NEXT:     global %11 l: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4)) [linkage=external];
// DEFAULT-NEXT:     global %12 m: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %13 n: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %14 o: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %15 p: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %16 q: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %17 r: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %18 s: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %19 t: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %20 u: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1)) [linkage=external];
// DEFAULT-NEXT:     global %21 v: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2)) [linkage=external];
// DEFAULT-NEXT:     global %22 w: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(3)) [linkage=external];
// DEFAULT-NEXT:     global %23 x: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4)) [linkage=external];
// DEFAULT-NEXT:     fn %25 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%0), read<complex<f32>>(%0)), neg<f32>(const<f32>(1.0))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%1), read<complex<f32>>(%1)), neg<f32>(const<f32>(4.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%2), read<complex<f32>>(%2)), neg<f32>(const<f32>(9.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%3), read<complex<f32>>(%3)), neg<f32>(const<f32>(16.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%4), read<complex<f64>>(%4)), neg<f64>(const<f64>(1.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%5), read<complex<f64>>(%5)), neg<f64>(const<f64>(4.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%6), read<complex<f64>>(%6)), neg<f64>(const<f64>(9.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%7), read<complex<f64>>(%7)), neg<f64>(const<f64>(16.0)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%8), read<complex<f80>>(%8)), neg<f80>(const<f80>(1)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%9), read<complex<f80>>(%9)), neg<f80>(const<f80>(4)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%10), read<complex<f80>>(%10)), neg<f80>(const<f80>(9)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%11), read<complex<f80>>(%11)), neg<f80>(const<f80>(16)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%12), read<complex<f32>>(%12)), neg<f32>(const<f32>(1.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%13), read<complex<f32>>(%13)), neg<f32>(const<f32>(4.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%14), read<complex<f32>>(%14)), neg<f32>(const<f32>(9.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%15), read<complex<f32>>(%15)), neg<f32>(const<f32>(16.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%16), read<complex<f64>>(%16)), neg<f64>(const<f64>(1.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%17), read<complex<f64>>(%17)), neg<f64>(const<f64>(4.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%18), read<complex<f64>>(%18)), neg<f64>(const<f64>(9.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%19), read<complex<f64>>(%19)), neg<f64>(const<f64>(16.0)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%20), read<complex<f80>>(%20)), neg<f80>(const<f80>(1)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%21), read<complex<f80>>(%21)), neg<f80>(const<f80>(4)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%22), read<complex<f80>>(%22)), neg<f80>(const<f80>(9)))), ne<complex<f80>, exceptions=observable>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%23), read<complex<f80>>(%23)), neg<f80>(const<f80>(16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
