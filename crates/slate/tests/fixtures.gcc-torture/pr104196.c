/* PR tree-optimization/104196 */

int a = 6;

int main() {
  while (1) {
    int b = a < 0 && 0 < -__INT_MAX__ - a ? 0 : a;
    if (b != 4096 - __INT_MAX__) {
      if (a < 6)
        __builtin_abort();
      break;
    }
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/104196
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 6 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (1 as i32) != (0 as i32) {
// CHECK-NEXT:         let mut b: i32 =
// CHECK-NEXT:             if (unsafe { a }) < (0 as i32) && (0 as i32) < -(2147483647 as i32) - unsafe { a } {
// CHECK-NEXT:                 0 as i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 unsafe { a }
// CHECK-NEXT:             };
// CHECK-NEXT:         if b != (4096 as i32) - (2147483647 as i32) {
// CHECK-NEXT:             if (unsafe { a }) < (6 as i32) {
// CHECK-NEXT:                 unsafe { __builtin_abort() };
// CHECK-NEXT:             }
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
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
