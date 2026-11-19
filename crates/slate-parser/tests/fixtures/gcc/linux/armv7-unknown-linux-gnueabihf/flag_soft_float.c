// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir -msoft-float

#ifdef __SOFTFP__
int val__SOFTFP__[__SOFTFP__];
#endif
#ifdef __ARM_PCS_VFP
int val__ARM_PCS_VFP[__ARM_PCS_VFP];
#endif
#ifdef __ARM_PCS
int val__ARM_PCS[__ARM_PCS];
#endif

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: module {
// CHECK-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// CHECK-NEXT:         endian = little;
// CHECK-NEXT:         pointer [size=4, align=4];
// CHECK-NEXT:         stack_alignment = 8;
// CHECK-NEXT:         long_double = f64;
// CHECK-NEXT:         storage bool [size=1, align=1];
// CHECK-NEXT:         storage i8, u8 [size=1, align=1];
// CHECK-NEXT:         storage i16, u16 [size=2, align=2];
// CHECK-NEXT:         storage i32, u32 [size=4, align=4];
// CHECK-NEXT:         storage i64, u64 [size=8, align=8];
// CHECK-NEXT:         storage bf16 [size=2, align=2];
// CHECK-NEXT:         storage f16 [size=2, align=2];
// CHECK-NEXT:         storage f32 [size=4, align=4];
// CHECK-NEXT:         storage f64 [size=8, align=8];
// CHECK-NEXT:         storage f128 [size=16, align=16];
// CHECK-NEXT:         storage d32 [size=4, align=4];
// CHECK-NEXT:         storage d64 [size=8, align=8];
// CHECK-NEXT:         storage d128 [size=16, align=16];
// CHECK-NEXT:     }
// CHECK-NEXT:     global %[[VALUE_val__SOFTFP__:[0-9]+]] val__SOFTFP__: array<i32, 1> [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_val__ARM_PCS:[0-9]+]] val__ARM_PCS: array<i32, 1> [storage=static] [linkage=external];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
