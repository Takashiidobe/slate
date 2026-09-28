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
// IR-NEXT:     global %0 g: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %1 @pure_nomem(%2 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %3 r: i32 [storage=automatic];
// IR-NEXT:         asm "mov %1, %0" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%3);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%2);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%3);
// IR-NEXT:     }
// IR-NEXT:     fn %4 @volatile_nomem(%5 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %6 r: i32 [storage=automatic];
// IR-NEXT:         asm volatile "mov %1, %0" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%6);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%5);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%6);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @memory_clobber(%8 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %9 r: i32 [storage=automatic];
// IR-NEXT:         asm "mov %1, %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%9);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%8);
// IR-NEXT:             clobbers: memory;
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%9);
// IR-NEXT:     }
// IR-NEXT:     fn %10 @memory_input(%11 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %12 r: i32 [storage=automatic];
// IR-NEXT:         asm "mov %1, %0" [dialect=att] [options=pure,readonly,nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%12);
// IR-NEXT:             in 1 "m" [mem] width 32 place<i32>(%11);
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%12);
// IR-NEXT:     }
// IR-NEXT:     fn %13 @memory_output(%14 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "mov %1, %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "m" [mem] width 32 place<i32>(%0);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%14);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %15 @no_outputs(%16 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "r" [reg] width 32 read<i32>(%16);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %17 @with_goto(%19 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %20 r: i32 [storage=automatic];
// IR-NEXT:         asm goto "mov %1, %0" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "mov " %1 ", " %0;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%20);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%19);
// IR-NEXT:             labels: %18;
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%20);
// IR-NEXT:         label %18 out:
// IR-NEXT:             return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %21 @basic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "nop" [dialect=att] [options=nostack];
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
