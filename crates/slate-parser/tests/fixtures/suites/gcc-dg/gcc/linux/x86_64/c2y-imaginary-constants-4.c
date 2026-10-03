/* Test that imaginary constants are accepted in C2Y mode: compat warnings.  */
/* { dg-do compile } */
/* { dg-options "-std=c2y -Wc23-c2y-compat" } */
/* { dg-add-options float32 } */
/* { dg-add-options float64 } */
/* { dg-add-options float32x } */
/* { dg-require-effective-target float32 } */
/* { dg-require-effective-target float32x } */
/* { dg-require-effective-target float64 } */

_Complex _Float32 a = 1.if32;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float32 b = 2.F32j;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float32 c = 3.f32i;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float32 d = 4.JF32;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float64 e = 1.if64;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float64 f = 2.F64j;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float64 g = 3.f64i;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float64 h = 4.JF64;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float32x i = 1.if32x;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float32x j = 2.F32xj;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float32x k = 3.f32xI;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float32x l = 4.JF32x;	/* { dg-warning "imaginary constants are a C2Y feature" } */
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

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
