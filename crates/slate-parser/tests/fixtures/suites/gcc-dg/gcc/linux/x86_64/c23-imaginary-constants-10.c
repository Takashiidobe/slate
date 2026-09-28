/* Test that imaginary constants are diagnosed in C23 mode: -pedantic-errors.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-add-options float64x } */
/* { dg-require-effective-target float64x } */

_Complex _Float64x a = 1.if64x;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64x b = 2.F64xj;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64x c = 3.f64xi;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
_Complex _Float64x d = 4.JF64x;	/* { dg-error "imaginary constants are a C2Y feature or GCC extension" } */
__extension__ _Complex _Float64x e = 1.if64x;
__extension__ _Complex _Float64x f = 2.F64xj;
__extension__ _Complex _Float64x g = 3.f64xi;
__extension__ _Complex _Float64x h = 4.JF64x;

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
// DEFAULT-NEXT:     global %0 a: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2)) [linkage=external];
// DEFAULT-NEXT:     global %2 c: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(3)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4)) [linkage=external];
// DEFAULT-NEXT:     global %4 e: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(1)) [linkage=external];
// DEFAULT-NEXT:     global %5 f: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(2)) [linkage=external];
// DEFAULT-NEXT:     global %6 g: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(3)) [linkage=external];
// DEFAULT-NEXT:     global %7 h: complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = const<f80>(0), index1 = const<f80>(4)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
