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
// IR-NEXT:     type @type4 nested = struct {
// IR-NEXT:         field0 value: @type0;
// IR-NEXT:         field1 c: i32;
// IR-NEXT:     } [size=12, align=4, offsets=[0, 8]];
// IR-NEXT:     type @type5 large = struct {
// IR-NEXT:         field0 a: i64;
// IR-NEXT:         field1 b: i64;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     fn %6 @complex_float(%7 value: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%7);
// IR-NEXT:     }
// IR-NEXT:     fn %8 @complex_double(%9 value: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f64>>(%9);
// IR-NEXT:     }
// IR-NEXT:     fn %10 @complex_long_double(%11 value: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f80>>(%11);
// IR-NEXT:     }
// IR-NEXT:     fn %12 @record_pair(%13 value: @type0) -> @type0 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type0, reason=return>(read<@type0>(%13));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @record_float_pair(%15 value: @type1) -> @type1 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type1, reason=return>(read<@type1>(%15));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @record_double_pair(%17 value: @type2) -> @type2 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type2, reason=return>(read<@type2>(%17));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @record_mixed(%19 value: @type3) -> @type3 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type3, reason=return>(read<@type3>(%19));
// IR-NEXT:     }
// IR-NEXT:     fn %20 @record_nested(%21 value: @type4) -> @type4 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type4, reason=return>(read<@type4>(%21));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @record_large(%23 value: @type5) -> @type5 [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type5, reason=return>(read<@type5>(%23));
// IR-NEXT:     }
// IR-NEXT:     fn %24 @scalar(%25 value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i32>(%25);
// IR-NEXT:     }
// IR-NEXT:     fn %26 @wide_integer(%27 value: i128) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<i128>(%27);
// IR-NEXT:     }
// IR-NEXT:     fn %28 @forward(%29 callback: ptr<fn(complex<f64>) -> complex<f64>>, %30 value: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(scalar, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(read<ptr<fn(complex<f64>) -> complex<f64>>>(%29), read<complex<f64>>(%30));
// IR-NEXT:     }
// IR-NEXT:     fn %31 @variadic_sink(%34 tag: i32, ...) -> complex<f64> [linkage=external] [abi=sysv64(scalar) -> native_c];
// IR-NEXT:     fn %32 @variadic_forward(%33 value: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<complex<f64>, signature=fn(i32, ...) -> complex<f64>, abi=sysv64(scalar, native_c) -> native_c>(%31, const<i32>(1), read<complex<f64>>(%33));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
