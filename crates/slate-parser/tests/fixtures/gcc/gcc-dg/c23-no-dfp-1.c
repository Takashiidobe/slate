/* Test DFP types and constants rejected if no DFP support.  Bug
   91985.  */
/* { dg-do compile { target { ! dfp } } } */
/* { dg-options "-std=c23" } */

_Decimal32 d32a; /* { dg-error "not supported" } */
_Decimal64 d64a; /* { dg-error "not supported" } */
_Decimal128 d128a; /* { dg-error "not supported" } */

_Bool d32b = 1.0DF; /* { dg-error "not supported" } */
_Bool d64b = 1.0DD; /* { dg-error "not supported" } */
_Bool d128b = 1.0DL; /* { dg-error "not supported" } */

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %0 d32a: d32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 d64a: d64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 d128a: d128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 d32b: bool [storage=static] = ne<d32, reason=assign, exceptions=ignore>(const<d32>(1.0), const<d32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %4 d64b: bool [storage=static] = ne<d64, reason=assign, exceptions=ignore>(const<d64>(1.0), const<d64>(0)) [linkage=external];
// DEFAULT-NEXT:     global %5 d128b: bool [storage=static] = ne<d128, reason=assign, exceptions=ignore>(const<d128>(1.0), const<d128>(0)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
