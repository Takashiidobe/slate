// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir -mpclmul -mamx-avx512 -msha -mno-avx10.2

#ifdef __PCLMUL__
int has__PCLMUL__;
#endif
#ifdef __SSE2__
int has__SSE2__;
#endif
#ifdef __SSE4_1__
int has__SSE4_1__;
#endif
#ifdef __SHA__
int has__SHA__;
#endif
#ifdef __AMX_TILE__
int has__AMX_TILE__;
#endif
#ifdef __AMX_AVX512__
int has__AMX_AVX512__;
#endif
#ifdef __AVX10_2__
int has__AVX10_2__;
#endif
#ifdef __AVX10_1__
int has__AVX10_1__;
#endif
#ifdef __AVX512F__
int has__AVX512F__;
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
// CHECK-NEXT:     global %[[VALUE_has__PCLMUL__:[0-9]+]] has__PCLMUL__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE2__:[0-9]+]] has__SSE2__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SSE4_1__:[0-9]+]] has__SSE4_1__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__SHA__:[0-9]+]] has__SHA__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__AMX_TILE__:[0-9]+]] has__AMX_TILE__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__AVX10_1__:[0-9]+]] has__AVX10_1__: i32 [storage=static] [linkage=external];
// CHECK-NEXT:     global %[[VALUE_has__AVX512F__:[0-9]+]] has__AVX512F__: i32 [storage=static] [linkage=external];
// CHECK-NEXT: }
// SLATE-FILECHECK-END CHECK
