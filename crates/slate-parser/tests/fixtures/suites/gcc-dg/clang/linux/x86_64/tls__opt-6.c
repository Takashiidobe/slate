/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-require-effective-target tls } */

extern void abort (void);
extern void exit (int);

struct A
{
  char a;
  int b;
  long long c;
};
extern __thread struct A a1, a2, a3, a4;
extern struct A *f1a (void);
extern struct A *f2a (void);
extern struct A *f3a (void);
extern struct A *f4a (void);
extern struct A *f5a (void);
extern struct A *f6a (void);
extern struct A *f7a (void);
extern struct A *f8a (void);
extern struct A *f9a (void);
extern struct A *f10a (void);
extern int f1b (void);
extern int f2b (void);
extern int f3b (void);
extern int f4b (void);
extern int f5b (void);
extern int f6b (void);
extern int f7b (void);
extern int f8b (void);
extern int f9b (void);
extern int f10b (void);
extern void check1 (void);
extern void check2 (void);
__thread int dummy = 12;
__thread struct A local = { 1, 2, 3 };

int
main (void)
{
  struct A *p;

  if (local.a != 1 || local.b != 2 || local.c != 3)
    abort ();
  if (a1.a != 4 || a1.b != 5 || a1.c != 6)
    abort ();
  if (a2.a != 22 || a2.b != 23 || a2.c != 24)
    abort ();
  if (a3.a != 10 || a3.b != 11 || a3.c != 12)
    abort ();
  if (a4.a != 25 || a4.b != 26 || a4.c != 27)
    abort ();
  check1 ();
  check2 ();
  if (f1a () != &a1 || f2a () != &a2 || f3a () != &a3 || f4a () != &a4)
    abort ();
  p = f5a (); if (p->a != 16 || p->b != 16 + 1 || p->c != 16 + 2)
    abort ();
  p = f6a (); if (p->a != 19 || p->b != 19 + 1 || p->c != 19 + 2)
    abort ();
  if (f7a () != &a2 || f8a () != &a4)
    abort ();
  p = f9a (); if (p->a != 28 || p->b != 28 + 1 || p->c != 28 + 2)
    abort ();
  p = f10a (); if (p->a != 31 || p->b != 31 + 1 || p->c != 31 + 2)
    abort ();

  exit (0);
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     extern %3 a1: @type0 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %4 a2: @type0 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %5 a3: @type0 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %6 a4: @type0 [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %29 dummy: i32 [storage=thread] = const<i32>(12) [linkage=external];
// DEFAULT-NEXT:     global %30 local: @type0 [storage=thread] = aggregate<@type0, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = const<i32>(2), field2 = widen<i64, reason=assign>(const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%33 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @f1a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %8 @f2a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %9 @f3a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %10 @f4a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %11 @f5a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %12 @f6a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %13 @f7a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %14 @f8a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %15 @f9a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %16 @f10a() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %17 @f1b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %18 @f2b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %19 @f3b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @f4b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @f5b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %22 @f6b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @f7b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %24 @f8b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @f9b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @f10b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %27 @check1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %28 @check2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %31 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %32 p: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%30))), const<i32>(1)), ne<i32>(read<i32>(field1(%30)), const<i32>(2))), ne<i64>(read<i64>(field2(%30)), widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%3))), const<i32>(4)), ne<i32>(read<i32>(field1(%3)), const<i32>(5))), ne<i64>(read<i64>(field2(%3)), widen<i64, reason=usual_arith>(const<i32>(6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%4))), const<i32>(22)), ne<i32>(read<i32>(field1(%4)), const<i32>(23))), ne<i64>(read<i64>(field2(%4)), widen<i64, reason=usual_arith>(const<i32>(24))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%5))), const<i32>(10)), ne<i32>(read<i32>(field1(%5)), const<i32>(11))), ne<i64>(read<i64>(field2(%5)), widen<i64, reason=usual_arith>(const<i32>(12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%6))), const<i32>(25)), ne<i32>(read<i32>(field1(%6)), const<i32>(26))), ne<i64>(read<i64>(field2(%6)), widen<i64, reason=usual_arith>(const<i32>(27))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%28);
// DEFAULT-NEXT:         let %34: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<@type0>>(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%7), addr_of<ptr<@type0>>(%3))
// DEFAULT-NEXT:             write<bool>(%34, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%34, ne<ptr<@type0>>(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%8), addr_of<ptr<@type0>>(%4)));
// DEFAULT-NEXT:         let %35: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%34)
// DEFAULT-NEXT:             write<bool>(%35, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%35, ne<ptr<@type0>>(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%9), addr_of<ptr<@type0>>(%5)));
// DEFAULT-NEXT:         let %36: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%35)
// DEFAULT-NEXT:             write<bool>(%36, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%36, ne<ptr<@type0>>(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%10), addr_of<ptr<@type0>>(%6)));
// DEFAULT-NEXT:         if read<bool>(%36)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<ptr<@type0>>(%32, call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%11));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%11);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type0>>(%32))))), const<i32>(16)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%32)))), add<i32, overflow=ub>(const<i32>(16), const<i32>(1)))), ne<i64>(read<i64>(field2(deref(read<ptr<@type0>>(%32)))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(16), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<ptr<@type0>>(%32, call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%12));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%12);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type0>>(%32))))), const<i32>(19)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%32)))), add<i32, overflow=ub>(const<i32>(19), const<i32>(1)))), ne<i64>(read<i64>(field2(deref(read<ptr<@type0>>(%32)))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(19), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %37: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<@type0>>(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%13), addr_of<ptr<@type0>>(%4))
// DEFAULT-NEXT:             write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%37, ne<ptr<@type0>>(call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%14), addr_of<ptr<@type0>>(%6)));
// DEFAULT-NEXT:         if read<bool>(%37)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<ptr<@type0>>(%32, call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%15));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%15);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type0>>(%32))))), const<i32>(28)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%32)))), add<i32, overflow=ub>(const<i32>(28), const<i32>(1)))), ne<i64>(read<i64>(field2(deref(read<ptr<@type0>>(%32)))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(28), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<ptr<@type0>>(%32, call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%16));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%16);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type0>>(%32))))), const<i32>(31)), ne<i32>(read<i32>(field1(deref(read<ptr<@type0>>(%32)))), add<i32, overflow=ub>(const<i32>(31), const<i32>(1)))), ne<i64>(read<i64>(field2(deref(read<ptr<@type0>>(%32)))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(31), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
