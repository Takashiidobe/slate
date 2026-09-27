// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
typedef float v4f __attribute__((vector_size(16)));

void classes(long x, double d, v4f v) {
    asm("// %0 %1 %2" : "=r"(x), "=w"(d), "=x"(v));
    asm("// %0 %1 %2 %3" : : "I"(1), "Q"(x), "rZ"(0L), "y"(v));
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
// IR-NEXT:     type @type0 v4f = vector<f32, 4>;
// IR-NEXT:     fn %1 @classes(%2 x: i64, %3 d: f64, %4 v: vector<f32, 4>) -> void [linkage=external] [abi=aapcs64(scalar, scalar, direct) -> void] [fallthrough=ret_void] {
// IR-NEXT:         asm "// %0 %1 %2" {
// IR-NEXT:             template: "// " %0 " " %1 " " %2;
// IR-NEXT:             lateout 0 "r" [reg] place<i64>(%2);
// IR-NEXT:             lateout 1 "w" [vreg] place<f64>(%3);
// IR-NEXT:             lateout 2 "x" [vreg_low16] place<vector<f32, 4>>(%4);
// IR-NEXT:         }
// IR-NEXT:         asm "// %0 %1 %2 %3" {
// IR-NEXT:             template: "// " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "I" [imm] const<i32>(1);
// IR-NEXT:             in 1 "Q" [mem] place<i64>(%2);
// IR-NEXT:             in 2 "rZ" [reg | imm] const<i64>(0);
// IR-NEXT:             in 3 "y" [vreg_low8] read<vector<f32, 4>>(%4);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
