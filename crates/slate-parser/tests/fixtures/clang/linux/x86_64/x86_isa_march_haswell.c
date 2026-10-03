// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir -march=haswell

#ifdef __AVX2__
int has__AVX2__;
#endif
#ifdef __BMI2__
int has__BMI2__;
#endif
#ifdef __FMA__
int has__FMA__;
#endif
#ifdef __PCLMUL__
int has__PCLMUL__;
#endif
#ifdef __AES__
int has__AES__;
#endif
#ifdef __RDRND__
int has__RDRND__;
#endif
#ifdef __FSGSBASE__
int has__FSGSBASE__;
#endif
#ifdef __INVPCID__
int has__INVPCID__;
#endif
#ifdef __SGX__
int has__SGX__;
#endif
#ifdef __AVX512F__
int has__AVX512F__;
#endif
#ifdef __corei7
int has__corei7;
#endif
#ifdef __corei7__
int has__corei7__;
#endif
#ifdef __tune_corei7__
int has__tune_corei7__;
#endif
#ifdef __haswell__
int has__haswell__;
#endif
#ifdef __k8__
int has__k8__;
#endif

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
// CHECK-NEXT:     global %[[VALUE_has__AVX2__:[0-9]+]] has__AVX2__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__BMI2__:[0-9]+]] has__BMI2__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__FMA__:[0-9]+]] has__FMA__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__PCLMUL__:[0-9]+]] has__PCLMUL__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__RDRND__:[0-9]+]] has__RDRND__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__FSGSBASE__:[0-9]+]] has__FSGSBASE__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__INVPCID__:[0-9]+]] has__INVPCID__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__corei7:[0-9]+]] has__corei7: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__corei7__:[0-9]+]] has__corei7__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__tune_corei7__:[0-9]+]] has__tune_corei7__: i32 [storage=static] [linkage=external];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
