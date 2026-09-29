// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=c23

int counter(void) {
  static int calls = 3;
  static int zero;
  static int table[3] = {1, 2, 3};
  calls += 1;
  return calls + zero + table[1];
}

int other(void) {
  static int calls;
  return ++calls;
}

int calls = 7;

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
// IR-NEXT:     global %[[VALUE_calls:[0-9]+]] calls: i32 [storage=static] = const<i32>(3) [linkage=internal];
// IR-NEXT:     global %[[VALUE_zero:[0-9]+]] zero: i32 [storage=static] [linkage=internal];
// IR-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3)) [linkage=internal];
// IR-NEXT:     global %[[VALUE_calls_2:[0-9]+]] calls: i32 [storage=static] [linkage=internal];
// IR-NEXT:     global %[[VALUE_calls_3:[0-9]+]] calls: i32 [storage=static] = const<i32>(7) [linkage=external];
// IR-NEXT:     fn %[[VALUE_counter:[0-9]+]] @counter() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_calls]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_calls]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_calls]]), read<i32>(%[[VALUE_zero]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_table]]), const<i32>(1)))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_other:[0-9]+]] @other() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_calls_2]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// IR-NEXT:         write<i32>(%[[VALUE_calls_2]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         return read<i32>(%[[VALUE3]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
