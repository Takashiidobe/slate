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
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsf:[0-9]+]] @fabsf(%[[VALUE1:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsl:[0-9]+]] @fabsl(%[[VALUE2:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_cabs:[0-9]+]] @cabs(%[[VALUE3:[0-9]+]] <unnamed>: complex<f64>) -> f64 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_cabsf:[0-9]+]] @cabsf(%[[VALUE4:[0-9]+]] <unnamed>: complex<f32>) -> f32 [linkage=external] [abi=sysv64(native_c) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_cabsl:[0-9]+]] @cabsl(%[[VALUE5:[0-9]+]] <unnamed>: complex<f80>) -> f80 [linkage=external] [abi=sysv64(byval<align=16>) -> scalar];
// DEFAULT-NEXT:     fn %[[VALUE_link_error:[0-9]+]] @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrt:[0-9]+]] @__builtin_sqrt(%[[VALUE6:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_x:[0-9]+]] x: complex<f64>, %[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> void [linkage=external] [abi=sysv64(native_c, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], read<complex<f64>>(%[[VALUE_x]])), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], neg<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_x]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], read<complex<f64>>(%[[VALUE_x]])), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], not<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>>(%[[VALUE_x]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%[[VALUE_a]]), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%[[VALUE_a]]), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%[[VALUE_a]]), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f64>(%[[VALUE_a]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%[[VALUE_a]])), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%[[VALUE_a]]), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%[[VALUE_a]])), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%[[VALUE_a]])), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%[[VALUE_a]])), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%[[VALUE_a]]), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%[[VALUE_a]]), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), read<f64>(%[[VALUE_a]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%[[VALUE_a]])), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f64>(%[[VALUE_a]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f64>(%[[VALUE_a]]), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), neg<f64>(read<f64>(%[[VALUE_a]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], read<f64>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f64>(read<f64>(%[[VALUE_a]])), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), neg<f64>(read<f64>(%[[VALUE_a]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f64>(read<f64>(%[[VALUE_b]])), read<f64>(%[[VALUE_a]])), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f64, signature=fn(complex<f64>) -> f64, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabs]], sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]])), complex_convert<complex<f64>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), neg<f64>(read<f64>(%[[VALUE_b]]))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrtf:[0-9]+]] @__builtin_sqrtf(%[[VALUE7:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_testf:[0-9]+]] @testf(%[[VALUE_x_2:[0-9]+]] x: complex<f32>, %[[VALUE_a_2:[0-9]+]] a: f32, %[[VALUE_b_2:[0-9]+]] b: f32) -> void [linkage=external] [abi=sysv64(native_c, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], read<complex<f32>>(%[[VALUE_x_2]])), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], neg<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_x_2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], read<complex<f32>>(%[[VALUE_x_2]])), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], not<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>>(%[[VALUE_x_2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%[[VALUE_a_2]]), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%[[VALUE_a_2]]), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%[[VALUE_a_2]]), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f32>(%[[VALUE_a_2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%[[VALUE_a_2]])), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%[[VALUE_a_2]]), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%[[VALUE_a_2]])), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%[[VALUE_a_2]])), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%[[VALUE_a_2]])), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%[[VALUE_a_2]]), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%[[VALUE_a_2]]), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), read<f32>(%[[VALUE_a_2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%[[VALUE_a_2]])), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f32>(%[[VALUE_a_2]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f32>(%[[VALUE_a_2]]), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), neg<f32>(read<f32>(%[[VALUE_a_2]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_a_2]])), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], add<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f32>(read<f32>(%[[VALUE_a_2]])), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), neg<f32>(read<f32>(%[[VALUE_a_2]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a_2]]), read<f32>(%[[VALUE_b_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a_2]]), read<f32>(%[[VALUE_b_2]])), mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f32>(read<f32>(%[[VALUE_b_2]])), read<f32>(%[[VALUE_a_2]])), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a_2]]), read<f32>(%[[VALUE_b_2]]))), call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f32, signature=fn(complex<f32>) -> f32, abi=sysv64(native_c) -> scalar>(%[[VALUE_cabsf]], sub<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a_2]]), read<f32>(%[[VALUE_b_2]])), complex_convert<complex<f32>, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), mul<f32, rounding=nearest_even, exceptions=observable, contract=fast>(read<f32>(%[[VALUE_a_2]]), neg<f32>(read<f32>(%[[VALUE_b_2]]))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrtl:[0-9]+]] @__builtin_sqrtl(%[[VALUE8:[0-9]+]] <unnamed>: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_testl:[0-9]+]] @testl(%[[VALUE_x_3:[0-9]+]] x: complex<f80>, %[[VALUE_a_3:[0-9]+]] a: f80, %[[VALUE_b_3:[0-9]+]] b: f80) -> void [linkage=external] [abi=sysv64(byval<align=16>, scalar, scalar) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], read<complex<f80>>(%[[VALUE_x_3]])), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], neg<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_x_3]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], read<complex<f80>>(%[[VALUE_x_3]])), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], not<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>>(%[[VALUE_x_3]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%[[VALUE_a_3]]), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%[[VALUE_a_3]]), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%[[VALUE_a_3]]), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f80>(%[[VALUE_a_3]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%[[VALUE_a_3]])), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%[[VALUE_a_3]]), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%[[VALUE_a_3]])), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%[[VALUE_a_3]])), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%[[VALUE_a_3]])), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%[[VALUE_a_3]]), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%[[VALUE_a_3]]), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), read<f80>(%[[VALUE_a_3]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%[[VALUE_a_3]])), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), read<f80>(%[[VALUE_a_3]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<f80>(%[[VALUE_a_3]]), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(neg<complex<i32>, complex=true, overflow=ub>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))), neg<f80>(read<f80>(%[[VALUE_a_3]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], read<f80>(%[[VALUE_a_3]])), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], add<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(neg<f80>(read<f80>(%[[VALUE_a_3]])), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), neg<f80>(read<f80>(%[[VALUE_a_3]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_a_3]]), read<f80>(%[[VALUE_b_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_a_3]]), read<f80>(%[[VALUE_b_3]])), mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(neg<f80>(read<f80>(%[[VALUE_b_3]])), read<f80>(%[[VALUE_a_3]])), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_a_3]]), read<f80>(%[[VALUE_b_3]]))), call<f80, signature=fn(f80) -> f80>(%[[VALUE___builtin_sqrtl]], int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(2)))), call<f80, signature=fn(complex<f80>) -> f80, abi=sysv64(byval<align=16>) -> scalar>(%[[VALUE_cabsl]], sub<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_a_3]]), read<f80>(%[[VALUE_b_3]])), complex_convert<complex<f80>, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))), mul<f80, rounding=nearest_even, exceptions=observable, contract=fast>(read<f80>(%[[VALUE_a_3]]), neg<f80>(read<f80>(%[[VALUE_b_3]]))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(complex<f64>, f64, f64) -> void, abi=sysv64(native_c, scalar, scalar) -> void>(%[[VALUE_test]], real_to_complex<complex<f64>, reason=arg>(int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f32>, f32, f32) -> void, abi=sysv64(native_c, scalar, scalar) -> void>(%[[VALUE_testf]], real_to_complex<complex<f32>, reason=arg>(int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f80>, f80, f80) -> void, abi=sysv64(byval<align=16>, scalar, scalar) -> void>(%[[VALUE_testl]], real_to_complex<complex<f80>, reason=arg>(int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0))), int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)), int_to_float<f80, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
