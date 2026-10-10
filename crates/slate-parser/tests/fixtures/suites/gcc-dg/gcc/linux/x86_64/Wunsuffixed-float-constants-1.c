/* { dg-do compile } */
/* { dg-options "-std=gnu99 -Wunsuffixed-float-constants" } */

#define VAL 0.5;

double a = 1.1d;

/* With FLOAT_CONST_DECIMAL64 switched to ON these would have type
   _Decimal64.  */

double b = VAL;		/* { dg-warning "unsuffixed floating constant" } */
double c = 1.2;		/* { dg-warning "unsuffixed floating constant" } */

/* With FLOAT_CONST_DECIMAL64 switched to ON these are still binary.  */

double d = 0x5.0p1;	/* No warning for hex constant.  */
double e = 3.1i;	/* No warning for imaginary constant.  */

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: f64 [storage=static] = const<f64>(1.1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: f64 [storage=static] = const<f64>(0.5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: f64 [storage=static] = const<f64>(1.2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] = const<f64>(10.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: f64 [storage=static] = complex_to_real<f64, reason=assign>(aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.1))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
