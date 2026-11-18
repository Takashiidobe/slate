double chain(double a, double b, double c) { return a * b / c; }
double cast(double a, double b, double c) { return (double)(a * b) / c; }
float single(float a, float b) { return a + b; }
double folded(void) { return 0.1 + 0.2; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES NOSSE
// SLATE-FILECHECK-PREFIX-ARGS NOSSE -mno-sse -std=c11 -ffloat-store -fexcess-precision=standard
// SLATE-FILECHECK-DEFINES NOSSE2
// SLATE-FILECHECK-PREFIX-ARGS NOSSE2 -mno-sse2
// SLATE-FILECHECK-DEFINES SSE
// SLATE-FILECHECK-PREFIX-ARGS SSE

// SLATE-FILECHECK-BEGIN NOSSE
// NOSSE: module {
// NOSSE-NEXT:     target "i686-unknown-linux-gnu" {
// NOSSE-NEXT:         endian = little;
// NOSSE-NEXT:         pointer [size=4, align=4];
// NOSSE-NEXT:         stack_alignment = 16;
// NOSSE-NEXT:         long_double = f80;
// NOSSE-NEXT:         storage bool [size=1, align=1];
// NOSSE-NEXT:         storage i8, u8 [size=1, align=1];
// NOSSE-NEXT:         storage i16, u16 [size=2, align=2];
// NOSSE-NEXT:         storage i32, u32 [size=4, align=4];
// NOSSE-NEXT:         storage i64, u64 [size=8, align=4];
// NOSSE-NEXT:         storage i128, u128 [size=16, align=16];
// NOSSE-NEXT:         storage bf16 [size=2, align=2];
// NOSSE-NEXT:         storage f16 [size=2, align=2];
// NOSSE-NEXT:         storage f32 [size=4, align=4];
// NOSSE-NEXT:         storage f64 [size=8, align=4];
// NOSSE-NEXT:         storage f80 [size=12, align=4];
// NOSSE-NEXT:         storage f128 [size=16, align=16];
// NOSSE-NEXT:         storage d32 [size=4, align=4];
// NOSSE-NEXT:         storage d64 [size=8, align=8];
// NOSSE-NEXT:         storage d128 [size=16, align=16];
// NOSSE-NEXT:     }
// NOSSE-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE-NEXT:         return float_narrow<f64>(div<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a]])), float_widen<f80>(read<f64>(%[[VALUE_b]]))), float_widen<f80>(read<f64>(%[[VALUE_c]]))));
// NOSSE-NEXT:     }
// NOSSE-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE-NEXT:         return float_narrow<f64>(div<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_2]])), float_widen<f80>(read<f64>(%[[VALUE_b_2]]))), float_widen<f80>(read<f64>(%[[VALUE_c_2]]))));
// NOSSE-NEXT:     }
// NOSSE-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_a_3:[0-9]+]] a: f32, %[[VALUE_b_3:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE-NEXT:         return float_narrow<f32>(add<f80>(float_widen<f80>(read<f32>(%[[VALUE_a_3]])), float_widen<f80>(read<f32>(%[[VALUE_b_3]]))));
// NOSSE-NEXT:     }
// NOSSE-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE-NEXT:         return const<f64>(0.30000000000000004);
// NOSSE-NEXT:     }
// NOSSE-NEXT: }
// SLATE-FILECHECK-END NOSSE
// SLATE-FILECHECK-BEGIN NOSSE2
// NOSSE2: module {
// NOSSE2-NEXT:     target "i686-unknown-linux-gnu" {
// NOSSE2-NEXT:         endian = little;
// NOSSE2-NEXT:         pointer [size=4, align=4];
// NOSSE2-NEXT:         stack_alignment = 16;
// NOSSE2-NEXT:         long_double = f80;
// NOSSE2-NEXT:         storage bool [size=1, align=1];
// NOSSE2-NEXT:         storage i8, u8 [size=1, align=1];
// NOSSE2-NEXT:         storage i16, u16 [size=2, align=2];
// NOSSE2-NEXT:         storage i32, u32 [size=4, align=4];
// NOSSE2-NEXT:         storage i64, u64 [size=8, align=4];
// NOSSE2-NEXT:         storage i128, u128 [size=16, align=16];
// NOSSE2-NEXT:         storage bf16 [size=2, align=2];
// NOSSE2-NEXT:         storage f16 [size=2, align=2];
// NOSSE2-NEXT:         storage f32 [size=4, align=4];
// NOSSE2-NEXT:         storage f64 [size=8, align=4];
// NOSSE2-NEXT:         storage f80 [size=12, align=4];
// NOSSE2-NEXT:         storage f128 [size=16, align=16];
// NOSSE2-NEXT:         storage d32 [size=4, align=4];
// NOSSE2-NEXT:         storage d64 [size=8, align=8];
// NOSSE2-NEXT:         storage d128 [size=16, align=16];
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return float_narrow<f64>(div<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a]])), float_widen<f80>(read<f64>(%[[VALUE_b]]))), float_widen<f80>(read<f64>(%[[VALUE_c]]))));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return float_narrow<f64>(div<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_2]])), float_widen<f80>(read<f64>(%[[VALUE_b_2]]))), float_widen<f80>(read<f64>(%[[VALUE_c_2]]))));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_a_3:[0-9]+]] a: f32, %[[VALUE_b_3:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return add<f32>(read<f32>(%[[VALUE_a_3]]), read<f32>(%[[VALUE_b_3]]));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return const<f64>(0.30000000000000004);
// NOSSE2-NEXT:     }
// NOSSE2-NEXT: }
// SLATE-FILECHECK-END NOSSE2
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
// SSE-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return div<f64>(mul<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return div<f64>(mul<f64>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_a_3:[0-9]+]] a: f32, %[[VALUE_b_3:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return add<f32>(read<f32>(%[[VALUE_a_3]]), read<f32>(%[[VALUE_b_3]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return add<f64>(const<f64>(0.1), const<f64>(0.2));
// SSE-NEXT:     }
// SSE-NEXT: }
// SLATE-FILECHECK-END SSE
