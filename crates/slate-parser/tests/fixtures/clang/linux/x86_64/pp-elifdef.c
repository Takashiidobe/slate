#ifdef FIRST
int first;
#elifdef SECOND
int second;
#elifndef THIRD
int not_third;
#else
int fallback;
#endif
#if 0
#ifdef DEAD
#elifdef ALSO_DEAD
#endif
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES FIRST FIRST
// SLATE-FILECHECK-DEFINES SECOND SECOND
// SLATE-FILECHECK-DEFINES THIRD THIRD

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
// DEFAULT-NEXT:     global %[[VALUE_not_third:[0-9]+]] not_third: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FIRST
// FIRST: module {
// FIRST-NEXT:     target "x86_64-unknown-linux-gnu" {
// FIRST-NEXT:         endian = little;
// FIRST-NEXT:         pointer [size=8, align=8];
// FIRST-NEXT:         stack_alignment = 16;
// FIRST-NEXT:         long_double = f80;
// FIRST-NEXT:         storage bool [size=1, align=1];
// FIRST-NEXT:         storage i8, u8 [size=1, align=1];
// FIRST-NEXT:         storage i16, u16 [size=2, align=2];
// FIRST-NEXT:         storage i32, u32 [size=4, align=4];
// FIRST-NEXT:         storage i64, u64 [size=8, align=8];
// FIRST-NEXT:         storage i128, u128 [size=16, align=16];
// FIRST-NEXT:         storage bf16 [size=2, align=2];
// FIRST-NEXT:         storage f16 [size=2, align=2];
// FIRST-NEXT:         storage f32 [size=4, align=4];
// FIRST-NEXT:         storage f64 [size=8, align=8];
// FIRST-NEXT:         storage f80 [size=16, align=16];
// FIRST-NEXT:         storage f128 [size=16, align=16];
// FIRST-NEXT:         storage d32 [size=4, align=4];
// FIRST-NEXT:         storage d64 [size=8, align=8];
// FIRST-NEXT:         storage d128 [size=16, align=16];
// FIRST-NEXT:     }
// FIRST-NEXT:     global %[[VALUE_first:[0-9]+]] first: i32 [storage=static] [linkage=external];
// FIRST-NEXT: }
// SLATE-FILECHECK-END FIRST
// SLATE-FILECHECK-BEGIN SECOND
// SECOND: module {
// SECOND-NEXT:     target "x86_64-unknown-linux-gnu" {
// SECOND-NEXT:         endian = little;
// SECOND-NEXT:         pointer [size=8, align=8];
// SECOND-NEXT:         stack_alignment = 16;
// SECOND-NEXT:         long_double = f80;
// SECOND-NEXT:         storage bool [size=1, align=1];
// SECOND-NEXT:         storage i8, u8 [size=1, align=1];
// SECOND-NEXT:         storage i16, u16 [size=2, align=2];
// SECOND-NEXT:         storage i32, u32 [size=4, align=4];
// SECOND-NEXT:         storage i64, u64 [size=8, align=8];
// SECOND-NEXT:         storage i128, u128 [size=16, align=16];
// SECOND-NEXT:         storage bf16 [size=2, align=2];
// SECOND-NEXT:         storage f16 [size=2, align=2];
// SECOND-NEXT:         storage f32 [size=4, align=4];
// SECOND-NEXT:         storage f64 [size=8, align=8];
// SECOND-NEXT:         storage f80 [size=16, align=16];
// SECOND-NEXT:         storage f128 [size=16, align=16];
// SECOND-NEXT:         storage d32 [size=4, align=4];
// SECOND-NEXT:         storage d64 [size=8, align=8];
// SECOND-NEXT:         storage d128 [size=16, align=16];
// SECOND-NEXT:     }
// SECOND-NEXT:     global %[[VALUE_second:[0-9]+]] second: i32 [storage=static] [linkage=external];
// SECOND-NEXT: }
// SLATE-FILECHECK-END SECOND
// SLATE-FILECHECK-BEGIN THIRD
// THIRD: module {
// THIRD-NEXT:     target "x86_64-unknown-linux-gnu" {
// THIRD-NEXT:         endian = little;
// THIRD-NEXT:         pointer [size=8, align=8];
// THIRD-NEXT:         stack_alignment = 16;
// THIRD-NEXT:         long_double = f80;
// THIRD-NEXT:         storage bool [size=1, align=1];
// THIRD-NEXT:         storage i8, u8 [size=1, align=1];
// THIRD-NEXT:         storage i16, u16 [size=2, align=2];
// THIRD-NEXT:         storage i32, u32 [size=4, align=4];
// THIRD-NEXT:         storage i64, u64 [size=8, align=8];
// THIRD-NEXT:         storage i128, u128 [size=16, align=16];
// THIRD-NEXT:         storage bf16 [size=2, align=2];
// THIRD-NEXT:         storage f16 [size=2, align=2];
// THIRD-NEXT:         storage f32 [size=4, align=4];
// THIRD-NEXT:         storage f64 [size=8, align=8];
// THIRD-NEXT:         storage f80 [size=16, align=16];
// THIRD-NEXT:         storage f128 [size=16, align=16];
// THIRD-NEXT:         storage d32 [size=4, align=4];
// THIRD-NEXT:         storage d64 [size=8, align=8];
// THIRD-NEXT:         storage d128 [size=16, align=16];
// THIRD-NEXT:     }
// THIRD-NEXT:     global %[[VALUE_fallback:[0-9]+]] fallback: i32 [storage=static] [linkage=external];
// THIRD-NEXT: }
// SLATE-FILECHECK-END THIRD
