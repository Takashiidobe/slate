/* Copyright (C) 2000 Free Software Foundation, Inc.  */

/* { dg-do run } */

/* Tests various macro abuse is correctly expanded.  */

#if DEBUG
extern int puts (const char *);
#else
#define puts(X)
#endif
extern void abort (void);
extern int strcmp(const char *s1, const char *s2);

#define err(str) do { puts(str); abort(); } while (0)
#define j(x, y) x + y
#define k(x, y) j(x + 2, y +
#define glue(x, y) x ## y
#define xglue(x, y) glue(x, y)

/* Functions called when macros are left unexpanded.  */
int q(int x)		{return x + 40;}
int B(int x)		{return x + 20;}
int foo(int x)		{return x + 10;}
int bar(int x, int y)	{return x + y;}
int baz(int x, int y)	{return x + y;}
int toupper(int x)	{return x + 32;}
int M(int x)		{return x * 2;}

int main (int argc, char *argv[])
{
#define q(x) x
  if (q(q)(2) != 42)
    err ("q");

#define A(x) B(x)
  if (A(A(2)) != 42)
    err ("A");

#define E(x) A x
#define F (22)
  if (E(F) != 42)
    err ("E(F)");

#define COMMA ,
#define NASTY(a) j(a 37)
  if (NASTY (5 COMMA) != 42)
    err ("NASTY");

#define bar(x, y) foo(x(y, 0))
#define apply(x, y) foo(x(y, 22))
#define bam bar
  if (bar(bar, 32) != 42)	/* foo(bar(32, 0)).  */
    err ("bar bar");
  if (bar(bam, 32) != 42)	/* Same.  */
    err ("bar bam");
  if (apply(bar, baz) != 42)	/* foo(foo(baz(22, 0))).  */
    err ("apply bar baz");

  /* Taken from glibc.  */
#define __tobody(c, f) f (c)
#define toupper(c) __tobody (c, toupper)
  if (toupper (10) != 42)	/* toupper (10). */
    err ("toupper");

  /* This tests that M gets expanded the right no. of times.  Too many
     times, and we get excess "2 +"s and the wrong sum.  Derived from
     nested stpcpy in dggettext.c.  */
#define M(x) 2 + M(x)
#define stpcpy(a) M(a)
  if (stpcpy (stpcpy (9)) != 42) /*  2 + M (2 + M (9)) */
    err ("stpcpy");

  /* Another test derived from nested stpcpy's of dggettext.c.  Uses
     macro A(x) and function B(x) as defined above.  The problem was
     the same - excess "1 +"s and the wrong sum.  */
#define B(x) 1 + B(x)
#define C(x) A(x)
  if (C(B(0)) != 42)		/* 1 + B (1 + B (0)) */
    err ("C");

  /* More tests derived from gcc itself - the use of XEXP and COST.
     These first two should both expand to the same thing.  */
  {
    int insn = 6, i = 2, b = 2;
#define XEXP(RTX, N)  (RTX * N + 2)
#define PATTERN(INSN) XEXP(INSN, 3)
    if (XEXP (PATTERN (insn), i) != 42)	/* ((insn * 3 + 2) * i + 2) */
      err ("XEXP (PATTERN)");
    if (XEXP (XEXP (insn, 3), i) != 42)	/* ((insn * 3 + 2) * i + 2) */
      err ("XEXP (XEXP)");

#define COST(X) XEXP (XEXP (X, 4), 4)
    if (COST (b) != 42)		/* ((b * 4 + 2) * 4 + 2) */
      err ("COST");
  }

  /* This tests macro recursion and expand-after-paste.  */
#define FORTYTWO "forty"
#define TWO TWO "-two"
  if (strcmp (glue(FORTY, TWO), "forty"))
    err ("glue");
  if (strcmp (xglue(FORTY, TWO), "forty-two"))
    err ("xglue");

  /* Test ability to call macro over multiple logical lines.  */
  if (q
      (42) != 42
      || q (
	 42) != 42
      || q (42
	    ) != 42
      || q
      (
       42
       )
      != 42)
    err ("q over multiple lines");

  /* Corner case.  Test that macro expansion is turned off for later
     q, when not at start but at end of argument context, and supplied
     with the '(' necessary for expansion.  */
  if (q(1 + q)(1) != 42)	/* 1 + q(1) */
    err ("Nested q");

  /* This looks like it has too many ')', but it hasn't.  */
  if (k(1, 4) 35) != 42)
    err ("k");

  /* Phew! */
  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %41 .str41: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 111, 114, 116, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([102, 111, 114, 116, 121, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %44 .str44: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([102, 111, 114, 116, 121, 45, 116, 119, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %45 .str45: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([102, 111, 114, 116, 121, 45, 116, 119, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @strcmp(%26 s1: ptr<const i8>, %27 s2: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @q(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%5), const<i32>(40));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @B(%7 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%7), const<i32>(20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @foo(%9 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%9), const<i32>(10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @bar(%11 x: i32, %12 y: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%11), read<i32>(%12));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @baz(%14 x: i32, %15 y: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%14), read<i32>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @toupper(%17 x: i32) -> i32 [linkage=external] [memory=read] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%17), const<i32>(32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @M(%19 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i32, overflow=ub>(read<i32>(%19), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main(%21 argc: i32, %22 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%4, const<i32>(2)), const<i32>(42))
// DEFAULT-NEXT:             do %28
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%6, call<i32, signature=fn(i32) -> i32>(%6, const<i32>(2))), const<i32>(42))
// DEFAULT-NEXT:             do %29
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%6, const<i32>(22)), const<i32>(42))
// DEFAULT-NEXT:             do %30
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(const<i32>(5), const<i32>(37)), const<i32>(42))
// DEFAULT-NEXT:             do %31
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%8, call<i32, signature=fn(i32, i32) -> i32>(%10, const<i32>(32), const<i32>(0))), const<i32>(42))
// DEFAULT-NEXT:             do %32
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%8, call<i32, signature=fn(i32, i32) -> i32>(%10, const<i32>(32), const<i32>(0))), const<i32>(42))
// DEFAULT-NEXT:             do %33
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%8, call<i32, signature=fn(i32) -> i32>(%8, call<i32, signature=fn(i32, i32) -> i32>(%13, const<i32>(22), const<i32>(0)))), const<i32>(42))
// DEFAULT-NEXT:             do %34
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%16, const<i32>(10)), const<i32>(42))
// DEFAULT-NEXT:             do %35
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(const<i32>(2), call<i32, signature=fn(i32) -> i32>(%18, add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(2), const<i32>(2)), call<i32, signature=fn(i32) -> i32>(%18, const<i32>(9))))), const<i32>(42))
// DEFAULT-NEXT:             do %36
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(const<i32>(1), call<i32, signature=fn(i32) -> i32>(%6, add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)), call<i32, signature=fn(i32) -> i32>(%6, const<i32>(0))))), const<i32>(42))
// DEFAULT-NEXT:             do %37
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %23 insn: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:             let %24 i: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             let %25 b: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:             if ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%23), const<i32>(3)), const<i32>(2)), read<i32>(%24)), const<i32>(2)), const<i32>(42))
// DEFAULT-NEXT:                 do %38
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             if ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%23), const<i32>(3)), const<i32>(2)), read<i32>(%24)), const<i32>(2)), const<i32>(42))
// DEFAULT-NEXT:                 do %39
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             if ne<i32>(add<i32, overflow=ub>(mul<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%25), const<i32>(4)), const<i32>(2)), const<i32>(4)), const<i32>(2)), const<i32>(42))
// DEFAULT-NEXT:                 do %40
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%41)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%42))), const<i32>(0))
// DEFAULT-NEXT:             do %43
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%44)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%45))), const<i32>(0))
// DEFAULT-NEXT:             do %46
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(const<i32>(42), const<i32>(42)), ne<i32>(const<i32>(42), const<i32>(42))), ne<i32>(const<i32>(42), const<i32>(42))), ne<i32>(const<i32>(42), const<i32>(42)))
// DEFAULT-NEXT:             do %47
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(const<i32>(1), call<i32, signature=fn(i32) -> i32>(%4, const<i32>(1))), const<i32>(42))
// DEFAULT-NEXT:             do %48
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(1), const<i32>(2)), const<i32>(4)), const<i32>(35)), const<i32>(42))
// DEFAULT-NEXT:             do %49
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
