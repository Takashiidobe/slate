// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
typedef float v4f __attribute__((vector_size(16)));

void widths(char c, int i, long l, float f, v4f v) {
    asm("// %0 %1 %2 %w2 %x1" : : "r"(c), "r"(i), "r"(l));
    asm("// %0 %b0 %h0 %s0 %d0 %q0 %1" : "+w"(v) : "w"(f));
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
// IR-NEXT:     type @type[[TYPE_v4f:[0-9]+]] v4f = vector<f32, 4>;
// IR-NEXT:     fn %[[VALUE_widths:[0-9]+]] @widths(%[[VALUE_c:[0-9]+]] c: u8, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_l:[0-9]+]] l: i64, %[[VALUE_f:[0-9]+]] f: f32, %[[VALUE_v:[0-9]+]] v: vector<f32, 4>) -> void [linkage=external] [abi=aapcs64(scalar, scalar, scalar, scalar, direct) -> void] [fallthrough=ret_void] {
// IR-NEXT:         asm "// %0 %1 %2 %w2 %x1" [options=nostack,preserves_flags] {
// IR-NEXT:             template: "// " %0 " " %1 " " %2 " " %w2(32) " " %x1(64);
// IR-NEXT:             in 0 "r" [reg] width 8 read<u8>(%[[VALUE_c]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<i32>(%[[VALUE_i]]);
// IR-NEXT:             in 2 "r" [reg] width 64 read<i64>(%[[VALUE_l]]);
// IR-NEXT:         }
// IR-NEXT:         asm "// %0 %b0 %h0 %s0 %d0 %q0 %1" [options=pure,nomem,nostack,preserves_flags] {
// IR-NEXT:             template: "// " %0 " " %b0(8) " " %h0(16) " " %s0(32) " " %d0(64) " " %q0(128) " " %1;
// IR-NEXT:             inlateout 0 "w" [vreg] width 128 place<vector<f32, 4>>(%[[VALUE_v]]);
// IR-NEXT:             in 1 "w" [vreg] width 32 read<f32>(%[[VALUE_f]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
