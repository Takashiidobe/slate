__bf16 value;
__bf16 vector __attribute__((vector_size(8)));

__bf16 same(__bf16 x, __bf16 y) { return x + y; }
_Float16 half(__bf16 x, _Float16 y) { return x + y; }
float single(__bf16 x, float y) { return x + y; }
__bf16 with_int(__bf16 x, int i) { return x + i; }
double with_literal(__bf16 x) { return x + 1.0; }

__bf16 narrow(_Float16 x) { return x; }
_Float16 widen(__bf16 x) { return x; }
__bf16 from_double(double d) { return (__bf16)d; }
int to_int(__bf16 x) { return (int)x; }
__bf16 from_int(int i) { return i; }

int compare(__bf16 x, _Float16 y) { return x > y; }
__bf16 negate(__bf16 x) { return -x; }
int truthy(__bf16 x) { return x ? 1 : 0; }

int variadic(int, ...);
int unpromoted(__bf16 x) { return variadic(0, x); }

__bf16 literal = 1.5bf16;
__bf16 literal_upper = 1.5BF16;
__bf16 literal_hex = 0x1.8p+1bf16;
__bf16 literal_max = 3.38953138925153547590470800371487867e+38BF16;
int literal_imaginary_size = sizeof(__typeof__(1.5bf16i));
int literal_generic = _Generic(1.5bf16, __bf16: 1, default: 0);
int literal_folded[(int)3.7bf16];

int classify = __builtin_classify_type(value);
int sizes = sizeof(__bf16) + _Alignof(__bf16);

// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

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
// IR-NEXT:     global %[[VALUE_value:[0-9]+]] value: bf16 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_vector:[0-9]+]] vector: vector<bf16, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_literal:[0-9]+]] literal: bf16 [storage=static] = const<bf16>(1.5) [linkage=external];
// IR-NEXT:     global %[[VALUE_literal_upper:[0-9]+]] literal_upper: bf16 [storage=static] = const<bf16>(1.5) [linkage=external];
// IR-NEXT:     global %[[VALUE_literal_hex:[0-9]+]] literal_hex: bf16 [storage=static] = const<bf16>(3) [linkage=external];
// IR-NEXT:     global %[[VALUE_literal_max:[0-9]+]] literal_max: bf16 [storage=static] = const<bf16>(3.39E+38) [linkage=external];
// IR-NEXT:     global %[[VALUE_literal_imaginary_size:[0-9]+]] literal_imaginary_size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))) [linkage=external];
// IR-NEXT:     global %[[VALUE_literal_generic:[0-9]+]] literal_generic: i32 [storage=static] = const<i32>(1) [linkage=external];
// IR-NEXT:     global %[[VALUE_literal_folded:[0-9]+]] literal_folded: array<i32, 3> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_classify:[0-9]+]] classify: i32 [storage=static] = const<i32>(8) [linkage=external];
// IR-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(2), const<u64>(2)))) [linkage=external];
// IR-NEXT:     fn %[[VALUE_same:[0-9]+]] @same(%[[VALUE_x:[0-9]+]] x: bf16, %[[VALUE_y:[0-9]+]] y: bf16) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<bf16, rounding=nearest_even, exceptions=ignore, contract=on>(read<bf16>(%[[VALUE_x]]), read<bf16>(%[[VALUE_y]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_half:[0-9]+]] @half(%[[VALUE_x_2:[0-9]+]] x: bf16, %[[VALUE_y_2:[0-9]+]] y: f16) -> f16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f16, rounding=nearest_even, exceptions=ignore, contract=on>(float_narrow<f16, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(read<bf16>(%[[VALUE_x_2]])), read<f16>(%[[VALUE_y_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_x_3:[0-9]+]] x: bf16, %[[VALUE_y_3:[0-9]+]] y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f32, reason=usual_arith>(read<bf16>(%[[VALUE_x_3]])), read<f32>(%[[VALUE_y_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_with_int:[0-9]+]] @with_int(%[[VALUE_x_4:[0-9]+]] x: bf16, %[[VALUE_i:[0-9]+]] i: i32) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<bf16, rounding=nearest_even, exceptions=ignore, contract=on>(read<bf16>(%[[VALUE_x_4]]), int_to_float<bf16, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_with_literal:[0-9]+]] @with_literal(%[[VALUE_x_5:[0-9]+]] x: bf16) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f64, reason=usual_arith>(read<bf16>(%[[VALUE_x_5]])), const<f64>(1.0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_narrow:[0-9]+]] @narrow(%[[VALUE_x_6:[0-9]+]] x: f16) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_narrow<bf16, reason=return, rounding=nearest_even, exceptions=ignore>(read<f16>(%[[VALUE_x_6]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_x_7:[0-9]+]] x: bf16) -> f16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_narrow<f16, reason=return, rounding=nearest_even, exceptions=ignore>(read<bf16>(%[[VALUE_x_7]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_double:[0-9]+]] @from_double(%[[VALUE_d:[0-9]+]] d: f64) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_narrow<bf16, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f64>(%[[VALUE_d]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_to_int:[0-9]+]] @to_int(%[[VALUE_x_8:[0-9]+]] x: bf16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<bf16>(%[[VALUE_x_8]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_from_int:[0-9]+]] @from_int(%[[VALUE_i_2:[0-9]+]] i: i32) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return int_to_float<bf16, reason=return, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i_2]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_x_9:[0-9]+]] x: bf16, %[[VALUE_y_4:[0-9]+]] y: f16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(gt<f16, exceptions=ignore>(float_narrow<f16, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(read<bf16>(%[[VALUE_x_9]])), read<f16>(%[[VALUE_y_4]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_negate:[0-9]+]] @negate(%[[VALUE_x_10:[0-9]+]] x: bf16) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<bf16>(read<bf16>(%[[VALUE_x_10]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_truthy:[0-9]+]] @truthy(%[[VALUE_x_11:[0-9]+]] x: bf16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<i32>(ne<bf16, exceptions=ignore>(read<bf16>(%[[VALUE_x_11]]), const<bf16>(0)), const<i32>(1), const<i32>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_variadic:[0-9]+]] @variadic(%[[VALUE0:[0-9]+]] <unnamed>: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_unpromoted:[0-9]+]] @unpromoted(%[[VALUE_x_12:[0-9]+]] x: bf16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, ...) -> i32>(%[[VALUE_variadic]], const<i32>(0), read<bf16>(%[[VALUE_x_12]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
