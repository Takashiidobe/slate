// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
void classes(int x, float f, double d) {
    asm("@ %0 %1 %2" : "=r"(x), "=t"(f), "=w"(d));
    asm("@ %0 %1 %2" : : "I"(1), "Q"(x), "l"(x));
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
// IR-NEXT:     fn %0 @classes(%1 x: i32, %2 f: f32, %3 d: f64) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "@ %0 %1 %2" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "@ " %0 " " %1 " " %2;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%1);
// IR-NEXT:             lateout 1 "t" [sreg] width 32 place<f32>(%2);
// IR-NEXT:             lateout 2 "w" [dreg] width 64 place<f64>(%3);
// IR-NEXT:         }
// IR-NEXT:         asm "@ %0 %1 %2" [options=nostack,preserves_flags] [alternative=none] {
// IR-NEXT:             template: "@ " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "I" [imm] width 32 const<i32>(1);
// IR-NEXT:             in 1 "Q" [mem] width 32 place<i32>(%1);
// IR-NEXT:             in 2 "l" [unresolved("l")] width 32 read<i32>(%1);
// IR-NEXT:             rejected: 0 (operand 2: unresolved("l"));
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
