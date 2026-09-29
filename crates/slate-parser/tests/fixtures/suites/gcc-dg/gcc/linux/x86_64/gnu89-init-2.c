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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:         field2 k: array<i32, 4>;
// DEFAULT-NEXT:     } [size=24, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type2 C = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 D = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: @type2;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %4 a: @type0 [storage=static] = copy<@type0, reason=assign>(read<@type0>(compound_literal %16 [storage=static] = aggregate<@type0, zero_fill=true>(field1 = const<i32>(6), field2 = aggregate<array<i32, 4>, zero_fill=true>(index2 = const<i32>(12))))) [linkage=external];
// DEFAULT-NEXT:     global %5 b: @type1 [storage=static] = copy<@type1, reason=assign>(read<@type1>(compound_literal %17 [storage=static] = aggregate<@type1, zero_fill=false>())) [linkage=external];
// DEFAULT-NEXT:     global %6 c: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=true>(index2 = const<i32>(6), index3 = const<i32>(7), index4 = const<i32>(8)) [linkage=external];
// DEFAULT-NEXT:     global %7 d: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=true>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %8 e: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %9 f: array<i32, 2> [storage=static] = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %10 g: array<@type2, 3> [storage=static] = aggregate<array<@type2, 3>, zero_fill=true>(index1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %19 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(12)))), index2 = copy<@type2, reason=assign>(read<@type2>(compound_literal %18 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(13))))) [linkage=external];
// DEFAULT-NEXT:     global %11 h: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = const<i32>(14), field1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %20 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(15))))) [linkage=external];
// DEFAULT-NEXT:     global %12 i: array<@type3, 2> [storage=static] [align=16] = aggregate<array<@type3, 2>, zero_fill=false>(index0 = aggregate<@type3, zero_fill=false>(field0 = const<i32>(0), field1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %22 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(16))))), index1 = aggregate<@type3, zero_fill=true>(field1 = copy<@type2, reason=assign>(read<@type2>(compound_literal %21 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(17)))))) [linkage=external];
// DEFAULT-NEXT:     global %13 j: ptr<const i32> [storage=static] = conditional<ptr<const i32>>(ne<i32>(const<i32>(1), const<i32>(0)), null<ptr<const i32>>, addr_of<ptr<const i32>>(compound_literal %23 [storage=static] = const<i32>(26))) [linkage=internal];
// DEFAULT-NEXT:     global %14 k: i32 [storage=static] = add<i32, overflow=ub>(reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(24))), const<i32>(4)) [linkage=external];
// DEFAULT-NEXT:     global %15 l: i32 [storage=static] = reinterpret<i32, reason=explicit, fits=unknown>(truncate<u32, reason=explicit, fits=always>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
