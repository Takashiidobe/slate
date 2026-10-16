int *__restrict__ x;

int foo(int y) {
  *x = y;
  return *x;
}

extern void abort(void);

int main() {
  int i = 0;
  x     = &i;
  if (foo(1) != 1)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: static mut x: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn foo(mut y: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { x } = y;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *unsafe { x } }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x = std::ptr::addr_of_mut!(i);
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo(1 as i32) != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
