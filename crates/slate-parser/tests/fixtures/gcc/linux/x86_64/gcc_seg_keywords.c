// SLATE-FILECHECK-DEFINES X86-64 SEG
// SLATE-FILECHECK-DEFINES STRICT
// SLATE-FILECHECK-PREFIX-ARGS STRICT -std=c11

#ifdef SEG
struct T { int x; };
int __seg_gs *p;
__seg_fs const int *q;
int *__seg_fs r;
int f(__seg_gs struct T *t) { return t->x; }
#else
int __seg_fs, __seg_gs;
#endif

// SLATE-FILECHECK-BEGIN X86-64
// X86-64: module {
// X86-64-NEXT:     target "x86_64-unknown-linux-gnu" {
// X86-64-NEXT:         endian = little;
// X86-64-NEXT:         pointer [size=8, align=8];
// X86-64-NEXT:         stack_alignment = 16;
// X86-64-NEXT:         long_double = f80;
// X86-64-NEXT:         storage bool [size=1, align=1];
// X86-64-NEXT:         storage i8, u8 [size=1, align=1];
// X86-64-NEXT:         storage i16, u16 [size=2, align=2];
// X86-64-NEXT:         storage i32, u32 [size=4, align=4];
// X86-64-NEXT:         storage i64, u64 [size=8, align=8];
// X86-64-NEXT:         storage i128, u128 [size=16, align=16];
// X86-64-NEXT:         storage bf16 [size=2, align=2];
// X86-64-NEXT:         storage f16 [size=2, align=2];
// X86-64-NEXT:         storage f32 [size=4, align=4];
// X86-64-NEXT:         storage f64 [size=8, align=8];
// X86-64-NEXT:         storage f80 [size=16, align=16];
// X86-64-NEXT:         storage f128 [size=16, align=16];
// X86-64-NEXT:         storage d32 [size=4, align=4];
// X86-64-NEXT:         storage d64 [size=8, align=8];
// X86-64-NEXT:         storage d128 [size=16, align=16];
// X86-64-NEXT:     }
// X86-64-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// X86-64-NEXT:         field0 x: i32;
// X86-64-NEXT:     } [size=4, align=4, offsets=[0]];
// X86-64-NEXT:     global %[[VALUE_p:[0-9]+]] p: ptr<i32> [storage=static] [linkage=external];
// X86-64-NEXT:     global %[[VALUE_q:[0-9]+]] q: ptr<const i32> [storage=static] [linkage=external];
// X86-64-NEXT:     global %[[VALUE_r:[0-9]+]] r: ptr<i32> [storage=static] [linkage=external];
// X86-64-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_t:[0-9]+]] t: ptr<@type[[TYPE_T]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// X86-64-NEXT:         return read<i32>(field0(deref(read<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]]))));
// X86-64-NEXT:     }
// X86-64-NEXT: }
// SLATE-FILECHECK-END X86-64
// SLATE-FILECHECK-BEGIN STRICT
// STRICT: module {
// STRICT-NEXT:     target "x86_64-unknown-linux-gnu" {
// STRICT-NEXT:         endian = little;
// STRICT-NEXT:         pointer [size=8, align=8];
// STRICT-NEXT:         stack_alignment = 16;
// STRICT-NEXT:         long_double = f80;
// STRICT-NEXT:         storage bool [size=1, align=1];
// STRICT-NEXT:         storage i8, u8 [size=1, align=1];
// STRICT-NEXT:         storage i16, u16 [size=2, align=2];
// STRICT-NEXT:         storage i32, u32 [size=4, align=4];
// STRICT-NEXT:         storage i64, u64 [size=8, align=8];
// STRICT-NEXT:         storage i128, u128 [size=16, align=16];
// STRICT-NEXT:         storage bf16 [size=2, align=2];
// STRICT-NEXT:         storage f16 [size=2, align=2];
// STRICT-NEXT:         storage f32 [size=4, align=4];
// STRICT-NEXT:         storage f64 [size=8, align=8];
// STRICT-NEXT:         storage f80 [size=16, align=16];
// STRICT-NEXT:         storage f128 [size=16, align=16];
// STRICT-NEXT:         storage d32 [size=4, align=4];
// STRICT-NEXT:         storage d64 [size=8, align=8];
// STRICT-NEXT:         storage d128 [size=16, align=16];
// STRICT-NEXT:     }
// STRICT-NEXT:     global %[[VALUE___seg_fs:[0-9]+]] __seg_fs: i32 [storage=static] [linkage=external];
// STRICT-NEXT:     global %[[VALUE___seg_gs:[0-9]+]] __seg_gs: i32 [storage=static] [linkage=external];
// STRICT-NEXT: }
// SLATE-FILECHECK-END STRICT
