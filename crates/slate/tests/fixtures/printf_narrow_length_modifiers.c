#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
  short          s  = 300;
  unsigned short us = 60000;
  signed char    c  = -5;
  unsigned char  uc = 200;
  intmax_t       j  = 123456789L;
  uintmax_t      ju = 123456789UL;
  ptrdiff_t      t  = -7;
  printf("%hd %hu %hhd %hhu %jd %ju %td\n", s, us, c, uc, j, ju, t);

  int          full_int = 300;
  int          negative = -1;
  unsigned int wide     = 70000;
  printf("%hhd %hhd %hhu %hd %hu\n", full_int, uc, negative, wide, wide);
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let s: i16 = 300;
// CHECK-NEXT:     let us: u16 = ((60000 as i32) as i16) as u16;
// CHECK-NEXT:     let c: i8 = -5;
// CHECK-NEXT:     let uc: u8 = ((200 as i32) as i8) as u8;
// CHECK-NEXT:     let j: i64 = 123456789;
// CHECK-NEXT:     let ju: u64 = 123456789;
// CHECK-NEXT:     let t: i64 = -7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%hd %hu %hhd %hhu %jd %ju %td\n".as_ptr(),
// CHECK-NEXT:             s as i32,
// CHECK-NEXT:             (us as u32) as i32,
// CHECK-NEXT:             c as i32,
// CHECK-NEXT:             (uc as u32) as i32,
// CHECK-NEXT:             j,
// CHECK-NEXT:             ju,
// CHECK-NEXT:             t,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let full_int: i32 = 300;
// CHECK-NEXT:     let negative: i32 = -1;
// CHECK-NEXT:     let wide: u32 = 70000;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%hhd %hhd %hhu %hd %hu\n".as_ptr(),
// CHECK-NEXT:             full_int,
// CHECK-NEXT:             (uc as u32) as i32,
// CHECK-NEXT:             negative,
// CHECK-NEXT:             wide,
// CHECK-NEXT:             wide,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
