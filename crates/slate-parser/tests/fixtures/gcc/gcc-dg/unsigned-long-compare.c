/* Copyright (C) 2006 Free Software Foundation, Inc. */
/* Contributed by Carlos O'Donell on 2006-01-30 */

/* Test a division corner case where the expression simplifies
   to a comparison, and the optab expansion is wrong. The optab 
   expansion emits a function whose return is unbiased and needs
   adjustment. */
/* Origin: Carlos O'Donell <carlos@codesourcery.com> */
/* { dg-do run } */
/* { dg-options "" } */
#include <stdlib.h>

#define BIG_CONSTANT 0xFFFFFFFF80000000ULL

int main (void)
{
  unsigned long long OneULL = 1ULL;
  unsigned long long result;

  result = OneULL / BIG_CONSTANT; 
  if (result)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%5 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 OneULL: u64 [storage=automatic] = const<u64>(1);
// DEFAULT-NEXT:         let %4 result: u64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%4, div<u64, by_zero=ub>(read<u64>(%3), const<u64>(18446744071562067968)));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(%4), const<u64>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
