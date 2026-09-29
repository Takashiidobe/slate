// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct floats { float a, b; };
struct zero_tail { double d; float tail[0]; };
struct flexible { double d; float tail[]; };
struct empty {};
struct empty_then_float { struct empty e; float f; };
struct halves { _Float16 a, b; };
struct long_double { long double x; };

struct floats floats(struct floats value) { return value; }
struct zero_tail zero_tail(struct zero_tail value) { return value; }
struct flexible flexible(struct flexible value) { return value; }
struct empty_then_float empty_then_float(struct empty_then_float value) { return value; }
struct halves halves(struct halves value) { return value; }
struct long_double long_double(struct long_double value) { return value; }
_Complex _Float16 complex_half(_Complex _Float16 value) { return value; }
_Complex long double complex_long_double(_Complex long double value) { return value; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "aarch64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f128;
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
// IR-NEXT:     type @type[[TYPE_floats:[0-9]+]] floats = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_zero_tail:[0-9]+]] zero_tail = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:         field1 tail: array<f32, 0>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_flexible:[0-9]+]] flexible = struct {
// IR-NEXT:         field0 d: f64;
// IR-NEXT:         field1 tail: array<f32, incomplete>;
// IR-NEXT:     } [size=8, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_empty:[0-9]+]] empty = struct {
// IR-NEXT:     } [size=0, align=1, offsets=[]];
// IR-NEXT:     type @type[[TYPE_empty_then_float:[0-9]+]] empty_then_float = struct {
// IR-NEXT:         field0 e: @type[[TYPE_empty]];
// IR-NEXT:         field1 f: f32;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE_halves:[0-9]+]] halves = struct {
// IR-NEXT:         field0 a: f16;
// IR-NEXT:         field1 b: f16;
// IR-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type[[TYPE_long_double:[0-9]+]] long_double = struct {
// IR-NEXT:         field0 x: f128;
// IR-NEXT:     } [size=16, align=16, offsets=[0]];
// IR-NEXT:     fn %[[VALUE_floats:[0-9]+]] @floats(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_floats]]) -> @type[[TYPE_floats]] [linkage=external] [abi=aapcs64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_floats]], reason=return>(read<@type[[TYPE_floats]]>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_zero_tail:[0-9]+]] @zero_tail(%[[VALUE_value_2:[0-9]+]] value: @type[[TYPE_zero_tail]]) -> @type[[TYPE_zero_tail]] [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_zero_tail]], reason=return>(read<@type[[TYPE_zero_tail]]>(%[[VALUE_value_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_flexible:[0-9]+]] @flexible(%[[VALUE_value_3:[0-9]+]] value: @type[[TYPE_flexible]]) -> @type[[TYPE_flexible]] [linkage=external] [abi=aapcs64(coerce<i64>) -> coerce<i64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_flexible]], reason=return>(read<@type[[TYPE_flexible]]>(%[[VALUE_value_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_empty_then_float:[0-9]+]] @empty_then_float(%[[VALUE_value_4:[0-9]+]] value: @type[[TYPE_empty_then_float]]) -> @type[[TYPE_empty_then_float]] [linkage=external] [abi=aapcs64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_empty_then_float]], reason=return>(read<@type[[TYPE_empty_then_float]]>(%[[VALUE_value_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_halves:[0-9]+]] @halves(%[[VALUE_value_5:[0-9]+]] value: @type[[TYPE_halves]]) -> @type[[TYPE_halves]] [linkage=external] [abi=aapcs64(coerce<f16, f16>) -> coerce<f16, f16>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_halves]], reason=return>(read<@type[[TYPE_halves]]>(%[[VALUE_value_5]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_long_double:[0-9]+]] @long_double(%[[VALUE_value_6:[0-9]+]] value: @type[[TYPE_long_double]]) -> @type[[TYPE_long_double]] [linkage=external] [abi=aapcs64(coerce<f128>) -> coerce<f128>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_long_double]], reason=return>(read<@type[[TYPE_long_double]]>(%[[VALUE_value_6]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_half:[0-9]+]] @complex_half(%[[VALUE_value_7:[0-9]+]] value: complex<f16>) -> complex<f16> [linkage=external] [abi=aapcs64(coerce<f16, f16>) -> coerce<f16, f16>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f16>>(%[[VALUE_value_7]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_long_double:[0-9]+]] @complex_long_double(%[[VALUE_value_8:[0-9]+]] value: complex<f128>) -> complex<f128> [linkage=external] [abi=aapcs64(coerce<f128, f128>) -> coerce<f128, f128>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f128>>(%[[VALUE_value_8]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
