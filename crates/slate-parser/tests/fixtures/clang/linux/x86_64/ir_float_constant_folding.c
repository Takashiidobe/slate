// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

enum {
  ARITHMETIC = (int)(1.0 / 3 * 3 + 0.5),
  NARROWED = (int)(float)2.9,
  SINGLE = (int)(3 * 1.5f),
  NEGATED = (int)-(2.5),
  COMPARED = 1.5 > 1,
  SELECTED = (int)(1 ? 2.5 : 3.5),
  EXTENDED = (int)(long double)7.25,
  NOT_ZERO = !0.0,
  SINGLE_ROUNDING = 0.1f + 0.2f == 0.3f,
  DOUBLE_ROUNDING = 0.1 + 0.2 == 0.3,
  EXTENDED_PRODUCT =
      (int)((long double)0.1 * 10 * 1000000000000000000.0L - 1000000000000000000.0L),
};

_Static_assert((int)(2.5 * 2) == 5, "");
char bound[(int)(2.5 * 2)];
struct Bits { int field : (int)(1.5 * 2); };

int values[] = {ARITHMETIC, NARROWED,  SINGLE,          NEGATED,
                COMPARED,   SELECTED,  EXTENDED,        NOT_ZERO,
                SINGLE_ROUNDING, DOUBLE_ROUNDING, EXTENDED_PRODUCT};

int select(int c) {
  switch (c) {
  case (int)(2.5 * 2):
    return 1;
  }
  return 0;
}

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
// IR-NEXT:         %0 ARITHMETIC = const<i32>(1);
// IR-NEXT:         %1 NARROWED = const<i32>(2);
// IR-NEXT:         %2 SINGLE = const<i32>(4);
// IR-NEXT:         %3 NEGATED = const<i32>(-2);
// IR-NEXT:         %4 COMPARED = const<i32>(1);
// IR-NEXT:         %5 SELECTED = const<i32>(2);
// IR-NEXT:         %6 EXTENDED = const<i32>(7);
// IR-NEXT:         %7 NOT_ZERO = const<i32>(1);
// IR-NEXT:         %8 SINGLE_ROUNDING = const<i32>(1);
// IR-NEXT:         %9 DOUBLE_ROUNDING = const<i32>(0);
// IR-NEXT:         %10 EXTENDED_PRODUCT = const<i32>(55);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type1 Bits = struct {
// IR-NEXT:         field0 field: i32 : 3;
// IR-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// IR-NEXT:     type @type2 = enum : i32 {
// IR-NEXT:         %0 OVERFLOWED = const<i32>(2147483647);
// IR-NEXT:         %1 NEGATIVE_OVERFLOW = const<i32>(-2147483648);
// IR-NEXT:         %2 INFINITE = const<i32>(2147483647);
// IR-NEXT:         %3 NOT_A_NUMBER = const<i32>(0);
// IR-NEXT:         %4 WRAPPED = const<i32>(-128);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %12 bound: array<i8, 5> [storage=static] [linkage=external];
// IR-NEXT:     global %14 values: array<i32, 11> [storage=static] [align=16] = aggregate<array<i32, 11>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(4), index3 = const<i32>(-2), index4 = const<i32>(1), index5 = const<i32>(2), index6 = const<i32>(7), index7 = const<i32>(1), index8 = const<i32>(1), index9 = const<i32>(0), index10 = const<i32>(55)) [linkage=external];
// IR-NEXT:     global %23 out_of_range: array<i32, 5> [storage=static] [align=16] = aggregate<array<i32, 5>, zero_fill=false>(index0 = const<i32>(2147483647), index1 = const<i32>(-2147483648), index2 = const<i32>(2147483647), index3 = const<i32>(0), index4 = const<i32>(-128)) [linkage=external];
// IR-NEXT:     fn %15 @select(%16 c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         switch %24 read<i32>(%16)
// IR-NEXT:             {
// IR-NEXT:                 case %24 const<i32>(5):
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:             }
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
