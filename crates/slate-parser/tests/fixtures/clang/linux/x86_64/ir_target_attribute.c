// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
__attribute__((target("avx2"))) int one(int x) { return x; }

__attribute__((target(" avx2, fma ,popcnt"))) int list(int x) { return x; }

__attribute__((target("arch=haswell,tune=znver4,no-sse4a"))) int arch(int x) {
    return x;
}

__attribute__((target("avx512f"))) int declared(int x);

int declared(int x) { return x; }

__attribute__((target("avx2"))) int later(int x);

__attribute__((target("avx512f,avx512bw"))) int later(int x) { return x; }

int plain(int x) { return x; }

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
// IR-NEXT:     fn %[[VALUE_one:[0-9]+]] @one(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [target=+avx2] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_list:[0-9]+]] @list(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [target=+avx2,+fma,+popcnt] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_arch:[0-9]+]] @arch(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=external] [target=arch=haswell,tune=znver4,-sse4a] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_declared:[0-9]+]] @declared(%[[VALUE_x_4:[0-9]+]] x: i32) -> i32 [linkage=external] [target=+avx512f] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x_4]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_later:[0-9]+]] @later(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [target=+avx512f,+avx512bw] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x_5]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_x_6:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_x_6]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
