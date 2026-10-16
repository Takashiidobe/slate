/* PR tree-optimization/58385 */

extern void abort(void);

int a, b = 1;

int foo() {
  b = 0;
  return 0;
}

int main() {
  ((0 || a) & foo() >= 0) <= 1 && 1;
  if (b)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/58385
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     (((0 as i32) != (0 as i32) || (unsafe { a }) != (0 as i32)) as i32)
// CHECK-NEXT:         & ((foo() >= (0 as i32)) as i32)
// CHECK-NEXT:         <= (1 as i32)
// CHECK-NEXT:         && (1 as i32) != (0 as i32);
// CHECK-NEXT:     if (unsafe { b }) != (0 as i32) {
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
