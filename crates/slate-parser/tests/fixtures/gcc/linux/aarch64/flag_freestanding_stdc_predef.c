// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES HOSTED
// SLATE-FILECHECK-DEFINES FREESTANDING
// SLATE-FILECHECK-PREFIX-ARGS FREESTANDING -ffreestanding

#ifdef __STDC_IEC_559__
int val__STDC_IEC_559__;
#endif
#ifdef __STDC_IEC_559_COMPLEX__
int val__STDC_IEC_559_COMPLEX__;
#endif
#ifdef __STDC_IEC_60559_BFP__
int val__STDC_IEC_60559_BFP__;
#endif
#ifdef __STDC_IEC_60559_COMPLEX__
int val__STDC_IEC_60559_COMPLEX__;
#endif
#ifdef _STDC_PREDEF_H
int val_STDC_PREDEF_H;
#endif

// SLATE-FILECHECK-BEGIN HOSTED
// HOSTED: module {
// HOSTED-NEXT:     target "aarch64-unknown-linux-gnu" {
// HOSTED-NEXT:         endian = little;
// HOSTED-NEXT:         pointer [size=8, align=8];
// HOSTED-NEXT:         stack_alignment = 16;
// HOSTED-NEXT:         long_double = f128;
// HOSTED-NEXT:         storage bool [size=1, align=1];
// HOSTED-NEXT:         storage i8, u8 [size=1, align=1];
// HOSTED-NEXT:         storage i16, u16 [size=2, align=2];
// HOSTED-NEXT:         storage i32, u32 [size=4, align=4];
// HOSTED-NEXT:         storage i64, u64 [size=8, align=8];
// HOSTED-NEXT:         storage i128, u128 [size=16, align=16];
// HOSTED-NEXT:         storage bf16 [size=2, align=2];
// HOSTED-NEXT:         storage f16 [size=2, align=2];
// HOSTED-NEXT:         storage f32 [size=4, align=4];
// HOSTED-NEXT:         storage f64 [size=8, align=8];
// HOSTED-NEXT:         storage f128 [size=16, align=16];
// HOSTED-NEXT:         storage d32 [size=4, align=4];
// HOSTED-NEXT:         storage d64 [size=8, align=8];
// HOSTED-NEXT:         storage d128 [size=16, align=16];
// HOSTED-NEXT:     }
// HOSTED-NEXT:     global %[[VALUE_val__STDC_IEC_559__:[0-9]+]] val__STDC_IEC_559__: i32 [storage=static] [linkage=external];
// HOSTED-NEXT:     global %[[VALUE_val__STDC_IEC_559_COMPLEX__:[0-9]+]] val__STDC_IEC_559_COMPLEX__: i32 [storage=static] [linkage=external];
// HOSTED-NEXT:     global %[[VALUE_val__STDC_IEC_60559_BFP__:[0-9]+]] val__STDC_IEC_60559_BFP__: i32 [storage=static] [linkage=external];
// HOSTED-NEXT:     global %[[VALUE_val__STDC_IEC_60559_COMPLEX__:[0-9]+]] val__STDC_IEC_60559_COMPLEX__: i32 [storage=static] [linkage=external];
// HOSTED-NEXT:     global %[[VALUE_val_STDC_PREDEF_H:[0-9]+]] val_STDC_PREDEF_H: i32 [storage=static] [linkage=external];
// HOSTED-NEXT: }
// SLATE-FILECHECK-END HOSTED
// SLATE-FILECHECK-BEGIN FREESTANDING
// FREESTANDING: module {
// FREESTANDING-NEXT:     target "aarch64-unknown-linux-gnu" {
// FREESTANDING-NEXT:         endian = little;
// FREESTANDING-NEXT:         pointer [size=8, align=8];
// FREESTANDING-NEXT:         stack_alignment = 16;
// FREESTANDING-NEXT:         long_double = f128;
// FREESTANDING-NEXT:         storage bool [size=1, align=1];
// FREESTANDING-NEXT:         storage i8, u8 [size=1, align=1];
// FREESTANDING-NEXT:         storage i16, u16 [size=2, align=2];
// FREESTANDING-NEXT:         storage i32, u32 [size=4, align=4];
// FREESTANDING-NEXT:         storage i64, u64 [size=8, align=8];
// FREESTANDING-NEXT:         storage i128, u128 [size=16, align=16];
// FREESTANDING-NEXT:         storage bf16 [size=2, align=2];
// FREESTANDING-NEXT:         storage f16 [size=2, align=2];
// FREESTANDING-NEXT:         storage f32 [size=4, align=4];
// FREESTANDING-NEXT:         storage f64 [size=8, align=8];
// FREESTANDING-NEXT:         storage f128 [size=16, align=16];
// FREESTANDING-NEXT:         storage d32 [size=4, align=4];
// FREESTANDING-NEXT:         storage d64 [size=8, align=8];
// FREESTANDING-NEXT:         storage d128 [size=16, align=16];
// FREESTANDING-NEXT:     }
// FREESTANDING-NEXT: }
// SLATE-FILECHECK-END FREESTANDING
