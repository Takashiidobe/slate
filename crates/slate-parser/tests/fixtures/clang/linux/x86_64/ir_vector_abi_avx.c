// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -mavx

typedef int v8si __attribute__((vector_size(32)));
typedef int v16si __attribute__((vector_size(64)));
typedef int v32si __attribute__((vector_size(128)));

v8si vector_256(v8si value) {
    return value;
}

v16si vector_512(v16si value) {
    return value;
}

v32si vector_1024(v32si value) {
    return value;
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
// IR-NEXT:     type @type[[TYPE_v8si:[0-9]+]] v8si = vector<i32, 8>;
// IR-NEXT:     type @type[[TYPE_v16si:[0-9]+]] v16si = vector<i32, 16>;
// IR-NEXT:     type @type[[TYPE_v32si:[0-9]+]] v32si = vector<i32, 32>;
// IR-NEXT:     fn %[[VALUE_vector_256:[0-9]+]] @vector_256(%[[VALUE_value:[0-9]+]] value: vector<i32, 8>) -> vector<i32, 8> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 8>>(%[[VALUE_value]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_512:[0-9]+]] @vector_512(%[[VALUE_value_2:[0-9]+]] value: vector<i32, 16>) -> vector<i32, 16> [linkage=external] [abi=sysv64(byval<align=64>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 16>>(%[[VALUE_value_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_1024:[0-9]+]] @vector_1024(%[[VALUE_value_3:[0-9]+]] value: vector<i32, 32>) -> vector<i32, 32> [linkage=external] [abi=sysv64(byval<align=128>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 32>>(%[[VALUE_value_3]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
