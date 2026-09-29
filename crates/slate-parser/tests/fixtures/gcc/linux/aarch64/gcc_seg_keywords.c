// SLATE-FILECHECK-DEFINES AARCH64

#ifdef SEG
struct T { int x; };
int __seg_gs *p;
__seg_fs const int *q;
int *__seg_fs r;
int f(__seg_gs struct T *t) { return t->x; }
#else
int __seg_fs, __seg_gs;
#endif

// SLATE-FILECHECK-BEGIN AARCH64
// AARCH64: module {
// AARCH64-NEXT:     target "aarch64-unknown-linux-gnu" {
// AARCH64-NEXT:         endian = little;
// AARCH64-NEXT:         pointer [size=8, align=8];
// AARCH64-NEXT:         stack_alignment = 16;
// AARCH64-NEXT:         long_double = f128;
// AARCH64-NEXT:         storage bool [size=1, align=1];
// AARCH64-NEXT:         storage i8, u8 [size=1, align=1];
// AARCH64-NEXT:         storage i16, u16 [size=2, align=2];
// AARCH64-NEXT:         storage i32, u32 [size=4, align=4];
// AARCH64-NEXT:         storage i64, u64 [size=8, align=8];
// AARCH64-NEXT:         storage i128, u128 [size=16, align=16];
// AARCH64-NEXT:         storage bf16 [size=2, align=2];
// AARCH64-NEXT:         storage f16 [size=2, align=2];
// AARCH64-NEXT:         storage f32 [size=4, align=4];
// AARCH64-NEXT:         storage f64 [size=8, align=8];
// AARCH64-NEXT:         storage f128 [size=16, align=16];
// AARCH64-NEXT:         storage d32 [size=4, align=4];
// AARCH64-NEXT:         storage d64 [size=8, align=8];
// AARCH64-NEXT:         storage d128 [size=16, align=16];
// AARCH64-NEXT:     }
// AARCH64-NEXT:     global %[[VALUE___seg_fs:[0-9]+]] __seg_fs: i32 [storage=static] [linkage=external];
// AARCH64-NEXT:     global %[[VALUE___seg_gs:[0-9]+]] __seg_gs: i32 [storage=static] [linkage=external];
// AARCH64-NEXT: }
// SLATE-FILECHECK-END AARCH64
