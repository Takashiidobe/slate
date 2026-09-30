/* Test C23 built-in functions: test functions new in C23 are indeed
   declared as built-in as expected.  DFP tests.  */
/* { dg-do compile } */
/* { dg-options "-std=c23" } */

int fabsd32 (void); /* { dg-warning "conflicting types for built-in function" } */
int fabsd64 (void); /* { dg-warning "conflicting types for built-in function" } */
int fabsd128 (void); /* { dg-warning "conflicting types for built-in function" } */
int nand32 (void); /* { dg-warning "conflicting types for built-in function" } */
int nand64 (void); /* { dg-warning "conflicting types for built-in function" } */
int nand128 (void); /* { dg-warning "conflicting types for built-in function" } */

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
// DEFAULT-NEXT:     fn %[[VALUE_fabsd32:[0-9]+]] @fabsd32() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabsd64:[0-9]+]] @fabsd64() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabsd128:[0-9]+]] @fabsd128() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nand32:[0-9]+]] @nand32() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nand64:[0-9]+]] @nand64() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nand128:[0-9]+]] @nand128() -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
