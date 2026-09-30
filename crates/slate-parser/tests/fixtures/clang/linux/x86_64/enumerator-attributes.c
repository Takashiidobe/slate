enum Standard {
  BRACKET_WITH_VALUE [[deprecated]] = 1,
  BRACKET_WITH_MESSAGE [[deprecated("use BRACKET_WITH_VALUE")]] = 2,
  BRACKET_WITHOUT_VALUE [[maybe_unused]],
  BRACKET_UNKNOWN [[vendor::unrecognized]] = 4,
};

enum Gnu {
  GNU_WITH_VALUE __attribute__((deprecated)) = 1,
  GNU_WITHOUT_VALUE __attribute__((unused)),
  GNU_UNKNOWN __attribute__((unrecognized_attribute)),
  GNU_BOTH_SPELLINGS __attribute__((deprecated)) [[maybe_unused]] = 8,
  PLAIN = 16,
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
// DEFAULT-NEXT:     type @type[[TYPE_Standard:[0-9]+]] Standard = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_BRACKET_WITH_VALUE:[0-9]+]] BRACKET_WITH_VALUE = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_BRACKET_WITH_MESSAGE:[0-9]+]] BRACKET_WITH_MESSAGE = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_BRACKET_WITHOUT_VALUE:[0-9]+]] BRACKET_WITHOUT_VALUE = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_BRACKET_UNKNOWN:[0-9]+]] BRACKET_UNKNOWN = const<i32>(4);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Gnu:[0-9]+]] Gnu = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_BRACKET_WITH_VALUE]] GNU_WITH_VALUE = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_BRACKET_WITH_MESSAGE]] GNU_WITHOUT_VALUE = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_BRACKET_WITHOUT_VALUE]] GNU_UNKNOWN = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_BRACKET_UNKNOWN]] GNU_BOTH_SPELLINGS = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE_PLAIN:[0-9]+]] PLAIN = const<i32>(16);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
