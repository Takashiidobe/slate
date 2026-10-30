static double literal = 0.1;

double chain(double a, double b, double c) { return a * b / c; }
double stored(double a, double b, double c) {
  double t = a * b;
  return t / c;
}
double cast(double a, double b, double c) { return (double)(a * b) / c; }
int compare(double a, double b, double c) { return a * b == c; }
float single(float a, float b) { return a + b; }
double from_int(long long a, double b) { return a + b; }
double folded(void) { return 0.1 + 0.2; }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES STANDARD
// SLATE-FILECHECK-PREFIX-ARGS STANDARD -mfpmath=387 -std=c11
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -mfpmath=387 -std=gnu11
// SLATE-FILECHECK-DEFINES STORE
// SLATE-FILECHECK-PREFIX-ARGS STORE -mfpmath=387 -std=gnu11 -ffloat-store
// SLATE-FILECHECK-DEFINES NOSSE2
// SLATE-FILECHECK-PREFIX-ARGS NOSSE2 -mno-sse2 -std=c11
// SLATE-FILECHECK-DEFINES SSE
// SLATE-FILECHECK-PREFIX-ARGS SSE -mfpmath=sse

// SLATE-FILECHECK-BEGIN STANDARD
// STANDARD: module {
// STANDARD-NEXT:     target "x86_64-unknown-linux-gnu" {
// STANDARD-NEXT:         endian = little;
// STANDARD-NEXT:         pointer [size=8, align=8];
// STANDARD-NEXT:         stack_alignment = 16;
// STANDARD-NEXT:         long_double = f80;
// STANDARD-NEXT:         storage bool [size=1, align=1];
// STANDARD-NEXT:         storage i8, u8 [size=1, align=1];
// STANDARD-NEXT:         storage i16, u16 [size=2, align=2];
// STANDARD-NEXT:         storage i32, u32 [size=4, align=4];
// STANDARD-NEXT:         storage i64, u64 [size=8, align=8];
// STANDARD-NEXT:         storage i128, u128 [size=16, align=16];
// STANDARD-NEXT:         storage bf16 [size=2, align=2];
// STANDARD-NEXT:         storage f16 [size=2, align=2];
// STANDARD-NEXT:         storage f32 [size=4, align=4];
// STANDARD-NEXT:         storage f64 [size=8, align=8];
// STANDARD-NEXT:         storage f80 [size=16, align=16];
// STANDARD-NEXT:         storage f128 [size=16, align=16];
// STANDARD-NEXT:         storage d32 [size=4, align=4];
// STANDARD-NEXT:         storage d64 [size=8, align=8];
// STANDARD-NEXT:         storage d128 [size=16, align=16];
// STANDARD-NEXT:     }
// STANDARD-NEXT:     global %[[VALUE_literal:[0-9]+]] literal: f64 [storage=static] = float_narrow<f64>(const<f80>(0.100000000000000000001)) [linkage=internal];
// STANDARD-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STANDARD-NEXT:         return float_narrow<f64>(div<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a]])), float_widen<f80>(read<f64>(%[[VALUE_b]]))), float_widen<f80>(read<f64>(%[[VALUE_c]]))));
// STANDARD-NEXT:     }
// STANDARD-NEXT:     fn %[[VALUE_stored:[0-9]+]] @stored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STANDARD-NEXT:         let %[[VALUE_t:[0-9]+]] t: f64 [storage=automatic] = float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_2]])), float_widen<f80>(read<f64>(%[[VALUE_b_2]]))));
// STANDARD-NEXT:         return float_narrow<f64>(div<f80>(float_widen<f80>(read<f64>(%[[VALUE_t]])), float_widen<f80>(read<f64>(%[[VALUE_c_2]]))));
// STANDARD-NEXT:     }
// STANDARD-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c_3:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STANDARD-NEXT:         return float_narrow<f64>(div<f80>(float_widen<f80>(float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_3]])), float_widen<f80>(read<f64>(%[[VALUE_b_3]]))))), float_widen<f80>(read<f64>(%[[VALUE_c_3]]))));
// STANDARD-NEXT:     }
// STANDARD-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_a_4:[0-9]+]] a: f64, %[[VALUE_b_4:[0-9]+]] b: f64, %[[VALUE_c_4:[0-9]+]] c: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// STANDARD-NEXT:         return from_bool<i32>(eq<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_4]])), float_widen<f80>(read<f64>(%[[VALUE_b_4]]))), float_widen<f80>(read<f64>(%[[VALUE_c_4]]))));
// STANDARD-NEXT:     }
// STANDARD-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_a_5:[0-9]+]] a: f32, %[[VALUE_b_5:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// STANDARD-NEXT:         return float_narrow<f32>(add<f80>(float_widen<f80>(read<f32>(%[[VALUE_a_5]])), float_widen<f80>(read<f32>(%[[VALUE_b_5]]))));
// STANDARD-NEXT:     }
// STANDARD-NEXT:     fn %[[VALUE_from_int:[0-9]+]] @from_int(%[[VALUE_a_6:[0-9]+]] a: i64, %[[VALUE_b_6:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STANDARD-NEXT:         return float_narrow<f64>(add<f80>(int_to_float<f80>(read<i64>(%[[VALUE_a_6]])), float_widen<f80>(read<f64>(%[[VALUE_b_6]]))));
// STANDARD-NEXT:     }
// STANDARD-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STANDARD-NEXT:         return float_narrow<f64>(add<f80>(const<f80>(0.100000000000000000001), const<f80>(0.200000000000000000003)));
// STANDARD-NEXT:     }
// STANDARD-NEXT: }
// SLATE-FILECHECK-END STANDARD
// SLATE-FILECHECK-BEGIN FAST
// FAST: module {
// FAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// FAST-NEXT:         endian = little;
// FAST-NEXT:         pointer [size=8, align=8];
// FAST-NEXT:         stack_alignment = 16;
// FAST-NEXT:         long_double = f80;
// FAST-NEXT:         storage bool [size=1, align=1];
// FAST-NEXT:         storage i8, u8 [size=1, align=1];
// FAST-NEXT:         storage i16, u16 [size=2, align=2];
// FAST-NEXT:         storage i32, u32 [size=4, align=4];
// FAST-NEXT:         storage i64, u64 [size=8, align=8];
// FAST-NEXT:         storage i128, u128 [size=16, align=16];
// FAST-NEXT:         storage bf16 [size=2, align=2];
// FAST-NEXT:         storage f16 [size=2, align=2];
// FAST-NEXT:         storage f32 [size=4, align=4];
// FAST-NEXT:         storage f64 [size=8, align=8];
// FAST-NEXT:         storage f80 [size=16, align=16];
// FAST-NEXT:         storage f128 [size=16, align=16];
// FAST-NEXT:         storage d32 [size=4, align=4];
// FAST-NEXT:         storage d64 [size=8, align=8];
// FAST-NEXT:         storage d128 [size=16, align=16];
// FAST-NEXT:     }
// FAST-NEXT:     global %[[VALUE_literal:[0-9]+]] literal: f64 [storage=static] = const<f64>(0.1) [linkage=internal];
// FAST-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return float_narrow<f64>(div<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a]])), float_widen<f80>(read<f64>(%[[VALUE_b]]))), float_widen<f80>(read<f64>(%[[VALUE_c]]))));
// FAST-NEXT:     }
// FAST-NEXT:     fn %[[VALUE_stored:[0-9]+]] @stored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         let %[[VALUE_t:[0-9]+]] t: f64 [storage=automatic] = float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_2]])), float_widen<f80>(read<f64>(%[[VALUE_b_2]]))));
// FAST-NEXT:         return float_narrow<f64>(div<f80>(float_widen<f80>(read<f64>(%[[VALUE_t]])), float_widen<f80>(read<f64>(%[[VALUE_c_2]]))));
// FAST-NEXT:     }
// FAST-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c_3:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return float_narrow<f64>(div<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_3]])), float_widen<f80>(read<f64>(%[[VALUE_b_3]]))), float_widen<f80>(read<f64>(%[[VALUE_c_3]]))));
// FAST-NEXT:     }
// FAST-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_a_4:[0-9]+]] a: f64, %[[VALUE_b_4:[0-9]+]] b: f64, %[[VALUE_c_4:[0-9]+]] c: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return from_bool<i32>(eq<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_4]])), float_widen<f80>(read<f64>(%[[VALUE_b_4]]))), float_widen<f80>(read<f64>(%[[VALUE_c_4]]))));
// FAST-NEXT:     }
// FAST-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_a_5:[0-9]+]] a: f32, %[[VALUE_b_5:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return float_narrow<f32>(add<f80>(float_widen<f80>(read<f32>(%[[VALUE_a_5]])), float_widen<f80>(read<f32>(%[[VALUE_b_5]]))));
// FAST-NEXT:     }
// FAST-NEXT:     fn %[[VALUE_from_int:[0-9]+]] @from_int(%[[VALUE_a_6:[0-9]+]] a: i64, %[[VALUE_b_6:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return float_narrow<f64>(add<f80>(int_to_float<f80>(read<i64>(%[[VALUE_a_6]])), float_widen<f80>(read<f64>(%[[VALUE_b_6]]))));
// FAST-NEXT:     }
// FAST-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return const<f64>(0.30000000000000004);
// FAST-NEXT:     }
// FAST-NEXT: }
// SLATE-FILECHECK-END FAST
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
// STORE-NEXT:     global %[[VALUE_literal:[0-9]+]] literal: f64 [storage=static] = const<f64>(0.1) [linkage=internal];
// STORE-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STORE-NEXT:         return float_narrow<f64>(div<f80>(float_widen<f80>(float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a]])), float_widen<f80>(read<f64>(%[[VALUE_b]]))))), float_widen<f80>(read<f64>(%[[VALUE_c]]))));
// STORE-NEXT:     }
// STORE-NEXT:     fn %[[VALUE_stored:[0-9]+]] @stored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STORE-NEXT:         let %[[VALUE_t:[0-9]+]] t: f64 [storage=automatic] = float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_2]])), float_widen<f80>(read<f64>(%[[VALUE_b_2]]))));
// STORE-NEXT:         return float_narrow<f64>(div<f80>(float_widen<f80>(read<f64>(%[[VALUE_t]])), float_widen<f80>(read<f64>(%[[VALUE_c_2]]))));
// STORE-NEXT:     }
// STORE-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c_3:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STORE-NEXT:         return float_narrow<f64>(div<f80>(float_widen<f80>(float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_3]])), float_widen<f80>(read<f64>(%[[VALUE_b_3]]))))), float_widen<f80>(read<f64>(%[[VALUE_c_3]]))));
// STORE-NEXT:     }
// STORE-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_a_4:[0-9]+]] a: f64, %[[VALUE_b_4:[0-9]+]] b: f64, %[[VALUE_c_4:[0-9]+]] c: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// STORE-NEXT:         return from_bool<i32>(eq<f64>(float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_4]])), float_widen<f80>(read<f64>(%[[VALUE_b_4]])))), read<f64>(%[[VALUE_c_4]])));
// STORE-NEXT:     }
// STORE-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_a_5:[0-9]+]] a: f32, %[[VALUE_b_5:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// STORE-NEXT:         return float_narrow<f32>(add<f80>(float_widen<f80>(read<f32>(%[[VALUE_a_5]])), float_widen<f80>(read<f32>(%[[VALUE_b_5]]))));
// STORE-NEXT:     }
// STORE-NEXT:     fn %[[VALUE_from_int:[0-9]+]] @from_int(%[[VALUE_a_6:[0-9]+]] a: i64, %[[VALUE_b_6:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STORE-NEXT:         return float_narrow<f64>(add<f80>(float_widen<f80>(int_to_float<f64>(read<i64>(%[[VALUE_a_6]]))), float_widen<f80>(read<f64>(%[[VALUE_b_6]]))));
// STORE-NEXT:     }
// STORE-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// STORE-NEXT:         return const<f64>(0.30000000000000004);
// STORE-NEXT:     }
// STORE-NEXT: }
// SLATE-FILECHECK-END STORE
// SLATE-FILECHECK-BEGIN NOSSE2
// NOSSE2: module {
// NOSSE2-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOSSE2-NEXT:         endian = little;
// NOSSE2-NEXT:         pointer [size=8, align=8];
// NOSSE2-NEXT:         stack_alignment = 16;
// NOSSE2-NEXT:         long_double = f80;
// NOSSE2-NEXT:         storage bool [size=1, align=1];
// NOSSE2-NEXT:         storage i8, u8 [size=1, align=1];
// NOSSE2-NEXT:         storage i16, u16 [size=2, align=2];
// NOSSE2-NEXT:         storage i32, u32 [size=4, align=4];
// NOSSE2-NEXT:         storage i64, u64 [size=8, align=8];
// NOSSE2-NEXT:         storage i128, u128 [size=16, align=16];
// NOSSE2-NEXT:         storage bf16 [size=2, align=2];
// NOSSE2-NEXT:         storage f16 [size=2, align=2];
// NOSSE2-NEXT:         storage f32 [size=4, align=4];
// NOSSE2-NEXT:         storage f64 [size=8, align=8];
// NOSSE2-NEXT:         storage f80 [size=16, align=16];
// NOSSE2-NEXT:         storage f128 [size=16, align=16];
// NOSSE2-NEXT:         storage d32 [size=4, align=4];
// NOSSE2-NEXT:         storage d64 [size=8, align=8];
// NOSSE2-NEXT:         storage d128 [size=16, align=16];
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     global %[[VALUE_literal:[0-9]+]] literal: f64 [storage=static] = float_narrow<f64>(const<f80>(0.100000000000000000001)) [linkage=internal];
// NOSSE2-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return float_narrow<f64>(div<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a]])), float_widen<f80>(read<f64>(%[[VALUE_b]]))), float_widen<f80>(read<f64>(%[[VALUE_c]]))));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_stored:[0-9]+]] @stored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         let %[[VALUE_t:[0-9]+]] t: f64 [storage=automatic] = float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_2]])), float_widen<f80>(read<f64>(%[[VALUE_b_2]]))));
// NOSSE2-NEXT:         return float_narrow<f64>(div<f80>(float_widen<f80>(read<f64>(%[[VALUE_t]])), float_widen<f80>(read<f64>(%[[VALUE_c_2]]))));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c_3:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return float_narrow<f64>(div<f80>(float_widen<f80>(float_narrow<f64>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_3]])), float_widen<f80>(read<f64>(%[[VALUE_b_3]]))))), float_widen<f80>(read<f64>(%[[VALUE_c_3]]))));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_a_4:[0-9]+]] a: f64, %[[VALUE_b_4:[0-9]+]] b: f64, %[[VALUE_c_4:[0-9]+]] c: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return from_bool<i32>(eq<f80>(mul<f80>(float_widen<f80>(read<f64>(%[[VALUE_a_4]])), float_widen<f80>(read<f64>(%[[VALUE_b_4]]))), float_widen<f80>(read<f64>(%[[VALUE_c_4]]))));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_a_5:[0-9]+]] a: f32, %[[VALUE_b_5:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return add<f32>(read<f32>(%[[VALUE_a_5]]), read<f32>(%[[VALUE_b_5]]));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_from_int:[0-9]+]] @from_int(%[[VALUE_a_6:[0-9]+]] a: i64, %[[VALUE_b_6:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return float_narrow<f64>(add<f80>(int_to_float<f80>(read<i64>(%[[VALUE_a_6]])), float_widen<f80>(read<f64>(%[[VALUE_b_6]]))));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// NOSSE2-NEXT:         return float_narrow<f64>(add<f80>(const<f80>(0.100000000000000000001), const<f80>(0.200000000000000000003)));
// NOSSE2-NEXT:     }
// NOSSE2-NEXT: }
// SLATE-FILECHECK-END NOSSE2
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
// SSE-NEXT:     global %[[VALUE_literal:[0-9]+]] literal: f64 [storage=static] = const<f64>(0.1) [linkage=internal];
// SSE-NEXT:     fn %[[VALUE_chain:[0-9]+]] @chain(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return div<f64>(mul<f64>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_stored:[0-9]+]] @stored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         let %[[VALUE_t:[0-9]+]] t: f64 [storage=automatic] = mul<f64>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]]));
// SSE-NEXT:         return div<f64>(read<f64>(%[[VALUE_t]]), read<f64>(%[[VALUE_c_2]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_cast:[0-9]+]] @cast(%[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c_3:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return div<f64>(mul<f64>(read<f64>(%[[VALUE_a_3]]), read<f64>(%[[VALUE_b_3]])), read<f64>(%[[VALUE_c_3]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_compare:[0-9]+]] @compare(%[[VALUE_a_4:[0-9]+]] a: f64, %[[VALUE_b_4:[0-9]+]] b: f64, %[[VALUE_c_4:[0-9]+]] c: f64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return from_bool<i32>(eq<f64>(mul<f64>(read<f64>(%[[VALUE_a_4]]), read<f64>(%[[VALUE_b_4]])), read<f64>(%[[VALUE_c_4]])));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_single:[0-9]+]] @single(%[[VALUE_a_5:[0-9]+]] a: f32, %[[VALUE_b_5:[0-9]+]] b: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return add<f32>(read<f32>(%[[VALUE_a_5]]), read<f32>(%[[VALUE_b_5]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_from_int:[0-9]+]] @from_int(%[[VALUE_a_6:[0-9]+]] a: i64, %[[VALUE_b_6:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return add<f64>(int_to_float<f64>(read<i64>(%[[VALUE_a_6]])), read<f64>(%[[VALUE_b_6]]));
// SSE-NEXT:     }
// SSE-NEXT:     fn %[[VALUE_folded:[0-9]+]] @folded() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// SSE-NEXT:         return add<f64>(const<f64>(0.1), const<f64>(0.2));
// SSE-NEXT:     }
// SSE-NEXT: }
// SLATE-FILECHECK-END SSE
