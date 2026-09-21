// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

typedef char v1c __attribute__((vector_size(1)));
typedef short v2ss __attribute__((vector_size(4)));
typedef int v2si __attribute__((vector_size(8)));
typedef float v2sf __attribute__((vector_size(8)));
typedef long long v1ll __attribute__((vector_size(8)));
typedef double v1df __attribute__((vector_size(8)));
typedef int v4si __attribute__((vector_size(16)));
typedef double v2df __attribute__((vector_size(16)));
typedef int v8si __attribute__((vector_size(32)));
typedef int v16si __attribute__((vector_size(64)));

v1c vector_byte(v1c value) {
    return value;
}

v2ss vector_word(v2ss value) {
    return value;
}

v2si vector_integer_pair(v2si value) {
    return value;
}

v2sf vector_float_pair(v2sf value) {
    return value;
}

v1ll vector_one_long_long(v1ll value) {
    return value;
}

v1df vector_one_double(v1df value) {
    return value;
}

v4si vector_128(v4si value) {
    return value;
}

v2df vector_128_double(v2df value) {
    return value;
}

v8si vector_256(v8si value) {
    return value;
}

v16si vector_512(v16si value) {
    return value;
}

v4si forward(v4si (*callback)(v4si), v4si value) {
    return callback(value);
}

int vector_sink(int tag, ...);

int vector_variadic(v4si narrow, v8si wide) {
    return vector_sink(1, narrow, wide);
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
// IR-NEXT:     type @type0 v1c = vector<i8, 1>;
// IR-NEXT:     type @type1 v2ss = vector<i16, 2>;
// IR-NEXT:     type @type2 v2si = vector<i32, 2>;
// IR-NEXT:     type @type3 v2sf = vector<f32, 2>;
// IR-NEXT:     type @type4 v1ll = vector<i64, 1>;
// IR-NEXT:     type @type5 v1df = vector<f64, 1>;
// IR-NEXT:     type @type6 v4si = vector<i32, 4>;
// IR-NEXT:     type @type7 v2df = vector<f64, 2>;
// IR-NEXT:     type @type8 v8si = vector<i32, 8>;
// IR-NEXT:     type @type9 v16si = vector<i32, 16>;
// IR-NEXT:     fn %10 @vector_byte(%11 value: vector<i8, 1>) -> vector<i8, 1> [linkage=external] [abi=sysv64(coerce<i8>) -> coerce<i8>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i8, 1>>(%11);
// IR-NEXT:     }
// IR-NEXT:     fn %12 @vector_word(%13 value: vector<i16, 2>) -> vector<i16, 2> [linkage=external] [abi=sysv64(coerce<i32>) -> coerce<i32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i16, 2>>(%13);
// IR-NEXT:     }
// IR-NEXT:     fn %14 @vector_integer_pair(%15 value: vector<i32, 2>) -> vector<i32, 2> [linkage=external] [abi=sysv64(coerce<f64>) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 2>>(%15);
// IR-NEXT:     }
// IR-NEXT:     fn %16 @vector_float_pair(%17 value: vector<f32, 2>) -> vector<f32, 2> [linkage=external] [abi=sysv64(coerce<f64>) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<f32, 2>>(%17);
// IR-NEXT:     }
// IR-NEXT:     fn %18 @vector_one_long_long(%19 value: vector<i64, 1>) -> vector<i64, 1> [linkage=external] [abi=sysv64(coerce<f64>) -> coerce<f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i64, 1>>(%19);
// IR-NEXT:     }
// IR-NEXT:     fn %20 @vector_one_double(%21 value: vector<f64, 1>) -> vector<f64, 1> [linkage=external] [abi=sysv64(byval<align=8>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<f64, 1>>(%21);
// IR-NEXT:     }
// IR-NEXT:     fn %22 @vector_128(%23 value: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 4>>(%23);
// IR-NEXT:     }
// IR-NEXT:     fn %24 @vector_128_double(%25 value: vector<f64, 2>) -> vector<f64, 2> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<f64, 2>>(%25);
// IR-NEXT:     }
// IR-NEXT:     fn %26 @vector_256(%27 value: vector<i32, 8>) -> vector<i32, 8> [linkage=external] [abi=sysv64(byval<align=32>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 8>>(%27);
// IR-NEXT:     }
// IR-NEXT:     fn %28 @vector_512(%29 value: vector<i32, 16>) -> vector<i32, 16> [linkage=external] [abi=sysv64(byval<align=64>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 16>>(%29);
// IR-NEXT:     }
// IR-NEXT:     fn %30 @forward(%31 callback: ptr<fn(vector<i32, 4>) -> vector<i32, 4>>, %32 value: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(scalar, direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<vector<i32, 4>, signature=fn(vector<i32, 4>) -> vector<i32, 4>, abi=sysv64(direct) -> direct>(read<ptr<fn(vector<i32, 4>) -> vector<i32, 4>>>(%31), read<vector<i32, 4>>(%32));
// IR-NEXT:     }
// IR-NEXT:     fn %33 @vector_sink(%37 tag: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %34 @vector_variadic(%35 narrow: vector<i32, 4>, %36 wide: vector<i32, 8>) -> i32 [linkage=external] [abi=sysv64(direct, byval<align=32>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, ...) -> i32, abi=sysv64(scalar, direct, byval<align=32>) -> scalar>(%33, const<i32>(1), read<vector<i32, 4>>(%35), read<vector<i32, 8>>(%36));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
