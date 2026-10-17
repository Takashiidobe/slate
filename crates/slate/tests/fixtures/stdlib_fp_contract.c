#include <stdio.h>

int main(void) {
  volatile double x = 0x1.fffffffffffffp+0;
  double          y = x;
  double          z = -(x * x);
  double          contracted;
  double          uncontracted;

  {
#pragma STDC FP_CONTRACT ON
    contracted = x * y + z;
  }

  {
#pragma STDC FP_CONTRACT OFF
    uncontracted = x * y + z;
  }

  printf("%.20e %.20e\n", contracted, uncontracted);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: f64 = 1.9999999999999998f64;
// CHECK-NEXT:     let y: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// CHECK-NEXT:     let z: f64 = -((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) })
// CHECK-NEXT:         * unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) });
// CHECK-NEXT:     let contracted: f64 = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) * y
// CHECK-NEXT:         + z;
// CHECK-NEXT:     let uncontracted: f64 = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) })
// CHECK-NEXT:         * y + z;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.20e %.20e\n".as_ptr(), contracted, uncontracted) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
