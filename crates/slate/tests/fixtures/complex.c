#include <stdio.h>

int main(void) {
  double _Complex a    = __builtin_complex(1.0, 2.0);
  double _Complex b    = __builtin_complex(3.0, 4.0);
  double _Complex sum  = a + b;
  double _Complex diff = a - b;
  double _Complex prod = a * b;
  double _Complex quot = a / b;
  printf("%d\n", (int)__real__ sum);
  printf("%d\n", (int)__imag__ sum);
  printf("%d\n", (int)__real__ diff);
  printf("%d\n", (int)__imag__ diff);
  printf("%d\n", (int)__real__ prod);
  printf("%d\n", (int)__imag__ prod);
  printf("%d\n", (int)(100.0 * __real__ quot));
  printf("%d\n", (int)(100.0 * __imag__ quot));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: 1.0f64,
// CHECK-NEXT:         im: 2.0f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     let b: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: 3.0f64,
// CHECK-NEXT:         im: 4.0f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     let sum: num_complex::Complex<f64> = a + b;
// CHECK-NEXT:     let diff: num_complex::Complex<f64> = a - b;
// CHECK-NEXT:     let prod: num_complex::Complex<f64> = a.c_mul(b, false);
// CHECK-NEXT:     let quot: num_complex::Complex<f64> = a.c_div(b, false);
// CHECK-NEXT:     println!("{}", sum.re as i32);
// CHECK-NEXT:     println!("{}", sum.im as i32);
// CHECK-NEXT:     println!("{}", diff.re as i32);
// CHECK-NEXT:     println!("{}", diff.im as i32);
// CHECK-NEXT:     println!("{}", prod.re as i32);
// CHECK-NEXT:     println!("{}", prod.im as i32);
// CHECK-NEXT:     println!("{}", (100.0f64 * quot.re) as i32);
// CHECK-NEXT:     println!("{}", (100.0f64 * quot.im) as i32);
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
