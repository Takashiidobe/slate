/* { dg-do run } */
__attribute__((noipa)) int foo(double a, double b) {
  double c = a - b;
  if (!__builtin_isfinite(c)) {
    if (__builtin_isnan(c)) {
      if (!__builtin_isnan(a) && !__builtin_isnan(b))
        return 1;
    } else if (__builtin_isfinite(a) && __builtin_isfinite(b))
      return 2;
  } else if (c == 0 && a != b)
    return 3;
  return 4;
}

int main() {
  double a = __builtin_inf();
  if (foo(a, a) != 1)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_inf() -> f64;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-do run }
// CHECK-NEXT: fn foo(mut a: f64, mut b: f64) -> i32 {
// CHECK-NEXT:     let mut c: f64 = a - b;
// CHECK-NEXT:     if !c.is_finite() {
// CHECK-NEXT:         if c.is_nan() {
// CHECK-NEXT:             if !a.is_nan() && !b.is_nan() {
// CHECK-NEXT:                 return 1 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if a.is_finite() && b.is_finite() {
// CHECK-NEXT:                 return 2 as i32;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if c == ((0 as i32) as f64) && a != b {
// CHECK-NEXT:             return 3 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     4 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: f64 = f64::INFINITY;
// CHECK-NEXT:     if foo(a, a) != (1 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
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
