/* { dg-options "-fcx-fortran-rules" } */
#include <stdio.h>

static void show(const char *name, double _Complex z) { printf("%s %a %a\n", name, __real__ z, __imag__ z); }

int main(void) {
#ifdef __STDC_IEC_559_COMPLEX__
  puts("iec_559_complex");
#endif
  volatile double big = 1e300, one = 1.0, zero = 0.0, inf = __builtin_inf();
  double _Complex z = big + big * 1.0i;
  show("big", z / z);
  show("small", (one + 2.0i) / (3.0 + 4.0i));
  show("tilted", (one + 2.0i) / (4.0 + 3.0i));
  show("equal", (one + 2.0i) / (3.0 - 3.0i));
  show("zero", (one + one * 1.0i) / (zero + zero * 1.0i));
  show("inf", (inf + one * 1.0i) / (one + one * 1.0i));
  show("mul", (inf + inf * 1.0i) * (one + zero * 1.0i));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-fcx-fortran-rules" }
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// X86_64-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const i8, z: num_complex::Complex<f64>) {
// I686-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const i8, z: num_complex::Complex<f64>) {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: fn show(name: *const u8, z: num_complex::Complex<f64>) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%s %a %a\n".as_ptr(), name, z.re, z.im) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let big: f64 = 1e300f64;
// CHECK-NEXT:     let one: f64 = 1.0f64;
// CHECK-NEXT:     let zero: f64 = 0.0f64;
// CHECK-NEXT:     let inf: f64 = f64::INFINITY;
// CHECK-NEXT:     let mut z: num_complex::Complex<f64> =
// CHECK-NEXT:         (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// CHECK-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) })
// CHECK-NEXT:                 * num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 1.0f64,
// CHECK-NEXT:                 };
// CHECK-NEXT:     show(c"big".as_ptr(), z.c_div_improved(z));
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"small".as_ptr(),
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:             + num_complex::Complex {
// CHECK-NEXT:                 re: 0.0f64,
// CHECK-NEXT:                 im: 2.0f64,
// CHECK-NEXT:             })
// CHECK-NEXT:         .c_div_improved(
// CHECK-NEXT:             3.0f64
// CHECK-NEXT:                 + num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 4.0f64,
// CHECK-NEXT:                 },
// CHECK-NEXT:         ),
// CHECK-NEXT:     );
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"tilted".as_ptr(),
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:             + num_complex::Complex {
// CHECK-NEXT:                 re: 0.0f64,
// CHECK-NEXT:                 im: 2.0f64,
// CHECK-NEXT:             })
// CHECK-NEXT:         .c_div_improved(
// CHECK-NEXT:             4.0f64
// CHECK-NEXT:                 + num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 3.0f64,
// CHECK-NEXT:                 },
// CHECK-NEXT:         ),
// CHECK-NEXT:     );
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"equal".as_ptr(),
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:             + num_complex::Complex {
// CHECK-NEXT:                 re: 0.0f64,
// CHECK-NEXT:                 im: 2.0f64,
// CHECK-NEXT:             })
// CHECK-NEXT:         .c_div_improved(
// CHECK-NEXT:             3.0f64
// CHECK-NEXT:                 - num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 3.0f64,
// CHECK-NEXT:                 },
// CHECK-NEXT:         ),
// CHECK-NEXT:     );
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"zero".as_ptr(),
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:                 * num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 1.0f64,
// CHECK-NEXT:                 })
// CHECK-NEXT:         .c_div_improved(
// CHECK-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// CHECK-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// CHECK-NEXT:                     * num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 1.0f64,
// CHECK-NEXT:                     },
// CHECK-NEXT:         ),
// CHECK-NEXT:     );
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"inf".as_ptr(),
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// CHECK-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:                 * num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 1.0f64,
// CHECK-NEXT:                 })
// CHECK-NEXT:         .c_div_improved(
// CHECK-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:                     * num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 1.0f64,
// CHECK-NEXT:                     },
// CHECK-NEXT:         ),
// CHECK-NEXT:     );
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"mul".as_ptr(),
// CHECK-NEXT:         ((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// CHECK-NEXT:             + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) })
// CHECK-NEXT:                 * num_complex::Complex {
// CHECK-NEXT:                     re: 0.0f64,
// CHECK-NEXT:                     im: 1.0f64,
// CHECK-NEXT:                 })
// CHECK-NEXT:         .c_mul(
// CHECK-NEXT:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:                 + (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) })
// CHECK-NEXT:                     * num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 1.0f64,
// CHECK-NEXT:                     },
// CHECK-NEXT:             true,
// CHECK-NEXT:         ),
// CHECK-NEXT:     );
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
