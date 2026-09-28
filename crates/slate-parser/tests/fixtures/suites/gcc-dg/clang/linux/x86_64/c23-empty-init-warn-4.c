/* Test that no C23 warnings are produced (by default) about initializers
 * that might not zero padding bits. Expected results are the same as GNU11
 * despite the fact that empty initializers zero padding bits.
 */
/* { dg-do run } */
/* { dg-options "-std=c23" } */

struct A { unsigned char a; long long b; };
struct B { unsigned char a; long long b; struct A c[3]; };
struct C { struct A a; };
struct D { unsigned char a; long long b; struct C c; };
struct E { long long a; long long b; };
union U { unsigned char a; long long b; };
union V { long long a; long long b; };
struct F { long long a; union U b; };

int
main (int argc, char *argv[])
{
  struct A a = {}; /* ZI because it omits a, b */
  struct B b = {}; /* ZI because it omits a, b, c */
  struct B c = { argc, 2 }; /* ZI because it omits c[0], c[1], c[2] */

  /* ZI because it omits c[0], c[1] */
  struct B d = { .b = argc, .a = 2, .c[2].a = 3, .c[2].b = 4 };

  /* Padding bits might not be initialized according to C23 but GCC treats
     the explicitly initialized padding of c[0] and c[2] as contagious */
  struct B e = { argc, 2, .c[2] = {}, .c[1] = { 9 }, .c[0] = {},
		 .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };

  /* Padding bits might not be initialized according to C23 but GCC treats
     the explicitly initialized padding of c as contagious */
  struct B f = { argc, 2, {},
		 .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };

  /* Padding bits might not be initialized */
  struct B g = { argc, 2, .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };

  union U h = {}; /* ZI because it omits a, b */
  union U i = { argc }; /* Padding bits might not be initialized */
  union U j = { .a = argc }; /* Padding bits might not be initialized */
  union U k = { .b = argc }; /* Largest member is initialized */
  struct D l = {}; /* ZI because it omits a, b, c */
  struct D m = { argc, 2 }; /* ZI because it omits c */

  /* Padding bits might not be initialized according to C23 but GCC treats
     the explicitly initialized padding of c as contagious */
  struct D n = { argc, 2, {}, .c.a.a = 3, .c.a.b = 4 };

  struct C o = { {} }; /* ZI because it omits a.a, a.b */

  /* All (struct) padding is explicitly initialized */
  struct C p = { {}, .a.a = argc, .a.b = 4 };

  struct E q = { argc, 2 }; /* No padding */
  union V r = { argc }; /* No padding */

  /* All (union) padding is explicitly initialized */
  struct F s = { argc, {}, .b.a = 2 };

  /* Empty initializer is overridden and therefore ineffective */
  struct F t = { argc, {}, .b = {.a = 2} };

  return 0;
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
// DEFAULT-NEXT:     fn %8 @main(%9 argc: i32, %10 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 a: @type0 [storage=automatic] = aggregate<@type0, zero_fill=true>();
// DEFAULT-NEXT:         let %12 b: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>();
// DEFAULT-NEXT:         let %13 c: @type1 [storage=automatic] = aggregate<@type1, zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %14 d: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), field1 = widen<i64, reason=assign>(read<i32>(%9)), field2 = aggregate<array<@type0, 3>, zero_fill=true>(index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))));
// DEFAULT-NEXT:         let %15 e: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %16 f: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %17 g: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %18 h: @type5 [storage=automatic] = aggregate<@type5, zero_fill=false>();
// DEFAULT-NEXT:         let %19 i: @type5 [storage=automatic] = aggregate<@type5, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))));
// DEFAULT-NEXT:         let %20 j: @type5 [storage=automatic] = aggregate<@type5, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))));
// DEFAULT-NEXT:         let %21 k: @type5 [storage=automatic] = aggregate<@type5, zero_fill=false>(field1 = widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %22 l: @type3 [storage=automatic] = aggregate<@type3, zero_fill=true>();
// DEFAULT-NEXT:         let %23 m: @type3 [storage=automatic] = aggregate<@type3, zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %24 n: @type3 [storage=automatic] = aggregate<@type3, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))));
// DEFAULT-NEXT:         let %25 o: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=true>());
// DEFAULT-NEXT:         let %26 p: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = aggregate<@type0, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%9))), field1 = widen<i64, reason=assign>(const<i32>(4))));
// DEFAULT-NEXT:         let %27 q: @type4 [storage=automatic] = aggregate<@type4, zero_fill=false>(field0 = widen<i64, reason=assign>(read<i32>(%9)), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %28 r: @type6 [storage=automatic] = aggregate<@type6, zero_fill=false>(field0 = widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %29 s: @type7 [storage=automatic] = aggregate<@type7, zero_fill=false>(field0 = widen<i64, reason=assign>(read<i32>(%9)), field1 = aggregate<@type5, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         let %30 t: @type7 [storage=automatic] = aggregate<@type7, zero_fill=false>(field0 = widen<i64, reason=assign>(read<i32>(%9)), field1 = aggregate<@type5, zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
