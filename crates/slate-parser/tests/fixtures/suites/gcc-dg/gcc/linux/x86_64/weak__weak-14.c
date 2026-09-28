// { dg-do run }
// { dg-require-weak "" }
// { dg-require-alias "" }
// { dg-options "-O2 -fno-common" }

// Copyright 2005 Free Software Foundation, Inc.
// Contributed by Alexandre Oliva <aoliva@redhat.com>

// PR middle-end/24295

// The unit-at-a-time call graph code used to fail to emit variables
// without external linkage that were only used indirectly, through
// aliases.  We might then get linker failures because the static
// variable was not defined, or run-time errors because the weak alias
// ended up pointing somewhere random.

#include <stdlib.h>

static unsigned long lv1 = 0xdeadbeefUL;
#pragma weak Av1a = lv1
extern unsigned long Av1a;

static unsigned long lf1(void) { return 0x510bea7UL; }
#pragma weak Af1a = lf1
extern unsigned long Af1a(void);

int main (void) {
  if (! &Av1a
      || ! &Af1a
      || Av1a != 0xdeadbeefUL
      || Af1a() != 0x510bea7UL)
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT gnu23
// SLATE-FILECHECK-ARGS -fno-common
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
// DEFAULT-NEXT:     global %3 lv1: u64 [storage=static] = const<u64>(3735928559) [linkage=internal];
// DEFAULT-NEXT:     global %4 Av1a: u64 [storage=static] [linkage=external] [weak] [alias="lv1"];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%8 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @lf1() -> u64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<u64>(84983463);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @Af1a() -> u64 [linkage=external] [weak] [alias="lf1"];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(not<bool>(ne<ptr<u64>>(addr_of<ptr<u64>>(%4), null<ptr<u64>>)), not<bool>(ne<ptr<fn() -> u64>>(addr_of<ptr<fn() -> u64>>(%6), null<ptr<fn() -> u64>>))), ne<u64>(read<u64>(%4), const<u64>(3735928559)))
// DEFAULT-NEXT:             write<bool>(%9, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%9, ne<u64>(call<u64, signature=fn() -> u64>(%6), const<u64>(84983463)));
// DEFAULT-NEXT:         if read<bool>(%9)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
