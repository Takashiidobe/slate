// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

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

struct hfa4 { float a, b, c, d; };
struct hfa5 { float a, b, c, d, e; };
struct big { int a[9]; };
typedef int v4 __attribute__((vector_size(16)));

struct hfa4 pass_hfa4(struct hfa4 value) { return value; }
struct hfa5 pass_hfa5(struct hfa5 value) { return value; }
struct big pass_big(struct big value) { return value; }
_Complex double pass_complex(_Complex double value) { return value; }
long long pass_long_long(int a, long long b) { return b + a; }
v4 pass_vector(v4 value) { return value; }
double variadic(int count, ...) { return count; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "thumbv7a-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 8;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
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
// IR-NEXT:     type @type11 hfa4 = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:         field2 c: f32;
// IR-NEXT:         field3 d: f32;
// IR-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// IR-NEXT:     type @type12 hfa5 = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:         field2 c: f32;
// IR-NEXT:         field3 d: f32;
// IR-NEXT:         field4 e: f32;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// IR-NEXT:     type @type13 big = struct {
// IR-NEXT:         field0 a: array<i32, 9>;
// IR-NEXT:     } [size=36, align=4, offsets=[0]];
// IR-NEXT:     type @type14 v4 = vector<i32, 4>;
// IR-NEXT:     fn %5 @complex_float(%6 value: complex<f32>) -> complex<f32> [linkage=external] [abi=aapcs32_hard_float(coerce<f32, f32>) -> coerce<f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%6);
// IR-NEXT:     }
// IR-NEXT:     fn %7 @complex_double(%8 value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%8);
// IR-NEXT:     }
// IR-NEXT:     fn %9 @complex_long_double(%10 value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%10);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @record_pair(%12 value: @type0) -> @type0 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%12));
// IR-NEXT:     }
// IR-NEXT:     fn %13 @record_float_pair(%14 value: @type1) -> @type1 [linkage=external] [abi=aapcs32_hard_float(coerce<f32, f32>) -> coerce<f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%14));
// IR-NEXT:     }
// IR-NEXT:     fn %15 @record_double_pair(%16 value: @type2) -> @type2 [linkage=external] [abi=aapcs32_hard_float(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%16));
// IR-NEXT:     }
// IR-NEXT:     fn %17 @record_mixed(%18 value: @type3) -> @type3 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type3, reason=return>(read<@type3>(%18));
// IR-NEXT:     }
// IR-NEXT:     fn %19 @record_large(%20 value: @type4) -> @type4 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type4, reason=return>(read<@type4>(%20));
// IR-NEXT:     }
// IR-NEXT:     fn %21 @scalar(%22 value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%22);
// IR-NEXT:     }
// IR-NEXT:     fn %23 @forward(%24 callback: ptr<fn(complex<f64>) -> complex<f64>>, %25 value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(scalar, coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, abi=aapcs32_hard_float(coerce<f64, f64>) -> coerce<f64, f64>>(read<ptr<fn(complex<f64>) -> complex<f64>>>(%24), read<complex<f64>>(%25));
// IR-NEXT:     }
// IR-NEXT:     fn %26 @variadic_sink(%70 tag: i32, ...) -> complex<f64> [linkage=external] [abi=aapcs32(scalar) -> native_c];
// IR-NEXT:     fn %27 @variadic_forward(%28 value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, abi=aapcs32(scalar, native_c) -> native_c>(%26, const<i32>(1), read<complex<f64>>(%28));
// IR-NEXT:     }
// IR-NEXT:     fn %35 @vector_byte(%36 value: vector<i8, 1>) -> vector<i8, 1> [linkage=external] [abi=aapcs32_hard_float(coerce<i32>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i8, 1>>(%36);
// IR-NEXT:     }
// IR-NEXT:     fn %37 @vector_word(%38 value: vector<i16, 2>) -> vector<i16, 2> [linkage=external] [abi=aapcs32_hard_float(coerce<i32>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i16, 2>>(%38);
// IR-NEXT:     }
// IR-NEXT:     fn %39 @vector_integer_pair(%40 value: vector<i32, 2>) -> vector<i32, 2> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 2>>(%40);
// IR-NEXT:     }
// IR-NEXT:     fn %41 @vector_one_double(%42 value: vector<f64, 1>) -> vector<f64, 1> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<f64, 1>>(%42);
// IR-NEXT:     }
// IR-NEXT:     fn %43 @vector_128(%44 value: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 4>>(%44);
// IR-NEXT:     }
// IR-NEXT:     fn %45 @vector_256(%46 value: vector<i32, 8>) -> vector<i32, 8> [linkage=external] [abi=aapcs32_hard_float(direct) -> sret<align=8>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 8>>(%46);
// IR-NEXT:     }
// IR-NEXT:     fn %47 @vector_sink(%71 tag: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %48 @vector_variadic(%49 narrow: vector<i32, 4>, %50 wide: vector<i32, 8>) -> i32 [linkage=external] [abi=aapcs32_hard_float(direct, direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, abi=aapcs32(scalar, direct, direct) -> scalar>(%47, const<i32>(1), read<vector<i32, 4>>(%49), read<vector<i32, 8>>(%50));
// IR-NEXT:     }
// IR-NEXT:     fn %55 @pass_hfa4(%56 value: @type11) -> @type11 [linkage=external] [abi=aapcs32_hard_float(coerce<f32, f32, f32, f32>) -> coerce<f32, f32, f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type11, reason=return>(read<@type11>(%56));
// IR-NEXT:     }
// IR-NEXT:     fn %57 @pass_hfa5(%58 value: @type12) -> @type12 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type12, reason=return>(read<@type12>(%58));
// IR-NEXT:     }
// IR-NEXT:     fn %59 @pass_big(%60 value: @type13) -> @type13 [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type13, reason=return>(read<@type13>(%60));
// IR-NEXT:     }
// IR-NEXT:     fn %61 @pass_complex(%62 value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%62);
// IR-NEXT:     }
// IR-NEXT:     fn %63 @pass_long_long(%64 a: i32, %65 b: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i64>(read<i64>(%65), widen<i64>(read<i32>(%64)));
// IR-NEXT:     }
// IR-NEXT:     fn %66 @pass_vector(%67 value: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 4>>(%67);
// IR-NEXT:     }
// IR-NEXT:     fn %68 @variadic(%69 count: i32, ...) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return int_to_float<f64>(read<i32>(%69));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
