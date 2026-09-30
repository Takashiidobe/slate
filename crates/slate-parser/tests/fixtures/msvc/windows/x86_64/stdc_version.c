#ifdef __STDC_VERSION__
long stdc_version = __STDC_VERSION__;
#else
int no_stdc_version;
#endif

// SLATE-FILECHECK-DEFINES NONE
// SLATE-FILECHECK-DEFINES C11
// SLATE-FILECHECK-STD C11 c11
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23

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
// NONE-NEXT:     global %[[VALUE_no_stdc_version:[0-9]+]] no_stdc_version: i32 [storage=static] [linkage=external];
// NONE-NEXT: }
// SLATE-FILECHECK-END NONE
// SLATE-FILECHECK-BEGIN C11
// C11: module {
// C11-NEXT:     target "x86_64-pc-windows-msvc" {
// C11-NEXT:         endian = little;
// C11-NEXT:         pointer [size=8, align=8];
// C11-NEXT:         stack_alignment = 16;
// C11-NEXT:         long_double = f64;
// C11-NEXT:         storage bool [size=1, align=1];
// C11-NEXT:         storage i8, u8 [size=1, align=1];
// C11-NEXT:         storage i16, u16 [size=2, align=2];
// C11-NEXT:         storage i32, u32 [size=4, align=4];
// C11-NEXT:         storage i64, u64 [size=8, align=8];
// C11-NEXT:         storage i128, u128 [size=16, align=16];
// C11-NEXT:         storage bf16 [size=2, align=2];
// C11-NEXT:         storage f16 [size=2, align=2];
// C11-NEXT:         storage f32 [size=4, align=4];
// C11-NEXT:         storage f64 [size=8, align=8];
// C11-NEXT:         storage f128 [size=16, align=16];
// C11-NEXT:         storage d32 [size=4, align=4];
// C11-NEXT:         storage d64 [size=8, align=8];
// C11-NEXT:         storage d128 [size=16, align=16];
// C11-NEXT:     }
// C11-NEXT:     global %[[VALUE_stdc_version:[0-9]+]] stdc_version: i32 [storage=static] = const<i32>(201112) [linkage=external];
// C11-NEXT: }
// SLATE-FILECHECK-END C11
// SLATE-FILECHECK-BEGIN C17
// C17: module {
// C17-NEXT:     target "x86_64-pc-windows-msvc" {
// C17-NEXT:         endian = little;
// C17-NEXT:         pointer [size=8, align=8];
// C17-NEXT:         stack_alignment = 16;
// C17-NEXT:         long_double = f64;
// C17-NEXT:         storage bool [size=1, align=1];
// C17-NEXT:         storage i8, u8 [size=1, align=1];
// C17-NEXT:         storage i16, u16 [size=2, align=2];
// C17-NEXT:         storage i32, u32 [size=4, align=4];
// C17-NEXT:         storage i64, u64 [size=8, align=8];
// C17-NEXT:         storage i128, u128 [size=16, align=16];
// C17-NEXT:         storage bf16 [size=2, align=2];
// C17-NEXT:         storage f16 [size=2, align=2];
// C17-NEXT:         storage f32 [size=4, align=4];
// C17-NEXT:         storage f64 [size=8, align=8];
// C17-NEXT:         storage f128 [size=16, align=16];
// C17-NEXT:         storage d32 [size=4, align=4];
// C17-NEXT:         storage d64 [size=8, align=8];
// C17-NEXT:         storage d128 [size=16, align=16];
// C17-NEXT:     }
// C17-NEXT:     global %[[VALUE_stdc_version:[0-9]+]] stdc_version: i32 [storage=static] = const<i32>(201710) [linkage=external];
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-pc-windows-msvc" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f64;
// C23-NEXT:         storage bool [size=1, align=1];
// C23-NEXT:         storage i8, u8 [size=1, align=1];
// C23-NEXT:         storage i16, u16 [size=2, align=2];
// C23-NEXT:         storage i32, u32 [size=4, align=4];
// C23-NEXT:         storage i64, u64 [size=8, align=8];
// C23-NEXT:         storage i128, u128 [size=16, align=16];
// C23-NEXT:         storage bf16 [size=2, align=2];
// C23-NEXT:         storage f16 [size=2, align=2];
// C23-NEXT:         storage f32 [size=4, align=4];
// C23-NEXT:         storage f64 [size=8, align=8];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %[[VALUE_stdc_version:[0-9]+]] stdc_version: i32 [storage=static] = const<i32>(202312) [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
