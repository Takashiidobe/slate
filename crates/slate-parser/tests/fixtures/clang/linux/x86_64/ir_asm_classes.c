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
    asm("# %0 %a1" : "+V"(x) : "p"(&x));
}

struct big { long a, b, c; };
int g;

void letters(int x, double d, v4f v, long l, __int128 w, struct big b) {
    asm volatile("# %0 %1 %2 %3 %4 %5" : : "X"(x), "X"(5), "X"(d), "X"(v), "X"(b), "X"(&g));
    asm("# %0 %1" : "=X"(x), "=X"(d));
    asm("# %0 %1 %2" : : "Yi"(v), "Yt"(d), "Y2"(d));
    asm("# %0 %1 %2" : "=A"(l), "+A"(w) : "l"(l));
    asm("# %0 %1" : : "rx"(d), "g"(d));
}

__attribute__((target("avx512f"))) void masks(long l, unsigned char k) {
    asm("# %0" : : "Yk"(k));
    asm("# %0" : : "Ym"(l));
}

void spilled(int i, char c, double d, long double f) {
    asm("# %0 %1 %2" : "=t"(d), "=u"(f) : "f"(f));
    asm("# %0" : : "t"(i));
    asm("# %0 %1" : "=y"(i) : "y"(c));
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
// IR-NEXT:     type @type[[TYPE_v4f:[0-9]+]] v4f = vector<f32, 4>;
// IR-NEXT:     type @type[[TYPE_big:[0-9]+]] big = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_classes:[0-9]+]] @classes(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_c:[0-9]+]] c: i8, %[[VALUE_v:[0-9]+]] v: vector<f32, 4>, %[[VALUE_l:[0-9]+]] l: i64, %[[VALUE_f:[0-9]+]] f: f80, %[[VALUE_g_2:[0-9]+]] g: f80) -> void [linkage=external] [abi=sysv64(scalar, scalar, direct, scalar, scalar, scalar) -> void] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             lateout 1 "q" [reg] width 8 place<i8>(%[[VALUE_c]]);
// IR-NEXT:             lateout 2 "x" [xmm_reg] width 128 place<vector<f32, 4>>(%[[VALUE_v]]);
// IR-NEXT:             lateout 3 "a" [{ax}] width 64 place<i64>(%[[VALUE_l]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "i" [imm | sym] -> imm width 32 const<i32>(42);
// IR-NEXT:             in 1 "m" [mem] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             lateout 0 "Q" [reg_abcd] width 8 place<i8>(%[[VALUE_c]]);
// IR-NEXT:             lateout 1 "R" [reg_legacy] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 2 "g" [reg | mem | imm | sym] -> reg width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 3 "Yz" [{xmm0}] width 128 read<vector<f32, 4>>(%[[VALUE_v]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3 %4" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3 " " %4;
// IR-NEXT:             in 0 "b" [{bx}] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 1 "c" [{cx}] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 2 "d" [{dx}] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 3 "S" [{si}] width 64 read<i64>(%[[VALUE_l]]);
// IR-NEXT:             in 4 "D" [{di}] width 64 read<i64>(%[[VALUE_l]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "t" [{st}] width 128 read<f80>(%[[VALUE_f]]);
// IR-NEXT:             in 1 "u" [{st(1)}] width 128 read<f80>(%[[VALUE_g_2]]);
// IR-NEXT:             in 2 "v" [zmm_reg] width 128 read<vector<f32, 4>>(%[[VALUE_v]]);
// IR-NEXT:             in 3 "X" [imm | sym | reg | xmm_reg | mem] -> reg width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "r,m" [reg, mem] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 1 "l,?rn" [reg, reg | imm] width 64 read<i64>(%[[VALUE_l]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "r#m" [reg] width 32 read<i32>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %a1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %a1;
// IR-NEXT:             inlateout 0 "V" [mem] width 32 place<i32>(%[[VALUE_x]]);
// IR-NEXT:             in 1 "p" [reg] width 64 addr_of<ptr<i32>>(%[[VALUE_x]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_letters:[0-9]+]] @letters(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_d:[0-9]+]] d: f64, %[[VALUE_v_2:[0-9]+]] v: vector<f32, 4>, %[[VALUE_l_2:[0-9]+]] l: i64, %[[VALUE_w:[0-9]+]] w: i128, %[[VALUE_b:[0-9]+]] b: @type[[TYPE_big]]) -> void [linkage=external] [abi=sysv64(scalar, scalar, direct, scalar, scalar, native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         asm volatile "# %0 %1 %2 %3 %4 %5" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3 " " %4 " " %5;
// IR-NEXT:             in 0 "X" [imm | sym | reg | xmm_reg | mem] -> reg width 32 read<i32>(%[[VALUE_x_2]]);
// IR-NEXT:             in 1 "X" [imm | sym | reg | xmm_reg | mem] -> imm width 32 const<i32>(5);
// IR-NEXT:             in 2 "X" [imm | sym | reg | xmm_reg | mem] -> xmm_reg width 64 read<f64>(%[[VALUE_d]]);
// IR-NEXT:             in 3 "X" [imm | sym | reg | xmm_reg | mem] -> xmm_reg width 128 read<vector<f32, 4>>(%[[VALUE_v_2]]);
// IR-NEXT:             in 4 "X" [imm | sym | reg | xmm_reg | mem] -> mem width 192 place<@type[[TYPE_big]]>(%[[VALUE_b]]);
// IR-NEXT:             in 5 "X" [imm | sym | reg | xmm_reg | mem] -> sym width 64 sym<offset=0>(%[[VALUE_g]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "X" [imm | sym | reg | xmm_reg | mem] -> reg width 32 place<i32>(%[[VALUE_x_2]]);
// IR-NEXT:             lateout 1 "X" [imm | sym | reg | xmm_reg | mem] -> xmm_reg width 64 place<f64>(%[[VALUE_d]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             in 0 "Yi" [xmm_reg] width 128 read<vector<f32, 4>>(%[[VALUE_v_2]]);
// IR-NEXT:             in 1 "Yt" [xmm_reg] width 64 read<f64>(%[[VALUE_d]]);
// IR-NEXT:             in 2 "Y2" [xmm_reg] width 64 read<f64>(%[[VALUE_d]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             lateout 0 "A" [{dx:ax} | {ax}] -> {ax} width 64 place<i64>(%[[VALUE_l_2]]);
// IR-NEXT:             inlateout 1 "A" [{dx:ax} | {ax}] -> {dx:ax} width 128 place<i128>(%[[VALUE_w]]);
// IR-NEXT:             in 2 "l" [reg] width 64 read<i64>(%[[VALUE_l_2]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "rx" [reg | xmm_reg] -> xmm_reg width 64 read<f64>(%[[VALUE_d]]);
// IR-NEXT:             in 1 "g" [reg | mem | imm | sym] -> reg width 64 read<f64>(%[[VALUE_d]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_masks:[0-9]+]] @masks(%[[VALUE_l_3:[0-9]+]] l: i64, %[[VALUE_k:[0-9]+]] k: u8) -> void [linkage=external] [target=+avx512f] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "Yk" [kreg] width 8 read<u8>(%[[VALUE_k]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "Ym" [mmx_reg] width 64 read<i64>(%[[VALUE_l_3]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_spilled:[0-9]+]] @spilled(%[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_c_2:[0-9]+]] c: i8, %[[VALUE_d_2:[0-9]+]] d: f64, %[[VALUE_f_2:[0-9]+]] f: f80) -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1 %2" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2;
// IR-NEXT:             lateout 0 "t" [{st}] width 64 place<f64>(%[[VALUE_d_2]]);
// IR-NEXT:             lateout 1 "u" [{st(1)}] width 128 place<f80>(%[[VALUE_f_2]]);
// IR-NEXT:             in 2 "f" [x87_reg] width 128 read<f80>(%[[VALUE_f_2]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             in 0 "t" [{st}] width 32 read<i32>(%[[VALUE_i]]);
// IR-NEXT:             rejected: 0 (operand 0: width);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=pure,nomem,nostack] [alternative=none] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             lateout 0 "y" [mmx_reg] width 32 place<i32>(%[[VALUE_i]]);
// IR-NEXT:             in 1 "y" [mmx_reg] width 8 read<i8>(%[[VALUE_c_2]]);
// IR-NEXT:             rejected: 0 (operand 1: width);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
