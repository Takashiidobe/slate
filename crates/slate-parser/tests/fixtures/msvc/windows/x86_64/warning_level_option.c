long level = _MSVC_WARNING_LEVEL;

// SLATE-FILECHECK-DEFINES NONE
// SLATE-FILECHECK-DEFINES W3
// SLATE-FILECHECK-PREFIX-ARGS W3 /W3
// SLATE-FILECHECK-DEFINES SEPARATE
// SLATE-FILECHECK-PREFIX-ARGS SEPARATE /W 4
// SLATE-FILECHECK-DEFINES OUT_OF_RANGE
// SLATE-FILECHECK-PREFIX-ARGS OUT_OF_RANGE /W4 /W5
// SLATE-FILECHECK-DEFINES NO_WARNINGS
// SLATE-FILECHECK-PREFIX-ARGS NO_WARNINGS /W4 /w
// SLATE-FILECHECK-DEFINES LAST_WINS
// SLATE-FILECHECK-PREFIX-ARGS LAST_WINS /w /W2
// SLATE-FILECHECK-DEFINES WALL
// SLATE-FILECHECK-PREFIX-ARGS WALL /Wall /W2 /w

// SLATE-FILECHECK-BEGIN NONE
// NONE: module {
// NONE-NEXT:     target "x86_64-pc-windows-msvc" {
// NONE-NEXT:         endian = little;
// NONE-NEXT:         pointer [size=8, align=8];
// NONE-NEXT:         stack_alignment = 16;
// NONE-NEXT:         long_double = f64;
// NONE-NEXT:         storage bool [size=1, align=1];
// NONE-NEXT:         storage i8, u8 [size=1, align=1];
// NONE-NEXT:         storage i16, u16 [size=2, align=2];
// NONE-NEXT:         storage i32, u32 [size=4, align=4];
// NONE-NEXT:         storage i64, u64 [size=8, align=8];
// NONE-NEXT:         storage i128, u128 [size=16, align=16];
// NONE-NEXT:         storage bf16 [size=2, align=2];
// NONE-NEXT:         storage f16 [size=2, align=2];
// NONE-NEXT:         storage f32 [size=4, align=4];
// NONE-NEXT:         storage f64 [size=8, align=8];
// NONE-NEXT:         storage f128 [size=16, align=16];
// NONE-NEXT:         storage d32 [size=4, align=4];
// NONE-NEXT:         storage d64 [size=8, align=8];
// NONE-NEXT:         storage d128 [size=16, align=16];
// NONE-NEXT:     }
// NONE-NEXT:     global %[[VALUE_level:[0-9]+]] level: i32 [storage=static] = const<i32>(1) [linkage=external];
// NONE-NEXT: }
// SLATE-FILECHECK-END NONE
// SLATE-FILECHECK-BEGIN W3
// W3: module {
// W3-NEXT:     target "x86_64-pc-windows-msvc" {
// W3-NEXT:         endian = little;
// W3-NEXT:         pointer [size=8, align=8];
// W3-NEXT:         stack_alignment = 16;
// W3-NEXT:         long_double = f64;
// W3-NEXT:         storage bool [size=1, align=1];
// W3-NEXT:         storage i8, u8 [size=1, align=1];
// W3-NEXT:         storage i16, u16 [size=2, align=2];
// W3-NEXT:         storage i32, u32 [size=4, align=4];
// W3-NEXT:         storage i64, u64 [size=8, align=8];
// W3-NEXT:         storage i128, u128 [size=16, align=16];
// W3-NEXT:         storage bf16 [size=2, align=2];
// W3-NEXT:         storage f16 [size=2, align=2];
// W3-NEXT:         storage f32 [size=4, align=4];
// W3-NEXT:         storage f64 [size=8, align=8];
// W3-NEXT:         storage f128 [size=16, align=16];
// W3-NEXT:         storage d32 [size=4, align=4];
// W3-NEXT:         storage d64 [size=8, align=8];
// W3-NEXT:         storage d128 [size=16, align=16];
// W3-NEXT:     }
// W3-NEXT:     global %[[VALUE_level:[0-9]+]] level: i32 [storage=static] = const<i32>(3) [linkage=external];
// W3-NEXT: }
// SLATE-FILECHECK-END W3
// SLATE-FILECHECK-BEGIN SEPARATE
// SEPARATE: module {
// SEPARATE-NEXT:     target "x86_64-pc-windows-msvc" {
// SEPARATE-NEXT:         endian = little;
// SEPARATE-NEXT:         pointer [size=8, align=8];
// SEPARATE-NEXT:         stack_alignment = 16;
// SEPARATE-NEXT:         long_double = f64;
// SEPARATE-NEXT:         storage bool [size=1, align=1];
// SEPARATE-NEXT:         storage i8, u8 [size=1, align=1];
// SEPARATE-NEXT:         storage i16, u16 [size=2, align=2];
// SEPARATE-NEXT:         storage i32, u32 [size=4, align=4];
// SEPARATE-NEXT:         storage i64, u64 [size=8, align=8];
// SEPARATE-NEXT:         storage i128, u128 [size=16, align=16];
// SEPARATE-NEXT:         storage bf16 [size=2, align=2];
// SEPARATE-NEXT:         storage f16 [size=2, align=2];
// SEPARATE-NEXT:         storage f32 [size=4, align=4];
// SEPARATE-NEXT:         storage f64 [size=8, align=8];
// SEPARATE-NEXT:         storage f128 [size=16, align=16];
// SEPARATE-NEXT:         storage d32 [size=4, align=4];
// SEPARATE-NEXT:         storage d64 [size=8, align=8];
// SEPARATE-NEXT:         storage d128 [size=16, align=16];
// SEPARATE-NEXT:     }
// SEPARATE-NEXT:     global %[[VALUE_level:[0-9]+]] level: i32 [storage=static] = const<i32>(4) [linkage=external];
// SEPARATE-NEXT: }
// SLATE-FILECHECK-END SEPARATE
// SLATE-FILECHECK-BEGIN OUT_OF_RANGE
// OUT_OF_RANGE: module {
// OUT_OF_RANGE-NEXT:     target "x86_64-pc-windows-msvc" {
// OUT_OF_RANGE-NEXT:         endian = little;
// OUT_OF_RANGE-NEXT:         pointer [size=8, align=8];
// OUT_OF_RANGE-NEXT:         stack_alignment = 16;
// OUT_OF_RANGE-NEXT:         long_double = f64;
// OUT_OF_RANGE-NEXT:         storage bool [size=1, align=1];
// OUT_OF_RANGE-NEXT:         storage i8, u8 [size=1, align=1];
// OUT_OF_RANGE-NEXT:         storage i16, u16 [size=2, align=2];
// OUT_OF_RANGE-NEXT:         storage i32, u32 [size=4, align=4];
// OUT_OF_RANGE-NEXT:         storage i64, u64 [size=8, align=8];
// OUT_OF_RANGE-NEXT:         storage i128, u128 [size=16, align=16];
// OUT_OF_RANGE-NEXT:         storage bf16 [size=2, align=2];
// OUT_OF_RANGE-NEXT:         storage f16 [size=2, align=2];
// OUT_OF_RANGE-NEXT:         storage f32 [size=4, align=4];
// OUT_OF_RANGE-NEXT:         storage f64 [size=8, align=8];
// OUT_OF_RANGE-NEXT:         storage f128 [size=16, align=16];
// OUT_OF_RANGE-NEXT:         storage d32 [size=4, align=4];
// OUT_OF_RANGE-NEXT:         storage d64 [size=8, align=8];
// OUT_OF_RANGE-NEXT:         storage d128 [size=16, align=16];
// OUT_OF_RANGE-NEXT:     }
// OUT_OF_RANGE-NEXT:     global %[[VALUE_level:[0-9]+]] level: i32 [storage=static] = const<i32>(1) [linkage=external];
// OUT_OF_RANGE-NEXT: }
// SLATE-FILECHECK-END OUT_OF_RANGE
// SLATE-FILECHECK-BEGIN NO_WARNINGS
// NO_WARNINGS: module {
// NO_WARNINGS-NEXT:     target "x86_64-pc-windows-msvc" {
// NO_WARNINGS-NEXT:         endian = little;
// NO_WARNINGS-NEXT:         pointer [size=8, align=8];
// NO_WARNINGS-NEXT:         stack_alignment = 16;
// NO_WARNINGS-NEXT:         long_double = f64;
// NO_WARNINGS-NEXT:         storage bool [size=1, align=1];
// NO_WARNINGS-NEXT:         storage i8, u8 [size=1, align=1];
// NO_WARNINGS-NEXT:         storage i16, u16 [size=2, align=2];
// NO_WARNINGS-NEXT:         storage i32, u32 [size=4, align=4];
// NO_WARNINGS-NEXT:         storage i64, u64 [size=8, align=8];
// NO_WARNINGS-NEXT:         storage i128, u128 [size=16, align=16];
// NO_WARNINGS-NEXT:         storage bf16 [size=2, align=2];
// NO_WARNINGS-NEXT:         storage f16 [size=2, align=2];
// NO_WARNINGS-NEXT:         storage f32 [size=4, align=4];
// NO_WARNINGS-NEXT:         storage f64 [size=8, align=8];
// NO_WARNINGS-NEXT:         storage f128 [size=16, align=16];
// NO_WARNINGS-NEXT:         storage d32 [size=4, align=4];
// NO_WARNINGS-NEXT:         storage d64 [size=8, align=8];
// NO_WARNINGS-NEXT:         storage d128 [size=16, align=16];
// NO_WARNINGS-NEXT:     }
// NO_WARNINGS-NEXT:     global %[[VALUE_level:[0-9]+]] level: i32 [storage=static] = const<i32>(0) [linkage=external];
// NO_WARNINGS-NEXT: }
// SLATE-FILECHECK-END NO_WARNINGS
// SLATE-FILECHECK-BEGIN LAST_WINS
// LAST_WINS: module {
// LAST_WINS-NEXT:     target "x86_64-pc-windows-msvc" {
// LAST_WINS-NEXT:         endian = little;
// LAST_WINS-NEXT:         pointer [size=8, align=8];
// LAST_WINS-NEXT:         stack_alignment = 16;
// LAST_WINS-NEXT:         long_double = f64;
// LAST_WINS-NEXT:         storage bool [size=1, align=1];
// LAST_WINS-NEXT:         storage i8, u8 [size=1, align=1];
// LAST_WINS-NEXT:         storage i16, u16 [size=2, align=2];
// LAST_WINS-NEXT:         storage i32, u32 [size=4, align=4];
// LAST_WINS-NEXT:         storage i64, u64 [size=8, align=8];
// LAST_WINS-NEXT:         storage i128, u128 [size=16, align=16];
// LAST_WINS-NEXT:         storage bf16 [size=2, align=2];
// LAST_WINS-NEXT:         storage f16 [size=2, align=2];
// LAST_WINS-NEXT:         storage f32 [size=4, align=4];
// LAST_WINS-NEXT:         storage f64 [size=8, align=8];
// LAST_WINS-NEXT:         storage f128 [size=16, align=16];
// LAST_WINS-NEXT:         storage d32 [size=4, align=4];
// LAST_WINS-NEXT:         storage d64 [size=8, align=8];
// LAST_WINS-NEXT:         storage d128 [size=16, align=16];
// LAST_WINS-NEXT:     }
// LAST_WINS-NEXT:     global %[[VALUE_level:[0-9]+]] level: i32 [storage=static] = const<i32>(2) [linkage=external];
// LAST_WINS-NEXT: }
// SLATE-FILECHECK-END LAST_WINS
// SLATE-FILECHECK-BEGIN WALL
// WALL: module {
// WALL-NEXT:     target "x86_64-pc-windows-msvc" {
// WALL-NEXT:         endian = little;
// WALL-NEXT:         pointer [size=8, align=8];
// WALL-NEXT:         stack_alignment = 16;
// WALL-NEXT:         long_double = f64;
// WALL-NEXT:         storage bool [size=1, align=1];
// WALL-NEXT:         storage i8, u8 [size=1, align=1];
// WALL-NEXT:         storage i16, u16 [size=2, align=2];
// WALL-NEXT:         storage i32, u32 [size=4, align=4];
// WALL-NEXT:         storage i64, u64 [size=8, align=8];
// WALL-NEXT:         storage i128, u128 [size=16, align=16];
// WALL-NEXT:         storage bf16 [size=2, align=2];
// WALL-NEXT:         storage f16 [size=2, align=2];
// WALL-NEXT:         storage f32 [size=4, align=4];
// WALL-NEXT:         storage f64 [size=8, align=8];
// WALL-NEXT:         storage f128 [size=16, align=16];
// WALL-NEXT:         storage d32 [size=4, align=4];
// WALL-NEXT:         storage d64 [size=8, align=8];
// WALL-NEXT:         storage d128 [size=16, align=16];
// WALL-NEXT:     }
// WALL-NEXT:     global %[[VALUE_level:[0-9]+]] level: i32 [storage=static] = const<i32>(100) [linkage=external];
// WALL-NEXT: }
// SLATE-FILECHECK-END WALL
