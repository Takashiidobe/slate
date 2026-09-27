// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
typedef float v4f __attribute__((vector_size(16)));

void classes(int x, char c, v4f v, long l, long double f, long double g) {
    asm("# %0 %1 %2 %3" : "=r"(x), "=q"(c), "=x"(v), "=a"(l));
    asm("# %0 %1" : : "i"(42), "m"(x));
    asm("# %0 %1 %2 %3" : "=Q"(c), "=R"(x) : "g"(x), "Yz"(v));
    asm("# %0 %1 %2 %3 %4" : : "b"(x), "c"(x), "d"(x), "S"(l), "D"(l));
    asm("# %0 %1 %2 %3" : : "t"(f), "u"(g), "v"(v), "X"(x));
    asm("# %0 %1" : "=r,m"(x) : "l,?rn"(l));
    asm("# %0" : : "r#m"(x));
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
// IR-NEXT:     type @type0 v4f = vector<f32, 4>;
// IR-NEXT:     fn %1 @classes(%2 x: i32, %3 c: i8, %4 v: vector<f32, 4>, %5 l: i64, %6 f: f80, %7 g: f80) -> void [linkage=external] [abi=sysv64(scalar, scalar, direct, scalar, scalar, scalar) -> void] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%2);
// IR-NEXT:             lateout 1 "q" [reg] width 8 place<i8>(%3);
// IR-NEXT:             lateout 2 "x" [xmm_reg] width 128 place<vector<f32, 4>>(%4);
// IR-NEXT:             lateout 3 "a" [{ax}] width 64 place<i64>(%5);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "i" [imm | sym] -> imm width 32 const<i32>(42);
// IR-NEXT:             in 1 "m" [mem] width 32 place<i32>(%2);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             lateout 0 "Q" [reg_abcd] width 8 place<i8>(%3);
// IR-NEXT:             lateout 1 "R" [reg_legacy] width 32 place<i32>(%2);
// IR-NEXT:             in 2 "g" [reg | mem | imm | sym] -> reg width 32 read<i32>(%2);
// IR-NEXT:             in 3 "Yz" [{xmm0}] width 128 read<vector<f32, 4>>(%4);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3 %4" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3 " " %4;
// IR-NEXT:             in 0 "b" [{bx}] width 32 read<i32>(%2);
// IR-NEXT:             in 1 "c" [{cx}] width 32 read<i32>(%2);
// IR-NEXT:             in 2 "d" [{dx}] width 32 read<i32>(%2);
// IR-NEXT:             in 3 "S" [{si}] width 64 read<i64>(%5);
// IR-NEXT:             in 4 "D" [{di}] width 64 read<i64>(%5);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "t" [{st}] width 128 read<f80>(%6);
// IR-NEXT:             in 1 "u" [{st(1)}] width 128 read<f80>(%7);
// IR-NEXT:             in 2 "v" [zmm_reg] width 128 read<vector<f32, 4>>(%4);
// IR-NEXT:             in 3 "X" [unresolved("X")] width 32 read<i32>(%2);
// IR-NEXT:             rejected: 0 (operand 0: clobber-only);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] [alternative=1] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "r,m" [reg, mem] width 32 place<i32>(%2);
// IR-NEXT:             in 1 "l,?rn" [unresolved("l"), reg | imm] -> reg width 64 read<i64>(%5);
// IR-NEXT:             rejected: 0 (operand 1: unresolved("l"));
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "r#m" [reg] width 32 read<i32>(%2);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
