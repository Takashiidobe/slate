// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES BASE
// SLATE-FILECHECK-DEFINES GENERAL-REGS-ONLY
// SLATE-FILECHECK-PREFIX-ARGS GENERAL-REGS-ONLY -mgeneral-regs-only
// SLATE-FILECHECK-DEFINES LATER-SSE
// SLATE-FILECHECK-PREFIX-ARGS LATER-SSE -mgeneral-regs-only -msse -msse2
// SLATE-FILECHECK-DEFINES EARLIER-SSE
// SLATE-FILECHECK-PREFIX-ARGS EARLIER-SSE -msse -msse2 -mgeneral-regs-only

// SLATE-FILECHECK-DEFINES AVX-AFTER
// SLATE-FILECHECK-PREFIX-ARGS AVX-AFTER -mgeneral-regs-only -mavx
// SLATE-FILECHECK-DEFINES AVX-BEFORE
// SLATE-FILECHECK-PREFIX-ARGS AVX-BEFORE -mavx -mgeneral-regs-only
#ifdef __AVX__
int has_avx;
#endif
#ifdef __POPCNT__
int has_popcnt;
#endif
#ifdef __CRC32__
int has_crc32;
#endif
#ifdef __MMX__
int has_mmx;
#endif
#ifdef __SSE__
int has_sse;
#endif
#ifdef __SSE2__
int has_sse2;
#endif
#ifdef __SSE_MATH__
int has_sse_math;
#endif
#ifdef _SOFT_FLOAT
int has_soft_float;
#endif
#ifdef __GCC_IEC_559
int val_gcc_iec_559[__GCC_IEC_559 + 1];
#endif
#ifdef __STDC_IEC_559__
int has_stdc_iec_559;
#endif
#ifdef __FLT_EVAL_METHOD__
int val_flt_eval_method[__FLT_EVAL_METHOD__ + 2];
#endif

// SLATE-FILECHECK-BEGIN BASE
// BASE: module {
// BASE-NEXT:     target "x86_64-unknown-linux-gnu" {
// BASE-NEXT:         endian = little;
// BASE-NEXT:         pointer [size=8, align=8];
// BASE-NEXT:         stack_alignment = 16;
// BASE-NEXT:         long_double = f80;
// BASE-NEXT:         storage bool [size=1, align=1];
// BASE-NEXT:         storage i8, u8 [size=1, align=1];
// BASE-NEXT:         storage i16, u16 [size=2, align=2];
// BASE-NEXT:         storage i32, u32 [size=4, align=4];
// BASE-NEXT:         storage i64, u64 [size=8, align=8];
// BASE-NEXT:         storage i128, u128 [size=16, align=16];
// BASE-NEXT:         storage bf16 [size=2, align=2];
// BASE-NEXT:         storage f16 [size=2, align=2];
// BASE-NEXT:         storage f32 [size=4, align=4];
// BASE-NEXT:         storage f64 [size=8, align=8];
// BASE-NEXT:         storage f80 [size=16, align=16];
// BASE-NEXT:         storage f128 [size=16, align=16];
// BASE-NEXT:         storage d32 [size=4, align=4];
// BASE-NEXT:         storage d64 [size=8, align=8];
// BASE-NEXT:         storage d128 [size=16, align=16];
// BASE-NEXT:     }
// BASE-NEXT:     global %[[VALUE_has_mmx:[0-9]+]] has_mmx: i32 [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_has_sse:[0-9]+]] has_sse: i32 [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_has_sse2:[0-9]+]] has_sse2: i32 [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_has_sse_math:[0-9]+]] has_sse_math: i32 [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 3> [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_has_stdc_iec_559:[0-9]+]] has_stdc_iec_559: i32 [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_val_flt_eval_method:[0-9]+]] val_flt_eval_method: array<i32, 2> [storage=static] [linkage=external];
// BASE-NEXT: }
// SLATE-FILECHECK-END BASE
// SLATE-FILECHECK-BEGIN GENERAL-REGS-ONLY
// GENERAL-REGS-ONLY: module {
// GENERAL-REGS-ONLY-NEXT:     target "x86_64-unknown-linux-gnu" {
// GENERAL-REGS-ONLY-NEXT:         endian = little;
// GENERAL-REGS-ONLY-NEXT:         pointer [size=8, align=8];
// GENERAL-REGS-ONLY-NEXT:         stack_alignment = 16;
// GENERAL-REGS-ONLY-NEXT:         long_double = f80;
// GENERAL-REGS-ONLY-NEXT:         storage bool [size=1, align=1];
// GENERAL-REGS-ONLY-NEXT:         storage i8, u8 [size=1, align=1];
// GENERAL-REGS-ONLY-NEXT:         storage i16, u16 [size=2, align=2];
// GENERAL-REGS-ONLY-NEXT:         storage i32, u32 [size=4, align=4];
// GENERAL-REGS-ONLY-NEXT:         storage i64, u64 [size=8, align=8];
// GENERAL-REGS-ONLY-NEXT:         storage i128, u128 [size=16, align=16];
// GENERAL-REGS-ONLY-NEXT:         storage bf16 [size=2, align=2];
// GENERAL-REGS-ONLY-NEXT:         storage f16 [size=2, align=2];
// GENERAL-REGS-ONLY-NEXT:         storage f32 [size=4, align=4];
// GENERAL-REGS-ONLY-NEXT:         storage f64 [size=8, align=8];
// GENERAL-REGS-ONLY-NEXT:         storage f80 [size=16, align=16];
// GENERAL-REGS-ONLY-NEXT:         storage f128 [size=16, align=16];
// GENERAL-REGS-ONLY-NEXT:         storage d32 [size=4, align=4];
// GENERAL-REGS-ONLY-NEXT:         storage d64 [size=8, align=8];
// GENERAL-REGS-ONLY-NEXT:         storage d128 [size=16, align=16];
// GENERAL-REGS-ONLY-NEXT:     }
// GENERAL-REGS-ONLY-NEXT:     global %[[VALUE_has_soft_float:[0-9]+]] has_soft_float: i32 [storage=static] [linkage=external];
// GENERAL-REGS-ONLY-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 1> [storage=static] [linkage=external];
// GENERAL-REGS-ONLY-NEXT:     global %[[VALUE_val_flt_eval_method:[0-9]+]] val_flt_eval_method: array<i32, 2> [storage=static] [linkage=external];
// GENERAL-REGS-ONLY-NEXT: }
// SLATE-FILECHECK-END GENERAL-REGS-ONLY
// SLATE-FILECHECK-BEGIN LATER-SSE
// LATER-SSE: module {
// LATER-SSE-NEXT:     target "x86_64-unknown-linux-gnu" {
// LATER-SSE-NEXT:         endian = little;
// LATER-SSE-NEXT:         pointer [size=8, align=8];
// LATER-SSE-NEXT:         stack_alignment = 16;
// LATER-SSE-NEXT:         long_double = f80;
// LATER-SSE-NEXT:         storage bool [size=1, align=1];
// LATER-SSE-NEXT:         storage i8, u8 [size=1, align=1];
// LATER-SSE-NEXT:         storage i16, u16 [size=2, align=2];
// LATER-SSE-NEXT:         storage i32, u32 [size=4, align=4];
// LATER-SSE-NEXT:         storage i64, u64 [size=8, align=8];
// LATER-SSE-NEXT:         storage i128, u128 [size=16, align=16];
// LATER-SSE-NEXT:         storage bf16 [size=2, align=2];
// LATER-SSE-NEXT:         storage f16 [size=2, align=2];
// LATER-SSE-NEXT:         storage f32 [size=4, align=4];
// LATER-SSE-NEXT:         storage f64 [size=8, align=8];
// LATER-SSE-NEXT:         storage f80 [size=16, align=16];
// LATER-SSE-NEXT:         storage f128 [size=16, align=16];
// LATER-SSE-NEXT:         storage d32 [size=4, align=4];
// LATER-SSE-NEXT:         storage d64 [size=8, align=8];
// LATER-SSE-NEXT:         storage d128 [size=16, align=16];
// LATER-SSE-NEXT:     }
// LATER-SSE-NEXT:     global %[[VALUE_has_sse:[0-9]+]] has_sse: i32 [storage=static] [linkage=external];
// LATER-SSE-NEXT:     global %[[VALUE_has_sse2:[0-9]+]] has_sse2: i32 [storage=static] [linkage=external];
// LATER-SSE-NEXT:     global %[[VALUE_has_sse_math:[0-9]+]] has_sse_math: i32 [storage=static] [linkage=external];
// LATER-SSE-NEXT:     global %[[VALUE_has_soft_float:[0-9]+]] has_soft_float: i32 [storage=static] [linkage=external];
// LATER-SSE-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 3> [storage=static] [linkage=external];
// LATER-SSE-NEXT:     global %[[VALUE_has_stdc_iec_559:[0-9]+]] has_stdc_iec_559: i32 [storage=static] [linkage=external];
// LATER-SSE-NEXT:     global %[[VALUE_val_flt_eval_method:[0-9]+]] val_flt_eval_method: array<i32, 2> [storage=static] [linkage=external];
// LATER-SSE-NEXT: }
// SLATE-FILECHECK-END LATER-SSE
// SLATE-FILECHECK-BEGIN EARLIER-SSE
// EARLIER-SSE: module {
// EARLIER-SSE-NEXT:     target "x86_64-unknown-linux-gnu" {
// EARLIER-SSE-NEXT:         endian = little;
// EARLIER-SSE-NEXT:         pointer [size=8, align=8];
// EARLIER-SSE-NEXT:         stack_alignment = 16;
// EARLIER-SSE-NEXT:         long_double = f80;
// EARLIER-SSE-NEXT:         storage bool [size=1, align=1];
// EARLIER-SSE-NEXT:         storage i8, u8 [size=1, align=1];
// EARLIER-SSE-NEXT:         storage i16, u16 [size=2, align=2];
// EARLIER-SSE-NEXT:         storage i32, u32 [size=4, align=4];
// EARLIER-SSE-NEXT:         storage i64, u64 [size=8, align=8];
// EARLIER-SSE-NEXT:         storage i128, u128 [size=16, align=16];
// EARLIER-SSE-NEXT:         storage bf16 [size=2, align=2];
// EARLIER-SSE-NEXT:         storage f16 [size=2, align=2];
// EARLIER-SSE-NEXT:         storage f32 [size=4, align=4];
// EARLIER-SSE-NEXT:         storage f64 [size=8, align=8];
// EARLIER-SSE-NEXT:         storage f80 [size=16, align=16];
// EARLIER-SSE-NEXT:         storage f128 [size=16, align=16];
// EARLIER-SSE-NEXT:         storage d32 [size=4, align=4];
// EARLIER-SSE-NEXT:         storage d64 [size=8, align=8];
// EARLIER-SSE-NEXT:         storage d128 [size=16, align=16];
// EARLIER-SSE-NEXT:     }
// EARLIER-SSE-NEXT:     global %[[VALUE_has_soft_float:[0-9]+]] has_soft_float: i32 [storage=static] [linkage=external];
// EARLIER-SSE-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 1> [storage=static] [linkage=external];
// EARLIER-SSE-NEXT:     global %[[VALUE_val_flt_eval_method:[0-9]+]] val_flt_eval_method: array<i32, 2> [storage=static] [linkage=external];
// EARLIER-SSE-NEXT: }
// SLATE-FILECHECK-END EARLIER-SSE
// SLATE-FILECHECK-BEGIN AVX-AFTER
// AVX-AFTER: module {
// AVX-AFTER-NEXT:     target "x86_64-unknown-linux-gnu" {
// AVX-AFTER-NEXT:         endian = little;
// AVX-AFTER-NEXT:         pointer [size=8, align=8];
// AVX-AFTER-NEXT:         stack_alignment = 16;
// AVX-AFTER-NEXT:         long_double = f80;
// AVX-AFTER-NEXT:         storage bool [size=1, align=1];
// AVX-AFTER-NEXT:         storage i8, u8 [size=1, align=1];
// AVX-AFTER-NEXT:         storage i16, u16 [size=2, align=2];
// AVX-AFTER-NEXT:         storage i32, u32 [size=4, align=4];
// AVX-AFTER-NEXT:         storage i64, u64 [size=8, align=8];
// AVX-AFTER-NEXT:         storage i128, u128 [size=16, align=16];
// AVX-AFTER-NEXT:         storage bf16 [size=2, align=2];
// AVX-AFTER-NEXT:         storage f16 [size=2, align=2];
// AVX-AFTER-NEXT:         storage f32 [size=4, align=4];
// AVX-AFTER-NEXT:         storage f64 [size=8, align=8];
// AVX-AFTER-NEXT:         storage f80 [size=16, align=16];
// AVX-AFTER-NEXT:         storage f128 [size=16, align=16];
// AVX-AFTER-NEXT:         storage d32 [size=4, align=4];
// AVX-AFTER-NEXT:         storage d64 [size=8, align=8];
// AVX-AFTER-NEXT:         storage d128 [size=16, align=16];
// AVX-AFTER-NEXT:     }
// AVX-AFTER-NEXT:     global %[[VALUE_has_avx:[0-9]+]] has_avx: i32 [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_has_popcnt:[0-9]+]] has_popcnt: i32 [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_has_crc32:[0-9]+]] has_crc32: i32 [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_has_sse:[0-9]+]] has_sse: i32 [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_has_sse2:[0-9]+]] has_sse2: i32 [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_has_sse_math:[0-9]+]] has_sse_math: i32 [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_has_soft_float:[0-9]+]] has_soft_float: i32 [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 3> [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_has_stdc_iec_559:[0-9]+]] has_stdc_iec_559: i32 [storage=static] [linkage=external];
// AVX-AFTER-NEXT:     global %[[VALUE_val_flt_eval_method:[0-9]+]] val_flt_eval_method: array<i32, 2> [storage=static] [linkage=external];
// AVX-AFTER-NEXT: }
// SLATE-FILECHECK-END AVX-AFTER
// SLATE-FILECHECK-BEGIN AVX-BEFORE
// AVX-BEFORE: module {
// AVX-BEFORE-NEXT:     target "x86_64-unknown-linux-gnu" {
// AVX-BEFORE-NEXT:         endian = little;
// AVX-BEFORE-NEXT:         pointer [size=8, align=8];
// AVX-BEFORE-NEXT:         stack_alignment = 16;
// AVX-BEFORE-NEXT:         long_double = f80;
// AVX-BEFORE-NEXT:         storage bool [size=1, align=1];
// AVX-BEFORE-NEXT:         storage i8, u8 [size=1, align=1];
// AVX-BEFORE-NEXT:         storage i16, u16 [size=2, align=2];
// AVX-BEFORE-NEXT:         storage i32, u32 [size=4, align=4];
// AVX-BEFORE-NEXT:         storage i64, u64 [size=8, align=8];
// AVX-BEFORE-NEXT:         storage i128, u128 [size=16, align=16];
// AVX-BEFORE-NEXT:         storage bf16 [size=2, align=2];
// AVX-BEFORE-NEXT:         storage f16 [size=2, align=2];
// AVX-BEFORE-NEXT:         storage f32 [size=4, align=4];
// AVX-BEFORE-NEXT:         storage f64 [size=8, align=8];
// AVX-BEFORE-NEXT:         storage f80 [size=16, align=16];
// AVX-BEFORE-NEXT:         storage f128 [size=16, align=16];
// AVX-BEFORE-NEXT:         storage d32 [size=4, align=4];
// AVX-BEFORE-NEXT:         storage d64 [size=8, align=8];
// AVX-BEFORE-NEXT:         storage d128 [size=16, align=16];
// AVX-BEFORE-NEXT:     }
// AVX-BEFORE-NEXT:     global %[[VALUE_has_popcnt:[0-9]+]] has_popcnt: i32 [storage=static] [linkage=external];
// AVX-BEFORE-NEXT:     global %[[VALUE_has_crc32:[0-9]+]] has_crc32: i32 [storage=static] [linkage=external];
// AVX-BEFORE-NEXT:     global %[[VALUE_has_soft_float:[0-9]+]] has_soft_float: i32 [storage=static] [linkage=external];
// AVX-BEFORE-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 1> [storage=static] [linkage=external];
// AVX-BEFORE-NEXT:     global %[[VALUE_val_flt_eval_method:[0-9]+]] val_flt_eval_method: array<i32, 2> [storage=static] [linkage=external];
// AVX-BEFORE-NEXT: }
// SLATE-FILECHECK-END AVX-BEFORE
