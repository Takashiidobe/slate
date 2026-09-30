#ifdef USE_INT
typedef int Value;
#else
typedef char Value;
#endif

Value value;

#ifdef ONLY_LEFT
typedef int LeftOnly;
LeftOnly left_value;
#else
typedef int RightOnly;
RightOnly right_value;
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES INT USE_INT
// SLATE-FILECHECK-DEFINES LEFT ONLY_LEFT

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
// DEFAULT-NEXT:     type @type[[TYPE_Value:[0-9]+]] Value = i8;
// DEFAULT-NEXT:     type @type[[TYPE_RightOnly:[0-9]+]] RightOnly = i32;
// DEFAULT-NEXT:     global %[[VALUE_value:[0-9]+]] value: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_right_value:[0-9]+]] right_value: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN INT
// INT: module {
// INT-NEXT:     target "x86_64-unknown-linux-gnu" {
// INT-NEXT:         endian = little;
// INT-NEXT:         pointer [size=8, align=8];
// INT-NEXT:         stack_alignment = 16;
// INT-NEXT:         long_double = f80;
// INT-NEXT:         storage bool [size=1, align=1];
// INT-NEXT:         storage i8, u8 [size=1, align=1];
// INT-NEXT:         storage i16, u16 [size=2, align=2];
// INT-NEXT:         storage i32, u32 [size=4, align=4];
// INT-NEXT:         storage i64, u64 [size=8, align=8];
// INT-NEXT:         storage i128, u128 [size=16, align=16];
// INT-NEXT:         storage bf16 [size=2, align=2];
// INT-NEXT:         storage f16 [size=2, align=2];
// INT-NEXT:         storage f32 [size=4, align=4];
// INT-NEXT:         storage f64 [size=8, align=8];
// INT-NEXT:         storage f80 [size=16, align=16];
// INT-NEXT:         storage f128 [size=16, align=16];
// INT-NEXT:         storage d32 [size=4, align=4];
// INT-NEXT:         storage d64 [size=8, align=8];
// INT-NEXT:         storage d128 [size=16, align=16];
// INT-NEXT:     }
// INT-NEXT:     type @type[[TYPE_Value:[0-9]+]] Value = i32;
// INT-NEXT:     type @type[[TYPE_RightOnly:[0-9]+]] RightOnly = i32;
// INT-NEXT:     global %[[VALUE_value:[0-9]+]] value: i32 [storage=static] [linkage=external];
// INT-NEXT:     global %[[VALUE_right_value:[0-9]+]] right_value: i32 [storage=static] [linkage=external];
// INT-NEXT: }
// SLATE-FILECHECK-END INT
// SLATE-FILECHECK-BEGIN LEFT
// LEFT: module {
// LEFT-NEXT:     target "x86_64-unknown-linux-gnu" {
// LEFT-NEXT:         endian = little;
// LEFT-NEXT:         pointer [size=8, align=8];
// LEFT-NEXT:         stack_alignment = 16;
// LEFT-NEXT:         long_double = f80;
// LEFT-NEXT:         storage bool [size=1, align=1];
// LEFT-NEXT:         storage i8, u8 [size=1, align=1];
// LEFT-NEXT:         storage i16, u16 [size=2, align=2];
// LEFT-NEXT:         storage i32, u32 [size=4, align=4];
// LEFT-NEXT:         storage i64, u64 [size=8, align=8];
// LEFT-NEXT:         storage i128, u128 [size=16, align=16];
// LEFT-NEXT:         storage bf16 [size=2, align=2];
// LEFT-NEXT:         storage f16 [size=2, align=2];
// LEFT-NEXT:         storage f32 [size=4, align=4];
// LEFT-NEXT:         storage f64 [size=8, align=8];
// LEFT-NEXT:         storage f80 [size=16, align=16];
// LEFT-NEXT:         storage f128 [size=16, align=16];
// LEFT-NEXT:         storage d32 [size=4, align=4];
// LEFT-NEXT:         storage d64 [size=8, align=8];
// LEFT-NEXT:         storage d128 [size=16, align=16];
// LEFT-NEXT:     }
// LEFT-NEXT:     type @type[[TYPE_Value:[0-9]+]] Value = i8;
// LEFT-NEXT:     type @type[[TYPE_LeftOnly:[0-9]+]] LeftOnly = i32;
// LEFT-NEXT:     global %[[VALUE_value:[0-9]+]] value: i8 [storage=static] [linkage=external];
// LEFT-NEXT:     global %[[VALUE_left_value:[0-9]+]] left_value: i32 [storage=static] [linkage=external];
// LEFT-NEXT: }
// SLATE-FILECHECK-END LEFT
