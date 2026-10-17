#include <stdio.h>

int main(void) {
  double        d  = 7.75;
  float         f  = (float)d;
  int           i  = (int)d;
  unsigned int  u  = (unsigned int)-1;
  long          l  = (long)i;
  unsigned char c  = (unsigned char)260;
  _Bool         b0 = (_Bool)0.0;
  _Bool         b1 = (_Bool)-3;
  printf("%f\n", f);
  printf("%d\n", i);
  printf("%u\n", u);
  printf("%ld\n", l);
  printf("%u\n", c);
  printf("%d %d\n", b0, b1);
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
// CHECK-NEXT:     let d: f64 = 7.75f64;
// CHECK-NEXT:     let f: f32 = d as f32;
// CHECK-NEXT:     let i: i32 = d as i32;
// CHECK-NEXT:     let u: u32 = -(1 as i32) as u32;
// CHECK-NEXT:     let l: i64 = i as i64;
// CHECK-NEXT:     let c: u8 = ((260 as i32) as i8) as u8;
// CHECK-NEXT:     let b0: bool = 0.0f64 != 0.0f64;
// CHECK-NEXT:     let b1: bool = -(3 as i32) != (0 as i32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), f as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", i);
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), u) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), l) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), (c as u32) as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{} {}", b0 as i32, b1 as i32);
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
