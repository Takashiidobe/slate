extern void abort(void);

_Complex v = 3.0 + 1.0iF;

void foo(_Complex z, int *x) {
  if (z != v)
    abort();
}

_Complex bar(_Complex z) __attribute__((pure));
_Complex bar(_Complex z) { return v; }

int baz(void) {
  int a, i;
  for (i = 0; i < 6; i++)
    foo(bar(1.0iF * i), &a);
  return 0;
}

int main() {
  baz();
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
// DEFAULT-NEXT:     global %1 v: complex<f64> [storage=static] = add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(const<f64>(3.0), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo(%3 z: complex<f64>, %4 x: ptr<i32>) -> void [linkage=external] [abi=sysv64(native_c, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<complex<f64>, exceptions=observable>(read<complex<f64>>(%3), read<complex<f64>>(%1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 z: complex<f64>) -> complex<f64> [linkage=external] [memory=read] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<complex<f64>>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%9, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(complex<f64>, ptr<i32>) -> void, abi=sysv64(native_c, scalar) -> void>(%2, call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%5, complex_convert<complex<f64>, reason=arg>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(read<i32>(%9))))), addr_of<ptr<i32>>(%8));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
