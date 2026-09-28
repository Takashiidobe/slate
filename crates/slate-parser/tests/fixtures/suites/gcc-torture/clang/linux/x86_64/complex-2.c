void abort(void);
void exit(int);

__complex__ double f(__complex__ double x, __complex__ double y) {
  x += y;
  return x;
}

__complex__ double ag = 1.0 + 1.0i;
__complex__ double bg = -2.0 + 2.0i;

int main(void) {
  __complex__ double a, b, c;

  a = ag;
  b = -2.0 + 2.0i;
  c = f(a, b);

  if (a != 1.0 + 1.0i)
    abort();
  if (b != -2.0 + 2.0i)
    abort();
  if (c != -1.0 + 3.0i)
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
// DEFAULT-NEXT:     global %5 ag: complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))) [linkage=external];
// DEFAULT-NEXT:     global %6 bg: complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(neg<f64>(const<f64>(2.0)), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @f(%3 x: complex<f64>, %4 y: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12: complex<f64> [synthetic] = read<complex<f64>>(%3);
// DEFAULT-NEXT:         let %13: complex<f64> [synthetic] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%12), read<complex<f64>>(%4));
// DEFAULT-NEXT:         write<complex<f64>>(%3, read<complex<f64>>(%13));
// DEFAULT-NEXT:         return read<complex<f64>>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 a: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %9 b: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         let %10 c: complex<f64> [storage=automatic];
// DEFAULT-NEXT:         write<complex<f64>>(%8, read<complex<f64>>(%5));
// DEFAULT-NEXT:         write<complex<f64>>(%9, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(neg<f64>(const<f64>(2.0)), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))));
// DEFAULT-NEXT:         write<complex<f64>>(%10, call<complex<f64>, signature=fn(complex<f64>, complex<f64>) -> complex<f64>, abi=sysv64(native_c, native_c) -> native_c>(%2, read<complex<f64>>(%8), read<complex<f64>>(%9)));
// DEFAULT-NEXT:         call<complex<f64>, signature=fn(complex<f64>, complex<f64>) -> complex<f64>, abi=sysv64(native_c, native_c) -> native_c>(%2, read<complex<f64>>(%8), read<complex<f64>>(%9));
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%8), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%9), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(neg<f64>(const<f64>(2.0)), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(2.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=ignore>(read<complex<f64>>(%10), add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(neg<f64>(const<f64>(1.0)), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(3.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
