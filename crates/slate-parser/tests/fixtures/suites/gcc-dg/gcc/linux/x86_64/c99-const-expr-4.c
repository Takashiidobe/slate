/* Test for constant expressions: const variable with value 0 is not a
   null pointer constant so the conditional expression should have
   type void * and the assignment is OK.  */
/* Origin: Joseph Myers <joseph@codesourcery.com> */
/* { dg-do compile } */
/* { dg-options "-std=iso9899:1999 -O2" } */
int *p;
long *q;
static void *const n = 0;
int j;
void f(void) { q = j ? p : n; }

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     global %0 p: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 q: ptr<i64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 n: ptr<void> [storage=static] [const] = null<ptr<void>> [linkage=internal];
// DEFAULT-NEXT:     global %3 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i64>>(%1, pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(read<i32>(%3), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%0)), read<ptr<void>>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
