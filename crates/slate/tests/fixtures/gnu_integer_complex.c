#include <stdio.h>

static __complex__ int global_value = 17 + 19i;

int main(void) {
  __complex__ int first     = 5 + 7i;
  __complex__ int second    = -3 + 11i;
  __complex__ int imaginary = 13i;
  printf("%d %d %d %d %d %d %d %d\n", __real__ first, __imag__ first,
         __real__ second, __imag__ second, __real__ imaginary,
         __imag__ imaginary, __real__ global_value, __imag__ global_value);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut global_value: num_complex::Complex<i32> = {
// CHECK-NEXT:     let [[T_0:__t[0-9]+]] = 17i32;
// CHECK-NEXT:     let [[T_1:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:         re: 0i32,
// CHECK-NEXT:         im: 19i32,
// CHECK-NEXT:     };
// CHECK-NEXT:     num_complex::Complex {
// CHECK-NEXT:         re: [[T_0]] + [[T_1]].re,
// CHECK-NEXT:         im: [[T_1]].im,
// CHECK-NEXT:     }
// CHECK-NEXT: };
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let first: num_complex::Complex<i32> = 5
// CHECK-NEXT:         + num_complex::Complex {
// CHECK-NEXT:             re: 0,
// CHECK-NEXT:             im: 7,
// CHECK-NEXT:         };
// CHECK-NEXT:     let second: num_complex::Complex<i32> = -3
// CHECK-NEXT:         + num_complex::Complex {
// CHECK-NEXT:             re: 0,
// CHECK-NEXT:             im: 11,
// CHECK-NEXT:         };
// CHECK-NEXT:     let imaginary: num_complex::Complex<i32> = num_complex::Complex {
// CHECK-NEXT:         re: 0,
// CHECK-NEXT:         im: 13,
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {}", first.re, first.im, second.re, second.im, imaginary
// CHECK-NEXT:         .re, imaginary.im, unsafe { global_value.re }, unsafe { global_value.im }
// CHECK-NEXT:     );
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
