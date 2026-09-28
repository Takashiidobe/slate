/* Test __typeof__ propagation of noreturn function attributes with -std=gnu23:
   these are part of the type of a function pointer with GNU __typeof__, but
   not with C23 typeof.  */
/* { dg-do link } */
/* { dg-options "-std=gnu23 -O2" } */

_Noreturn void f (void);

__typeof__ (&f) volatile p;
__typeof__ (&p) volatile pp;

void link_failure (void);

void
g (void)
{
  (*p) ();
  link_failure ();
}

void
h (void)
{
  (**pp) ();
  link_failure ();
}

volatile int flag;
volatile int x;

int
main (void)
{
  if (flag)
    g ();
  if (flag)
    h ();
  return x;
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
// DEFAULT-NEXT:     global %1 p: volatile ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 pp: volatile ptr<volatile ptr<fn() -> void>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 flag: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 x: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @f() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @link_failure() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>, volatile>(%1));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @h() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>, volatile>(deref(read<ptr<volatile ptr<fn() -> void>>, volatile>(%2))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%6), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%4);
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%6), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         return read<i32, volatile>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
