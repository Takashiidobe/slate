// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES CLANG

#ifdef __k8__
int has__k8__;
#endif
#ifdef __pentium4__
int has__pentium4__;
#endif
#ifdef __tune_pentium4__
int has__tune_pentium4__;
#endif
#ifdef __SSE2__
int has__SSE2__;
#endif
#ifdef __SSE2_MATH__
int has__SSE2_MATH__;
#endif

// SLATE-FILECHECK-BEGIN CLANG
// CLANG: module {
// CLANG-NEXT:     target "i686-unknown-linux-gnu" {
// CLANG-NEXT:         endian = little;
// CLANG-NEXT:         pointer [size=4, align=4];
// CLANG-NEXT:         stack_alignment = 16;
// CLANG-NEXT:         long_double = f80;
// CLANG-NEXT:         storage bool [size=1, align=1];
// CLANG-NEXT:         storage i8, u8 [size=1, align=1];
// CLANG-NEXT:         storage i16, u16 [size=2, align=2];
// CLANG-NEXT:         storage i32, u32 [size=4, align=4];
// CLANG-NEXT:         storage i64, u64 [size=8, align=4];
// CLANG-NEXT:         storage i128, u128 [size=16, align=16];
// CLANG-NEXT:         storage bf16 [size=2, align=2];
// CLANG-NEXT:         storage f16 [size=2, align=2];
// CLANG-NEXT:         storage f32 [size=4, align=4];
// CLANG-NEXT:         storage f64 [size=8, align=4];
// CLANG-NEXT:         storage f80 [size=12, align=4];
// CLANG-NEXT:         storage f128 [size=16, align=16];
// CLANG-NEXT:         storage d32 [size=4, align=4];
// CLANG-NEXT:         storage d64 [size=8, align=8];
// CLANG-NEXT:         storage d128 [size=16, align=16];
// CLANG-NEXT:     }
// CLANG-NEXT:     global %0 has__pentium4__: i32 [storage=static] [linkage=external];
// CLANG-NEXT:     global %1 has__tune_pentium4__: i32 [storage=static] [linkage=external];
// CLANG-NEXT:     global %2 has__SSE2__: i32 [storage=static] [linkage=external];
// CLANG-NEXT:     global %3 has__SSE2_MATH__: i32 [storage=static] [linkage=external];
// CLANG-NEXT: }
// SLATE-FILECHECK-END CLANG
