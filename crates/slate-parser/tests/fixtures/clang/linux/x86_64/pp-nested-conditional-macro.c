#define INNER 1
#ifdef A
#undef INNER
#define INNER 2
#endif
#define OUTER INNER
#define WRAP(value) (value + INNER)
int nested[OUTER];
int wrapped[WRAP(1)];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A

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
// DEFAULT-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: array<i32, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_wrapped:[0-9]+]] wrapped: array<i32, 2> [storage=static] [linkage=external];
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
// A-NEXT:     global %[[VALUE_nested:[0-9]+]] nested: array<i32, 2> [storage=static] [linkage=external];
// A-NEXT:     global %[[VALUE_wrapped:[0-9]+]] wrapped: array<i32, 3> [storage=static] [linkage=external];
// A-NEXT: }
// SLATE-FILECHECK-END A
