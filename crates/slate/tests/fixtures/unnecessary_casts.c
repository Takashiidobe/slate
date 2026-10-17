#include <stdio.h>

int main(void) {
  unsigned char a = 200;
  unsigned char b = 100;
  unsigned char c = 0;
  for (int i = 0; i < 1; i++) {
    c = a + b;
  }
  printf("%d\n", c);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: u8 = ((200 as i32) as i8) as u8;
// CHECK-NEXT:     let b: u8 = 100;
// CHECK-NEXT:     let mut c: u8 = 0;
// CHECK-NEXT:     for i in 0 as i32..1 as i32 {
// CHECK-NEXT:         c = ((((a as u32) as i32) + ((b as u32) as i32)) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", (c as u32) as i32);
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
