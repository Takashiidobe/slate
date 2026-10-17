#include <complex.h>
#include <stdio.h>

int main(void) {
  volatile double real      = 0.5;
  volatile double imaginary = -0.25;
  double complex  a         = __builtin_complex(real, imaginary);
  double complex  b         = __builtin_complex(real, -imaginary);
  double complex  product;
  double complex  quotient;

  {
#pragma STDC CX_LIMITED_RANGE ON
    product = a * b;
  }

  {
#pragma STDC CX_LIMITED_RANGE OFF
    quotient = a / b;
  }

  printf("%.4f %.4f %.4f %.4f\n", creal(product), cimag(product),
         creal(quotient), cimag(quotient));
  return creal(product) == 0.3125 && cimag(product) == 0.0 &&
                 creal(quotient) == 0.6 && cimag(quotient) == -0.8
             ? 0
             : 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn cimag(__z: num_complex::Complex<f64>) -> f64;
// CHECK-NEXT:     fn creal(__z: num_complex::Complex<f64>) -> f64;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let real: f64 = 0.5f64;
// CHECK-NEXT:     let imaginary: f64 = -0.25f64;
// CHECK-NEXT:     let mut a: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) },
// CHECK-NEXT:         im: unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) },
// CHECK-NEXT:     };
// CHECK-NEXT:     let b: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: unsafe { std::ptr::read_volatile(std::ptr::addr_of!(real)) },
// CHECK-NEXT:         im: -unsafe { std::ptr::read_volatile(std::ptr::addr_of!(imaginary)) },
// CHECK-NEXT:     };
// CHECK-NEXT:     let product: num_complex::Complex<f64> = a.c_mul(b, true);
// CHECK-NEXT:     let quotient: num_complex::Complex<f64> = a.c_div(b, false);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%.4f %.4f %.4f %.4f\n".as_ptr(),
// CHECK-NEXT:             unsafe { creal(product) },
// CHECK-NEXT:             unsafe { cimag(product) },
// CHECK-NEXT:             unsafe { creal(quotient) },
// CHECK-NEXT:             unsafe { cimag(quotient) },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if (unsafe { creal(product) }) == 0.3125f64 && (unsafe { cimag(product) }) == 0.0f64
// CHECK-NEXT:         && (unsafe { creal(quotient) }) == 0.6f64
// CHECK-NEXT:         && (unsafe { cimag(quotient) }) == -0.8f64
// CHECK-NEXT:     {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1
// CHECK-NEXT:     }
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
