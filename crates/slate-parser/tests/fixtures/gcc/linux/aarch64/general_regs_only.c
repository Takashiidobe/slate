// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES BASE
// SLATE-FILECHECK-DEFINES GENERAL-REGS-ONLY
// SLATE-FILECHECK-PREFIX-ARGS GENERAL-REGS-ONLY -mgeneral-regs-only
// SLATE-FILECHECK-DEFINES SIMD-STILL-OFF
// SLATE-FILECHECK-PREFIX-ARGS SIMD-STILL-OFF -march=armv8-a+simd -mgeneral-regs-only

#ifdef __ARM_FP
int val_arm_fp[__ARM_FP + 1];
#endif
#ifdef __ARM_NEON
int has_neon;
#endif
#ifdef __ARM_NEON_FP
int val_arm_neon_fp[__ARM_NEON_FP + 1];
#endif
#ifdef __GCC_IEC_559
int val_gcc_iec_559[__GCC_IEC_559 + 1];
#endif
#ifdef __STDC_IEC_559__
int has_stdc_iec_559;
#endif
#ifdef __FP_FAST_FMA
int has_fp_fast_fma;
#endif

// SLATE-FILECHECK-BEGIN BASE
// BASE: module {
// BASE-NEXT:     target "aarch64-unknown-linux-gnu" {
// BASE-NEXT:         endian = little;
// BASE-NEXT:         pointer [size=8, align=8];
// BASE-NEXT:         stack_alignment = 16;
// BASE-NEXT:         long_double = f128;
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
// BASE-NEXT:         storage f128 [size=16, align=16];
// BASE-NEXT:         storage d32 [size=4, align=4];
// BASE-NEXT:         storage d64 [size=8, align=8];
// BASE-NEXT:         storage d128 [size=16, align=16];
// BASE-NEXT:     }
// BASE-NEXT:     global %[[VALUE_val_arm_fp:[0-9]+]] val_arm_fp: array<i32, 15> [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_has_neon:[0-9]+]] has_neon: i32 [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 3> [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_has_stdc_iec_559:[0-9]+]] has_stdc_iec_559: i32 [storage=static] [linkage=external];
// BASE-NEXT:     global %[[VALUE_has_fp_fast_fma:[0-9]+]] has_fp_fast_fma: i32 [storage=static] [linkage=external];
// BASE-NEXT: }
// SLATE-FILECHECK-END BASE
// SLATE-FILECHECK-BEGIN GENERAL-REGS-ONLY
// GENERAL-REGS-ONLY: module {
// GENERAL-REGS-ONLY-NEXT:     target "aarch64-unknown-linux-gnu" {
// GENERAL-REGS-ONLY-NEXT:         endian = little;
// GENERAL-REGS-ONLY-NEXT:         pointer [size=8, align=8];
// GENERAL-REGS-ONLY-NEXT:         stack_alignment = 16;
// GENERAL-REGS-ONLY-NEXT:         long_double = f128;
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
// GENERAL-REGS-ONLY-NEXT:         storage f128 [size=16, align=16];
// GENERAL-REGS-ONLY-NEXT:         storage d32 [size=4, align=4];
// GENERAL-REGS-ONLY-NEXT:         storage d64 [size=8, align=8];
// GENERAL-REGS-ONLY-NEXT:         storage d128 [size=16, align=16];
// GENERAL-REGS-ONLY-NEXT:     }
// GENERAL-REGS-ONLY-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 1> [storage=static] [linkage=external];
// GENERAL-REGS-ONLY-NEXT: }
// SLATE-FILECHECK-END GENERAL-REGS-ONLY
// SLATE-FILECHECK-BEGIN SIMD-STILL-OFF
// SIMD-STILL-OFF: module {
// SIMD-STILL-OFF-NEXT:     target "aarch64-unknown-linux-gnu" {
// SIMD-STILL-OFF-NEXT:         endian = little;
// SIMD-STILL-OFF-NEXT:         pointer [size=8, align=8];
// SIMD-STILL-OFF-NEXT:         stack_alignment = 16;
// SIMD-STILL-OFF-NEXT:         long_double = f128;
// SIMD-STILL-OFF-NEXT:         storage bool [size=1, align=1];
// SIMD-STILL-OFF-NEXT:         storage i8, u8 [size=1, align=1];
// SIMD-STILL-OFF-NEXT:         storage i16, u16 [size=2, align=2];
// SIMD-STILL-OFF-NEXT:         storage i32, u32 [size=4, align=4];
// SIMD-STILL-OFF-NEXT:         storage i64, u64 [size=8, align=8];
// SIMD-STILL-OFF-NEXT:         storage i128, u128 [size=16, align=16];
// SIMD-STILL-OFF-NEXT:         storage bf16 [size=2, align=2];
// SIMD-STILL-OFF-NEXT:         storage f16 [size=2, align=2];
// SIMD-STILL-OFF-NEXT:         storage f32 [size=4, align=4];
// SIMD-STILL-OFF-NEXT:         storage f64 [size=8, align=8];
// SIMD-STILL-OFF-NEXT:         storage f128 [size=16, align=16];
// SIMD-STILL-OFF-NEXT:         storage d32 [size=4, align=4];
// SIMD-STILL-OFF-NEXT:         storage d64 [size=8, align=8];
// SIMD-STILL-OFF-NEXT:         storage d128 [size=16, align=16];
// SIMD-STILL-OFF-NEXT:     }
// SIMD-STILL-OFF-NEXT:     global %[[VALUE_val_gcc_iec_559:[0-9]+]] val_gcc_iec_559: array<i32, 1> [storage=static] [linkage=external];
// SIMD-STILL-OFF-NEXT: }
// SLATE-FILECHECK-END SIMD-STILL-OFF
