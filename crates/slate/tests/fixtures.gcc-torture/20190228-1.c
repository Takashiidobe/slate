/* PR tree-optimization/89536 */
/* Testcase by Zhendong Su <su@cs.ucdavis.edu> */

int a = 1;

int main(void) {
  a = ~(a && 1);
  if (a < -1)
    a = ~a;

  if (!a)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/89536
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by Zhendong Su <su@cs.ucdavis.edu>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = !(((unsafe { a }) != (0 as i32) && (1 as i32) != (0 as i32)) as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a }) < -(1 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = !unsafe { a };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if !((unsafe { a }) != (0 as i32)) {
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
