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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     extern %[[VALUE_a1:[0-9]+]] a1: @type[[TYPE_A]] [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_a2:[0-9]+]] a2: @type[[TYPE_A]] [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_a3:[0-9]+]] a3: @type[[TYPE_A]] [storage=thread] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_a4:[0-9]+]] a4: @type[[TYPE_A]] [storage=thread] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dummy:[0-9]+]] dummy: i32 [storage=thread] = const<i32>(12) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_local:[0-9]+]] local: @type[[TYPE_A]] [storage=thread] = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), field1 = const<i32>(2), field2 = widen<i64, reason=assign>(const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1a:[0-9]+]] @f1a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2a:[0-9]+]] @f2a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f3a:[0-9]+]] @f3a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f4a:[0-9]+]] @f4a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f5a:[0-9]+]] @f5a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f6a:[0-9]+]] @f6a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f7a:[0-9]+]] @f7a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f8a:[0-9]+]] @f8a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f9a:[0-9]+]] @f9a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f10a:[0-9]+]] @f10a() -> ptr<@type[[TYPE_A]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1b:[0-9]+]] @f1b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f2b:[0-9]+]] @f2b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f3b:[0-9]+]] @f3b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f4b:[0-9]+]] @f4b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f5b:[0-9]+]] @f5b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f6b:[0-9]+]] @f6b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f7b:[0-9]+]] @f7b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f8b:[0-9]+]] @f8b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f9b:[0-9]+]] @f9b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f10b:[0-9]+]] @f10b() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_check1:[0-9]+]] @check1() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_check2:[0-9]+]] @check2() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_A]]> [storage=automatic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_local]]))), const<i32>(1)), ne<i32>(read<i32>(field1(%[[VALUE_local]])), const<i32>(2))), ne<i64>(read<i64>(field2(%[[VALUE_local]])), widen<i64, reason=usual_arith>(const<i32>(3))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_a1]]))), const<i32>(4)), ne<i32>(read<i32>(field1(%[[VALUE_a1]])), const<i32>(5))), ne<i64>(read<i64>(field2(%[[VALUE_a1]])), widen<i64, reason=usual_arith>(const<i32>(6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_a2]]))), const<i32>(22)), ne<i32>(read<i32>(field1(%[[VALUE_a2]])), const<i32>(23))), ne<i64>(read<i64>(field2(%[[VALUE_a2]])), widen<i64, reason=usual_arith>(const<i32>(24))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_a3]]))), const<i32>(10)), ne<i32>(read<i32>(field1(%[[VALUE_a3]])), const<i32>(11))), ne<i64>(read<i64>(field2(%[[VALUE_a3]])), widen<i64, reason=usual_arith>(const<i32>(12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_a4]]))), const<i32>(25)), ne<i32>(read<i32>(field1(%[[VALUE_a4]])), const<i32>(26))), ne<i64>(read<i64>(field2(%[[VALUE_a4]])), widen<i64, reason=usual_arith>(const<i32>(27))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check1]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_check2]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_A]]>>(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f1a]]), addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a1]]))
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<ptr<@type[[TYPE_A]]>>(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f2a]]), addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a2]])));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<ptr<@type[[TYPE_A]]>>(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f3a]]), addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a3]])));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<ptr<@type[[TYPE_A]]>>(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f4a]]), addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a4]])));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]], call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f5a]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]]))))), const<i32>(16)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]])))), add<i32, overflow=ub>(const<i32>(16), const<i32>(1)))), ne<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]])))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(16), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]], call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f6a]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]]))))), const<i32>(19)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]])))), add<i32, overflow=ub>(const<i32>(19), const<i32>(1)))), ne<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]])))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(19), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_A]]>>(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f7a]]), addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a2]]))
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<ptr<@type[[TYPE_A]]>>(call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f8a]]), addr_of<ptr<@type[[TYPE_A]]>>(%[[VALUE_a4]])));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]], call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f9a]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]]))))), const<i32>(28)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]])))), add<i32, overflow=ub>(const<i32>(28), const<i32>(1)))), ne<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]])))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(28), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]], call<ptr<@type[[TYPE_A]]>, signature=fn() -> ptr<@type[[TYPE_A]]>>(%[[VALUE_f10a]]));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]]))))), const<i32>(31)), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]])))), add<i32, overflow=ub>(const<i32>(31), const<i32>(1)))), ne<i64>(read<i64>(field2(deref(read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]])))), widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(31), const<i32>(2)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
