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
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: volatile ptr<fn() -> void> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pp:[0-9]+]] pp: volatile ptr<volatile ptr<fn() -> void>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_link_failure:[0-9]+]] @link_failure() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>, volatile>(%[[VALUE_p]]));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_link_failure]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h:[0-9]+]] @h() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>, volatile>(deref(read<ptr<volatile ptr<fn() -> void>>, volatile>(%[[VALUE_pp]]))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_link_failure]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%[[VALUE_flag]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32, volatile>(%[[VALUE_flag]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_h]]);
// DEFAULT-NEXT:         return read<i32, volatile>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
