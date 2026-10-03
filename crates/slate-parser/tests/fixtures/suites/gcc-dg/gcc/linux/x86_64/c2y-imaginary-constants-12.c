/* Test that integral imaginary constants are diagnosed in C2Y mode: -pedantic-errors.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu2y -pedantic-errors" } */

_Complex float a = 1i;		/* { dg-error "imaginary constants are a GCC extension" } */
_Complex float b = 2j;		/* { dg-error "imaginary constants are a GCC extension" } */
_Complex float c = 3I;		/* { dg-error "imaginary constants are a GCC extension" } */
_Complex float d = 4J;		/* { dg-error "imaginary constants are a GCC extension" } */
_Complex double e = 1il;	/* { dg-error "imaginary constants are a GCC extension" } */
_Complex double f = 2Lj;	/* { dg-error "imaginary constants are a GCC extension" } */
_Complex double g = 3lI;	/* { dg-error "imaginary constants are a GCC extension" } */
_Complex double h = 4JL;	/* { dg-error "imaginary constants are a GCC extension" } */
_Complex long double i = 1ill;	/* { dg-error "imaginary constants are a GCC extension" } */
_Complex long double j = 2LLj;	/* { dg-error "imaginary constants are a GCC extension" } */
_Complex long double k = 3llI;	/* { dg-error "imaginary constants are a GCC extension" } */
_Complex long double l = 4JLL;	/* { dg-error "imaginary constants are a GCC extension" } */
__extension__ _Complex float m = 1i;
__extension__ _Complex float n = 2j;
__extension__ _Complex float o = 3I;
__extension__ _Complex float p = 4J;
__extension__ _Complex double q = 1il;
__extension__ _Complex double r = 2Lj;
__extension__ _Complex double s = 3lI;
__extension__ _Complex double t = 4JL;
__extension__ _Complex long double u = 1ill;
__extension__ _Complex long double v = 2LLj;
__extension__ _Complex long double w = 3llI;
__extension__ _Complex long double x = 4JLL;

// SLATE-FILECHECK-STD DEFAULT gnu2y
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: complex<f64> [storage=static] = complex_convert<complex<f64>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: complex<f64> [storage=static] = complex_convert<complex<f64>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: complex<f64> [storage=static] = complex_convert<complex<f64>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: complex<f64> [storage=static] = complex_convert<complex<f64>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: complex<f80> [storage=static] = complex_convert<complex<f80>, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: complex<f80> [storage=static] = complex_convert<complex<f80>, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: complex<f80> [storage=static] = complex_convert<complex<f80>, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: complex<f80> [storage=static] = complex_convert<complex<f80>, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_m:[0-9]+]] m: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_n:[0-9]+]] n: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_o:[0-9]+]] o: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: complex<f32> [storage=static] = complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: complex<f64> [storage=static] = complex_convert<complex<f64>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_r:[0-9]+]] r: complex<f64> [storage=static] = complex_convert<complex<f64>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: complex<f64> [storage=static] = complex_convert<complex<f64>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: complex<f64> [storage=static] = complex_convert<complex<f64>, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_u:[0-9]+]] u: complex<f80> [storage=static] = complex_convert<complex<f80>, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: complex<f80> [storage=static] = complex_convert<complex<f80>, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(2))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_w:[0-9]+]] w: complex<f80> [storage=static] = complex_convert<complex<f80>, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: complex<f80> [storage=static] = complex_convert<complex<f80>, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(aggregate<complex<i64>, zero_fill=false>(index0 = const<i64>(0), index1 = const<i64>(4))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
