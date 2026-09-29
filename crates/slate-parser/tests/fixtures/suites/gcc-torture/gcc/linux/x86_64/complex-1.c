void abort(void);
void exit(int);

double g0(double x) { return 1.0; }

double g1(double x) { return -1.0; }

double g2(double x) { return 0.0; }

__complex__ double xcexp(__complex__ double x) {
  double r;

  r          = g0(__real__ x);
  __real__ x = r * g1(__imag__ x);
  __imag__ x = r * g2(__imag__ x);
  return x;
}

int main(void) {
  __complex__ double x;

  x = xcexp(1.0i);
  if (__real__ x != -1.0)
    abort();
  if (__imag__ x != 0.0)
    abort();
  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_g0:[0-9]+]] @g0(%[[VALUE_x:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<f64>(1.0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g1:[0-9]+]] @g1(%[[VALUE_x_2:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(const<f64>(1.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g2:[0-9]+]] @g2(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<f64>(0.0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_xcexp:[0-9]+]] @xcexp(%[[VALUE_x_4:[0-9]+]] x: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%[[VALUE_r]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_g0]], read<f64>(real(%[[VALUE_x_4]]))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_g0]], read<f64>(real(%[[VALUE_x_4]])));
// DEFAULT-NEXT:         write<f64>(real(%[[VALUE_x_4]]), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_r]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_g1]], read<f64>(imag(%[[VALUE_x_4]])))));
// DEFAULT-NEXT:         mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_r]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_g1]], read<f64>(imag(%[[VALUE_x_4]]))));
// DEFAULT-NEXT:         write<f64>(imag(%[[VALUE_x_4]]), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_r]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_g2]], read<f64>(imag(%[[VALUE_x_4]])))));
// DEFAULT-NEXT:         mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_r]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_g2]], read<f64>(imag(%[[VALUE_x_4]]))));
// DEFAULT-NEXT:         return read<complex<f64>>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_5:[0-9]+]] x: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%[[VALUE_x_5]], call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_xcexp]], aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_xcexp]], aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)));
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(real(%[[VALUE_x_5]])), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(imag(%[[VALUE_x_5]])), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
