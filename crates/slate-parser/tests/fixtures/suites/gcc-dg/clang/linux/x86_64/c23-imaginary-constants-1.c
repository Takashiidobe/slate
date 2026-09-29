/* Test that imaginary constants are diagnosed in C23 mode: -pedantic.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic" } */

_Complex float a = 1.if;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float b = 2.Fj;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float c = 3.fI;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float d = 4.JF;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double e = 1.i;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double f = 2.j;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double g = 3.I;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double h = 4.J;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double i = 1.il;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double j = 2.Lj;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double k = 3.lI;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double l = 4.JL;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
__extension__ _Complex float m = 1.if;
__extension__ _Complex float n = 2.Fj;
__extension__ _Complex float o = 3.fI;
__extension__ _Complex float p = 4.JF;
__extension__ _Complex double q = 1.i;
__extension__ _Complex double r = 2.j;
__extension__ _Complex double s = 3.I;
__extension__ _Complex double t = 4.J;
__extension__ _Complex long double u = 1.il;
__extension__ _Complex long double v = 2.Lj;
__extension__ _Complex long double w = 3.lI;
__extension__ _Complex long double x = 4.JL;

int
main ()
{
  if (a * a != -1.f
      || b * b != -4.f
      || c * c != -9.f
      || d * d != -16.f
      || e * e != -1.
      || f * f != -4.
      || g * g != -9.
      || h * h != -16.
      || i * i != -1.L
      || j * j != -4.L
      || k * k != -9.L
      || l * l != -16.L
      || m * m != -1.f
      || n * n != -4.f
      || o * o != -9.f
      || p * p != -16.f
      || q * q != -1.
      || r * r != -4.
      || s * s != -9.
      || t * t != -16.
      || u * u != -1.L
      || v * v != -4.L
      || w * w != -9.L
      || x * x != -16.L)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=ignore>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_a]]), read<complex<f32>>(%[[VALUE_a]])), neg<f32>(const<f32>(1.0))), ne<complex<f32>, exceptions=ignore>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_b]]), read<complex<f32>>(%[[VALUE_b]])), neg<f32>(const<f32>(4.0)))), ne<complex<f32>, exceptions=ignore>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_c]]), read<complex<f32>>(%[[VALUE_c]])), neg<f32>(const<f32>(9.0)))), ne<complex<f32>, exceptions=ignore>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_d]]), read<complex<f32>>(%[[VALUE_d]])), neg<f32>(const<f32>(16.0)))), ne<complex<f64>, exceptions=ignore>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_e]]), read<complex<f64>>(%[[VALUE_e]])), neg<f64>(const<f64>(1.0)))), ne<complex<f64>, exceptions=ignore>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_f]]), read<complex<f64>>(%[[VALUE_f]])), neg<f64>(const<f64>(4.0)))), ne<complex<f64>, exceptions=ignore>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_g]]), read<complex<f64>>(%[[VALUE_g]])), neg<f64>(const<f64>(9.0)))), ne<complex<f64>, exceptions=ignore>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_h]]), read<complex<f64>>(%[[VALUE_h]])), neg<f64>(const<f64>(16.0)))), ne<complex<f80>, exceptions=ignore>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_i]]), read<complex<f80>>(%[[VALUE_i]])), neg<f80>(const<f80>(1)))), ne<complex<f80>, exceptions=ignore>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_j]]), read<complex<f80>>(%[[VALUE_j]])), neg<f80>(const<f80>(4)))), ne<complex<f80>, exceptions=ignore>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_k]]), read<complex<f80>>(%[[VALUE_k]])), neg<f80>(const<f80>(9)))), ne<complex<f80>, exceptions=ignore>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_l]]), read<complex<f80>>(%[[VALUE_l]])), neg<f80>(const<f80>(16)))), ne<complex<f32>, exceptions=ignore>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_m]]), read<complex<f32>>(%[[VALUE_m]])), neg<f32>(const<f32>(1.0)))), ne<complex<f32>, exceptions=ignore>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_n]]), read<complex<f32>>(%[[VALUE_n]])), neg<f32>(const<f32>(4.0)))), ne<complex<f32>, exceptions=ignore>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_o]]), read<complex<f32>>(%[[VALUE_o]])), neg<f32>(const<f32>(9.0)))), ne<complex<f32>, exceptions=ignore>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f32>>(%[[VALUE_p]]), read<complex<f32>>(%[[VALUE_p]])), neg<f32>(const<f32>(16.0)))), ne<complex<f64>, exceptions=ignore>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_q]]), read<complex<f64>>(%[[VALUE_q]])), neg<f64>(const<f64>(1.0)))), ne<complex<f64>, exceptions=ignore>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_r]]), read<complex<f64>>(%[[VALUE_r]])), neg<f64>(const<f64>(4.0)))), ne<complex<f64>, exceptions=ignore>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_s]]), read<complex<f64>>(%[[VALUE_s]])), neg<f64>(const<f64>(9.0)))), ne<complex<f64>, exceptions=ignore>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_t]]), read<complex<f64>>(%[[VALUE_t]])), neg<f64>(const<f64>(16.0)))), ne<complex<f80>, exceptions=ignore>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_u]]), read<complex<f80>>(%[[VALUE_u]])), neg<f80>(const<f80>(1)))), ne<complex<f80>, exceptions=ignore>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_v]]), read<complex<f80>>(%[[VALUE_v]])), neg<f80>(const<f80>(4)))), ne<complex<f80>, exceptions=ignore>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_w]]), read<complex<f80>>(%[[VALUE_w]])), neg<f80>(const<f80>(9)))), ne<complex<f80>, exceptions=ignore>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f80>>(%[[VALUE_x]]), read<complex<f80>>(%[[VALUE_x]])), neg<f80>(const<f80>(16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
