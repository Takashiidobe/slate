// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES GCC

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

// SLATE-FILECHECK-BEGIN GCC
// GCC: module {
// GCC-NEXT:     target "i686-unknown-linux-gnu" {
// GCC-NEXT:         endian = little;
// GCC-NEXT:         pointer [size=4, align=4];
// GCC-NEXT:         stack_alignment = 16;
// GCC-NEXT:         long_double = f80;
// GCC-NEXT:         storage bool [size=1, align=1];
// GCC-NEXT:         storage i8, u8 [size=1, align=1];
// GCC-NEXT:         storage i16, u16 [size=2, align=2];
// GCC-NEXT:         storage i32, u32 [size=4, align=4];
// GCC-NEXT:         storage i64, u64 [size=8, align=4];
// GCC-NEXT:         storage i128, u128 [size=16, align=16];
// GCC-NEXT:         storage bf16 [size=2, align=2];
// GCC-NEXT:         storage f16 [size=2, align=2];
// GCC-NEXT:         storage f32 [size=4, align=4];
// GCC-NEXT:         storage f64 [size=8, align=4];
// GCC-NEXT:         storage f80 [size=12, align=4];
// GCC-NEXT:         storage f128 [size=16, align=16];
// GCC-NEXT:         storage d32 [size=4, align=4];
// GCC-NEXT:         storage d64 [size=8, align=8];
// GCC-NEXT:         storage d128 [size=16, align=16];
// GCC-NEXT:     }
// GCC-NEXT:     global %[[VALUE_has__k8__:[0-9]+]] has__k8__: i32 [storage=static] [linkage=external];
// GCC-NEXT:     global %[[VALUE_has__SSE2__:[0-9]+]] has__SSE2__: i32 [storage=static] [linkage=external];
// GCC-NEXT: }
// SLATE-FILECHECK-END GCC
