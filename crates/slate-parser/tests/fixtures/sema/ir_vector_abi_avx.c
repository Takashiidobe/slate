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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 v8si = vector<i32, 8>;
// IR-NEXT:     type @type1 v16si = vector<i32, 16>;
// IR-NEXT:     type @type2 v32si = vector<i32, 32>;
// IR-NEXT:     fn %3 @vector_256(%4 value: vector<i32, 8>) -> vector<i32, 8> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 8>>(%4);
// IR-NEXT:     }
// IR-NEXT:     fn %5 @vector_512(%6 value: vector<i32, 16>) -> vector<i32, 16> [linkage=external] [abi=sysv64(byval<align=64>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 16>>(%6);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @vector_1024(%8 value: vector<i32, 32>) -> vector<i32, 32> [linkage=external] [abi=sysv64(byval<align=128>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 32>>(%8);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
