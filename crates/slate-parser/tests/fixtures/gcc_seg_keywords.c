// SLATE-FILECHECK-DEFINES X86-64 SEG
// SLATE-FILECHECK-PREFIX-ARGS X86-64 --target=x86_64-unknown-linux-gnu --flavor=gcc
// SLATE-FILECHECK-DEFINES I686 SEG
// SLATE-FILECHECK-PREFIX-ARGS I686 --target=i686-unknown-linux-gnu --flavor=gcc
// SLATE-FILECHECK-DEFINES STRICT
// SLATE-FILECHECK-PREFIX-ARGS STRICT --target=x86_64-unknown-linux-gnu --flavor=gcc -std=c11
// SLATE-FILECHECK-DEFINES AARCH64
// SLATE-FILECHECK-PREFIX-ARGS AARCH64 --target=aarch64-unknown-linux-gnu --flavor=gcc

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
// X86-64-NEXT:     type @type0 T = struct {
// X86-64-NEXT:         field0 x: i32;
// X86-64-NEXT:     } [size=4, align=4, offsets=[0]];
// X86-64-NEXT:     global %1 p: ptr<i32> [storage=static] [linkage=external];
// X86-64-NEXT:     global %2 q: ptr<const i32> [storage=static] [linkage=external];
// X86-64-NEXT:     global %3 r: ptr<i32> [storage=static] [linkage=external];
// X86-64-NEXT:     fn %4 @f(%5 t: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// X86-64-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%5))));
// X86-64-NEXT:     }
// X86-64-NEXT: }
// SLATE-FILECHECK-END X86-64
// SLATE-FILECHECK-BEGIN I686
// I686: module {
// I686-NEXT:     target "i686-unknown-linux-gnu" {
// I686-NEXT:         endian = little;
// I686-NEXT:         pointer [size=4, align=4];
// I686-NEXT:         stack_alignment = 16;
// I686-NEXT:         long_double = f80;
// I686-NEXT:         storage bool [size=1, align=1];
// I686-NEXT:         storage i8, u8 [size=1, align=1];
// I686-NEXT:         storage i16, u16 [size=2, align=2];
// I686-NEXT:         storage i32, u32 [size=4, align=4];
// I686-NEXT:         storage i64, u64 [size=8, align=4];
// I686-NEXT:         storage i128, u128 [size=16, align=16];
// I686-NEXT:         storage bf16 [size=2, align=2];
// I686-NEXT:         storage f16 [size=2, align=2];
// I686-NEXT:         storage f32 [size=4, align=4];
// I686-NEXT:         storage f64 [size=8, align=4];
// I686-NEXT:         storage f80 [size=12, align=4];
// I686-NEXT:         storage f128 [size=16, align=16];
// I686-NEXT:         storage d32 [size=4, align=4];
// I686-NEXT:         storage d64 [size=8, align=8];
// I686-NEXT:         storage d128 [size=16, align=16];
// I686-NEXT:     }
// I686-NEXT:     type @type0 T = struct {
// I686-NEXT:         field0 x: i32;
// I686-NEXT:     } [size=4, align=4, offsets=[0]];
// I686-NEXT:     global %1 p: ptr<i32> [storage=static] [linkage=external];
// I686-NEXT:     global %2 q: ptr<const i32> [storage=static] [linkage=external];
// I686-NEXT:     global %3 r: ptr<i32> [storage=static] [linkage=external];
// I686-NEXT:     fn %4 @f(%5 t: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// I686-NEXT:         return read<i32>(field0(deref(read<ptr<@type0>>(%5))));
// I686-NEXT:     }
// I686-NEXT: }
// SLATE-FILECHECK-END I686
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
// STRICT-NEXT:     global %0 __seg_fs: i32 [storage=static] [linkage=external];
// STRICT-NEXT:     global %1 __seg_gs: i32 [storage=static] [linkage=external];
// STRICT-NEXT: }
// SLATE-FILECHECK-END STRICT
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
// AARCH64-NEXT:     global %0 __seg_fs: i32 [storage=static] [linkage=external];
// AARCH64-NEXT:     global %1 __seg_gs: i32 [storage=static] [linkage=external];
// AARCH64-NEXT: }
// SLATE-FILECHECK-END AARCH64
