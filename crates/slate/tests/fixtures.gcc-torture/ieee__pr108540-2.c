/* { dg-do run } */
/* PR tree-optimization/108540 */

__attribute__((noipa)) int foo(int x, double d) {
  if (x == 42)
    d = -0.0;
  if (d == 0.0)
    return 42;
  return 12;
}

int main() {
  if (foo(42, 5.0) != 42 || foo(42, 0.0) != 42 || foo(42, -0.0) != 42 ||
      foo(10, 5.0) != 12 || foo(10, 0.0) != 42 || foo(10, -0.0) != 42)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! PR tree-optimization/108540
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut d: f64) -> i32 {
// CHECK-NEXT:     if x == (42 as i32) {
// CHECK-NEXT:         d = -0.0f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if d == 0.0f64 {
// CHECK-NEXT:         return 42 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     12 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if foo(42 as i32, 5.0f64) != (42 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = foo(42 as i32, 0.0f64) != (42 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = foo(42 as i32, -0.0f64) != (42 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_1]] {
// CHECK-NEXT:         [[V_2]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_2]] = foo(10 as i32, 5.0f64) != (12 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_2]] {
// CHECK-NEXT:         [[V_3]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_3]] = foo(10 as i32, 0.0f64) != (42 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_3]] {
// CHECK-NEXT:         [[V_4]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_4]] = foo(10 as i32, -0.0f64) != (42 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_4]] {
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
