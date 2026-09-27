/* Check for various valid and erroneous "noreturn" cases. */
/* { dg-do compile } */
/* { dg-options "-O2 -Wmissing-noreturn" } */

extern void exit (int);

extern void foo1(void) __attribute__ ((__noreturn__));
void
foo1(void)
{
} /* { dg-warning "'noreturn' function does return" "detect falling off end of noreturn" } */

extern void foo2(void) __attribute__ ((__noreturn__));
void
foo2(void)
{
  exit(0);
} /* { dg-bogus "warning:" "this function should not get any warnings" } */

extern void foo3(void);
void
foo3(void)
{
} /* { dg-bogus "warning:" "this function should not get any warnings" } */

extern void foo4(void);
void
foo4(void) /* { dg-warning "candidate for attribute 'noreturn'" "detect noreturn candidate" } */
{
  exit(0);
}

extern void foo5(void) __attribute__ ((__noreturn__));
void
foo5(void)
{
  return; /* { dg-warning "'noreturn' has a 'return' statement" "detect invalid return" } */
}         /* { dg-warning "function does return" "detect return from noreturn" { target c } .-1 } */

extern void foo6(void);
void
foo6(void)
{
  return;
} /* { dg-bogus "warning:" "this function should not get any warnings" } */

extern void foo7(void);
void
foo7(void)
{
  foo6();
} /* { dg-bogus "warning:" "this function should not get any warnings" } */

extern void foo8(void) __attribute__ ((__noreturn__));
void
foo8(void)
{
  foo7();
} /* { dg-warning "'noreturn' function does return" "detect return from tail call" } */

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
// DEFAULT-NEXT:     fn %0 @exit(%9 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @foo1() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @foo2() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @foo3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @foo4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @foo5() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @foo6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @foo7() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @foo8() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
