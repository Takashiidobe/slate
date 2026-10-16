/* PR tree-optimization/69097 */

__attribute__((noinline, noclone)) int f1(int x, int y) { return x % y; }

__attribute__((noinline, noclone)) int f2(int x, int y) { return x % -y; }

__attribute__((noinline, noclone)) int f3(int x, int y) {
  int z = -y;
  return x % z;
}

int main() {
  if (f1(-__INT_MAX__ - 1, 1) != 0 || f2(-__INT_MAX__ - 1, -1) != 0 ||
      f3(-__INT_MAX__ - 1, -1) != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/69097
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut x: i32, mut y: i32) -> i32 {
// CHECK-NEXT:     x % y
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut x: i32, mut y: i32) -> i32 {
// CHECK-NEXT:     x % -y
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut x: i32, mut y: i32) -> i32 {
// CHECK-NEXT:     let mut z: i32 = -y;
// CHECK-NEXT:     x % z
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bool;
// CHECK-NEXT:     if f1(-(2147483647 as i32) - (1 as i32), 1 as i32) != (0 as i32) {
// CHECK-NEXT:         [[V_0]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_0]] = f2(-(2147483647 as i32) - (1 as i32), -(1 as i32)) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_0]] {
// CHECK-NEXT:         [[V_1]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = f3(-(2147483647 as i32) - (1 as i32), -(1 as i32)) != (0 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_1]] {
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
