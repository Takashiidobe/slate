/* PR c/102989 */
/* { dg-do compile { target { bitint && exceptions } } } */
/* { dg-options "-O2 -std=gnu23 -fnon-call-exceptions -fexceptions" } */

__attribute__((noipa)) void
baz (int *p)
{
}

#if __BITINT_MAXWIDTH__ >= 575
void
foo (volatile _BitInt(575) *p, _BitInt(575) q)
{
  int a __attribute__((cleanup (baz))) = 1;
  *p = q;
}

_BitInt(575)
bar (volatile _BitInt(575) *p)
{
  int a __attribute__((cleanup (baz))) = 1;
  return *p;
}

_BitInt(575)
qux (long double l)
{
  int a __attribute__((cleanup (baz))) = 1;
  return l;
}

long double
corge (_BitInt(575) b)
{
  int a __attribute__((cleanup (baz))) = 1;
  return b;
}

_BitInt(575)
garply (_BitInt(575) x, _BitInt(575) y)
{
  int a __attribute__((cleanup (baz))) = 1;
  return x / y;
}

_BitInt(575)
waldo (_BitInt(575) x, _BitInt(575) y)
{
  int a __attribute__((cleanup (baz))) = 1;
  return x % y;
}
#endif

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
// DEFAULT-NEXT:     fn %0 @baz(%1 p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @foo(%3 p: ptr<volatile i575b>, %4 q: i575b) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 a: i32 [storage=automatic] [cleanup=baz] = const<i32>(1);
// DEFAULT-NEXT:         write<i575b, volatile>(deref(read<ptr<volatile i575b>>(%3)), read<i575b>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 p: ptr<volatile i575b>) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 a: i32 [storage=automatic] [cleanup=baz] = const<i32>(1);
// DEFAULT-NEXT:         return read<i575b, volatile>(deref(read<ptr<volatile i575b>>(%7)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @qux(%10 l: f80) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 a: i32 [storage=automatic] [cleanup=baz] = const<i32>(1);
// DEFAULT-NEXT:         return float_to_int<i575b, reason=return, out_of_range=ub, exceptions=ignore>(read<f80>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @corge(%13 b: i575b) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 a: i32 [storage=automatic] [cleanup=baz] = const<i32>(1);
// DEFAULT-NEXT:         return int_to_float<f80, reason=return, exact=false, rounding=nearest_even, exceptions=ignore>(read<i575b>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @garply(%16 x: i575b, %17 y: i575b) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 a: i32 [storage=automatic] [cleanup=baz] = const<i32>(1);
// DEFAULT-NEXT:         return div<i575b, by_zero=ub, min_by_neg_one=ub>(read<i575b>(%16), read<i575b>(%17));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @waldo(%20 x: i575b, %21 y: i575b) -> i575b [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %22 a: i32 [storage=automatic] [cleanup=baz] = const<i32>(1);
// DEFAULT-NEXT:         return rem<i575b, by_zero=ub, min_by_neg_one=ub>(read<i575b>(%20), read<i575b>(%21));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
