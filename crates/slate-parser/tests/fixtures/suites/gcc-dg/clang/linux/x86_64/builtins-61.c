/* { dg-do compile } */
/* { dg-options "-O -ffast-math -fdump-tree-optimized" } */
/* { dg-require-effective-target c99_runtime } */
/* { dg-require-effective-target libc_has_complex_functions } */

double test1 (double x)
{
  return __real __builtin_cexp(x * (__extension__ 1.0iF));
}

double test2(double x)
{
  return __imag __builtin_cexp((__extension__ 1.0iF) * x);
}

double test3(double x)
{
  _Complex c = __builtin_cexp(x * (__extension__ 1.0iF));
  return __imag c + __real c;
}

double test4(double x, double y)
{
  _Complex c = __builtin_cexp(x);
  x = __builtin_exp (x);
  return x - __real c;
}

/* { dg-final { scan-tree-dump "cexpi" "optimized" } } */
/* { dg-final { scan-tree-dump "sin" "optimized" } } */
/* { dg-final { scan-tree-dump "cos" "optimized" } } */
/* { dg-final { scan-tree-dump "return 0.0" "optimized" } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %12 @__builtin_cexp(%11 <unnamed>: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>];
// DEFAULT-NEXT:     fn %0 @test1(%1 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return complex_to_real<f64, reason=explicit>(call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%12, mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<f64>(%1), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @test2(%3 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return complex_to_imag<f64, reason=explicit>(call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%12, mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0))), read<f64>(%3))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @test3(%5 x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 c: complex<f64> [storage=automatic] = call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%12, mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<f64>(%5), complex_convert<complex<f64>, reason=usual_arith>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(imag(%6)), read<f64>(real(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_exp(%13 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @test4(%8 x: f64, %9 y: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 c: complex<f64> [storage=automatic] = call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(coerce<f64, f64>) -> coerce<f64, f64>>(%12, real_to_complex<complex<f64>, reason=arg>(read<f64>(%8)));
// DEFAULT-NEXT:         write<f64>(%8, call<f64, signature=fn(f64) -> f64>(%14, read<f64>(%8)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%14, read<f64>(%8));
// DEFAULT-NEXT:         return sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%8), read<f64>(real(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
