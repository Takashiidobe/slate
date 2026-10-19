#include <stdio.h>

typedef int           my_int;
typedef unsigned char byte;
typedef long long     wide;

struct Box {
  my_int value;
  byte   tag;
};

static my_int add_alias(my_int a, my_int b) {
  my_int c = a + b;
  return c;
}

int main(void) {
  my_int     x = 40;
  byte       y = 200;
  wide       z = 9000000000LL;
  struct Box bx;
  bx.value = add_alias(x, 2);
  bx.tag   = y;
  printf("%d\n", bx.value);
  printf("%d\n", bx.tag);
  printf("%lld\n", z);
  printf("%d\n", (int)sizeof(my_int));
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
// CHECK-NEXT: struct Box {
// CHECK-NEXT:     value: i32,
// CHECK-NEXT:     tag: u8,
// CHECK-NEXT: }
// CHECK-NEXT: fn add_alias(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let c: i32 = a + b;
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: i32 = 40;
// CHECK-NEXT:     let y: u8 = 200u8;
// CHECK-NEXT:     let z: i64 = 9000000000;
// CHECK-NEXT:     let mut bx: Box = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     bx.value = add_alias(x, 2i32);
// CHECK-NEXT:     bx.tag = y;
// CHECK-NEXT:     println!("{}", bx.value);
// CHECK-NEXT:     println!("{}", (bx.tag as u32) as i32);
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), z) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", 4i32);
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
