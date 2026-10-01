/* { dg-do compile } */
/* { dg-options "-O2 -ffast-math" } */
/* { dg-prune-output "non-standard ABI extension" } */
/* { dg-additional-options "-fno-common" { target hppa*-*-hpux* } } */
/* { dg-additional-options "-msse" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-require-effective-target int32plus } */

typedef float __m128 __attribute__ ((__vector_size__ (16)));
__m128 a, d, e;
int b;
struct dt_interpolation c;
__m128
fn1 (float p1)
{
  return (__attribute__ ((__vector_size__ (4 * sizeof 0))) float){ p1 };
}
__m128
fn2 (float p1)
{
  return fn1 (p1);
}
struct dt_interpolation
{
  int width;
};
void
fn3 (struct dt_interpolation *p1, int *p2)
{
  int i = 0, n = 0;
  while (i < 2 * p1->width)
    n = i++;
  *p2 = n;
}
void
fn4 ()
{
  __m128 f;
  fn3 (&c, &b);
  __m128 g = fn2 (1.f / b);
  e = (__m128){};
  __m128 h = e;
  for (int i = 0; i < 2 * c.width; i++)
    {
      for (; c.width;)
	f = a;
      h = f;
    }
  d = h * g;
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
// DEFAULT-NEXT:     type @type[[TYPE___m128:[0-9]+]] __m128 = vector<f32, 4>;
// DEFAULT-NEXT:     type @type[[TYPE_dt_interpolation:[0-9]+]] dt_interpolation = struct {
// DEFAULT-NEXT:         field0 width: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: vector<f32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: vector<f32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: vector<f32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: @type[[TYPE_dt_interpolation]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fn1:[0-9]+]] @fn1(%[[VALUE_p1:[0-9]+]] p1: f32) -> vector<f32, 4> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<vector<f32, 4>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=true>(index0 = read<f32>(%[[VALUE_p1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn2:[0-9]+]] @fn2(%[[VALUE_p1_2:[0-9]+]] p1: f32) -> vector<f32, 4> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<vector<f32, 4>, signature=fn(f32) -> vector<f32, 4>, abi=sysv64(scalar) -> direct>(%[[VALUE_fn1]], read<f32>(%[[VALUE_p1_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn3:[0-9]+]] @fn3(%[[VALUE_p1_3:[0-9]+]] p1: ptr<@type[[TYPE_dt_interpolation]]>, %[[VALUE_p2:[0-9]+]] p2: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] lt<i32>(read<i32>(%[[VALUE_i]]), mul<i32, overflow=ub>(const<i32>(2), read<i32>(field0(deref(read<ptr<@type[[TYPE_dt_interpolation]]>>(%[[VALUE_p1_3]]))))))
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_p2]])), read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fn4:[0-9]+]] @fn4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: vector<f32, 4> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_dt_interpolation]]>, ptr<i32>) -> void>(%[[VALUE_fn3]], addr_of<ptr<@type[[TYPE_dt_interpolation]]>>(%[[VALUE_c]]), addr_of<ptr<i32>>(%[[VALUE_b]]));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: vector<f32, 4> [storage=automatic] = call<vector<f32, 4>, signature=fn(f32) -> vector<f32, 4>, abi=sysv64(scalar) -> direct>(%[[VALUE_fn2]], div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(const<f32>(1.0), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_e]], read<vector<f32, 4>>(compound_literal %[[VALUE4:[0-9]+]] [storage=automatic] = aggregate<vector<f32, 4>, zero_fill=true>()));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: vector<f32, 4> [storage=automatic] = read<vector<f32, 4>>(%[[VALUE_e]]);
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), mul<i32, overflow=ub>(const<i32>(2), read<i32>(field0(%[[VALUE_c]]))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: ne<i32>(read<i32>(field0(%[[VALUE_c]])), const<i32>(0))
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             write<vector<f32, 4>>(%[[VALUE_f]], read<vector<f32, 4>>(%[[VALUE_a]]));
// DEFAULT-NEXT:                     write<vector<f32, 4>>(%[[VALUE_h]], read<vector<f32, 4>>(%[[VALUE_f]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<vector<f32, 4>>(%[[VALUE_d]], mul<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(read<vector<f32, 4>>(%[[VALUE_h]]), read<vector<f32, 4>>(%[[VALUE_g]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
