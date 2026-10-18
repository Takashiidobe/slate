/* Test C23 variadic functions with no named parameters, or last named
   parameter with a declaration not allowed in C17.  Execution tests.  */
/* { dg-do run } */
/* { dg-options "-O2 -std=c23 -pedantic-errors" } */

#include <stdarg.h>

#ifdef __AVR__
/* AVR doesn't have that much stack... */
struct S { int a[500]; };
#else
struct S { int a[1024]; };
#endif

int
f1 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  va_end (ap);
  return r;
}

int
f2 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  return r;
}

int
f3 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  return r;
}

int
f4 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  return r;
}

int
f5 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  return r;
}

int
f6 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  return r;
}

int
f7 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  return r;
}

int
f8 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  return r;
}

struct S
s1 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  va_end (ap);
  struct S s = {};
  s.a[0] = r;
  return s;
}

struct S
s2 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  struct S s = {};
  s.a[0] = r;
  return s;
}

struct S
s3 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  struct S s = {};
  s.a[0] = r;
  return s;
}

struct S
s4 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  struct S s = {};
  s.a[0] = r;
  return s;
}

struct S
s5 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  struct S s = {};
  s.a[0] = r;
  return s;
}

struct S
s6 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  struct S s = {};
  s.a[0] = r;
  return s;
}

struct S
s7 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  struct S s = {};
  s.a[0] = r;
  return s;
}

struct S
s8 (...)
{
  int r = 0;
  va_list ap;
  va_start (ap);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  r += va_arg (ap, int);
  va_end (ap);
  struct S s = {};
  s.a[0] = r;
  return s;
}

int
b1 (void)
{
  return f8 (1, 2, 3, 4, 5, 6, 7, 8);
}

int
b2 (void)
{
  return s8 (1, 2, 3, 4, 5, 6, 7, 8).a[0];
}

int
main ()
{
  if (f1 (1) != 1 || f2 (1, 2) != 3 || f3 (1, 2, 3) != 6
      || f4 (1, 2, 3, 4) != 10 || f5 (1, 2, 3, 4, 5) != 15
      || f6 (1, 2, 3, 4, 5, 6) != 21 || f7 (1, 2, 3, 4, 5, 6, 7) != 28
      || f8 (1, 2, 3, 4, 5, 6, 7, 8) != 36)
    __builtin_abort ();
  if (s1 (1).a[0] != 1 || s2 (1, 2).a[0] != 3 || s3 (1, 2, 3).a[0] != 6
      || s4 (1, 2, 3, 4).a[0] != 10 || s5 (1, 2, 3, 4, 5).a[0] != 15
      || s6 (1, 2, 3, 4, 5, 6).a[0] != 21
      || s7 (1, 2, 3, 4, 5, 6, 7).a[0] != 28
      || s8 (1, 2, 3, 4, 5, 6, 7, 8).a[0] != 36)
    __builtin_abort ();
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
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: array<i32, 1024>;
// DEFAULT-NEXT:     } [size=4096, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_2]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_3]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_3]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_3]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_3]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_3]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_3]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_4]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), read<i32>(%[[VALUE18]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_4]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_4]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_4]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_4]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE25]]), read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_4]], read<i32>(%[[VALUE26]]));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_4]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_4]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_5]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), read<i32>(%[[VALUE30]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_5]], read<i32>(%[[VALUE32]]));
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_5]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_5]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_5]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE37]]), read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_5]], read<i32>(%[[VALUE38]]));
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_5]]);
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_5]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_5]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE43]]), read<i32>(%[[VALUE42]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_5]], read<i32>(%[[VALUE44]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_6:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_6]], read<i32>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE49]]), read<i32>(%[[VALUE48]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_6]], read<i32>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE52]]), read<i32>(%[[VALUE51]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_6]], read<i32>(%[[VALUE53]]));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE55]]), read<i32>(%[[VALUE54]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_6]], read<i32>(%[[VALUE56]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE58]]), read<i32>(%[[VALUE57]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_6]], read<i32>(%[[VALUE59]]));
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE61]]), read<i32>(%[[VALUE60]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_6]], read<i32>(%[[VALUE62]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_7:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_7:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE64]]), read<i32>(%[[VALUE63]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_7]], read<i32>(%[[VALUE65]]));
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE67]]), read<i32>(%[[VALUE66]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_7]], read<i32>(%[[VALUE68]]));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE70]]), read<i32>(%[[VALUE69]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_7]], read<i32>(%[[VALUE71]]));
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE73]]), read<i32>(%[[VALUE72]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_7]], read<i32>(%[[VALUE74]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE76]]), read<i32>(%[[VALUE75]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_7]], read<i32>(%[[VALUE77]]));
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE79]]), read<i32>(%[[VALUE78]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_7]], read<i32>(%[[VALUE80]]));
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE82]]), read<i32>(%[[VALUE81]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_7]], read<i32>(%[[VALUE83]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_8:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_8:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE85]]), read<i32>(%[[VALUE84]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_8]], read<i32>(%[[VALUE86]]));
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE88]]), read<i32>(%[[VALUE87]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_8]], read<i32>(%[[VALUE89]]));
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE91]]), read<i32>(%[[VALUE90]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_8]], read<i32>(%[[VALUE92]]));
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE94:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE94]]), read<i32>(%[[VALUE93]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_8]], read<i32>(%[[VALUE95]]));
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE97]]), read<i32>(%[[VALUE96]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_8]], read<i32>(%[[VALUE98]]));
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE100]]), read<i32>(%[[VALUE99]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_8]], read<i32>(%[[VALUE101]]));
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE103]]), read<i32>(%[[VALUE102]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_8]], read<i32>(%[[VALUE104]]));
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE106]]), read<i32>(%[[VALUE105]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_8]], read<i32>(%[[VALUE107]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s1:[0-9]+]] @s1(...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_9:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_9:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_9]]);
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE109]]), read<i32>(%[[VALUE108]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_9]], read<i32>(%[[VALUE110]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%[[VALUE_s]])), const<i32>(0))), read<i32>(%[[VALUE_r_9]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_s]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s2:[0-9]+]] @s2(...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_10:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_10:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_10]]);
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE112]]), read<i32>(%[[VALUE111]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_10]], read<i32>(%[[VALUE113]]));
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_10]]);
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE115]]), read<i32>(%[[VALUE114]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_10]], read<i32>(%[[VALUE116]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%[[VALUE_s_2]])), const<i32>(0))), read<i32>(%[[VALUE_r_10]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_s_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s3:[0-9]+]] @s3(...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_11:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_11:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         let %[[VALUE118:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_11]]);
// DEFAULT-NEXT:         let %[[VALUE119:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE118]]), read<i32>(%[[VALUE117]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_11]], read<i32>(%[[VALUE119]]));
// DEFAULT-NEXT:         let %[[VALUE120:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         let %[[VALUE121:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_11]]);
// DEFAULT-NEXT:         let %[[VALUE122:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE121]]), read<i32>(%[[VALUE120]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_11]], read<i32>(%[[VALUE122]]));
// DEFAULT-NEXT:         let %[[VALUE123:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         let %[[VALUE124:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_11]]);
// DEFAULT-NEXT:         let %[[VALUE125:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE124]]), read<i32>(%[[VALUE123]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_11]], read<i32>(%[[VALUE125]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         let %[[VALUE_s_3:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%[[VALUE_s_3]])), const<i32>(0))), read<i32>(%[[VALUE_r_11]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_s_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s4:[0-9]+]] @s4(...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_12:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_12:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         let %[[VALUE126:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         let %[[VALUE127:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_12]]);
// DEFAULT-NEXT:         let %[[VALUE128:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE127]]), read<i32>(%[[VALUE126]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_12]], read<i32>(%[[VALUE128]]));
// DEFAULT-NEXT:         let %[[VALUE129:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_12]]);
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE130]]), read<i32>(%[[VALUE129]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_12]], read<i32>(%[[VALUE131]]));
// DEFAULT-NEXT:         let %[[VALUE132:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         let %[[VALUE133:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_12]]);
// DEFAULT-NEXT:         let %[[VALUE134:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE133]]), read<i32>(%[[VALUE132]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_12]], read<i32>(%[[VALUE134]]));
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_12]]);
// DEFAULT-NEXT:         let %[[VALUE137:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE136]]), read<i32>(%[[VALUE135]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_12]], read<i32>(%[[VALUE137]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         let %[[VALUE_s_4:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%[[VALUE_s_4]])), const<i32>(0))), read<i32>(%[[VALUE_r_12]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_s_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s5:[0-9]+]] @s5(...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_13:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_13:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         let %[[VALUE139:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_13]]);
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE139]]), read<i32>(%[[VALUE138]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_13]], read<i32>(%[[VALUE140]]));
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_13]]);
// DEFAULT-NEXT:         let %[[VALUE143:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE142]]), read<i32>(%[[VALUE141]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_13]], read<i32>(%[[VALUE143]]));
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         let %[[VALUE145:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_13]]);
// DEFAULT-NEXT:         let %[[VALUE146:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE145]]), read<i32>(%[[VALUE144]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_13]], read<i32>(%[[VALUE146]]));
// DEFAULT-NEXT:         let %[[VALUE147:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_13]]);
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE148]]), read<i32>(%[[VALUE147]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_13]], read<i32>(%[[VALUE149]]));
// DEFAULT-NEXT:         let %[[VALUE150:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         let %[[VALUE151:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_13]]);
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE151]]), read<i32>(%[[VALUE150]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_13]], read<i32>(%[[VALUE152]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         let %[[VALUE_s_5:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%[[VALUE_s_5]])), const<i32>(0))), read<i32>(%[[VALUE_r_13]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_s_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s6:[0-9]+]] @s6(...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_14:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_14:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         let %[[VALUE153:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_14]]);
// DEFAULT-NEXT:         let %[[VALUE155:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE154]]), read<i32>(%[[VALUE153]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_14]], read<i32>(%[[VALUE155]]));
// DEFAULT-NEXT:         let %[[VALUE156:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         let %[[VALUE157:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_14]]);
// DEFAULT-NEXT:         let %[[VALUE158:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE157]]), read<i32>(%[[VALUE156]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_14]], read<i32>(%[[VALUE158]]));
// DEFAULT-NEXT:         let %[[VALUE159:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         let %[[VALUE160:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_14]]);
// DEFAULT-NEXT:         let %[[VALUE161:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE160]]), read<i32>(%[[VALUE159]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_14]], read<i32>(%[[VALUE161]]));
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         let %[[VALUE163:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_14]]);
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE163]]), read<i32>(%[[VALUE162]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_14]], read<i32>(%[[VALUE164]]));
// DEFAULT-NEXT:         let %[[VALUE165:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_14]]);
// DEFAULT-NEXT:         let %[[VALUE167:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE166]]), read<i32>(%[[VALUE165]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_14]], read<i32>(%[[VALUE167]]));
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         let %[[VALUE169:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_14]]);
// DEFAULT-NEXT:         let %[[VALUE170:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE169]]), read<i32>(%[[VALUE168]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_14]], read<i32>(%[[VALUE170]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         let %[[VALUE_s_6:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%[[VALUE_s_6]])), const<i32>(0))), read<i32>(%[[VALUE_r_14]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_s_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s7:[0-9]+]] @s7(...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_15:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_15:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE171:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE172:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_15]]);
// DEFAULT-NEXT:         let %[[VALUE173:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE172]]), read<i32>(%[[VALUE171]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_15]], read<i32>(%[[VALUE173]]));
// DEFAULT-NEXT:         let %[[VALUE174:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE175:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_15]]);
// DEFAULT-NEXT:         let %[[VALUE176:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE175]]), read<i32>(%[[VALUE174]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_15]], read<i32>(%[[VALUE176]]));
// DEFAULT-NEXT:         let %[[VALUE177:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE178:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_15]]);
// DEFAULT-NEXT:         let %[[VALUE179:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE178]]), read<i32>(%[[VALUE177]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_15]], read<i32>(%[[VALUE179]]));
// DEFAULT-NEXT:         let %[[VALUE180:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE181:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_15]]);
// DEFAULT-NEXT:         let %[[VALUE182:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE181]]), read<i32>(%[[VALUE180]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_15]], read<i32>(%[[VALUE182]]));
// DEFAULT-NEXT:         let %[[VALUE183:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE184:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_15]]);
// DEFAULT-NEXT:         let %[[VALUE185:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE184]]), read<i32>(%[[VALUE183]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_15]], read<i32>(%[[VALUE185]]));
// DEFAULT-NEXT:         let %[[VALUE186:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE187:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_15]]);
// DEFAULT-NEXT:         let %[[VALUE188:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE187]]), read<i32>(%[[VALUE186]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_15]], read<i32>(%[[VALUE188]]));
// DEFAULT-NEXT:         let %[[VALUE189:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE190:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_15]]);
// DEFAULT-NEXT:         let %[[VALUE191:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE190]]), read<i32>(%[[VALUE189]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_15]], read<i32>(%[[VALUE191]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_15]]);
// DEFAULT-NEXT:         let %[[VALUE_s_7:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%[[VALUE_s_7]])), const<i32>(0))), read<i32>(%[[VALUE_r_15]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_s_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_s8:[0-9]+]] @s8(...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64() -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_16:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_ap_16:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE192:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE193:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_16]]);
// DEFAULT-NEXT:         let %[[VALUE194:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE193]]), read<i32>(%[[VALUE192]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_16]], read<i32>(%[[VALUE194]]));
// DEFAULT-NEXT:         let %[[VALUE195:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE196:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_16]]);
// DEFAULT-NEXT:         let %[[VALUE197:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE196]]), read<i32>(%[[VALUE195]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_16]], read<i32>(%[[VALUE197]]));
// DEFAULT-NEXT:         let %[[VALUE198:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE199:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_16]]);
// DEFAULT-NEXT:         let %[[VALUE200:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE199]]), read<i32>(%[[VALUE198]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_16]], read<i32>(%[[VALUE200]]));
// DEFAULT-NEXT:         let %[[VALUE201:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE202:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_16]]);
// DEFAULT-NEXT:         let %[[VALUE203:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE202]]), read<i32>(%[[VALUE201]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_16]], read<i32>(%[[VALUE203]]));
// DEFAULT-NEXT:         let %[[VALUE204:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE205:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_16]]);
// DEFAULT-NEXT:         let %[[VALUE206:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE205]]), read<i32>(%[[VALUE204]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_16]], read<i32>(%[[VALUE206]]));
// DEFAULT-NEXT:         let %[[VALUE207:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE208:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_16]]);
// DEFAULT-NEXT:         let %[[VALUE209:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE208]]), read<i32>(%[[VALUE207]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_16]], read<i32>(%[[VALUE209]]));
// DEFAULT-NEXT:         let %[[VALUE210:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE211:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_16]]);
// DEFAULT-NEXT:         let %[[VALUE212:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE211]]), read<i32>(%[[VALUE210]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_16]], read<i32>(%[[VALUE212]]));
// DEFAULT-NEXT:         let %[[VALUE213:[0-9]+]]: i32 [synthetic] = va_arg<i32>(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE214:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_r_16]]);
// DEFAULT-NEXT:         let %[[VALUE215:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE214]]), read<i32>(%[[VALUE213]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r_16]], read<i32>(%[[VALUE215]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_16]]);
// DEFAULT-NEXT:         let %[[VALUE_s_8:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic] = aggregate<@type[[TYPE_S]], zero_fill=true>();
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(%[[VALUE_s_8]])), const<i32>(0))), read<i32>(%[[VALUE_r_16]]));
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_s_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_b1:[0-9]+]] @b1() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(...) -> i32>(%[[VALUE_f8]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_b2:[0-9]+]] @b2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE216:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_s8]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8)))), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE217:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(...) -> i32>(%[[VALUE_f1]], const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%[[VALUE217]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE217]], ne<i32>(call<i32, signature=fn(...) -> i32>(%[[VALUE_f2]], const<i32>(1), const<i32>(2)), const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE218:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE217]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE218]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE218]], ne<i32>(call<i32, signature=fn(...) -> i32>(%[[VALUE_f3]], const<i32>(1), const<i32>(2), const<i32>(3)), const<i32>(6)));
// DEFAULT-NEXT:         let %[[VALUE219:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE218]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE219]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE219]], ne<i32>(call<i32, signature=fn(...) -> i32>(%[[VALUE_f4]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4)), const<i32>(10)));
// DEFAULT-NEXT:         let %[[VALUE220:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE219]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE220]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE220]], ne<i32>(call<i32, signature=fn(...) -> i32>(%[[VALUE_f5]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5)), const<i32>(15)));
// DEFAULT-NEXT:         let %[[VALUE221:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE220]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE221]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE221]], ne<i32>(call<i32, signature=fn(...) -> i32>(%[[VALUE_f6]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6)), const<i32>(21)));
// DEFAULT-NEXT:         let %[[VALUE222:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE221]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE222]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE222]], ne<i32>(call<i32, signature=fn(...) -> i32>(%[[VALUE_f7]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7)), const<i32>(28)));
// DEFAULT-NEXT:         let %[[VALUE223:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE222]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE223]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE223]], ne<i32>(call<i32, signature=fn(...) -> i32>(%[[VALUE_f8]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8)), const<i32>(36)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE223]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         let %[[VALUE224:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE225:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar) -> native_c>(%[[VALUE_s1]], const<i32>(1)))), const<i32>(0)))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%[[VALUE224]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE224]], ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE226:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar) -> native_c>(%[[VALUE_s2]], const<i32>(1), const<i32>(2)))), const<i32>(0)))), const<i32>(3)));
// DEFAULT-NEXT:         let %[[VALUE227:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE224]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE227]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE227]], ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE228:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar) -> native_c>(%[[VALUE_s3]], const<i32>(1), const<i32>(2), const<i32>(3)))), const<i32>(0)))), const<i32>(6)));
// DEFAULT-NEXT:         let %[[VALUE229:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE227]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE229]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE229]], ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE230:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_s4]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4)))), const<i32>(0)))), const<i32>(10)));
// DEFAULT-NEXT:         let %[[VALUE231:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE229]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE231]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE231]], ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE232:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_s5]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5)))), const<i32>(0)))), const<i32>(15)));
// DEFAULT-NEXT:         let %[[VALUE233:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE231]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE233]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE233]], ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE234:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_s6]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6)))), const<i32>(0)))), const<i32>(21)));
// DEFAULT-NEXT:         let %[[VALUE235:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE233]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE235]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE235]], ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE236:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_s7]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7)))), const<i32>(0)))), const<i32>(28)));
// DEFAULT-NEXT:         let %[[VALUE237:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE235]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE237]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE237]], ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(1024)>(field0(temporary %[[VALUE238:[0-9]+]] = call<@type[[TYPE_S]], signature=fn(...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar) -> native_c>(%[[VALUE_s8]], const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8)))), const<i32>(0)))), const<i32>(36)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE237]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
