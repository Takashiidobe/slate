/* { dg-skip-if "signed zero not supported" { "vax-*-*" } } */
extern void abort(void);

double __attribute__((noinline)) not_fabs(double x) {
  return x >= 0.0 ? x : -x;
}

int main() {
  double x = -0.0;
  double y;

  y = not_fabs(x);

  if (!__builtin_signbit(y))
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-skip-if "signed zero not supported" { "vax-*-*" } }
// CHECK-NEXT: fn not_fabs(mut x: f64) -> f64 {
// CHECK-NEXT:     if x >= 0.0f64 { x } else { -x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: f64 = -0.0f64;
// CHECK-NEXT:     let mut y: f64 = not_fabs(x);
// CHECK-NEXT:     if !y.is_sign_negative() {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
