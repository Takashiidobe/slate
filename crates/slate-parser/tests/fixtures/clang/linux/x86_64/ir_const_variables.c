// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

const int limit = 3;

typedef const int constant;

int g(void) {
  const int k = 4;
  int x;
  int *const q = &x;
  const int *p = &x;
  const int table[2] = {1, 2};
  constant t = 7;
  constexpr int c = 5;
  static const int s = 6;
  return *q + *p + k + table[1] + t + c + s + limit;
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
// IR-NEXT:     type @type[[TYPE_constant:[0-9]+]] constant = i32;
// IR-NEXT:     global %[[VALUE_limit:[0-9]+]] limit: i32 [storage=static] [const] = const<i32>(3) [linkage=external];
// IR-NEXT:     global %[[VALUE_s:[0-9]+]] s: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] [const] = const<i32>(4);
// IR-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i32> [storage=automatic] [const] = addr_of<ptr<i32>>(%[[VALUE_x]]);
// IR-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(addr_of<ptr<i32>>(%[[VALUE_x]]));
// IR-NEXT:         let %[[VALUE_table:[0-9]+]] table: array<i32, 2> [storage=automatic] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2));
// IR-NEXT:         let %[[VALUE_t:[0-9]+]] t: i32 [storage=automatic] [const] = const<i32>(7);
// IR-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] [const] [constexpr] = const<i32>(5);
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%[[VALUE_q]]))), read<i32>(deref(read<ptr<const i32>>(%[[VALUE_p]])))), read<i32>(%[[VALUE_k]])), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(2)>(%[[VALUE_table]]), const<i32>(1))))), read<i32>(%[[VALUE_t]])), read<i32>(%[[VALUE_c]])), read<i32>(%[[VALUE_s]])), read<i32>(%[[VALUE_limit]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
