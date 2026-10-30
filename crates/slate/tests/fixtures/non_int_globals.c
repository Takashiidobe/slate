#include <stdio.h>

static char          small = 12;
static unsigned char byte  = 200;
static float         ratio = 1.5f;
static double        total = 2.25;

static char add_char(char a, char b) { return a + b; }

static float scale(float value, float factor) { return value * factor; }

static double add_double(double a, double b) { return a + b; }

int main(void) {
  small = add_char(small, 3);
  byte  = byte + 1;
  ratio = scale(ratio, 2.0f);
  total = add_double(total, ratio);
  printf("%d\n", small);
  printf("%u\n", byte);
  printf("%f\n", ratio);
  printf("%f\n", total);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut small: i8 = 12;
// CHECK-NEXT: static mut byte: u8 = 200;
// CHECK-NEXT: static mut ratio: f32 = 1.5f32;
// CHECK-NEXT: static mut total: f64 = 2.25f64;
// CHECK-NEXT: fn add_char(a: i8, b: i8) -> i8 {
// CHECK-NEXT:     ((a as i32) + (b as i32)) as i8
// CHECK-NEXT: }
// CHECK-NEXT: fn scale(value: f32, factor: f32) -> f32 {
// CHECK-NEXT:     value * factor
// CHECK-NEXT: }
// CHECK-NEXT: fn add_double(a: f64, b: f64) -> f64 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         small = add_char(small, 3i8);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         byte = ((((byte as u32) as i32) + 1) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ratio = scale(ratio, 2.0f32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         total = add_double(total, ratio as f64);
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", (unsafe { small }) as i32);
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), (byte as u32) as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), ratio as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), total) };
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
