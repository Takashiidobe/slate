/* { dg-do compile } */
/* { dg-options "-std=c99 -pedantic-errors" } */

double a = 1.d;		/* { dg-error "double constant" } */
double b = 1.D;		/* { dg-error "double constant" } */
double c = 1.1e+2d;	/* { dg-error "double constant" } */

double d = 1.di;	/* { dg-error "imaginary constants" } */
double e = 1.dj;	/* { dg-error "imaginary constants" } */
double f = 1.id;	/* { dg-error "imaginary constants" } */
double g = 1.jd;	/* { dg-error "imaginary constants" } */

double h = 0x1.5p1d;	/* { dg-error "double constant" } */

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: f64 [storage=static] = const<f64>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: f64 [storage=static] = const<f64>(1.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: f64 [storage=static] = const<f64>(110.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: f64 [storage=static] = const<f64>(2.625) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
