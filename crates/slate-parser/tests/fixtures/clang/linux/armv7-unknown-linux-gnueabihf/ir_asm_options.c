// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
int keeps_flags(int x) {
    int r;
    asm("mov %0, %1" : "=r"(r) : "r"(x));
    return r;
}

int clobbers_flags(int x) {
    int r;
    asm("adds %0, %1, 1" : "=r"(r) : "r"(x) : "cc");
    return r;
}

int reads_memory(int x) {
    int r;
    asm("ldr %0, %1" : "=r"(r) : "m"(x));
    return r;
}

void basic(void) {
    asm("nop");
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
// IR-NEXT:     fn %[[VALUE_keeps_flags:[0-9]+]] @keeps_flags(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// IR-NEXT:         asm "mov %0, %1" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "mov " %0 ", " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_r]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_r]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_clobbers_flags:[0-9]+]] @clobbers_flags(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: i32 [storage=automatic];
// IR-NEXT:         asm "adds %0, %1, 1" [options=pure,nomem,nostack] {
// IR-NEXT:             template: "adds " %0 ", " %1 ", 1";
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_r_2]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x_2]]);
// IR-NEXT:             clobbers: cc;
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_r_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_reads_memory:[0-9]+]] @reads_memory(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: i32 [storage=automatic];
// IR-NEXT:         asm "ldr %0, %1" [options=pure,readonly,nostack,preserves_flags] {
// IR-NEXT:             template: "ldr " %0 ", " %1;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_r_3]]);
// IR-NEXT:             in 1 "m" [mem] width 32 place<i32>(%[[VALUE_x_3]]);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_r_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_basic:[0-9]+]] @basic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "nop" [options=nostack,preserves_flags];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
