#ifndef OUTER_GUARD
#define OUTER_GUARD

#ifndef INNER_GUARD
#define INNER_GUARD
#define NESTED_VALUE 1
#endif

#ifdef SOME_FEATURE
#define FEATURE_VALUE 1
#else
#define FEATURE_VALUE 2
#endif

#endif

int nested = NESTED_VALUE;
int feature = FEATURE_VALUE;

#ifndef LEVEL_A
#define LEVEL_A
#ifndef LEVEL_B
#define LEVEL_B
#ifndef LEVEL_C
#define LEVEL_C
#define TRIPLE_NESTED 3
#endif
#endif
#endif

int triple = TRIPLE_NESTED;

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES FEATURE SOME_FEATURE

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
// DEFAULT-NEXT:     global %0 nested: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %1 feature: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %2 triple: i32 [storage=static] = const<i32>(3) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN FEATURE
// FEATURE: module {
// FEATURE-NEXT:     target "x86_64-unknown-linux-gnu" {
// FEATURE-NEXT:         endian = little;
// FEATURE-NEXT:         pointer [size=8, align=8];
// FEATURE-NEXT:         stack_alignment = 16;
// FEATURE-NEXT:         long_double = f80;
// FEATURE-NEXT:         storage bool [size=1, align=1];
// FEATURE-NEXT:         storage i8, u8 [size=1, align=1];
// FEATURE-NEXT:         storage i16, u16 [size=2, align=2];
// FEATURE-NEXT:         storage i32, u32 [size=4, align=4];
// FEATURE-NEXT:         storage i64, u64 [size=8, align=8];
// FEATURE-NEXT:         storage i128, u128 [size=16, align=16];
// FEATURE-NEXT:         storage bf16 [size=2, align=2];
// FEATURE-NEXT:         storage f16 [size=2, align=2];
// FEATURE-NEXT:         storage f32 [size=4, align=4];
// FEATURE-NEXT:         storage f64 [size=8, align=8];
// FEATURE-NEXT:         storage f80 [size=16, align=16];
// FEATURE-NEXT:         storage f128 [size=16, align=16];
// FEATURE-NEXT:         storage d32 [size=4, align=4];
// FEATURE-NEXT:         storage d64 [size=8, align=8];
// FEATURE-NEXT:         storage d128 [size=16, align=16];
// FEATURE-NEXT:     }
// FEATURE-NEXT:     global %0 nested: i32 [storage=static] = const<i32>(1) [linkage=external];
// FEATURE-NEXT:     global %1 feature: i32 [storage=static] = const<i32>(1) [linkage=external];
// FEATURE-NEXT:     global %2 triple: i32 [storage=static] = const<i32>(3) [linkage=external];
// FEATURE-NEXT: }
// SLATE-FILECHECK-END FEATURE
