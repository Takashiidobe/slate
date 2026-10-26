// SLATE-FILECHECK-ARGS --dump-ir -fcomplex-arithmetic=promoted
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-WARNING DEFAULT

double _Complex folded = (1.0 + 2.0i) / (3.0 + 4.0i);

double _Complex divide(double _Complex a, double _Complex b) { return a / b; }

float _Complex divide_float(float _Complex a, float _Complex b) { return a / b; }

double _Complex divide_real(double _Complex a, double b) { return a / b; }

double _Complex real_divide(double a, double _Complex b) { return a / b; }

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: -Whigher-precision-for-complex-division
// DEFAULT:   ⚠ excess precision is requested but the target does not support excess
// DEFAULT:   │ precision which may result in observable differences in complex division
// DEFAULT:   │ behavior, additional uses where the requested higher precision cannot be
// DEFAULT:   │ honored were found but not diagnosed
// DEFAULT:    ╭─[tests/fixtures/clang/windows/x86_64/complex_arithmetic_promoted.c:2:26]
// DEFAULT:  1 │
// DEFAULT:  2 │ double _Complex folded = (1.0 + 2.0i) / (3.0 + 4.0i);
// DEFAULT:    ·                          ───────────────────────────
// DEFAULT:  3 │
// DEFAULT:    ╰────
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-DEFAULT
// IR-DEFAULT: module {
// IR-DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-DEFAULT-NEXT:         endian = little;
// IR-DEFAULT-NEXT:         pointer [size=8, align=8];
// IR-DEFAULT-NEXT:         stack_alignment = 16;
// IR-DEFAULT-NEXT:         long_double = f64;
// IR-DEFAULT-NEXT:         storage bool [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// IR-DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f16 [size=2, align=2];
// IR-DEFAULT-NEXT:         storage f32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage f64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage f128 [size=16, align=16];
// IR-DEFAULT-NEXT:         storage d32 [size=4, align=4];
// IR-DEFAULT-NEXT:         storage d64 [size=8, align=8];
// IR-DEFAULT-NEXT:         storage d128 [size=16, align=16];
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     global %[[VALUE_folded:[0-9]+]] folded: complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(4.0)))) [linkage=external];
// IR-DEFAULT-NEXT:     fn %[[VALUE_divide:[0-9]+]] @divide(%[[VALUE_a:[0-9]+]] a: complex<f64>, %[[VALUE_b:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=win64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<complex<f64>>(%[[VALUE_a]]), read<complex<f64>>(%[[VALUE_b]]));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_divide_float:[0-9]+]] @divide_float(%[[VALUE_a_2:[0-9]+]] a: complex<f32>, %[[VALUE_b_2:[0-9]+]] b: complex<f32>) -> complex<f32> [linkage=external] [abi=win64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return complex_convert<complex<f32>, reason=excess_precision, rounding=nearest_even, exceptions=ignore>(div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(complex_convert<complex<f64>, reason=excess_precision>(read<complex<f32>>(%[[VALUE_a_2]])), complex_convert<complex<f64>, reason=excess_precision>(read<complex<f32>>(%[[VALUE_b_2]]))));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_divide_real:[0-9]+]] @divide_real(%[[VALUE_a_3:[0-9]+]] a: complex<f64>, %[[VALUE_b_3:[0-9]+]] b: f64) -> complex<f64> [linkage=external] [abi=win64(native_c, scalar) -> native_c] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_3]]), read<f64>(%[[VALUE_b_3]]));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT:     fn %[[VALUE_real_divide:[0-9]+]] @real_divide(%[[VALUE_a_4:[0-9]+]] a: f64, %[[VALUE_b_4:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=win64(scalar, native_c) -> native_c] [fallthrough=ub_if_used] {
// IR-DEFAULT-NEXT:         return div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=improved>(read<f64>(%[[VALUE_a_4]]), read<complex<f64>>(%[[VALUE_b_4]]));
// IR-DEFAULT-NEXT:     }
// IR-DEFAULT-NEXT: }
// SLATE-FILECHECK-END IR-DEFAULT
