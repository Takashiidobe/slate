int eval_method = __FLT_EVAL_METHOD__;

#ifdef __ARM_FP_FAST
int arm_fp_fast = __ARM_FP_FAST;
#else
int arm_fp_fast = -1;
#endif

#ifdef __FAST_MATH__
int fast_math = __FAST_MATH__;
#else
int fast_math = -1;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -march=armv8.2-a+fp16
// SLATE-FILECHECK-DEFINES PERMITTED
// SLATE-FILECHECK-PREFIX-ARGS PERMITTED -march=armv8.2-a+fp16 -fpermitted-flt-eval-methods=ts-18661-3
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -ffast-math
// SLATE-FILECHECK-DEFINES UNSAFE
// SLATE-FILECHECK-PREFIX-ARGS UNSAFE -funsafe-math-optimizations

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f128;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(16) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_arm_fp_fast:[0-9]+]] arm_fp_fast: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN PERMITTED
// PERMITTED: module {
// PERMITTED-NEXT:     target "aarch64-unknown-linux-gnu" {
// PERMITTED-NEXT:         endian = little;
// PERMITTED-NEXT:         pointer [size=8, align=8];
// PERMITTED-NEXT:         stack_alignment = 16;
// PERMITTED-NEXT:         long_double = f128;
// PERMITTED-NEXT:         storage bool [size=1, align=1];
// PERMITTED-NEXT:         storage i8, u8 [size=1, align=1];
// PERMITTED-NEXT:         storage i16, u16 [size=2, align=2];
// PERMITTED-NEXT:         storage i32, u32 [size=4, align=4];
// PERMITTED-NEXT:         storage i64, u64 [size=8, align=8];
// PERMITTED-NEXT:         storage i128, u128 [size=16, align=16];
// PERMITTED-NEXT:         storage bf16 [size=2, align=2];
// PERMITTED-NEXT:         storage f16 [size=2, align=2];
// PERMITTED-NEXT:         storage f32 [size=4, align=4];
// PERMITTED-NEXT:         storage f64 [size=8, align=8];
// PERMITTED-NEXT:         storage f128 [size=16, align=16];
// PERMITTED-NEXT:         storage d32 [size=4, align=4];
// PERMITTED-NEXT:         storage d64 [size=8, align=8];
// PERMITTED-NEXT:         storage d128 [size=16, align=16];
// PERMITTED-NEXT:     }
// PERMITTED-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// PERMITTED-NEXT:     global %[[VALUE_arm_fp_fast:[0-9]+]] arm_fp_fast: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// PERMITTED-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// PERMITTED-NEXT: }
// SLATE-FILECHECK-END PERMITTED
// SLATE-FILECHECK-BEGIN FAST
// FAST: module {
// FAST-NEXT:     target "aarch64-unknown-linux-gnu" {
// FAST-NEXT:         endian = little;
// FAST-NEXT:         pointer [size=8, align=8];
// FAST-NEXT:         stack_alignment = 16;
// FAST-NEXT:         long_double = f128;
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
// FAST-NEXT:         storage f128 [size=16, align=16];
// FAST-NEXT:         storage d32 [size=4, align=4];
// FAST-NEXT:         storage d64 [size=8, align=8];
// FAST-NEXT:         storage d128 [size=16, align=16];
// FAST-NEXT:     }
// FAST-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST-NEXT:     global %[[VALUE_arm_fp_fast:[0-9]+]] arm_fp_fast: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT: }
// SLATE-FILECHECK-END FAST
// SLATE-FILECHECK-BEGIN UNSAFE
// UNSAFE: module {
// UNSAFE-NEXT:     target "aarch64-unknown-linux-gnu" {
// UNSAFE-NEXT:         endian = little;
// UNSAFE-NEXT:         pointer [size=8, align=8];
// UNSAFE-NEXT:         stack_alignment = 16;
// UNSAFE-NEXT:         long_double = f128;
// UNSAFE-NEXT:         storage bool [size=1, align=1];
// UNSAFE-NEXT:         storage i8, u8 [size=1, align=1];
// UNSAFE-NEXT:         storage i16, u16 [size=2, align=2];
// UNSAFE-NEXT:         storage i32, u32 [size=4, align=4];
// UNSAFE-NEXT:         storage i64, u64 [size=8, align=8];
// UNSAFE-NEXT:         storage i128, u128 [size=16, align=16];
// UNSAFE-NEXT:         storage bf16 [size=2, align=2];
// UNSAFE-NEXT:         storage f16 [size=2, align=2];
// UNSAFE-NEXT:         storage f32 [size=4, align=4];
// UNSAFE-NEXT:         storage f64 [size=8, align=8];
// UNSAFE-NEXT:         storage f128 [size=16, align=16];
// UNSAFE-NEXT:         storage d32 [size=4, align=4];
// UNSAFE-NEXT:         storage d64 [size=8, align=8];
// UNSAFE-NEXT:         storage d128 [size=16, align=16];
// UNSAFE-NEXT:     }
// UNSAFE-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_arm_fp_fast:[0-9]+]] arm_fp_fast: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// UNSAFE-NEXT: }
// SLATE-FILECHECK-END UNSAFE
