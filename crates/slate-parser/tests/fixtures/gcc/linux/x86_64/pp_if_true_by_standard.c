// SLATE-FILECHECK-DEFINES C99
// SLATE-FILECHECK-STD C99 c99
// SLATE-FILECHECK-DEFINES C23
// SLATE-FILECHECK-STD C23 c23
#if true
int true_is_one;
#else
int true_is_zero;
#endif
#if false || !true
int false_or_not_true;
#endif

// SLATE-FILECHECK-BEGIN C99
// C99: module {
// C99-NEXT:     target "x86_64-unknown-linux-gnu" {
// C99-NEXT:         endian = little;
// C99-NEXT:         pointer [size=8, align=8];
// C99-NEXT:         stack_alignment = 16;
// C99-NEXT:         long_double = f80;
// C99-NEXT:         storage bool [size=1, align=1];
// C99-NEXT:         storage i8, u8 [size=1, align=1];
// C99-NEXT:         storage i16, u16 [size=2, align=2];
// C99-NEXT:         storage i32, u32 [size=4, align=4];
// C99-NEXT:         storage i64, u64 [size=8, align=8];
// C99-NEXT:         storage i128, u128 [size=16, align=16];
// C99-NEXT:         storage bf16 [size=2, align=2];
// C99-NEXT:         storage f16 [size=2, align=2];
// C99-NEXT:         storage f32 [size=4, align=4];
// C99-NEXT:         storage f64 [size=8, align=8];
// C99-NEXT:         storage f80 [size=16, align=16];
// C99-NEXT:         storage f128 [size=16, align=16];
// C99-NEXT:         storage d32 [size=4, align=4];
// C99-NEXT:         storage d64 [size=8, align=8];
// C99-NEXT:         storage d128 [size=16, align=16];
// C99-NEXT:     }
// C99-NEXT:     global %[[VALUE_true_is_zero:[0-9]+]] true_is_zero: i32 [storage=static] [linkage=external];
// C99-NEXT:     global %[[VALUE_false_or_not_true:[0-9]+]] false_or_not_true: i32 [storage=static] [linkage=external];
// C99-NEXT: }
// SLATE-FILECHECK-END C99
// SLATE-FILECHECK-BEGIN C23
// C23: module {
// C23-NEXT:     target "x86_64-unknown-linux-gnu" {
// C23-NEXT:         endian = little;
// C23-NEXT:         pointer [size=8, align=8];
// C23-NEXT:         stack_alignment = 16;
// C23-NEXT:         long_double = f80;
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
// C23-NEXT:         storage f80 [size=16, align=16];
// C23-NEXT:         storage f128 [size=16, align=16];
// C23-NEXT:         storage d32 [size=4, align=4];
// C23-NEXT:         storage d64 [size=8, align=8];
// C23-NEXT:         storage d128 [size=16, align=16];
// C23-NEXT:     }
// C23-NEXT:     global %[[VALUE_true_is_one:[0-9]+]] true_is_one: i32 [storage=static] [linkage=external];
// C23-NEXT: }
// SLATE-FILECHECK-END C23
