// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir -march=tigerlake

#ifdef __SGX__
int has__SGX__;
#endif
#ifdef __KL__
int has__KL__;
#endif
#ifdef __WIDEKL__
int has__WIDEKL__;
#endif
#ifdef __AVX512VP2INTERSECT__
int has__AVX512VP2INTERSECT__;
#endif
#ifdef __MOVDIRI__
int has__MOVDIRI__;
#endif
#ifdef __icelake_client__
int has__icelake_client__;
#endif
#ifdef __tigerlake__
int has__tigerlake__;
#endif
#ifdef __tune_tigerlake__
int has__tune_tigerlake__;
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
// CHECK-NEXT:     global %[[VALUE_has__SGX__:[0-9]+]] has__SGX__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__KL__:[0-9]+]] has__KL__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__WIDEKL__:[0-9]+]] has__WIDEKL__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__AVX512VP2INTERSECT__:[0-9]+]] has__AVX512VP2INTERSECT__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__MOVDIRI__:[0-9]+]] has__MOVDIRI__: i32 [storage=static] [linkage=external];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
