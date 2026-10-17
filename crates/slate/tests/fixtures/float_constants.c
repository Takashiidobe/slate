#include <stdio.h>

static float half(float x) { return x * 0.5f; }
static double third(double x) { return x / 3.0; }

int main(void) {
  float  f0 = 0.1f;
  float  fmn = 1.17549435e-38f;
  float  fsub = 1.4e-45f;
  float  fmx = 3.40282347e+38f;
  double d0 = 0.1;
  double dsub = 4.9406564584124654e-324;
  double dmx = 1.7976931348623157e+308;
  double nz = -0.0;
  double neg = -2.5;
  double big = 1e300;
  double inf = 1e999;
  float  finf = -1e999f;
  printf("%a %a %a %a\n", f0, fmn, fsub, fmx);
  printf("%a %a %a %a %a %a\n", d0, dsub, dmx, nz, neg, big);
  printf("%f %f\n", inf, finf);
  printf("%a %a\n", half(3.0f), third(1.0));
  printf("%.17g %.9g\n", 1.0 / 3.0, 2.0f / 3.0f);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn half(x: f32) -> f32 {
// CHECK-NEXT:     x * 0.5f32
// CHECK-NEXT: }
// CHECK-NEXT: fn third(x: f64) -> f64 {
// CHECK-NEXT:     x / 3.0f64
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let f0: f32 = 0.1f32;
// CHECK-NEXT:     let fmn: f32 = 1.1754944e-38f32;
// CHECK-NEXT:     let fsub: f32 = 1e-45f32;
// CHECK-NEXT:     let fmx: f32 = 3.4028235e38f32;
// CHECK-NEXT:     let d0: f64 = 0.1f64;
// CHECK-NEXT:     let dsub: f64 = 5e-324f64;
// CHECK-NEXT:     let dmx: f64 = 1.7976931348623157e308f64;
// CHECK-NEXT:     let nz: f64 = -0.0f64;
// CHECK-NEXT:     let neg: f64 = -2.5f64;
// CHECK-NEXT:     let big: f64 = 1e300f64;
// CHECK-NEXT:     let inf: f64 = f64::from_bits(0x7ff0000000000000);
// CHECK-NEXT:     let finf: f32 = -f32::from_bits(0x7f800000);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%a %a %a %a\n".as_ptr(), f0 as f64, fmn as f64, fsub as f64, fmx as f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%a %a %a %a %a %a\n".as_ptr(), d0, dsub, dmx, nz, neg, big) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%f %f\n".as_ptr(), inf, finf as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%a %a\n".as_ptr(), half(3.0f32) as f64, third(1.0f64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%.17g %.9g\n".as_ptr(), 1.0f64 / 3.0f64, (2.0f32 / 3.0f32) as f64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
