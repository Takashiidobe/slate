// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#pragma STDC FLOAT_CONST_DECIMAL64 ON
_Static_assert(_Generic(1.0, double: 1, default: 0), "clang ignores the pragma");

double f(void) {
  double b = 2.0;
#pragma STDC FLOAT_CONST_DECIMAL64 OFF
  return b * 3.0;
}

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
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_b:[0-9]+]] b: f64 [storage=automatic] = const<f64>(2.0);
// IR-NEXT:         return mul<f64>(read<f64>(%[[VALUE_b]]), const<f64>(3.0));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
