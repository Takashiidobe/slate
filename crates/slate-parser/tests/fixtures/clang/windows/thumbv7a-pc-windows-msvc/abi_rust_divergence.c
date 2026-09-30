// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct floats { float a, b; };
struct doubles { double a, b; };
struct mixed { float a; int b; };
struct five { float a, b, c, d, e; };

struct floats floats(struct floats value) { return value; }
struct doubles doubles(struct doubles value) { return value; }
struct mixed mixed(struct mixed value) { return value; }
struct five five(struct five value) { return value; }
_Complex float complex_float(_Complex float value) { return value; }
_Complex int complex_int(_Complex int value) { return value; }

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
// IR-NEXT:     type @type[[TYPE_floats:[0-9]+]] floats = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_doubles:[0-9]+]] doubles = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_mixed:[0-9]+]] mixed = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_five:[0-9]+]] five = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:         field2 c: f32;
// IR-NEXT:         field3 d: f32;
// IR-NEXT:         field4 e: f32;
// IR-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// IR-NEXT:     fn %[[VALUE_floats:[0-9]+]] @floats(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_floats]]) -> @type[[TYPE_floats]] [linkage=external] [abi=aapcs32_hard_float(coerce<f32, f32>) -> coerce<f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_floats]], reason=return>(read<@type[[TYPE_floats]]>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_doubles:[0-9]+]] @doubles(%[[VALUE_value_2:[0-9]+]] value: @type[[TYPE_doubles]]) -> @type[[TYPE_doubles]] [linkage=external] [abi=aapcs32_hard_float(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_doubles]], reason=return>(read<@type[[TYPE_doubles]]>(%[[VALUE_value_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_mixed:[0-9]+]] @mixed(%[[VALUE_value_3:[0-9]+]] value: @type[[TYPE_mixed]]) -> @type[[TYPE_mixed]] [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_mixed]], reason=return>(read<@type[[TYPE_mixed]]>(%[[VALUE_value_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_five:[0-9]+]] @five(%[[VALUE_value_4:[0-9]+]] value: @type[[TYPE_five]]) -> @type[[TYPE_five]] [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_five]], reason=return>(read<@type[[TYPE_five]]>(%[[VALUE_value_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_float:[0-9]+]] @complex_float(%[[VALUE_value_5:[0-9]+]] value: complex<f32>) -> complex<f32> [linkage=external] [abi=aapcs32_hard_float(coerce<f32, f32>) -> coerce<f32, f32>] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<f32>>(%[[VALUE_value_5]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_complex_int:[0-9]+]] @complex_int(%[[VALUE_value_6:[0-9]+]] value: complex<i32>) -> complex<i32> [linkage=external] [abi=aapcs32_hard_float(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return read<complex<i32>>(%[[VALUE_value_6]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
