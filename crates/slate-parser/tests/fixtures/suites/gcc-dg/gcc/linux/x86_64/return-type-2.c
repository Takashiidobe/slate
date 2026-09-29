/* Bogus warnings claiming we fall off the end of a non-void function.
   By Kaveh R. Ghazi <ghazi@caip.rutgers.edu> 8/27/2000.  */
/* { dg-do compile } */
/* { dg-options "-O2 -Wreturn-type" } */

extern void abort (void) __attribute__ ((__noreturn__));

int
foo1 (int i)
{
  if (i)
    return i;

  abort ();
} /* { dg-bogus "control reaches end of non-void function" "warning for falling off end of non-void function" } */

__inline__ int
foo2 (int i)
{
  if (i)
    return i;

  abort ();
} /* { dg-bogus "control reaches end of non-void function" "warning for falling off end of non-void function" } */

static int
foo3 (int i)
{
  if (i)
    return i;

  abort ();
} /* { dg-bogus "control reaches end of non-void function" "warning for falling off end of non-void function" } */

static __inline__ int
foo4 (int i)
{
  if (i)
    return i;

  abort ();
} /* { dg-bogus "control reaches end of non-void function" "warning for falling off end of non-void function" } */

int bar (int i)
{
  return foo1 (i) + foo2 (i) + foo3 (i) + foo4 (i);
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_i:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_i_2:[0-9]+]] i: i32) -> i32 [linkage=external] [inline=hint] [definition=inline_only] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(0))
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo3:[0-9]+]] @foo3(%[[VALUE_i_3:[0-9]+]] i: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(0))
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4(%[[VALUE_i_4:[0-9]+]] i: i32) -> i32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(0))
// DEFAULT-NEXT:             return read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_i_5:[0-9]+]] i: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo1]], read<i32>(%[[VALUE_i_5]])), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo2]], read<i32>(%[[VALUE_i_5]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo3]], read<i32>(%[[VALUE_i_5]]))), call<i32, signature=fn(i32) -> i32>(%[[VALUE_foo4]], read<i32>(%[[VALUE_i_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
