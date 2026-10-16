/* PR rtl-optimization/118638 */

__attribute__((noipa)) int foo(int x) {
  int a  = x != -3, b, c;
  a     *= 3;
  b      = 2 * x - 9;
  a      = a + b;
  a      = ~a;
  c      = a & 1;
  return -c;
}

int main() {
  if (foo(0) != -1)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/118638
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) -> i32 {
// CHECK-NEXT:     let mut a: i32 = (x != -(3 as i32)) as i32;
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     let mut c: i32 = 0 as i32;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = a;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] * (3 as i32);
// CHECK-NEXT:     a = [[V_1]];
// CHECK-NEXT:     b = (2 as i32) * x - (9 as i32);
// CHECK-NEXT:     a += b;
// CHECK-NEXT:     a = !a;
// CHECK-NEXT:     c = a & (1 as i32);
// CHECK-NEXT:     -c
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(0 as i32) != -(1 as i32) {
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
