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
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES UNSAFE
// SLATE-FILECHECK-PREFIX-ARGS UNSAFE -funsafe-math-optimizations
// SLATE-FILECHECK-DEFINES SIGNED
// SLATE-FILECHECK-PREFIX-ARGS SIGNED -funsafe-math-optimizations -fsigned-zeros

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
// DEFAULT-NEXT:     global %[[VALUE_arm_fp_fast:[0-9]+]] arm_fp_fast: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
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
// UNSAFE-NEXT:     global %[[VALUE_arm_fp_fast:[0-9]+]] arm_fp_fast: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// UNSAFE-NEXT: }
// SLATE-FILECHECK-END UNSAFE
// SLATE-FILECHECK-BEGIN SIGNED
// SIGNED: module {
// SIGNED-NEXT:     target "aarch64-unknown-linux-gnu" {
// SIGNED-NEXT:         endian = little;
// SIGNED-NEXT:         pointer [size=8, align=8];
// SIGNED-NEXT:         stack_alignment = 16;
// SIGNED-NEXT:         long_double = f128;
// SIGNED-NEXT:         storage bool [size=1, align=1];
// SIGNED-NEXT:         storage i8, u8 [size=1, align=1];
// SIGNED-NEXT:         storage i16, u16 [size=2, align=2];
// SIGNED-NEXT:         storage i32, u32 [size=4, align=4];
// SIGNED-NEXT:         storage i64, u64 [size=8, align=8];
// SIGNED-NEXT:         storage i128, u128 [size=16, align=16];
// SIGNED-NEXT:         storage bf16 [size=2, align=2];
// SIGNED-NEXT:         storage f16 [size=2, align=2];
// SIGNED-NEXT:         storage f32 [size=4, align=4];
// SIGNED-NEXT:         storage f64 [size=8, align=8];
// SIGNED-NEXT:         storage f128 [size=16, align=16];
// SIGNED-NEXT:         storage d32 [size=4, align=4];
// SIGNED-NEXT:         storage d64 [size=8, align=8];
// SIGNED-NEXT:         storage d128 [size=16, align=16];
// SIGNED-NEXT:     }
// SIGNED-NEXT:     global %[[VALUE_arm_fp_fast:[0-9]+]] arm_fp_fast: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SIGNED-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SIGNED-NEXT: }
// SLATE-FILECHECK-END SIGNED
