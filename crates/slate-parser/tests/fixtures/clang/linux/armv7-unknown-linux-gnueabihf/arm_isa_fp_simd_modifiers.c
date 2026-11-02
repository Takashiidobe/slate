// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES VFP
// SLATE-FILECHECK-PREFIX-ARGS VFP -mfpu=vfp
// SLATE-FILECHECK-DEFINES NO-FP
// SLATE-FILECHECK-PREFIX-ARGS NO-FP -march=armv7-a+nofp
// SLATE-FILECHECK-DEFINES NO-FP-FP
// SLATE-FILECHECK-PREFIX-ARGS NO-FP-FP -march=armv7-a+nofp+fp
// SLATE-FILECHECK-DEFINES NO-SIMD
// SLATE-FILECHECK-PREFIX-ARGS NO-SIMD -march=armv7-a+nosimd
// SLATE-FILECHECK-DEFINES NO-FP-EXPLICIT-FPU
// SLATE-FILECHECK-PREFIX-ARGS NO-FP-EXPLICIT-FPU -march=armv7-a+nofp -mfpu=neon
#ifdef __ARM_FP
int has_fp;
#endif
#ifdef __ARM_VFPV2__
int has_vfpv2;
#endif
#ifdef __ARM_VFPV3__
int has_vfpv3;
#endif
#ifdef __ARM_NEON
int has_neon;
#endif

// SLATE-FILECHECK-BEGIN VFP
// VFP: module {
// VFP-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// VFP-NEXT:         endian = little;
// VFP-NEXT:         pointer [size=4, align=4];
// VFP-NEXT:         stack_alignment = 8;
// VFP-NEXT:         long_double = f64;
// VFP-NEXT:         storage bool [size=1, align=1];
// VFP-NEXT:         storage i8, u8 [size=1, align=1];
// VFP-NEXT:         storage i16, u16 [size=2, align=2];
// VFP-NEXT:         storage i32, u32 [size=4, align=4];
// VFP-NEXT:         storage i64, u64 [size=8, align=8];
// VFP-NEXT:         storage bf16 [size=2, align=2];
// VFP-NEXT:         storage f16 [size=2, align=2];
// VFP-NEXT:         storage f32 [size=4, align=4];
// VFP-NEXT:         storage f64 [size=8, align=8];
// VFP-NEXT:         storage f128 [size=16, align=16];
// VFP-NEXT:         storage d32 [size=4, align=4];
// VFP-NEXT:         storage d64 [size=8, align=8];
// VFP-NEXT:         storage d128 [size=16, align=16];
// VFP-NEXT:     }
// VFP-NEXT:     global %[[VALUE_has_fp:[0-9]+]] has_fp: i32 [storage=static] [linkage=external];
// VFP-NEXT:     global %[[VALUE_has_vfpv2:[0-9]+]] has_vfpv2: i32 [storage=static] [linkage=external];
// VFP-NEXT: }
// SLATE-FILECHECK-END VFP
// SLATE-FILECHECK-BEGIN NO-FP
// NO-FP: module {
// NO-FP-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// NO-FP-NEXT:         endian = little;
// NO-FP-NEXT:         pointer [size=4, align=4];
// NO-FP-NEXT:         stack_alignment = 8;
// NO-FP-NEXT:         long_double = f64;
// NO-FP-NEXT:         storage bool [size=1, align=1];
// NO-FP-NEXT:         storage i8, u8 [size=1, align=1];
// NO-FP-NEXT:         storage i16, u16 [size=2, align=2];
// NO-FP-NEXT:         storage i32, u32 [size=4, align=4];
// NO-FP-NEXT:         storage i64, u64 [size=8, align=8];
// NO-FP-NEXT:         storage bf16 [size=2, align=2];
// NO-FP-NEXT:         storage f16 [size=2, align=2];
// NO-FP-NEXT:         storage f32 [size=4, align=4];
// NO-FP-NEXT:         storage f64 [size=8, align=8];
// NO-FP-NEXT:         storage f128 [size=16, align=16];
// NO-FP-NEXT:         storage d32 [size=4, align=4];
// NO-FP-NEXT:         storage d64 [size=8, align=8];
// NO-FP-NEXT:         storage d128 [size=16, align=16];
// NO-FP-NEXT:     }
// NO-FP-NEXT: }
// SLATE-FILECHECK-END NO-FP
// SLATE-FILECHECK-BEGIN NO-FP-FP
// NO-FP-FP: module {
// NO-FP-FP-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// NO-FP-FP-NEXT:         endian = little;
// NO-FP-FP-NEXT:         pointer [size=4, align=4];
// NO-FP-FP-NEXT:         stack_alignment = 8;
// NO-FP-FP-NEXT:         long_double = f64;
// NO-FP-FP-NEXT:         storage bool [size=1, align=1];
// NO-FP-FP-NEXT:         storage i8, u8 [size=1, align=1];
// NO-FP-FP-NEXT:         storage i16, u16 [size=2, align=2];
// NO-FP-FP-NEXT:         storage i32, u32 [size=4, align=4];
// NO-FP-FP-NEXT:         storage i64, u64 [size=8, align=8];
// NO-FP-FP-NEXT:         storage bf16 [size=2, align=2];
// NO-FP-FP-NEXT:         storage f16 [size=2, align=2];
// NO-FP-FP-NEXT:         storage f32 [size=4, align=4];
// NO-FP-FP-NEXT:         storage f64 [size=8, align=8];
// NO-FP-FP-NEXT:         storage f128 [size=16, align=16];
// NO-FP-FP-NEXT:         storage d32 [size=4, align=4];
// NO-FP-FP-NEXT:         storage d64 [size=8, align=8];
// NO-FP-FP-NEXT:         storage d128 [size=16, align=16];
// NO-FP-FP-NEXT:     }
// NO-FP-FP-NEXT:     global %[[VALUE_has_fp:[0-9]+]] has_fp: i32 [storage=static] [linkage=external];
// NO-FP-FP-NEXT:     global %[[VALUE_has_vfpv2:[0-9]+]] has_vfpv2: i32 [storage=static] [linkage=external];
// NO-FP-FP-NEXT:     global %[[VALUE_has_vfpv3:[0-9]+]] has_vfpv3: i32 [storage=static] [linkage=external];
// NO-FP-FP-NEXT:     global %[[VALUE_has_neon:[0-9]+]] has_neon: i32 [storage=static] [linkage=external];
// NO-FP-FP-NEXT: }
// SLATE-FILECHECK-END NO-FP-FP
// SLATE-FILECHECK-BEGIN NO-SIMD
// NO-SIMD: module {
// NO-SIMD-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// NO-SIMD-NEXT:         endian = little;
// NO-SIMD-NEXT:         pointer [size=4, align=4];
// NO-SIMD-NEXT:         stack_alignment = 8;
// NO-SIMD-NEXT:         long_double = f64;
// NO-SIMD-NEXT:         storage bool [size=1, align=1];
// NO-SIMD-NEXT:         storage i8, u8 [size=1, align=1];
// NO-SIMD-NEXT:         storage i16, u16 [size=2, align=2];
// NO-SIMD-NEXT:         storage i32, u32 [size=4, align=4];
// NO-SIMD-NEXT:         storage i64, u64 [size=8, align=8];
// NO-SIMD-NEXT:         storage bf16 [size=2, align=2];
// NO-SIMD-NEXT:         storage f16 [size=2, align=2];
// NO-SIMD-NEXT:         storage f32 [size=4, align=4];
// NO-SIMD-NEXT:         storage f64 [size=8, align=8];
// NO-SIMD-NEXT:         storage f128 [size=16, align=16];
// NO-SIMD-NEXT:         storage d32 [size=4, align=4];
// NO-SIMD-NEXT:         storage d64 [size=8, align=8];
// NO-SIMD-NEXT:         storage d128 [size=16, align=16];
// NO-SIMD-NEXT:     }
// NO-SIMD-NEXT:     global %[[VALUE_has_fp:[0-9]+]] has_fp: i32 [storage=static] [linkage=external];
// NO-SIMD-NEXT:     global %[[VALUE_has_vfpv2:[0-9]+]] has_vfpv2: i32 [storage=static] [linkage=external];
// NO-SIMD-NEXT:     global %[[VALUE_has_vfpv3:[0-9]+]] has_vfpv3: i32 [storage=static] [linkage=external];
// NO-SIMD-NEXT: }
// SLATE-FILECHECK-END NO-SIMD
// SLATE-FILECHECK-BEGIN NO-FP-EXPLICIT-FPU
// NO-FP-EXPLICIT-FPU: module {
// NO-FP-EXPLICIT-FPU-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// NO-FP-EXPLICIT-FPU-NEXT:         endian = little;
// NO-FP-EXPLICIT-FPU-NEXT:         pointer [size=4, align=4];
// NO-FP-EXPLICIT-FPU-NEXT:         stack_alignment = 8;
// NO-FP-EXPLICIT-FPU-NEXT:         long_double = f64;
// NO-FP-EXPLICIT-FPU-NEXT:         storage bool [size=1, align=1];
// NO-FP-EXPLICIT-FPU-NEXT:         storage i8, u8 [size=1, align=1];
// NO-FP-EXPLICIT-FPU-NEXT:         storage i16, u16 [size=2, align=2];
// NO-FP-EXPLICIT-FPU-NEXT:         storage i32, u32 [size=4, align=4];
// NO-FP-EXPLICIT-FPU-NEXT:         storage i64, u64 [size=8, align=8];
// NO-FP-EXPLICIT-FPU-NEXT:         storage bf16 [size=2, align=2];
// NO-FP-EXPLICIT-FPU-NEXT:         storage f16 [size=2, align=2];
// NO-FP-EXPLICIT-FPU-NEXT:         storage f32 [size=4, align=4];
// NO-FP-EXPLICIT-FPU-NEXT:         storage f64 [size=8, align=8];
// NO-FP-EXPLICIT-FPU-NEXT:         storage f128 [size=16, align=16];
// NO-FP-EXPLICIT-FPU-NEXT:         storage d32 [size=4, align=4];
// NO-FP-EXPLICIT-FPU-NEXT:         storage d64 [size=8, align=8];
// NO-FP-EXPLICIT-FPU-NEXT:         storage d128 [size=16, align=16];
// NO-FP-EXPLICIT-FPU-NEXT:     }
// NO-FP-EXPLICIT-FPU-NEXT:     global %[[VALUE_has_fp:[0-9]+]] has_fp: i32 [storage=static] [linkage=external];
// NO-FP-EXPLICIT-FPU-NEXT:     global %[[VALUE_has_vfpv2:[0-9]+]] has_vfpv2: i32 [storage=static] [linkage=external];
// NO-FP-EXPLICIT-FPU-NEXT:     global %[[VALUE_has_vfpv3:[0-9]+]] has_vfpv3: i32 [storage=static] [linkage=external];
// NO-FP-EXPLICIT-FPU-NEXT:     global %[[VALUE_has_neon:[0-9]+]] has_neon: i32 [storage=static] [linkage=external];
// NO-FP-EXPLICIT-FPU-NEXT: }
// SLATE-FILECHECK-END NO-FP-EXPLICIT-FPU
