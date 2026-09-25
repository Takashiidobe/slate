/* Test unreachable in <stddef.h> for C23.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors -O2" } */

#include <stddef.h>

#ifndef unreachable
#error "unreachable not defined"
#endif

extern void                      *p;
extern __typeof__(unreachable()) *p;

volatile int x = 1;

extern void not_defined(void);

extern void exit(int);

int main() {
  if (x == 2) {
    unreachable();
    not_defined();
  }
  exit(0);
}






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
// DEFAULT-NEXT:     extern %0 p: ptr<void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 x: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %2 @not_defined() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @exit(%5 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if eq<i32>(read<i32, volatile>(%1), const<i32>(2))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(__builtin_unreachable);
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
