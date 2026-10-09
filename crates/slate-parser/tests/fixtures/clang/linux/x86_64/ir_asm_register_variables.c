// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23

long syscall3(long number, long a, long b, long c) {
    register long r10 asm("r10") = c;
    register long rax asm("rax") = number;
    register int q asm("ecx") = 0;
    asm volatile("syscall" : "+r"(rax) : "r"(a), "r"(b), "r"(r10) : "rcx", "r11", "memory");
    asm("incl %0" : "+q"(q));
    return rax + q;
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
// IR-NEXT:     fn %[[VALUE_syscall3:[0-9]+]] @syscall3(%[[VALUE_number:[0-9]+]] number: i64, %[[VALUE_a:[0-9]+]] a: i64, %[[VALUE_b:[0-9]+]] b: i64, %[[VALUE_c:[0-9]+]] c: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r10:[0-9]+]] r10: i64 [storage=automatic] [register="r10"] = read<i64>(%[[VALUE_c]]);
// IR-NEXT:         let %[[VALUE_rax:[0-9]+]] rax: i64 [storage=automatic] [register="rax"] = read<i64>(%[[VALUE_number]]);
// IR-NEXT:         let %[[VALUE_q:[0-9]+]] q: i32 [storage=automatic] [register="ecx"] = const<i32>(0);
// IR-NEXT:         asm volatile "syscall" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "syscall";
// IR-NEXT:             inlateout 0 "r" [reg] -> {rax} width 64 place<i64>(%[[VALUE_rax]]);
// IR-NEXT:             in 1 "r" [reg] width 64 read<i64>(%[[VALUE_a]]);
// IR-NEXT:             in 2 "r" [reg] width 64 read<i64>(%[[VALUE_b]]);
// IR-NEXT:             in 3 "r" [reg] -> {r10} width 64 read<i64>(%[[VALUE_r10]]);
// IR-NEXT:             clobbers: "rcx" as cx, "r11" as r11, memory;
// IR-NEXT:         }
// IR-NEXT:         asm "incl %0" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "incl " %0;
// IR-NEXT:             inlateout 0 "q" [reg] -> {ecx} width 32 place<i32>(%[[VALUE_q]]);
// IR-NEXT:         }
// IR-NEXT:         return add<i64, overflow=ub>(read<i64>(%[[VALUE_rax]]), widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_q]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
