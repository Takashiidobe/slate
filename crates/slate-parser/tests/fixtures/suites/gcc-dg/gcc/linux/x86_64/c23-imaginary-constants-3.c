/* Test that imaginary constants are diagnosed in C23 mode: -pedantic.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic" } */
/* { dg-add-options float32 } */
/* { dg-add-options float64 } */
/* { dg-add-options float32x } */
/* { dg-require-effective-target float32 } */
/* { dg-require-effective-target float32x } */
/* { dg-require-effective-target float64 } */

_Complex _Float32 a = 1.if32;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32 b = 2.F32j;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32 c = 3.f32i;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32 d = 4.JF32;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64 e = 1.if64;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64 f = 2.F64j;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64 g = 3.f64i;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64 h = 4.JF64;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32x i = 1.if32x;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32x j = 2.F32xj;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32x k = 3.f32xI;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32x l = 4.JF32x;	/* { dg-warning "imaginary constants are a C2Y feature or GCC extension" } */
__extension__ _Complex _Float32 m = 1.if32;
__extension__ _Complex _Float32 n = 2.F32j;
__extension__ _Complex _Float32 o = 3.f32i;
__extension__ _Complex _Float32 p = 4.JF32;
__extension__ _Complex _Float64 q = 1.if64;
__extension__ _Complex _Float64 r = 2.F64j;
__extension__ _Complex _Float64 s = 3.f64i;
__extension__ _Complex _Float64 t = 4.JF64;
__extension__ _Complex _Float32x u = 1.if32x;
__extension__ _Complex _Float32x v = 2.F32xj;
__extension__ _Complex _Float32x w = 3.f32xI;
__extension__ _Complex _Float32x x = 4.JF32x;

int
main ()
{
  if (a * a != -1.f32
      || b * b != -4.f32
      || c * c != -9.f32
      || d * d != -16.f32
      || e * e != -1.f64
      || f * f != -4.f64
      || g * g != -9.f64
      || h * h != -16.f64
      || i * i != -1.f32x
      || j * j != -4.f32x
      || k * k != -9.f32x
      || l * l != -16.f32x
      || m * m != -1.f32
      || n * n != -4.f32
      || o * o != -9.f32
      || p * p != -16.f32
      || q * q != -1.f64
      || r * r != -4.f64
      || s * s != -9.f64
      || t * t != -16.f64
      || u * u != -1.f32x
      || v * v != -4.f32x
      || w * w != -9.f32x
      || x * x != -16.f32x)
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
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_a]]), read<complex<f32>>(%[[VALUE_a]])), neg<f32>(const<f32>(1.0))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_b]]), read<complex<f32>>(%[[VALUE_b]])), neg<f32>(const<f32>(4.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_c]]), read<complex<f32>>(%[[VALUE_c]])), neg<f32>(const<f32>(9.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_d]]), read<complex<f32>>(%[[VALUE_d]])), neg<f32>(const<f32>(16.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_e]]), read<complex<f64>>(%[[VALUE_e]])), neg<f64>(const<f64>(1.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_f]]), read<complex<f64>>(%[[VALUE_f]])), neg<f64>(const<f64>(4.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_g]]), read<complex<f64>>(%[[VALUE_g]])), neg<f64>(const<f64>(9.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_h]]), read<complex<f64>>(%[[VALUE_h]])), neg<f64>(const<f64>(16.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_i]]), read<complex<f64>>(%[[VALUE_i]])), neg<f64>(const<f64>(1.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_j]]), read<complex<f64>>(%[[VALUE_j]])), neg<f64>(const<f64>(4.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_k]]), read<complex<f64>>(%[[VALUE_k]])), neg<f64>(const<f64>(9.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_l]]), read<complex<f64>>(%[[VALUE_l]])), neg<f64>(const<f64>(16.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_m]]), read<complex<f32>>(%[[VALUE_m]])), neg<f32>(const<f32>(1.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_n]]), read<complex<f32>>(%[[VALUE_n]])), neg<f32>(const<f32>(4.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_o]]), read<complex<f32>>(%[[VALUE_o]])), neg<f32>(const<f32>(9.0)))), ne<complex<f32>, exceptions=observable>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_p]]), read<complex<f32>>(%[[VALUE_p]])), neg<f32>(const<f32>(16.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_q]]), read<complex<f64>>(%[[VALUE_q]])), neg<f64>(const<f64>(1.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_r]]), read<complex<f64>>(%[[VALUE_r]])), neg<f64>(const<f64>(4.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_s]]), read<complex<f64>>(%[[VALUE_s]])), neg<f64>(const<f64>(9.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_t]]), read<complex<f64>>(%[[VALUE_t]])), neg<f64>(const<f64>(16.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_u]]), read<complex<f64>>(%[[VALUE_u]])), neg<f64>(const<f64>(1.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_v]]), read<complex<f64>>(%[[VALUE_v]])), neg<f64>(const<f64>(4.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_w]]), read<complex<f64>>(%[[VALUE_w]])), neg<f64>(const<f64>(9.0)))), ne<complex<f64>, exceptions=observable>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_x]]), read<complex<f64>>(%[[VALUE_x]])), neg<f64>(const<f64>(16.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
