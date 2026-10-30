#include <stdio.h>

static short sadd(short a, short b) {
  short c = a + b;
  return c;
}

int main(void) {
  short          s   = 30000;
  unsigned short us  = 60000;
  short          neg = -12345;
  printf("%d\n", sadd(s, -30000));
  printf("%d\n", us);
  printf("%d\n", neg);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn sadd(a: i16, b: i16) -> i16 {
// CHECK-NEXT:     let c: i16 = ((a as i32) + (b as i32)) as i16;
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let s: i16 = 30000;
// CHECK-NEXT:     let us: u16 = 60000u16;
// CHECK-NEXT:     let neg: i16 = -12345;
// CHECK-NEXT:     println!("{}", sadd(s, - 30000i16) as i32);
// CHECK-NEXT:     println!("{}", (us as u32) as i32);
// CHECK-NEXT:     println!("{}", neg as i32);
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
