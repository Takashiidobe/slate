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
// IR-NEXT:     target "armv7-unknown-linux-gnueabihf" {
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
// IR-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_float_pair:[0-9]+]] float_pair = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_double_pair:[0-9]+]] double_pair = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_mixed:[0-9]+]] mixed = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_large:[0-9]+]] large = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:         field2 c: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-NEXT:     type @type[[TYPE_v1c:[0-9]+]] v1c = vector<u8, 1>;
// IR-NEXT:     type @type[[TYPE_v2ss:[0-9]+]] v2ss = vector<i16, 2>;
// IR-NEXT:     type @type[[TYPE_v2si:[0-9]+]] v2si = vector<i32, 2>;
// IR-NEXT:     type @type[[TYPE_v1df:[0-9]+]] v1df = vector<f64, 1>;
// IR-NEXT:     type @type[[TYPE_v4si:[0-9]+]] v4si = vector<i32, 4>;
// IR-NEXT:     type @type[[TYPE_v8si:[0-9]+]] v8si = vector<i32, 8>;
// IR-NEXT:     fn %[[VALUE_complex_float:[0-9]+]] @complex_float(%[[VALUE_value:[0-9]+]] value: complex<f32>) -> complex<f32> [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%[[VALUE_value]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_double:[0-9]+]] @complex_double(%[[VALUE_value_2:[0-9]+]] value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%[[VALUE_value_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_long_double:[0-9]+]] @complex_long_double(%[[VALUE_value_3:[0-9]+]] value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%[[VALUE_value_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_pair:[0-9]+]] @record_pair(%[[VALUE_value_4:[0-9]+]] value: @type[[TYPE_pair]]) -> @type[[TYPE_pair]] [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_pair]], reason=return>(read<@type[[TYPE_pair]]>(%[[VALUE_value_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_float_pair:[0-9]+]] @record_float_pair(%[[VALUE_value_5:[0-9]+]] value: @type[[TYPE_float_pair]]) -> @type[[TYPE_float_pair]] [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_float_pair]], reason=return>(read<@type[[TYPE_float_pair]]>(%[[VALUE_value_5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_double_pair:[0-9]+]] @record_double_pair(%[[VALUE_value_6:[0-9]+]] value: @type[[TYPE_double_pair]]) -> @type[[TYPE_double_pair]] [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_double_pair]], reason=return>(read<@type[[TYPE_double_pair]]>(%[[VALUE_value_6]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_mixed:[0-9]+]] @record_mixed(%[[VALUE_value_7:[0-9]+]] value: @type[[TYPE_mixed]]) -> @type[[TYPE_mixed]] [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_mixed]], reason=return>(read<@type[[TYPE_mixed]]>(%[[VALUE_value_7]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_large:[0-9]+]] @record_large(%[[VALUE_value_8:[0-9]+]] value: @type[[TYPE_large]]) -> @type[[TYPE_large]] [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_large]], reason=return>(read<@type[[TYPE_large]]>(%[[VALUE_value_8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_scalar:[0-9]+]] @scalar(%[[VALUE_value_9:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_value_9]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_forward:[0-9]+]] @forward(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(complex<f64>) -> complex<f64>>, %[[VALUE_value_10:[0-9]+]] value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(scalar, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=aapcs32_hard_float(native_c) -> native_c>(read<ptr<fn(complex<f64>) -> complex<f64>>>(%[[VALUE_callback]]), read<complex<f64>>(%[[VALUE_value_10]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_variadic_sink:[0-9]+]] @variadic_sink(%[[VALUE_tag:[0-9]+]] tag: i32, ...) -> complex<f64> [linkage=external] [abi=aapcs32(scalar) -> native_c];
// IR-NEXT:     fn %[[VALUE_variadic_forward:[0-9]+]] @variadic_forward(%[[VALUE_value_11:[0-9]+]] value: complex<f64>) -> complex<f64> [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, signature=fn(i32, ...) -> complex<f64>, abi=aapcs32(scalar, native_c) -> native_c>(%[[VALUE_variadic_sink]], const<i32>(1), read<complex<f64>>(%[[VALUE_value_11]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_byte:[0-9]+]] @vector_byte(%[[VALUE_value_12:[0-9]+]] value: vector<u8, 1>) -> vector<u8, 1> [linkage=external] [abi=aapcs32_hard_float(coerce<i32>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<u8, 1>>(%[[VALUE_value_12]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_word:[0-9]+]] @vector_word(%[[VALUE_value_13:[0-9]+]] value: vector<i16, 2>) -> vector<i16, 2> [linkage=external] [abi=aapcs32_hard_float(coerce<i32>) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i16, 2>>(%[[VALUE_value_13]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_integer_pair:[0-9]+]] @vector_integer_pair(%[[VALUE_value_14:[0-9]+]] value: vector<i32, 2>) -> vector<i32, 2> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 2>>(%[[VALUE_value_14]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_one_double:[0-9]+]] @vector_one_double(%[[VALUE_value_15:[0-9]+]] value: vector<f64, 1>) -> vector<f64, 1> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<f64, 1>>(%[[VALUE_value_15]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_128:[0-9]+]] @vector_128(%[[VALUE_value_16:[0-9]+]] value: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=aapcs32_hard_float(direct) -> direct] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 4>>(%[[VALUE_value_16]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_256:[0-9]+]] @vector_256(%[[VALUE_value_17:[0-9]+]] value: vector<i32, 8>) -> vector<i32, 8> [linkage=external] [abi=aapcs32_hard_float(direct) -> sret<align=8>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<vector<i32, 8>>(%[[VALUE_value_17]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_vector_sink:[0-9]+]] @vector_sink(%[[VALUE_tag_2:[0-9]+]] tag: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_vector_variadic:[0-9]+]] @vector_variadic(%[[VALUE_narrow:[0-9]+]] narrow: vector<i32, 4>, %[[VALUE_wide:[0-9]+]] wide: vector<i32, 8>) -> i32 [linkage=external] [abi=aapcs32_hard_float(direct, direct) -> scalar] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, ...) -> i32, abi=aapcs32(scalar, direct, direct) -> scalar>(%[[VALUE_vector_sink]], const<i32>(1), read<vector<i32, 4>>(%[[VALUE_narrow]]), read<vector<i32, 8>>(%[[VALUE_wide]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
