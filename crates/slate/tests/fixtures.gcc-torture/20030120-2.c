/* PR 8848 */

extern void abort();

int foo(int status) {
  int s = 0;
  if (status == 1)
    s = 1;
  if (status == 3)
    s = 3;
  if (status == 4)
    s = 4;
  return s;
}

int main() {
  if (foo(3) != 3)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR 8848
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut status: i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0 as i32;
// CHECK-NEXT:     if status == (1 as i32) {
// CHECK-NEXT:         s = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if status == (3 as i32) {
// CHECK-NEXT:         s = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if status == (4 as i32) {
// CHECK-NEXT:         s = 4 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo(3 as i32) != (3 as i32) {
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
