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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 value: bf16 [storage=static] [linkage=external];
// IR-NEXT:     global %1 vector: vector<bf16, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %36 classify: i32 [storage=static] = const<i32>(8) [linkage=external];
// IR-NEXT:     global %37 sizes: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(2), const<u64>(2)))) [linkage=external];
// IR-NEXT:     fn %2 @same(%3 x: bf16, %4 y: bf16) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<bf16, rounding=nearest_even, exceptions=ignore>(read<bf16>(%3), read<bf16>(%4));
// IR-NEXT:     }
// IR-NEXT:     fn %5 @half(%6 x: bf16, %7 y: f16) -> f16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f16, rounding=nearest_even, exceptions=ignore>(float_narrow<f16, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(read<bf16>(%6)), read<f16>(%7));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @single(%9 x: bf16, %10 y: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore>(float_widen<f32, reason=usual_arith>(read<bf16>(%9)), read<f32>(%10));
// IR-NEXT:     }
// IR-NEXT:     fn %11 @with_int(%12 x: bf16, %13 i: i32) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<bf16, rounding=nearest_even, exceptions=ignore>(read<bf16>(%12), int_to_float<bf16, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%13)));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @with_literal(%15 x: bf16) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<bf16>(%15)), const<f64>(1.0));
// IR-NEXT:     }
// IR-NEXT:     fn %16 @narrow(%17 x: f16) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_narrow<bf16, reason=return, rounding=nearest_even, exceptions=ignore>(read<f16>(%17));
// IR-NEXT:     }
// IR-NEXT:     fn %18 @widen(%19 x: bf16) -> f16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_narrow<f16, reason=return, rounding=nearest_even, exceptions=ignore>(read<bf16>(%19));
// IR-NEXT:     }
// IR-NEXT:     fn %20 @from_double(%21 d: f64) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_narrow<bf16, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f64>(%21));
// IR-NEXT:     }
// IR-NEXT:     fn %22 @to_int(%23 x: bf16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(read<bf16>(%23));
// IR-NEXT:     }
// IR-NEXT:     fn %24 @from_int(%25 i: i32) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return int_to_float<bf16, reason=return, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%25));
// IR-NEXT:     }
// IR-NEXT:     fn %26 @compare(%27 x: bf16, %28 y: f16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return from_bool<i32, reason=return>(gt<f16, exceptions=ignore>(float_narrow<f16, reason=usual_arith, rounding=nearest_even, exceptions=ignore>(read<bf16>(%27)), read<f16>(%28)));
// IR-NEXT:     }
// IR-NEXT:     fn %29 @negate(%30 x: bf16) -> bf16 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return neg<bf16>(read<bf16>(%30));
// IR-NEXT:     }
// IR-NEXT:     fn %31 @truthy(%32 x: bf16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<i32>(ne<bf16, exceptions=ignore>(read<bf16>(%32), const<bf16>(0)), const<i32>(1), const<i32>(0));
// IR-NEXT:     }
// IR-NEXT:     fn %33 @variadic(%38 <unnamed>: i32, ...) -> i32 [linkage=external];
// IR-NEXT:     fn %34 @unpromoted(%35 x: bf16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return call<i32, signature=fn(i32, ...) -> i32>(%33, const<i32>(0), read<bf16>(%35));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
