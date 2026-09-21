// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct pair { int a; int b; };
struct float_pair { float a; float b; };
struct double_pair { double a; double b; };
struct mixed { int a; double b; };
struct large { long a; long b; long c; };

float _Complex complex_float(float _Complex value) { return value; }
double _Complex complex_double(double _Complex value) { return value; }
long double _Complex complex_long_double(long double _Complex value) { return value; }
struct pair record_pair(struct pair value) { return value; }
struct float_pair record_float_pair(struct float_pair value) { return value; }
struct double_pair record_double_pair(struct double_pair value) { return value; }
struct mixed record_mixed(struct mixed value) { return value; }
struct large record_large(struct large value) { return value; }
int scalar(int value) { return value; }
__int128 wide_integer(__int128 value) { return value; }

double _Complex forward(double _Complex (*callback)(double _Complex), double _Complex value) {
    return callback(value);
}

double _Complex variadic_sink(int tag, ...);
double _Complex variadic_forward(double _Complex value) {
    return variadic_sink(1, value);
}

typedef char v1c __attribute__((vector_size(1)));
typedef short v2ss __attribute__((vector_size(4)));
typedef int v2si __attribute__((vector_size(8)));
typedef double v1df __attribute__((vector_size(8)));
typedef int v4si __attribute__((vector_size(16)));
typedef int v8si __attribute__((vector_size(32)));

v1c vector_byte(v1c value) { return value; }
v2ss vector_word(v2ss value) { return value; }
v2si vector_integer_pair(v2si value) { return value; }
v1df vector_one_double(v1df value) { return value; }
v4si vector_128(v4si value) { return value; }
v8si vector_256(v8si value) { return value; }

int vector_sink(int tag, ...);
int vector_variadic(v4si narrow, v8si wide) {
    return vector_sink(1, narrow, wide);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:     type @type0 pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type1 float_pair = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type2 double_pair = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type3 mixed = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type4 large = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 c: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-NEXT:     type @type5 v1c = vector<i8, 1>;
// IR-NEXT:     type @type6 v2ss = vector<i16, 2>;
// IR-NEXT:     type @type7 v2si = vector<i32, 2>;
// IR-NEXT:     type @type8 v1df = vector<f64, 1>;
// IR-NEXT:     type @type9 v4si = vector<i32, 4>;
// IR-NEXT:     type @type10 v8si = vector<i32, 8>;
// IR-NEXT:     fn %5 @complex_float(%6 value: complex<f32>) -> complex<f32> [linkage=external] [abi=win_arm64(coerce<f32, f32>) -> coerce<f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%6);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @complex_double(%8 value: complex<f64>) -> complex<f64> [linkage=external] [abi=win_arm64(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%8);
// IR-NEXT:     }
// IR-NEXT:     fn %9 @complex_long_double(%10 value: complex<f64>) -> complex<f64> [linkage=external] [abi=win_arm64(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%10);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @record_pair(%12 value: @type0) -> @type0 [linkage=external] [abi=win_arm64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%12));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @record_float_pair(%14 value: @type1) -> @type1 [linkage=external] [abi=win_arm64(coerce<f32, f32>) -> coerce<f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%14));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @record_double_pair(%16 value: @type2) -> @type2 [linkage=external] [abi=win_arm64(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%16));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @record_mixed(%18 value: @type3) -> @type3 [linkage=external] [abi=win_arm64(coerce<i64, i64>) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type3, reason=return>(read<@type3>(%18));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @record_large(%20 value: @type4) -> @type4 [linkage=external] [abi=win_arm64(coerce<i64, i64>) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type4, reason=return>(read<@type4>(%20));
// IR-NEXT:     }
// IR-NEXT:     fn %21 @scalar(%22 value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%22);
// IR-NEXT:     }
// IR-NEXT:     fn %23 @wide_integer(%24 value: i128) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i128>(%24);
// IR-NEXT:     }
// IR-NEXT:     fn %25 @forward(%26 callback: ptr<fn(complex<f64>) -> complex<f64>>, %27 value: complex<f64>) -> complex<f64> [linkage=external] [abi=win_arm64(scalar, coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=win_arm64(coerce<f64, f64>) -> coerce<f64, f64>>(read<ptr<fn(complex<f64>) -> complex<f64>>>(%26), read<complex<f64>>(%27));
// IR-NEXT:     }
// IR-NEXT:     fn %28 @variadic_sink(%53 tag: i32, ...) -> complex<f64> [linkage=external] [abi=win_arm64(scalar) -> coerce<f64, f64>];
// IR-NEXT:     fn %29 @variadic_forward(%30 value: complex<f64>) -> complex<f64> [linkage=external] [abi=win_arm64(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, signature=fn(i32, ...) -> complex<f64>, abi=win_arm64(scalar, coerce<i64, i64>) -> coerce<f64, f64>>(%28, const<i32>(1), read<complex<f64>>(%30));
// IR-NEXT:     }
// IR-NEXT:     fn %37 @vector_byte(%38 value: vector<i8, 1>) -> vector<i8, 1> [linkage=external] [abi=win_arm64(coerce<i32>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i8, 1>>(%38);
// IR-NEXT:     }
// IR-NEXT:     fn %39 @vector_word(%40 value: vector<i16, 2>) -> vector<i16, 2> [linkage=external] [abi=win_arm64(coerce<i32>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i16, 2>>(%40);
// IR-NEXT:     }
// IR-NEXT:     fn %41 @vector_integer_pair(%42 value: vector<i32, 2>) -> vector<i32, 2> [linkage=external] [abi=win_arm64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 2>>(%42);
// IR-NEXT:     }
// IR-NEXT:     fn %43 @vector_one_double(%44 value: vector<f64, 1>) -> vector<f64, 1> [linkage=external] [abi=win_arm64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<f64, 1>>(%44);
// IR-NEXT:     }
// IR-NEXT:     fn %45 @vector_128(%46 value: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=win_arm64(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 4>>(%46);
// IR-NEXT:     }
// IR-NEXT:     fn %47 @vector_256(%48 value: vector<i32, 8>) -> vector<i32, 8> [linkage=external] [abi=win_arm64(byref<align=16>) -> sret<align=16>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 8>>(%48);
// IR-NEXT:     }
// IR-NEXT:     fn %49 @vector_sink(%54 tag: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %50 @vector_variadic(%51 narrow: vector<i32, 4>, %52 wide: vector<i32, 8>) -> i32 [linkage=external] [abi=win_arm64(direct, byref<align=16>) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, ...) -> i32, abi=win_arm64(scalar, direct, byref<align=16>) -> scalar>(%49, const<i32>(1), read<vector<i32, 4>>(%51), read<vector<i32, 8>>(%52));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
