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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %1 calls: i32 [storage=static] = const<i32>(3) [linkage=internal];
// IR-NEXT:     global %2 zero: i32 [storage=static] [linkage=internal];
// IR-NEXT:     global %3 table: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3)) [linkage=internal];
// IR-NEXT:     global %5 calls: i32 [storage=static] [linkage=internal];
// IR-NEXT:     global %6 calls: i32 [storage=static] = const<i32>(7) [linkage=external];
// IR-NEXT:     fn %0 @counter() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %7: i32 [synthetic] = read<i32>(%1);
// IR-NEXT:         let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// IR-NEXT:         write<i32>(%1, read<i32>(%8));
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%1), read<i32>(%2)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(3)>(%3), const<i32>(1)))));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @other() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %9: i32 [synthetic] = read<i32>(%5);
// IR-NEXT:         let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// IR-NEXT:         write<i32>(%5, read<i32>(%10));
// IR-NEXT:         return read<i32>(%10);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
