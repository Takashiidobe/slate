// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES PRECISE
// SLATE-FILECHECK-PREFIX-ARGS PRECISE -ffp-model=precise
// SLATE-FILECHECK-DEFINES STRICT
// SLATE-FILECHECK-PREFIX-ARGS STRICT -ffp-model=strict
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -ffp-model=fast
// SLATE-FILECHECK-DEFINES AGGRESSIVE
// SLATE-FILECHECK-PREFIX-ARGS AGGRESSIVE -ffp-model=aggressive
// SLATE-FILECHECK-DEFINES RESET
// SLATE-FILECHECK-PREFIX-ARGS RESET -ffast-math -frounding-math -ffp-model=precise
// SLATE-FILECHECK-DEFINES KEPT
// SLATE-FILECHECK-PREFIX-ARGS KEPT -ffp-model=strict -ffp-model=precise
// SLATE-FILECHECK-DEFINES FAST_THEN_OFF
// SLATE-FILECHECK-PREFIX-ARGS FAST_THEN_OFF -ffp-contract=off -ffp-model=fast -fno-unsafe-math-optimizations
// SLATE-FILECHECK-DEFINES STRICT_THEN_SAFE
// SLATE-FILECHECK-PREFIX-ARGS STRICT_THEN_SAFE -ffp-model=strict -fno-unsafe-math-optimizations
// SLATE-FILECHECK-DEFINES PRECISE_THEN_FAST
// SLATE-FILECHECK-PREFIX-ARGS PRECISE_THEN_FAST -ffp-contract=off -ffp-model=precise -ffast-math -fno-fast-math
// SLATE-FILECHECK-DEFINES FINITE
// SLATE-FILECHECK-PREFIX-ARGS FINITE -ffp-model=fast -ffinite-math-only
// SLATE-FILECHECK-DEFINES ROUNDING_KEPT
// SLATE-FILECHECK-PREFIX-ARGS ROUNDING_KEPT -frounding-math -fno-fast-math

#ifdef __FAST_MATH__
int fast_math = __FAST_MATH__;
#else
int fast_math = -1;
#endif
#ifdef __NO_MATH_ERRNO__
int no_math_errno = __NO_MATH_ERRNO__;
#else
int no_math_errno = -1;
#endif
int finite_math_only = __FINITE_MATH_ONLY__;

double fused(double a, double b, double c) { return a * b + c; }

double _Complex divide(double _Complex a, double _Complex b) { return a / b; }

// SLATE-FILECHECK-BEGIN PRECISE
// PRECISE: module {
// PRECISE-NEXT:     target "x86_64-unknown-linux-gnu" {
// PRECISE-NEXT:         endian = little;
// PRECISE-NEXT:         pointer [size=8, align=8];
// PRECISE-NEXT:         stack_alignment = 16;
// PRECISE-NEXT:         long_double = f80;
// PRECISE-NEXT:         storage bool [size=1, align=1];
// PRECISE-NEXT:         storage i8, u8 [size=1, align=1];
// PRECISE-NEXT:         storage i16, u16 [size=2, align=2];
// PRECISE-NEXT:         storage i32, u32 [size=4, align=4];
// PRECISE-NEXT:         storage i64, u64 [size=8, align=8];
// PRECISE-NEXT:         storage i128, u128 [size=16, align=16];
// PRECISE-NEXT:         storage bf16 [size=2, align=2];
// PRECISE-NEXT:         storage f16 [size=2, align=2];
// PRECISE-NEXT:         storage f32 [size=4, align=4];
// PRECISE-NEXT:         storage f64 [size=8, align=8];
// PRECISE-NEXT:         storage f80 [size=16, align=16];
// PRECISE-NEXT:         storage f128 [size=16, align=16];
// PRECISE-NEXT:         storage d32 [size=4, align=4];
// PRECISE-NEXT:         storage d64 [size=8, align=8];
// PRECISE-NEXT:         storage d128 [size=16, align=16];
// PRECISE-NEXT:     }
// PRECISE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// PRECISE-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// PRECISE-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// PRECISE-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// PRECISE-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// PRECISE-NEXT:     }
// PRECISE-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// PRECISE-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// PRECISE-NEXT:     }
// PRECISE-NEXT: }
// SLATE-FILECHECK-END PRECISE
// SLATE-FILECHECK-BEGIN STRICT
// STRICT: module {
// STRICT-NEXT:     target "x86_64-unknown-linux-gnu" {
// STRICT-NEXT:         endian = little;
// STRICT-NEXT:         pointer [size=8, align=8];
// STRICT-NEXT:         stack_alignment = 16;
// STRICT-NEXT:         long_double = f80;
// STRICT-NEXT:         storage bool [size=1, align=1];
// STRICT-NEXT:         storage i8, u8 [size=1, align=1];
// STRICT-NEXT:         storage i16, u16 [size=2, align=2];
// STRICT-NEXT:         storage i32, u32 [size=4, align=4];
// STRICT-NEXT:         storage i64, u64 [size=8, align=8];
// STRICT-NEXT:         storage i128, u128 [size=16, align=16];
// STRICT-NEXT:         storage bf16 [size=2, align=2];
// STRICT-NEXT:         storage f16 [size=2, align=2];
// STRICT-NEXT:         storage f32 [size=4, align=4];
// STRICT-NEXT:         storage f64 [size=8, align=8];
// STRICT-NEXT:         storage f80 [size=16, align=16];
// STRICT-NEXT:         storage f128 [size=16, align=16];
// STRICT-NEXT:         storage d32 [size=4, align=4];
// STRICT-NEXT:         storage d64 [size=8, align=8];
// STRICT-NEXT:         storage d128 [size=16, align=16];
// STRICT-NEXT:     }
// STRICT-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// STRICT-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// STRICT-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// STRICT-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STRICT-NEXT:         return add<f64, rounding=environment, exceptions=observable, contract=off>(mul<f64, rounding=environment, exceptions=observable, contract=off>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// STRICT-NEXT:     }
// STRICT-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// STRICT-NEXT:         return div<complex<f64>, complex=true, rounding=environment, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// STRICT-NEXT:     }
// STRICT-NEXT: }
// SLATE-FILECHECK-END STRICT
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
// FAST-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// FAST-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// FAST-NEXT:     }
// FAST-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST-NEXT:         return complex_convert<complex<f64>, reason=excess_precision, rounding=nearest_even, exceptions=ignore>(div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_a_2]])), complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_b_2]]))));
// FAST-NEXT:     }
// FAST-NEXT: }
// SLATE-FILECHECK-END FAST
// SLATE-FILECHECK-BEGIN AGGRESSIVE
// AGGRESSIVE: module {
// AGGRESSIVE-NEXT:     target "x86_64-unknown-linux-gnu" {
// AGGRESSIVE-NEXT:         endian = little;
// AGGRESSIVE-NEXT:         pointer [size=8, align=8];
// AGGRESSIVE-NEXT:         stack_alignment = 16;
// AGGRESSIVE-NEXT:         long_double = f80;
// AGGRESSIVE-NEXT:         storage bool [size=1, align=1];
// AGGRESSIVE-NEXT:         storage i8, u8 [size=1, align=1];
// AGGRESSIVE-NEXT:         storage i16, u16 [size=2, align=2];
// AGGRESSIVE-NEXT:         storage i32, u32 [size=4, align=4];
// AGGRESSIVE-NEXT:         storage i64, u64 [size=8, align=8];
// AGGRESSIVE-NEXT:         storage i128, u128 [size=16, align=16];
// AGGRESSIVE-NEXT:         storage bf16 [size=2, align=2];
// AGGRESSIVE-NEXT:         storage f16 [size=2, align=2];
// AGGRESSIVE-NEXT:         storage f32 [size=4, align=4];
// AGGRESSIVE-NEXT:         storage f64 [size=8, align=8];
// AGGRESSIVE-NEXT:         storage f80 [size=16, align=16];
// AGGRESSIVE-NEXT:         storage f128 [size=16, align=16];
// AGGRESSIVE-NEXT:         storage d32 [size=4, align=4];
// AGGRESSIVE-NEXT:         storage d64 [size=8, align=8];
// AGGRESSIVE-NEXT:         storage d128 [size=16, align=16];
// AGGRESSIVE-NEXT:     }
// AGGRESSIVE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// AGGRESSIVE-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// AGGRESSIVE-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// AGGRESSIVE-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// AGGRESSIVE-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// AGGRESSIVE-NEXT:     }
// AGGRESSIVE-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// AGGRESSIVE-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// AGGRESSIVE-NEXT:     }
// AGGRESSIVE-NEXT: }
// SLATE-FILECHECK-END AGGRESSIVE
// SLATE-FILECHECK-BEGIN RESET
// RESET: module {
// RESET-NEXT:     target "x86_64-unknown-linux-gnu" {
// RESET-NEXT:         endian = little;
// RESET-NEXT:         pointer [size=8, align=8];
// RESET-NEXT:         stack_alignment = 16;
// RESET-NEXT:         long_double = f80;
// RESET-NEXT:         storage bool [size=1, align=1];
// RESET-NEXT:         storage i8, u8 [size=1, align=1];
// RESET-NEXT:         storage i16, u16 [size=2, align=2];
// RESET-NEXT:         storage i32, u32 [size=4, align=4];
// RESET-NEXT:         storage i64, u64 [size=8, align=8];
// RESET-NEXT:         storage i128, u128 [size=16, align=16];
// RESET-NEXT:         storage bf16 [size=2, align=2];
// RESET-NEXT:         storage f16 [size=2, align=2];
// RESET-NEXT:         storage f32 [size=4, align=4];
// RESET-NEXT:         storage f64 [size=8, align=8];
// RESET-NEXT:         storage f80 [size=16, align=16];
// RESET-NEXT:         storage f128 [size=16, align=16];
// RESET-NEXT:         storage d32 [size=4, align=4];
// RESET-NEXT:         storage d64 [size=8, align=8];
// RESET-NEXT:         storage d128 [size=16, align=16];
// RESET-NEXT:     }
// RESET-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// RESET-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// RESET-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// RESET-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// RESET-NEXT:         return add<f64, rounding=environment, exceptions=ignore, contract=on>(mul<f64, rounding=environment, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// RESET-NEXT:     }
// RESET-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// RESET-NEXT:         return div<complex<f64>, complex=true, rounding=environment, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// RESET-NEXT:     }
// RESET-NEXT: }
// SLATE-FILECHECK-END RESET
// SLATE-FILECHECK-BEGIN KEPT
// KEPT: module {
// KEPT-NEXT:     target "x86_64-unknown-linux-gnu" {
// KEPT-NEXT:         endian = little;
// KEPT-NEXT:         pointer [size=8, align=8];
// KEPT-NEXT:         stack_alignment = 16;
// KEPT-NEXT:         long_double = f80;
// KEPT-NEXT:         storage bool [size=1, align=1];
// KEPT-NEXT:         storage i8, u8 [size=1, align=1];
// KEPT-NEXT:         storage i16, u16 [size=2, align=2];
// KEPT-NEXT:         storage i32, u32 [size=4, align=4];
// KEPT-NEXT:         storage i64, u64 [size=8, align=8];
// KEPT-NEXT:         storage i128, u128 [size=16, align=16];
// KEPT-NEXT:         storage bf16 [size=2, align=2];
// KEPT-NEXT:         storage f16 [size=2, align=2];
// KEPT-NEXT:         storage f32 [size=4, align=4];
// KEPT-NEXT:         storage f64 [size=8, align=8];
// KEPT-NEXT:         storage f80 [size=16, align=16];
// KEPT-NEXT:         storage f128 [size=16, align=16];
// KEPT-NEXT:         storage d32 [size=4, align=4];
// KEPT-NEXT:         storage d64 [size=8, align=8];
// KEPT-NEXT:         storage d128 [size=16, align=16];
// KEPT-NEXT:     }
// KEPT-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// KEPT-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// KEPT-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// KEPT-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// KEPT-NEXT:         return add<f64, rounding=environment, exceptions=observable, contract=on>(mul<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// KEPT-NEXT:     }
// KEPT-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// KEPT-NEXT:         return div<complex<f64>, complex=true, rounding=environment, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// KEPT-NEXT:     }
// KEPT-NEXT: }
// SLATE-FILECHECK-END KEPT
// SLATE-FILECHECK-BEGIN FAST_THEN_OFF
// FAST_THEN_OFF: module {
// FAST_THEN_OFF-NEXT:     target "x86_64-unknown-linux-gnu" {
// FAST_THEN_OFF-NEXT:         endian = little;
// FAST_THEN_OFF-NEXT:         pointer [size=8, align=8];
// FAST_THEN_OFF-NEXT:         stack_alignment = 16;
// FAST_THEN_OFF-NEXT:         long_double = f80;
// FAST_THEN_OFF-NEXT:         storage bool [size=1, align=1];
// FAST_THEN_OFF-NEXT:         storage i8, u8 [size=1, align=1];
// FAST_THEN_OFF-NEXT:         storage i16, u16 [size=2, align=2];
// FAST_THEN_OFF-NEXT:         storage i32, u32 [size=4, align=4];
// FAST_THEN_OFF-NEXT:         storage i64, u64 [size=8, align=8];
// FAST_THEN_OFF-NEXT:         storage i128, u128 [size=16, align=16];
// FAST_THEN_OFF-NEXT:         storage bf16 [size=2, align=2];
// FAST_THEN_OFF-NEXT:         storage f16 [size=2, align=2];
// FAST_THEN_OFF-NEXT:         storage f32 [size=4, align=4];
// FAST_THEN_OFF-NEXT:         storage f64 [size=8, align=8];
// FAST_THEN_OFF-NEXT:         storage f80 [size=16, align=16];
// FAST_THEN_OFF-NEXT:         storage f128 [size=16, align=16];
// FAST_THEN_OFF-NEXT:         storage d32 [size=4, align=4];
// FAST_THEN_OFF-NEXT:         storage d64 [size=8, align=8];
// FAST_THEN_OFF-NEXT:         storage d128 [size=16, align=16];
// FAST_THEN_OFF-NEXT:     }
// FAST_THEN_OFF-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// FAST_THEN_OFF-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST_THEN_OFF-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST_THEN_OFF-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST_THEN_OFF-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// FAST_THEN_OFF-NEXT:     }
// FAST_THEN_OFF-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FAST_THEN_OFF-NEXT:         return complex_convert<complex<f64>, reason=excess_precision, rounding=nearest_even, exceptions=ignore>(div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_a_2]])), complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_b_2]]))));
// FAST_THEN_OFF-NEXT:     }
// FAST_THEN_OFF-NEXT: }
// SLATE-FILECHECK-END FAST_THEN_OFF
// SLATE-FILECHECK-BEGIN STRICT_THEN_SAFE
// STRICT_THEN_SAFE: module {
// STRICT_THEN_SAFE-NEXT:     target "x86_64-unknown-linux-gnu" {
// STRICT_THEN_SAFE-NEXT:         endian = little;
// STRICT_THEN_SAFE-NEXT:         pointer [size=8, align=8];
// STRICT_THEN_SAFE-NEXT:         stack_alignment = 16;
// STRICT_THEN_SAFE-NEXT:         long_double = f80;
// STRICT_THEN_SAFE-NEXT:         storage bool [size=1, align=1];
// STRICT_THEN_SAFE-NEXT:         storage i8, u8 [size=1, align=1];
// STRICT_THEN_SAFE-NEXT:         storage i16, u16 [size=2, align=2];
// STRICT_THEN_SAFE-NEXT:         storage i32, u32 [size=4, align=4];
// STRICT_THEN_SAFE-NEXT:         storage i64, u64 [size=8, align=8];
// STRICT_THEN_SAFE-NEXT:         storage i128, u128 [size=16, align=16];
// STRICT_THEN_SAFE-NEXT:         storage bf16 [size=2, align=2];
// STRICT_THEN_SAFE-NEXT:         storage f16 [size=2, align=2];
// STRICT_THEN_SAFE-NEXT:         storage f32 [size=4, align=4];
// STRICT_THEN_SAFE-NEXT:         storage f64 [size=8, align=8];
// STRICT_THEN_SAFE-NEXT:         storage f80 [size=16, align=16];
// STRICT_THEN_SAFE-NEXT:         storage f128 [size=16, align=16];
// STRICT_THEN_SAFE-NEXT:         storage d32 [size=4, align=4];
// STRICT_THEN_SAFE-NEXT:         storage d64 [size=8, align=8];
// STRICT_THEN_SAFE-NEXT:         storage d128 [size=16, align=16];
// STRICT_THEN_SAFE-NEXT:     }
// STRICT_THEN_SAFE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// STRICT_THEN_SAFE-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// STRICT_THEN_SAFE-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// STRICT_THEN_SAFE-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STRICT_THEN_SAFE-NEXT:         return add<f64, rounding=environment, exceptions=observable, contract=off>(mul<f64, rounding=environment, exceptions=observable, contract=off>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// STRICT_THEN_SAFE-NEXT:     }
// STRICT_THEN_SAFE-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// STRICT_THEN_SAFE-NEXT:         return div<complex<f64>, complex=true, rounding=environment, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// STRICT_THEN_SAFE-NEXT:     }
// STRICT_THEN_SAFE-NEXT: }
// SLATE-FILECHECK-END STRICT_THEN_SAFE
// SLATE-FILECHECK-BEGIN PRECISE_THEN_FAST
// PRECISE_THEN_FAST: module {
// PRECISE_THEN_FAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// PRECISE_THEN_FAST-NEXT:         endian = little;
// PRECISE_THEN_FAST-NEXT:         pointer [size=8, align=8];
// PRECISE_THEN_FAST-NEXT:         stack_alignment = 16;
// PRECISE_THEN_FAST-NEXT:         long_double = f80;
// PRECISE_THEN_FAST-NEXT:         storage bool [size=1, align=1];
// PRECISE_THEN_FAST-NEXT:         storage i8, u8 [size=1, align=1];
// PRECISE_THEN_FAST-NEXT:         storage i16, u16 [size=2, align=2];
// PRECISE_THEN_FAST-NEXT:         storage i32, u32 [size=4, align=4];
// PRECISE_THEN_FAST-NEXT:         storage i64, u64 [size=8, align=8];
// PRECISE_THEN_FAST-NEXT:         storage i128, u128 [size=16, align=16];
// PRECISE_THEN_FAST-NEXT:         storage bf16 [size=2, align=2];
// PRECISE_THEN_FAST-NEXT:         storage f16 [size=2, align=2];
// PRECISE_THEN_FAST-NEXT:         storage f32 [size=4, align=4];
// PRECISE_THEN_FAST-NEXT:         storage f64 [size=8, align=8];
// PRECISE_THEN_FAST-NEXT:         storage f80 [size=16, align=16];
// PRECISE_THEN_FAST-NEXT:         storage f128 [size=16, align=16];
// PRECISE_THEN_FAST-NEXT:         storage d32 [size=4, align=4];
// PRECISE_THEN_FAST-NEXT:         storage d64 [size=8, align=8];
// PRECISE_THEN_FAST-NEXT:         storage d128 [size=16, align=16];
// PRECISE_THEN_FAST-NEXT:     }
// PRECISE_THEN_FAST-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// PRECISE_THEN_FAST-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// PRECISE_THEN_FAST-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// PRECISE_THEN_FAST-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// PRECISE_THEN_FAST-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// PRECISE_THEN_FAST-NEXT:     }
// PRECISE_THEN_FAST-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// PRECISE_THEN_FAST-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// PRECISE_THEN_FAST-NEXT:     }
// PRECISE_THEN_FAST-NEXT: }
// SLATE-FILECHECK-END PRECISE_THEN_FAST
// SLATE-FILECHECK-BEGIN FINITE
// FINITE: module {
// FINITE-NEXT:     target "x86_64-unknown-linux-gnu" {
// FINITE-NEXT:         endian = little;
// FINITE-NEXT:         pointer [size=8, align=8];
// FINITE-NEXT:         stack_alignment = 16;
// FINITE-NEXT:         long_double = f80;
// FINITE-NEXT:         storage bool [size=1, align=1];
// FINITE-NEXT:         storage i8, u8 [size=1, align=1];
// FINITE-NEXT:         storage i16, u16 [size=2, align=2];
// FINITE-NEXT:         storage i32, u32 [size=4, align=4];
// FINITE-NEXT:         storage i64, u64 [size=8, align=8];
// FINITE-NEXT:         storage i128, u128 [size=16, align=16];
// FINITE-NEXT:         storage bf16 [size=2, align=2];
// FINITE-NEXT:         storage f16 [size=2, align=2];
// FINITE-NEXT:         storage f32 [size=4, align=4];
// FINITE-NEXT:         storage f64 [size=8, align=8];
// FINITE-NEXT:         storage f80 [size=16, align=16];
// FINITE-NEXT:         storage f128 [size=16, align=16];
// FINITE-NEXT:         storage d32 [size=4, align=4];
// FINITE-NEXT:         storage d64 [size=8, align=8];
// FINITE-NEXT:         storage d128 [size=16, align=16];
// FINITE-NEXT:     }
// FINITE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// FINITE-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// FINITE-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// FINITE-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FINITE-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// FINITE-NEXT:     }
// FINITE-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// FINITE-NEXT:         return complex_convert<complex<f64>, reason=excess_precision, rounding=nearest_even, exceptions=ignore>(div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_a_2]])), complex_convert<complex<f80>, reason=excess_precision>(read<complex<f64>>(%[[VALUE_b_2]]))));
// FINITE-NEXT:     }
// FINITE-NEXT: }
// SLATE-FILECHECK-END FINITE
// SLATE-FILECHECK-BEGIN ROUNDING_KEPT
// ROUNDING_KEPT: module {
// ROUNDING_KEPT-NEXT:     target "x86_64-unknown-linux-gnu" {
// ROUNDING_KEPT-NEXT:         endian = little;
// ROUNDING_KEPT-NEXT:         pointer [size=8, align=8];
// ROUNDING_KEPT-NEXT:         stack_alignment = 16;
// ROUNDING_KEPT-NEXT:         long_double = f80;
// ROUNDING_KEPT-NEXT:         storage bool [size=1, align=1];
// ROUNDING_KEPT-NEXT:         storage i8, u8 [size=1, align=1];
// ROUNDING_KEPT-NEXT:         storage i16, u16 [size=2, align=2];
// ROUNDING_KEPT-NEXT:         storage i32, u32 [size=4, align=4];
// ROUNDING_KEPT-NEXT:         storage i64, u64 [size=8, align=8];
// ROUNDING_KEPT-NEXT:         storage i128, u128 [size=16, align=16];
// ROUNDING_KEPT-NEXT:         storage bf16 [size=2, align=2];
// ROUNDING_KEPT-NEXT:         storage f16 [size=2, align=2];
// ROUNDING_KEPT-NEXT:         storage f32 [size=4, align=4];
// ROUNDING_KEPT-NEXT:         storage f64 [size=8, align=8];
// ROUNDING_KEPT-NEXT:         storage f80 [size=16, align=16];
// ROUNDING_KEPT-NEXT:         storage f128 [size=16, align=16];
// ROUNDING_KEPT-NEXT:         storage d32 [size=4, align=4];
// ROUNDING_KEPT-NEXT:         storage d64 [size=8, align=8];
// ROUNDING_KEPT-NEXT:         storage d128 [size=16, align=16];
// ROUNDING_KEPT-NEXT:     }
// ROUNDING_KEPT-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// ROUNDING_KEPT-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32, overflow=ub>(const<i32>(1)) [linkage=external];
// ROUNDING_KEPT-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// ROUNDING_KEPT-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// ROUNDING_KEPT-NEXT:         return add<f64, rounding=environment, exceptions=ignore, contract=on>(mul<f64, rounding=environment, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// ROUNDING_KEPT-NEXT:     }
// ROUNDING_KEPT-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a_2:[0-9]+]] a: complex<f64>, %[[VALUE_b_2:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// ROUNDING_KEPT-NEXT:         return div<complex<f64>, complex=true, rounding=environment, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_2]]), read<complex<f64>>(%[[VALUE_b_2]]));
// ROUNDING_KEPT-NEXT:     }
// ROUNDING_KEPT-NEXT: }
// SLATE-FILECHECK-END ROUNDING_KEPT
