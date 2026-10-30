double chain(double a, double b, double c) { return a * b / c; }
int eval_method = __FLT_EVAL_METHOD__;

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES SSE
// SLATE-FILECHECK-PREFIX-ARGS SSE -mfpmath=sse
// SLATE-FILECHECK-DEFINES STORE
// SLATE-FILECHECK-PREFIX-ARGS STORE -ffloat-store
// SLATE-FILECHECK-DEFINES X87
// SLATE-FILECHECK-PREFIX-ARGS X87 -mno-sse -mfpmath=387

// SLATE-FILECHECK-BEGIN SSE
// SSE: module {
// SSE-NEXT:     target "x86_64-unknown-linux-gnu" {
// SSE-NEXT:         endian = little;
// SSE-NEXT:         pointer [size=8, align=8];
// SSE-NEXT:         stack_alignment = 16;
// SSE-NEXT:         long_double = f80;
// SSE-NEXT:         storage bool [size=1, align=1];
// SSE-NEXT:         storage i8, u8 [size=1, align=1];
// SSE-NEXT:         storage i16, u16 [size=2, align=2];
// SSE-NEXT:         storage i32, u32 [size=4, align=4];
// SSE-NEXT:         storage i64, u64 [size=8, align=8];
// SSE-NEXT:         storage i128, u128 [size=16, align=16];
// SSE-NEXT:         storage bf16 [size=2, align=2];
// SSE-NEXT:         storage f16 [size=2, align=2];
// SSE-NEXT:         storage f32 [size=4, align=4];
// SSE-NEXT:         storage f64 [size=8, align=8];
// SSE-NEXT:         storage f80 [size=16, align=16];
// SSE-NEXT:         storage f128 [size=16, align=16];
// SSE-NEXT:         storage d32 [size=4, align=4];
// SSE-NEXT:         storage d64 [size=8, align=8];
// SSE-NEXT:         storage d128 [size=16, align=16];
// SSE-NEXT:     }
// SSE-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// SSE-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return div<f64>(mul<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// SSE-NEXT:     }
// SSE-NEXT: }
// SLATE-FILECHECK-END SSE
// SLATE-FILECHECK-BEGIN STORE
// STORE: module {
// STORE-NEXT:     target "x86_64-unknown-linux-gnu" {
// STORE-NEXT:         endian = little;
// STORE-NEXT:         pointer [size=8, align=8];
// STORE-NEXT:         stack_alignment = 16;
// STORE-NEXT:         long_double = f80;
// STORE-NEXT:         storage bool [size=1, align=1];
// STORE-NEXT:         storage i8, u8 [size=1, align=1];
// STORE-NEXT:         storage i16, u16 [size=2, align=2];
// STORE-NEXT:         storage i32, u32 [size=4, align=4];
// STORE-NEXT:         storage i64, u64 [size=8, align=8];
// STORE-NEXT:         storage i128, u128 [size=16, align=16];
// STORE-NEXT:         storage bf16 [size=2, align=2];
// STORE-NEXT:         storage f16 [size=2, align=2];
// STORE-NEXT:         storage f32 [size=4, align=4];
// STORE-NEXT:         storage f64 [size=8, align=8];
// STORE-NEXT:         storage f80 [size=16, align=16];
// STORE-NEXT:         storage f128 [size=16, align=16];
// STORE-NEXT:         storage d32 [size=4, align=4];
// STORE-NEXT:         storage d64 [size=8, align=8];
// STORE-NEXT:         storage d128 [size=16, align=16];
// STORE-NEXT:     }
// STORE-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(0) [linkage=external];
// STORE-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STORE-NEXT:         return div<f64>(mul<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// STORE-NEXT:     }
// STORE-NEXT: }
// SLATE-FILECHECK-END STORE
// SLATE-FILECHECK-BEGIN X87
// X87: module {
// X87-NEXT:     target "x86_64-unknown-linux-gnu" {
// X87-NEXT:         endian = little;
// X87-NEXT:         pointer [size=8, align=8];
// X87-NEXT:         stack_alignment = 16;
// X87-NEXT:         long_double = f80;
// X87-NEXT:         storage bool [size=1, align=1];
// X87-NEXT:         storage i8, u8 [size=1, align=1];
// X87-NEXT:         storage i16, u16 [size=2, align=2];
// X87-NEXT:         storage i32, u32 [size=4, align=4];
// X87-NEXT:         storage i64, u64 [size=8, align=8];
// X87-NEXT:         storage i128, u128 [size=16, align=16];
// X87-NEXT:         storage bf16 [size=2, align=2];
// X87-NEXT:         storage f16 [size=2, align=2];
// X87-NEXT:         storage f32 [size=4, align=4];
// X87-NEXT:         storage f64 [size=8, align=8];
// X87-NEXT:         storage f80 [size=16, align=16];
// X87-NEXT:         storage f128 [size=16, align=16];
// X87-NEXT:         storage d32 [size=4, align=4];
// X87-NEXT:         storage d64 [size=8, align=8];
// X87-NEXT:         storage d128 [size=16, align=16];
// X87-NEXT:     }
// X87-NEXT:     global %[[VALUE_eval_method:[0-9]+]] eval_method: i32 [storage=static] = const<i32>(2) [linkage=external];
// X87-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// X87-NEXT:         return div<f64>(mul<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// X87-NEXT:     }
// X87-NEXT: }
// SLATE-FILECHECK-END X87
