struct outer {
  unsigned long bits[128 / sizeof(unsigned long)];
  union {
    int i;
    char c;
  } value;
#ifdef WITH_EXTRA
  int extra;
#else
  int fallback;
#endif
};

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES EXTRA WITH_EXTRA

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
// DEFAULT-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// DEFAULT-NEXT:         field0 bits: array<u64, 16>;
// DEFAULT-NEXT:         field1 value: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field2 fallback: i32;
// DEFAULT-NEXT:     } [size=136, align=8, offsets=[0, 128, 132]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = union {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 c: i8;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN EXTRA
// EXTRA: module {
// EXTRA-NEXT:     target "x86_64-unknown-linux-gnu" {
// EXTRA-NEXT:         endian = little;
// EXTRA-NEXT:         pointer [size=8, align=8];
// EXTRA-NEXT:         stack_alignment = 16;
// EXTRA-NEXT:         long_double = f80;
// EXTRA-NEXT:         storage bool [size=1, align=1];
// EXTRA-NEXT:         storage i8, u8 [size=1, align=1];
// EXTRA-NEXT:         storage i16, u16 [size=2, align=2];
// EXTRA-NEXT:         storage i32, u32 [size=4, align=4];
// EXTRA-NEXT:         storage i64, u64 [size=8, align=8];
// EXTRA-NEXT:         storage i128, u128 [size=16, align=16];
// EXTRA-NEXT:         storage bf16 [size=2, align=2];
// EXTRA-NEXT:         storage f16 [size=2, align=2];
// EXTRA-NEXT:         storage f32 [size=4, align=4];
// EXTRA-NEXT:         storage f64 [size=8, align=8];
// EXTRA-NEXT:         storage f80 [size=16, align=16];
// EXTRA-NEXT:         storage f128 [size=16, align=16];
// EXTRA-NEXT:         storage d32 [size=4, align=4];
// EXTRA-NEXT:         storage d64 [size=8, align=8];
// EXTRA-NEXT:         storage d128 [size=16, align=16];
// EXTRA-NEXT:     }
// EXTRA-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// EXTRA-NEXT:         field0 bits: array<u64, 16>;
// EXTRA-NEXT:         field1 value: @type[[TYPE0:[0-9]+]];
// EXTRA-NEXT:         field2 extra: i32;
// EXTRA-NEXT:     } [size=136, align=8, offsets=[0, 128, 132]];
// EXTRA-NEXT:     type @type[[TYPE0]] = union {
// EXTRA-NEXT:         field0 i: i32;
// EXTRA-NEXT:         field1 c: i8;
// EXTRA-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// EXTRA-NEXT: }
// SLATE-FILECHECK-END EXTRA
