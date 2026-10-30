int eval_method = __FLT_EVAL_METHOD__;
int eval_method_ts = __FLT_EVAL_METHOD_TS_18661_3__;

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

#ifdef __FP_FAST_FMA
int fast_fma = __FP_FAST_FMA;
#else
int fast_fma = -1;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES X87
// SLATE-FILECHECK-PREFIX-ARGS X87 -mfpmath=387
// SLATE-FILECHECK-DEFINES X87FMA
// SLATE-FILECHECK-PREFIX-ARGS X87FMA -mfpmath=387 -mfma
// SLATE-FILECHECK-DEFINES SSE
// SLATE-FILECHECK-PREFIX-ARGS SSE -mfpmath=sse
// SLATE-FILECHECK-DEFINES LAST
// SLATE-FILECHECK-PREFIX-ARGS LAST -mfpmath=387 -mfpmath=sse
// SLATE-FILECHECK-DEFINES BOTH
// SLATE-FILECHECK-PREFIX-ARGS BOTH -mfpmath=both
// SLATE-FILECHECK-DEFINES MIXED
// SLATE-FILECHECK-PREFIX-ARGS MIXED -mfpmath=387+sse
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -mfpmath=387 -ffast-math
// SLATE-FILECHECK-DEFINES STORE
// SLATE-FILECHECK-PREFIX-ARGS STORE -ffloat-store -fno-float-store -ffloat-store

// SLATE-FILECHECK-BEGIN X87
// X87: module {
// X87-NEXT:     target "x86_64-unknown-linux-gnu" {
// X87-NEXT:         endian = little;
// X87-NEXT:         pointer [size=8, align=8];
// X87-NEXT:         stack_alignment = 16;
// X87-NEXT:         long_double = f80;
// X87-NEXT:         storage bool [size=1, align=1];
// X87-NEXT:         storage i8, u8 [size=1, align=1];
// X87-NEXT:         storage i16, u16 [size=2, align=2];
// X87-NEXT:         storage i32, u32 [size=4, align=4];
// X87-NEXT:         storage i64, u64 [size=8, align=8];
// X87-NEXT:         storage i128, u128 [size=16, align=16];
// X87-NEXT:         storage bf16 [size=2, align=2];
// X87-NEXT:         storage f16 [size=2, align=2];
// X87-NEXT:         storage f32 [size=4, align=4];
// X87-NEXT:         storage f64 [size=8, align=8];
// X87-NEXT:         storage f80 [size=16, align=16];
// X87-NEXT:         storage f128 [size=16, align=16];
// X87-NEXT:         storage d32 [size=4, align=4];
// X87-NEXT:         storage d64 [size=8, align=8];
// X87-NEXT:         storage d128 [size=16, align=16];
// X87-NEXT:     }
// X87-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// X87-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(2) [linkage=external];
// X87-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// X87-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// X87-NEXT:     global %[[VALUE_fast_fma:[0-9]+]] fast_fma: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// X87-NEXT: }
// SLATE-FILECHECK-END X87
// SLATE-FILECHECK-BEGIN X87FMA
// X87FMA: module {
// X87FMA-NEXT:     target "x86_64-unknown-linux-gnu" {
// X87FMA-NEXT:         endian = little;
// X87FMA-NEXT:         pointer [size=8, align=8];
// X87FMA-NEXT:         stack_alignment = 16;
// X87FMA-NEXT:         long_double = f80;
// X87FMA-NEXT:         storage bool [size=1, align=1];
// X87FMA-NEXT:         storage i8, u8 [size=1, align=1];
// X87FMA-NEXT:         storage i16, u16 [size=2, align=2];
// X87FMA-NEXT:         storage i32, u32 [size=4, align=4];
// X87FMA-NEXT:         storage i64, u64 [size=8, align=8];
// X87FMA-NEXT:         storage i128, u128 [size=16, align=16];
// X87FMA-NEXT:         storage bf16 [size=2, align=2];
// X87FMA-NEXT:         storage f16 [size=2, align=2];
// X87FMA-NEXT:         storage f32 [size=4, align=4];
// X87FMA-NEXT:         storage f64 [size=8, align=8];
// X87FMA-NEXT:         storage f80 [size=16, align=16];
// X87FMA-NEXT:         storage f128 [size=16, align=16];
// X87FMA-NEXT:         storage d32 [size=4, align=4];
// X87FMA-NEXT:         storage d64 [size=8, align=8];
// X87FMA-NEXT:         storage d128 [size=16, align=16];
// X87FMA-NEXT:     }
// X87FMA-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// X87FMA-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(2) [linkage=external];
// X87FMA-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// X87FMA-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// X87FMA-NEXT:     global %[[VALUE_fast_fma:[0-9]+]] fast_fma: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// X87FMA-NEXT: }
// SLATE-FILECHECK-END X87FMA
// SLATE-FILECHECK-BEGIN SSE
// SSE: module {
// SSE-NEXT:     target "x86_64-unknown-linux-gnu" {
// SSE-NEXT:         endian = little;
// SSE-NEXT:         pointer [size=8, align=8];
// SSE-NEXT:         stack_alignment = 16;
// SSE-NEXT:         long_double = f80;
// SSE-NEXT:         storage bool [size=1, align=1];
// SSE-NEXT:         storage i8, u8 [size=1, align=1];
// SSE-NEXT:         storage i16, u16 [size=2, align=2];
// SSE-NEXT:         storage i32, u32 [size=4, align=4];
// SSE-NEXT:         storage i64, u64 [size=8, align=8];
// SSE-NEXT:         storage i128, u128 [size=16, align=16];
// SSE-NEXT:         storage bf16 [size=2, align=2];
// SSE-NEXT:         storage f16 [size=2, align=2];
// SSE-NEXT:         storage f32 [size=4, align=4];
// SSE-NEXT:         storage f64 [size=8, align=8];
// SSE-NEXT:         storage f80 [size=16, align=16];
// SSE-NEXT:         storage f128 [size=16, align=16];
// SSE-NEXT:         storage d32 [size=4, align=4];
// SSE-NEXT:         storage d64 [size=8, align=8];
// SSE-NEXT:         storage d128 [size=16, align=16];
// SSE-NEXT:     }
// SSE-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// SSE-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(0) [linkage=external];
// SSE-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSE-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// SSE-NEXT:     global %[[VALUE_fast_fma:[0-9]+]] fast_fma: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SSE-NEXT: }
// SLATE-FILECHECK-END SSE
// SLATE-FILECHECK-BEGIN LAST
// LAST: module {
// LAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// LAST-NEXT:         endian = little;
// LAST-NEXT:         pointer [size=8, align=8];
// LAST-NEXT:         stack_alignment = 16;
// LAST-NEXT:         long_double = f80;
// LAST-NEXT:         storage bool [size=1, align=1];
// LAST-NEXT:         storage i8, u8 [size=1, align=1];
// LAST-NEXT:         storage i16, u16 [size=2, align=2];
// LAST-NEXT:         storage i32, u32 [size=4, align=4];
// LAST-NEXT:         storage i64, u64 [size=8, align=8];
// LAST-NEXT:         storage i128, u128 [size=16, align=16];
// LAST-NEXT:         storage bf16 [size=2, align=2];
// LAST-NEXT:         storage f16 [size=2, align=2];
// LAST-NEXT:         storage f32 [size=4, align=4];
// LAST-NEXT:         storage f64 [size=8, align=8];
// LAST-NEXT:         storage f80 [size=16, align=16];
// LAST-NEXT:         storage f128 [size=16, align=16];
// LAST-NEXT:         storage d32 [size=4, align=4];
// LAST-NEXT:         storage d64 [size=8, align=8];
// LAST-NEXT:         storage d128 [size=16, align=16];
// LAST-NEXT:     }
// LAST-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// LAST-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(0) [linkage=external];
// LAST-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// LAST-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// LAST-NEXT:     global %[[VALUE_fast_fma:[0-9]+]] fast_fma: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// LAST-NEXT: }
// SLATE-FILECHECK-END LAST
// SLATE-FILECHECK-BEGIN BOTH
// BOTH: module {
// BOTH-NEXT:     target "x86_64-unknown-linux-gnu" {
// BOTH-NEXT:         endian = little;
// BOTH-NEXT:         pointer [size=8, align=8];
// BOTH-NEXT:         stack_alignment = 16;
// BOTH-NEXT:         long_double = f80;
// BOTH-NEXT:         storage bool [size=1, align=1];
// BOTH-NEXT:         storage i8, u8 [size=1, align=1];
// BOTH-NEXT:         storage i16, u16 [size=2, align=2];
// BOTH-NEXT:         storage i32, u32 [size=4, align=4];
// BOTH-NEXT:         storage i64, u64 [size=8, align=8];
// BOTH-NEXT:         storage i128, u128 [size=16, align=16];
// BOTH-NEXT:         storage bf16 [size=2, align=2];
// BOTH-NEXT:         storage f16 [size=2, align=2];
// BOTH-NEXT:         storage f32 [size=4, align=4];
// BOTH-NEXT:         storage f64 [size=8, align=8];
// BOTH-NEXT:         storage f80 [size=16, align=16];
// BOTH-NEXT:         storage f128 [size=16, align=16];
// BOTH-NEXT:         storage d32 [size=4, align=4];
// BOTH-NEXT:         storage d64 [size=8, align=8];
// BOTH-NEXT:         storage d128 [size=16, align=16];
// BOTH-NEXT:     }
// BOTH-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// BOTH-NEXT:     global %[[VALUE_fast_fma:[0-9]+]] fast_fma: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// BOTH-NEXT: }
// SLATE-FILECHECK-END BOTH
// SLATE-FILECHECK-BEGIN MIXED
// MIXED: module {
// MIXED-NEXT:     target "x86_64-unknown-linux-gnu" {
// MIXED-NEXT:         endian = little;
// MIXED-NEXT:         pointer [size=8, align=8];
// MIXED-NEXT:         stack_alignment = 16;
// MIXED-NEXT:         long_double = f80;
// MIXED-NEXT:         storage bool [size=1, align=1];
// MIXED-NEXT:         storage i8, u8 [size=1, align=1];
// MIXED-NEXT:         storage i16, u16 [size=2, align=2];
// MIXED-NEXT:         storage i32, u32 [size=4, align=4];
// MIXED-NEXT:         storage i64, u64 [size=8, align=8];
// MIXED-NEXT:         storage i128, u128 [size=16, align=16];
// MIXED-NEXT:         storage bf16 [size=2, align=2];
// MIXED-NEXT:         storage f16 [size=2, align=2];
// MIXED-NEXT:         storage f32 [size=4, align=4];
// MIXED-NEXT:         storage f64 [size=8, align=8];
// MIXED-NEXT:         storage f80 [size=16, align=16];
// MIXED-NEXT:         storage f128 [size=16, align=16];
// MIXED-NEXT:         storage d32 [size=4, align=4];
// MIXED-NEXT:         storage d64 [size=8, align=8];
// MIXED-NEXT:         storage d128 [size=16, align=16];
// MIXED-NEXT:     }
// MIXED-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MIXED-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MIXED-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// MIXED-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// MIXED-NEXT:     global %[[VALUE_fast_fma:[0-9]+]] fast_fma: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// MIXED-NEXT: }
// SLATE-FILECHECK-END MIXED
// SLATE-FILECHECK-BEGIN FAST
// FAST: module {
// FAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// FAST-NEXT:         endian = little;
// FAST-NEXT:         pointer [size=8, align=8];
// FAST-NEXT:         stack_alignment = 16;
// FAST-NEXT:         long_double = f80;
// FAST-NEXT:         storage bool [size=1, align=1];
// FAST-NEXT:         storage i8, u8 [size=1, align=1];
// FAST-NEXT:         storage i16, u16 [size=2, align=2];
// FAST-NEXT:         storage i32, u32 [size=4, align=4];
// FAST-NEXT:         storage i64, u64 [size=8, align=8];
// FAST-NEXT:         storage i128, u128 [size=16, align=16];
// FAST-NEXT:         storage bf16 [size=2, align=2];
// FAST-NEXT:         storage f16 [size=2, align=2];
// FAST-NEXT:         storage f32 [size=4, align=4];
// FAST-NEXT:         storage f64 [size=8, align=8];
// FAST-NEXT:         storage f80 [size=16, align=16];
// FAST-NEXT:         storage f128 [size=16, align=16];
// FAST-NEXT:         storage d32 [size=4, align=4];
// FAST-NEXT:         storage d64 [size=8, align=8];
// FAST-NEXT:         storage d128 [size=16, align=16];
// FAST-NEXT:     }
// FAST-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// FAST-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(2) [linkage=external];
// FAST-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FAST-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FAST-NEXT:     global %[[VALUE_fast_fma:[0-9]+]] fast_fma: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FAST-NEXT: }
// SLATE-FILECHECK-END FAST
// SLATE-FILECHECK-BEGIN STORE
// STORE: module {
// STORE-NEXT:     target "x86_64-unknown-linux-gnu" {
// STORE-NEXT:         endian = little;
// STORE-NEXT:         pointer [size=8, align=8];
// STORE-NEXT:         stack_alignment = 16;
// STORE-NEXT:         long_double = f80;
// STORE-NEXT:         storage bool [size=1, align=1];
// STORE-NEXT:         storage i8, u8 [size=1, align=1];
// STORE-NEXT:         storage i16, u16 [size=2, align=2];
// STORE-NEXT:         storage i32, u32 [size=4, align=4];
// STORE-NEXT:         storage i64, u64 [size=8, align=8];
// STORE-NEXT:         storage i128, u128 [size=16, align=16];
// STORE-NEXT:         storage bf16 [size=2, align=2];
// STORE-NEXT:         storage f16 [size=2, align=2];
// STORE-NEXT:         storage f32 [size=4, align=4];
// STORE-NEXT:         storage f64 [size=8, align=8];
// STORE-NEXT:         storage f80 [size=16, align=16];
// STORE-NEXT:         storage f128 [size=16, align=16];
// STORE-NEXT:         storage d32 [size=4, align=4];
// STORE-NEXT:         storage d64 [size=8, align=8];
// STORE-NEXT:         storage d128 [size=16, align=16];
// STORE-NEXT:     }
// STORE-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// STORE-NEXT:     global %[[VALUE_eval_method_ts:[0-9]+]] eval_method_ts: i32 [storage=static] = const<i32>(0) [linkage=external];
// STORE-NEXT:     global %[[VALUE_sse_math:[0-9]+]] sse_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// STORE-NEXT:     global %[[VALUE_sse2_math:[0-9]+]] sse2_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// STORE-NEXT:     global %[[VALUE_fast_fma:[0-9]+]] fast_fma: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// STORE-NEXT: }
// SLATE-FILECHECK-END STORE
