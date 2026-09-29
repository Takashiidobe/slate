/* Verify diagnostics for aliases to strings containing extended
   identifiers or bad characters.  */
/* { dg-do compile } */
/* { dg-options "-std=gnu99 -w" } */
/* { dg-require-alias "" } */
/* { dg-require-ascii-locale "" } */
/* { dg-skip-if "" { powerpc*-*-aix* } } */

void f0 (void) __attribute__((alias("\xa1"))); /* { dg-error "undefined symbol '\\\\241'" } */
void f1 (void) __attribute__((alias("\u00e9"))); /* { dg-error "undefined symbol '\\\\U000000e9'" } */
void f2 (void) __attribute__((alias("\uffff"))); /* { dg-error "undefined symbol '\\\\U0000ffff'" } */
void f3 (void) __attribute__((alias("\U000fffff"))); /* { dg-error "undefined symbol '\\\\U000fffff'" } */
void f4 (void) __attribute__((alias("\U00ffffff"))); /* { dg-error "undefined symbol '\\\\U00ffffff'" } */
void f5 (void) __attribute__((alias("\U0fffffff"))); /* { dg-error "undefined symbol '\\\\U0fffffff'" } */

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
// DEFAULT-NEXT:     fn %[[VALUE_f0:[0-9]+]] @f0() -> void [linkage=external] [alias="\\xa1"];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> void [linkage=external] [alias="\\u00e9"];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [alias="\\uffff"];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=external] [alias="\\U000fffff"];
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4() -> void [linkage=external] [alias="\\U00ffffff"];
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5() -> void [linkage=external] [alias="\\U0fffffff"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
