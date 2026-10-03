// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23
typedef int v4si __attribute__((vector_size(16)));
typedef unsigned v4su __attribute__((vector_size(16)));
typedef long long v2di __attribute__((vector_size(16)));
typedef long long v4di __attribute__((vector_size(32)));
typedef int v16si __attribute__((vector_size(64)));
typedef float v16sf __attribute__((vector_size(64)));

int reduce(v4si v) { return __builtin_reduce_add(v) + __builtin_reduce_max(v); }

unsigned reduce_unsigned(v4su v) { return __builtin_reduce_min(v); }

v4su elementwise(v4su a, v4su b) {
    return __builtin_elementwise_max(__builtin_elementwise_popcount(a), b);
}

__attribute__((target("avx2"))) v2di extract(v4di v) {
    return __builtin_ia32_extract128i256(v, 1);
}

__attribute__((target("avx512f"))) v16si ternary(v16si a, v16si b, v16si c, unsigned short k) {
    return __builtin_ia32_pternlogd512_mask(a, b, c, 0xEA, (unsigned short)-1)
         + __builtin_ia32_pternlogd512_mask(a, b, c, 0xEA, k);
}

__attribute__((target("avx512f"))) float fadd(v16sf v) {
    return __builtin_ia32_reduce_fadd_ps512(-0.0f, v);
}

unsigned long long ticks(void) { return __rdtsc(); }

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
// IR-NEXT:     type @type[[TYPE_v4si:[0-9]+]] v4si = vector<i32, 4>;
// IR-NEXT:     type @type[[TYPE_v4su:[0-9]+]] v4su = vector<u32, 4>;
// IR-NEXT:     type @type[[TYPE_v2di:[0-9]+]] v2di = vector<i64, 2>;
// IR-NEXT:     type @type[[TYPE_v4di:[0-9]+]] v4di = vector<i64, 4>;
// IR-NEXT:     type @type[[TYPE_v16si:[0-9]+]] v16si = vector<i32, 16>;
// IR-NEXT:     type @type[[TYPE_v16sf:[0-9]+]] v16sf = vector<f32, 16>;
// IR-NEXT:     fn %[[VALUE_reduce:[0-9]+]] @reduce(%[[VALUE_v:[0-9]+]] v: vector<i32, 4>) -> i32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32, overflow=ub>(intrinsic<i32, llvm.vector.reduce.add>(read<vector<i32, 4>>(%[[VALUE_v]])), intrinsic<i32, llvm.vector.reduce.smax>(read<vector<i32, 4>>(%[[VALUE_v]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_reduce_unsigned:[0-9]+]] @reduce_unsigned(%[[VALUE_v_2:[0-9]+]] v: vector<u32, 4>) -> u32 [linkage=external] [abi=sysv64(direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return intrinsic<u32, llvm.vector.reduce.umin>(read<vector<u32, 4>>(%[[VALUE_v_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_elementwise:[0-9]+]] @elementwise(%[[VALUE_a:[0-9]+]] a: vector<u32, 4>, %[[VALUE_b:[0-9]+]] b: vector<u32, 4>) -> vector<u32, 4> [linkage=external] [abi=sysv64(direct, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return intrinsic<vector<u32, 4>, llvm.umax>(intrinsic<vector<u32, 4>, llvm.ctpop>(read<vector<u32, 4>>(%[[VALUE_a]])), read<vector<u32, 4>>(%[[VALUE_b]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_extract:[0-9]+]] @extract(%[[VALUE_v_3:[0-9]+]] v: vector<i64, 4>) -> vector<i64, 2> [linkage=external] [target=+avx2] [abi=sysv64(byval<align=32>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return shuffle<vector<i64, 2>, mask=[2, 3]>(read<vector<i64, 4>>(%[[VALUE_v_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE___builtin_ia32_pternlogd512_mask:[0-9]+]] @__builtin_ia32_pternlogd512_mask(%[[VALUE0:[0-9]+]] <unnamed>: vector<i32, 16>, %[[VALUE1:[0-9]+]] <unnamed>: vector<i32, 16>, %[[VALUE2:[0-9]+]] <unnamed>: vector<i32, 16>, %[[VALUE3:[0-9]+]] <unnamed>: i32, %[[VALUE4:[0-9]+]] <unnamed>: u16) -> vector<i32, 16> [linkage=external] [memory=none] [abi=sysv64(byval<align=64>, byval<align=64>, byval<align=64>, scalar, scalar) -> direct];
// IR-NEXT:     fn %[[VALUE_ternary:[0-9]+]] @ternary(%[[VALUE_a_2:[0-9]+]] a: vector<i32, 16>, %[[VALUE_b_2:[0-9]+]] b: vector<i32, 16>, %[[VALUE_c:[0-9]+]] c: vector<i32, 16>, %[[VALUE_k:[0-9]+]] k: u16) -> vector<i32, 16> [linkage=external] [target=+avx512f] [abi=sysv64(byval<align=64>, byval<align=64>, byval<align=64>, scalar) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<vector<i32, 16>, elementwise=true, overflow=wrap>(intrinsic<vector<i32, 16>, llvm.x86.avx512.pternlog.d.512>(read<vector<i32, 16>>(%[[VALUE_a_2]]), read<vector<i32, 16>>(%[[VALUE_b_2]]), read<vector<i32, 16>>(%[[VALUE_c]]), const<i32>(234)), call<vector<i32, 16>, signature=fn(vector<i32, 16>, vector<i32, 16>, vector<i32, 16>, i32, u16) -> vector<i32, 16>, abi=sysv64(byval<align=64>, byval<align=64>, byval<align=64>, scalar, scalar) -> direct>(%[[VALUE___builtin_ia32_pternlogd512_mask]], read<vector<i32, 16>>(%[[VALUE_a_2]]), read<vector<i32, 16>>(%[[VALUE_b_2]]), read<vector<i32, 16>>(%[[VALUE_c]]), const<i32>(234), read<u16>(%[[VALUE_k]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fadd:[0-9]+]] @fadd(%[[VALUE_v_4:[0-9]+]] v: vector<f32, 16>) -> f32 [linkage=external] [target=+avx512f] [abi=sysv64(byval<align=64>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: vector<f32, 16> [synthetic] = read<vector<f32, 16>>(%[[VALUE_v_4]]);
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: vector<f32, 8> [synthetic] = add<vector<f32, 8>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(shuffle<vector<f32, 8>, mask=[0, 1, 2, 3, 4, 5, 6, 7]>(read<vector<f32, 16>>(%[[VALUE5]])), shuffle<vector<f32, 8>, mask=[8, 9, 10, 11, 12, 13, 14, 15]>(read<vector<f32, 16>>(%[[VALUE5]])));
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: vector<f32, 4> [synthetic] = add<vector<f32, 4>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(shuffle<vector<f32, 4>, mask=[0, 1, 2, 3]>(read<vector<f32, 8>>(%[[VALUE6]])), shuffle<vector<f32, 4>, mask=[4, 5, 6, 7]>(read<vector<f32, 8>>(%[[VALUE6]])));
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: vector<f32, 2> [synthetic] = add<vector<f32, 2>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(shuffle<vector<f32, 2>, mask=[0, 1]>(read<vector<f32, 4>>(%[[VALUE7]])), shuffle<vector<f32, 2>, mask=[2, 3]>(read<vector<f32, 4>>(%[[VALUE7]])));
// IR-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(neg<f32>(const<f32>(0.0)), lane<f32>(add<vector<f32, 1>, elementwise=true, rounding=nearest_even, exceptions=ignore, contract=on>(shuffle<vector<f32, 1>, mask=[0]>(read<vector<f32, 2>>(%[[VALUE8]])), shuffle<vector<f32, 1>, mask=[1]>(read<vector<f32, 2>>(%[[VALUE8]]))), const<i32>(0)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_ticks:[0-9]+]] @ticks() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return intrinsic<u64, llvm.x86.rdtsc>();
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
