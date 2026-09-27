/* { dg-do run } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

/* MUL_OVERFLOW should not be folded into unsigned multiplication,
   because it sometimes overflows and sometimes does not.  */
__attribute__((noinline, noclone)) long int
fn1 (long int x, long int y, int *ovf)
{
  long int res;
  x &= 65535;
  y = (y & 65535) - (__LONG_MAX__ / 65535 + 32768);
  *ovf = __builtin_mul_overflow (x, y, &res);
  return res;
}

/* MUL_OVERFLOW should not be folded into unsigned multiplication,
   because it sometimes overflows and sometimes does not.  */
__attribute__((noinline, noclone)) signed char
fn2 (long int x, long int y, int *ovf)
{
  signed char res;
  x = (x & 63) + (__SCHAR_MAX__ / 4);
  y = (y & 3) + 4;
  *ovf = __builtin_mul_overflow (x, y, &res);
  return res;
}

/* ADD_OVERFLOW should be folded into unsigned additrion,
   because it sometimes overflows and sometimes does not.  */
__attribute__((noinline, noclone)) unsigned char
fn3 (unsigned char x, unsigned char y, int *ovf)
{
  unsigned char res;
  x = (x & 63) + ((unsigned char) ~0 - 65);
  y = (y & 3);
  *ovf = __builtin_add_overflow (x, y, &res);
  return res;
}

/* ADD_OVERFLOW should be folded into unsigned additrion,
   because it sometimes overflows and sometimes does not.  */
__attribute__((noinline, noclone)) unsigned char
fn4 (unsigned char x, unsigned char y, int *ovf)
{
  unsigned char res;
  x = (x & 15) + ((unsigned char) ~0 - 16);
  y = (y & 3) + 16;
  *ovf = __builtin_add_overflow (x, y, &res);
  return res;
}

/* MUL_OVERFLOW should not be folded into unsigned multiplication,
   because it sometimes overflows and sometimes does not.  */
__attribute__((noinline, noclone)) long int
fn5 (long int x, unsigned long int y, int *ovf)
{
  long int res;
  y = -65536UL + (y & 65535);
  *ovf = __builtin_mul_overflow (x, y, &res);
  return res;
}

int
main ()
{
  int ovf;
  if (fn1 (0, 0, &ovf) != 0
      || ovf
      || fn1 (65535, 0, &ovf) != (long int) ((__LONG_MAX__ / 65535 + 32768UL) * -65535UL)
      || !ovf)
    __builtin_abort ();
  if (fn2 (0, 0, &ovf) != (signed char) (__SCHAR_MAX__ / 4 * 4U)
      || ovf
      || fn2 (0, 1, &ovf) != (signed char) (__SCHAR_MAX__ / 4 * 5U)
      || !ovf)
    __builtin_abort ();
  if (fn3 (0, 0, &ovf) != (unsigned char) ~0 - 65
      || ovf
      || fn3 (63, 2, &ovf) != (unsigned char) ~0
      || ovf
      || fn3 (62, 3, &ovf) != (unsigned char) ~0
      || ovf
      || fn3 (63, 3, &ovf) != 0
      || !ovf)
    __builtin_abort ();
  if (fn4 (0, 0, &ovf) != (unsigned char) ~0
      || ovf
      || fn4 (1, 0, &ovf) != 0
      || !ovf
      || fn4 (0, 1, &ovf) != 0
      || !ovf
      || fn4 (63, 3, &ovf) != 17
      || !ovf)
    __builtin_abort ();
  if (fn5 (0, 0, &ovf) != 0
      || ovf
      || fn5 (1, 0, &ovf) != -65536L
      || !ovf
      || fn5 (2, 32768, &ovf) != -65536L
      || !ovf
      || fn5 (4, 32768 + 16384 + 8192, &ovf) != -32768L
      || !ovf)
    __builtin_abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "ADD_OVERFLOW" 2 "optimized" } } */
/* { dg-final { scan-tree-dump-times "SUB_OVERFLOW" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-times "MUL_OVERFLOW" 3 "optimized" } } */

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
// DEFAULT-NEXT:     fn %0 @fn1(%1 x: i64, %2 y: i64, %3 ovf: ptr<i32>) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 res: i64 [storage=automatic];
// DEFAULT-NEXT:         let %28: i64 [synthetic] = read<i64>(%1);
// DEFAULT-NEXT:         let %29: i64 [synthetic] = and<i64>(read<i64>(%28), widen<i64, reason=usual_arith>(const<i32>(65535)));
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%29));
// DEFAULT-NEXT:         write<i64>(%2, sub<i64, overflow=ub>(and<i64>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(65535))), add<i64, overflow=ub>(div<i64, by_zero=ub, min_by_neg_one=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(65535))), widen<i64, reason=usual_arith>(const<i32>(32768)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%3)), from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%1), read<i64>(%2), deref(addr_of<ptr<i64>>(%4)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%1), read<i64>(%2), deref(addr_of<ptr<i64>>(%4))));
// DEFAULT-NEXT:         return read<i64>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @fn2(%6 x: i64, %7 y: i64, %8 ovf: ptr<i32>) -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 res: i8 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%6, add<i64, overflow=ub>(and<i64>(read<i64>(%6), widen<i64, reason=usual_arith>(const<i32>(63))), widen<i64, reason=usual_arith>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4)))));
// DEFAULT-NEXT:         write<i64>(%7, add<i64, overflow=ub>(and<i64>(read<i64>(%7), widen<i64, reason=usual_arith>(const<i32>(3))), widen<i64, reason=usual_arith>(const<i32>(4))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%8)), from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%6), read<i64>(%7), deref(addr_of<ptr<i8>>(%9)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%6), read<i64>(%7), deref(addr_of<ptr<i8>>(%9))));
// DEFAULT-NEXT:         return read<i8>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @fn3(%11 x: u8, %12 y: u8, %13 ovf: ptr<i32>) -> u8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 res: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%11, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%11))), const<i32>(63)), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(65))))));
// DEFAULT-NEXT:         write<u8>(%12, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%12))), const<i32>(3)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%13)), from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%11), read<u8>(%12), deref(addr_of<ptr<u8>>(%14)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%11), read<u8>(%12), deref(addr_of<ptr<u8>>(%14))));
// DEFAULT-NEXT:         return read<u8>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @fn4(%16 x: u8, %17 y: u8, %18 ovf: ptr<i32>) -> u8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %19 res: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%16, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%16))), const<i32>(15)), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(16))))));
// DEFAULT-NEXT:         write<u8>(%17, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%17))), const<i32>(3)), const<i32>(16)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%18)), from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%16), read<u8>(%17), deref(addr_of<ptr<u8>>(%19)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%16), read<u8>(%17), deref(addr_of<ptr<u8>>(%19))));
// DEFAULT-NEXT:         return read<u8>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @fn5(%21 x: i64, %22 y: u64, %23 ovf: ptr<i32>) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24 res: i64 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(%22, add<u64, overflow=wrap>(neg<u64, overflow=wrap>(const<u64>(65536)), and<u64>(read<u64>(%22), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(65535))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%23)), from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%21), read<u64>(%22), deref(addr_of<ptr<i64>>(%24)))));
// DEFAULT-NEXT:         from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%21), read<u64>(%22), deref(addr_of<ptr<i64>>(%24))));
// DEFAULT-NEXT:         return read<i64>(%24);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %26 ovf: i32 [storage=automatic];
// DEFAULT-NEXT:         let %30: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(call<i64, signature=fn(i64, i64, ptr<i32>) -> i64>(%0, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), addr_of<ptr<i32>>(%26)), widen<i64, reason=usual_arith>(const<i32>(0))), ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%30, ne<i64>(call<i64, signature=fn(i64, i64, ptr<i32>) -> i64>(%0, widen<i64, reason=arg>(const<i32>(65535)), widen<i64, reason=arg>(const<i32>(0)), addr_of<ptr<i32>>(%26)), reinterpret<i64, reason=explicit, fits=unknown>(mul<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(div<i64, by_zero=ub, min_by_neg_one=ub>(const<i64>(9223372036854775807), widen<i64, reason=usual_arith>(const<i32>(65535)))), const<u64>(32768)), neg<u64, overflow=wrap>(const<u64>(65535))))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%30), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         let %31: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i64, i64, ptr<i32>) -> i8>(%5, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), addr_of<ptr<i32>>(%26))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4))), const<u32>(4)))))), ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%31, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%31, ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i64, i64, ptr<i32>) -> i8>(%5, widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(1)), addr_of<ptr<i32>>(%26))), widen<i32, reason=promotion>(reinterpret<i8, reason=explicit, fits=unknown>(truncate<u8, reason=explicit, fits=unknown>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4))), const<u32>(5)))))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%31), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         let %32: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%10, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%26)))), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(65))), ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%32, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%32, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%10, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(63))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(2))), addr_of<ptr<i32>>(%26)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0))))))));
// DEFAULT-NEXT:         let %33: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%32), ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%33, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%33, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%10, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(62))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3))), addr_of<ptr<i32>>(%26)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0))))))));
// DEFAULT-NEXT:         let %34: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%33), ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%34, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%34, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%10, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(63))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3))), addr_of<ptr<i32>>(%26)))), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%34), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         let %35: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%15, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%26)))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0))))))), ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%35, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%35, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%15, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%26)))), const<i32>(0)));
// DEFAULT-NEXT:         let %36: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%35), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%36, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%36, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%15, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(1))), addr_of<ptr<i32>>(%26)))), const<i32>(0)));
// DEFAULT-NEXT:         let %37: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%36), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%37, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%37, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%15, reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(63))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3))), addr_of<ptr<i32>>(%26)))), const<i32>(17)));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%37), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         let %38: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(call<i64, signature=fn(i64, u64, ptr<i32>) -> i64>(%20, widen<i64, reason=arg>(const<i32>(0)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), addr_of<ptr<i32>>(%26)), widen<i64, reason=usual_arith>(const<i32>(0))), ne<i32>(read<i32>(%26), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%38, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%38, ne<i64>(call<i64, signature=fn(i64, u64, ptr<i32>) -> i64>(%20, widen<i64, reason=arg>(const<i32>(1)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(0))), addr_of<ptr<i32>>(%26)), neg<i64, overflow=ub>(const<i64>(65536))));
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%38), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%39, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%39, ne<i64>(call<i64, signature=fn(i64, u64, ptr<i32>) -> i64>(%20, widen<i64, reason=arg>(const<i32>(2)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32768))), addr_of<ptr<i32>>(%26)), neg<i64, overflow=ub>(const<i64>(65536))));
// DEFAULT-NEXT:         let %40: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%39), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%40, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%40, ne<i64>(call<i64, signature=fn(i64, u64, ptr<i32>) -> i64>(%20, widen<i64, reason=arg>(const<i32>(4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(add<i32, overflow=ub>(add<i32, overflow=ub>(const<i32>(32768), const<i32>(16384)), const<i32>(8192)))), addr_of<ptr<i32>>(%26)), neg<i64, overflow=ub>(const<i64>(32768))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%40), not<bool>(ne<i32>(read<i32>(%26), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%27);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
