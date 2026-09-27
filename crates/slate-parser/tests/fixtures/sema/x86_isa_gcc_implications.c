// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES NO-FMA
// SLATE-FILECHECK-PREFIX-ARGS NO-FMA --flavor=gcc --target=x86_64-unknown-linux-gnu -mavx512f -mno-fma
// SLATE-FILECHECK-DEFINES V4-NO-F16C
// SLATE-FILECHECK-PREFIX-ARGS V4-NO-F16C --flavor=gcc --target=x86_64-unknown-linux-gnu -march=x86-64-v4 -mno-f16c
// SLATE-FILECHECK-DEFINES NO-AVX
// SLATE-FILECHECK-PREFIX-ARGS NO-AVX --flavor=gcc --target=x86_64-unknown-linux-gnu -mfma -mno-avx
// SLATE-FILECHECK-DEFINES CANCEL
// SLATE-FILECHECK-PREFIX-ARGS CANCEL --flavor=gcc --target=x86_64-unknown-linux-gnu -mavx -mno-avx
// SLATE-FILECHECK-DEFINES NO-XSAVE
// SLATE-FILECHECK-PREFIX-ARGS NO-XSAVE --flavor=gcc --target=x86_64-unknown-linux-gnu -mavx512f -mno-xsave
// SLATE-FILECHECK-DEFINES CLANG-NO-FMA
// SLATE-FILECHECK-PREFIX-ARGS CLANG-NO-FMA --flavor=clang --target=x86_64-unknown-linux-gnu -mavx512f -mno-fma

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

// SLATE-FILECHECK-BEGIN NO-FMA
// NO-FMA: module {
// NO-FMA-NEXT:     target "x86_64-unknown-linux-gnu" {
// NO-FMA-NEXT:         endian = little;
// NO-FMA-NEXT:         pointer [size=8, align=8];
// NO-FMA-NEXT:         stack_alignment = 16;
// NO-FMA-NEXT:         long_double = f80;
// NO-FMA-NEXT:         storage bool [size=1, align=1];
// NO-FMA-NEXT:         storage i8, u8 [size=1, align=1];
// NO-FMA-NEXT:         storage i16, u16 [size=2, align=2];
// NO-FMA-NEXT:         storage i32, u32 [size=4, align=4];
// NO-FMA-NEXT:         storage i64, u64 [size=8, align=8];
// NO-FMA-NEXT:         storage i128, u128 [size=16, align=16];
// NO-FMA-NEXT:         storage bf16 [size=2, align=2];
// NO-FMA-NEXT:         storage f16 [size=2, align=2];
// NO-FMA-NEXT:         storage f32 [size=4, align=4];
// NO-FMA-NEXT:         storage f64 [size=8, align=8];
// NO-FMA-NEXT:         storage f80 [size=16, align=16];
// NO-FMA-NEXT:         storage f128 [size=16, align=16];
// NO-FMA-NEXT:         storage d32 [size=4, align=4];
// NO-FMA-NEXT:         storage d64 [size=8, align=8];
// NO-FMA-NEXT:         storage d128 [size=16, align=16];
// NO-FMA-NEXT:     }
// NO-FMA-NEXT:     global %0 val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// NO-FMA-NEXT:     global %1 val__AVX__: array<i32, 2> [storage=static] [linkage=external];
// NO-FMA-NEXT:     global %2 val__AVX2__: array<i32, 2> [storage=static] [linkage=external];
// NO-FMA-NEXT:     global %3 val__AVX512F__: array<i32, 2> [storage=static] [linkage=external];
// NO-FMA-NEXT:     global %4 val__XSAVE__: array<i32, 2> [storage=static] [linkage=external];
// NO-FMA-NEXT:     global %5 val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// NO-FMA-NEXT:     global %6 val__BIGGEST_ALIGNMENT__: array<i32, 65> [storage=static] [align=16] [linkage=external];
// NO-FMA-NEXT: }
// SLATE-FILECHECK-END NO-FMA
// SLATE-FILECHECK-BEGIN V4-NO-F16C
// V4-NO-F16C: module {
// V4-NO-F16C-NEXT:     target "x86_64-unknown-linux-gnu" {
// V4-NO-F16C-NEXT:         endian = little;
// V4-NO-F16C-NEXT:         pointer [size=8, align=8];
// V4-NO-F16C-NEXT:         stack_alignment = 16;
// V4-NO-F16C-NEXT:         long_double = f80;
// V4-NO-F16C-NEXT:         storage bool [size=1, align=1];
// V4-NO-F16C-NEXT:         storage i8, u8 [size=1, align=1];
// V4-NO-F16C-NEXT:         storage i16, u16 [size=2, align=2];
// V4-NO-F16C-NEXT:         storage i32, u32 [size=4, align=4];
// V4-NO-F16C-NEXT:         storage i64, u64 [size=8, align=8];
// V4-NO-F16C-NEXT:         storage i128, u128 [size=16, align=16];
// V4-NO-F16C-NEXT:         storage bf16 [size=2, align=2];
// V4-NO-F16C-NEXT:         storage f16 [size=2, align=2];
// V4-NO-F16C-NEXT:         storage f32 [size=4, align=4];
// V4-NO-F16C-NEXT:         storage f64 [size=8, align=8];
// V4-NO-F16C-NEXT:         storage f80 [size=16, align=16];
// V4-NO-F16C-NEXT:         storage f128 [size=16, align=16];
// V4-NO-F16C-NEXT:         storage d32 [size=4, align=4];
// V4-NO-F16C-NEXT:         storage d64 [size=8, align=8];
// V4-NO-F16C-NEXT:         storage d128 [size=16, align=16];
// V4-NO-F16C-NEXT:     }
// V4-NO-F16C-NEXT:     global %0 val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %1 val__AVX__: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %2 val__AVX2__: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %3 val__FMA__: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %4 val__AVX512F__: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %5 val__AVX512VL__: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %6 val__XSAVE__: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %7 val__EVEX256__: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %8 val__FP_FAST_FMA: array<i32, 2> [storage=static] [linkage=external];
// V4-NO-F16C-NEXT:     global %9 val__BIGGEST_ALIGNMENT__: array<i32, 65> [storage=static] [align=16] [linkage=external];
// V4-NO-F16C-NEXT: }
// SLATE-FILECHECK-END V4-NO-F16C
// SLATE-FILECHECK-BEGIN NO-AVX
// NO-AVX: module {
// NO-AVX-NEXT:     target "x86_64-unknown-linux-gnu" {
// NO-AVX-NEXT:         endian = little;
// NO-AVX-NEXT:         pointer [size=8, align=8];
// NO-AVX-NEXT:         stack_alignment = 16;
// NO-AVX-NEXT:         long_double = f80;
// NO-AVX-NEXT:         storage bool [size=1, align=1];
// NO-AVX-NEXT:         storage i8, u8 [size=1, align=1];
// NO-AVX-NEXT:         storage i16, u16 [size=2, align=2];
// NO-AVX-NEXT:         storage i32, u32 [size=4, align=4];
// NO-AVX-NEXT:         storage i64, u64 [size=8, align=8];
// NO-AVX-NEXT:         storage i128, u128 [size=16, align=16];
// NO-AVX-NEXT:         storage bf16 [size=2, align=2];
// NO-AVX-NEXT:         storage f16 [size=2, align=2];
// NO-AVX-NEXT:         storage f32 [size=4, align=4];
// NO-AVX-NEXT:         storage f64 [size=8, align=8];
// NO-AVX-NEXT:         storage f80 [size=16, align=16];
// NO-AVX-NEXT:         storage f128 [size=16, align=16];
// NO-AVX-NEXT:         storage d32 [size=4, align=4];
// NO-AVX-NEXT:         storage d64 [size=8, align=8];
// NO-AVX-NEXT:         storage d128 [size=16, align=16];
// NO-AVX-NEXT:     }
// NO-AVX-NEXT:     global %0 val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// NO-AVX-NEXT:     global %1 val__XSAVE__: array<i32, 2> [storage=static] [linkage=external];
// NO-AVX-NEXT:     global %2 val__BIGGEST_ALIGNMENT__: array<i32, 17> [storage=static] [align=16] [linkage=external];
// NO-AVX-NEXT: }
// SLATE-FILECHECK-END NO-AVX
// SLATE-FILECHECK-BEGIN CANCEL
// CANCEL: module {
// CANCEL-NEXT:     target "x86_64-unknown-linux-gnu" {
// CANCEL-NEXT:         endian = little;
// CANCEL-NEXT:         pointer [size=8, align=8];
// CANCEL-NEXT:         stack_alignment = 16;
// CANCEL-NEXT:         long_double = f80;
// CANCEL-NEXT:         storage bool [size=1, align=1];
// CANCEL-NEXT:         storage i8, u8 [size=1, align=1];
// CANCEL-NEXT:         storage i16, u16 [size=2, align=2];
// CANCEL-NEXT:         storage i32, u32 [size=4, align=4];
// CANCEL-NEXT:         storage i64, u64 [size=8, align=8];
// CANCEL-NEXT:         storage i128, u128 [size=16, align=16];
// CANCEL-NEXT:         storage bf16 [size=2, align=2];
// CANCEL-NEXT:         storage f16 [size=2, align=2];
// CANCEL-NEXT:         storage f32 [size=4, align=4];
// CANCEL-NEXT:         storage f64 [size=8, align=8];
// CANCEL-NEXT:         storage f80 [size=16, align=16];
// CANCEL-NEXT:         storage f128 [size=16, align=16];
// CANCEL-NEXT:         storage d32 [size=4, align=4];
// CANCEL-NEXT:         storage d64 [size=8, align=8];
// CANCEL-NEXT:         storage d128 [size=16, align=16];
// CANCEL-NEXT:     }
// CANCEL-NEXT:     global %0 val__BIGGEST_ALIGNMENT__: array<i32, 17> [storage=static] [align=16] [linkage=external];
// CANCEL-NEXT: }
// SLATE-FILECHECK-END CANCEL
// SLATE-FILECHECK-BEGIN NO-XSAVE
// NO-XSAVE: module {
// NO-XSAVE-NEXT:     target "x86_64-unknown-linux-gnu" {
// NO-XSAVE-NEXT:         endian = little;
// NO-XSAVE-NEXT:         pointer [size=8, align=8];
// NO-XSAVE-NEXT:         stack_alignment = 16;
// NO-XSAVE-NEXT:         long_double = f80;
// NO-XSAVE-NEXT:         storage bool [size=1, align=1];
// NO-XSAVE-NEXT:         storage i8, u8 [size=1, align=1];
// NO-XSAVE-NEXT:         storage i16, u16 [size=2, align=2];
// NO-XSAVE-NEXT:         storage i32, u32 [size=4, align=4];
// NO-XSAVE-NEXT:         storage i64, u64 [size=8, align=8];
// NO-XSAVE-NEXT:         storage i128, u128 [size=16, align=16];
// NO-XSAVE-NEXT:         storage bf16 [size=2, align=2];
// NO-XSAVE-NEXT:         storage f16 [size=2, align=2];
// NO-XSAVE-NEXT:         storage f32 [size=4, align=4];
// NO-XSAVE-NEXT:         storage f64 [size=8, align=8];
// NO-XSAVE-NEXT:         storage f80 [size=16, align=16];
// NO-XSAVE-NEXT:         storage f128 [size=16, align=16];
// NO-XSAVE-NEXT:         storage d32 [size=4, align=4];
// NO-XSAVE-NEXT:         storage d64 [size=8, align=8];
// NO-XSAVE-NEXT:         storage d128 [size=16, align=16];
// NO-XSAVE-NEXT:     }
// NO-XSAVE-NEXT:     global %0 val__SSE4_2__: array<i32, 2> [storage=static] [linkage=external];
// NO-XSAVE-NEXT:     global %1 val__BIGGEST_ALIGNMENT__: array<i32, 17> [storage=static] [align=16] [linkage=external];
// NO-XSAVE-NEXT: }
// SLATE-FILECHECK-END NO-XSAVE
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
