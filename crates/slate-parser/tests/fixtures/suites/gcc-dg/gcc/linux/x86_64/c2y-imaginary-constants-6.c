/* Test that imaginary constants are accepted in C2Y mode: compat warnings.  */
/* { dg-do compile } */
/* { dg-options "-std=c2y -Wc23-c2y-compat" } */
/* { dg-add-options float128 } */
/* { dg-require-effective-target float128 } */

_Complex _Float128 a = 1.if128;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float128 b = 2.F128j;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float128 c = 3.f128i;	/* { dg-warning "imaginary constants are a C2Y feature" } */
_Complex _Float128 d = 4.JF128;	/* { dg-warning "imaginary constants are a C2Y feature" } */
__extension__ _Complex _Float128 e = 1.if128;
__extension__ _Complex _Float128 f = 2.F128j;
__extension__ _Complex _Float128 g = 3.f128i;
__extension__ _Complex _Float128 h = 4.JF128;

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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(4)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: complex<f128> [storage=static] = aggregate<complex<f128>, zero_fill=false>(index0 = const<f128>(0), index1 = const<f128>(4)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
