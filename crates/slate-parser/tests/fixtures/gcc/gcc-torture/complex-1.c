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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%13 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @g0(%3 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<f64>(1.0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @g1(%5 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(const<f64>(1.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @g2(%7 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<f64>(0.0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @xcexp(%9 x: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 r: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%10, call<f64, signature=fn(f64) -> f64>(%2, read<f64>(real(%9))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%2, read<f64>(real(%9)));
// DEFAULT-NEXT:         write<f64>(real(%9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%10), call<f64, signature=fn(f64) -> f64>(%4, read<f64>(imag(%9)))));
// DEFAULT-NEXT:         mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%10), call<f64, signature=fn(f64) -> f64>(%4, read<f64>(imag(%9))));
// DEFAULT-NEXT:         write<f64>(imag(%9), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%10), call<f64, signature=fn(f64) -> f64>(%6, read<f64>(imag(%9)))));
// DEFAULT-NEXT:         mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%10), call<f64, signature=fn(f64) -> f64>(%6, read<f64>(imag(%9))));
// DEFAULT-NEXT:         return read<complex<f64>>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 x: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%12, call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%8, aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%8, aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)));
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(real(%12)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(imag(%12)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
