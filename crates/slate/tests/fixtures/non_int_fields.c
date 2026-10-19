#include <stdio.h>

struct Mixed {
  char          tag;
  unsigned char code;
  float         ratio;
  double        total;
};

union Scalar {
  char          tag;
  unsigned char code;
  float         ratio;
  double        total;
};

static int struct_values(void) {
  struct Mixed m;
  m.tag   = 'A';
  m.code  = 200;
  m.ratio = 1.5f;
  m.total = 2.25;
  printf("%c\n", m.tag);
  printf("%u\n", m.code);
  printf("%f\n", m.ratio);
  printf("%f\n", m.total);
  return m.code;
}

static double union_double(double value) {
  union Scalar s;
  s.total = value;
  return s.total;
}

static int union_char(char value) {
  union Scalar s;
  s.tag = value;
  return s.tag;
}

int main(void) {
  printf("%d\n", struct_values());
  printf("%f\n", union_double(4.5));
  printf("%d\n", union_char('Z'));
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Mixed {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     code: u8,
// CHECK-NEXT:     ratio: f32,
// CHECK-NEXT:     total: f64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Scalar {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     code: u8,
// CHECK-NEXT:     ratio: f32,
// CHECK-NEXT:     total: f64,
// CHECK-NEXT: }
// CHECK-NEXT: fn struct_values() -> i32 {
// CHECK-NEXT:     let mut m: Mixed = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     m.tag = 65;
// CHECK-NEXT:     m.code = 200u8;
// CHECK-NEXT:     m.ratio = 1.5f32;
// CHECK-NEXT:     m.total = 2.25f64;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%c\n".as_ptr(), m.tag as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), (m.code as u32) as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), m.ratio as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), m.total) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     (m.code as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn union_double(value: f64) -> f64 {
// CHECK-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.total = value;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { s.total }
// CHECK-NEXT: }
// CHECK-NEXT: fn union_char(value: i8) -> i32 {
// CHECK-NEXT:     let mut s: Scalar = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         s.tag = value;
// CHECK-NEXT:     }
// CHECK-NEXT:     (unsafe { s.tag }) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", struct_values());
// CHECK-NEXT:     unsafe { printf(c"%f\n".as_ptr(), union_double(4.5f64)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", union_char(90i8));
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
