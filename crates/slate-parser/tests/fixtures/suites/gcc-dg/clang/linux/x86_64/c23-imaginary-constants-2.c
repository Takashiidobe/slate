/* Test that imaginary constants are diagnosed in C23 mode: -pedantic-errors.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

_Complex float a = 1.if;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float b = 2.Fj;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float c = 3.fI;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex float d = 4.JF;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double e = 1.i;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double f = 2.j;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double g = 3.I;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex double h = 4.J;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double i = 1.il;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double j = 2.Lj;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double k = 3.lI;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex long double l = 4.JL;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
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
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
