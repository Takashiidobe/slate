#define WIDTH 4
#pragma push_macro("WIDTH")
#undef WIDTH
#define WIDTH 8
int inner[WIDTH];
#pragma pop_macro("WIDTH")
int outer[WIDTH];
#pragma push_macro("FRESH")
#define FRESH 1
#pragma pop_macro("FRESH")
#ifdef FRESH
int fresh;
#endif
#pragma pop_macro("WIDTH")
int unmatched_pop[WIDTH];
#ifdef WIDE
#pragma push_macro("WIDTH")
#define WIDTH 16
int wide[WIDTH];
#pragma pop_macro("WIDTH")
#endif
int after[WIDTH];

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WIDE WIDE

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
// DEFAULT-NEXT:     global %[[VALUE_inner:[0-9]+]] inner: array<i32, 8> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_outer:[0-9]+]] outer: array<i32, 4> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unmatched_pop:[0-9]+]] unmatched_pop: array<i32, 4> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_after:[0-9]+]] after: array<i32, 4> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN WIDE
// WIDE: module {
// WIDE-NEXT:     target "x86_64-unknown-linux-gnu" {
// WIDE-NEXT:         endian = little;
// WIDE-NEXT:         pointer [size=8, align=8];
// WIDE-NEXT:         stack_alignment = 16;
// WIDE-NEXT:         long_double = f80;
// WIDE-NEXT:         storage bool [size=1, align=1];
// WIDE-NEXT:         storage i8, u8 [size=1, align=1];
// WIDE-NEXT:         storage i16, u16 [size=2, align=2];
// WIDE-NEXT:         storage i32, u32 [size=4, align=4];
// WIDE-NEXT:         storage i64, u64 [size=8, align=8];
// WIDE-NEXT:         storage i128, u128 [size=16, align=16];
// WIDE-NEXT:         storage bf16 [size=2, align=2];
// WIDE-NEXT:         storage f16 [size=2, align=2];
// WIDE-NEXT:         storage f32 [size=4, align=4];
// WIDE-NEXT:         storage f64 [size=8, align=8];
// WIDE-NEXT:         storage f80 [size=16, align=16];
// WIDE-NEXT:         storage f128 [size=16, align=16];
// WIDE-NEXT:         storage d32 [size=4, align=4];
// WIDE-NEXT:         storage d64 [size=8, align=8];
// WIDE-NEXT:         storage d128 [size=16, align=16];
// WIDE-NEXT:     }
// WIDE-NEXT:     global %[[VALUE_inner:[0-9]+]] inner: array<i32, 8> [storage=static] [align=16] [linkage=external];
// WIDE-NEXT:     global %[[VALUE_outer:[0-9]+]] outer: array<i32, 4> [storage=static] [align=16] [linkage=external];
// WIDE-NEXT:     global %[[VALUE_unmatched_pop:[0-9]+]] unmatched_pop: array<i32, 4> [storage=static] [align=16] [linkage=external];
// WIDE-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: array<i32, 16> [storage=static] [align=16] [linkage=external];
// WIDE-NEXT:     global %[[VALUE_after:[0-9]+]] after: array<i32, 4> [storage=static] [align=16] [linkage=external];
// WIDE-NEXT: }
// SLATE-FILECHECK-END WIDE
