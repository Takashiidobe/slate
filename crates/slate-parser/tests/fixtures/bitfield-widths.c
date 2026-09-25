enum Width { WIDTH = 5 };

struct BitFields {
  unsigned first : 1 + 2, second : WIDTH;
  unsigned : 0;
  unsigned final : sizeof(int) * 2;
};

struct Container {
  struct {
    unsigned nested : 7;
  } bits;
};

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
// DEFAULT-NEXT:     type @type0 Width = enum : u32 {
// DEFAULT-NEXT:         %0 WIDTH = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 BitFields = struct {
// DEFAULT-NEXT:         field0 first: u32 : 3;
// DEFAULT-NEXT:         field1 second: u32 : 5;
// DEFAULT-NEXT:         field2 <anonymous>: u32 : 0;
// DEFAULT-NEXT:         field3 final: u32 : 8;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0, 4, 4], bit_offsets=[Some(0), Some(3), Some(32), Some(32)], bit_units=[(0, 1), (4, 1)], field_units=[Some(0), Some(0), None, Some(1)]];
// DEFAULT-NEXT:     type @type2 Container = struct {
// DEFAULT-NEXT:         field0 bits: @type3;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 nested: u32 : 7;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 1)], field_units=[Some(0)]];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
