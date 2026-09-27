/* Test that no GNU11 warnings are produced about initializers of objects with
 * static storage duration that might not have zeroed padding bits if they
 * instead had automatic storage duration.
 */
/* { dg-do run } */
/* { dg-options "-std=gnu11 -Wzero-init-padding-bits=all" } */

struct A { unsigned char a; long long b; };
struct B { unsigned char a; long long b; struct A c[3]; };
struct C { struct A a; };
struct D { unsigned char a; long long b; struct C c; };
struct E { long long a; long long b; };
union U { unsigned char a; long long b; };
union V { long long a; long long b; };
struct F { long long a; union U b; };

int
main ()
{
  static struct A a = {};
  static struct B b = {};
  static struct B c = { 1, 2 };
  static struct B d = { .b = 1, .a = 2, .c[2].a = 3, .c[2].b = 4 };

  static struct B e = { 1, 2, .c[2] = {}, .c[1] = { 9 }, .c[0] = {},
                  .c[0].a = 3, .c[0].b = 4, .c[1].a = 5,
                  .c[1].b = 6, .c[2].a = 7, .c[2].b = 8 };

  static struct B f = { 1, 2, {},
                  .c[0].a = 3, .c[0].b = 4, .c[1].a = 5,
                  .c[1].b = 6, .c[2].a = 7, .c[2].b = 8 };

  static struct B g = { 1, 2, .c[0].a = 3, .c[0].b = 4, .c[1].a = 5,
                  .c[1].b = 6, .c[2].a = 7, .c[2].b = 8 };

  static union U h = {};
  static union U i = { 1 };
  static union U j = { .a = 1 };
  static union U k = { .b = 1 };
  static struct D l = {};
  static struct D m = { 1, 2 };
  static struct D n = { 1, 2, {}, .c.a.a = 3, .c.a.b = 4 };
  static struct C o = { {} };
  static struct C p = { {}, .a.a = 3, .a.b = 4 };
  static struct E q = { 1, 2 };
  static union V r = { 1 };
  static struct F s = { 1, {}, .b.a = 2 };
  static struct F t = { 1, {}, .b = {.a = 2} };

  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT gnu11
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
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: array<@type0, 3>;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 a: @type0;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type3 D = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: @type2;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type4 E = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type5 U = union {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type6 V = union {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type7 F = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: @type5;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %9 a: @type0 [storage=static] = aggregate<@type0, zero_fill=true>() [linkage=internal];
// DEFAULT-NEXT:     global %10 b: @type1 [storage=static] = aggregate<@type1, zero_fill=true>() [linkage=internal];
// DEFAULT-NEXT:     global %11 c: @type1 [storage=static] = aggregate<@type1, zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %12 d: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), field1 = widen<i64, reason=assign>(const<i32>(1)), field2 = aggregate<array<@type0, 3>, zero_fill=true>(index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))))) [linkage=internal];
// DEFAULT-NEXT:     global %13 e: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8))))) [linkage=internal];
// DEFAULT-NEXT:     global %14 f: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8))))) [linkage=internal];
// DEFAULT-NEXT:     global %15 g: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8))))) [linkage=internal];
// DEFAULT-NEXT:     global %16 h: @type5 [storage=static] = aggregate<@type5, zero_fill=false>() [linkage=internal];
// DEFAULT-NEXT:     global %17 i: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %18 j: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)))) [linkage=internal];
// DEFAULT-NEXT:     global %19 k: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %20 l: @type3 [storage=static] = aggregate<@type3, zero_fill=true>() [linkage=internal];
// DEFAULT-NEXT:     global %21 m: @type3 [storage=static] = aggregate<@type3, zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %22 n: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))))) [linkage=internal];
// DEFAULT-NEXT:     global %23 o: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=true>()) [linkage=internal];
// DEFAULT-NEXT:     global %24 p: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))) [linkage=internal];
// DEFAULT-NEXT:     global %25 q: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(1)), field1 = widen<i64, reason=assign>(const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %26 r: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(1))) [linkage=internal];
// DEFAULT-NEXT:     global %27 s: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(1)), field1 = aggregate<@type5, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))))) [linkage=internal];
// DEFAULT-NEXT:     global %28 t: @type7 [storage=static] = aggregate<@type7, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(1)), field1 = aggregate<@type5, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))))) [linkage=internal];
// DEFAULT-NEXT:     fn %8 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
