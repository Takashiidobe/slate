#include <stdio.h>

static double pick(double *p) {
  *p = *p + 5.0;
  return *p;
}

int main(void) {
  double _Complex z = __builtin_complex(1.0, 2.0);
  __real__ z        = 7.0;
  __imag__ z        = 11.0;
  double r          = pick(&__real__ z);
  double i          = pick(&__imag__ z);
  printf("%d\n", (int)__real__ z);
  printf("%d\n", (int)__imag__ z);
  printf("%d\n", (int)r);
  printf("%d\n", (int)i);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn pick(p: &mut f64) -> f64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(p as *mut f64) = *(p as *mut f64) + 5.0f64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { *(p as *mut f64) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut z: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:         re: 1.0f64,
// CHECK-NEXT:         im: 2.0f64,
// CHECK-NEXT:     };
// CHECK-NEXT:     z.re = 7.0f64;
// CHECK-NEXT:     z.im = 11.0f64;
// CHECK-NEXT:     let r: f64 = pick(unsafe { &mut (*std::ptr::addr_of_mut!(z.re)) });
// CHECK-NEXT:     let i: f64 = pick(unsafe { &mut (*std::ptr::addr_of_mut!(z.im)) });
// CHECK-NEXT:     println!("{}", z.re as i32);
// CHECK-NEXT:     println!("{}", z.im as i32);
// CHECK-NEXT:     println!("{}", r as i32);
// CHECK-NEXT:     println!("{}", i as i32);
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
