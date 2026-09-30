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
// DEFAULT-NEXT:     fn %[[VALUE_acospi:[0-9]+]] @acospi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acospif:[0-9]+]] @acospif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acospil:[0-9]+]] @acospil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinpi:[0-9]+]] @asinpi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinpif:[0-9]+]] @asinpif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinpil:[0-9]+]] @asinpil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan2pi:[0-9]+]] @atan2pi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan2pif:[0-9]+]] @atan2pif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan2pil:[0-9]+]] @atan2pil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanpi:[0-9]+]] @atanpi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanpif:[0-9]+]] @atanpif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanpil:[0-9]+]] @atanpil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cospi:[0-9]+]] @cospi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cospif:[0-9]+]] @cospif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cospil:[0-9]+]] @cospil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10:[0-9]+]] @exp10() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10f:[0-9]+]] @exp10f() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10l:[0-9]+]] @exp10l() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_roundeven:[0-9]+]] @roundeven() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_roundevenf:[0-9]+]] @roundevenf() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_roundevenl:[0-9]+]] @roundevenl() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinpi:[0-9]+]] @sinpi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinpif:[0-9]+]] @sinpif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinpil:[0-9]+]] @sinpil() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strdup:[0-9]+]] @strdup() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strndup:[0-9]+]] @strndup() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanpi:[0-9]+]] @tanpi() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanpif:[0-9]+]] @tanpif() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanpil:[0-9]+]] @tanpil() -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
