// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23
typedef float v4 __attribute__((vector_size(16)));

long syscall1(long number, long a) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = a;
    __asm__ volatile("svc #0" : "+r"(x0) : "r"(x8) : "memory");
    return x0;
}

int narrow(int a) {
    register int w __asm__("w1") = a;
    __asm__("" : "+r"(w));
    return w;
}

double scalar(double a) {
    register double d __asm__("d3") = a;
    __asm__("" : "+w"(d));
    return d;
}

v4 low16(v4 a) {
    register v4 q __asm__("v15") = a;
    __asm__("" : "+x"(q));
    return q;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_v4:[0-9]+]] v4 = vector<f32, 4>;
// IR-NEXT:     fn %[[VALUE_syscall1:[0-9]+]] @syscall1(%[[VALUE_number:[0-9]+]] number: i64, %[[VALUE_a:[0-9]+]] a: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_x8:[0-9]+]] x8: i64 [storage=automatic] [register="x8"] = read<i64>(%[[VALUE_number]]);
// IR-NEXT:         let %[[VALUE_x0:[0-9]+]] x0: i64 [storage=automatic] [register="x0"] = read<i64>(%[[VALUE_a]]);
// IR-NEXT:         asm volatile "svc #[[#NODE0:]]" [options=nostack,preserves_flags] {
// IR-NEXT:             template: "svc #[[#NODE0]]";
// IR-NEXT:             inlateout 0 "r" [reg] -> {x0} width 64 place<i64>(%[[VALUE_x0]]);
// IR-NEXT:             in 1 "r" [reg] -> {x8} width 64 read<i64>(%[[VALUE_x8]]);
// IR-NEXT:             clobbers: memory;
// IR-NEXT:         }
// IR-NEXT:         return read<i64>(%[[VALUE_x0]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_narrow:[0-9]+]] @narrow(%[[VALUE_a_2:[0-9]+]] a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_w:[0-9]+]] w: i32 [storage=automatic] [register="w1"] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         asm "" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             inlateout 0 "r" [reg] -> {w1} width 32 place<i32>(%[[VALUE_w]]);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_w]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_scalar:[0-9]+]] @scalar(%[[VALUE_a_3:[0-9]+]] a: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic] [register="d3"] = read<f64>(%[[VALUE_a_3]]);
// IR-NEXT:         asm "" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             inlateout 0 "w" [vreg] -> {d3} width 64 place<f64>(%[[VALUE_d]]);
// IR-NEXT:         }
// IR-NEXT:         return read<f64>(%[[VALUE_d]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_low16:[0-9]+]] @low16(%[[VALUE_a_4:[0-9]+]] a: vector<f32, 4>) -> vector<f32, 4> [linkage=external] [abi=aapcs64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: vector<f32, 4> [storage=automatic] [register="v15"] = read<vector<f32, 4>>(%[[VALUE_a_4]]);
// IR-NEXT:         asm "" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             inlateout 0 "x" [vreg_low16] -> {v15} width 128 place<vector<f32, 4>>(%[[VALUE_q]]);
// IR-NEXT:         }
// IR-NEXT:         return read<vector<f32, 4>>(%[[VALUE_q]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
