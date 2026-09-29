/* Test qualifiers on function return types after DR#423: those
   qualifiers are now ignored for all purposes (except that _Atomic
   still affects the type), but should still get warnings.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -Wignored-qualifiers" } */

const int f1 (void); /* { dg-warning "qualifiers ignored" } */
volatile int f2 (void) { return 0; } /* { dg-warning "qualifiers ignored" } */
const volatile void f3 (void) { } /* { dg-warning "qualifiers ignored" } */
const void f4 (void); /* { dg-warning "qualifiers ignored" } */
_Atomic int f5 (void); /* { dg-warning "qualifiers ignored" } */
_Atomic int f6 (void) { return 0; } /* { dg-warning "qualifiers ignored" } */

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
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
