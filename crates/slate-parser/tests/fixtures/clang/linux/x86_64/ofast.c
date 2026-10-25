#ifdef __FAST_MATH__
int fast_math = __FAST_MATH__;
#endif
#ifdef __FINITE_MATH_ONLY__
int finite_math_only = __FINITE_MATH_ONLY__;
#endif
#ifdef __OPTIMIZE__
int optimize = __OPTIMIZE__;
#endif

double fused(double a, double b, double c) { return a * b + c; }

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES OFAST
// SLATE-FILECHECK-PREFIX-ARGS OFAST -Ofast
// SLATE-FILECHECK-DEFINES LATER_LEVEL
// SLATE-FILECHECK-PREFIX-ARGS LATER_LEVEL -Ofast -O2
// SLATE-FILECHECK-DEFINES NEGATED_BEFORE
// SLATE-FILECHECK-PREFIX-ARGS NEGATED_BEFORE -fno-fast-math -Ofast
// SLATE-FILECHECK-DEFINES COMPONENT_BEFORE
// SLATE-FILECHECK-PREFIX-ARGS COMPONENT_BEFORE -fno-finite-math-only -Ofast
// SLATE-FILECHECK-DEFINES CONTRACT_BEFORE
// SLATE-FILECHECK-PREFIX-ARGS CONTRACT_BEFORE -ffp-contract=off -Ofast

// SLATE-FILECHECK-BEGIN OFAST
// OFAST: module {
// OFAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// OFAST-NEXT:         endian = little;
// OFAST-NEXT:         pointer [size=8, align=8];
// OFAST-NEXT:         stack_alignment = 16;
// OFAST-NEXT:         long_double = f80;
// OFAST-NEXT:         storage bool [size=1, align=1];
// OFAST-NEXT:         storage i8, u8 [size=1, align=1];
// OFAST-NEXT:         storage i16, u16 [size=2, align=2];
// OFAST-NEXT:         storage i32, u32 [size=4, align=4];
// OFAST-NEXT:         storage i64, u64 [size=8, align=8];
// OFAST-NEXT:         storage i128, u128 [size=16, align=16];
// OFAST-NEXT:         storage bf16 [size=2, align=2];
// OFAST-NEXT:         storage f16 [size=2, align=2];
// OFAST-NEXT:         storage f32 [size=4, align=4];
// OFAST-NEXT:         storage f64 [size=8, align=8];
// OFAST-NEXT:         storage f80 [size=16, align=16];
// OFAST-NEXT:         storage f128 [size=16, align=16];
// OFAST-NEXT:         storage d32 [size=4, align=4];
// OFAST-NEXT:         storage d64 [size=8, align=8];
// OFAST-NEXT:         storage d128 [size=16, align=16];
// OFAST-NEXT:     }
// OFAST-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// OFAST-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// OFAST-NEXT:     global %[[VALUE_optimize:[0-9]+]] optimize: i32 [storage=static] = const<i32>(1) [linkage=external];
// OFAST-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// OFAST-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// OFAST-NEXT:     }
// OFAST-NEXT: }
// SLATE-FILECHECK-END OFAST
// SLATE-FILECHECK-BEGIN LATER_LEVEL
// LATER_LEVEL: module {
// LATER_LEVEL-NEXT:     target "x86_64-unknown-linux-gnu" {
// LATER_LEVEL-NEXT:         endian = little;
// LATER_LEVEL-NEXT:         pointer [size=8, align=8];
// LATER_LEVEL-NEXT:         stack_alignment = 16;
// LATER_LEVEL-NEXT:         long_double = f80;
// LATER_LEVEL-NEXT:         storage bool [size=1, align=1];
// LATER_LEVEL-NEXT:         storage i8, u8 [size=1, align=1];
// LATER_LEVEL-NEXT:         storage i16, u16 [size=2, align=2];
// LATER_LEVEL-NEXT:         storage i32, u32 [size=4, align=4];
// LATER_LEVEL-NEXT:         storage i64, u64 [size=8, align=8];
// LATER_LEVEL-NEXT:         storage i128, u128 [size=16, align=16];
// LATER_LEVEL-NEXT:         storage bf16 [size=2, align=2];
// LATER_LEVEL-NEXT:         storage f16 [size=2, align=2];
// LATER_LEVEL-NEXT:         storage f32 [size=4, align=4];
// LATER_LEVEL-NEXT:         storage f64 [size=8, align=8];
// LATER_LEVEL-NEXT:         storage f80 [size=16, align=16];
// LATER_LEVEL-NEXT:         storage f128 [size=16, align=16];
// LATER_LEVEL-NEXT:         storage d32 [size=4, align=4];
// LATER_LEVEL-NEXT:         storage d64 [size=8, align=8];
// LATER_LEVEL-NEXT:         storage d128 [size=16, align=16];
// LATER_LEVEL-NEXT:     }
// LATER_LEVEL-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// LATER_LEVEL-NEXT:     global %[[VALUE_optimize:[0-9]+]] optimize: i32 [storage=static] = const<i32>(1) [linkage=external];
// LATER_LEVEL-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// LATER_LEVEL-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// LATER_LEVEL-NEXT:     }
// LATER_LEVEL-NEXT: }
// SLATE-FILECHECK-END LATER_LEVEL
// SLATE-FILECHECK-BEGIN NEGATED_BEFORE
// NEGATED_BEFORE: module {
// NEGATED_BEFORE-NEXT:     target "x86_64-unknown-linux-gnu" {
// NEGATED_BEFORE-NEXT:         endian = little;
// NEGATED_BEFORE-NEXT:         pointer [size=8, align=8];
// NEGATED_BEFORE-NEXT:         stack_alignment = 16;
// NEGATED_BEFORE-NEXT:         long_double = f80;
// NEGATED_BEFORE-NEXT:         storage bool [size=1, align=1];
// NEGATED_BEFORE-NEXT:         storage i8, u8 [size=1, align=1];
// NEGATED_BEFORE-NEXT:         storage i16, u16 [size=2, align=2];
// NEGATED_BEFORE-NEXT:         storage i32, u32 [size=4, align=4];
// NEGATED_BEFORE-NEXT:         storage i64, u64 [size=8, align=8];
// NEGATED_BEFORE-NEXT:         storage i128, u128 [size=16, align=16];
// NEGATED_BEFORE-NEXT:         storage bf16 [size=2, align=2];
// NEGATED_BEFORE-NEXT:         storage f16 [size=2, align=2];
// NEGATED_BEFORE-NEXT:         storage f32 [size=4, align=4];
// NEGATED_BEFORE-NEXT:         storage f64 [size=8, align=8];
// NEGATED_BEFORE-NEXT:         storage f80 [size=16, align=16];
// NEGATED_BEFORE-NEXT:         storage f128 [size=16, align=16];
// NEGATED_BEFORE-NEXT:         storage d32 [size=4, align=4];
// NEGATED_BEFORE-NEXT:         storage d64 [size=8, align=8];
// NEGATED_BEFORE-NEXT:         storage d128 [size=16, align=16];
// NEGATED_BEFORE-NEXT:     }
// NEGATED_BEFORE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// NEGATED_BEFORE-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// NEGATED_BEFORE-NEXT:     global %[[VALUE_optimize:[0-9]+]] optimize: i32 [storage=static] = const<i32>(1) [linkage=external];
// NEGATED_BEFORE-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NEGATED_BEFORE-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// NEGATED_BEFORE-NEXT:     }
// NEGATED_BEFORE-NEXT: }
// SLATE-FILECHECK-END NEGATED_BEFORE
// SLATE-FILECHECK-BEGIN COMPONENT_BEFORE
// COMPONENT_BEFORE: module {
// COMPONENT_BEFORE-NEXT:     target "x86_64-unknown-linux-gnu" {
// COMPONENT_BEFORE-NEXT:         endian = little;
// COMPONENT_BEFORE-NEXT:         pointer [size=8, align=8];
// COMPONENT_BEFORE-NEXT:         stack_alignment = 16;
// COMPONENT_BEFORE-NEXT:         long_double = f80;
// COMPONENT_BEFORE-NEXT:         storage bool [size=1, align=1];
// COMPONENT_BEFORE-NEXT:         storage i8, u8 [size=1, align=1];
// COMPONENT_BEFORE-NEXT:         storage i16, u16 [size=2, align=2];
// COMPONENT_BEFORE-NEXT:         storage i32, u32 [size=4, align=4];
// COMPONENT_BEFORE-NEXT:         storage i64, u64 [size=8, align=8];
// COMPONENT_BEFORE-NEXT:         storage i128, u128 [size=16, align=16];
// COMPONENT_BEFORE-NEXT:         storage bf16 [size=2, align=2];
// COMPONENT_BEFORE-NEXT:         storage f16 [size=2, align=2];
// COMPONENT_BEFORE-NEXT:         storage f32 [size=4, align=4];
// COMPONENT_BEFORE-NEXT:         storage f64 [size=8, align=8];
// COMPONENT_BEFORE-NEXT:         storage f80 [size=16, align=16];
// COMPONENT_BEFORE-NEXT:         storage f128 [size=16, align=16];
// COMPONENT_BEFORE-NEXT:         storage d32 [size=4, align=4];
// COMPONENT_BEFORE-NEXT:         storage d64 [size=8, align=8];
// COMPONENT_BEFORE-NEXT:         storage d128 [size=16, align=16];
// COMPONENT_BEFORE-NEXT:     }
// COMPONENT_BEFORE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// COMPONENT_BEFORE-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// COMPONENT_BEFORE-NEXT:     global %[[VALUE_optimize:[0-9]+]] optimize: i32 [storage=static] = const<i32>(1) [linkage=external];
// COMPONENT_BEFORE-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// COMPONENT_BEFORE-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// COMPONENT_BEFORE-NEXT:     }
// COMPONENT_BEFORE-NEXT: }
// SLATE-FILECHECK-END COMPONENT_BEFORE
// SLATE-FILECHECK-BEGIN CONTRACT_BEFORE
// CONTRACT_BEFORE: module {
// CONTRACT_BEFORE-NEXT:     target "x86_64-unknown-linux-gnu" {
// CONTRACT_BEFORE-NEXT:         endian = little;
// CONTRACT_BEFORE-NEXT:         pointer [size=8, align=8];
// CONTRACT_BEFORE-NEXT:         stack_alignment = 16;
// CONTRACT_BEFORE-NEXT:         long_double = f80;
// CONTRACT_BEFORE-NEXT:         storage bool [size=1, align=1];
// CONTRACT_BEFORE-NEXT:         storage i8, u8 [size=1, align=1];
// CONTRACT_BEFORE-NEXT:         storage i16, u16 [size=2, align=2];
// CONTRACT_BEFORE-NEXT:         storage i32, u32 [size=4, align=4];
// CONTRACT_BEFORE-NEXT:         storage i64, u64 [size=8, align=8];
// CONTRACT_BEFORE-NEXT:         storage i128, u128 [size=16, align=16];
// CONTRACT_BEFORE-NEXT:         storage bf16 [size=2, align=2];
// CONTRACT_BEFORE-NEXT:         storage f16 [size=2, align=2];
// CONTRACT_BEFORE-NEXT:         storage f32 [size=4, align=4];
// CONTRACT_BEFORE-NEXT:         storage f64 [size=8, align=8];
// CONTRACT_BEFORE-NEXT:         storage f80 [size=16, align=16];
// CONTRACT_BEFORE-NEXT:         storage f128 [size=16, align=16];
// CONTRACT_BEFORE-NEXT:         storage d32 [size=4, align=4];
// CONTRACT_BEFORE-NEXT:         storage d64 [size=8, align=8];
// CONTRACT_BEFORE-NEXT:         storage d128 [size=16, align=16];
// CONTRACT_BEFORE-NEXT:     }
// CONTRACT_BEFORE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// CONTRACT_BEFORE-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// CONTRACT_BEFORE-NEXT:     global %[[VALUE_optimize:[0-9]+]] optimize: i32 [storage=static] = const<i32>(1) [linkage=external];
// CONTRACT_BEFORE-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// CONTRACT_BEFORE-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// CONTRACT_BEFORE-NEXT:     }
// CONTRACT_BEFORE-NEXT: }
// SLATE-FILECHECK-END CONTRACT_BEFORE
