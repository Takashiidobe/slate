// SLATE-FILECHECK-DEFINES CP1251
// SLATE-FILECHECK-PREFIX-ARGS CP1251 /source-charset:.1251 /execution-charset:utf-8
// SLATE-FILECHECK-DEFINES ASCII
// SLATE-FILECHECK-PREFIX-ARGS ASCII /source-charset:us-ascii /execution-charset:utf-8
// SLATE-FILECHECK-DEFINES ROUND_TRIP
// SLATE-FILECHECK-PREFIX-ARGS ROUND_TRIP /source-charset:iso-8859-5 /execution-charset:iso-8859-5
// SLATE-FILECHECK-ARGS --compact-ir

const char text[] = "ÆéˆÁ¶";

// SLATE-FILECHECK-BEGIN CP1251
// CP1251: module {
// CP1251-NEXT:     target "x86_64-pc-windows-msvc" {
// CP1251-NEXT:         endian = little;
// CP1251-NEXT:         pointer [size=8, align=8];
// CP1251-NEXT:         stack_alignment = 16;
// CP1251-NEXT:         long_double = f64;
// CP1251-NEXT:         storage bool [size=1, align=1];
// CP1251-NEXT:         storage i8, u8 [size=1, align=1];
// CP1251-NEXT:         storage i16, u16 [size=2, align=2];
// CP1251-NEXT:         storage i32, u32 [size=4, align=4];
// CP1251-NEXT:         storage i64, u64 [size=8, align=8];
// CP1251-NEXT:         storage i128, u128 [size=16, align=16];
// CP1251-NEXT:         storage bf16 [size=2, align=2];
// CP1251-NEXT:         storage f16 [size=2, align=2];
// CP1251-NEXT:         storage f32 [size=4, align=4];
// CP1251-NEXT:         storage f64 [size=8, align=8];
// CP1251-NEXT:         storage f128 [size=16, align=16];
// CP1251-NEXT:         storage d32 [size=4, align=4];
// CP1251-NEXT:         storage d64 [size=8, align=8];
// CP1251-NEXT:         storage d128 [size=16, align=16];
// CP1251-NEXT:     }
// CP1251-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 12> [storage=static] [const] = code_units<array<i8, 12>>([208, 150, 208, 185, 226, 130, 172, 208, 145, 194, 182, 0]) [linkage=external];
// CP1251-NEXT: }
// SLATE-FILECHECK-END CP1251
// SLATE-FILECHECK-BEGIN ASCII
// ASCII: module {
// ASCII-NEXT:     target "x86_64-pc-windows-msvc" {
// ASCII-NEXT:         endian = little;
// ASCII-NEXT:         pointer [size=8, align=8];
// ASCII-NEXT:         stack_alignment = 16;
// ASCII-NEXT:         long_double = f64;
// ASCII-NEXT:         storage bool [size=1, align=1];
// ASCII-NEXT:         storage i8, u8 [size=1, align=1];
// ASCII-NEXT:         storage i16, u16 [size=2, align=2];
// ASCII-NEXT:         storage i32, u32 [size=4, align=4];
// ASCII-NEXT:         storage i64, u64 [size=8, align=8];
// ASCII-NEXT:         storage i128, u128 [size=16, align=16];
// ASCII-NEXT:         storage bf16 [size=2, align=2];
// ASCII-NEXT:         storage f16 [size=2, align=2];
// ASCII-NEXT:         storage f32 [size=4, align=4];
// ASCII-NEXT:         storage f64 [size=8, align=8];
// ASCII-NEXT:         storage f128 [size=16, align=16];
// ASCII-NEXT:         storage d32 [size=4, align=4];
// ASCII-NEXT:         storage d64 [size=8, align=8];
// ASCII-NEXT:         storage d128 [size=16, align=16];
// ASCII-NEXT:     }
// ASCII-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([70, 105, 8, 65, 54, 0]) [linkage=external];
// ASCII-NEXT: }
// SLATE-FILECHECK-END ASCII
// SLATE-FILECHECK-BEGIN ROUND_TRIP
// ROUND_TRIP: module {
// ROUND_TRIP-NEXT:     target "x86_64-pc-windows-msvc" {
// ROUND_TRIP-NEXT:         endian = little;
// ROUND_TRIP-NEXT:         pointer [size=8, align=8];
// ROUND_TRIP-NEXT:         stack_alignment = 16;
// ROUND_TRIP-NEXT:         long_double = f64;
// ROUND_TRIP-NEXT:         storage bool [size=1, align=1];
// ROUND_TRIP-NEXT:         storage i8, u8 [size=1, align=1];
// ROUND_TRIP-NEXT:         storage i16, u16 [size=2, align=2];
// ROUND_TRIP-NEXT:         storage i32, u32 [size=4, align=4];
// ROUND_TRIP-NEXT:         storage i64, u64 [size=8, align=8];
// ROUND_TRIP-NEXT:         storage i128, u128 [size=16, align=16];
// ROUND_TRIP-NEXT:         storage bf16 [size=2, align=2];
// ROUND_TRIP-NEXT:         storage f16 [size=2, align=2];
// ROUND_TRIP-NEXT:         storage f32 [size=4, align=4];
// ROUND_TRIP-NEXT:         storage f64 [size=8, align=8];
// ROUND_TRIP-NEXT:         storage f128 [size=16, align=16];
// ROUND_TRIP-NEXT:         storage d32 [size=4, align=4];
// ROUND_TRIP-NEXT:         storage d64 [size=8, align=8];
// ROUND_TRIP-NEXT:         storage d128 [size=16, align=16];
// ROUND_TRIP-NEXT:     }
// ROUND_TRIP-NEXT:     global %[[VALUE_text:[0-9]+]] text: array<i8, 6> [storage=static] [const] = code_units<array<i8, 6>>([198, 233, 136, 193, 182, 0]) [linkage=external];
// ROUND_TRIP-NEXT: }
// SLATE-FILECHECK-END ROUND_TRIP
