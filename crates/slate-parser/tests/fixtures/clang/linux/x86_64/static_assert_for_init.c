int f(int n) {
  int total = 0;
  for (static_assert(sizeof(int) == 4, "int is 32 bits"); n > 0; n--)
    total += n;
  for (_Static_assert(1, "c11 spelling"); total > 100;)
    total /= 2;
  return total;
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
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_n:[0-9]+]] n: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: gt<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: gt<i32>(read<i32>(%[[VALUE_total]]), const<i32>(100))
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE6]]), const<i32>(2));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
