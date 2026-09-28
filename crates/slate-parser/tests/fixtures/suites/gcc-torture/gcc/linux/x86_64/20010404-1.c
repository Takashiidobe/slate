// SLATE-FILECHECK-DEFINES DEFAULT

/* This testcase caused a floating point exception in the compiler when
   compiled with -O2. The crash occurs when trying to simplify division
   and modulo operations.  */

#include <limits.h>

extern void bar (int);

void foo ()
{
  int a = INT_MIN;
  int b = -1;
  bar (a / b);
  bar (a % b);
}

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
// DEFAULT-NEXT:     fn %0 @bar(%4 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 a: i32 [storage=automatic] = sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1));
// DEFAULT-NEXT:         let %3 b: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%2), read<i32>(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%2), read<i32>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
