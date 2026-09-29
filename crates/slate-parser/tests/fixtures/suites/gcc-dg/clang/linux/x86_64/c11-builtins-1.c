/* Test C11 built-in functions: test functions new in C23 are not
   declared as built-in for C11.  */
/* { dg-do compile } */
/* { dg-options "-std=c11" } */

int exp10 (void);
int exp10f (void);
int exp10l (void);
int fabsd32 (void);
int fabsd64 (void);
int fabsd128 (void);
int nand32 (void);
int nand64 (void);
int nand128 (void);
int roundeven (void);
int roundevenf (void);
int roundevenl (void);
int strdup (void);
int strndup (void);

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     fn %[[VALUE_exp10:[0-9]+]] @exp10() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10f:[0-9]+]] @exp10f() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10l:[0-9]+]] @exp10l() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabsd32:[0-9]+]] @fabsd32() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabsd64:[0-9]+]] @fabsd64() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabsd128:[0-9]+]] @fabsd128() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nand32:[0-9]+]] @nand32() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nand64:[0-9]+]] @nand64() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nand128:[0-9]+]] @nand128() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_roundeven:[0-9]+]] @roundeven() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_roundevenf:[0-9]+]] @roundevenf() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_roundevenl:[0-9]+]] @roundevenl() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strdup:[0-9]+]] @strdup() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strndup:[0-9]+]] @strndup() -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
