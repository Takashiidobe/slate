__attribute__((cold, noinline, noclone, noipa)) int cold_function(void);
__attribute__((hot, flatten, leaf)) int hot_function(void);
__attribute__((optimize(0), naked)) int optimized_function(void);
__attribute__((no_split_stack)) int split_function(void);
__attribute__((returns_twice)) int returns_twice_function(void);

// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     fn %[[VALUE_cold_function:[0-9]+]] @cold_function() -> i32 [linkage=external] [inline=never];
// DEFAULT-NEXT:     fn %[[VALUE_hot_function:[0-9]+]] @hot_function() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_optimized_function:[0-9]+]] @optimized_function() -> i32 [linkage=external] [naked];
// DEFAULT-NEXT:     fn %[[VALUE_split_function:[0-9]+]] @split_function() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_returns_twice_function:[0-9]+]] @returns_twice_function() -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
