/* PR target/60072 */

int c = 1;

__attribute__((optimize(1))) static int *foo(int *p) { return p; }

int main() {
  *foo(&c) = 2;
  return c - 2;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/60072
// CHECK-NEXT: static mut c: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: *mut i32) -> *mut i32 {
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *foo(unsafe { std::ptr::addr_of_mut!(c) }) = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { c }) - (2 as i32)
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
