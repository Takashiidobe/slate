#include <stdio.h>

static char add_char(char a, char b) {
  char c = a + b;
  return c;
}

int main(void) {
  char          x      = 10;
  unsigned char y      = 200;
  signed char   z      = -5;
  char          letter = 'A';
  printf("%d\n", add_char(x, z));
  printf("%d\n", y);
  printf("%c\n", letter);
  printf("%c\n", letter + 1);
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
// CHECK-NEXT: fn add_char(a: i8, b: i8) -> i8 {
// CHECK-NEXT:     let c: i8 = ((a as i32) + (b as i32)) as i8;
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: i8 = 10;
// CHECK-NEXT:     let y: u8 = ((200 as i32) as i8) as u8;
// CHECK-NEXT:     let z: i8 = -5;
// CHECK-NEXT:     let letter: i8 = 65;
// CHECK-NEXT:     println!("{}", add_char(x, z) as i32);
// CHECK-NEXT:     println!("{}", (y as u32) as i32);
// CHECK-NEXT:     unsafe { printf(c"%c\n".as_ptr(), letter as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%c\n".as_ptr(), (letter as i32) + 1) };
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
