/* Test C23 built-in functions: test functions new in C23 are indeed
   declared as built-in as expected.  Non-DFP tests.  */
/* { dg-do compile } */
/* { dg-options "-std=c23" } */

/* Keep this list sorted alphabetically by function name.  */
int acospi (void); /* { dg-warning "conflicting types for built-in function" } */
int acospif (void); /* { dg-warning "conflicting types for built-in function" } */
int acospil (void); /* { dg-warning "conflicting types for built-in function" } */
int asinpi (void); /* { dg-warning "conflicting types for built-in function" } */
int asinpif (void); /* { dg-warning "conflicting types for built-in function" } */
int asinpil (void); /* { dg-warning "conflicting types for built-in function" } */
int atan2pi (void); /* { dg-warning "conflicting types for built-in function" } */
int atan2pif (void); /* { dg-warning "conflicting types for built-in function" } */
int atan2pil (void); /* { dg-warning "conflicting types for built-in function" } */
int atanpi (void); /* { dg-warning "conflicting types for built-in function" } */
int atanpif (void); /* { dg-warning "conflicting types for built-in function" } */
int atanpil (void); /* { dg-warning "conflicting types for built-in function" } */
int cospi (void); /* { dg-warning "conflicting types for built-in function" } */
int cospif (void); /* { dg-warning "conflicting types for built-in function" } */
int cospil (void); /* { dg-warning "conflicting types for built-in function" } */
int exp10 (void); /* { dg-warning "conflicting types for built-in function" } */
int exp10f (void); /* { dg-warning "conflicting types for built-in function" } */
int exp10l (void); /* { dg-warning "conflicting types for built-in function" } */
int roundeven (void); /* { dg-warning "conflicting types for built-in function" } */
int roundevenf (void); /* { dg-warning "conflicting types for built-in function" } */
int roundevenl (void); /* { dg-warning "conflicting types for built-in function" } */
int sinpi (void); /* { dg-warning "conflicting types for built-in function" } */
int sinpif (void); /* { dg-warning "conflicting types for built-in function" } */
int sinpil (void); /* { dg-warning "conflicting types for built-in function" } */
int strdup (void); /* { dg-warning "conflicting types for built-in function" } */
int strndup (void); /* { dg-warning "conflicting types for built-in function" } */
int tanpi (void); /* { dg-warning "conflicting types for built-in function" } */
int tanpif (void); /* { dg-warning "conflicting types for built-in function" } */
int tanpil (void); /* { dg-warning "conflicting types for built-in function" } */

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
// DEFAULT-NEXT:     fn %0 @acospi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @acospif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @acospil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @asinpi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @asinpif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @asinpil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @atan2pi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @atan2pif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @atan2pil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @atanpi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %10 @atanpif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @atanpil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @cospi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %13 @cospif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %14 @cospil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @exp10() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %16 @exp10f() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @exp10l() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %18 @roundeven() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @roundevenf() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @roundevenl() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @sinpi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %22 @sinpif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @sinpil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %24 @strdup() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @strndup() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @tanpi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %27 @tanpif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %28 @tanpil() -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
