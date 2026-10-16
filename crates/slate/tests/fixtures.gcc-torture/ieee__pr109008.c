/* { dg-do run } */
/* PR tree-optimization/109008 */

__attribute__((noipa)) double foo(double eps) {
  double d = 1. + eps;
  if (d == 1.)
    return eps;
  return 0.0;
}

int main() {
  if (foo(__DBL_EPSILON__ / 8.0) == 0.0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! PR tree-optimization/109008
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut eps: f64) -> f64 {
// CHECK-NEXT:     let mut d: f64 = 1.0f64 + eps;
// CHECK-NEXT:     if d == 1.0f64 {
// CHECK-NEXT:         return eps;
// CHECK-NEXT:     }
// CHECK-NEXT:     0.0f64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(2.220446049250313e-16f64 / 8.0f64) == 0.0f64 {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
