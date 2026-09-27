// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES V9
// SLATE-FILECHECK-PREFIX-ARGS V9 --flavor=gcc -march=armv9-a
// SLATE-FILECHECK-DEFINES V9-STRICT
// SLATE-FILECHECK-PREFIX-ARGS V9-STRICT --flavor=gcc -march=armv9-a -std=c11
// SLATE-FILECHECK-DEFINES NOFP
// SLATE-FILECHECK-PREFIX-ARGS NOFP --flavor=gcc -march=armv8.6-a+nofp
// SLATE-FILECHECK-DEFINES NOSIMD-SIMD
// SLATE-FILECHECK-PREFIX-ARGS NOSIMD-SIMD --flavor=gcc -march=armv9.5-a+nosimd+simd
// SLATE-FILECHECK-DEFINES SVE-FIXED
// SLATE-FILECHECK-PREFIX-ARGS SVE-FIXED --flavor=gcc -march=armv8-a+sve -msve-vector-bits=256
// SLATE-FILECHECK-DEFINES CRYPTO
// SLATE-FILECHECK-PREFIX-ARGS CRYPTO --flavor=gcc -march=armv8.4-a+crypto

#ifdef __ARM_BF16_FORMAT_ALTERNATIVE
int val__ARM_BF16_FORMAT_ALTERNATIVE[__ARM_BF16_FORMAT_ALTERNATIVE + 1];
#endif
#ifdef __ARM_FEATURE_BF16
int val__ARM_FEATURE_BF16[__ARM_FEATURE_BF16 + 1];
#endif
#ifdef __ARM_FEATURE_BF16_VECTOR_ARITHMETIC
int val__ARM_FEATURE_BF16_VECTOR_ARITHMETIC[__ARM_FEATURE_BF16_VECTOR_ARITHMETIC + 1];
#endif
#ifdef __ARM_FEATURE_COMPLEX
int val__ARM_FEATURE_COMPLEX[__ARM_FEATURE_COMPLEX + 1];
#endif
#ifdef __ARM_FEATURE_CSSC
int val__ARM_FEATURE_CSSC[__ARM_FEATURE_CSSC + 1];
#endif
#ifdef __ARM_FEATURE_FAMINMAX
int val__ARM_FEATURE_FAMINMAX[__ARM_FEATURE_FAMINMAX + 1];
#endif
#ifdef __ARM_FEATURE_FMA
int val__ARM_FEATURE_FMA[__ARM_FEATURE_FMA + 1];
#endif
#ifdef __ARM_FEATURE_FP16_FML
int val__ARM_FEATURE_FP16_FML[__ARM_FEATURE_FP16_FML + 1];
#endif
#ifdef __ARM_FEATURE_FRINT
int val__ARM_FEATURE_FRINT[__ARM_FEATURE_FRINT + 1];
#endif
#ifdef __ARM_FEATURE_JCVT
int val__ARM_FEATURE_JCVT[__ARM_FEATURE_JCVT + 1];
#endif
#ifdef __ARM_FEATURE_LUT
int val__ARM_FEATURE_LUT[__ARM_FEATURE_LUT + 1];
#endif
#ifdef __ARM_FEATURE_MOPS
int val__ARM_FEATURE_MOPS[__ARM_FEATURE_MOPS + 1];
#endif
#ifdef __ARM_FEATURE_NUMERIC_MAXMIN
int val__ARM_FEATURE_NUMERIC_MAXMIN[__ARM_FEATURE_NUMERIC_MAXMIN + 1];
#endif
#ifdef __ARM_FEATURE_QRDMX
int val__ARM_FEATURE_QRDMX[__ARM_FEATURE_QRDMX + 1];
#endif
#ifdef __ARM_FEATURE_SHA3
int val__ARM_FEATURE_SHA3[__ARM_FEATURE_SHA3 + 1];
#endif
#ifdef __ARM_FEATURE_SM4
int val__ARM_FEATURE_SM4[__ARM_FEATURE_SM4 + 1];
#endif
#ifdef __ARM_FEATURE_SVE
int val__ARM_FEATURE_SVE[__ARM_FEATURE_SVE + 1];
#endif
#ifdef __ARM_FEATURE_SVE2
int val__ARM_FEATURE_SVE2[__ARM_FEATURE_SVE2 + 1];
#endif
#ifdef __ARM_FEATURE_SVE_BITS
int val__ARM_FEATURE_SVE_BITS[__ARM_FEATURE_SVE_BITS + 1];
#endif
#ifdef __ARM_FEATURE_SVE_PREDICATE_OPERATORS
int val__ARM_FEATURE_SVE_PREDICATE_OPERATORS[__ARM_FEATURE_SVE_PREDICATE_OPERATORS + 1];
#endif
#ifdef __ARM_FEATURE_SVE_VECTOR_OPERATORS
int val__ARM_FEATURE_SVE_VECTOR_OPERATORS[__ARM_FEATURE_SVE_VECTOR_OPERATORS + 1];
#endif
#ifdef __ARM_FP16_FORMAT_IEEE
int val__ARM_FP16_FORMAT_IEEE[__ARM_FP16_FORMAT_IEEE + 1];
#endif
#ifdef __FLT_EVAL_METHOD__
int val__FLT_EVAL_METHOD__[__FLT_EVAL_METHOD__ + 1];
#endif
#ifdef __FLT_EVAL_METHOD_C99__
int val__FLT_EVAL_METHOD_C99__[__FLT_EVAL_METHOD_C99__ + 1];
#endif
#ifdef __FP_FAST_FMA
int val__FP_FAST_FMA[__FP_FAST_FMA + 1];
#endif
#ifdef __GCC_DESTRUCTIVE_SIZE
int val__GCC_DESTRUCTIVE_SIZE[__GCC_DESTRUCTIVE_SIZE + 1];
#endif
#ifdef __GCC_IEC_559
int val__GCC_IEC_559[__GCC_IEC_559 + 1];
#endif
#ifdef __STDC_IEC_559__
int val__STDC_IEC_559__[__STDC_IEC_559__ + 1];
#endif

// SLATE-FILECHECK-BEGIN V9
// V9: module {
// V9-NEXT:     target "aarch64-unknown-linux-gnu" {
// V9-NEXT:         endian = little;
// V9-NEXT:         pointer [size=8, align=8];
// V9-NEXT:         stack_alignment = 16;
// V9-NEXT:         long_double = f128;
// V9-NEXT:         storage bool [size=1, align=1];
// V9-NEXT:         storage i8, u8 [size=1, align=1];
// V9-NEXT:         storage i16, u16 [size=2, align=2];
// V9-NEXT:         storage i32, u32 [size=4, align=4];
// V9-NEXT:         storage i64, u64 [size=8, align=8];
// V9-NEXT:         storage i128, u128 [size=16, align=16];
// V9-NEXT:         storage bf16 [size=2, align=2];
// V9-NEXT:         storage f16 [size=2, align=2];
// V9-NEXT:         storage f32 [size=4, align=4];
// V9-NEXT:         storage f64 [size=8, align=8];
// V9-NEXT:         storage f128 [size=16, align=16];
// V9-NEXT:         storage d32 [size=4, align=4];
// V9-NEXT:         storage d64 [size=8, align=8];
// V9-NEXT:         storage d128 [size=16, align=16];
// V9-NEXT:     }
// V9-NEXT:     global %0 val__ARM_FEATURE_COMPLEX: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %1 val__ARM_FEATURE_FMA: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %2 val__ARM_FEATURE_FP16_FML: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %3 val__ARM_FEATURE_FRINT: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %4 val__ARM_FEATURE_JCVT: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %5 val__ARM_FEATURE_NUMERIC_MAXMIN: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %6 val__ARM_FEATURE_QRDMX: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %7 val__ARM_FEATURE_SVE: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %8 val__ARM_FEATURE_SVE2: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %9 val__ARM_FEATURE_SVE_BITS: array<i32, 1> [storage=static] [linkage=external];
// V9-NEXT:     global %10 val__ARM_FEATURE_SVE_PREDICATE_OPERATORS: array<i32, 3> [storage=static] [linkage=external];
// V9-NEXT:     global %11 val__ARM_FEATURE_SVE_VECTOR_OPERATORS: array<i32, 3> [storage=static] [linkage=external];
// V9-NEXT:     global %12 val__ARM_FP16_FORMAT_IEEE: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %13 val__FLT_EVAL_METHOD__: array<i32, 17> [storage=static] [linkage=external];
// V9-NEXT:     global %14 val__FLT_EVAL_METHOD_C99__: array<i32, 17> [storage=static] [linkage=external];
// V9-NEXT:     global %15 val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT:     global %16 val__GCC_DESTRUCTIVE_SIZE: array<i32, 65> [storage=static] [linkage=external];
// V9-NEXT:     global %17 val__GCC_IEC_559: array<i32, 3> [storage=static] [linkage=external];
// V9-NEXT:     global %18 val__STDC_IEC_559__: array<i32, 2> [storage=static] [linkage=external];
// V9-NEXT: }
// SLATE-FILECHECK-END V9
// SLATE-FILECHECK-BEGIN V9-STRICT
// V9-STRICT: module {
// V9-STRICT-NEXT:     target "aarch64-unknown-linux-gnu" {
// V9-STRICT-NEXT:         endian = little;
// V9-STRICT-NEXT:         pointer [size=8, align=8];
// V9-STRICT-NEXT:         stack_alignment = 16;
// V9-STRICT-NEXT:         long_double = f128;
// V9-STRICT-NEXT:         storage bool [size=1, align=1];
// V9-STRICT-NEXT:         storage i8, u8 [size=1, align=1];
// V9-STRICT-NEXT:         storage i16, u16 [size=2, align=2];
// V9-STRICT-NEXT:         storage i32, u32 [size=4, align=4];
// V9-STRICT-NEXT:         storage i64, u64 [size=8, align=8];
// V9-STRICT-NEXT:         storage i128, u128 [size=16, align=16];
// V9-STRICT-NEXT:         storage bf16 [size=2, align=2];
// V9-STRICT-NEXT:         storage f16 [size=2, align=2];
// V9-STRICT-NEXT:         storage f32 [size=4, align=4];
// V9-STRICT-NEXT:         storage f64 [size=8, align=8];
// V9-STRICT-NEXT:         storage f128 [size=16, align=16];
// V9-STRICT-NEXT:         storage d32 [size=4, align=4];
// V9-STRICT-NEXT:         storage d64 [size=8, align=8];
// V9-STRICT-NEXT:         storage d128 [size=16, align=16];
// V9-STRICT-NEXT:     }
// V9-STRICT-NEXT:     global %0 val__ARM_FEATURE_COMPLEX: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %1 val__ARM_FEATURE_FMA: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %2 val__ARM_FEATURE_FP16_FML: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %3 val__ARM_FEATURE_FRINT: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %4 val__ARM_FEATURE_JCVT: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %5 val__ARM_FEATURE_NUMERIC_MAXMIN: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %6 val__ARM_FEATURE_QRDMX: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %7 val__ARM_FEATURE_SVE: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %8 val__ARM_FEATURE_SVE2: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %9 val__ARM_FEATURE_SVE_BITS: array<i32, 1> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %10 val__ARM_FEATURE_SVE_PREDICATE_OPERATORS: array<i32, 3> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %11 val__ARM_FEATURE_SVE_VECTOR_OPERATORS: array<i32, 3> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %12 val__ARM_FP16_FORMAT_IEEE: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %13 val__FLT_EVAL_METHOD__: array<i32, 1> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %14 val__FLT_EVAL_METHOD_C99__: array<i32, 17> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %15 val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %16 val__GCC_DESTRUCTIVE_SIZE: array<i32, 65> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %17 val__GCC_IEC_559: array<i32, 3> [storage=static] [linkage=external];
// V9-STRICT-NEXT:     global %18 val__STDC_IEC_559__: array<i32, 2> [storage=static] [linkage=external];
// V9-STRICT-NEXT: }
// SLATE-FILECHECK-END V9-STRICT
// SLATE-FILECHECK-BEGIN NOFP
// NOFP: module {
// NOFP-NEXT:     target "aarch64-unknown-linux-gnu" {
// NOFP-NEXT:         endian = little;
// NOFP-NEXT:         pointer [size=8, align=8];
// NOFP-NEXT:         stack_alignment = 16;
// NOFP-NEXT:         long_double = f128;
// NOFP-NEXT:         storage bool [size=1, align=1];
// NOFP-NEXT:         storage i8, u8 [size=1, align=1];
// NOFP-NEXT:         storage i16, u16 [size=2, align=2];
// NOFP-NEXT:         storage i32, u32 [size=4, align=4];
// NOFP-NEXT:         storage i64, u64 [size=8, align=8];
// NOFP-NEXT:         storage i128, u128 [size=16, align=16];
// NOFP-NEXT:         storage bf16 [size=2, align=2];
// NOFP-NEXT:         storage f16 [size=2, align=2];
// NOFP-NEXT:         storage f32 [size=4, align=4];
// NOFP-NEXT:         storage f64 [size=8, align=8];
// NOFP-NEXT:         storage f128 [size=16, align=16];
// NOFP-NEXT:         storage d32 [size=4, align=4];
// NOFP-NEXT:         storage d64 [size=8, align=8];
// NOFP-NEXT:         storage d128 [size=16, align=16];
// NOFP-NEXT:     }
// NOFP-NEXT:     global %0 val__FLT_EVAL_METHOD__: array<i32, 1> [storage=static] [linkage=external];
// NOFP-NEXT:     global %1 val__FLT_EVAL_METHOD_C99__: array<i32, 1> [storage=static] [linkage=external];
// NOFP-NEXT:     global %2 val__GCC_DESTRUCTIVE_SIZE: array<i32, 257> [storage=static] [linkage=external];
// NOFP-NEXT:     global %3 val__GCC_IEC_559: array<i32, 1> [storage=static] [linkage=external];
// NOFP-NEXT: }
// SLATE-FILECHECK-END NOFP
// SLATE-FILECHECK-BEGIN NOSIMD-SIMD
// NOSIMD-SIMD: module {
// NOSIMD-SIMD-NEXT:     target "aarch64-unknown-linux-gnu" {
// NOSIMD-SIMD-NEXT:         endian = little;
// NOSIMD-SIMD-NEXT:         pointer [size=8, align=8];
// NOSIMD-SIMD-NEXT:         stack_alignment = 16;
// NOSIMD-SIMD-NEXT:         long_double = f128;
// NOSIMD-SIMD-NEXT:         storage bool [size=1, align=1];
// NOSIMD-SIMD-NEXT:         storage i8, u8 [size=1, align=1];
// NOSIMD-SIMD-NEXT:         storage i16, u16 [size=2, align=2];
// NOSIMD-SIMD-NEXT:         storage i32, u32 [size=4, align=4];
// NOSIMD-SIMD-NEXT:         storage i64, u64 [size=8, align=8];
// NOSIMD-SIMD-NEXT:         storage i128, u128 [size=16, align=16];
// NOSIMD-SIMD-NEXT:         storage bf16 [size=2, align=2];
// NOSIMD-SIMD-NEXT:         storage f16 [size=2, align=2];
// NOSIMD-SIMD-NEXT:         storage f32 [size=4, align=4];
// NOSIMD-SIMD-NEXT:         storage f64 [size=8, align=8];
// NOSIMD-SIMD-NEXT:         storage f128 [size=16, align=16];
// NOSIMD-SIMD-NEXT:         storage d32 [size=4, align=4];
// NOSIMD-SIMD-NEXT:         storage d64 [size=8, align=8];
// NOSIMD-SIMD-NEXT:         storage d128 [size=16, align=16];
// NOSIMD-SIMD-NEXT:     }
// NOSIMD-SIMD-NEXT:     global %0 val__ARM_FEATURE_BF16: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %1 val__ARM_FEATURE_BF16_VECTOR_ARITHMETIC: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %2 val__ARM_FEATURE_FMA: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %3 val__ARM_FEATURE_FP16_FML: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %4 val__ARM_FEATURE_FRINT: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %5 val__ARM_FEATURE_JCVT: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %6 val__ARM_FEATURE_NUMERIC_MAXMIN: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %7 val__ARM_FEATURE_QRDMX: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %8 val__ARM_FP16_FORMAT_IEEE: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %9 val__FLT_EVAL_METHOD__: array<i32, 17> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %10 val__FLT_EVAL_METHOD_C99__: array<i32, 17> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %11 val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %12 val__GCC_DESTRUCTIVE_SIZE: array<i32, 65> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %13 val__GCC_IEC_559: array<i32, 3> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT:     global %14 val__STDC_IEC_559__: array<i32, 2> [storage=static] [linkage=external];
// NOSIMD-SIMD-NEXT: }
// SLATE-FILECHECK-END NOSIMD-SIMD
// SLATE-FILECHECK-BEGIN SVE-FIXED
// SVE-FIXED: module {
// SVE-FIXED-NEXT:     target "aarch64-unknown-linux-gnu" {
// SVE-FIXED-NEXT:         endian = little;
// SVE-FIXED-NEXT:         pointer [size=8, align=8];
// SVE-FIXED-NEXT:         stack_alignment = 16;
// SVE-FIXED-NEXT:         long_double = f128;
// SVE-FIXED-NEXT:         storage bool [size=1, align=1];
// SVE-FIXED-NEXT:         storage i8, u8 [size=1, align=1];
// SVE-FIXED-NEXT:         storage i16, u16 [size=2, align=2];
// SVE-FIXED-NEXT:         storage i32, u32 [size=4, align=4];
// SVE-FIXED-NEXT:         storage i64, u64 [size=8, align=8];
// SVE-FIXED-NEXT:         storage i128, u128 [size=16, align=16];
// SVE-FIXED-NEXT:         storage bf16 [size=2, align=2];
// SVE-FIXED-NEXT:         storage f16 [size=2, align=2];
// SVE-FIXED-NEXT:         storage f32 [size=4, align=4];
// SVE-FIXED-NEXT:         storage f64 [size=8, align=8];
// SVE-FIXED-NEXT:         storage f128 [size=16, align=16];
// SVE-FIXED-NEXT:         storage d32 [size=4, align=4];
// SVE-FIXED-NEXT:         storage d64 [size=8, align=8];
// SVE-FIXED-NEXT:         storage d128 [size=16, align=16];
// SVE-FIXED-NEXT:     }
// SVE-FIXED-NEXT:     global %0 val__ARM_FEATURE_COMPLEX: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %1 val__ARM_FEATURE_FMA: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %2 val__ARM_FEATURE_NUMERIC_MAXMIN: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %3 val__ARM_FEATURE_SVE: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %4 val__ARM_FEATURE_SVE_BITS: array<i32, 257> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %5 val__ARM_FEATURE_SVE_PREDICATE_OPERATORS: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %6 val__ARM_FEATURE_SVE_VECTOR_OPERATORS: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %7 val__ARM_FP16_FORMAT_IEEE: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %8 val__FLT_EVAL_METHOD__: array<i32, 17> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %9 val__FLT_EVAL_METHOD_C99__: array<i32, 17> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %10 val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %11 val__GCC_DESTRUCTIVE_SIZE: array<i32, 257> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %12 val__GCC_IEC_559: array<i32, 3> [storage=static] [linkage=external];
// SVE-FIXED-NEXT:     global %13 val__STDC_IEC_559__: array<i32, 2> [storage=static] [linkage=external];
// SVE-FIXED-NEXT: }
// SLATE-FILECHECK-END SVE-FIXED
// SLATE-FILECHECK-BEGIN CRYPTO
// CRYPTO: module {
// CRYPTO-NEXT:     target "aarch64-unknown-linux-gnu" {
// CRYPTO-NEXT:         endian = little;
// CRYPTO-NEXT:         pointer [size=8, align=8];
// CRYPTO-NEXT:         stack_alignment = 16;
// CRYPTO-NEXT:         long_double = f128;
// CRYPTO-NEXT:         storage bool [size=1, align=1];
// CRYPTO-NEXT:         storage i8, u8 [size=1, align=1];
// CRYPTO-NEXT:         storage i16, u16 [size=2, align=2];
// CRYPTO-NEXT:         storage i32, u32 [size=4, align=4];
// CRYPTO-NEXT:         storage i64, u64 [size=8, align=8];
// CRYPTO-NEXT:         storage i128, u128 [size=16, align=16];
// CRYPTO-NEXT:         storage bf16 [size=2, align=2];
// CRYPTO-NEXT:         storage f16 [size=2, align=2];
// CRYPTO-NEXT:         storage f32 [size=4, align=4];
// CRYPTO-NEXT:         storage f64 [size=8, align=8];
// CRYPTO-NEXT:         storage f128 [size=16, align=16];
// CRYPTO-NEXT:         storage d32 [size=4, align=4];
// CRYPTO-NEXT:         storage d64 [size=8, align=8];
// CRYPTO-NEXT:         storage d128 [size=16, align=16];
// CRYPTO-NEXT:     }
// CRYPTO-NEXT:     global %0 val__ARM_FEATURE_COMPLEX: array<i32, 2> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %1 val__ARM_FEATURE_FMA: array<i32, 2> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %2 val__ARM_FEATURE_JCVT: array<i32, 2> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %3 val__ARM_FEATURE_NUMERIC_MAXMIN: array<i32, 2> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %4 val__ARM_FEATURE_QRDMX: array<i32, 2> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %5 val__ARM_FP16_FORMAT_IEEE: array<i32, 2> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %6 val__FLT_EVAL_METHOD__: array<i32, 1> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %7 val__FLT_EVAL_METHOD_C99__: array<i32, 1> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %8 val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %9 val__GCC_DESTRUCTIVE_SIZE: array<i32, 257> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %10 val__GCC_IEC_559: array<i32, 3> [storage=static] [linkage=external];
// CRYPTO-NEXT:     global %11 val__STDC_IEC_559__: array<i32, 2> [storage=static] [linkage=external];
// CRYPTO-NEXT: }
// SLATE-FILECHECK-END CRYPTO
