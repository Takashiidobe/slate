// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

struct S { int a[3]; int z; };
struct P { int x; int y; };
struct T { struct P p[2]; int k; };
struct F { int n; char d[]; };
struct G { char c; long d[]; };

struct S s = {.a = 1, 2, 3, 4};
struct T t = {.p[1] = 5, 6, 7};
struct F f = {1};
struct F named = {.n = 2};
struct F braced = {3, {'a', 'b'}};
struct F elided = {4, 'x', 'y', 'z'};
struct F designated = {.d = {'q'}};
struct G wide = {'c', {7, 8}};
static_assert(sizeof(struct F) == 4);
static_assert(sizeof(braced) == 4);
static_assert(sizeof(struct G) == 8);
static_assert(_Alignof(struct G) == 8);

int local(void) {
  struct S l = {.a = 1, 2, .z = 9};
  return l.a[1];
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 S = struct {
// IR-NEXT:         field0 a: array<i32, 3>;
// IR-NEXT:         field1 z: i32;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 12]];
// IR-NEXT:     type @type1 P = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type2 T = struct {
// IR-NEXT:         field0 p: array<@type1, 2>;
// IR-NEXT:         field1 k: i32;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 16]];
// IR-NEXT:     type @type3 F = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 d: array<i8, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type4 G = struct {
// IR-NEXT:         field0 c: i8;
// IR-NEXT:         field1 d: array<i64, incomplete>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// IR-NEXT:     global %5 s: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3)), field1 = const<i32>(4)) [linkage=external];
// IR-NEXT:     global %6 t: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = aggregate<array<@type1, 2>, zero_fill=true>(index1 = aggregate<@type1, zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(6))), field1 = const<i32>(7)) [linkage=external];
// IR-NEXT:     global %7 f: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// IR-NEXT:     global %8 named: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %9 braced: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = const<i32>(3), field1 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(98)))) [linkage=external];
// IR-NEXT:     global %10 elided: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = const<i32>(4), field1 = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(120)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(121)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(122)))) [linkage=external];
// IR-NEXT:     global %11 designated: @type3 [storage=static] = aggregate<@type3, zero_fill=true>(field1 = aggregate<array<i8, 1>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(113)))) [linkage=external];
// IR-NEXT:     global %12 wide: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(99)), field1 = aggregate<array<i64, 2>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(7)), index1 = widen<i64, reason=assign>(const<i32>(8)))) [linkage=external];
// IR-NEXT:     fn %13 @local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %14 l: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = aggregate<array<i32, 3>, zero_fill=true>(index0 = const<i32>(1), index1 = const<i32>(2)), field1 = const<i32>(9));
// IR-NEXT:         return read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(field0(%14)), const<i32>(1))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
