enum { N = 4 };

struct s {
  int a;
  static_assert (N == 4);
  _Static_assert (sizeof (int) == 4, "int");
  int b[N];
};

union u {
  _Static_assert (N > 0, "positive");
  int i;
};

int f(void) {
  struct local {
    _Static_assert (N == 4, "local");
    int x;
  } v = {N};
  return v.x + (int)sizeof (struct s) + (int)sizeof (union u);
}

// SLATE-FILECHECK-STD DEFAULT c23
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_N:[0-9]+]] N = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: array<i32, 4>;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_local:[0-9]+]] local = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: @type[[TYPE_local]] [storage=automatic] = aggregate<@type[[TYPE_local]], zero_fill=false>(field0 = const<i32>(4));
// DEFAULT-NEXT:         return add<i32>(add<i32>(read<i32>(field0(%[[VALUE_v]])), reinterpret<i32>(truncate<u32>(const<u64>(20)))), reinterpret<i32>(truncate<u32>(const<u64>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
