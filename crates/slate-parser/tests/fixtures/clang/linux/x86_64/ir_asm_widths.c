// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
typedef float v4f __attribute__((vector_size(16)));
typedef float v8f __attribute__((vector_size(32)));
typedef float v16f __attribute__((vector_size(64)));
struct Half { short a, b; };

void widths(char c, short s, int i, long l, _Bool b, struct Half h) {
    asm("# %0 %1 %2 %3" : "+r"(c), "+r"(s), "+r"(i), "+r"(l));
    asm("# %0 %1" : : "r"(b), "r"(h));
    asm("# %b0 %h0 %w0 %k0 %q0" : "+a"(i));
    asm("# %0 %1 %k2" : "=r"(c) : "0"(i), "r"(l));
}

void vectors(v4f v, v8f w, v16f z, float f) {
    asm("# %0 %1 %2 %3" : : "x"(v), "x"(w), "x"(z), "x"(f));
    asm("# %0 %x0 %t0 %g0" : "+v"(v));
    asm("# %0" : "+x,m"(w));
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
// IR-NEXT:     type @type[[TYPE_v8f:[0-9]+]] v8f = vector<f32, 8>;
// IR-NEXT:     type @type[[TYPE_v16f:[0-9]+]] v16f = vector<f32, 16>;
// IR-NEXT:     type @type[[TYPE_Half:[0-9]+]] Half = struct {
// IR-NEXT:         field0 a: i16;
// IR-NEXT:         field1 b: i16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     fn %[[VALUE_widths:[0-9]+]] @widths(%[[VALUE_c:[0-9]+]] c: i8, %[[VALUE_s:[0-9]+]] s: i16, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_l:[0-9]+]] l: i64, %[[VALUE_b:[0-9]+]] b: bool, %[[VALUE_h:[0-9]+]] h: @type[[TYPE_Half]]) -> void [linkage=external] [abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             inlateout 0 "r" [reg] width 8 place<i8>(%[[VALUE_c]]);
// IR-NEXT:             inlateout 1 "r" [reg] width 16 place<i16>(%[[VALUE_s]]);
// IR-NEXT:             inlateout 2 "r" [reg] width 32 place<i32>(%[[VALUE_i]]);
// IR-NEXT:             inlateout 3 "r" [reg] width 64 place<i64>(%[[VALUE_l]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1;
// IR-NEXT:             in 0 "r" [reg] width 8 read<bool>(%[[VALUE_b]]);
// IR-NEXT:             in 1 "r" [reg] width 32 read<@type[[TYPE_Half]]>(%[[VALUE_h]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %b0 %h0 %w0 %k0 %q0" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %b0(8) " " %h0(high8) " " %w0(16) " " %k0(32) " " %q0(64);
// IR-NEXT:             inlateout 0 "a" [{ax}] width 32 place<i32>(%[[VALUE_i]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %1 %k2" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %0(32) " " %k1(32);
// IR-NEXT:             inlateout 0 "r" [reg] width 8 place<i8>(%[[VALUE_c]]) from read<i32>(%[[VALUE_i]]);
// IR-NEXT:             in 1 "r" [reg] width 64 read<i64>(%[[VALUE_l]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vectors:[0-9]+]] @vectors(%[[VALUE_v:[0-9]+]] v: vector<f32, 4>, %[[VALUE_w:[0-9]+]] w: vector<f32, 8>, %[[VALUE_z:[0-9]+]] z: vector<f32, 16>, %[[VALUE_f:[0-9]+]] f: f32) -> void [linkage=external] [abi=sysv64(direct, byval<align=32>, byval<align=64>, scalar) -> void] [fallthrough=ret_void] {
// IR-NEXT:         asm "# %0 %1 %2 %3" [dialect=att] [options=nostack] {
// IR-NEXT:             template: "# " %0 " " %1 " " %2 " " %3;
// IR-NEXT:             in 0 "x" [xmm_reg] width 128 read<vector<f32, 4>>(%[[VALUE_v]]);
// IR-NEXT:             in 1 "x" [ymm_reg] width 256 read<vector<f32, 8>>(%[[VALUE_w]]);
// IR-NEXT:             in 2 "x" [zmm_reg] width 512 read<vector<f32, 16>>(%[[VALUE_z]]);
// IR-NEXT:             in 3 "x" [xmm_reg] width 32 read<f32>(%[[VALUE_f]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0 %x0 %t0 %g0" [dialect=att] [options=pure,nomem,nostack] {
// IR-NEXT:             template: "# " %0 " " %x0(128) " " %t0(256) " " %g0(512);
// IR-NEXT:             inlateout 0 "v" [zmm_reg] width 128 place<vector<f32, 4>>(%[[VALUE_v]]);
// IR-NEXT:         }
// IR-NEXT:         asm "# %0" [dialect=att] [options=pure,nomem,nostack] [alternative=0] {
// IR-NEXT:             template: "# " %0;
// IR-NEXT:             inlateout 0 "x,m" [ymm_reg, mem] width 256 place<vector<f32, 8>>(%[[VALUE_w]]);
// IR-NEXT:         }
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
