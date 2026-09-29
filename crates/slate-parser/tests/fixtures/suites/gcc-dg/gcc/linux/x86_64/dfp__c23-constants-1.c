/* Test that DFP constants are accepted in C23 mode.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

int a = (int) 1.1DF;
int b = (int) 2.df;
int c = (int) .33DD;
int d = (int) 2e1dd;
int e = (int) .3e2DL;
int f = (int) 4.5e3dl;
int g = (int) 5.e0DF;
int h = (int) 1e+2df;
int i = (int) 1000e-3DL;

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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d32>(1.1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d32>(2.)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d64>(.33)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d64>(2e1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d128>(.3e2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d128>(4.5e3)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d32>(5.e0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d32>(1e+2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] = float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<d128>(1000e-3)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
