/* Test whether __compound_literal.* objects are not emitted unless
   they are actually needed.  */
/* Origin: Jakub Jelinek <jakub@redhat.com> */
/* { dg-do compile } */
/* { dg-options "-std=gnu89 -O2" } */
/* { dg-final { scan-assembler-not "__compound_literal" } } */

struct A { int i; int j; int k[4]; };
struct B { };
struct C { int i; };
struct D { int i; struct C j; };

struct A a = (struct A) { .j = 6, .k[2] = 12 };
struct B b = (struct B) { };
int c[] = (int []) { [2] = 6, 7, 8 };
int d[] = (int [3]) { 1 };
int e[2] = (int []) { 1, 2 };
int f[2] = (int [2]) { 1 };
struct C g[3] = { [2] = (struct C) { 13 }, [1] = (const struct C) { 12 } };
struct D h = { .j = (struct C) { 15 }, .i = 14 };
struct D i[2] = { [1].j = (const struct C) { 17 },
		  [0] = { 0, (struct C) { 16 } } };
static const int *j = 1 ? (const int *) 0 : & (const int) { 26 };
int k = (int) sizeof ((int [6]) { 1, 2, 3, 4, 5, 6 }) + 4;
int l = (int) sizeof ((struct C) { 16 });

// SLATE-FILECHECK-STD DEFAULT gnu89
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
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:         field2 k: array<i32, 4>;
// DEFAULT-NEXT:     } [size=24, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_C:[0-9]+]] C = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: @type[[TYPE_C]];
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: @type[[TYPE_A]] [storage=static] = copy<@type[[TYPE_A]], reason=assign>(read<@type[[TYPE_A]]>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_A]], zero_fill=true>(field1 = const<i32>(6), field2 = aggregate<array<i32, 4>, zero_fill=true>(index2 = const<i32>(12))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE_B]] [storage=static] = copy<@type[[TYPE_B]], reason=assign>(read<@type[[TYPE_B]]>(compound_literal %[[VALUE1:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_B]], zero_fill=false>())) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=true>(index2 = const<i32>(6), index3 = const<i32>(7), index4 = const<i32>(8)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=true>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: array<@type[[TYPE_C]], 3> [storage=static] = aggregate<array<@type[[TYPE_C]], 3>, zero_fill=true>(index1 = copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(compound_literal %[[VALUE2:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = const<i32>(12)))), index2 = copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(compound_literal %[[VALUE3:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = const<i32>(13))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: @type[[TYPE_D]] [storage=static] = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = const<i32>(14), field1 = copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(compound_literal %[[VALUE4:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = const<i32>(15))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: array<@type[[TYPE_D]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE_D]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_D]], zero_fill=false>(field0 = const<i32>(0), field1 = copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(compound_literal %[[VALUE5:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = const<i32>(16))))), index1 = aggregate<@type[[TYPE_D]], zero_fill=true>(field1 = copy<@type[[TYPE_C]], reason=assign>(read<@type[[TYPE_C]]>(compound_literal %[[VALUE6:[0-9]+]] [storage=static] = aggregate<@type[[TYPE_C]], zero_fill=false>(field0 = const<i32>(17)))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_j:[0-9]+]] j: ptr<const i32> [storage=static] = conditional<ptr<const i32>>(ne<i32>(const<i32>(1), const<i32>(0)), null<ptr<const i32>>, addr_of<ptr<const i32>>(compound_literal %[[VALUE7:[0-9]+]] [storage=static] = const<i32>(26))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_k:[0-9]+]] k: i32 [storage=static] = add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(24))), const<i32>(4)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_l:[0-9]+]] l: i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
