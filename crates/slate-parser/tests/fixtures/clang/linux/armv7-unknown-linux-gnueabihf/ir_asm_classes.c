// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23
void classes(int x, float f, double d) {
    asm("@ %0 %1 %2" : "=r"(x), "=t"(f), "=w"(d));
    asm("@ %0 %1 %2" : : "I"(1), "Q"(x), "l"(x));
}

typedef float v4 __attribute__((vector_size(16)));

void vfp(float f, double d, v4 q) {
    asm("@ %0 %1 %2" : "=x"(f), "=t"(d), "=x"(d));
    asm("@ %0 %1 %2" : "=w"(q), "=t"(q), "=x"(q));
}

void memory(int *p) {
    asm("@ %0 %1 %2 %3" : : "Uv"(*p), "Uy"(*p), "Uq"(*p), "Ut"(*p));
}

void pinned(void) {
    register int r asm("r4");
    register float s asm("s3");
    asm("@ %0 %1" : "=r"(r), "=t"(s));
}

void clobbers(void) {
    asm volatile("" ::: "r0", "a2", "v1", "sb", "sl", "fp", "ip", "lr", "s31", "d31", "q15");
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "armv7-unknown-linux-gnueabihf" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 8;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
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
// IR-NEXT:     fn %[[VALUE_classes:[0-9]+]] @classes(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_f:[0-9]+]] f: f32, %[[VALUE_d:[0-9]+]] d: f64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "@ %0 %1 %2" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "@ " %0 " " %1 " " %2;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             lateout 1 "t" [sreg] width 32 place<f32>(%[[VALUE_f]]);
// IR-NEXT:             lateout 2 "w" [dreg] width 64 place<f64>(%[[VALUE_d]]);
// IR-NEXT:         }
// IR-NEXT:         asm "@ %0 %1 %2" [options=nostack,preserves_flags] {
// IR-NEXT:             template: "@ " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "I" [imm] width 32 const<i32>(1);
// IR-NEXT:             in 1 "Q" [mem] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 2 "l" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vfp:[0-9]+]] @vfp(%[[VALUE_f_2:[0-9]+]] f: f32, %[[VALUE_d_2:[0-9]+]] d: f64, %[[VALUE_q:[0-9]+]] q: vector<f32, 4> [c="v4"]) -> void [linkage=external] [abi=aapcs32_hard_float(scalar, scalar, direct) -> void] [fallthrough=ret_void] [c="void(float, double, v4)"] {
// IR-NEXT:         asm "@ %0 %1 %2" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "@ " %0 " " %1 " " %2;
// IR-NEXT:             lateout 0 "x" [sreg_low16] width 32 place<f32>(%[[VALUE_f_2]]);
// IR-NEXT:             lateout 1 "t" [dreg_low16] width 64 place<f64>(%[[VALUE_d_2]]);
// IR-NEXT:             lateout 2 "x" [dreg_low8] width 64 place<f64>(%[[VALUE_d_2]]);
// IR-NEXT:         }
// IR-NEXT:         asm "@ %0 %1 %2" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "@ " %0 " " %1 " " %2;
// IR-NEXT:             lateout 0 "w" [qreg] width 128 place<vector<f32, 4>>(%[[VALUE_q]]);
// IR-NEXT:             lateout 1 "t" [qreg_low8] width 128 place<vector<f32, 4>>(%[[VALUE_q]]);
// IR-NEXT:             lateout 2 "x" [qreg_low4] width 128 place<vector<f32, 4>>(%[[VALUE_q]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_memory:[0-9]+]] @memory(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "@ %0 %1 %2 %3" [options=nostack,preserves_flags] {
// IR-NEXT:             template: "@ " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "Uv" [mem] width 32 place<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// IR-NEXT:             in 1 "Uy" [mem] width 32 place<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// IR-NEXT:             in 2 "Uq" [mem] width 32 place<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// IR-NEXT:             in 3 "Ut" [mem] width 32 place<i32>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_pinned:[0-9]+]] @pinned() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] [register="r4"];
// IR-NEXT:         let %[[VALUE_s:[0-9]+]] s: f32 [storage=automatic] [register="s3"];
// IR-NEXT:         asm "@ %0 %1" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "@ " %0 " " %1;
// IR-NEXT:             lateout 0 "r" [reg] -> {r4} width 32 place<i32>(%[[VALUE_r]]);
// IR-NEXT:             lateout 1 "t" [sreg] -> {s3} width 32 place<f32>(%[[VALUE_s]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_clobbers:[0-9]+]] @clobbers() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile "" [options=nostack,preserves_flags] {
// IR-NEXT:             clobbers: "r0" as r0, "a2" as r1, "v1" as r4, "sb" as r9, "sl" as r10, "fp" as r11, "ip" as r12, "lr" as r14, "s31" as s31, "d31" as d31, "q15" as q15;
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
