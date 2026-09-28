// SLATE-FILECHECK-DEFINES I686 SEG

#ifdef SEG
struct T { int x; };
int __seg_gs *p;
__seg_fs const int *q;
int *__seg_fs r;
int f(__seg_gs struct T *t) { return t->x; }
#else
int __seg_fs, __seg_gs;
#endif

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
