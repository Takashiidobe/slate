// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

enum {
  OVERFLOWED = (int)1e10,
  NEGATIVE_OVERFLOW = (int)-1e10,
  INFINITE = (int)(1.0 / 0.0),
  NOT_A_NUMBER = (int)(0.0 / 0.0),
  WRAPPED = (int)(signed char)-200.7,
};
int out_of_range[] = {OVERFLOWED, NEGATIVE_OVERFLOW, INFINITE, NOT_A_NUMBER, WRAPPED};

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
// IR-NEXT:     type @type0 = enum : i32 {
// IR-NEXT:         %0 OVERFLOWED = const<i32>(1410065408);
// IR-NEXT:         %1 NEGATIVE_OVERFLOW = const<i32>(-1410065408);
// IR-NEXT:         %2 INFINITE = const<i32>(0);
// IR-NEXT:         %3 NOT_A_NUMBER = const<i32>(0);
// IR-NEXT:         %4 WRAPPED = const<i32>(56);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %6 out_of_range: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(1410065408), index1 = const<i32>(-1410065408), index2 = const<i32>(0), index3 = const<i32>(0), index4 = const<i32>(56)) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
