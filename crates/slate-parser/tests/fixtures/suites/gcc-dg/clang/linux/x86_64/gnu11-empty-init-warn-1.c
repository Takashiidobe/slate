/* Test that GNU11 warnings are produced about initializers that might
 * not zero padding bits of structs or unions.
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
main (int argc, char *argv[])
{
  struct A a = {}; /* ZI because it omits a, b */
  struct B b = {}; /* ZI because it omits a, b, c */
  struct B c = { argc, 2 }; /* ZI because it omits c[0], c[1], c[2] */

  /* ZI because it omits c[0], c[1] */
  struct B d = { .b = argc, .a = 2, .c[2].a = 3, .c[2].b = 4 };

  struct B e = { argc, 2, .c[2] = {}, .c[1] = { 9 }, .c[0] = {}, /* { dg-warning "padding might not be initialized to zero" } */
		 .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };

  struct B f = { argc, 2, {}, /* { dg-warning "padding might not be initialized to zero" } */
		 .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6,
		 .c[2].a = 7, .c[2].b = 8 };

  struct B g = { argc, 2, .c[0].a = 3, .c[0].b = 4, .c[1].a = 5, .c[1].b = 6, /* { dg-warning "padding might not be initialized to zero" } */
		 .c[2].a = 7, .c[2].b = 8 };

  union U h = {}; /* ZI because it omits a, b */
  union U i = { argc }; /* { dg-warning "padding might not be initialized to zero" } */
  union U j = { .a = argc }; /* { dg-warning "padding might not be initialized to zero" } */
  union U k = { .b = argc }; /* Largest member is initialized */
  struct D l = {}; /* ZI because it omits a, b, c */
  struct D m = { argc, 2 }; /* ZI because it omits c */
  struct D n = { argc, 2, {}, .c.a.a = 3, .c.a.b = 4 }; /* { dg-warning "padding might not be initialized to zero" } */
  struct C o = { {} }; /* ZI because it omits a.a, a.b */
  struct C p = { {}, .a.a = argc, .a.b = 4 }; /* { dg-warning "padding might not be initialized to zero" } */
  struct E q = { argc, 2 }; /* No padding */
  union V r = { argc }; /* No padding */
  struct F s = { argc, {}, .b.a = 2 }; /* { dg-warning "padding might not be initialized to zero" } */
  struct F t = { argc, {}, .b = {.a = 2} }; /* { dg-warning "padding might not be initialized to zero" } */

  return 0;
}

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
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: array<@type[[TYPE_A]], 3>;
// DEFAULT-NEXT:     } [size=64, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 a: @type[[TYPE_A]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:         field2 c: @type[[TYPE_C]];
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_E:[0-9]+]] E = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// DEFAULT-NEXT:         field0 a: u8;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = union {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: @type[[TYPE_U]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: @type[[TYPE_A]] [storage=automatic] = aggregate<@type[[TYPE_A]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2))), field1 = widen<i64, reason=assign>(read<i32>(%[[VALUE_argc]])), field2 = aggregate<array<@type[[TYPE_A]], 3>, zero_fill=true>(index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))));
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type[[TYPE_A]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type[[TYPE_A]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %[[VALUE_g:[0-9]+]] g: @type[[TYPE_B]] [storage=automatic] = aggregate<@type[[TYPE_B]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<array<@type[[TYPE_A]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4))), index1 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(5))), field1 = widen<i64, reason=assign>(const<i32>(6))), index2 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(7))), field1 = widen<i64, reason=assign>(const<i32>(8)))));
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: @type[[TYPE_U]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>();
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: @type[[TYPE_U]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))));
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: @type[[TYPE_U]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: @type[[TYPE_U]] [storage=automatic] = aggregate<@type[[TYPE_U]], zero_fill=false>(field1 = widen<i64, reason=assign>(read<i32>(%[[VALUE_argc]])));
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: @type[[TYPE_D]] [storage=automatic] = aggregate<@type[[TYPE_D]], zero_fill=true>();
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: @type[[TYPE_D]] [storage=automatic] = aggregate<@type[[TYPE_D]], zero_fill=true>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: @type[[TYPE_D]] [storage=automatic] = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))), field1 = widen<i64, reason=assign>(const<i32>(2)), field2 = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))), field1 = widen<i64, reason=assign>(const<i32>(4)))));
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: @type[[TYPE_C]] [storage=automatic] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = aggregate<@type[[TYPE_A]], zero_fill=true>());
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: @type[[TYPE_C]] [storage=automatic] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE_argc]]))), field1 = widen<i64, reason=assign>(const<i32>(4))));
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: @type[[TYPE_E]] [storage=automatic] = aggregate<@type[[TYPE_E]], zero_fill=false>(field0 = widen<i64, reason=assign>(read<i32>(%[[VALUE_argc]])), field1 = widen<i64, reason=assign>(const<i32>(2)));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_V]] [storage=automatic] = aggregate<@type[[TYPE_V]], zero_fill=false>(field0 = widen<i64, reason=assign>(read<i32>(%[[VALUE_argc]])));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_F]] [storage=automatic] = aggregate<@type[[TYPE_F]], zero_fill=false>(field0 = widen<i64, reason=assign>(read<i32>(%[[VALUE_argc]])), field1 = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: @type[[TYPE_F]] [storage=automatic] = aggregate<@type[[TYPE_F]], zero_fill=false>(field0 = widen<i64, reason=assign>(read<i32>(%[[VALUE_argc]])), field1 = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(2)))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
