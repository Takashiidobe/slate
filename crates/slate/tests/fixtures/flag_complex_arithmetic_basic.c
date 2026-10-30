/* { dg-options "-fcomplex-arithmetic=basic" } */
#include <stdio.h>
#include <math.h>

int main(void) {
  volatile double big = 1e300;
  double _Complex z = big + big * 1.0i;
  double _Complex q = z / z;
  double _Complex small = (1.0 + 2.0i) / (3.0 + 4.0i);
  printf("%d %d %.6f %.6f\n", isnan(__real__ q) || isinf(__real__ q), isnan(__imag__ q) || isinf(__imag__ q),
         __real__ small, __imag__ small);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fcomplex-arithmetic=basic" }
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let big: f64 = 1e300f64;
// CHECK-NEXT:     let mut z: num_complex::Complex<f64> =
// CHECK-NEXT:         (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// CHECK-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// CHECK-NEXT:                 * num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 1.0f64,
// CHECK-NEXT:                 };
// CHECK-NEXT:     let mut q: num_complex::Complex<f64> = z.c_div(z, true);
// CHECK-NEXT:     let small: num_complex::Complex<f64> = (1.0f64
// CHECK-NEXT:         + num_complex::Complex {
// CHECK-NEXT:             re: 0.0f64,
// CHECK-NEXT:             im: 2.0f64,
// CHECK-NEXT:         })
// CHECK-NEXT:     .c_div(
// CHECK-NEXT:         3.0f64
// CHECK-NEXT:             + num_complex::Complex {
// CHECK-NEXT:                 re: 0.0f64,
// CHECK-NEXT:                 im: 4.0f64,
// CHECK-NEXT:             },
// CHECK-NEXT:         true,
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %.6f %.6f\n".as_ptr(),
// CHECK-NEXT:             (q.re.is_nan()
// CHECK-NEXT:                 || (if q.re.is_infinite() {
// CHECK-NEXT:                     if q.re.is_sign_negative() { -1i32 } else { 1i32 }
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     0i32
// CHECK-NEXT:                 }) != 0) as i32,
// CHECK-NEXT:             (q.im.is_nan()
// CHECK-NEXT:                 || (if q.im.is_infinite() {
// CHECK-NEXT:                     if q.im.is_sign_negative() { -1i32 } else { 1i32 }
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     0i32
// CHECK-NEXT:                 }) != 0) as i32,
// CHECK-NEXT:             small.re,
// CHECK-NEXT:             small.im,
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
