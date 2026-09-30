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
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: u32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ovf:[0-9]+]] ovf: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_sub<bool>(read<i32>(%[[VALUE_x]]), read<u32>(%[[VALUE_y]]), deref(addr_of<ptr<i32>>(%[[VALUE_res]]))));
// DEFAULT-NEXT:         let %[[VALUE_res2:[0-9]+]] res2: i32 [storage=automatic] = read<i32>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE_res3:[0-9]+]] res3: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_res2]]), const<i32>(2));
// DEFAULT-NEXT:         read<i32>(%[[VALUE_ovf]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_res]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_x_2:[0-9]+]] x: i8, %[[VALUE_y_2:[0-9]+]] y: i64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_2:[0-9]+]] res: i16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ovf_2:[0-9]+]] ovf: i32 [storage=automatic] = from_bool<i32, reason=assign>(overflow_mul<bool>(read<i8>(%[[VALUE_x_2]]), read<i64>(%[[VALUE_y_2]]), deref(addr_of<ptr<i16>>(%[[VALUE_res_2]]))));
// DEFAULT-NEXT:         let %[[VALUE_res2_2:[0-9]+]] res2: i32 [storage=automatic] = widen<i32, reason=assign>(read<i16>(%[[VALUE_res_2]]));
// DEFAULT-NEXT:         let %[[VALUE_res3_2:[0-9]+]] res3: i32 [storage=automatic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE_res2_2]]), const<i32>(2));
// DEFAULT-NEXT:         read<i32>(%[[VALUE_ovf_2]]);
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i16>(%[[VALUE_res_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3(%[[VALUE_x_3:[0-9]+]] x: i8, %[[VALUE_y_3:[0-9]+]] y: u16, %[[VALUE_ovf_3:[0-9]+]] ovf: ptr<i32>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_3:[0-9]+]] res: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ovf_3]])), from_bool<i32, reason=assign>(overflow_add<bool>(read<i8>(%[[VALUE_x_3]]), read<u16>(%[[VALUE_y_3]]), deref(addr_of<ptr<i32>>(%[[VALUE_res_3]])))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_res_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4(%[[VALUE_x_4:[0-9]+]] x: i64, %[[VALUE_y_4:[0-9]+]] y: i64, %[[VALUE_ovf_4:[0-9]+]] ovf: ptr<i32>) -> i64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_4:[0-9]+]] res: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i64 [synthetic] = read<i64>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i64 [synthetic] = and<i64>(read<i64>(%[[VALUE0]]), widen<i64, reason=usual_arith>(const<i32>(65535)));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x_4]], read<i64>(%[[VALUE1]]));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_y_4]], sub<i64, overflow=ub>(and<i64>(read<i64>(%[[VALUE_y_4]]), widen<i64, reason=usual_arith>(const<i32>(65535))), widen<i64, reason=usual_arith>(const<i32>(32768))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ovf_4]])), from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%[[VALUE_x_4]]), read<i64>(%[[VALUE_y_4]]), deref(addr_of<ptr<i64>>(%[[VALUE_res_4]])))));
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_res_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn5:[0-9]+]] @fn5(%[[VALUE_x_5:[0-9]+]] x: i64, %[[VALUE_y_5:[0-9]+]] y: i64, %[[VALUE_ovf_5:[0-9]+]] ovf: ptr<i32>) -> i8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_5:[0-9]+]] res: i8 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(%[[VALUE_x_5]], add<i64, overflow=ub>(and<i64>(read<i64>(%[[VALUE_x_5]]), widen<i64, reason=usual_arith>(const<i32>(63))), widen<i64, reason=usual_arith>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4)))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_y_5]], add<i64, overflow=ub>(and<i64>(read<i64>(%[[VALUE_y_5]]), widen<i64, reason=usual_arith>(const<i32>(3))), widen<i64, reason=usual_arith>(const<i32>(5))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ovf_5]])), from_bool<i32, reason=assign>(overflow_mul<bool>(read<i64>(%[[VALUE_x_5]]), read<i64>(%[[VALUE_y_5]]), deref(addr_of<ptr<i8>>(%[[VALUE_res_5]])))));
// DEFAULT-NEXT:         return read<i8>(%[[VALUE_res_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn6:[0-9]+]] @fn6(%[[VALUE_x_6:[0-9]+]] x: u8, %[[VALUE_y_6:[0-9]+]] y: u8, %[[VALUE_ovf_6:[0-9]+]] ovf: ptr<i32>) -> u8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_6:[0-9]+]] res: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_x_6]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_6]]))), const<i32>(63)), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(66))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_y_6]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_y_6]]))), const<i32>(3)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ovf_6]])), from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%[[VALUE_x_6]]), read<u8>(%[[VALUE_y_6]]), deref(addr_of<ptr<u8>>(%[[VALUE_res_6]])))));
// DEFAULT-NEXT:         return read<u8>(%[[VALUE_res_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn7:[0-9]+]] @fn7(%[[VALUE_x_7:[0-9]+]] x: u8, %[[VALUE_y_7:[0-9]+]] y: u8, %[[VALUE_ovf_7:[0-9]+]] ovf: ptr<i32>) -> u8 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_res_7:[0-9]+]] res: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_x_7]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_x_7]]))), const<i32>(15)), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(15))))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_y_7]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_y_7]]))), const<i32>(3)), const<i32>(16)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_ovf_7]])), from_bool<i32, reason=assign>(overflow_add<bool>(read<u8>(%[[VALUE_x_7]]), read<u8>(%[[VALUE_y_7]]), deref(addr_of<ptr<u8>>(%[[VALUE_res_7]])))));
// DEFAULT-NEXT:         return read<u8>(%[[VALUE_res_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_ovf_8:[0-9]+]] ovf: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, u32) -> i32>(%[[VALUE_fn1]], neg<i32, overflow=ub>(const<i32>(10)), reinterpret<u32, reason=arg, fits=always>(const<i32>(2147483647))), reinterpret<i32, reason=explicit, fits=unknown>(sub<u32, overflow=wrap>(neg<u32, overflow=wrap>(const<u32>(10)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i32>(call<i32, signature=fn(i8, i64) -> i32>(%[[VALUE_fn2]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0))), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE3]], ne<i32>(call<i32, signature=fn(i8, i64) -> i32>(%[[VALUE_fn2]], truncate<i8, reason=arg, fits=always>(const<i32>(32)), widen<i64, reason=arg>(const<i32>(16383))), widen<i32, reason=promotion>(reinterpret<i16, reason=explicit, fits=unknown>(truncate<u16, reason=explicit, fits=unknown>(const<u64>(524256))))));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(call<i32, signature=fn(i8, u16, ptr<i32>) -> i32>(%[[VALUE_fn3]], truncate<i8, reason=arg, fits=always>(const<i32>(127)), reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))), addr_of<ptr<i32>>(%[[VALUE_ovf_8]])), add<i32, overflow=ub>(const<i32>(127), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u16, reason=explicit, fits=unknown>(truncate<i16, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))))), ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(call<i32, signature=fn(i8, u16, ptr<i32>) -> i32>(%[[VALUE_fn3]], truncate<i8, reason=arg, fits=unknown>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%[[VALUE_ovf_8]])), sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(127)), const<i32>(1))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE4]]), ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i64>(call<i64, signature=fn(i64, i64, ptr<i32>) -> i64>(%[[VALUE_fn4]], widen<i64, reason=arg>(const<i32>(65535)), widen<i64, reason=arg>(const<i32>(0)), addr_of<ptr<i32>>(%[[VALUE_ovf_8]])), mul<i64, overflow=ub>(const<i64>(65535), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(32768))))), ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i64, i64, ptr<i32>) -> i8>(%[[VALUE_fn5]], widen<i64, reason=arg>(const<i32>(0)), widen<i64, reason=arg>(const<i32>(0)), addr_of<ptr<i32>>(%[[VALUE_ovf_8]]))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4)), const<i32>(5))))), not<bool>(ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(i64, i64, ptr<i32>) -> i8>(%[[VALUE_fn5]], widen<i64, reason=arg>(const<i32>(63)), widen<i64, reason=arg>(const<i32>(3)), addr_of<ptr<i32>>(%[[VALUE_ovf_8]]))), widen<i32, reason=promotion>(truncate<i8, reason=explicit, fits=unknown>(mul<i32, overflow=ub>(add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(127), const<i32>(4)), const<i32>(63)), const<i32>(8))))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE5]]), not<bool>(ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%[[VALUE_fn6]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%[[VALUE_ovf_8]])))), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0)))))), const<i32>(66))), ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%[[VALUE_fn6]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(63))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3))), addr_of<ptr<i32>>(%[[VALUE_ovf_8]])))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(not<i32>(const<i32>(0))))))));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE6]]), ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%[[VALUE_fn7]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(0))), addr_of<ptr<i32>>(%[[VALUE_ovf_8]])))), const<i32>(0)), not<bool>(ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0))))
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(u8, u8, ptr<i32>) -> u8>(%[[VALUE_fn7]], reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(63))), reinterpret<u8, reason=arg, fits=unknown>(truncate<i8, reason=arg, fits=always>(const<i32>(3))), addr_of<ptr<i32>>(%[[VALUE_ovf_8]])))), const<i32>(18)));
// DEFAULT-NEXT:         if logical_or<bool>(read<bool>(%[[VALUE7]]), not<bool>(ne<i32>(read<i32>(%[[VALUE_ovf_8]]), const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
