double fused(double a, double b, double c) { return a * b + c; }

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES ISO
// SLATE-FILECHECK-PREFIX-ARGS ISO -std=c11
// SLATE-FILECHECK-DEFINES ON
// SLATE-FILECHECK-PREFIX-ARGS ON -ffp-contract=on
// SLATE-FILECHECK-DEFINES ISO_FAST
// SLATE-FILECHECK-PREFIX-ARGS ISO_FAST -std=c11 -ffp-contract=fast
// SLATE-FILECHECK-DEFINES ISO_FAST_MATH
// SLATE-FILECHECK-PREFIX-ARGS ISO_FAST_MATH -std=c11 -ffast-math
// SLATE-FILECHECK-DEFINES EXPLICIT_WINS
// SLATE-FILECHECK-PREFIX-ARGS EXPLICIT_WINS -ffast-math -ffp-contract=off

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ISO
// ISO: module {
// ISO-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO-NEXT:         endian = little;
// ISO-NEXT:         pointer [size=8, align=8];
// ISO-NEXT:         stack_alignment = 16;
// ISO-NEXT:         long_double = f80;
// ISO-NEXT:         storage bool [size=1, align=1];
// ISO-NEXT:         storage i8, u8 [size=1, align=1];
// ISO-NEXT:         storage i16, u16 [size=2, align=2];
// ISO-NEXT:         storage i32, u32 [size=4, align=4];
// ISO-NEXT:         storage i64, u64 [size=8, align=8];
// ISO-NEXT:         storage i128, u128 [size=16, align=16];
// ISO-NEXT:         storage bf16 [size=2, align=2];
// ISO-NEXT:         storage f16 [size=2, align=2];
// ISO-NEXT:         storage f32 [size=4, align=4];
// ISO-NEXT:         storage f64 [size=8, align=8];
// ISO-NEXT:         storage f80 [size=16, align=16];
// ISO-NEXT:         storage f128 [size=16, align=16];
// ISO-NEXT:         storage d32 [size=4, align=4];
// ISO-NEXT:         storage d64 [size=8, align=8];
// ISO-NEXT:         storage d128 [size=16, align=16];
// ISO-NEXT:     }
// ISO-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// ISO-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=off>(mul<f64, rounding=nearest_even, exceptions=observable, contract=off>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// ISO-NEXT:     }
// ISO-NEXT: }
// SLATE-FILECHECK-END ISO
// SLATE-FILECHECK-BEGIN ON
// ON: module {
// ON-NEXT:     target "x86_64-unknown-linux-gnu" {
// ON-NEXT:         endian = little;
// ON-NEXT:         pointer [size=8, align=8];
// ON-NEXT:         stack_alignment = 16;
// ON-NEXT:         long_double = f80;
// ON-NEXT:         storage bool [size=1, align=1];
// ON-NEXT:         storage i8, u8 [size=1, align=1];
// ON-NEXT:         storage i16, u16 [size=2, align=2];
// ON-NEXT:         storage i32, u32 [size=4, align=4];
// ON-NEXT:         storage i64, u64 [size=8, align=8];
// ON-NEXT:         storage i128, u128 [size=16, align=16];
// ON-NEXT:         storage bf16 [size=2, align=2];
// ON-NEXT:         storage f16 [size=2, align=2];
// ON-NEXT:         storage f32 [size=4, align=4];
// ON-NEXT:         storage f64 [size=8, align=8];
// ON-NEXT:         storage f80 [size=16, align=16];
// ON-NEXT:         storage f128 [size=16, align=16];
// ON-NEXT:         storage d32 [size=4, align=4];
// ON-NEXT:         storage d64 [size=8, align=8];
// ON-NEXT:         storage d128 [size=16, align=16];
// ON-NEXT:     }
// ON-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// ON-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=on>(mul<f64, rounding=nearest_even, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// ON-NEXT:     }
// ON-NEXT: }
// SLATE-FILECHECK-END ON
// SLATE-FILECHECK-BEGIN ISO_FAST
// ISO_FAST: module {
// ISO_FAST-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO_FAST-NEXT:         endian = little;
// ISO_FAST-NEXT:         pointer [size=8, align=8];
// ISO_FAST-NEXT:         stack_alignment = 16;
// ISO_FAST-NEXT:         long_double = f80;
// ISO_FAST-NEXT:         storage bool [size=1, align=1];
// ISO_FAST-NEXT:         storage i8, u8 [size=1, align=1];
// ISO_FAST-NEXT:         storage i16, u16 [size=2, align=2];
// ISO_FAST-NEXT:         storage i32, u32 [size=4, align=4];
// ISO_FAST-NEXT:         storage i64, u64 [size=8, align=8];
// ISO_FAST-NEXT:         storage i128, u128 [size=16, align=16];
// ISO_FAST-NEXT:         storage bf16 [size=2, align=2];
// ISO_FAST-NEXT:         storage f16 [size=2, align=2];
// ISO_FAST-NEXT:         storage f32 [size=4, align=4];
// ISO_FAST-NEXT:         storage f64 [size=8, align=8];
// ISO_FAST-NEXT:         storage f80 [size=16, align=16];
// ISO_FAST-NEXT:         storage f128 [size=16, align=16];
// ISO_FAST-NEXT:         storage d32 [size=4, align=4];
// ISO_FAST-NEXT:         storage d64 [size=8, align=8];
// ISO_FAST-NEXT:         storage d128 [size=16, align=16];
// ISO_FAST-NEXT:     }
// ISO_FAST-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// ISO_FAST-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// ISO_FAST-NEXT:     }
// ISO_FAST-NEXT: }
// SLATE-FILECHECK-END ISO_FAST
// SLATE-FILECHECK-BEGIN ISO_FAST_MATH
// ISO_FAST_MATH: module {
// ISO_FAST_MATH-NEXT:     target "x86_64-unknown-linux-gnu" {
// ISO_FAST_MATH-NEXT:         endian = little;
// ISO_FAST_MATH-NEXT:         pointer [size=8, align=8];
// ISO_FAST_MATH-NEXT:         stack_alignment = 16;
// ISO_FAST_MATH-NEXT:         long_double = f80;
// ISO_FAST_MATH-NEXT:         storage bool [size=1, align=1];
// ISO_FAST_MATH-NEXT:         storage i8, u8 [size=1, align=1];
// ISO_FAST_MATH-NEXT:         storage i16, u16 [size=2, align=2];
// ISO_FAST_MATH-NEXT:         storage i32, u32 [size=4, align=4];
// ISO_FAST_MATH-NEXT:         storage i64, u64 [size=8, align=8];
// ISO_FAST_MATH-NEXT:         storage i128, u128 [size=16, align=16];
// ISO_FAST_MATH-NEXT:         storage bf16 [size=2, align=2];
// ISO_FAST_MATH-NEXT:         storage f16 [size=2, align=2];
// ISO_FAST_MATH-NEXT:         storage f32 [size=4, align=4];
// ISO_FAST_MATH-NEXT:         storage f64 [size=8, align=8];
// ISO_FAST_MATH-NEXT:         storage f80 [size=16, align=16];
// ISO_FAST_MATH-NEXT:         storage f128 [size=16, align=16];
// ISO_FAST_MATH-NEXT:         storage d32 [size=4, align=4];
// ISO_FAST_MATH-NEXT:         storage d64 [size=8, align=8];
// ISO_FAST_MATH-NEXT:         storage d128 [size=16, align=16];
// ISO_FAST_MATH-NEXT:     }
// ISO_FAST_MATH-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// ISO_FAST_MATH-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// ISO_FAST_MATH-NEXT:     }
// ISO_FAST_MATH-NEXT: }
// SLATE-FILECHECK-END ISO_FAST_MATH
// SLATE-FILECHECK-BEGIN EXPLICIT_WINS
// EXPLICIT_WINS: module {
// EXPLICIT_WINS-NEXT:     target "x86_64-unknown-linux-gnu" {
// EXPLICIT_WINS-NEXT:         endian = little;
// EXPLICIT_WINS-NEXT:         pointer [size=8, align=8];
// EXPLICIT_WINS-NEXT:         stack_alignment = 16;
// EXPLICIT_WINS-NEXT:         long_double = f80;
// EXPLICIT_WINS-NEXT:         storage bool [size=1, align=1];
// EXPLICIT_WINS-NEXT:         storage i8, u8 [size=1, align=1];
// EXPLICIT_WINS-NEXT:         storage i16, u16 [size=2, align=2];
// EXPLICIT_WINS-NEXT:         storage i32, u32 [size=4, align=4];
// EXPLICIT_WINS-NEXT:         storage i64, u64 [size=8, align=8];
// EXPLICIT_WINS-NEXT:         storage i128, u128 [size=16, align=16];
// EXPLICIT_WINS-NEXT:         storage bf16 [size=2, align=2];
// EXPLICIT_WINS-NEXT:         storage f16 [size=2, align=2];
// EXPLICIT_WINS-NEXT:         storage f32 [size=4, align=4];
// EXPLICIT_WINS-NEXT:         storage f64 [size=8, align=8];
// EXPLICIT_WINS-NEXT:         storage f80 [size=16, align=16];
// EXPLICIT_WINS-NEXT:         storage f128 [size=16, align=16];
// EXPLICIT_WINS-NEXT:         storage d32 [size=4, align=4];
// EXPLICIT_WINS-NEXT:         storage d64 [size=8, align=8];
// EXPLICIT_WINS-NEXT:         storage d128 [size=16, align=16];
// EXPLICIT_WINS-NEXT:     }
// EXPLICIT_WINS-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// EXPLICIT_WINS-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// EXPLICIT_WINS-NEXT:     }
// EXPLICIT_WINS-NEXT: }
// SLATE-FILECHECK-END EXPLICIT_WINS
