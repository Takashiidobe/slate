/* A simple check to see whether indirect calls are
   being sibcall optimized on targets that do support
   this notion, i.e. have the according call patterns
   in place.

   Copyright (C) 2002 Free Software Foundation Inc.
   Contributed by Andreas Bauer <baueran@in.tum.de>  */

/* { dg-do run { target epiphany-*-* i?86-*-* x86_64-*-* s390*-*-* } } */
/* { dg-skip-if "" { { i?86-*-* x86_64-*-* } && { ia32 && { ! nonpic } } } } */
/* { dg-options "-Wno-old-style-definition -O2 -foptimize-sibling-calls -fno-ipa-cp" } */

extern void abort (void);
extern void exit (int);

int foo (int);
int bar (int);

int (*ptr) (int);
int *f_addr;

int
main ()
{
  ptr = bar;
  foo (7);
  exit (0);
}

int __attribute__ ((noinline))
bar (b)
     int b;
{
  if (f_addr == (int*) __builtin_return_address (0))
    return b;
  else
    abort ();
}

int __attribute__ ((noinline))
foo (f)
     int f;
{
  f_addr = (int*) __builtin_return_address (0);
  return (*ptr)(f);
}

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
// DEFAULT-NEXT:     global %[[VALUE_ptr:[0-9]+]] ptr: ptr<fn(i32) -> i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f_addr:[0-9]+]] f_addr: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_f:[0-9]+]] f: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_f_addr]], pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE___builtin_return_address:[0-9]+]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         return call<i32, signature=fn(i32) -> i32>(read<ptr<fn(i32) -> i32>>(%[[VALUE_ptr]]), read<i32>(%[[VALUE_f]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<ptr<i32>>(read<ptr<i32>>(%[[VALUE_f_addr]]), pointer_cast<ptr<i32>, reason=explicit>(call<ptr<void>, signature=fn(u32) -> ptr<void>>(%[[VALUE___builtin_return_address]], reinterpret<u32, reason=arg, fits=always>(const<i32>(0)))))
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_b]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<fn(i32) -> i32>>(%[[VALUE_ptr]], function_decay<ptr<fn(i32) -> i32>>(%[[VALUE_bar]]));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo]], const<i32>(7));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_return_address]] @__builtin_return_address(%[[VALUE1:[0-9]+]] <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
