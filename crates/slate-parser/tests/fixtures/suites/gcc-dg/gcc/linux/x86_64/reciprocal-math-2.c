/* Test __RECIPROCAL_MATH__ is not defined with -fno-reciprocal-math.  */
/* { dg-do compile } */
/* { dg-options "-fno-reciprocal-math" } */

#ifdef __RECIPROCAL_MATH__
#error "__RECIPROCAL_MATH__ defined"
#endif

#pragma GCC optimize "-freciprocal-math"
#ifndef __RECIPROCAL_MATH__
#error "__RECIPROCAL_MATH__ not defined"
#endif

#pragma GCC optimize "-fno-reciprocal-math"
#ifdef __RECIPROCAL_MATH__
#error "__RECIPROCAL_MATH__ defined"
#endif

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fno-reciprocal-math
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
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
