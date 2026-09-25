int scalar = 7;
char message[6] = "hello";
int matrix[2][2] = {{1, 2}, {3, 4}};
struct Point {
  int x;
  int y;
};
struct Point point = {.y = 9, .x = 4};
int values[4] = {[2] = 8, [0] = 1};
int selected =
#ifdef ENABLED
  11;
#else
  22;
#endif

int main() {
  return 0;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES ENABLED ENABLED

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
// DEFAULT-NEXT:     type @type0 Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %0 scalar: i32 [storage=static] = const<i32>(7) [linkage=external];
// DEFAULT-NEXT:     global %1 message: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=external];
// DEFAULT-NEXT:     global %2 matrix: array<array<i32, 2>, 2> [storage=static] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4))) [linkage=external];
// DEFAULT-NEXT:     global %4 point: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(9)) [linkage=external];
// DEFAULT-NEXT:     global %5 values: array<i32, 4> [storage=static] = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(1), index2 = const<i32>(8)) [linkage=external];
// DEFAULT-NEXT:     global %6 selected: i32 [storage=static] = const<i32>(22) [linkage=external];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ENABLED
// ENABLED: module {
// ENABLED-NEXT:     target "x86_64-unknown-linux-gnu" {
// ENABLED-NEXT:         endian = little;
// ENABLED-NEXT:         pointer [size=8, align=8];
// ENABLED-NEXT:         stack_alignment = 16;
// ENABLED-NEXT:         long_double = f80;
// ENABLED-NEXT:         storage bool [size=1, align=1];
// ENABLED-NEXT:         storage i8, u8 [size=1, align=1];
// ENABLED-NEXT:         storage i16, u16 [size=2, align=2];
// ENABLED-NEXT:         storage i32, u32 [size=4, align=4];
// ENABLED-NEXT:         storage i64, u64 [size=8, align=8];
// ENABLED-NEXT:         storage i128, u128 [size=16, align=16];
// ENABLED-NEXT:         storage bf16 [size=2, align=2];
// ENABLED-NEXT:         storage f16 [size=2, align=2];
// ENABLED-NEXT:         storage f32 [size=4, align=4];
// ENABLED-NEXT:         storage f64 [size=8, align=8];
// ENABLED-NEXT:         storage f80 [size=16, align=16];
// ENABLED-NEXT:         storage f128 [size=16, align=16];
// ENABLED-NEXT:         storage d32 [size=4, align=4];
// ENABLED-NEXT:         storage d64 [size=8, align=8];
// ENABLED-NEXT:         storage d128 [size=16, align=16];
// ENABLED-NEXT:     }
// ENABLED-NEXT:     type @type0 Point = struct {
// ENABLED-NEXT:         field0 x: i32;
// ENABLED-NEXT:         field1 y: i32;
// ENABLED-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// ENABLED-NEXT:     global %0 scalar: i32 [storage=static] = const<i32>(7) [linkage=external];
// ENABLED-NEXT:     global %1 message: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=external];
// ENABLED-NEXT:     global %2 matrix: array<array<i32, 2>, 2> [storage=static] = aggregate<array<array<i32, 2>, 2>, zero_fill=false>(index0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2)), index1 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4))) [linkage=external];
// ENABLED-NEXT:     global %4 point: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(4), field1 = const<i32>(9)) [linkage=external];
// ENABLED-NEXT:     global %5 values: array<i32, 4> [storage=static] = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(1), index2 = const<i32>(8)) [linkage=external];
// ENABLED-NEXT:     global %6 selected: i32 [storage=static] = const<i32>(11) [linkage=external];
// ENABLED-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// ENABLED-NEXT:         return const<i32>(0);
// ENABLED-NEXT:     }
// ENABLED-NEXT: }
// SLATE-FILECHECK-END ENABLED
