/* Test C23 variadic functions with no named parameters, or last named
   parameter with a declaration not allowed in C17.  Execution tests.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */

#include <stdarg.h>

extern void abort (void);
extern void exit (int);
struct s { char c[1000]; };

struct s
f (...)
{
  va_list ap;
  va_start (ap);
  double r = va_arg (ap, int);
  r += va_arg (ap, double);
  r += va_arg (ap, int);
  r += va_arg (ap, double);
  va_end (ap);
  struct s ret = {};
  ret.c[0] = r;
  ret.c[999] = 42;
  return ret;
}

struct s
g (...)
{
  va_list ap;
  va_start (ap, random ! ignored, ignored ** text);	/* { dg-warning "'va_start' macro used with additional arguments other than identifier of the last named argument" } */
  for (int i = 0; i < 10; i++)
    if (va_arg (ap, double) != i)
      abort ();
  va_end (ap);
  struct s ret = {};
  ret.c[0] = 17;
  ret.c[999] = 58;
  return ret;
}

struct s
h1 (register int x, ...)
{
  va_list ap;
  va_start (ap);
  for (int i = 0; i < 10; i++)
    {
      if (va_arg (ap, double) != i)
	abort ();
      i++;
      if (va_arg (ap, int) != i)
	abort ();
    }
  va_end (ap);
  struct s ret = {};
  ret.c[0] = 32;
  ret.c[999] = 95;
  return ret;
}

struct s
h2 (int x(), ...)
{
  va_list ap;
  va_start (ap);
  for (int i = 0; i < 10; i++)
    {
      if (va_arg (ap, double) != i)
	abort ();
      i++;
      if (va_arg (ap, int) != i)
	abort ();
    }
  va_end (ap);
  struct s ret = {};
  ret.c[0] = 5;
  ret.c[999] = 125;
  return ret;
}

struct s
h3 (int x[10], ...)
{
  va_list ap;
  va_start (ap);
  for (int i = 0; i < 10; i++)
    {
      if (va_arg (ap, double) != i)
	abort ();
      i++;
      if (va_arg (ap, int) != i)
	abort ();
    }
  va_end (ap);
  struct s ret = {};
  ret.c[0] = 8;
  ret.c[999] = 12;
  return ret;
}

struct s
h4 (char x, ...)
{
  va_list ap;
  va_start (ap);
  for (int i = 0; i < 10; i++)
    {
      if (va_arg (ap, double) != i)
	abort ();
      i++;
      if (va_arg (ap, int) != i)
	abort ();
    }
  va_end (ap);
  struct s ret = {};
  ret.c[0] = 18;
  ret.c[999] = 28;
  return ret;
}

struct s
h5 (float x, ...)
{
  va_list ap;
  va_start (ap);
  for (int i = 0; i < 10; i++)
    {
      if (va_arg (ap, double) != i)
	abort ();
      i++;
      if (va_arg (ap, int) != i)
	abort ();
    }
  va_end (ap);
  struct s ret = {};
  ret.c[0] = 38;
  ret.c[999] = 48;
  return ret;
}

struct s
h6 (volatile long x, ...)
{
  va_list ap;
  va_start (ap);
  for (int i = 0; i < 10; i++)
    {
      if (va_arg (ap, double) != i)
	abort ();
      i++;
      if (va_arg (ap, int) != i)
	abort ();
    }
  va_end (ap);
  struct s ret = {};
  ret.c[0] = 58;
  ret.c[999] = 68;
  return ret;
}

struct s
h7 (volatile struct s x, ...)
{
  va_list ap;
  va_start (ap);
  for (int i = 0; i < 10; i++)
    {
      if (va_arg (ap, double) != i)
	abort ();
      i++;
      if (va_arg (ap, int) != i)
	abort ();
    }
  va_end (ap);
  struct s ret = {};
  ret.c[0] = 78;
  ret.c[999] = 88;
  return ret;
}

int
main ()
{
  struct s x = f (1, 2.0, 3, 4.0);
  if (x.c[0] != 10 || x.c[999] != 42)
    abort ();
  x = g (0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
  if (x.c[0] != 17 || x.c[999] != 58)
    abort ();
  x = g (0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
  if (x.c[0] != 17 || x.c[999] != 58)
    abort ();
  x = h1 (0, 0.0, 1, 2.0, 3, 4.0, 5, 6.0, 7, 8.0, 9);
  if (x.c[0] != 32 || x.c[999] != 95)
    abort ();
  x = h2 (0, 0.0, 1, 2.0, 3, 4.0, 5, 6.0, 7, 8.0, 9);
  if (x.c[0] != 5 || x.c[999] != 125)
    abort ();
  x = h3 (0, 0.0, 1, 2.0, 3, 4.0, 5, 6.0, 7, 8.0, 9);
  if (x.c[0] != 8 || x.c[999] != 12)
    abort ();
  x = h4 (0, 0.0, 1, 2.0, 3, 4.0, 5, 6.0, 7, 8.0, 9);
  if (x.c[0] != 18 || x.c[999] != 28)
    abort ();
  x = h5 (0, 0.0, 1, 2.0, 3, 4.0, 5, 6.0, 7, 8.0, 9);
  if (x.c[0] != 38 || x.c[999] != 48)
    abort ();
  x = h6 (0, 0.0, 1, 2.0, 3, 4.0, 5, 6.0, 7, 8.0, 9);
  if (x.c[0] != 58 || x.c[999] != 68)
    abort ();
  x = h7 ((struct s) {}, 0.0, 1, 2.0, 3, 4.0, 5, 6.0, 7, 8.0, 9);
  if (x.c[0] != 78 || x.c[999] != 88)
    abort ();
  exit (0);
}

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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list [c="__gnuc_va_list"];
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 c: array<i8, 1000>;
// DEFAULT-NEXT:     } [size=1000, align=1, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: f64 [storage=automatic] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(va_arg<i32>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: f64 [synthetic] = va_arg<f64>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=off>(read<f64>(%[[VALUE2]]), read<f64>(%[[VALUE1]]));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_r]], read<f64>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=off>(read<f64>(%[[VALUE5]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE4]])));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_r]], read<f64>(%[[VALUE6]]));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: f64 [synthetic] = va_arg<f64>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=off>(read<f64>(%[[VALUE8]]), read<f64>(%[[VALUE7]]));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_r]], read<f64>(%[[VALUE9]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret]])), const<i32>(0))), float_to_int<i8, reason=assign, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_r]])));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g(...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             counted: %[[VALUE_i]] in const<i32>(0)..const<i32>(10)
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<f64, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_2]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]])))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         let %[[VALUE_ret_2:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_2]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_2]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(58)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h1:[0-9]+]] @h1(%[[VALUE_x:[0-9]+]] x: i32, ...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<f64, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_3]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i_2]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                     let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%[[VALUE_ap_3]]), read<i32>(%[[VALUE_i_2]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE_ret_3:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_3]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(32)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_3]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(95)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h2:[0-9]+]] @h2(%[[VALUE_x_2:[0-9]+]] x: ptr<fn() -> i32>, ...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] [c="struct s(int(void), ...)"] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         for %[[VALUE18:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<f64, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_4]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i_3]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                     let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%[[VALUE_ap_4]]), read<i32>(%[[VALUE_i_3]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         let %[[VALUE_ret_4:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_4]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_4]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(125)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h3:[0-9]+]] @h3(%[[VALUE_x_3:[0-9]+]] x: ptr<i32> [array=10], ...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] [c="struct s(int[10], ...)"] {
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         for %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<f64, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_5]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i_4]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                     let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%[[VALUE_ap_5]]), read<i32>(%[[VALUE_i_4]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         let %[[VALUE_ret_5:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_5]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(8)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_5]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h4:[0-9]+]] @h4(%[[VALUE_x_4:[0-9]+]] x: i8, ...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         for %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_5:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_5]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE29]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<f64, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_6]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i_5]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_5]]);
// DEFAULT-NEXT:                     let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_5]], read<i32>(%[[VALUE32]]));
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%[[VALUE_ap_6]]), read<i32>(%[[VALUE_i_5]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         let %[[VALUE_ret_6:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_6]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(18)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_6]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(28)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h5:[0-9]+]] @h5(%[[VALUE_x_5:[0-9]+]] x: f32, ...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap_7:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         for %[[VALUE33:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_6:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_6]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_6]]);
// DEFAULT-NEXT:                 let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_6]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<f64, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_7]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i_6]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_6]]);
// DEFAULT-NEXT:                     let %[[VALUE37:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_6]], read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%[[VALUE_ap_7]]), read<i32>(%[[VALUE_i_6]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE_ret_7:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_7]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(38)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_7]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(48)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h6:[0-9]+]] @h6(%[[VALUE_x_6:[0-9]+]] x: volatile i64, ...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] [c="struct s(volatile long, ...)"] {
// DEFAULT-NEXT:         let %[[VALUE_ap_8:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         for %[[VALUE38:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_7:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_7]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_7]]);
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE39]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_7]], read<i32>(%[[VALUE40]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<f64, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_8]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i_7]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE41:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_7]]);
// DEFAULT-NEXT:                     let %[[VALUE42:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE41]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_7]], read<i32>(%[[VALUE42]]));
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%[[VALUE_ap_8]]), read<i32>(%[[VALUE_i_7]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE_ret_8:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_8]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(58)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_8]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(68)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_h7:[0-9]+]] @h7(%[[VALUE_x_7:[0-9]+]] x: volatile @type[[TYPE_s]], ...) -> @type[[TYPE_s]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] [c="struct s(volatile struct s, ...)"] {
// DEFAULT-NEXT:         let %[[VALUE_ap_9:[0-9]+]] ap: va_list [storage=automatic] [c="va_list"];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         for %[[VALUE43:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_8:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_8]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_8]]);
// DEFAULT-NEXT:                 let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_8]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<f64, exceptions=observable>(va_arg<f64>(%[[VALUE_ap_9]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i_8]])))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_8]]);
// DEFAULT-NEXT:                     let %[[VALUE47:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_8]], read<i32>(%[[VALUE47]]));
// DEFAULT-NEXT:                     if ne<i32>(va_arg<i32>(%[[VALUE_ap_9]]), read<i32>(%[[VALUE_i_8]]))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         let %[[VALUE_ret_9:[0-9]+]] ret: @type[[TYPE_s]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>();
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_9]])), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(78)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_ret_9]])), const<i32>(999))), truncate<i8, reason=assign, fits=always>(const<i32>(88)));
// DEFAULT-NEXT:         return copy<@type[[TYPE_s]], reason=return>(read<@type[[TYPE_s]]>(%[[VALUE_ret_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_8:[0-9]+]] x: @type[[TYPE_s]] [storage=automatic] = copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_f]], const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(10)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(42)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_g]], const<f64>(0.0), const<f64>(1.0), const<f64>(2.0), const<f64>(3.0), const<f64>(4.0), const<f64>(5.0), const<f64>(6.0), const<f64>(7.0), const<f64>(8.0), const<f64>(9.0))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(17)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(58)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_g]], float_widen<f64, reason=vararg>(const<f32>(0.0)), float_widen<f64, reason=vararg>(const<f32>(1.0)), float_widen<f64, reason=vararg>(const<f32>(2.0)), float_widen<f64, reason=vararg>(const<f32>(3.0)), float_widen<f64, reason=vararg>(const<f32>(4.0)), float_widen<f64, reason=vararg>(const<f32>(5.0)), float_widen<f64, reason=vararg>(const<f32>(6.0)), float_widen<f64, reason=vararg>(const<f32>(7.0)), float_widen<f64, reason=vararg>(const<f32>(8.0)), float_widen<f64, reason=vararg>(const<f32>(9.0)))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(17)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(58)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(i32, ...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_h1]], const<i32>(0), const<f64>(0.0), const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0), const<i32>(5), const<f64>(6.0), const<i32>(7), const<f64>(8.0), const<i32>(9))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(32)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(95)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(ptr<fn() -> i32>, ...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_h2]], null<ptr<fn() -> i32>>, const<f64>(0.0), const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0), const<i32>(5), const<f64>(6.0), const<i32>(7), const<f64>(8.0), const<i32>(9))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(5)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(125)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(ptr<i32>, ...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_h3]], null<ptr<i32>>, const<f64>(0.0), const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0), const<i32>(5), const<f64>(6.0), const<i32>(7), const<f64>(8.0), const<i32>(9))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(8)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(i8, ...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_h4]], truncate<i8, reason=arg, fits=always>(const<i32>(0)), const<f64>(0.0), const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0), const<i32>(5), const<f64>(6.0), const<i32>(7), const<f64>(8.0), const<i32>(9))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(18)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(28)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(f32, ...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_h5]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), const<f64>(0.0), const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0), const<i32>(5), const<f64>(6.0), const<i32>(7), const<f64>(8.0), const<i32>(9))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(38)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(48)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(i64, ...) -> @type[[TYPE_s]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_h6]], widen<i64, reason=arg>(const<i32>(0)), const<f64>(0.0), const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0), const<i32>(5), const<f64>(6.0), const<i32>(7), const<f64>(8.0), const<i32>(9))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(58)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(68)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<@type[[TYPE_s]]>(%[[VALUE_x_8]], copy<@type[[TYPE_s]], reason=assign>(call<@type[[TYPE_s]], signature=fn(@type[[TYPE_s]], ...) -> @type[[TYPE_s]], abi=sysv64(native_c, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_h7]], copy<@type[[TYPE_s]], reason=arg>(read<@type[[TYPE_s]]>(compound_literal %[[VALUE48:[0-9]+]] [storage=automatic] = aggregate<@type[[TYPE_s]], zero_fill=true>())), const<f64>(0.0), const<i32>(1), const<f64>(2.0), const<i32>(3), const<f64>(4.0), const<i32>(5), const<f64>(6.0), const<i32>(7), const<f64>(8.0), const<i32>(9))));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(0))))), const<i32>(78)), ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1000)>(field0(%[[VALUE_x_8]])), const<i32>(999))))), const<i32>(88)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
