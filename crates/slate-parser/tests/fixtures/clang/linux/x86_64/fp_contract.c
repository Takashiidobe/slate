double fused(double a, double b, double c) { return a * b + c; }

double restored(double a, double b, double c) {
#pragma STDC FP_CONTRACT OFF
  {
#pragma STDC FP_CONTRACT DEFAULT
    return a * b + c;
  }
}

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES OFF
// SLATE-FILECHECK-PREFIX-ARGS OFF -ffp-contract=off
// SLATE-FILECHECK-DEFINES FAST
// SLATE-FILECHECK-PREFIX-ARGS FAST -ffp-contract=fast-honor-pragmas
// SLATE-FILECHECK-DEFINES FAST_MATH
// SLATE-FILECHECK-PREFIX-ARGS FAST_MATH -ffp-contract=off -ffast-math
// SLATE-FILECHECK-DEFINES OVERRIDE
// SLATE-FILECHECK-PREFIX-ARGS OVERRIDE -ffast-math -ffp-contract=on
// SLATE-FILECHECK-DEFINES RESTORED
// SLATE-FILECHECK-PREFIX-ARGS RESTORED -ffp-contract=off -ffast-math -fno-fast-math

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
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN OFF
// OFF: module {
// OFF-NEXT:     target "x86_64-unknown-linux-gnu" {
// OFF-NEXT:         endian = little;
// OFF-NEXT:         pointer [size=8, align=8];
// OFF-NEXT:         stack_alignment = 16;
// OFF-NEXT:         long_double = f80;
// OFF-NEXT:         storage bool [size=1, align=1];
// OFF-NEXT:         storage i8, u8 [size=1, align=1];
// OFF-NEXT:         storage i16, u16 [size=2, align=2];
// OFF-NEXT:         storage i32, u32 [size=4, align=4];
// OFF-NEXT:         storage i64, u64 [size=8, align=8];
// OFF-NEXT:         storage i128, u128 [size=16, align=16];
// OFF-NEXT:         storage bf16 [size=2, align=2];
// OFF-NEXT:         storage f16 [size=2, align=2];
// OFF-NEXT:         storage f32 [size=4, align=4];
// OFF-NEXT:         storage f64 [size=8, align=8];
// OFF-NEXT:         storage f80 [size=16, align=16];
// OFF-NEXT:         storage f128 [size=16, align=16];
// OFF-NEXT:         storage d32 [size=4, align=4];
// OFF-NEXT:         storage d64 [size=8, align=8];
// OFF-NEXT:         storage d128 [size=16, align=16];
// OFF-NEXT:     }
// OFF-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// OFF-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// OFF-NEXT:     }
// OFF-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// OFF-NEXT:         {
// OFF-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]]));
// OFF-NEXT:         }
// OFF-NEXT:     }
// OFF-NEXT: }
// SLATE-FILECHECK-END OFF
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
// FAST-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// FAST-NEXT:     }
// FAST-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST-NEXT:         {
// FAST-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]]));
// FAST-NEXT:         }
// FAST-NEXT:     }
// FAST-NEXT: }
// SLATE-FILECHECK-END FAST
// SLATE-FILECHECK-BEGIN FAST_MATH
// FAST_MATH: module {
// FAST_MATH-NEXT:     target "x86_64-unknown-linux-gnu" {
// FAST_MATH-NEXT:         endian = little;
// FAST_MATH-NEXT:         pointer [size=8, align=8];
// FAST_MATH-NEXT:         stack_alignment = 16;
// FAST_MATH-NEXT:         long_double = f80;
// FAST_MATH-NEXT:         storage bool [size=1, align=1];
// FAST_MATH-NEXT:         storage i8, u8 [size=1, align=1];
// FAST_MATH-NEXT:         storage i16, u16 [size=2, align=2];
// FAST_MATH-NEXT:         storage i32, u32 [size=4, align=4];
// FAST_MATH-NEXT:         storage i64, u64 [size=8, align=8];
// FAST_MATH-NEXT:         storage i128, u128 [size=16, align=16];
// FAST_MATH-NEXT:         storage bf16 [size=2, align=2];
// FAST_MATH-NEXT:         storage f16 [size=2, align=2];
// FAST_MATH-NEXT:         storage f32 [size=4, align=4];
// FAST_MATH-NEXT:         storage f64 [size=8, align=8];
// FAST_MATH-NEXT:         storage f80 [size=16, align=16];
// FAST_MATH-NEXT:         storage f128 [size=16, align=16];
// FAST_MATH-NEXT:         storage d32 [size=4, align=4];
// FAST_MATH-NEXT:         storage d64 [size=8, align=8];
// FAST_MATH-NEXT:         storage d128 [size=16, align=16];
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// FAST_MATH-NEXT:         {
// FAST_MATH-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]]));
// FAST_MATH-NEXT:         }
// FAST_MATH-NEXT:     }
// FAST_MATH-NEXT: }
// SLATE-FILECHECK-END FAST_MATH
// SLATE-FILECHECK-BEGIN OVERRIDE
// OVERRIDE: module {
// OVERRIDE-NEXT:     target "x86_64-unknown-linux-gnu" {
// OVERRIDE-NEXT:         endian = little;
// OVERRIDE-NEXT:         pointer [size=8, align=8];
// OVERRIDE-NEXT:         stack_alignment = 16;
// OVERRIDE-NEXT:         long_double = f80;
// OVERRIDE-NEXT:         storage bool [size=1, align=1];
// OVERRIDE-NEXT:         storage i8, u8 [size=1, align=1];
// OVERRIDE-NEXT:         storage i16, u16 [size=2, align=2];
// OVERRIDE-NEXT:         storage i32, u32 [size=4, align=4];
// OVERRIDE-NEXT:         storage i64, u64 [size=8, align=8];
// OVERRIDE-NEXT:         storage i128, u128 [size=16, align=16];
// OVERRIDE-NEXT:         storage bf16 [size=2, align=2];
// OVERRIDE-NEXT:         storage f16 [size=2, align=2];
// OVERRIDE-NEXT:         storage f32 [size=4, align=4];
// OVERRIDE-NEXT:         storage f64 [size=8, align=8];
// OVERRIDE-NEXT:         storage f80 [size=16, align=16];
// OVERRIDE-NEXT:         storage f128 [size=16, align=16];
// OVERRIDE-NEXT:         storage d32 [size=4, align=4];
// OVERRIDE-NEXT:         storage d64 [size=8, align=8];
// OVERRIDE-NEXT:         storage d128 [size=16, align=16];
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// OVERRIDE-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// OVERRIDE-NEXT:         {
// OVERRIDE-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]]));
// OVERRIDE-NEXT:         }
// OVERRIDE-NEXT:     }
// OVERRIDE-NEXT: }
// SLATE-FILECHECK-END OVERRIDE
// SLATE-FILECHECK-BEGIN RESTORED
// RESTORED: module {
// RESTORED-NEXT:     target "x86_64-unknown-linux-gnu" {
// RESTORED-NEXT:         endian = little;
// RESTORED-NEXT:         pointer [size=8, align=8];
// RESTORED-NEXT:         stack_alignment = 16;
// RESTORED-NEXT:         long_double = f80;
// RESTORED-NEXT:         storage bool [size=1, align=1];
// RESTORED-NEXT:         storage i8, u8 [size=1, align=1];
// RESTORED-NEXT:         storage i16, u16 [size=2, align=2];
// RESTORED-NEXT:         storage i32, u32 [size=4, align=4];
// RESTORED-NEXT:         storage i64, u64 [size=8, align=8];
// RESTORED-NEXT:         storage i128, u128 [size=16, align=16];
// RESTORED-NEXT:         storage bf16 [size=2, align=2];
// RESTORED-NEXT:         storage f16 [size=2, align=2];
// RESTORED-NEXT:         storage f32 [size=4, align=4];
// RESTORED-NEXT:         storage f64 [size=8, align=8];
// RESTORED-NEXT:         storage f80 [size=16, align=16];
// RESTORED-NEXT:         storage f128 [size=16, align=16];
// RESTORED-NEXT:         storage d32 [size=4, align=4];
// RESTORED-NEXT:         storage d64 [size=8, align=8];
// RESTORED-NEXT:         storage d128 [size=16, align=16];
// RESTORED-NEXT:     }
// RESTORED-NEXT:     fn %[[VALUE_fused:[0-9]+]] @fused(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// RESTORED-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// RESTORED-NEXT:     }
// RESTORED-NEXT:     fn %[[VALUE_restored:[0-9]+]] @restored(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// RESTORED-NEXT:         {
// RESTORED-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]]));
// RESTORED-NEXT:         }
// RESTORED-NEXT:     }
// RESTORED-NEXT: }
// SLATE-FILECHECK-END RESTORED
