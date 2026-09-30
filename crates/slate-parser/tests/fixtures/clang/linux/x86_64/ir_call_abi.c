// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct pair { int a; int b; };
struct float_pair { float a; float b; };
struct double_pair { double a; double b; };
struct mixed { int a; double b; };
struct nested { struct pair value; int c; };
struct large { long a; long b; long c; };

float _Complex complex_float(float _Complex value) { return value; }
double _Complex complex_double(double _Complex value) { return value; }
long double _Complex complex_long_double(long double _Complex value) { return value; }
struct pair record_pair(struct pair value) { return value; }
struct float_pair record_float_pair(struct float_pair value) { return value; }
struct double_pair record_double_pair(struct double_pair value) { return value; }
struct mixed record_mixed(struct mixed value) { return value; }
struct nested record_nested(struct nested value) { return value; }
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
// IR-NEXT:     type @type[[TYPE_nested:[0-9]+]] nested = struct {
// IR-NEXT:         field0 value: @type[[TYPE_pair]];
// IR-NEXT:         field1 c: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_large:[0-9]+]] large = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     fn %[[VALUE_complex_float:[0-9]+]] @complex_float(%[[VALUE_value:[0-9]+]] value: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%[[VALUE_value]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_double:[0-9]+]] @complex_double(%[[VALUE_value_2:[0-9]+]] value: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%[[VALUE_value_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_long_double:[0-9]+]] @complex_long_double(%[[VALUE_value_3:[0-9]+]] value: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f80>>(%[[VALUE_value_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_pair:[0-9]+]] @record_pair(%[[VALUE_value_4:[0-9]+]] value: @type[[TYPE_pair]]) -> @type[[TYPE_pair]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_pair]], reason=return>(read<@type[[TYPE_pair]]>(%[[VALUE_value_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_float_pair:[0-9]+]] @record_float_pair(%[[VALUE_value_5:[0-9]+]] value: @type[[TYPE_float_pair]]) -> @type[[TYPE_float_pair]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_float_pair]], reason=return>(read<@type[[TYPE_float_pair]]>(%[[VALUE_value_5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_double_pair:[0-9]+]] @record_double_pair(%[[VALUE_value_6:[0-9]+]] value: @type[[TYPE_double_pair]]) -> @type[[TYPE_double_pair]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_double_pair]], reason=return>(read<@type[[TYPE_double_pair]]>(%[[VALUE_value_6]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_mixed:[0-9]+]] @record_mixed(%[[VALUE_value_7:[0-9]+]] value: @type[[TYPE_mixed]]) -> @type[[TYPE_mixed]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_mixed]], reason=return>(read<@type[[TYPE_mixed]]>(%[[VALUE_value_7]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_nested:[0-9]+]] @record_nested(%[[VALUE_value_8:[0-9]+]] value: @type[[TYPE_nested]]) -> @type[[TYPE_nested]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_nested]], reason=return>(read<@type[[TYPE_nested]]>(%[[VALUE_value_8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_record_large:[0-9]+]] @record_large(%[[VALUE_value_9:[0-9]+]] value: @type[[TYPE_large]]) -> @type[[TYPE_large]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_large]], reason=return>(read<@type[[TYPE_large]]>(%[[VALUE_value_9]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_scalar:[0-9]+]] @scalar(%[[VALUE_value_10:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%[[VALUE_value_10]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_wide_integer:[0-9]+]] @wide_integer(%[[VALUE_value_11:[0-9]+]] value: i128) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i128>(%[[VALUE_value_11]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_forward:[0-9]+]] @forward(%[[VALUE_callback:[0-9]+]] callback: ptr<fn(complex<f64>) -> complex<f64>>, %[[VALUE_value_12:[0-9]+]] value: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(scalar, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(read<ptr<fn(complex<f64>) -> complex<f64>>>(%[[VALUE_callback]]), read<complex<f64>>(%[[VALUE_value_12]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_variadic_sink:[0-9]+]] @variadic_sink(%[[VALUE_tag:[0-9]+]] tag: i32, ...) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> native_c];
// IR-NEXT:     fn %[[VALUE_variadic_forward:[0-9]+]] @variadic_forward(%[[VALUE_value_13:[0-9]+]] value: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, signature=fn(i32, ...) -> complex<f64>, abi=sysv64(scalar, native_c) -> native_c>(%[[VALUE_variadic_sink]], const<i32>(1), read<complex<f64>>(%[[VALUE_value_13]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
