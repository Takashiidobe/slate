// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
int g;

int pure_nomem(int x) {
    int r;
    asm("mov %1, %0" : "=r"(r) : "r"(x));
    return r;
}

int volatile_nomem(int x) {
    int r;
    asm volatile("mov %1, %0" : "=r"(r) : "r"(x));
    return r;
}

int memory_clobber(int x) {
    int r;
    asm("mov %1, %0" : "=r"(r) : "r"(x) : "memory");
    return r;
}

int memory_input(int x) {
    int r;
    asm("mov %1, %0" : "=r"(r) : "m"(x));
    return r;
}

void memory_output(int x) {
    asm("mov %1, %0" : "=m"(g) : "r"(x));
}

void no_outputs(int x) {
    asm("# %0" : : "r"(x));
}

int with_goto(int x) {
    int r;
    asm goto("mov %1, %0" : "=r"(r) : "r"(x) : : out);
    return r;
out:
    return 0;
}

void basic(void) {
    asm("nop");
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
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
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_pure_nomem:[0-9]+]] @pure_nomem(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// IR-NEXT:         asm "mov %1, %0" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_r]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_r]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_volatile_nomem:[0-9]+]] @volatile_nomem(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: i32 [storage=automatic];
// IR-NEXT:         asm volatile "mov %1, %0" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_r_2]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x_2]]);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_r_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_memory_clobber:[0-9]+]] @memory_clobber(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: i32 [storage=automatic];
// IR-NEXT:         asm "mov %1, %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_r_3]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x_3]]);
// IR-NEXT:             clobbers: memory;
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_r_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_memory_input:[0-9]+]] @memory_input(%[[VALUE_x_4:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: i32 [storage=automatic];
// IR-NEXT:         asm "mov %1, %0" [dialect=att] [options=pure,readonly,nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_r_4]]);
// IR-NEXT:             in 1 "m" [mem] width 32 place<i32>(%[[VALUE_x_4]]);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_r_4]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_memory_output:[0-9]+]] @memory_output(%[[VALUE_x_5:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "mov %1, %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "m" [mem] width 32 place<i32>(%[[VALUE_g]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x_5]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_no_outputs:[0-9]+]] @no_outputs(%[[VALUE_x_6:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%[[VALUE_x_6]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_with_goto:[0-9]+]] @with_goto(%[[VALUE_x_7:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: i32 [storage=automatic];
// IR-NEXT:         asm goto "mov %1, %0" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_r_5]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_x_7]]);
// IR-NEXT:             labels: %[[VALUE_out:[0-9]+]];
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%[[VALUE_r_5]]);
// IR-NEXT:         label %[[VALUE_out]] out:
// IR-NEXT:             return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_basic:[0-9]+]] @basic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "nop" [dialect=att] [options=nostack];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
