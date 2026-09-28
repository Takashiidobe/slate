/* { dg-require-effective-target int32plus } */

/* Test arithmetics on bitfields.  */
#ifndef T

extern void abort(void);
extern void exit(int);

#ifndef FIELDS1
#define FIELDS1
#endif
#ifndef FIELDS2
#define FIELDS2
#endif

struct {
  FIELDS1 unsigned int i : 6, j : 11, k : 15;
  FIELDS2
} b;
struct {
  FIELDS1 unsigned int i : 5, j : 1, k : 26;
  FIELDS2
} c;
struct {
  FIELDS1 unsigned int i : 16, j : 8, k : 8;
  FIELDS2
} d;

unsigned int ret1(void) { return b.i; }
unsigned int ret2(void) { return b.j; }
unsigned int ret3(void) { return b.k; }
unsigned int ret4(void) { return c.i; }
unsigned int ret5(void) { return c.j; }
unsigned int ret6(void) { return c.k; }
unsigned int ret7(void) { return d.i; }
unsigned int ret8(void) { return d.j; }
unsigned int ret9(void) { return d.k; }

#define T(n, pre, post, op)                                                    \
  void fn1_##n(unsigned int x) { pre b.i post; }                               \
  void fn2_##n(unsigned int x) { pre b.j post; }                               \
  void fn3_##n(unsigned int x) { pre b.k post; }                               \
  void fn4_##n(unsigned int x) { pre c.i post; }                               \
  void fn5_##n(unsigned int x) { pre c.j post; }                               \
  void fn6_##n(unsigned int x) { pre c.k post; }                               \
  void fn7_##n(unsigned int x) { pre d.i post; }                               \
  void fn8_##n(unsigned int x) { pre d.j post; }                               \
  void fn9_##n(unsigned int x) { pre d.k post; }

#include "20040629-1.c"
#undef T

#define FAIL(n, i) abort()

int main(void) {
#define T(n, pre, post, op)                                                    \
  b.i = 51;                                                                    \
  b.j = 636;                                                                   \
  b.k = 31278;                                                                 \
  c.i = 21;                                                                    \
  c.j = 1;                                                                     \
  c.k = 33554432;                                                              \
  d.i = 26812;                                                                 \
  d.j = 156;                                                                   \
  d.k = 187;                                                                   \
  fn1_##n(3);                                                                  \
  if (ret1() != (op(51, 3) & ((1 << 6) - 1)))                                  \
    FAIL(n, 1);                                                                \
  b.i = 51;                                                                    \
  fn2_##n(251);                                                                \
  if (ret2() != (op(636, 251) & ((1 << 11) - 1)))                              \
    FAIL(n, 2);                                                                \
  b.j = 636;                                                                   \
  fn3_##n(13279);                                                              \
  if (ret3() != (op(31278, 13279) & ((1 << 15) - 1)))                          \
    FAIL(n, 3);                                                                \
  b.j = 31278;                                                                 \
  fn4_##n(24);                                                                 \
  if (ret4() != (op(21, 24) & ((1 << 5) - 1)))                                 \
    FAIL(n, 4);                                                                \
  c.i = 21;                                                                    \
  fn5_##n(1);                                                                  \
  if (ret5() != (op(1, 1) & ((1 << 1) - 1)))                                   \
    FAIL(n, 5);                                                                \
  c.j = 1;                                                                     \
  fn6_##n(264151);                                                             \
  if (ret6() != (op(33554432, 264151) & ((1 << 26) - 1)))                      \
    FAIL(n, 6);                                                                \
  c.k = 33554432;                                                              \
  fn7_##n(713);                                                                \
  if (ret7() != (op(26812, 713) & ((1 << 16) - 1)))                            \
    FAIL(n, 7);                                                                \
  d.i = 26812;                                                                 \
  fn8_##n(17);                                                                 \
  if (ret8() != (op(156, 17) & ((1 << 8) - 1)))                                \
    FAIL(n, 8);                                                                \
  d.j = 156;                                                                   \
  fn9_##n(199);                                                                \
  if (ret9() != (op(187, 199) & ((1 << 8) - 1)))                               \
    FAIL(n, 9);                                                                \
  d.k = 187;

#include "20040629-1.c"
#undef T
  return 0;
}

#else

#ifndef opadd
#define opadd(x, y)   (x + y)
#define opsub(x, y)   (x - y)
#define opinc(x, y)   (x + 1)
#define opdec(x, y)   (x - 1)
#define opand(x, y)   (x & y)
#define opior(x, y)   (x | y)
#define opxor(x, y)   (x ^ y)
#define opdiv(x, y)   (x / y)
#define oprem(x, y)   (x % y)
#define opadd3(x, y)  (x + 3)
#define opsub7(x, y)  (x - 7)
#define opand21(x, y) (x & 21)
#define opior19(x, y) (x | 19)
#define opxor37(x, y) (x ^ 37)
#define opdiv17(x, y) (x / 17)
#define oprem19(x, y) (x % 19)
#endif

T(1, , += x, opadd)
T(2, ++, , opinc)
T(3, , ++, opinc)
T(4, , -= x, opsub)
T(5, --, , opdec)
T(6, , --, opdec)
T(7, , &= x, opand)
T(8, , |= x, opior)
T(9, , ^= x, opxor)
T(a, , /= x, opdiv)
T(b, , %= x, oprem)
T(c, , += 3, opadd3)
T(d, , -= 7, opsub7)
T(e, , &= 21, opand21)
T(f, , |= 19, opior19)
T(g, , ^= 37, opxor37)
T(h, , /= 17, opdiv17)
T(i, , %= 19, oprem19)

#endif

#if 0
// SLATE-FILECHECK-DEFINES DEFAULT
#endif

#if 0
#endif

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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 i: u32 : 6;
// DEFAULT-NEXT:         field1 j: u32 : 11;
// DEFAULT-NEXT:         field2 k: u32 : 15;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 2], bit_offsets=[Some(0), Some(6), Some(17)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 i: u32 : 5;
// DEFAULT-NEXT:         field1 j: u32 : 1;
// DEFAULT-NEXT:         field2 k: u32 : 26;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0, 0], bit_offsets=[Some(0), Some(5), Some(6)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 i: u32 : 16;
// DEFAULT-NEXT:         field1 j: u32 : 8;
// DEFAULT-NEXT:         field2 k: u32 : 8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 2, 3], bit_offsets=[Some(0), Some(16), Some(24)], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %3 b: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 c: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 d: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%342 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @ret1() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @ret2() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @ret3() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @ret4() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @ret5() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @ret6() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @ret7() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @ret8() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @ret9() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u32, reason=return, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @fn1_1(%18 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %343: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %344: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%343))), read<u32>(%18));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%344));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @fn2_1(%20 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %345: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %346: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%345))), read<u32>(%20));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%346));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @fn3_1(%22 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %347: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %348: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%347))), read<u32>(%22));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%348));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @fn4_1(%24 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %349: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %350: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%349))), read<u32>(%24));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%350));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @fn5_1(%26 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %351: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %352: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%351))), read<u32>(%26));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%352));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @fn6_1(%28 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %353: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %354: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%353))), read<u32>(%28));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%354));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @fn7_1(%30 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %355: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %356: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%355))), read<u32>(%30));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%356));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @fn8_1(%32 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %357: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %358: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%357))), read<u32>(%32));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%358));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @fn9_1(%34 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %359: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %360: u32 [synthetic] = add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%359))), read<u32>(%34));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%360));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @fn1_2(%36 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %361: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %362: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%361)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%362));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @fn2_2(%38 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %363: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %364: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%363)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%364));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @fn3_2(%40 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %365: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %366: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%365)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%366));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %41 @fn4_2(%42 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %367: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %368: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%367)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%368));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @fn5_2(%44 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %369: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %370: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%369)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%370));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @fn6_2(%46 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %371: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %372: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%371)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%372));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @fn7_2(%48 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %373: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %374: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%373)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%374));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @fn8_2(%50 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %375: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %376: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%375)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%376));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %51 @fn9_2(%52 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %377: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %378: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%377)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%378));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @fn1_3(%54 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %379: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %380: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%379)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%380));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @fn2_3(%56 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %381: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %382: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%381)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%382));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @fn3_3(%58 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %383: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %384: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%383)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%384));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %59 @fn4_3(%60 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %385: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %386: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%385)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%386));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %61 @fn5_3(%62 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %387: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %388: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%387)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%388));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @fn6_3(%64 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %389: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %390: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%389)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%390));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %65 @fn7_3(%66 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %391: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %392: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%391)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%392));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %67 @fn8_3(%68 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %393: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %394: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%393)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%394));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %69 @fn9_3(%70 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %395: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %396: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%395)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%396));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %71 @fn1_4(%72 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %397: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %398: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%397))), read<u32>(%72));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%398));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %73 @fn2_4(%74 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %399: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %400: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%399))), read<u32>(%74));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%400));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @fn3_4(%76 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %401: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %402: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%401))), read<u32>(%76));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%402));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @fn4_4(%78 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %403: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %404: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%403))), read<u32>(%78));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%404));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %79 @fn5_4(%80 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %405: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %406: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%405))), read<u32>(%80));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%406));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @fn6_4(%82 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %407: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %408: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%407))), read<u32>(%82));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%408));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @fn7_4(%84 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %409: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %410: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%409))), read<u32>(%84));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%410));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @fn8_4(%86 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %411: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %412: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%411))), read<u32>(%86));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%412));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @fn9_4(%88 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %413: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %414: u32 [synthetic] = sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%413))), read<u32>(%88));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%414));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %89 @fn1_5(%90 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %415: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %416: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%415)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%416));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %91 @fn2_5(%92 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %417: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %418: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%417)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%418));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %93 @fn3_5(%94 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %419: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %420: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%419)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%420));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %95 @fn4_5(%96 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %421: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %422: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%421)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%422));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %97 @fn5_5(%98 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %423: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %424: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%423)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%424));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %99 @fn6_5(%100 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %425: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %426: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%425)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%426));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %101 @fn7_5(%102 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %427: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %428: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%427)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%428));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %103 @fn8_5(%104 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %429: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %430: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%429)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%430));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %105 @fn9_5(%106 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %431: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %432: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%431)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%432));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %107 @fn1_6(%108 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %433: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %434: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%433)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%434));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %109 @fn2_6(%110 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %435: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %436: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%435)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%436));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %111 @fn3_6(%112 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %437: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %438: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%437)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%438));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %113 @fn4_6(%114 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %439: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %440: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%439)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%440));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %115 @fn5_6(%116 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %441: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %442: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%441)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%442));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %117 @fn6_6(%118 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %443: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %444: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%443)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%444));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %119 @fn7_6(%120 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %445: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %446: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%445)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%446));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %121 @fn8_6(%122 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %447: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %448: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%447)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%448));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %123 @fn9_6(%124 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %449: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %450: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%449)), const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%450));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %125 @fn1_7(%126 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %451: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %452: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%451))), read<u32>(%126));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%452));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %127 @fn2_7(%128 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %453: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %454: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%453))), read<u32>(%128));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%454));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %129 @fn3_7(%130 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %455: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %456: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%455))), read<u32>(%130));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%456));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %131 @fn4_7(%132 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %457: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %458: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%457))), read<u32>(%132));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%458));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %133 @fn5_7(%134 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %459: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %460: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%459))), read<u32>(%134));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%460));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %135 @fn6_7(%136 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %461: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %462: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%461))), read<u32>(%136));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%462));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %137 @fn7_7(%138 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %463: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %464: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%463))), read<u32>(%138));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%464));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %139 @fn8_7(%140 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %465: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %466: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%465))), read<u32>(%140));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%466));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %141 @fn9_7(%142 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %467: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %468: u32 [synthetic] = and<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%467))), read<u32>(%142));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%468));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %143 @fn1_8(%144 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %469: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %470: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%469))), read<u32>(%144));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%470));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %145 @fn2_8(%146 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %471: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %472: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%471))), read<u32>(%146));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%472));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %147 @fn3_8(%148 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %473: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %474: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%473))), read<u32>(%148));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%474));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %149 @fn4_8(%150 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %475: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %476: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%475))), read<u32>(%150));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%476));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %151 @fn5_8(%152 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %477: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %478: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%477))), read<u32>(%152));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%478));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %153 @fn6_8(%154 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %479: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %480: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%479))), read<u32>(%154));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%480));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %155 @fn7_8(%156 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %481: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %482: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%481))), read<u32>(%156));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%482));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %157 @fn8_8(%158 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %483: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %484: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%483))), read<u32>(%158));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%484));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %159 @fn9_8(%160 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %485: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %486: u32 [synthetic] = or<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%485))), read<u32>(%160));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%486));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %161 @fn1_9(%162 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %487: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %488: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%487))), read<u32>(%162));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%488));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %163 @fn2_9(%164 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %489: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %490: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%489))), read<u32>(%164));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%490));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %165 @fn3_9(%166 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %491: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %492: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%491))), read<u32>(%166));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%492));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %167 @fn4_9(%168 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %493: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %494: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%493))), read<u32>(%168));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%494));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %169 @fn5_9(%170 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %495: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %496: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%495))), read<u32>(%170));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%496));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %171 @fn6_9(%172 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %497: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %498: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%497))), read<u32>(%172));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%498));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %173 @fn7_9(%174 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %499: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %500: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%499))), read<u32>(%174));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%500));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %175 @fn8_9(%176 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %501: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %502: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%501))), read<u32>(%176));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%502));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %177 @fn9_9(%178 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %503: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %504: u32 [synthetic] = xor<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%503))), read<u32>(%178));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%504));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %179 @fn1_a(%180 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %505: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %506: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%505))), read<u32>(%180));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%506));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %181 @fn2_a(%182 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %507: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %508: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%507))), read<u32>(%182));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%508));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %183 @fn3_a(%184 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %509: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %510: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%509))), read<u32>(%184));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%510));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %185 @fn4_a(%186 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %511: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %512: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%511))), read<u32>(%186));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%512));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %187 @fn5_a(%188 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %513: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %514: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%513))), read<u32>(%188));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%514));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %189 @fn6_a(%190 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %515: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %516: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%515))), read<u32>(%190));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%516));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %191 @fn7_a(%192 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %517: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %518: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%517))), read<u32>(%192));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%518));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %193 @fn8_a(%194 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %519: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %520: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%519))), read<u32>(%194));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%520));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %195 @fn9_a(%196 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %521: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %522: u32 [synthetic] = div<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%521))), read<u32>(%196));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%522));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %197 @fn1_b(%198 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %523: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %524: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%523))), read<u32>(%198));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%524));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %199 @fn2_b(%200 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %525: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %526: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%525))), read<u32>(%200));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%526));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %201 @fn3_b(%202 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %527: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %528: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%527))), read<u32>(%202));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%528));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %203 @fn4_b(%204 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %529: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %530: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%529))), read<u32>(%204));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%530));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %205 @fn5_b(%206 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %531: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %532: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%531))), read<u32>(%206));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%532));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %207 @fn6_b(%208 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %533: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %534: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%533))), read<u32>(%208));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%534));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %209 @fn7_b(%210 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %535: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %536: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%535))), read<u32>(%210));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%536));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %211 @fn8_b(%212 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %537: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %538: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%537))), read<u32>(%212));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%538));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %213 @fn9_b(%214 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %539: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %540: u32 [synthetic] = rem<u32, by_zero=ub>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%539))), read<u32>(%214));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%540));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %215 @fn1_c(%216 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %541: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %542: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%541)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%542));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %217 @fn2_c(%218 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %543: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %544: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%543)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%544));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %219 @fn3_c(%220 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %545: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %546: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%545)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%546));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %221 @fn4_c(%222 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %547: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %548: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%547)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%548));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %223 @fn5_c(%224 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %549: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %550: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%549)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%550));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %225 @fn6_c(%226 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %551: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %552: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%551)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%552));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %227 @fn7_c(%228 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %553: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %554: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%553)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%554));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %229 @fn8_c(%230 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %555: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %556: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%555)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%556));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %231 @fn9_c(%232 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %557: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %558: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%557)), const<i32>(3)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%558));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %233 @fn1_d(%234 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %559: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %560: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%559)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%560));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %235 @fn2_d(%236 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %561: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %562: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%561)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%562));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %237 @fn3_d(%238 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %563: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %564: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%563)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%564));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %239 @fn4_d(%240 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %565: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %566: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%565)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%566));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %241 @fn5_d(%242 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %567: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %568: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%567)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%568));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %243 @fn6_d(%244 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %569: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %570: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%569)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%570));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %245 @fn7_d(%246 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %571: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %572: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%571)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%572));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %247 @fn8_d(%248 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %573: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %574: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%573)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%574));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %249 @fn9_d(%250 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %575: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %576: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%575)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%576));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %251 @fn1_e(%252 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %577: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %578: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%577)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%578));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %253 @fn2_e(%254 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %579: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %580: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%579)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%580));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %255 @fn3_e(%256 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %581: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %582: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%581)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%582));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %257 @fn4_e(%258 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %583: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %584: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%583)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%584));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %259 @fn5_e(%260 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %585: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %586: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%585)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%586));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %261 @fn6_e(%262 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %587: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %588: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%587)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%588));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %263 @fn7_e(%264 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %589: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %590: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%589)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%590));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %265 @fn8_e(%266 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %591: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %592: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%591)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%592));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %267 @fn9_e(%268 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %593: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %594: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(and<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%593)), const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%594));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %269 @fn1_f(%270 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %595: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %596: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%595)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%596));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %271 @fn2_f(%272 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %597: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %598: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%597)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%598));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %273 @fn3_f(%274 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %599: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %600: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%599)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%600));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %275 @fn4_f(%276 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %601: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %602: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%601)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%602));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %277 @fn5_f(%278 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %603: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %604: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%603)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%604));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %279 @fn6_f(%280 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %605: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %606: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%605)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%606));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %281 @fn7_f(%282 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %607: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %608: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%607)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%608));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %283 @fn8_f(%284 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %609: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %610: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%609)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%610));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %285 @fn9_f(%286 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %611: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %612: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%611)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%612));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %287 @fn1_g(%288 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %613: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %614: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%613)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%614));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %289 @fn2_g(%290 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %615: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %616: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%615)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%616));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %291 @fn3_g(%292 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %617: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %618: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%617)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%618));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %293 @fn4_g(%294 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %619: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %620: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%619)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%620));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %295 @fn5_g(%296 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %621: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %622: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%621)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%622));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %297 @fn6_g(%298 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %623: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %624: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%623)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%624));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %299 @fn7_g(%300 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %625: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %626: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%625)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%626));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %301 @fn8_g(%302 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %627: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %628: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%627)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%628));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %303 @fn9_g(%304 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %629: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %630: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%629)), const<i32>(37)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%630));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %305 @fn1_h(%306 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %631: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %632: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%631)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%632));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %307 @fn2_h(%308 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %633: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %634: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%633)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%634));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %309 @fn3_h(%310 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %635: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %636: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%635)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%636));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %311 @fn4_h(%312 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %637: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %638: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%637)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%638));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %313 @fn5_h(%314 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %639: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %640: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%639)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%640));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %315 @fn6_h(%316 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %641: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %642: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%641)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%642));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %317 @fn7_h(%318 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %643: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %644: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%643)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%644));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %319 @fn8_h(%320 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %645: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %646: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%645)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%646));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %321 @fn9_h(%322 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %647: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %648: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%647)), const<i32>(17)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%648));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %323 @fn1_i(%324 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %649: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3));
// DEFAULT-NEXT:         let %650: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%649)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), read<u32>(%650));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %325 @fn2_i(%326 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %651: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3));
// DEFAULT-NEXT:         let %652: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%651)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), read<u32>(%652));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %327 @fn3_i(%328 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %653: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3));
// DEFAULT-NEXT:         let %654: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%653)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), read<u32>(%654));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %329 @fn4_i(%330 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %655: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5));
// DEFAULT-NEXT:         let %656: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%655)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), read<u32>(%656));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %331 @fn5_i(%332 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %657: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5));
// DEFAULT-NEXT:         let %658: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%657)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), read<u32>(%658));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %333 @fn6_i(%334 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %659: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5));
// DEFAULT-NEXT:         let %660: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%659)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), read<u32>(%660));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %335 @fn7_i(%336 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %661: u32 [synthetic] = read<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7));
// DEFAULT-NEXT:         let %662: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%661)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), read<u32>(%662));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %337 @fn8_i(%338 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %663: u32 [synthetic] = read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7));
// DEFAULT-NEXT:         let %664: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%663)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), read<u32>(%664));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %339 @fn9_i(%340 x: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %665: u32 [synthetic] = read<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7));
// DEFAULT-NEXT:         let %666: u32 [synthetic] = reinterpret<u32, reason=assign, fits=unknown>(rem<i32, by_zero=ub, min_by_neg_one=ub>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(%665)), const<i32>(19)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), read<u32>(%666));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %341 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%17, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%19, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%21, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%23, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%25, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%27, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%29, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%31, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%33, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%35, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%37, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%39, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%41, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%43, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%45, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%47, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%49, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%51, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%53, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%55, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%57, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%59, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%61, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%63, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%65, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%67, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%69, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%71, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%73, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%75, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%77, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%79, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%81, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%83, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%85, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%87, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%89, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%91, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%93, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%95, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%97, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%99, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%101, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%103, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%105, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%107, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%109, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%111, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%113, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%115, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%117, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%119, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%121, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%123, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%125, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%127, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%129, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%131, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%133, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%135, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%137, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%139, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%141, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%143, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%145, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%147, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%149, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%151, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%153, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%155, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%157, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%159, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%161, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%163, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%165, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%167, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%169, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%171, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%173, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%175, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%177, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%179, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%181, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%183, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%185, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%187, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%189, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%191, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%193, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%195, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%197, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%199, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(251)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%201, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(13279)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%203, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(24)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%205, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%207, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(264151)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%209, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(713)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%211, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%213, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(199)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%215, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(51), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%217, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(636), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%219, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(31278), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%221, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(21), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%223, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(1), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%225, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(33554432), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%227, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(26812), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%229, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(156), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%231, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(add<i32, overflow=ub>(const<i32>(187), const<i32>(3)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%233, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(51), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%235, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(636), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%237, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(31278), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%239, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(21), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%241, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(1), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%243, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(33554432), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%245, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(26812), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%247, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(156), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%249, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(sub<i32, overflow=ub>(const<i32>(187), const<i32>(7)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%251, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(51), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%253, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(636), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%255, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(31278), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%257, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(21), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%259, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(1), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%261, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(33554432), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%263, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(26812), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%265, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(156), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%267, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(and<i32>(const<i32>(187), const<i32>(21)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%269, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(51), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%271, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(636), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%273, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(31278), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%275, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(21), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%277, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(1), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%279, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(33554432), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%281, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(26812), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%283, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(156), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%285, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(or<i32>(const<i32>(187), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%287, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(51), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%289, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(636), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%291, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(31278), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%293, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(21), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%295, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(1), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%297, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(33554432), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%299, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(26812), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%301, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(156), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%303, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(xor<i32>(const<i32>(187), const<i32>(37)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%305, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%307, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%309, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%311, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%313, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%315, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%317, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%319, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%321, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(17)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=17..32>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%323, reinterpret<u32, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%8), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(51), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(6)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..6>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%325, reinterpret<u32, reason=arg, fits=always>(const<i32>(251)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%9), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(636), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(11)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(636)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%327, reinterpret<u32, reason=arg, fits=always>(const<i32>(13279)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%10), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(31278), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(15)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=6..17>(%3), reinterpret<u32, reason=assign, fits=always>(const<i32>(31278)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%329, reinterpret<u32, reason=arg, fits=always>(const<i32>(24)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%11), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(21), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(5)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..5>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%331, reinterpret<u32, reason=arg, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%12), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(1)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=5..6>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%333, reinterpret<u32, reason=arg, fits=always>(const<i32>(264151)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%13), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(33554432), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(26)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=6..32>(%5), reinterpret<u32, reason=assign, fits=always>(const<i32>(33554432)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%335, reinterpret<u32, reason=arg, fits=always>(const<i32>(713)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%14), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(26812), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(16)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield0<unit=0, bytes=0..4, bits=0..16>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(26812)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%337, reinterpret<u32, reason=arg, fits=always>(const<i32>(17)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%15), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(156), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(156)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%339, reinterpret<u32, reason=arg, fits=always>(const<i32>(199)));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn() -> u32>(%16), reinterpret<u32, reason=usual_arith, fits=unknown>(and<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(187), const<i32>(19)), sub<i32, overflow=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<u32>(bitfield2<unit=0, bytes=0..4, bits=24..32>(%7), reinterpret<u32, reason=assign, fits=always>(const<i32>(187)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
