// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23
long g;

int numbers(long a, int b) {
    asm volatile("# %0 %1 %2 %3 %4" : "+r"(a), "+m"(g) : "r"(b));
    asm volatile("# %q1 %k1" : "+a"(b));
    asm volatile("# %0 %1 %2 %3 %4" : "+r"(a), "=r"(b) : "r"(b), "m"(g));
    asm goto("# %0 %1 %l2 %1" : "+r"(a) : : : out);
    return a;
out:
    return 0;
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
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: i64 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_numbers:[0-9]+]] @numbers(%[[VALUE_a:[0-9]+]] a: i64, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         asm volatile "# %0 %1 %2 %3 %4" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %0 " " %1;
// IR-NEXT:             inlateout 0 "r" [reg] width 64 place<i64>(%[[VALUE_a]]);
// IR-NEXT:             inlateout 1 "m" [mem] width 64 place<i64>(%[[VALUE_g]]);
// IR-NEXT:             in 2 "r" [reg] width 32 read<i32>(%[[VALUE_b]]);
// IR-NEXT:         }
// IR-NEXT:         asm volatile "# %q1 %k1" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "# " %q0(64) " " %k0(32);
// IR-NEXT:             inlateout 0 "a" [{ax}] width 32 place<i32>(%[[VALUE_b]]);
// IR-NEXT:         }
// IR-NEXT:         asm volatile "# %0 %1 %2 %3 %4" [dialect=att] [options=readonly,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3 " " %0;
// IR-NEXT:             inlateout 0 "r" [reg] width 64 place<i64>(%[[VALUE_a]]);
// IR-NEXT:             lateout 1 "r" [reg] width 32 place<i32>(%[[VALUE_b]]);
// IR-NEXT:             in 2 "r" [reg] width 32 read<i32>(%[[VALUE_b]]);
// IR-NEXT:             in 3 "m" [mem] width 64 place<i64>(%[[VALUE_g]]);
// IR-NEXT:         }
// IR-NEXT:         asm goto "# %0 %1 %l2 %1" [dialect=att] [options=nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %0 " " %l0 " " %0;
// IR-NEXT:             inlateout 0 "r" [reg] width 64 place<i64>(%[[VALUE_a]]);
// IR-NEXT:             labels: %[[VALUE_out:[0-9]+]];
// IR-NEXT:         }
// IR-NEXT:         return truncate<i32, reason=return, fits=unknown>(read<i64>(%[[VALUE_a]]));
// IR-NEXT:         label %[[VALUE_out]] out:
// IR-NEXT:             return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
