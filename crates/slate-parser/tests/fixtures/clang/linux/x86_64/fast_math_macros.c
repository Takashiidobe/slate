#ifdef __FAST_MATH__
int fast_math = __FAST_MATH__;
#else
int fast_math = -1;
#endif

int finite_math_only = __FINITE_MATH_ONLY__;

#ifdef __NO_MATH_ERRNO__
int no_math_errno = __NO_MATH_ERRNO__;
#else
int no_math_errno = -1;
#endif

double add(double a, double b) {
    return a + b;
}

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -ffast-math
// SLATE-FILECHECK-DEFINES NOFAST
// SLATE-FILECHECK-PREFIX-ARGS NOFAST -ffast-math -fno-fast-math
// SLATE-FILECHECK-DEFINES PARTS
// SLATE-FILECHECK-PREFIX-ARGS PARTS -funsafe-math-optimizations -ffinite-math-only -fno-math-errno
// SLATE-FILECHECK-DEFINES TRAPPING
// SLATE-FILECHECK-PREFIX-ARGS TRAPPING -ffast-math -ftrapping-math
// SLATE-FILECHECK-DEFINES SNAN
// SLATE-FILECHECK-PREFIX-ARGS SNAN -fsignaling-nans -fcx-limited-range -finput-charset=UTF-8

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
// DEFAULT-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
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
// FAST-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// FAST-NEXT:     }
// FAST-NEXT: }
// SLATE-FILECHECK-END FAST
// SLATE-FILECHECK-BEGIN NOFAST
// NOFAST: module {
// NOFAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOFAST-NEXT:         endian = little;
// NOFAST-NEXT:         pointer [size=8, align=8];
// NOFAST-NEXT:         stack_alignment = 16;
// NOFAST-NEXT:         long_double = f80;
// NOFAST-NEXT:         storage bool [size=1, align=1];
// NOFAST-NEXT:         storage i8, u8 [size=1, align=1];
// NOFAST-NEXT:         storage i16, u16 [size=2, align=2];
// NOFAST-NEXT:         storage i32, u32 [size=4, align=4];
// NOFAST-NEXT:         storage i64, u64 [size=8, align=8];
// NOFAST-NEXT:         storage i128, u128 [size=16, align=16];
// NOFAST-NEXT:         storage bf16 [size=2, align=2];
// NOFAST-NEXT:         storage f16 [size=2, align=2];
// NOFAST-NEXT:         storage f32 [size=4, align=4];
// NOFAST-NEXT:         storage f64 [size=8, align=8];
// NOFAST-NEXT:         storage f80 [size=16, align=16];
// NOFAST-NEXT:         storage f128 [size=16, align=16];
// NOFAST-NEXT:         storage d32 [size=4, align=4];
// NOFAST-NEXT:         storage d64 [size=8, align=8];
// NOFAST-NEXT:         storage d128 [size=16, align=16];
// NOFAST-NEXT:     }
// NOFAST-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOFAST-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOFAST-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// NOFAST-NEXT:     }
// NOFAST-NEXT: }
// SLATE-FILECHECK-END NOFAST
// SLATE-FILECHECK-BEGIN PARTS
// PARTS: module {
// PARTS-NEXT:     target "x86_64-unknown-linux-gnu" {
// PARTS-NEXT:         endian = little;
// PARTS-NEXT:         pointer [size=8, align=8];
// PARTS-NEXT:         stack_alignment = 16;
// PARTS-NEXT:         long_double = f80;
// PARTS-NEXT:         storage bool [size=1, align=1];
// PARTS-NEXT:         storage i8, u8 [size=1, align=1];
// PARTS-NEXT:         storage i16, u16 [size=2, align=2];
// PARTS-NEXT:         storage i32, u32 [size=4, align=4];
// PARTS-NEXT:         storage i64, u64 [size=8, align=8];
// PARTS-NEXT:         storage i128, u128 [size=16, align=16];
// PARTS-NEXT:         storage bf16 [size=2, align=2];
// PARTS-NEXT:         storage f16 [size=2, align=2];
// PARTS-NEXT:         storage f32 [size=4, align=4];
// PARTS-NEXT:         storage f64 [size=8, align=8];
// PARTS-NEXT:         storage f80 [size=16, align=16];
// PARTS-NEXT:         storage f128 [size=16, align=16];
// PARTS-NEXT:         storage d32 [size=4, align=4];
// PARTS-NEXT:         storage d64 [size=8, align=8];
// PARTS-NEXT:         storage d128 [size=16, align=16];
// PARTS-NEXT:     }
// PARTS-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// PARTS-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// PARTS-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// PARTS-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// PARTS-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// PARTS-NEXT:     }
// PARTS-NEXT: }
// SLATE-FILECHECK-END PARTS
// SLATE-FILECHECK-BEGIN TRAPPING
// TRAPPING: module {
// TRAPPING-NEXT:     target "x86_64-unknown-linux-gnu" {
// TRAPPING-NEXT:         endian = little;
// TRAPPING-NEXT:         pointer [size=8, align=8];
// TRAPPING-NEXT:         stack_alignment = 16;
// TRAPPING-NEXT:         long_double = f80;
// TRAPPING-NEXT:         storage bool [size=1, align=1];
// TRAPPING-NEXT:         storage i8, u8 [size=1, align=1];
// TRAPPING-NEXT:         storage i16, u16 [size=2, align=2];
// TRAPPING-NEXT:         storage i32, u32 [size=4, align=4];
// TRAPPING-NEXT:         storage i64, u64 [size=8, align=8];
// TRAPPING-NEXT:         storage i128, u128 [size=16, align=16];
// TRAPPING-NEXT:         storage bf16 [size=2, align=2];
// TRAPPING-NEXT:         storage f16 [size=2, align=2];
// TRAPPING-NEXT:         storage f32 [size=4, align=4];
// TRAPPING-NEXT:         storage f64 [size=8, align=8];
// TRAPPING-NEXT:         storage f80 [size=16, align=16];
// TRAPPING-NEXT:         storage f128 [size=16, align=16];
// TRAPPING-NEXT:         storage d32 [size=4, align=4];
// TRAPPING-NEXT:         storage d64 [size=8, align=8];
// TRAPPING-NEXT:         storage d128 [size=16, align=16];
// TRAPPING-NEXT:     }
// TRAPPING-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// TRAPPING-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// TRAPPING-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// TRAPPING-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// TRAPPING-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// TRAPPING-NEXT:     }
// TRAPPING-NEXT: }
// SLATE-FILECHECK-END TRAPPING
// SLATE-FILECHECK-BEGIN SNAN
// SNAN: module {
// SNAN-NEXT:     target "x86_64-unknown-linux-gnu" {
// SNAN-NEXT:         endian = little;
// SNAN-NEXT:         pointer [size=8, align=8];
// SNAN-NEXT:         stack_alignment = 16;
// SNAN-NEXT:         long_double = f80;
// SNAN-NEXT:         storage bool [size=1, align=1];
// SNAN-NEXT:         storage i8, u8 [size=1, align=1];
// SNAN-NEXT:         storage i16, u16 [size=2, align=2];
// SNAN-NEXT:         storage i32, u32 [size=4, align=4];
// SNAN-NEXT:         storage i64, u64 [size=8, align=8];
// SNAN-NEXT:         storage i128, u128 [size=16, align=16];
// SNAN-NEXT:         storage bf16 [size=2, align=2];
// SNAN-NEXT:         storage f16 [size=2, align=2];
// SNAN-NEXT:         storage f32 [size=4, align=4];
// SNAN-NEXT:         storage f64 [size=8, align=8];
// SNAN-NEXT:         storage f80 [size=16, align=16];
// SNAN-NEXT:         storage f128 [size=16, align=16];
// SNAN-NEXT:         storage d32 [size=4, align=4];
// SNAN-NEXT:         storage d64 [size=8, align=8];
// SNAN-NEXT:         storage d128 [size=16, align=16];
// SNAN-NEXT:     }
// SNAN-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SNAN-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SNAN-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// SNAN-NEXT:     }
// SNAN-NEXT: }
// SLATE-FILECHECK-END SNAN
