#include <stdio.h>

static double avg(double a, double b) {
  double c = (a + b) / 2.0;
  return c;
}

int main(void) {
  float  x = 1.5f;
  double y = 2.25;
  printf("%f\n", x + 0.5f);
  printf("%f\n", avg(3.0, 4.0));
  printf("%.2f\n", y * 2.0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn avg(a: f64, b: f64) -> f64 {
// CHECK-NEXT:     let c: f64 = (a + b) / 2.0f64;
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: f32 = 1.5f32;
// CHECK-NEXT:     let y: f64 = 2.25f64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%f\n".as_ptr(), (x + 0.5f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%f\n".as_ptr(), avg(3.0f64, 4.0f64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%.2f\n".as_ptr(), y * 2.0f64) };
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
