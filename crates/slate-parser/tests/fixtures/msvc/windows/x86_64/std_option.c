#ifdef __STDC_VERSION__
long stdc_version = __STDC_VERSION__;
#endif
#ifdef __STDC_NO_ATOMICS__
int no_atomics;
#endif
#ifdef __STDC_NO_COMPLEX__
int no_complex;
#endif
#ifdef __STDC_NO_VLA__
int no_vla;
#endif
int always;

// SLATE-FILECHECK-DEFINES NONE
// SLATE-FILECHECK-DEFINES C11
// SLATE-FILECHECK-PREFIX-ARGS C11 /std:c11
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-PREFIX-ARGS C17 /std:c11 /std:c17
// SLATE-FILECHECK-DEFINES CLATEST
// SLATE-FILECHECK-PREFIX-ARGS CLATEST /std:clatest
// SLATE-FILECHECK-DEFINES ATOMICS
// SLATE-FILECHECK-PREFIX-ARGS ATOMICS /std:c11 /experimental:c11atomics
// SLATE-FILECHECK-DEFINES ATOMICS_ONLY
// SLATE-FILECHECK-PREFIX-ARGS ATOMICS_ONLY /experimental:c11atomics

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
// NONE-NEXT:     global %[[VALUE_always:[0-9]+]] always: i32 [storage=static] [linkage=external];
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
// C11-NEXT:     global %[[VALUE_no_atomics:[0-9]+]] no_atomics: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_no_complex:[0-9]+]] no_complex: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_no_vla:[0-9]+]] no_vla: i32 [storage=static] [linkage=external];
// C11-NEXT:     global %[[VALUE_always:[0-9]+]] always: i32 [storage=static] [linkage=external];
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
// C17-NEXT:     global %[[VALUE_no_atomics:[0-9]+]] no_atomics: i32 [storage=static] [linkage=external];
// C17-NEXT:     global %[[VALUE_no_complex:[0-9]+]] no_complex: i32 [storage=static] [linkage=external];
// C17-NEXT:     global %[[VALUE_no_vla:[0-9]+]] no_vla: i32 [storage=static] [linkage=external];
// C17-NEXT:     global %[[VALUE_always:[0-9]+]] always: i32 [storage=static] [linkage=external];
// C17-NEXT: }
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN CLATEST
// CLATEST: module {
// CLATEST-NEXT:     target "x86_64-pc-windows-msvc" {
// CLATEST-NEXT:         endian = little;
// CLATEST-NEXT:         pointer [size=8, align=8];
// CLATEST-NEXT:         stack_alignment = 16;
// CLATEST-NEXT:         long_double = f64;
// CLATEST-NEXT:         storage bool [size=1, align=1];
// CLATEST-NEXT:         storage i8, u8 [size=1, align=1];
// CLATEST-NEXT:         storage i16, u16 [size=2, align=2];
// CLATEST-NEXT:         storage i32, u32 [size=4, align=4];
// CLATEST-NEXT:         storage i64, u64 [size=8, align=8];
// CLATEST-NEXT:         storage i128, u128 [size=16, align=16];
// CLATEST-NEXT:         storage bf16 [size=2, align=2];
// CLATEST-NEXT:         storage f16 [size=2, align=2];
// CLATEST-NEXT:         storage f32 [size=4, align=4];
// CLATEST-NEXT:         storage f64 [size=8, align=8];
// CLATEST-NEXT:         storage f128 [size=16, align=16];
// CLATEST-NEXT:         storage d32 [size=4, align=4];
// CLATEST-NEXT:         storage d64 [size=8, align=8];
// CLATEST-NEXT:         storage d128 [size=16, align=16];
// CLATEST-NEXT:     }
// CLATEST-NEXT:     global %[[VALUE_stdc_version:[0-9]+]] stdc_version: i32 [storage=static] = const<i32>(202312) [linkage=external];
// CLATEST-NEXT:     global %[[VALUE_no_atomics:[0-9]+]] no_atomics: i32 [storage=static] [linkage=external];
// CLATEST-NEXT:     global %[[VALUE_no_complex:[0-9]+]] no_complex: i32 [storage=static] [linkage=external];
// CLATEST-NEXT:     global %[[VALUE_no_vla:[0-9]+]] no_vla: i32 [storage=static] [linkage=external];
// CLATEST-NEXT:     global %[[VALUE_always:[0-9]+]] always: i32 [storage=static] [linkage=external];
// CLATEST-NEXT: }
// SLATE-FILECHECK-END CLATEST
// SLATE-FILECHECK-BEGIN ATOMICS
// ATOMICS: module {
// ATOMICS-NEXT:     target "x86_64-pc-windows-msvc" {
// ATOMICS-NEXT:         endian = little;
// ATOMICS-NEXT:         pointer [size=8, align=8];
// ATOMICS-NEXT:         stack_alignment = 16;
// ATOMICS-NEXT:         long_double = f64;
// ATOMICS-NEXT:         storage bool [size=1, align=1];
// ATOMICS-NEXT:         storage i8, u8 [size=1, align=1];
// ATOMICS-NEXT:         storage i16, u16 [size=2, align=2];
// ATOMICS-NEXT:         storage i32, u32 [size=4, align=4];
// ATOMICS-NEXT:         storage i64, u64 [size=8, align=8];
// ATOMICS-NEXT:         storage i128, u128 [size=16, align=16];
// ATOMICS-NEXT:         storage bf16 [size=2, align=2];
// ATOMICS-NEXT:         storage f16 [size=2, align=2];
// ATOMICS-NEXT:         storage f32 [size=4, align=4];
// ATOMICS-NEXT:         storage f64 [size=8, align=8];
// ATOMICS-NEXT:         storage f128 [size=16, align=16];
// ATOMICS-NEXT:         storage d32 [size=4, align=4];
// ATOMICS-NEXT:         storage d64 [size=8, align=8];
// ATOMICS-NEXT:         storage d128 [size=16, align=16];
// ATOMICS-NEXT:     }
// ATOMICS-NEXT:     global %[[VALUE_stdc_version:[0-9]+]] stdc_version: i32 [storage=static] = const<i32>(201112) [linkage=external];
// ATOMICS-NEXT:     global %[[VALUE_no_complex:[0-9]+]] no_complex: i32 [storage=static] [linkage=external];
// ATOMICS-NEXT:     global %[[VALUE_no_vla:[0-9]+]] no_vla: i32 [storage=static] [linkage=external];
// ATOMICS-NEXT:     global %[[VALUE_always:[0-9]+]] always: i32 [storage=static] [linkage=external];
// ATOMICS-NEXT: }
// SLATE-FILECHECK-END ATOMICS
// SLATE-FILECHECK-BEGIN ATOMICS_ONLY
// ATOMICS_ONLY: module {
// ATOMICS_ONLY-NEXT:     target "x86_64-pc-windows-msvc" {
// ATOMICS_ONLY-NEXT:         endian = little;
// ATOMICS_ONLY-NEXT:         pointer [size=8, align=8];
// ATOMICS_ONLY-NEXT:         stack_alignment = 16;
// ATOMICS_ONLY-NEXT:         long_double = f64;
// ATOMICS_ONLY-NEXT:         storage bool [size=1, align=1];
// ATOMICS_ONLY-NEXT:         storage i8, u8 [size=1, align=1];
// ATOMICS_ONLY-NEXT:         storage i16, u16 [size=2, align=2];
// ATOMICS_ONLY-NEXT:         storage i32, u32 [size=4, align=4];
// ATOMICS_ONLY-NEXT:         storage i64, u64 [size=8, align=8];
// ATOMICS_ONLY-NEXT:         storage i128, u128 [size=16, align=16];
// ATOMICS_ONLY-NEXT:         storage bf16 [size=2, align=2];
// ATOMICS_ONLY-NEXT:         storage f16 [size=2, align=2];
// ATOMICS_ONLY-NEXT:         storage f32 [size=4, align=4];
// ATOMICS_ONLY-NEXT:         storage f64 [size=8, align=8];
// ATOMICS_ONLY-NEXT:         storage f128 [size=16, align=16];
// ATOMICS_ONLY-NEXT:         storage d32 [size=4, align=4];
// ATOMICS_ONLY-NEXT:         storage d64 [size=8, align=8];
// ATOMICS_ONLY-NEXT:         storage d128 [size=16, align=16];
// ATOMICS_ONLY-NEXT:     }
// ATOMICS_ONLY-NEXT:     global %[[VALUE_always:[0-9]+]] always: i32 [storage=static] [linkage=external];
// ATOMICS_ONLY-NEXT: }
// SLATE-FILECHECK-END ATOMICS_ONLY
