/* Test C11 _Noreturn.  Test valid code.  */
/* { dg-do compile } */
/* { dg-options "-std=c11 -pedantic-errors" } */

_Noreturn void exit (int);

_Noreturn int f1 (void);

_Noreturn void f2 (void);

static void _Noreturn f3 (void) { exit (0); }

/* Returning from a noreturn function is undefined at runtime, not a
   constraint violation, but recommended practice is to diagnose if
   such a return appears possible.  */

_Noreturn int
f4 (void)
{
  return 1; /* { dg-warning "has a 'return' statement" } */
  /* { dg-warning "does return" "second warning" { target *-*-* } .-1 } */
}

_Noreturn void
f5 (void)
{
  return; /* { dg-warning "has a 'return' statement" } */
  /* { dg-warning "does return" "second warning" { target *-*-* } .-1 } */
}

_Noreturn void
f6 (void)
{
} /* { dg-warning "does return" } */

_Noreturn void
f7 (int a)
{
  if (a)
    exit (0);
} /* { dg-warning "does return" } */

/* Declarations need not all have _Noreturn.  */

void f2 (void);

void f8 (void);
_Noreturn void f8 (void);

/* Duplicate _Noreturn is OK.  */
_Noreturn _Noreturn void _Noreturn f9 (void);

/* _Noreturn does not affect type compatibility.  */

void (*fp) (void) = f5;

/* noreturn is an ordinary identifier.  */

int noreturn;

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     global %11 fp: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%5) [linkage=external];
// DEFAULT-NEXT:     global %12 noreturn: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%13 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @f1() -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @f2() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @f3() -> void [linkage=internal] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f4() -> i32 [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f5() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f6() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f7(%8 a: i32) -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @f8() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @f9() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
