int eval_method = __FLT_EVAL_METHOD__;

#ifdef __SSE_MATH__
int sse_math = __SSE_MATH__;
#else
int sse_math = -1;
#endif

#ifdef __SSE2_MATH__
int sse2_math = __SSE2_MATH__;
#else
int sse2_math = -1;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES SSE
// SLATE-FILECHECK-PREFIX-ARGS SSE -msse2 -mfpmath=sse
// SLATE-FILECHECK-DEFINES NOSSE
// SLATE-FILECHECK-PREFIX-ARGS NOSSE -mno-sse -mfpmath=sse
// SLATE-FILECHECK-DEFINES FAST387
// SLATE-FILECHECK-PREFIX-ARGS FAST387 -mfpmath=387 -ffast-math

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=4];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=4];
// DEFAULT-NEXT:         storage f80 [size=12, align=4];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN SSE
// SSE: module {
// SSE-NEXT:     target "i686-unknown-linux-gnu" {
// SSE-NEXT:         endian = little;
// SSE-NEXT:         pointer [size=4, align=4];
// SSE-NEXT:         stack_alignment = 16;
// SSE-NEXT:         long_double = f80;
// SSE-NEXT:         storage bool [size=1, align=1];
// SSE-NEXT:         storage i8, u8 [size=1, align=1];
// SSE-NEXT:         storage i16, u16 [size=2, align=2];
// SSE-NEXT:         storage i32, u32 [size=4, align=4];
// SSE-NEXT:         storage i64, u64 [size=8, align=4];
// SSE-NEXT:         storage i128, u128 [size=16, align=16];
// SSE-NEXT:         storage bf16 [size=2, align=2];
// SSE-NEXT:         storage f16 [size=2, align=2];
// SSE-NEXT:         storage f32 [size=4, align=4];
// SSE-NEXT:         storage f64 [size=8, align=4];
// SSE-NEXT:         storage f80 [size=12, align=4];
// SSE-NEXT:         storage f128 [size=16, align=16];
// SSE-NEXT:         storage d32 [size=4, align=4];
// SSE-NEXT:         storage d64 [size=8, align=8];
// SSE-NEXT:         storage d128 [size=16, align=16];
// SSE-NEXT:     }
// SSE-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// SSE-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSE-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSE-NEXT: }
// SLATE-FILECHECK-END SSE
// SLATE-FILECHECK-BEGIN NOSSE
// NOSSE: module {
// NOSSE-NEXT:     target "i686-unknown-linux-gnu" {
// NOSSE-NEXT:         endian = little;
// NOSSE-NEXT:         pointer [size=4, align=4];
// NOSSE-NEXT:         stack_alignment = 16;
// NOSSE-NEXT:         long_double = f80;
// NOSSE-NEXT:         storage bool [size=1, align=1];
// NOSSE-NEXT:         storage i8, u8 [size=1, align=1];
// NOSSE-NEXT:         storage i16, u16 [size=2, align=2];
// NOSSE-NEXT:         storage i32, u32 [size=4, align=4];
// NOSSE-NEXT:         storage i64, u64 [size=8, align=4];
// NOSSE-NEXT:         storage i128, u128 [size=16, align=16];
// NOSSE-NEXT:         storage bf16 [size=2, align=2];
// NOSSE-NEXT:         storage f16 [size=2, align=2];
// NOSSE-NEXT:         storage f32 [size=4, align=4];
// NOSSE-NEXT:         storage f64 [size=8, align=4];
// NOSSE-NEXT:         storage f80 [size=12, align=4];
// NOSSE-NEXT:         storage f128 [size=16, align=16];
// NOSSE-NEXT:         storage d32 [size=4, align=4];
// NOSSE-NEXT:         storage d64 [size=8, align=8];
// NOSSE-NEXT:         storage d128 [size=16, align=16];
// NOSSE-NEXT:     }
// NOSSE-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOSSE-NEXT: }
// SLATE-FILECHECK-END NOSSE
// SLATE-FILECHECK-BEGIN FAST387
// FAST387: module {
// FAST387-NEXT:     target "i686-unknown-linux-gnu" {
// FAST387-NEXT:         endian = little;
// FAST387-NEXT:         pointer [size=4, align=4];
// FAST387-NEXT:         stack_alignment = 16;
// FAST387-NEXT:         long_double = f80;
// FAST387-NEXT:         storage bool [size=1, align=1];
// FAST387-NEXT:         storage i8, u8 [size=1, align=1];
// FAST387-NEXT:         storage i16, u16 [size=2, align=2];
// FAST387-NEXT:         storage i32, u32 [size=4, align=4];
// FAST387-NEXT:         storage i64, u64 [size=8, align=4];
// FAST387-NEXT:         storage i128, u128 [size=16, align=16];
// FAST387-NEXT:         storage bf16 [size=2, align=2];
// FAST387-NEXT:         storage f16 [size=2, align=2];
// FAST387-NEXT:         storage f32 [size=4, align=4];
// FAST387-NEXT:         storage f64 [size=8, align=4];
// FAST387-NEXT:         storage f80 [size=12, align=4];
// FAST387-NEXT:         storage f128 [size=16, align=16];
// FAST387-NEXT:         storage d32 [size=4, align=4];
// FAST387-NEXT:         storage d64 [size=8, align=8];
// FAST387-NEXT:         storage d128 [size=16, align=16];
// FAST387-NEXT:     }
// FAST387-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// FAST387-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FAST387-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FAST387-NEXT: }
// SLATE-FILECHECK-END FAST387
