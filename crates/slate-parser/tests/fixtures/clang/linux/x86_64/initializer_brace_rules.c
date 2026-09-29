typedef int v4 __attribute__((vector_size(16)));
struct S { int a; };
struct F { int n; int d[]; };
int scalar_empty = {};
int scalar_excess = {1, 2};
struct S nested_excess = {{1, 2}};
static int zero[] = {};
static int zero_rows[][8] = {};
_Static_assert(sizeof zero == 0, "");
_Static_assert(sizeof zero_rows == 0, "");
struct F flexible = {1, {2, 3}};
int f(v4 v) {
  v4 lanes[] = {v, v};
  _Static_assert(sizeof lanes == 2 * sizeof(v4), "");
  int local = {};
  return local + (int){} + lanes[1][0] + zero_rows[0][0];
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
// DEFAULT-NEXT:     type @type0 v4 = vector<i32, 4>;
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 F = struct {
// DEFAULT-NEXT:         field0 n: i32;
// DEFAULT-NEXT:         field1 d: array<i32, incomplete>;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %3 scalar_empty: i32 [storage=static] = aggregate<i32, zero_fill=true>() [linkage=external];
// DEFAULT-NEXT:     global %4 scalar_excess: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %5 nested_excess: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %6 zero: array<i32, 0> [storage=static] = aggregate<array<i32, 0>, zero_fill=false>() [linkage=internal];
// DEFAULT-NEXT:     global %7 zero_rows: array<array<i32, 8>, 0> [storage=static] = aggregate<array<array<i32, 8>, 0>, zero_fill=false>() [linkage=internal];
// DEFAULT-NEXT:     global %8 flexible: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = const<i32>(1), field1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3))) [linkage=external];
// DEFAULT-NEXT:     fn %9 @f(%10 v: vector<i32, 4>) -> i32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 lanes: array<vector<i32, 4>, 2> [storage=automatic] = aggregate<array<vector<i32, 4>, 2>, zero_fill=false>(index0 = read<vector<i32, 4>>(%10), index1 = read<vector<i32, 4>>(%10));
// DEFAULT-NEXT:         let %12 local: i32 [storage=automatic] = aggregate<i32, zero_fill=true>();
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%12), read<i32>(compound_literal %13 [storage=automatic] = aggregate<i32, zero_fill=true>())), read<i32>(lane(deref(ptr_offset<ptr<vector<i32, 4>>, subtract=false, element=vector<i32, 4>, overflow=ub>(array_decay<ptr<vector<i32, 4>>, length=Some(2)>(%11), const<i32>(1))), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(8)>(deref(ptr_offset<ptr<array<i32, 8>>, subtract=false, element=array<i32, 8>, overflow=ub>(array_decay<ptr<array<i32, 8>>, length=Some(0)>(%7), const<i32>(0)))), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
