#include <stdio.h>

static int neg_int(int x) { return -x; }

static double neg_double(double x) { return -x; }

int main(void) {
  printf("%d\n", neg_int(7));
  printf("%d\n", neg_int(-12));
  printf("%f\n", neg_double(1.5));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn neg_int(x: i32) -> i32 {
// CHECK-NEXT:     -x
// CHECK-NEXT: }
// CHECK-NEXT: fn neg_double(x: f64) -> f64 {
// CHECK-NEXT:     -x
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", neg_int(7 as i32));
// CHECK-NEXT:     println!("{}", neg_int(- (12 as i32)));
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), neg_double(1.5f64)) };
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
