#include <stdarg.h>
#include <stdio.h>

static _Float16 add16(_Float16 a, _Float16 b) { return a + b; }

static _Float16 mul16(_Float16 a, _Float16 b) { return a * b; }

static _Float16 sum_variadic(int n, ...) {
  va_list ap;
  va_start(ap, n);
  _Float16 total = (_Float16)0;
  for (int i = 0; i < n; i++) {
    total = total + va_arg(ap, _Float16);
  }
  va_end(ap);
  return total;
}

int main(void) {
  _Float16 a = 3.0f16;
  _Float16 b = 4.0f16;
  printf("%d\n", (int)add16(a, b));
  printf("%d\n", (int)mul16(a, b));
  printf("%d\n", (int)sum_variadic(3, (_Float16)1.0f16, (_Float16)2.0f16,
                                   (_Float16)3.0f16));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f16)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn add16(a: f16, b: f16) -> f16 {
// CHECK-NEXT:     ((a as f32) + (b as f32)) as f16
// CHECK-NEXT: }
// CHECK-NEXT: fn mul16(a: f16, b: f16) -> f16 {
// CHECK-NEXT:     ((a as f32) * (b as f32)) as f16
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C-unwind" fn sum_variadic(n: i32, mut __va_args: ...) -> f16 {
// CHECK-NEXT:     let mut ap: core::ffi::VaList<'static> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     ap = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::ffi::VaList<'_>,
// CHECK-NEXT:             core::ffi::VaList<'static>,
// CHECK-NEXT:         >(__va_args.clone())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut total: f16 = (0 as i32) as f16;
// CHECK-NEXT:     for i in 0..n {
// CHECK-NEXT:         total = ((total as f32)
// CHECK-NEXT:             + ((unsafe { f16::from_bits(ap.next_arg::<f64>().to_bits() as u16) })
// CHECK-NEXT:                 as f32)) as f16;
// CHECK-NEXT:     }
// CHECK-NEXT:     {}
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: f16 = 3f16;
// CHECK-NEXT:     let b: f16 = 4f16;
// CHECK-NEXT:     println!("{}", add16(a, b) as i32);
// CHECK-NEXT:     println!("{}", mul16(a, b) as i32);
// CHECK-NEXT:     println!("{}", (unsafe { sum_variadic(3 as i32, 1f16, 2f16, 3f16) }) as i32);
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
