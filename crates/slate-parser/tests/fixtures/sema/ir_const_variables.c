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
// IR-NEXT:     type @type0 constant = i32;
// IR-NEXT:     global %0 limit: i32 [storage=static] [const] = const<i32>(3) [linkage=external];
// IR-NEXT:     global %10 s: i32 [storage=static] [const] = const<i32>(6) [linkage=internal];
// IR-NEXT:     fn %2 @g() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %3 k: i32 [storage=automatic] [const] = const<i32>(4);
// IR-NEXT:         let %4 x: i32 [storage=automatic];
// IR-NEXT:         let %5 q: ptr<i32> [storage=automatic] [const] = addr_of<ptr<i32>>(%4);
// IR-NEXT:         let %6 p: ptr<const i32> [storage=automatic] = pointer_cast<ptr<const i32>, reason=assign>(addr_of<ptr<i32>>(%4));
// IR-NEXT:         let %7 table: array<i32, 2> [storage=automatic] [const] = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2));
// IR-NEXT:         let %8 t: i32 [storage=automatic] [const] = const<i32>(7);
// IR-NEXT:         let %9 c: i32 [storage=automatic] [const] [constexpr] = const<i32>(5);
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(deref(read<ptr<i32>>(%5))), read<i32>(deref(read<ptr<const i32>>(%6)))), read<i32>(%3)), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(2)>(%7), const<i32>(1))))), read<i32>(%8)), read<i32>(%9)), read<i32>(%10)), read<i32>(%0));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
