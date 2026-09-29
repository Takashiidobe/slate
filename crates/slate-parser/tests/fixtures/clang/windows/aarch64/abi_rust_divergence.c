// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

struct floats { float a, b; };
struct doubles { double a, b, c; };
struct ints { int a, b; };

void sink(int, ...);
void variadic_floats(struct floats value) { sink(1, value); }
void variadic_doubles(struct doubles value) { sink(1, value); }
void variadic_ints(struct ints value) { sink(1, value); }
void variadic_complex(_Complex float value) { sink(1, value); }
struct floats fixed_floats(struct floats value) { return value; }

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
// IR-NEXT:     type @type[[TYPE_floats:[0-9]+]] floats = struct {
// IR-NEXT:         field0 a: f32;
// IR-NEXT:         field1 b: f32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE_doubles:[0-9]+]] doubles = struct {
// IR-NEXT:         field0 a: f64;
// IR-NEXT:         field1 b: f64;
// IR-NEXT:         field2 c: f64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     type @type[[TYPE_ints:[0-9]+]] ints = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: i32;
// IR-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE0:[0-9]+]] <unnamed>: i32, ...) -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_variadic_floats:[0-9]+]] @variadic_floats(%[[VALUE_value:[0-9]+]] value: @type[[TYPE_floats]]) -> void [linkage=external] [abi=win_arm64(native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=win_arm64(scalar, coerce<i64>) -> void>(%[[VALUE_sink]], const<i32>(1), copy<@type[[TYPE_floats]], reason=vararg>(read<@type[[TYPE_floats]]>(%[[VALUE_value]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_variadic_doubles:[0-9]+]] @variadic_doubles(%[[VALUE_value_2:[0-9]+]] value: @type[[TYPE_doubles]]) -> void [linkage=external] [abi=win_arm64(native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=win_arm64(scalar, byref<align=8>) -> void>(%[[VALUE_sink]], const<i32>(1), copy<@type[[TYPE_doubles]], reason=vararg>(read<@type[[TYPE_doubles]]>(%[[VALUE_value_2]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_variadic_ints:[0-9]+]] @variadic_ints(%[[VALUE_value_3:[0-9]+]] value: @type[[TYPE_ints]]) -> void [linkage=external] [abi=win_arm64(native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=win_arm64(scalar, native_c) -> void>(%[[VALUE_sink]], const<i32>(1), copy<@type[[TYPE_ints]], reason=vararg>(read<@type[[TYPE_ints]]>(%[[VALUE_value_3]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_variadic_complex:[0-9]+]] @variadic_complex(%[[VALUE_value_4:[0-9]+]] value: complex<f32>) -> void [linkage=external] [abi=win_arm64(native_c) -> void] [fallthrough=ret_void] {
// IR-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=win_arm64(scalar, coerce<i64>) -> void>(%[[VALUE_sink]], const<i32>(1), read<complex<f32>>(%[[VALUE_value_4]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_fixed_floats:[0-9]+]] @fixed_floats(%[[VALUE_value_5:[0-9]+]] value: @type[[TYPE_floats]]) -> @type[[TYPE_floats]] [linkage=external] [abi=win_arm64(native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-NEXT:         return copy<@type[[TYPE_floats]], reason=return>(read<@type[[TYPE_floats]]>(%[[VALUE_value_5]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
