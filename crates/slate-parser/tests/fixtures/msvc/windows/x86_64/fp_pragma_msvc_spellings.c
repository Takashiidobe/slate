// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir /fp:contract
#pragma fenv_access(on)
double on(double a, double b, double c) { return a * b + c; }
#pragma fenv_access(off)
#pragma fp_contract(off)
double off(double a, double b, double c) { return a * b + c; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
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
// IR-NEXT:     fn %[[VALUE_on:[0-9]+]] @on(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f64, rounding=environment, exceptions=observable, contract=on>(mul<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_c]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_off:[0-9]+]] @off(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
