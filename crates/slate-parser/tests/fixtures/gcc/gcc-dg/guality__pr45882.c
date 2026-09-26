/* PR debug/45882 */
/* { dg-do run } */
/* { dg-options "-g" } */

/* Copyright (C) 2018 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#ifndef PREVENT_OPTIMIZATION_H
#define PREVENT_OPTIMIZATION_H

#ifdef PREVENT_OPTIMIZATION
#define ATTRIBUTE_USED __attribute__((used))
#else
#define ATTRIBUTE_USED
#endif

#endif

extern void        abort(void);
int                a[1024] ATTRIBUTE_USED;
volatile short int v;

__attribute__((noinline, noclone, used)) int foo(int i, int j) {
  int b = i;        /* { dg-final { gdb-test .+4 "b" "7" } } */
  int c = i + 4;    /* { dg-final { gdb-test .+3 "c" "11" } } */
  int d = a[i];     /* { dg-final { gdb-test .+2 "d" "112" } } */
  int e = a[i + 6]; /* { dg-final { gdb-test .+1 "e" "142" } } */
  ++v;
  return ++j;
}

int
main(void) {
  int l;
  asm("" : "=r"(l) : "0"(7));
  a[7]     = 112;
  a[7 + 6] = 142;
  if (foo(l, 7) != 8)
    abort();
  return l - 7;
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
// DEFAULT-NEXT:     global %1 a: array<i32, 1024> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %2 v: volatile i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @foo(%4 i: i32, %5 j: i32) -> i32 [linkage=external] [used] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 b: i32 [storage=automatic] = read<i32>(%4);
// DEFAULT-NEXT:         let %7 c: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%4), const<i32>(4));
// DEFAULT-NEXT:         let %8 d: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%1), read<i32>(%4))));
// DEFAULT-NEXT:         let %9 e: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%1), add<i32, overflow=ub>(read<i32>(%4), const<i32>(6)))));
// DEFAULT-NEXT:         let %12: i16 [synthetic] = read<i16, volatile>(%2);
// DEFAULT-NEXT:         let %13: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%12)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16, volatile>(%2, read<i16>(%13));
// DEFAULT-NEXT:         let %14: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%5, read<i32>(%15));
// DEFAULT-NEXT:         return read<i32>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 l: i32 [storage=automatic];
// DEFAULT-NEXT:         asm "" {
// DEFAULT-NEXT:             out 0 "=r" place<i32>(%11);
// DEFAULT-NEXT:             in 1 "0" const<i32>(7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%1), const<i32>(7))), const<i32>(112));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(%1), add<i32, overflow=ub>(const<i32>(7), const<i32>(6)))), const<i32>(142));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32) -> i32>(%3, read<i32>(%11), const<i32>(7)), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return sub<i32, overflow=ub>(read<i32>(%11), const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
