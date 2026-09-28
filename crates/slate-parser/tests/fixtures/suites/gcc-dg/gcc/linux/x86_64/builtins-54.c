/* { dg-do link } */
/* { dg-options "-O2 -ffast-math" } */

double fabs(double);
float fabsf(float);
long double fabsl(long double);
double cabs(__complex__ double);
float cabsf(__complex__ float);
long double cabsl(__complex__ long double);

void link_error (void);

void test(__complex__ double x, double a, double b)
{
  if (cabs(x) != cabs(-x))
    link_error();

  if (cabs(x) != cabs(~x))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (a+a*1i))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (a*1i+a))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (-a+a*-1i))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (-a+-a*1i))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (-a-a*1i))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (a*-1i-a))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (-a*1i-a))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (a*-1i+-a))
    link_error();

  if (fabs(a) * __builtin_sqrt(2) != cabs (-a*1i+-a))
    link_error();

  if (fabs(a*b) * __builtin_sqrt(2) != cabs (a*b-(-b*a*1i)))
    link_error();

  if (fabs(a*b) * __builtin_sqrt(2) != cabs (a*b*1i-a*-b))
    link_error();
}

void testf(__complex__ float x, float a, float b)
{
  if (cabsf(x) != cabsf(-x))
    link_error();

  if (cabsf(x) != cabsf(~x))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (a+a*1i))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (a*1i+a))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (-a+a*-1i))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (-a+-a*1i))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (-a-a*1i))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (a*-1i-a))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (-a*1i-a))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (a*-1i+-a))
    link_error();

  if (fabsf(a) * __builtin_sqrtf(2) != cabsf (-a*1i+-a))
    link_error();

  if (fabsf(a*b) * __builtin_sqrtf(2) != cabsf (a*b-(-b*a*1i)))
    link_error();

  if (fabsf(a*b) * __builtin_sqrtf(2) != cabsf (a*b*1i-a*-b))
    link_error();
}

void testl(__complex__ long double x, long double a, long double b)
{
  if (cabsl(x) != cabsl(-x))
    link_error();

  if (cabsl(x) != cabsl(~x))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (a+a*1i))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (a*1i+a))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (-a+a*-1i))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (-a+-a*1i))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (-a-a*1i))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (a*-1i-a))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (-a*1i-a))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (a*-1i+-a))
    link_error();

  if (fabsl(a) * __builtin_sqrtl(2) != cabsl (-a*1i+-a))
    link_error();

  if (fabsl(a*b) * __builtin_sqrtl(2) != cabsl (a*b-(-b*a*1i)))
    link_error();

  if (fabsl(a*b) * __builtin_sqrtl(2) != cabsl (a*b*1i-a*-b))
    link_error();
}

int main()
{
  test(0, 0, 0);
  testf(0, 0, 0);
  testl(0, 0, 0);
  return 0;
}


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
// DEFAULT-NEXT:     fn %0 @fabs(%20 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %1 @fabsf(%21 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @fabsl(%22 <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %3 @cabs(%23 <unnamed>: complex<f64>) -> f64 [linkage=external] [abi=sysv64(coerce<f64, f64>) -> scalar];
// DEFAULT-NEXT:     fn %4 @cabsf(%24 <unnamed>: complex<f32>) -> f32 [linkage=external] [abi=sysv64(coerce<pair<f32>>) -> scalar];
// DEFAULT-NEXT:     fn %5 @cabsl(%25 <unnamed>: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %6 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_sqrt(%26 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @test(%8 x: complex<f64>, %9 a: f64, %10 b: f64) -> void [linkage=external] [abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, read<complex<f64>>(%8)), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, neg<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, read<complex<f64>>(%8)), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, not<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%9), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%9), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%9), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f64>(%9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%9)), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%9), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%9)), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%9)), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%9)), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%9), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%9), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), read<f64>(%9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%9)), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f64>(%9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%9), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), neg<f64>(read<f64>(%9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, read<f64>(%9)), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%9)), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), neg<f64>(read<f64>(%9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%9), read<f64>(%10))), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%9), read<f64>(%10)), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f64>(read<f64>(%10)), read<f64>(%9)), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%0, mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%9), read<f64>(%10))), call<f64, signature=fn(f64) -> f64>(%27, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(coerce<f64, f64>) -> scalar>(%3, sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%9), read<f64>(%10)), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%9), neg<f64>(read<f64>(%10))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @__builtin_sqrtf(%28 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %11 @testf(%12 x: complex<f32>, %13 a: f32, %14 b: f32) -> void [linkage=external] [abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, read<complex<f32>>(%12)), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, neg<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, read<complex<f32>>(%12)), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, not<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%13), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%13), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%13), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f32>(%13))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%13)), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%13), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%13)), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%13)), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%13)), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%13), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%13), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), read<f32>(%13))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%13)), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f32>(%13))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%13), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), neg<f32>(read<f32>(%13)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, read<f32>(%13)), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%13)), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), neg<f32>(read<f32>(%13)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%13), read<f32>(%14))), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%13), read<f32>(%14)), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f32>(read<f32>(%14)), read<f32>(%13)), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%1, mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%13), read<f32>(%14))), call<f32, signature=fn(f32) -> f32>(%29, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(coerce<pair<f32>>) -> scalar>(%4, sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%13), read<f32>(%14)), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%13), neg<f32>(read<f32>(%14))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @__builtin_sqrtl(%30 <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %15 @testl(%16 x: complex<f80>, %17 a: f80, %18 b: f80) -> void [linkage=external] [abi=sysv64(byval<align=16>, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, read<complex<f80>>(%16)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, neg<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, read<complex<f80>>(%16)), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, not<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%16))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%17), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%17), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%17), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f80>(%17))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%17)), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%17), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%17)), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%17)), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%17)), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%17), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%17), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), read<f80>(%17))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%17)), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f80>(%17))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%17), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), neg<f80>(read<f80>(%17)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, read<f80>(%17)), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%17)), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), neg<f80>(read<f80>(%17)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%17), read<f80>(%18))), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%17), read<f80>(%18)), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f80>(read<f80>(%18)), read<f80>(%17)), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%2, mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%17), read<f80>(%18))), call<f80, signature=fn(f80) -> f80>(%31, int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%5, sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%17), read<f80>(%18)), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%17), neg<f80>(read<f80>(%18))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(coerce<f64, f64>, scalar, scalar) -> void>(%7, real_to_complex<complex<f64>, reason=arg>(int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(coerce<pair<f32>>, scalar, scalar) -> void>(%11, real_to_complex<complex<f32>, reason=arg>(int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%15, real_to_complex<complex<f80>, reason=arg>(int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))), int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
