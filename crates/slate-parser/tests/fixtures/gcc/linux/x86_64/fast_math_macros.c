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

#ifdef __ASSOCIATIVE_MATH__
int associative_math = __ASSOCIATIVE_MATH__;
#else
int associative_math = -1;
#endif

#ifdef __RECIPROCAL_MATH__
int reciprocal_math = __RECIPROCAL_MATH__;
#else
int reciprocal_math = -1;
#endif

#ifdef __NO_SIGNED_ZEROS__
int no_signed_zeros = __NO_SIGNED_ZEROS__;
#else
int no_signed_zeros = -1;
#endif

#ifdef __NO_TRAPPING_MATH__
int no_trapping_math = __NO_TRAPPING_MATH__;
#else
int no_trapping_math = -1;
#endif

#ifdef __SUPPORT_SNAN__
int support_snan = __SUPPORT_SNAN__;
#else
int support_snan = -1;
#endif

int iec_559 = __GCC_IEC_559;
int iec_559_complex = __GCC_IEC_559_COMPLEX;

#ifdef __STDC_IEC_559__
int stdc_iec_559 = __STDC_IEC_559__;
#else
int stdc_iec_559 = -1;
#endif

#ifdef __STDC_IEC_559_COMPLEX__
int stdc_iec_559_complex = __STDC_IEC_559_COMPLEX__;
#else
int stdc_iec_559_complex = -1;
#endif

double add(double a, double b) {
    return a + b;
}

#pragma GCC push_options
#pragma GCC optimize ("no-math-errno", "-fno-signed-zeros")
#ifdef __NO_MATH_ERRNO__
int pragma_no_math_errno = __NO_MATH_ERRNO__;
#else
int pragma_no_math_errno = -1;
#endif
#ifdef __NO_SIGNED_ZEROS__
int pragma_no_signed_zeros = __NO_SIGNED_ZEROS__;
#else
int pragma_no_signed_zeros = -1;
#endif
#pragma GCC pop_options
#ifdef __NO_SIGNED_ZEROS__
int popped_no_signed_zeros = __NO_SIGNED_ZEROS__;
#else
int popped_no_signed_zeros = -1;
#endif

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -ffast-math
// SLATE-FILECHECK-DEFINES NOFAST
// SLATE-FILECHECK-PREFIX-ARGS NOFAST -ffast-math -fno-fast-math
// SLATE-FILECHECK-DEFINES UNSAFE
// SLATE-FILECHECK-PREFIX-ARGS UNSAFE -funsafe-math-optimizations
// SLATE-FILECHECK-DEFINES ASSOC
// SLATE-FILECHECK-PREFIX-ARGS ASSOC -fassociative-math
// SLATE-FILECHECK-DEFINES SNAN
// SLATE-FILECHECK-PREFIX-ARGS SNAN -ffast-math -fsignaling-nans
// SLATE-FILECHECK-DEFINES STANDARD
// SLATE-FILECHECK-PREFIX-ARGS STANDARD -ffast-math -fexcess-precision=standard
// SLATE-FILECHECK-DEFINES ISO
// SLATE-FILECHECK-PREFIX-ARGS ISO -std=c11 -funsafe-math-optimizations -ffinite-math-only -fno-math-errno
// SLATE-FILECHECK-DEFINES COMPLEX
// SLATE-FILECHECK-PREFIX-ARGS COMPLEX -fcx-limited-range
// SLATE-FILECHECK-DEFINES CHARSET
// SLATE-FILECHECK-PREFIX-ARGS CHARSET -finput-charset=utf-8 -fpermitted-flt-eval-methods=c11

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
// DEFAULT-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
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
// FAST-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FAST-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// FAST-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FAST-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// FAST-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// FAST-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
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
// NOFAST-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(2) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// NOFAST-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// NOFAST-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOFAST-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// NOFAST-NEXT:     }
// NOFAST-NEXT: }
// SLATE-FILECHECK-END NOFAST
// SLATE-FILECHECK-BEGIN UNSAFE
// UNSAFE: module {
// UNSAFE-NEXT:     target "x86_64-unknown-linux-gnu" {
// UNSAFE-NEXT:         endian = little;
// UNSAFE-NEXT:         pointer [size=8, align=8];
// UNSAFE-NEXT:         stack_alignment = 16;
// UNSAFE-NEXT:         long_double = f80;
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
// UNSAFE-NEXT:         storage f80 [size=16, align=16];
// UNSAFE-NEXT:         storage f128 [size=16, align=16];
// UNSAFE-NEXT:         storage d32 [size=4, align=4];
// UNSAFE-NEXT:         storage d64 [size=8, align=8];
// UNSAFE-NEXT:         storage d128 [size=16, align=16];
// UNSAFE-NEXT:     }
// UNSAFE-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// UNSAFE-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// UNSAFE-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// UNSAFE-NEXT:     }
// UNSAFE-NEXT: }
// SLATE-FILECHECK-END UNSAFE
// SLATE-FILECHECK-BEGIN ASSOC
// ASSOC: module {
// ASSOC-NEXT:     target "x86_64-unknown-linux-gnu" {
// ASSOC-NEXT:         endian = little;
// ASSOC-NEXT:         pointer [size=8, align=8];
// ASSOC-NEXT:         stack_alignment = 16;
// ASSOC-NEXT:         long_double = f80;
// ASSOC-NEXT:         storage bool [size=1, align=1];
// ASSOC-NEXT:         storage i8, u8 [size=1, align=1];
// ASSOC-NEXT:         storage i16, u16 [size=2, align=2];
// ASSOC-NEXT:         storage i32, u32 [size=4, align=4];
// ASSOC-NEXT:         storage i64, u64 [size=8, align=8];
// ASSOC-NEXT:         storage i128, u128 [size=16, align=16];
// ASSOC-NEXT:         storage bf16 [size=2, align=2];
// ASSOC-NEXT:         storage f16 [size=2, align=2];
// ASSOC-NEXT:         storage f32 [size=4, align=4];
// ASSOC-NEXT:         storage f64 [size=8, align=8];
// ASSOC-NEXT:         storage f80 [size=16, align=16];
// ASSOC-NEXT:         storage f128 [size=16, align=16];
// ASSOC-NEXT:         storage d32 [size=4, align=4];
// ASSOC-NEXT:         storage d64 [size=8, align=8];
// ASSOC-NEXT:         storage d128 [size=16, align=16];
// ASSOC-NEXT:     }
// ASSOC-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(2) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(2) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = const<i32>(1) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// ASSOC-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ASSOC-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// ASSOC-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// ASSOC-NEXT:     }
// ASSOC-NEXT: }
// SLATE-FILECHECK-END ASSOC
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
// SNAN-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = const<i32>(1) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// SNAN-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// SNAN-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SNAN-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// SNAN-NEXT:     }
// SNAN-NEXT: }
// SLATE-FILECHECK-END SNAN
// SLATE-FILECHECK-BEGIN STANDARD
// STANDARD: module {
// STANDARD-NEXT:     target "x86_64-unknown-linux-gnu" {
// STANDARD-NEXT:         endian = little;
// STANDARD-NEXT:         pointer [size=8, align=8];
// STANDARD-NEXT:         stack_alignment = 16;
// STANDARD-NEXT:         long_double = f80;
// STANDARD-NEXT:         storage bool [size=1, align=1];
// STANDARD-NEXT:         storage i8, u8 [size=1, align=1];
// STANDARD-NEXT:         storage i16, u16 [size=2, align=2];
// STANDARD-NEXT:         storage i32, u32 [size=4, align=4];
// STANDARD-NEXT:         storage i64, u64 [size=8, align=8];
// STANDARD-NEXT:         storage i128, u128 [size=16, align=16];
// STANDARD-NEXT:         storage bf16 [size=2, align=2];
// STANDARD-NEXT:         storage f16 [size=2, align=2];
// STANDARD-NEXT:         storage f32 [size=4, align=4];
// STANDARD-NEXT:         storage f64 [size=8, align=8];
// STANDARD-NEXT:         storage f80 [size=16, align=16];
// STANDARD-NEXT:         storage f128 [size=16, align=16];
// STANDARD-NEXT:         storage d32 [size=4, align=4];
// STANDARD-NEXT:         storage d64 [size=8, align=8];
// STANDARD-NEXT:         storage d128 [size=16, align=16];
// STANDARD-NEXT:     }
// STANDARD-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// STANDARD-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STANDARD-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// STANDARD-NEXT:     }
// STANDARD-NEXT: }
// SLATE-FILECHECK-END STANDARD
// SLATE-FILECHECK-BEGIN ISO
// ISO: module {
// ISO-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO-NEXT:         endian = little;
// ISO-NEXT:         pointer [size=8, align=8];
// ISO-NEXT:         stack_alignment = 16;
// ISO-NEXT:         long_double = f80;
// ISO-NEXT:         storage bool [size=1, align=1];
// ISO-NEXT:         storage i8, u8 [size=1, align=1];
// ISO-NEXT:         storage i16, u16 [size=2, align=2];
// ISO-NEXT:         storage i32, u32 [size=4, align=4];
// ISO-NEXT:         storage i64, u64 [size=8, align=8];
// ISO-NEXT:         storage i128, u128 [size=16, align=16];
// ISO-NEXT:         storage bf16 [size=2, align=2];
// ISO-NEXT:         storage f16 [size=2, align=2];
// ISO-NEXT:         storage f32 [size=4, align=4];
// ISO-NEXT:         storage f64 [size=8, align=8];
// ISO-NEXT:         storage f80 [size=16, align=16];
// ISO-NEXT:         storage f128 [size=16, align=16];
// ISO-NEXT:         storage d32 [size=4, align=4];
// ISO-NEXT:         storage d64 [size=8, align=8];
// ISO-NEXT:         storage d128 [size=16, align=16];
// ISO-NEXT:     }
// ISO-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ISO-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ISO-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(0) [linkage=external];
// ISO-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// ISO-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ISO-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// ISO-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// ISO-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// ISO-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// ISO-NEXT:     }
// ISO-NEXT: }
// SLATE-FILECHECK-END ISO
// SLATE-FILECHECK-BEGIN COMPLEX
// COMPLEX: module {
// COMPLEX-NEXT:     target "x86_64-unknown-linux-gnu" {
// COMPLEX-NEXT:         endian = little;
// COMPLEX-NEXT:         pointer [size=8, align=8];
// COMPLEX-NEXT:         stack_alignment = 16;
// COMPLEX-NEXT:         long_double = f80;
// COMPLEX-NEXT:         storage bool [size=1, align=1];
// COMPLEX-NEXT:         storage i8, u8 [size=1, align=1];
// COMPLEX-NEXT:         storage i16, u16 [size=2, align=2];
// COMPLEX-NEXT:         storage i32, u32 [size=4, align=4];
// COMPLEX-NEXT:         storage i64, u64 [size=8, align=8];
// COMPLEX-NEXT:         storage i128, u128 [size=16, align=16];
// COMPLEX-NEXT:         storage bf16 [size=2, align=2];
// COMPLEX-NEXT:         storage f16 [size=2, align=2];
// COMPLEX-NEXT:         storage f32 [size=4, align=4];
// COMPLEX-NEXT:         storage f64 [size=8, align=8];
// COMPLEX-NEXT:         storage f80 [size=16, align=16];
// COMPLEX-NEXT:         storage f128 [size=16, align=16];
// COMPLEX-NEXT:         storage d32 [size=4, align=4];
// COMPLEX-NEXT:         storage d64 [size=8, align=8];
// COMPLEX-NEXT:         storage d128 [size=16, align=16];
// COMPLEX-NEXT:     }
// COMPLEX-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(2) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(0) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = const<i32>(1) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// COMPLEX-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// COMPLEX-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// COMPLEX-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// COMPLEX-NEXT:     }
// COMPLEX-NEXT: }
// SLATE-FILECHECK-END COMPLEX
// SLATE-FILECHECK-BEGIN CHARSET
// CHARSET: module {
// CHARSET-NEXT:     target "x86_64-unknown-linux-gnu" {
// CHARSET-NEXT:         endian = little;
// CHARSET-NEXT:         pointer [size=8, align=8];
// CHARSET-NEXT:         stack_alignment = 16;
// CHARSET-NEXT:         long_double = f80;
// CHARSET-NEXT:         storage bool [size=1, align=1];
// CHARSET-NEXT:         storage i8, u8 [size=1, align=1];
// CHARSET-NEXT:         storage i16, u16 [size=2, align=2];
// CHARSET-NEXT:         storage i32, u32 [size=4, align=4];
// CHARSET-NEXT:         storage i64, u64 [size=8, align=8];
// CHARSET-NEXT:         storage i128, u128 [size=16, align=16];
// CHARSET-NEXT:         storage bf16 [size=2, align=2];
// CHARSET-NEXT:         storage f16 [size=2, align=2];
// CHARSET-NEXT:         storage f32 [size=4, align=4];
// CHARSET-NEXT:         storage f64 [size=8, align=8];
// CHARSET-NEXT:         storage f80 [size=16, align=16];
// CHARSET-NEXT:         storage f128 [size=16, align=16];
// CHARSET-NEXT:         storage d32 [size=4, align=4];
// CHARSET-NEXT:         storage d64 [size=8, align=8];
// CHARSET-NEXT:         storage d128 [size=16, align=16];
// CHARSET-NEXT:     }
// CHARSET-NEXT:     global %[[VALUE_fast_math:[0-9]+]] fast_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_finite_math_only:[0-9]+]] finite_math_only: i32 [storage=static] = const<i32>(0) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_no_math_errno:[0-9]+]] no_math_errno: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_associative_math:[0-9]+]] associative_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_reciprocal_math:[0-9]+]] reciprocal_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_no_signed_zeros:[0-9]+]] no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_no_trapping_math:[0-9]+]] no_trapping_math: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_support_snan:[0-9]+]] support_snan: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_iec_559:[0-9]+]] iec_559: i32 [storage=static] = const<i32>(2) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_iec_559_complex:[0-9]+]] iec_559_complex: i32 [storage=static] = const<i32>(2) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_stdc_iec_559:[0-9]+]] stdc_iec_559: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_stdc_iec_559_complex:[0-9]+]] stdc_iec_559_complex: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_pragma_no_math_errno:[0-9]+]] pragma_no_math_errno: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_pragma_no_signed_zeros:[0-9]+]] pragma_no_signed_zeros: i32 [storage=static] = const<i32>(1) [linkage=external];
// CHARSET-NEXT:     global %[[VALUE_popped_no_signed_zeros:[0-9]+]] popped_no_signed_zeros: i32 [storage=static] = neg<i32>(const<i32>(1)) [linkage=external];
// CHARSET-NEXT:     fn %[[VALUE_add:[0-9]+]] @add(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// CHARSET-NEXT:         return add<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// CHARSET-NEXT:     }
// CHARSET-NEXT: }
// SLATE-FILECHECK-END CHARSET
