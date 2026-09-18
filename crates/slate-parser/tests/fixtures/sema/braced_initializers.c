// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

struct P { int x; int y; };
struct Q { char tag[4]; struct P p; int tail; };
struct A { int a[2]; int b; };
struct N { int k; struct { int u; int v; }; };
union U { int i; char c[4]; };

int ints[] = {1, 2, 3};
char chars[] = {97, 98, 0};
char braced_string[] = {"ab"};
struct P origin = {1, 2};
struct P half = {1};
struct P empty = {};
struct P swapped = {.y = 2, .x = 1};
struct Q nested = {"abc", {3, 4}, 5};
struct Q elided = {"abc", 3, 4, 5};
struct Q dotted = {.p.y = 9, .p.x = 8};
struct A elided_array = {1, 2, 3};
int matrix[2][2] = {1, 2, 3, 4};
int rows[][2] = {{1, 2}, {3}};
int sparse[6] = {[4] = 1, 2};
int ranged[8] = {[1 ... 3] = 7};
union U first_member = {1};
union U chosen = {.c = {1, 2}};
struct N anonymous = {1, 2, 3};
struct N anonymous_designated = {.u = 4};
struct P points[] = {{1, 2}, {3, 4}};
struct P flat_points[] = {1, 2, 3, 4};
const char *names[] = {"a", "b"};
char grid[2][3] = {"ab", "cd"};

int local(int n) {
  struct P p = {n, n + 1};
  int a[3] = {n};
  struct P copies[2] = {p, {1, 2}};
  return p.x + a[0] + copies[1].y;
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
// IR-NEXT:     type @type0 P = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 Q = struct {
// IR-NEXT:         field0 tag: array<i8, 4>;
// IR-NEXT:         field1 p: @type0;
// IR-NEXT:         field2 tail: i32;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 4, 12]];
// IR-NEXT:     type @type2 A = struct {
// IR-NEXT:         field0 a: array<i32, 2>;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// IR-NEXT:     type @type3 N = struct {
// IR-NEXT:         field0 k: i32;
// IR-NEXT:         field1 <anonymous>: @type4;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type4 = struct {
// IR-NEXT:         field0 u: i32;
// IR-NEXT:         field1 v: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type5 U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 c: array<i8, 4>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     global %6 ints: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3)) [linkage=external];
// IR-NEXT:     global %7 chars: array<i8, 3> [storage=static] = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(98)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// IR-NEXT:     global %8 braced_string: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([97, 98, 0]) [linkage=external];
// IR-NEXT:     global %9 origin: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %10 half: @type0 [storage=static] = aggregate<@type0, zero_fill=true>(field0 = const<i32>(1)) [linkage=external];
// IR-NEXT:     global %11 empty: @type0 [storage=static] = aggregate<@type0, zero_fill=true>() [linkage=external];
// IR-NEXT:     global %12 swapped: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %13 nested: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = code_units<array<i8, 4>>([97, 98, 99, 0]), field1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)), field2 = const<i32>(5)) [linkage=external];
// IR-NEXT:     global %14 elided: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = code_units<array<i8, 4>>([97, 98, 99, 0]), field1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)), field2 = const<i32>(5)) [linkage=external];
// IR-NEXT:     global %15 dotted: @type1 [storage=static] = aggregate<@type1, zero_fill=true>(field1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(8), field1 = const<i32>(9))) [linkage=external];
// IR-NEXT:     global %16 elided_array: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), field1 = const<i32>(3)) [linkage=external];
// IR-NEXT:     global %17 matrix: array<array<i32, 2>, 2> [storage=static] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %18 rows: array<array<i32, 2>, 2> [storage=static] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(3))) [linkage=external];
// IR-NEXT:     global %19 sparse: array<i32, 6> [storage=static] = aggregate<array<i32, 6>, zero_fill=true>(index4 = const<i32>(1), index5 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %20 ranged: array<i32, 8> [storage=static] = aggregate<array<i32, 8>, zero_fill=true>(index1..=3 = const<i32>(7)) [linkage=external];
// IR-NEXT:     global %21 first_member: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// IR-NEXT:     global %22 chosen: @type5 [storage=static] = aggregate<@type5, zero_fill=false>(field1 = aggregate<array<i8, 4>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// IR-NEXT:     global %23 anonymous: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = const<i32>(1), field1 = aggregate<@type4, zero_fill=false>(field0 = const<i32>(2), field1 = const<i32>(3))) [linkage=external];
// IR-NEXT:     global %24 anonymous_designated: @type3 [storage=static] = aggregate<@type3, zero_fill=true>(field1 = aggregate<@type4, zero_fill=true>(field0 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %25 points: array<@type0, 2> [storage=static] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %26 flat_points: array<@type0, 2> [storage=static] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %34 .str34: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// IR-NEXT:     global %35 .str35: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// IR-NEXT:     global %27 names: array<ptr<const i8>, 2> [storage=static] = aggregate<array<ptr<const i8>, 2>, zero_fill=false>(index0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%34)), index1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%35))) [linkage=external];
// IR-NEXT:     global %28 grid: array<array<i8, 3>, 2> [storage=static] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = code_units<array<i8, 3>>([97, 98, 0]), index1 = code_units<array<i8, 3>>([99, 100, 0])) [linkage=external];
// IR-NEXT:     fn %29 @local(%30 n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %31 p: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = read<i32>(%30), field1 = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1)));
// IR-NEXT:         let %32 a: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=true>(index0 = read<i32>(%30));
// IR-NEXT:         let %33 copies: array<@type0, 2> [storage=automatic] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = copy<@type0, reason=assign>(read<@type0>(%31)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)));
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(%31)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%32), const<i32>(0))))), read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%33), const<i32>(1))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
