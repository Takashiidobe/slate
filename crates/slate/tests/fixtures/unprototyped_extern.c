/* { dg-options "-std=gnu17" } */
#include <stdio.h>

int abs();
double atof();

int add(int a, int b) { return a + b; }

struct ops {
  int (*apply)();
};

int main() {
  int (*absolute)() = abs;
  struct ops ops = {add};
  printf("%d %d\n", abs(-7), absolute(-9));
  printf("%d %.1f\n", ops.apply(3, 4), atof("2.5"));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=gnu17" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abs(...) -> i32;
// CHECK-NEXT:     fn atof(...) -> f64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn add(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ops {
// CHECK-NEXT:     apply: Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let absolute: Option<unsafe extern "C-unwind" fn() -> i32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<*const (), Option<unsafe extern "C-unwind" fn() -> i32>>(
// CHECK-NEXT:             abs as *const (),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let ops: ops = ops {
// CHECK-NEXT:         apply: unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:             >(Some(add))
// CHECK-NEXT:         },
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {}", (-7i32).wrapping_abs(), unsafe {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                 Option<unsafe extern "C-unwind" fn(i32) -> i32>,
// CHECK-NEXT:             >(absolute)
// CHECK-NEXT:         }
// CHECK-NEXT:         .unwrap()(-9i32)
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %.1f\n".as_ptr(),
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn() -> i32>,
// CHECK-NEXT:                     Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// CHECK-NEXT:                 >(ops.apply)
// CHECK-NEXT:             }
// CHECK-NEXT:             .unwrap()(3i32, 4i32),
// CHECK-NEXT:             atof(c"2.5".as_ptr() as *mut i8),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
