double ret_variadic(int n, ...) { return n; }
int named_variadic(double d, ...) { return (int)d; }
float floats(float a, int b) { return a + b; }
long double extended(long double x) { return x; }
int integers(int a, int b) { return a + b; }
double call_through(double (*f)(double)) { return f(1.0); }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES SSE
// SLATE-FILECHECK-PREFIX-ARGS SSE -msseregparm
// SLATE-FILECHECK-DEFINES MABI
// SLATE-FILECHECK-PREFIX-ARGS MABI -mabi=ms

// SLATE-FILECHECK-BEGIN SSE
// SSE: module {
// SSE-NEXT:     target "i686-unknown-linux-gnu" {
// SSE-NEXT:         endian = little;
// SSE-NEXT:         pointer [size=4, align=4];
// SSE-NEXT:         stack_alignment = 16;
// SSE-NEXT:         long_double = f80;
// SSE-NEXT:         storage bool [size=1, align=1];
// SSE-NEXT:         storage i8, u8 [size=1, align=1];
// SSE-NEXT:         storage i16, u16 [size=2, align=2];
// SSE-NEXT:         storage i32, u32 [size=4, align=4];
// SSE-NEXT:         storage i64, u64 [size=8, align=4];
// SSE-NEXT:         storage i128, u128 [size=16, align=16];
// SSE-NEXT:         storage bf16 [size=2, align=2];
// SSE-NEXT:         storage f16 [size=2, align=2];
// SSE-NEXT:         storage f32 [size=4, align=4];
// SSE-NEXT:         storage f64 [size=8, align=4];
// SSE-NEXT:         storage f80 [size=12, align=4];
// SSE-NEXT:         storage f128 [size=16, align=16];
// SSE-NEXT:         storage d32 [size=4, align=4];
// SSE-NEXT:         storage d64 [size=8, align=8];
// SSE-NEXT:         storage d128 [size=16, align=16];
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_ret_variadic:[0-9]+]] @ret_variadic(%[[VALUE_n:[0-9]+]] n: i32, ...) -> f64 [linkage=external] [abi=x86_cdecl sseregparm(scalar) -> scalar] [fallthrough=ub_if_used] {
// SSE-NEXT:         return int_to_float<f64>(read<i32>(%[[VALUE_n]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_named_variadic:[0-9]+]] @named_variadic(%[[VALUE_d:[0-9]+]] d: f64, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return float_to_int<i32>(read<f64>(%[[VALUE_d]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_floats:[0-9]+]] @floats(%[[VALUE_a:[0-9]+]] a: f32, %[[VALUE_b:[0-9]+]] b: i32) -> f32 [linkage=external] [abi=x86_cdecl sseregparm(scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// SSE-NEXT:         return float_narrow<f32>(add<f80>(float_widen<f80>(read<f32>(%[[VALUE_a]])), int_to_float<f80>(read<i32>(%[[VALUE_b]]))));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_extended:[0-9]+]] @extended(%[[VALUE_x:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return read<f80>(%[[VALUE_x]]);
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_integers:[0-9]+]] @integers(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_call_through:[0-9]+]] @call_through(%[[VALUE_f:[0-9]+]] f: ptr<fn(f64) -> f64>) -> f64 [linkage=external] [abi=x86_cdecl sseregparm(scalar) -> scalar] [fallthrough=ub_if_used] {
// SSE-NEXT:         return call<f64, abi=x86_cdecl sseregparm(scalar) -> scalar>(read<ptr<fn(f64) -> f64>>(%[[VALUE_f]]), const<f64>(1.0));
// SSE-NEXT:     }
// SSE-NEXT: }
// SLATE-FILECHECK-END SSE
// SLATE-FILECHECK-BEGIN MABI
// MABI: module {
// MABI-NEXT:     target "i686-unknown-linux-gnu" {
// MABI-NEXT:         endian = little;
// MABI-NEXT:         pointer [size=4, align=4];
// MABI-NEXT:         stack_alignment = 16;
// MABI-NEXT:         long_double = f80;
// MABI-NEXT:         storage bool [size=1, align=1];
// MABI-NEXT:         storage i8, u8 [size=1, align=1];
// MABI-NEXT:         storage i16, u16 [size=2, align=2];
// MABI-NEXT:         storage i32, u32 [size=4, align=4];
// MABI-NEXT:         storage i64, u64 [size=8, align=4];
// MABI-NEXT:         storage i128, u128 [size=16, align=16];
// MABI-NEXT:         storage bf16 [size=2, align=2];
// MABI-NEXT:         storage f16 [size=2, align=2];
// MABI-NEXT:         storage f32 [size=4, align=4];
// MABI-NEXT:         storage f64 [size=8, align=4];
// MABI-NEXT:         storage f80 [size=12, align=4];
// MABI-NEXT:         storage f128 [size=16, align=16];
// MABI-NEXT:         storage d32 [size=4, align=4];
// MABI-NEXT:         storage d64 [size=8, align=8];
// MABI-NEXT:         storage d128 [size=16, align=16];
// MABI-NEXT:     }
// MABI-NEXT:     fn %[[VALUE_ret_variadic:[0-9]+]] @ret_variadic(%[[VALUE_n:[0-9]+]] n: i32, ...) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// MABI-NEXT:         return int_to_float<f64>(read<i32>(%[[VALUE_n]]));
// MABI-NEXT:     }
// MABI-NEXT:     fn %[[VALUE_named_variadic:[0-9]+]] @named_variadic(%[[VALUE_d:[0-9]+]] d: f64, ...) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// MABI-NEXT:         return float_to_int<i32>(read<f64>(%[[VALUE_d]]));
// MABI-NEXT:     }
// MABI-NEXT:     fn %[[VALUE_floats:[0-9]+]] @floats(%[[VALUE_a:[0-9]+]] a: f32, %[[VALUE_b:[0-9]+]] b: i32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// MABI-NEXT:         return float_narrow<f32>(add<f80>(float_widen<f80>(read<f32>(%[[VALUE_a]])), int_to_float<f80>(read<i32>(%[[VALUE_b]]))));
// MABI-NEXT:     }
// MABI-NEXT:     fn %[[VALUE_extended:[0-9]+]] @extended(%[[VALUE_x:[0-9]+]] x: f80) -> f80 [linkage=external] [fallthrough=ub_if_used] {
// MABI-NEXT:         return read<f80>(%[[VALUE_x]]);
// MABI-NEXT:     }
// MABI-NEXT:     fn %[[VALUE_integers:[0-9]+]] @integers(%[[VALUE_a_2:[0-9]+]] a: i32, %[[VALUE_b_2:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// MABI-NEXT:         return add<i32>(read<i32>(%[[VALUE_a_2]]), read<i32>(%[[VALUE_b_2]]));
// MABI-NEXT:     }
// MABI-NEXT:     fn %[[VALUE_call_through:[0-9]+]] @call_through(%[[VALUE_f:[0-9]+]] f: ptr<fn(f64) -> f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// MABI-NEXT:         return call<f64>(read<ptr<fn(f64) -> f64>>(%[[VALUE_f]]), const<f64>(1.0));
// MABI-NEXT:     }
// MABI-NEXT: }
// SLATE-FILECHECK-END MABI
