/* { dg-do run } */
/* { dg-options "-O2 -fdump-tree-optimized -g" } */

/* SUB_OVERFLOW should be folded into unsigned subtraction,
   because ovf is never used.  */
__attribute__((noinline, noclone)) int
fn1 (int x, unsigned int y)
{
  int res;
  int ovf = __builtin_sub_overflow (x, y, &res);
  int res2 = res;
  int res3 = res2 - 2;
  (void) ovf;
  return res;
}

/* MUL_OVERFLOW should be folded into unsigned multiplication,
   because ovf is never used.  */
__attribute__((noinline, noclone)) int
fn2 (signed char x, long int y)
{
  short int res;
  int ovf = __builtin_mul_overflow (x, y, &res);
  int res2 = res;
  int res3 = res2 - 2;
  (void) ovf;
  return res;
}

#if __SIZEOF_INT__ > __SIZEOF_SHORT__ && __SIZEOF_INT__ > 1
/* ADD_OVERFLOW should be folded into unsigned addition,
   because it never overflows.  */
__attribute__((noinline, noclone)) int
fn3 (signed char x, unsigned short y, int *ovf)
{
  int res;
  *ovf = __builtin_add_overflow (x, y, &res);
  return res;
}
#endif

/* MUL_OVERFLOW should be folded into unsigned multiplication,
   because it never overflows.  */
__attribute__((noinline, noclone)) long int
fn4 (long int x, long int y, int *ovf)
{
  long int res;
  x &= 65535;
  y = (y & 65535) - 32768;
  *ovf = __builtin_mul_overflow (x, y, &res);
  return res;
}

#if __SIZEOF_INT__ > 1
/* MUL_OVERFLOW should be folded into unsigned multiplication,
   because it always overflows.  */
__attribute__((noinline, noclone)) signed char
fn5 (long int x, long int y, int *ovf)
{
  signed char res;
  x = (x & 63) + (__SCHAR_MAX__ / 4);
  y = (y & 3) + 5;
  *ovf = __builtin_mul_overflow (x, y, &res);
  return res;
}
#endif

/* ADD_OVERFLOW should be folded into unsigned additrion,
   because it never overflows.  */
__attribute__((noinline, noclone)) unsigned char
fn6 (unsigned char x, unsigned char y, int *ovf)
{
  unsigned char res;
  x = (x & 63) + ((unsigned char) ~0 - 66);
  y = (y & 3);
  *ovf = __builtin_add_overflow (x, y, &res);
  return res;
}

/* ADD_OVERFLOW should be folded into unsigned additrion,
   because it always overflows.  */
__attribute__((noinline, noclone)) unsigned char
fn7 (unsigned char x, unsigned char y, int *ovf)
{
  unsigned char res;
  x = (x & 15) + ((unsigned char) ~0 - 15);
  y = (y & 3) + 16;
  *ovf = __builtin_add_overflow (x, y, &res);
  return res;
}

int
main ()
{
  int ovf;
  if (fn1 (-10, __INT_MAX__) != (int) (-10U - __INT_MAX__)
      || fn2 (0, 0) != 0
      || fn2 (32, 16383) != (short int) 524256ULL)
    __builtin_abort ();
#if __SIZEOF_INT__ > __SIZEOF_SHORT__ && __SIZEOF_INT__ > 1
  if (fn3 (__SCHAR_MAX__, (unsigned short) ~0, &ovf) != (int) (__SCHAR_MAX__ + (unsigned short) ~0)
      || ovf
      || fn3 (-__SCHAR_MAX__ - 1, 0, &ovf) != (int) (-__SCHAR_MAX__ - 1)
      || ovf)
    __builtin_abort ();
#endif
  if (fn4 (65535, 0, &ovf) != 65535L * -32768 || ovf)
    __builtin_abort ();
#if __SIZEOF_INT__ > 1
  if (fn5 (0, 0, &ovf) != (signed char) (__SCHAR_MAX__ / 4 * 5)
      || !ovf
      || fn5 (63, 3, &ovf) != (signed char) ((__SCHAR_MAX__ / 4 + 63) * 8)
      || !ovf)
    __builtin_abort ();
#endif
  if (fn6 (0, 0, &ovf) != (unsigned char) ~0 - 66
      || ovf
      || fn6 (63, 3, &ovf) != (unsigned char) ~0
      || ovf)
    __builtin_abort ();
  if (fn7 (0, 0, &ovf) != 0
      || !ovf
      || fn7 (63, 3, &ovf) != 18
      || !ovf)
    __builtin_abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-not "ADD_OVERFLOW" "optimized" } } */
/* { dg-final { scan-tree-dump-not "SUB_OVERFLOW" "optimized" } } */
/* { dg-final { scan-tree-dump-not "MUL_OVERFLOW" "optimized" } } */

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
// DEFAULT-NEXT:     fn %0 @fn1(%1 x: i32, %2 y: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 res: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 ovf: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_sub<bool>(read<i32>(%1), read<u32>(%2), deref(addr_of<ptr<i32>>(%3))));
// DEFAULT-NEXT:         let %5 res2: i32 [storage=automatic] = read<i32>(%3);
// DEFAULT-NEXT:         let %6 res3: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%5), const<i32>(2));
// DEFAULT-NEXT:         read<i32>(%4);
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @fn2(%8 x: i8, %9 y: i64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 res: i16 [storage=automatic];
// DEFAULT-NEXT:         let %11 ovf: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_mul<bool>(read<i8>(%8), read<i64>(%9), deref(addr_of<ptr<i16>>(%10))));
// DEFAULT-NEXT:         let %12 res2: i32 [storage=automatic] = widen<i32, reason=assign>(read<i16>(%10));
// DEFAULT-NEXT:         let %13 res3: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%12), const<i32>(2));
// DEFAULT-NEXT:         read<i32>(%11);
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i16>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @fn3(%15 x: i8, %16 y: u16, %17 ovf: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 res: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%17)), from_bool<i32, reason=assign>(overflow_add<bool>(read<i8>(%15), read<u16>(%16), deref(addr_of<ptr<i32>>(%18)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_add<bool>(read<i8>(%15), read<u16>(%16), deref(addr_of<ptr<i32>>(%18))));
// DEFAULT-NEXT:         return read<i32>(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @fn4(%20 x: i64, %21 y: i64, %22 ovf: ptr<i32>) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %23 res: i64 [storage=automatic];
// DEFAULT-NEXT:         let %42: i64 [synthetic] = read<i64>(%20);
// DEFAULT-NEXT:         let %43: i64 [synthetic] = and<i64>(read<i64>(%42), widen<i64, reason=usual_arith>(const<i32>(65535)));
// DEFAULT-NEXT:         write<i64>(%20, read<i64>(%43));
// DEFAULT-NEXT:         write<i64>(%21, sub<i64, overflow=ub>(and<i64>(read<i64>(%21), widen<i64, reason=usual_arith>(const<i32>(65535))), widen<i64, reason=usual_arith>(const<i32>(32768))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%22)), from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%20), read<i64>(%21), deref(addr_of<ptr<i64>>(%23)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%20), read<i64>(%21), deref(addr_of<ptr<i64>>(%23))));
// DEFAULT-NEXT:         return read<i64>(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @fn5(%25 x: i64, %26 y: i64, %27 ovf: ptr<i32>) -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %28 res: i8 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%25, add<i64, overflow=ub>(and<i64>(read<i64>(%25), widen<i64, reason=usual_arith>(const<i32>(63))), widen<i64, reason=usual_arith>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4)))));
// DEFAULT-NEXT:         write<i64>(%26, add<i64, overflow=ub>(and<i64>(read<i64>(%26), widen<i64, reason=usual_arith>(const<i32>(3))), widen<i64, reason=usual_arith>(const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%27)), from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%25), read<i64>(%26), deref(addr_of<ptr<i8>>(%28)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%25), read<i64>(%26), deref(addr_of<ptr<i8>>(%28))));
// DEFAULT-NEXT:         return read<i8>(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @fn6(%30 x: u8, %31 y: u8, %32 ovf: ptr<i32>) -> u8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %33 res: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%30, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%30))), const<i32>(63)), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(66))))));
// DEFAULT-NEXT:         write<u8>(%31, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%31))), const<i32>(3)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%32)), from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%30), read<u8>(%31), deref(addr_of<ptr<u8>>(%33)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%30), read<u8>(%31), deref(addr_of<ptr<u8>>(%33))));
// DEFAULT-NEXT:         return read<u8>(%33);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @fn7(%35 x: u8, %36 y: u8, %37 ovf: ptr<i32>) -> u8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %38 res: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%35, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%35))), const<i32>(15)), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(15))))));
// DEFAULT-NEXT:         write<u8>(%36, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%36))), const<i32>(3)), const<i32>(16)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%37)), from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%35), read<u8>(%36), deref(addr_of<ptr<u8>>(%38)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%35), read<u8>(%36), deref(addr_of<ptr<u8>>(%38))));
// DEFAULT-NEXT:         return read<u8>(%38);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %39 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %40 ovf: i32 [storage=automatic];
// DEFAULT-NEXT:         let %44: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, u32) -> i32>(%0, neg<i32, overflow=ub>(const<i32>(10)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), reinterpret<i32, reason=explicit, fits=unknown>(sub<u32, overflow=wrap>(neg<u32, overflow=wrap>(const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)))))
// DEFAULT-NEXT:             write<bool>(%44, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%44, ne<i32>(call<i32, signature=fn(i8, i64) -> i32>(%7, truncate<i8, reason=arg, fits=always>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))), const<i32>(0)));
// DEFAULT-NEXT:         let %45: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%44)
// DEFAULT-NEXT:             write<bool>(%45, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%45, ne<i32>(call<i32, signature=fn(i8, i64) -> i32>(%7, truncate<i8, reason=arg, fits=always>(const<i32>(32)), widen<i64, reason=arg>(const<i32>(16383))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(const<u64>(524256))))));
// DEFAULT-NEXT:         if read<bool>(%45)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%41);
// DEFAULT-NEXT:         let %46: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(call<i32, signature=fn(i8, u16, ptr<i32>) -> i32>(%14, truncate<i8, reason=arg, fits=always>(const<i32>(127)), reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))), addr_of<ptr<i32>>(%40)), add<i32, overflow=ub>(const<i32>(127), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))))), ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%46, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%46, ne<i32>(call<i32, signature=fn(i8, u16, ptr<i32>) -> i32>(%14, truncate<i8, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%40)), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%46), ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%41);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(call<i64, signature=fn(i64, i64, ptr<i32>) -> i64>(%19, widen<i64, reason=arg>(const<i32>(65535)), widen<i64, reason=arg>(const<i32>(0)), addr_of<ptr<i32>>(%40)), mul<i64, overflow=ub>(const<i64>(65535), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(32768))))), ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%41);
// DEFAULT-NEXT:         let %47: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i64, i64, ptr<i32>) -> i8>(%24, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), addr_of<ptr<i32>>(%40))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4)), const<i32>(5))))), not<bool>(ne<i32>(read<i32>(%40), const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%47, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%47, ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i64, i64, ptr<i32>) -> i8>(%24, widen<i64, reason=arg>(const<i32>(63)), widen<i64, reason=arg>(const<i32>(3)), addr_of<ptr<i32>>(%40))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4)), const<i32>(63)), const<i32>(8))))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%47), not<bool>(ne<i32>(read<i32>(%40), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%41);
// DEFAULT-NEXT:         let %48: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%29, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%40)))), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(66))), ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%48, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%48, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%29, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(63))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3))), addr_of<ptr<i32>>(%40)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0))))))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%48), ne<i32>(read<i32>(%40), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%41);
// DEFAULT-NEXT:         let %49: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%34, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%40)))), const<i32>(0)), not<bool>(ne<i32>(read<i32>(%40), const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%49, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%49, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%34, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(63))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3))), addr_of<ptr<i32>>(%40)))), const<i32>(18)));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%49), not<bool>(ne<i32>(read<i32>(%40), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%41);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
