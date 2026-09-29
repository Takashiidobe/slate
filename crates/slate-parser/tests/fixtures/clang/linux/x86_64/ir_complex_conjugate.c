// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu99

_Complex double zd;
_Complex int zi;

void conjugate(void) {
  zd = ~zd;
  zi = ~zi;
}

double conjugate_real(void) { return __real__ ~zd; }

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
// IR-NEXT:     global %[[VALUE_zd:[0-9]+]] zd: complex<f64> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_zi:[0-9]+]] zi: complex<i32> [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_conjugate:[0-9]+]] @conjugate() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<complex<f64>>(%[[VALUE_zd]], not<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_zd]])));
// IR-NEXT:         write<complex<i32>>(%[[VALUE_zi]], not<complex<i32>, complex=true, overflow=ub>(read<complex<i32>>(%[[VALUE_zi]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_conjugate_real:[0-9]+]] @conjugate_real() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return complex_to_real<f64, reason=explicit>(not<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_zd]])));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
