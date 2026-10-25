#ifdef __NO_INLINE__
int no_inline_defined;
#else
int no_inline_undefined;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES UNOPTIMIZED
// SLATE-FILECHECK-PREFIX-ARGS UNOPTIMIZED
// SLATE-FILECHECK-DEFINES OPTIMIZED
// SLATE-FILECHECK-PREFIX-ARGS OPTIMIZED -O2
// SLATE-FILECHECK-DEFINES NO_INLINE
// SLATE-FILECHECK-PREFIX-ARGS NO_INLINE -O2 -fno-inline
// SLATE-FILECHECK-DEFINES NO_INLINE_FUNCTIONS
// SLATE-FILECHECK-PREFIX-ARGS NO_INLINE_FUNCTIONS -O2 -fno-inline-functions
// SLATE-FILECHECK-DEFINES INLINE_WINS
// SLATE-FILECHECK-PREFIX-ARGS INLINE_WINS -O2 -fno-inline-functions -finline
// SLATE-FILECHECK-DEFINES NO_INLINE_WINS
// SLATE-FILECHECK-PREFIX-ARGS NO_INLINE_WINS -O2 -fno-inline -finline-hint-functions

// SLATE-FILECHECK-BEGIN UNOPTIMIZED
// UNOPTIMIZED: module {
// UNOPTIMIZED-NEXT:     target "x86_64-unknown-linux-gnu" {
// UNOPTIMIZED-NEXT:         endian = little;
// UNOPTIMIZED-NEXT:         pointer [size=8, align=8];
// UNOPTIMIZED-NEXT:         stack_alignment = 16;
// UNOPTIMIZED-NEXT:         long_double = f80;
// UNOPTIMIZED-NEXT:         storage bool [size=1, align=1];
// UNOPTIMIZED-NEXT:         storage i8, u8 [size=1, align=1];
// UNOPTIMIZED-NEXT:         storage i16, u16 [size=2, align=2];
// UNOPTIMIZED-NEXT:         storage i32, u32 [size=4, align=4];
// UNOPTIMIZED-NEXT:         storage i64, u64 [size=8, align=8];
// UNOPTIMIZED-NEXT:         storage i128, u128 [size=16, align=16];
// UNOPTIMIZED-NEXT:         storage bf16 [size=2, align=2];
// UNOPTIMIZED-NEXT:         storage f16 [size=2, align=2];
// UNOPTIMIZED-NEXT:         storage f32 [size=4, align=4];
// UNOPTIMIZED-NEXT:         storage f64 [size=8, align=8];
// UNOPTIMIZED-NEXT:         storage f80 [size=16, align=16];
// UNOPTIMIZED-NEXT:         storage f128 [size=16, align=16];
// UNOPTIMIZED-NEXT:         storage d32 [size=4, align=4];
// UNOPTIMIZED-NEXT:         storage d64 [size=8, align=8];
// UNOPTIMIZED-NEXT:         storage d128 [size=16, align=16];
// UNOPTIMIZED-NEXT:     }
// UNOPTIMIZED-NEXT:     global %[[VALUE_no_inline_defined:[0-9]+]] no_inline_defined: i32 [storage=static] [linkage=external];
// UNOPTIMIZED-NEXT: }
// SLATE-FILECHECK-END UNOPTIMIZED
// SLATE-FILECHECK-BEGIN OPTIMIZED
// OPTIMIZED: module {
// OPTIMIZED-NEXT:     target "x86_64-unknown-linux-gnu" {
// OPTIMIZED-NEXT:         endian = little;
// OPTIMIZED-NEXT:         pointer [size=8, align=8];
// OPTIMIZED-NEXT:         stack_alignment = 16;
// OPTIMIZED-NEXT:         long_double = f80;
// OPTIMIZED-NEXT:         storage bool [size=1, align=1];
// OPTIMIZED-NEXT:         storage i8, u8 [size=1, align=1];
// OPTIMIZED-NEXT:         storage i16, u16 [size=2, align=2];
// OPTIMIZED-NEXT:         storage i32, u32 [size=4, align=4];
// OPTIMIZED-NEXT:         storage i64, u64 [size=8, align=8];
// OPTIMIZED-NEXT:         storage i128, u128 [size=16, align=16];
// OPTIMIZED-NEXT:         storage bf16 [size=2, align=2];
// OPTIMIZED-NEXT:         storage f16 [size=2, align=2];
// OPTIMIZED-NEXT:         storage f32 [size=4, align=4];
// OPTIMIZED-NEXT:         storage f64 [size=8, align=8];
// OPTIMIZED-NEXT:         storage f80 [size=16, align=16];
// OPTIMIZED-NEXT:         storage f128 [size=16, align=16];
// OPTIMIZED-NEXT:         storage d32 [size=4, align=4];
// OPTIMIZED-NEXT:         storage d64 [size=8, align=8];
// OPTIMIZED-NEXT:         storage d128 [size=16, align=16];
// OPTIMIZED-NEXT:     }
// OPTIMIZED-NEXT:     global %[[VALUE_no_inline_undefined:[0-9]+]] no_inline_undefined: i32 [storage=static] [linkage=external];
// OPTIMIZED-NEXT: }
// SLATE-FILECHECK-END OPTIMIZED
// SLATE-FILECHECK-BEGIN NO_INLINE
// NO_INLINE: module {
// NO_INLINE-NEXT:     target "x86_64-unknown-linux-gnu" {
// NO_INLINE-NEXT:         endian = little;
// NO_INLINE-NEXT:         pointer [size=8, align=8];
// NO_INLINE-NEXT:         stack_alignment = 16;
// NO_INLINE-NEXT:         long_double = f80;
// NO_INLINE-NEXT:         storage bool [size=1, align=1];
// NO_INLINE-NEXT:         storage i8, u8 [size=1, align=1];
// NO_INLINE-NEXT:         storage i16, u16 [size=2, align=2];
// NO_INLINE-NEXT:         storage i32, u32 [size=4, align=4];
// NO_INLINE-NEXT:         storage i64, u64 [size=8, align=8];
// NO_INLINE-NEXT:         storage i128, u128 [size=16, align=16];
// NO_INLINE-NEXT:         storage bf16 [size=2, align=2];
// NO_INLINE-NEXT:         storage f16 [size=2, align=2];
// NO_INLINE-NEXT:         storage f32 [size=4, align=4];
// NO_INLINE-NEXT:         storage f64 [size=8, align=8];
// NO_INLINE-NEXT:         storage f80 [size=16, align=16];
// NO_INLINE-NEXT:         storage f128 [size=16, align=16];
// NO_INLINE-NEXT:         storage d32 [size=4, align=4];
// NO_INLINE-NEXT:         storage d64 [size=8, align=8];
// NO_INLINE-NEXT:         storage d128 [size=16, align=16];
// NO_INLINE-NEXT:     }
// NO_INLINE-NEXT:     global %[[VALUE_no_inline_defined:[0-9]+]] no_inline_defined: i32 [storage=static] [linkage=external];
// NO_INLINE-NEXT: }
// SLATE-FILECHECK-END NO_INLINE
// SLATE-FILECHECK-BEGIN NO_INLINE_FUNCTIONS
// NO_INLINE_FUNCTIONS: module {
// NO_INLINE_FUNCTIONS-NEXT:     target "x86_64-unknown-linux-gnu" {
// NO_INLINE_FUNCTIONS-NEXT:         endian = little;
// NO_INLINE_FUNCTIONS-NEXT:         pointer [size=8, align=8];
// NO_INLINE_FUNCTIONS-NEXT:         stack_alignment = 16;
// NO_INLINE_FUNCTIONS-NEXT:         long_double = f80;
// NO_INLINE_FUNCTIONS-NEXT:         storage bool [size=1, align=1];
// NO_INLINE_FUNCTIONS-NEXT:         storage i8, u8 [size=1, align=1];
// NO_INLINE_FUNCTIONS-NEXT:         storage i16, u16 [size=2, align=2];
// NO_INLINE_FUNCTIONS-NEXT:         storage i32, u32 [size=4, align=4];
// NO_INLINE_FUNCTIONS-NEXT:         storage i64, u64 [size=8, align=8];
// NO_INLINE_FUNCTIONS-NEXT:         storage i128, u128 [size=16, align=16];
// NO_INLINE_FUNCTIONS-NEXT:         storage bf16 [size=2, align=2];
// NO_INLINE_FUNCTIONS-NEXT:         storage f16 [size=2, align=2];
// NO_INLINE_FUNCTIONS-NEXT:         storage f32 [size=4, align=4];
// NO_INLINE_FUNCTIONS-NEXT:         storage f64 [size=8, align=8];
// NO_INLINE_FUNCTIONS-NEXT:         storage f80 [size=16, align=16];
// NO_INLINE_FUNCTIONS-NEXT:         storage f128 [size=16, align=16];
// NO_INLINE_FUNCTIONS-NEXT:         storage d32 [size=4, align=4];
// NO_INLINE_FUNCTIONS-NEXT:         storage d64 [size=8, align=8];
// NO_INLINE_FUNCTIONS-NEXT:         storage d128 [size=16, align=16];
// NO_INLINE_FUNCTIONS-NEXT:     }
// NO_INLINE_FUNCTIONS-NEXT:     global %[[VALUE_no_inline_defined:[0-9]+]] no_inline_defined: i32 [storage=static] [linkage=external];
// NO_INLINE_FUNCTIONS-NEXT: }
// SLATE-FILECHECK-END NO_INLINE_FUNCTIONS
// SLATE-FILECHECK-BEGIN INLINE_WINS
// INLINE_WINS: module {
// INLINE_WINS-NEXT:     target "x86_64-unknown-linux-gnu" {
// INLINE_WINS-NEXT:         endian = little;
// INLINE_WINS-NEXT:         pointer [size=8, align=8];
// INLINE_WINS-NEXT:         stack_alignment = 16;
// INLINE_WINS-NEXT:         long_double = f80;
// INLINE_WINS-NEXT:         storage bool [size=1, align=1];
// INLINE_WINS-NEXT:         storage i8, u8 [size=1, align=1];
// INLINE_WINS-NEXT:         storage i16, u16 [size=2, align=2];
// INLINE_WINS-NEXT:         storage i32, u32 [size=4, align=4];
// INLINE_WINS-NEXT:         storage i64, u64 [size=8, align=8];
// INLINE_WINS-NEXT:         storage i128, u128 [size=16, align=16];
// INLINE_WINS-NEXT:         storage bf16 [size=2, align=2];
// INLINE_WINS-NEXT:         storage f16 [size=2, align=2];
// INLINE_WINS-NEXT:         storage f32 [size=4, align=4];
// INLINE_WINS-NEXT:         storage f64 [size=8, align=8];
// INLINE_WINS-NEXT:         storage f80 [size=16, align=16];
// INLINE_WINS-NEXT:         storage f128 [size=16, align=16];
// INLINE_WINS-NEXT:         storage d32 [size=4, align=4];
// INLINE_WINS-NEXT:         storage d64 [size=8, align=8];
// INLINE_WINS-NEXT:         storage d128 [size=16, align=16];
// INLINE_WINS-NEXT:     }
// INLINE_WINS-NEXT:     global %[[VALUE_no_inline_undefined:[0-9]+]] no_inline_undefined: i32 [storage=static] [linkage=external];
// INLINE_WINS-NEXT: }
// SLATE-FILECHECK-END INLINE_WINS
// SLATE-FILECHECK-BEGIN NO_INLINE_WINS
// NO_INLINE_WINS: module {
// NO_INLINE_WINS-NEXT:     target "x86_64-unknown-linux-gnu" {
// NO_INLINE_WINS-NEXT:         endian = little;
// NO_INLINE_WINS-NEXT:         pointer [size=8, align=8];
// NO_INLINE_WINS-NEXT:         stack_alignment = 16;
// NO_INLINE_WINS-NEXT:         long_double = f80;
// NO_INLINE_WINS-NEXT:         storage bool [size=1, align=1];
// NO_INLINE_WINS-NEXT:         storage i8, u8 [size=1, align=1];
// NO_INLINE_WINS-NEXT:         storage i16, u16 [size=2, align=2];
// NO_INLINE_WINS-NEXT:         storage i32, u32 [size=4, align=4];
// NO_INLINE_WINS-NEXT:         storage i64, u64 [size=8, align=8];
// NO_INLINE_WINS-NEXT:         storage i128, u128 [size=16, align=16];
// NO_INLINE_WINS-NEXT:         storage bf16 [size=2, align=2];
// NO_INLINE_WINS-NEXT:         storage f16 [size=2, align=2];
// NO_INLINE_WINS-NEXT:         storage f32 [size=4, align=4];
// NO_INLINE_WINS-NEXT:         storage f64 [size=8, align=8];
// NO_INLINE_WINS-NEXT:         storage f80 [size=16, align=16];
// NO_INLINE_WINS-NEXT:         storage f128 [size=16, align=16];
// NO_INLINE_WINS-NEXT:         storage d32 [size=4, align=4];
// NO_INLINE_WINS-NEXT:         storage d64 [size=8, align=8];
// NO_INLINE_WINS-NEXT:         storage d128 [size=16, align=16];
// NO_INLINE_WINS-NEXT:     }
// NO_INLINE_WINS-NEXT:     global %[[VALUE_no_inline_defined:[0-9]+]] no_inline_defined: i32 [storage=static] [linkage=external];
// NO_INLINE_WINS-NEXT: }
// SLATE-FILECHECK-END NO_INLINE_WINS
