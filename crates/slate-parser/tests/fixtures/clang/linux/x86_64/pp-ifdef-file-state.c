#ifdef A
#define X
#endif
#ifdef X
int x_defined[1];
#else
int x_undefined[2];
#endif
#if defined(FLAG) && !defined X
int flag_without_x[3];
#endif
#ifndef X
int x_missing[4];
#endif

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES A A
// SLATE-FILECHECK-DEFINES FLAG FLAG
// SLATE-FILECHECK-DEFINES X X

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
// DEFAULT-NEXT:     global %[[VALUE_x_undefined:[0-9]+]] x_undefined: array<i32, 2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_x_missing:[0-9]+]] x_missing: array<i32, 4> [storage=static] [align=16] [linkage=external];
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
// A-NEXT:     global %[[VALUE_x_defined:[0-9]+]] x_defined: array<i32, 1> [storage=static] [linkage=external];
// A-NEXT: }
// SLATE-FILECHECK-END A
// SLATE-FILECHECK-BEGIN FLAG
// FLAG: module {
// FLAG-NEXT:     target "x86_64-unknown-linux-gnu" {
// FLAG-NEXT:         endian = little;
// FLAG-NEXT:         pointer [size=8, align=8];
// FLAG-NEXT:         stack_alignment = 16;
// FLAG-NEXT:         long_double = f80;
// FLAG-NEXT:         storage bool [size=1, align=1];
// FLAG-NEXT:         storage i8, u8 [size=1, align=1];
// FLAG-NEXT:         storage i16, u16 [size=2, align=2];
// FLAG-NEXT:         storage i32, u32 [size=4, align=4];
// FLAG-NEXT:         storage i64, u64 [size=8, align=8];
// FLAG-NEXT:         storage i128, u128 [size=16, align=16];
// FLAG-NEXT:         storage bf16 [size=2, align=2];
// FLAG-NEXT:         storage f16 [size=2, align=2];
// FLAG-NEXT:         storage f32 [size=4, align=4];
// FLAG-NEXT:         storage f64 [size=8, align=8];
// FLAG-NEXT:         storage f80 [size=16, align=16];
// FLAG-NEXT:         storage f128 [size=16, align=16];
// FLAG-NEXT:         storage d32 [size=4, align=4];
// FLAG-NEXT:         storage d64 [size=8, align=8];
// FLAG-NEXT:         storage d128 [size=16, align=16];
// FLAG-NEXT:     }
// FLAG-NEXT:     global %[[VALUE_x_undefined:[0-9]+]] x_undefined: array<i32, 2> [storage=static] [linkage=external];
// FLAG-NEXT:     global %[[VALUE_flag_without_x:[0-9]+]] flag_without_x: array<i32, 3> [storage=static] [linkage=external];
// FLAG-NEXT:     global %[[VALUE_x_missing:[0-9]+]] x_missing: array<i32, 4> [storage=static] [align=16] [linkage=external];
// FLAG-NEXT: }
// SLATE-FILECHECK-END FLAG
// SLATE-FILECHECK-BEGIN X
// X: module {
// X-NEXT:     target "x86_64-unknown-linux-gnu" {
// X-NEXT:         endian = little;
// X-NEXT:         pointer [size=8, align=8];
// X-NEXT:         stack_alignment = 16;
// X-NEXT:         long_double = f80;
// X-NEXT:         storage bool [size=1, align=1];
// X-NEXT:         storage i8, u8 [size=1, align=1];
// X-NEXT:         storage i16, u16 [size=2, align=2];
// X-NEXT:         storage i32, u32 [size=4, align=4];
// X-NEXT:         storage i64, u64 [size=8, align=8];
// X-NEXT:         storage i128, u128 [size=16, align=16];
// X-NEXT:         storage bf16 [size=2, align=2];
// X-NEXT:         storage f16 [size=2, align=2];
// X-NEXT:         storage f32 [size=4, align=4];
// X-NEXT:         storage f64 [size=8, align=8];
// X-NEXT:         storage f80 [size=16, align=16];
// X-NEXT:         storage f128 [size=16, align=16];
// X-NEXT:         storage d32 [size=4, align=4];
// X-NEXT:         storage d64 [size=8, align=8];
// X-NEXT:         storage d128 [size=16, align=16];
// X-NEXT:     }
// X-NEXT:     global %[[VALUE_x_defined:[0-9]+]] x_defined: array<i32, 1> [storage=static] [linkage=external];
// X-NEXT: }
// SLATE-FILECHECK-END X
