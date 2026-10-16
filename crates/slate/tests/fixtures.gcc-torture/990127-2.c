/* { dg-options "-mpc64"  { target { i?86-*-* x86_64-*-* } } } */

extern void abort(void);
extern void exit(int);

void fpEq(double x, double y) {
  if (x != y)
    abort();
}

void fpTest(double x, double y) {
  double result1 = (35.7 * 100.0) / 45.0;
  double result2 = (x * 100.0) / y;
  fpEq(result1, result2);
}

int main() {
  fpTest(35.7, 45.0);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-mpc64"  { target { i?86-*-* x86_64-*-* } } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fpEq(mut x: f64, mut y: f64) {
// CHECK-NEXT:     if x != y {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fpTest(mut x: f64, mut y: f64) {
// CHECK-NEXT:     let mut result1: f64 = 35.7f64 * 100.0f64 / 45.0f64;
// CHECK-NEXT:     let mut result2: f64 = x * 100.0f64 / y;
// CHECK-NEXT:     fpEq(result1, result2);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fpTest(35.7f64, 45.0f64);
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
