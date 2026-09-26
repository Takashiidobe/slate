#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF
#pragma STDC CX_LIMITED_RANGE ON
#pragma float_control(except, off)
#pragma float_control push

double ignored(double a, double b, double c) {
  double x = a;
#pragma STDC FENV_ACCESS ON
  return x * b + c;
}

_Complex double full_range(_Complex double a, _Complex double b) { return a / b; }

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES ISO
// SLATE-FILECHECK-STD ISO c17

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
// DEFAULT-NEXT:     fn %0 @ignored(%1 a: f64, %2 b: f64, %3 c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 x: f64 [storage=automatic] = read<f64>(%1);
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%4), read<f64>(%2)), read<f64>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @full_range(%6 a: complex<f64>, %7 b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>, coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%6), read<complex<f64>>(%7));
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
// ISO-NEXT:     fn %0 @ignored(%1 a: f64, %2 b: f64, %3 c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// ISO-NEXT:         let %4 x: f64 [storage=automatic] = read<f64>(%1);
// ISO-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=off>(mul<f64, rounding=nearest_even, exceptions=observable, contract=off>(read<f64>(%4), read<f64>(%2)), read<f64>(%3));
// ISO-NEXT:     }
// ISO-NEXT:     fn %5 @full_range(%6 a: complex<f64>, %7 b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>, coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// ISO-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%6), read<complex<f64>>(%7));
// ISO-NEXT:     }
// ISO-NEXT: }
// SLATE-FILECHECK-END ISO
