/* Test that imaginary constants are diagnosed in C23 mode: -pedantic-errors.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-add-options float32 } */
/* { dg-add-options float64 } */
/* { dg-add-options float32x } */
/* { dg-require-effective-target float32 } */
/* { dg-require-effective-target float32x } */
/* { dg-require-effective-target float64 } */

_Complex _Float32 a = 1.if32;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32 b = 2.F32j;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32 c = 3.f32i;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32 d = 4.JF32;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64 e = 1.if64;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64 f = 2.F64j;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64 g = 3.f64i;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64 h = 4.JF64;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32x i = 1.if32x;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32x j = 2.F32xj;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32x k = 3.f32xI;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float32x l = 4.JF32x;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
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
// DEFAULT-NEXT:     global %0 a: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %4 e: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %5 f: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %6 g: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %7 h: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %8 i: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %9 j: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %10 k: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %11 l: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %12 m: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %13 n: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %14 o: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %15 p: complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %16 q: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %17 r: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %18 s: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %19 t: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT:     global %20 u: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)) [linkage=external];
// DEFAULT-NEXT:     global %21 v: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0)) [linkage=external];
// DEFAULT-NEXT:     global %22 w: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %23 x: complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
