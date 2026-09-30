// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir -mavx512f


#ifdef __MMX__
int has__MMX__;
#endif
#ifdef __SSE__
int has__SSE__;
#endif
#ifdef __SSE2__
int has__SSE2__;
#endif
#ifdef __SSE3__
int has__SSE3__;
#endif
#ifdef __SSSE3__
int has__SSSE3__;
#endif
#ifdef __SSE4_1__
int has__SSE4_1__;
#endif
#ifdef __SSE4_2__
int has__SSE4_2__;
#endif
#ifdef __POPCNT__
int has__POPCNT__;
#endif
#ifdef __CRC32__
int has__CRC32__;
#endif
#ifdef __XSAVE__
int has__XSAVE__;
#endif
#ifdef __FXSR__
int has__FXSR__;
#endif
#ifdef __LAHF_SAHF__
int has__LAHF_SAHF__;
#endif
#ifdef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_16
int has__GCC_HAVE_SYNC_COMPARE_AND_SWAP_16;
#endif
#ifdef __AVX__
int has__AVX__;
#endif
#ifdef __AVX2__
int has__AVX2__;
#endif
#ifdef __FMA__
int has__FMA__;
#endif
#ifdef __FP_FAST_FMA
int has__FP_FAST_FMA;
#endif
#ifdef __FP_FAST_FMAF
int has__FP_FAST_FMAF;
#endif
#ifdef __FP_FAST_FMAF32
int has__FP_FAST_FMAF32;
#endif
#ifdef __FP_FAST_FMAF32x
int has__FP_FAST_FMAF32x;
#endif
#ifdef __FP_FAST_FMAF64
int has__FP_FAST_FMAF64;
#endif
#ifdef __MMX_WITH_SSE__
int has__MMX_WITH_SSE__;
#endif
#ifdef __F16C__
int has__F16C__;
#endif
#ifdef __AVX512F__
int has__AVX512F__;
#endif
#ifdef __AVX512BW__
int has__AVX512BW__;
#endif
#ifdef __AVX512CD__
int has__AVX512CD__;
#endif
#ifdef __AVX512DQ__
int has__AVX512DQ__;
#endif
#ifdef __AVX512VL__
int has__AVX512VL__;
#endif
#ifdef __BMI__
int has__BMI__;
#endif
#ifdef __BMI2__
int has__BMI2__;
#endif
#ifdef __LZCNT__
int has__LZCNT__;
#endif
#ifdef __MOVBE__
int has__MOVBE__;
#endif
#ifdef __SSE_MATH__
int has__SSE_MATH__;
#endif
#ifdef __SSE2_MATH__
int has__SSE2_MATH__;
#endif
#ifdef __k8
int has__k8;
#endif
#ifdef __k8__
int has__k8__;
#endif
#ifdef __tune_k8__
int has__tune_k8__;
#endif
#ifdef __pentium4
int has__pentium4;
#endif
#ifdef __pentium4__
int has__pentium4__;
#endif
#ifdef __tune_pentium4__
int has__tune_pentium4__;
#endif
int biggest[__BIGGEST_ALIGNMENT__];

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "x86_64-unknown-linux-gnu" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=8, align=8];
// CHECK-NEXT:         stack_alignment = 16;
// CHECK-NEXT:         long_double = f80;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=8];
// CHECK-NEXT:         storage i128, u128 [size=16, align=16];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f80 [size=16, align=16];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     global %[[VALUE_has__MMX__:[0-9]+]] has__MMX__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE__:[0-9]+]] has__SSE__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE2__:[0-9]+]] has__SSE2__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE3__:[0-9]+]] has__SSE3__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSSE3__:[0-9]+]] has__SSSE3__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE4_1__:[0-9]+]] has__SSE4_1__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE4_2__:[0-9]+]] has__SSE4_2__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__POPCNT__:[0-9]+]] has__POPCNT__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__CRC32__:[0-9]+]] has__CRC32__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__XSAVE__:[0-9]+]] has__XSAVE__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__FXSR__:[0-9]+]] has__FXSR__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__AVX__:[0-9]+]] has__AVX__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__AVX2__:[0-9]+]] has__AVX2__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__FP_FAST_FMA:[0-9]+]] has__FP_FAST_FMA: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__FP_FAST_FMAF:[0-9]+]] has__FP_FAST_FMAF: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__FP_FAST_FMAF32:[0-9]+]] has__FP_FAST_FMAF32: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__FP_FAST_FMAF32x:[0-9]+]] has__FP_FAST_FMAF32x: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__FP_FAST_FMAF64:[0-9]+]] has__FP_FAST_FMAF64: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__MMX_WITH_SSE__:[0-9]+]] has__MMX_WITH_SSE__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__AVX512F__:[0-9]+]] has__AVX512F__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE_MATH__:[0-9]+]] has__SSE_MATH__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE2_MATH__:[0-9]+]] has__SSE2_MATH__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__k8:[0-9]+]] has__k8: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__k8__:[0-9]+]] has__k8__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_biggest:[0-9]+]] biggest: array<i32, 64> [storage=static] [align=16] [linkage=external];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
