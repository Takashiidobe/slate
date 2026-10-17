#include <stdio.h>

static double zero(void) { return 0.0; }

static int compare_all(double a, double b) {
  return (a < b) | (a <= b) << 1 | (a > b) << 2 | (a >= b) << 3 | (a == b) << 4 |
         (a != b) << 5;
}

int main(void) {
  double nan = zero() / zero();
  double inf = 1.0 / zero();
  printf("%d %d %d\n", compare_all(nan, nan), compare_all(nan, 1.0), compare_all(1.0, 2.0));
  printf("%d %d\n", compare_all(-0.0, 0.0), compare_all(inf, -inf));
  printf("%d %d %d\n", !nan, nan ? 1 : 0, -0.0 ? 1 : 0);
  printf("%g %g %d\n", inf, -inf, inf - inf == inf - inf);

  float third = 1.0f / 3.0f;
  float sum   = 0.1f + 0.2f;
  printf("%.10f %.10f %d\n", third, sum, sum == 0.3f);
  double dsum = 0.1 + 0.2;
  printf("%.17g %d\n", dsum, dsum == 0.3);

  float f = 16777216.0f;
  f      += 1.0f;
  printf("%.1f\n", f);

  double d = 2.5;
  d       *= 3;
  d       -= 0.5;
  d       /= 4;
  printf("%g\n", d);

  int i = 7;
  printf("%g %g\n", i / 2.0, i / 2 * 1.0);
  printf("%g %g\n", -d, -(0.0));
  float neg = -1.5f;
  printf("%g %g\n", neg * neg, neg / 0.5f);

  double values[] = {3.5, -1.25, 0.0, 1e300};
  double max      = values[0];
  for (int k = 1; k < 4; k++)
    max = values[k] > max ? values[k] : max;
  printf("%g %g\n", max, values[3] * 10);
  printf("%d %d\n", 1.0f < 1.0, (float)0.1 == 0.1);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn zero() -> f64 {
// CHECK-NEXT:     0.0f64
// CHECK-NEXT: }
// CHECK-NEXT: fn compare_all(a: f64, b: f64) -> i32 {
// CHECK-NEXT:     ((a < b) as i32) | ((a <= b) as i32) << (1 as i32) | ((a > b) as i32) << (2 as i32)
// CHECK-NEXT:         | ((a >= b) as i32) << (3 as i32) | ((a == b) as i32) << (4 as i32)
// CHECK-NEXT:         | ((a != b) as i32) << (5 as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let nan: f64 = zero() / zero();
// CHECK-NEXT:     let inf: f64 = 1.0f64 / zero();
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", compare_all(nan, nan), compare_all(nan, 1.0f64), compare_all(1.0f64,
// CHECK-NEXT:         2.0f64)
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{} {}", compare_all(- 0.0f64, 0.0f64), compare_all(inf, - inf));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}", ! (nan != 0.0f64) as i32, if nan != 0.0f64 { 1 as i32 } else { 0 as
// CHECK-NEXT:         i32 }, if - 0.0f64 != 0.0f64 { 1 as i32 } else { 0 as i32 }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%g %g %d\n".as_ptr(), inf, -inf, (inf - inf == inf - inf) as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let third: f32 = 1.0f32 / 3.0f32;
// CHECK-NEXT:     let sum: f32 = 0.1f32 + 0.2f32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.10f %.10f %d\n".as_ptr(),
// CHECK-NEXT:             third as f64,
// CHECK-NEXT:             sum as f64,
// CHECK-NEXT:             (sum == 0.3f32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let dsum: f64 = 0.1f64 + 0.2f64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.17g %d\n".as_ptr(), dsum, (dsum == 0.3f64) as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut f: f32 = 16777216.0f32;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: f32 = f + 1.0f32;
// CHECK-NEXT:     f = [[V_0]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.1f\n".as_ptr(), f as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut d: f64 = 2.5f64;
// CHECK-NEXT:     d *= (3 as i32) as f64;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: f64 = d - 0.5f64;
// CHECK-NEXT:     d = [[V_1]];
// CHECK-NEXT:     d /= (4 as i32) as f64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g\n".as_ptr(), d) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let i: i32 = 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%g %g\n".as_ptr(), (i as f64) / 2.0f64, ((i / 2) as f64) * 1.0f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g %g\n".as_ptr(), -d, -0.0f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let neg: f32 = -1.5f32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g %g\n".as_ptr(), (neg * neg) as f64, (neg / 0.5f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let values: __SlateAlign16<[f64; 4]> = __SlateAlign16([
// CHECK-NEXT:         3.5f64,
// CHECK-NEXT:         -1.25f64,
// CHECK-NEXT:         0.0f64,
// CHECK-NEXT:         1e300f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut max: f64 = values.0[0];
// CHECK-NEXT:     for k in 1 as i32..4 as i32 {
// CHECK-NEXT:         max = if values.0[k as usize] > max { values.0[k as usize] } else { max };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%g %g\n".as_ptr(), max, values.0[3] * ((10 as i32) as f64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", ((1.0f32 as f64) < 1.0f64) as i32, (((0.1f64 as f32) as f64) == 0.1f64)
// CHECK-NEXT:         as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
