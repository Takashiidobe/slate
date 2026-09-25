enum Values {
  FIRST = 1 + 2 * 3,
  SECOND = (4 << 1),
};
int values[1 + 2 * 3];
int flags[(4 < 5) && 1];
#if 1 + 2 * 3 > 6
int selected[2 + 3];
#else
int rejected[0];
#endif

int main() {
  return 0;
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
// DEFAULT-NEXT:     type @type0 Values = enum : u32 {
// DEFAULT-NEXT:         %0 FIRST = const<i32>(7);
// DEFAULT-NEXT:         %1 SECOND = const<i32>(8);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %3 values: array<i32, 7> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 flags: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 selected: array<i32, 5> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
