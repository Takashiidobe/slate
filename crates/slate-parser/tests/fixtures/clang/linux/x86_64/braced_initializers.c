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
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_P:[0-9]+]] P = struct {
// IR-NEXT:         field0 x: i32;
// IR-NEXT:         field1 y: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_Q:[0-9]+]] Q = struct {
// IR-NEXT:         field0 tag: array<i8, 4>;
// IR-NEXT:         field1 p: @type[[TYPE_P]];
// IR-NEXT:         field2 tail: i32;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 4, 12]];
// IR-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct {
// IR-NEXT:         field0 a: array<i32, 2>;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_N:[0-9]+]] N = struct {
// IR-NEXT:         field0 k: i32;
// IR-NEXT:         field1 <anonymous>: @type[[TYPE0:[0-9]+]];
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE0]] = struct {
// IR-NEXT:         field0 u: i32;
// IR-NEXT:         field1 v: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_U:[0-9]+]] U = union {
// IR-NEXT:         field0 i: i32;
// IR-NEXT:         field1 c: array<i8, 4>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     global %[[VALUE_ints:[0-9]+]] ints: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3)) [linkage=external];
// IR-NEXT:     global %[[VALUE_chars:[0-9]+]] chars: array<i8, 3> [storage=static] = aggregate<array<i8, 3>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(97)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(98)), index2 = truncate<i8, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// IR-NEXT:     global %[[VALUE_braced_string:[0-9]+]] braced_string: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([97, 98, 0]) [linkage=external];
// IR-NEXT:     global %[[VALUE_origin:[0-9]+]] origin: @type[[TYPE_P]] [storage=static] = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %[[VALUE_half:[0-9]+]] half: @type[[TYPE_P]] [storage=static] = aggregate<@type[[TYPE_P]], zero_fill=true>(field0 = const<i32>(1)) [linkage=external];
// IR-NEXT:     global %[[VALUE_empty:[0-9]+]] empty: @type[[TYPE_P]] [storage=static] = aggregate<@type[[TYPE_P]], zero_fill=true>() [linkage=external];
// IR-NEXT:     global %[[VALUE_swapped:[0-9]+]] swapped: @type[[TYPE_P]] [storage=static] = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: @type[[TYPE_Q]] [storage=static] = aggregate<@type[[TYPE_Q]], zero_fill=false>(field0 = code_units<array<i8, 4>>([97, 98, 99, 0]), field1 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)), field2 = const<i32>(5)) [linkage=external];
// IR-NEXT:     global %[[VALUE_elided:[0-9]+]] elided: @type[[TYPE_Q]] [storage=static] = aggregate<@type[[TYPE_Q]], zero_fill=false>(field0 = code_units<array<i8, 4>>([97, 98, 99, 0]), field1 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4)), field2 = const<i32>(5)) [linkage=external];
// IR-NEXT:     global %[[VALUE_dotted:[0-9]+]] dotted: @type[[TYPE_Q]] [storage=static] = aggregate<@type[[TYPE_Q]], zero_fill=true>(field1 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(8), field1 = const<i32>(9))) [linkage=external];
// IR-NEXT:     global %[[VALUE_elided_array:[0-9]+]] elided_array: @type[[TYPE_A]] [storage=static] = aggregate<@type[[TYPE_A]], zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), field1 = const<i32>(3)) [linkage=external];
// IR-NEXT:     global %[[VALUE_matrix:[0-9]+]] matrix: array<array<i32, 2>, 2> [storage=static] [align=16] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %[[VALUE_rows:[0-9]+]] rows: array<array<i32, 2>, 2> [storage=static] [align=16] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=true>(index0 = const<i32>(3))) [linkage=external];
// IR-NEXT:     global %[[VALUE_sparse:[0-9]+]] sparse: array<i32, 6> [storage=static] [align=16] = aggregate<array<i32, 6>, zero_fill=true>(index4 = const<i32>(1), index5 = const<i32>(2)) [linkage=external];
// IR-NEXT:     global %[[VALUE_ranged:[0-9]+]] ranged: array<i32, 8> [storage=static] [align=16] = aggregate<array<i32, 8>, zero_fill=true>(index1..=3 = const<i32>(7)) [linkage=external];
// IR-NEXT:     global %[[VALUE_first_member:[0-9]+]] first_member: @type[[TYPE_U]] [storage=static] = aggregate<@type[[TYPE_U]], zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// IR-NEXT:     global %[[VALUE_chosen:[0-9]+]] chosen: @type[[TYPE_U]] [storage=static] = aggregate<@type[[TYPE_U]], zero_fill=false>(field1 = aggregate<array<i8, 4>, zero_fill=true>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(1)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(2)))) [linkage=external];
// IR-NEXT:     global %[[VALUE_anonymous:[0-9]+]] anonymous: @type[[TYPE_N]] [storage=static] = aggregate<@type[[TYPE_N]], zero_fill=false>(field0 = const<i32>(1), field1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<i32>(2), field1 = const<i32>(3))) [linkage=external];
// IR-NEXT:     global %[[VALUE_anonymous_designated:[0-9]+]] anonymous_designated: @type[[TYPE_N]] [storage=static] = aggregate<@type[[TYPE_N]], zero_fill=true>(field1 = aggregate<@type[[TYPE0]], zero_fill=true>(field0 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %[[VALUE_points:[0-9]+]] points: array<@type[[TYPE_P]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE_P]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %[[VALUE_flat_points:[0-9]+]] flat_points: array<@type[[TYPE_P]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE_P]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)), index1 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(4))) [linkage=external];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// IR-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([98, 0]) [linkage=internal];
// IR-NEXT:     global %[[VALUE_names:[0-9]+]] names: array<ptr<const i8>, 2> [storage=static] [align=16] = aggregate<array<ptr<const i8>, 2>, zero_fill=false>(index0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str]])), index1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%[[VALUE_str_2]]))) [linkage=external];
// IR-NEXT:     global %[[VALUE_grid:[0-9]+]] grid: array<array<i8, 3>, 2> [storage=static] = aggregate<array<array<i8, 3>, 2>, zero_fill=false>(index0 = code_units<array<i8, 3>>([97, 98, 0]), index1 = code_units<array<i8, 3>>([99, 100, 0])) [linkage=external];
// IR-NEXT:     fn %[[VALUE_local:[0-9]+]] @local(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_p:[0-9]+]] p: @type[[TYPE_P]] [storage=automatic] = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = read<i32>(%[[VALUE_n]]), field1 = add<i32, overflow=ub>(read<i32>(%[[VALUE_n]]), const<i32>(1)));
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<i32, 3> [storage=automatic] = aggregate<array<i32, 3>, zero_fill=true>(index0 = read<i32>(%[[VALUE_n]]));
// IR-NEXT:         let %[[VALUE_copies:[0-9]+]] copies: array<@type[[TYPE_P]], 2> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_P]], 2>, zero_fill=false>(index0 = copy<@type[[TYPE_P]], reason=assign>(read<@type[[TYPE_P]]>(%[[VALUE_p]])), index1 = aggregate<@type[[TYPE_P]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(2)));
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(%[[VALUE_p]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_a]]), const<i32>(0))))), read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_P]]>, subtract=false, element=@type[[TYPE_P]], overflow=ub>(array_decay<ptr<@type[[TYPE_P]]>, length=Some(2)>(%[[VALUE_copies]]), const<i32>(1))))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
