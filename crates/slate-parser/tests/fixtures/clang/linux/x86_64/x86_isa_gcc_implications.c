// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CLANG-NO-FMA
// SLATE-FILECHECK-PREFIX-ARGS CLANG-NO-FMA -mavx512f -mno-fma

#ifdef __SSE4_2__
int val__SSE4_2__[__SSE4_2__ + 1];
#endif
#ifdef __AVX__
int val__AVX__[__AVX__ + 1];
#endif
#ifdef __AVX2__
int val__AVX2__[__AVX2__ + 1];
#endif
#ifdef __FMA__
int val__FMA__[__FMA__ + 1];
#endif
#ifdef __F16C__
int val__F16C__[__F16C__ + 1];
#endif
#ifdef __AVX512F__
int val__AVX512F__[__AVX512F__ + 1];
#endif
#ifdef __AVX512VL__
int val__AVX512VL__[__AVX512VL__ + 1];
#endif
#ifdef __XSAVE__
int val__XSAVE__[__XSAVE__ + 1];
#endif
#ifdef __EVEX256__
int val__EVEX256__[__EVEX256__ + 1];
#endif
#ifdef __FP_FAST_FMA
int val__FP_FAST_FMA[__FP_FAST_FMA + 1];
#endif
#ifdef __BIGGEST_ALIGNMENT__
int val__BIGGEST_ALIGNMENT__[__BIGGEST_ALIGNMENT__ + 1];
#endif

// SLATE-FILECHECK-BEGIN CLANG-NO-FMA
// CLANG-NO-FMA: module {
// CLANG-NO-FMA-NEXT:     target "x86_64-unknown-linux-gnu" {
// CLANG-NO-FMA-NEXT:         endian = little;
// CLANG-NO-FMA-NEXT:         pointer [size=8, align=8];
// CLANG-NO-FMA-NEXT:         stack_alignment = 16;
// CLANG-NO-FMA-NEXT:         long_double = f80;
// CLANG-NO-FMA-NEXT:         storage bool [size=1, align=1];
// CLANG-NO-FMA-NEXT:         storage i8, u8 [size=1, align=1];
// CLANG-NO-FMA-NEXT:         storage i16, u16 [size=2, align=2];
// CLANG-NO-FMA-NEXT:         storage i32, u32 [size=4, align=4];
// CLANG-NO-FMA-NEXT:         storage i64, u64 [size=8, align=8];
// CLANG-NO-FMA-NEXT:         storage i128, u128 [size=16, align=16];
// CLANG-NO-FMA-NEXT:         storage bf16 [size=2, align=2];
// CLANG-NO-FMA-NEXT:         storage f16 [size=2, align=2];
// CLANG-NO-FMA-NEXT:         storage f32 [size=4, align=4];
// CLANG-NO-FMA-NEXT:         storage f64 [size=8, align=8];
// CLANG-NO-FMA-NEXT:         storage f80 [size=16, align=16];
// CLANG-NO-FMA-NEXT:         storage f128 [size=16, align=16];
// CLANG-NO-FMA-NEXT:         storage d32 [size=4, align=4];
// CLANG-NO-FMA-NEXT:         storage d64 [size=8, align=8];
// CLANG-NO-FMA-NEXT:         storage d128 [size=16, align=16];
// CLANG-NO-FMA-NEXT:     }
// CLANG-NO-FMA-NEXT:     global %0 val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// CLANG-NO-FMA-NEXT:     global %1 val__AVX__: array<i32, 2> [storage=static] [linkage=external];
// CLANG-NO-FMA-NEXT:     global %2 val__AVX2__: array<i32, 2> [storage=static] [linkage=external];
// CLANG-NO-FMA-NEXT:     global %3 val__F16C__: array<i32, 2> [storage=static] [linkage=external];
// CLANG-NO-FMA-NEXT:     global %4 val__XSAVE__: array<i32, 2> [storage=static] [linkage=external];
// CLANG-NO-FMA-NEXT:     global %5 val__BIGGEST_ALIGNMENT__: array<i32, 17> [storage=static] [align=16] [linkage=external];
// CLANG-NO-FMA-NEXT: }
// SLATE-FILECHECK-END CLANG-NO-FMA
