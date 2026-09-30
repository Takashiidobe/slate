enum flags { A = 1 << 3, B = A + 1, C, D = sizeof(int) * 4 };

typedef long word;

int table[] = { [A] = 1, [D + 1] = 2, [C ... D] = 3 };

int select_int(int a) {
  return _Generic(a, int: 1, word: 2, default: 3);
}

int select_type(void) {
  return _Generic(word, long: 4, char *: 5, default: 6);
}

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
// DEFAULT-NEXT:     type @type[[TYPE_flags:[0-9]+]] flags = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_A:[0-9]+]] A = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE_B:[0-9]+]] B = const<i32>(9);
// DEFAULT-NEXT:         %[[VALUE_C:[0-9]+]] C = const<i32>(10);
// DEFAULT-NEXT:         %[[VALUE_D:[0-9]+]] D = const<i32>(16);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_word:[0-9]+]] word = i64;
// DEFAULT-NEXT:     global %[[VALUE_table:[0-9]+]] table: array<i32, 18> [storage=static] [align=16] = aggregate<array<i32, 18>, zero_fill=true>(index8 = const<i32>(1), index10..=16 = const<i32>(3), index17 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_select_int:[0-9]+]] @select_int(%[[VALUE_a:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_select_type:[0-9]+]] @select_type() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
