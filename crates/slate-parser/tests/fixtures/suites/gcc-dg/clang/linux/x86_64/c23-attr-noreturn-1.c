/* Test C23 noreturn attribute: valid uses.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

[[noreturn]] void exit (int);

[[__noreturn__]] int f1 (void);

[[_Noreturn]] void f2 (void);

[[___Noreturn__]] static void f3 (void) { exit (0); }

/* Returning from a noreturn function is undefined at runtime, not a
   constraint violation, but recommended practice is to diagnose if
   such a return appears possible.  */

[[noreturn]] int
f4 (void)
{
  return 1; /* { dg-warning "has a 'return' statement" } */
  /* { dg-warning "does return" "second warning" { target *-*-* } .-1 } */
}

[[__noreturn__]] void
f5 (void)
{
  return; /* { dg-warning "has a 'return' statement" } */
  /* { dg-warning "does return" "second warning" { target *-*-* } .-1 } */
}

[[_Noreturn]] void
f6 (void)
{
} /* { dg-warning "does return" } */

[[___Noreturn__]] void
f7 (int a)
{
  if (a)
    exit (0);
} /* { dg-warning "does return" } */

/* Declarations need not all have the attribute (buf if the first does not,
   there is undefined behavior).  */

void f2 (void);

/* Duplicate attribute, and use with _Noreturn, is OK.  */
[[noreturn]] [[noreturn]] [[noreturn, __noreturn__]] void _Noreturn f9 (void);

/* The attribute does not affect type compatibility.  */

void (*fp) (void) = f5;

/* Unlike the function specifier, the attribute may be used on main.  */
[[noreturn]] int main ();

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_fp:[0-9]+]] fp: ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%[[VALUE_f5:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> void [linkage=internal] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4() -> i32 [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5]] @f5() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6() -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_a:[0-9]+]] a: i32) -> void [linkage=external] [noreturn] [fallthrough=ub] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [noreturn];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
