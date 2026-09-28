void abort(void);

__complex__ float __attribute__((noinline)) foo(__complex__ float x) {
  return x;
}

__complex__ float __attribute__((noinline)) bar(__complex__ float x) {
  return foo(x);
}

int main() {
  __complex__ float a, b;
  __real__ a = 9;
  __imag__ a = 42;

  b = bar(a);

  if (a != b)
    abort();

  return 0;
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: complex<f32>) -> complex<f32> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<complex<f32>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @bar(%4 x: complex<f32>) -> complex<f32> [linkage=external] [inline=never] [definition=emitted] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%1, read<complex<f32>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 a: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         let %7 b: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<f32>(real(%6), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(9)));
// DEFAULT-NEXT:         write<f32>(imag(%6), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(42)));
// DEFAULT-NEXT:         write<complex<f32>>(%7, call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%3, read<complex<f32>>(%6)));
// DEFAULT-NEXT:         call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%3, read<complex<f32>>(%6));
// DEFAULT-NEXT:         if ne<complex<f32>, exceptions=observable>(read<complex<f32>>(%6), read<complex<f32>>(%7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
