#define X 1
#ifdef A
#undef X
#define X 2
#endif
int redefined[X];
#ifdef A
#ifdef B
#undef X
#define X 5
#endif
#endif
int nested[X];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A
// SLATE-FILECHECK-DEFINES B B
// SLATE-FILECHECK-DEFINES AB A B

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
// DEFAULT-NEXT:     global %[[VALUE_redefined:[0-9]+]] redefined: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN A
// A: module {
// A-NEXT:     target "x86_64-unknown-linux-gnu" {
// A-NEXT:         endian = little;
// A-NEXT:         pointer [size=8, align=8];
// A-NEXT:         stack_alignment = 16;
// A-NEXT:         long_double = f80;
// A-NEXT:         storage bool [size=1, align=1];
// A-NEXT:         storage i8, u8 [size=1, align=1];
// A-NEXT:         storage i16, u16 [size=2, align=2];
// A-NEXT:         storage i32, u32 [size=4, align=4];
// A-NEXT:         storage i64, u64 [size=8, align=8];
// A-NEXT:         storage i128, u128 [size=16, align=16];
// A-NEXT:         storage bf16 [size=2, align=2];
// A-NEXT:         storage f16 [size=2, align=2];
// A-NEXT:         storage f32 [size=4, align=4];
// A-NEXT:         storage f64 [size=8, align=8];
// A-NEXT:         storage f80 [size=16, align=16];
// A-NEXT:         storage f128 [size=16, align=16];
// A-NEXT:         storage d32 [size=4, align=4];
// A-NEXT:         storage d64 [size=8, align=8];
// A-NEXT:         storage d128 [size=16, align=16];
// A-NEXT:     }
// A-NEXT:     global %[[VALUE_redefined:[0-9]+]] redefined: array<i32, 2> [storage=static] [linkage=external];
// A-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: array<i32, 2> [storage=static] [linkage=external];
// A-NEXT: }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN B
// B: module {
// B-NEXT:     target "x86_64-unknown-linux-gnu" {
// B-NEXT:         endian = little;
// B-NEXT:         pointer [size=8, align=8];
// B-NEXT:         stack_alignment = 16;
// B-NEXT:         long_double = f80;
// B-NEXT:         storage bool [size=1, align=1];
// B-NEXT:         storage i8, u8 [size=1, align=1];
// B-NEXT:         storage i16, u16 [size=2, align=2];
// B-NEXT:         storage i32, u32 [size=4, align=4];
// B-NEXT:         storage i64, u64 [size=8, align=8];
// B-NEXT:         storage i128, u128 [size=16, align=16];
// B-NEXT:         storage bf16 [size=2, align=2];
// B-NEXT:         storage f16 [size=2, align=2];
// B-NEXT:         storage f32 [size=4, align=4];
// B-NEXT:         storage f64 [size=8, align=8];
// B-NEXT:         storage f80 [size=16, align=16];
// B-NEXT:         storage f128 [size=16, align=16];
// B-NEXT:         storage d32 [size=4, align=4];
// B-NEXT:         storage d64 [size=8, align=8];
// B-NEXT:         storage d128 [size=16, align=16];
// B-NEXT:     }
// B-NEXT:     global %[[VALUE_redefined:[0-9]+]] redefined: array<i32, 1> [storage=static] [linkage=external];
// B-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: array<i32, 1> [storage=static] [linkage=external];
// B-NEXT: }
// SLATE-FILECHECK-END B
// SLATE-FILECHECK-BEGIN AB
// AB: module {
// AB-NEXT:     target "x86_64-unknown-linux-gnu" {
// AB-NEXT:         endian = little;
// AB-NEXT:         pointer [size=8, align=8];
// AB-NEXT:         stack_alignment = 16;
// AB-NEXT:         long_double = f80;
// AB-NEXT:         storage bool [size=1, align=1];
// AB-NEXT:         storage i8, u8 [size=1, align=1];
// AB-NEXT:         storage i16, u16 [size=2, align=2];
// AB-NEXT:         storage i32, u32 [size=4, align=4];
// AB-NEXT:         storage i64, u64 [size=8, align=8];
// AB-NEXT:         storage i128, u128 [size=16, align=16];
// AB-NEXT:         storage bf16 [size=2, align=2];
// AB-NEXT:         storage f16 [size=2, align=2];
// AB-NEXT:         storage f32 [size=4, align=4];
// AB-NEXT:         storage f64 [size=8, align=8];
// AB-NEXT:         storage f80 [size=16, align=16];
// AB-NEXT:         storage f128 [size=16, align=16];
// AB-NEXT:         storage d32 [size=4, align=4];
// AB-NEXT:         storage d64 [size=8, align=8];
// AB-NEXT:         storage d128 [size=16, align=16];
// AB-NEXT:     }
// AB-NEXT:     global %[[VALUE_redefined:[0-9]+]] redefined: array<i32, 2> [storage=static] [linkage=external];
// AB-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: array<i32, 5> [storage=static] [align=16] [linkage=external];
// AB-NEXT: }
// SLATE-FILECHECK-END AB
