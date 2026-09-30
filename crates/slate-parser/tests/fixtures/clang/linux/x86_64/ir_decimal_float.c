_Decimal32 a = 1.5DF;
_Decimal64 b = 1.0DD;
_Decimal128 c = 0.10DL;
_Decimal64 sum(_Decimal64 x, _Decimal64 y) { return x + y; }
_Decimal64 widen(_Decimal32 x, int i) { return x + i; }
_Decimal128 cast(double d) { return (_Decimal128)d; }
double back(_Decimal64 x) { return (double)x; }
int truthy(_Decimal32 x) { return x ? 1 : 0; }
int sizes = sizeof(_Decimal32) + sizeof(_Decimal64) + _Alignof(_Decimal128);

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
// IR-NEXT:     global %[[VALUE_a:[0-9]+]] a: d32 [storage=static] = const<d32>(1.5) [linkage=external];
// IR-NEXT:     global %[[VALUE_b:[0-9]+]] b: d64 [storage=static] = const<d64>(1.0) [linkage=external];
// IR-NEXT:     global %[[VALUE_c:[0-9]+]] c: d128 [storage=static] = const<d128>(0.10) [linkage=external];
// IR-NEXT:     global %[[VALUE_sizes:[0-9]+]] sizes: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(4), const<u64>(8)), const<u64>(16)))) [linkage=external];
// IR-NEXT:     fn %[[VALUE_sum:[0-9]+]] @sum(%[[VALUE_x:[0-9]+]] x: d64, %[[VALUE_y:[0-9]+]] y: d64) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<d64, rounding=nearest_even, exceptions=ignore, contract=on>(read<d64>(%[[VALUE_x]]), read<d64>(%[[VALUE_y]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_widen:[0-9]+]] @widen(%[[VALUE_x_2:[0-9]+]] x: d32, %[[VALUE_i:[0-9]+]] i: i32) -> d64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_widen<d64, reason=return>(add<d32, rounding=nearest_even, exceptions=ignore, contract=on>(read<d32>(%[[VALUE_x_2]]), int_to_float<d32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE_i]]))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_d:[0-9]+]] d: f64) -> d128 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_convert<d128, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<f64>(%[[VALUE_d]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_back:[0-9]+]] @back(%[[VALUE_x_3:[0-9]+]] x: d64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return float_convert<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(read<d64>(%[[VALUE_x_3]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_truthy:[0-9]+]] @truthy(%[[VALUE_x_4:[0-9]+]] x: d32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return conditional<i32>(ne<d32, exceptions=ignore>(read<d32>(%[[VALUE_x_4]]), const<d32>(0)), const<i32>(1), const<i32>(0));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
