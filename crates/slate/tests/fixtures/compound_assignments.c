#include <stdio.h>

int main(void) {
  int a  = 20;
  a     -= 5;
  printf("%d\n", a);
  a *= 3;
  printf("%d\n", a);
  a /= 5;
  printf("%d\n", a);
  a %= 7;
  printf("%d\n", a);
  a <<= 3;
  printf("%d\n", a);
  a >>= 2;
  printf("%d\n", a);
  a &= 6;
  printf("%d\n", a);
  a ^= 3;
  printf("%d\n", a);
  a |= 8;
  printf("%d\n", a);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 20;
// CHECK-NEXT:     a -= 5;
// CHECK-NEXT:     println!("{}", a);
// CHECK-NEXT:     a *= 3;
// CHECK-NEXT:     println!("{}", a);
// CHECK-NEXT:     a /= 5;
// CHECK-NEXT:     println!("{}", a);
// CHECK-NEXT:     a %= 7;
// CHECK-NEXT:     println!("{}", a);
// CHECK-NEXT:     a <<= 3i32;
// CHECK-NEXT:     println!("{}", a);
// CHECK-NEXT:     a >>= 2i32;
// CHECK-NEXT:     println!("{}", a);
// CHECK-NEXT:     a &= 6;
// CHECK-NEXT:     println!("{}", a);
// CHECK-NEXT:     a ^= 3;
// CHECK-NEXT:     println!("{}", a);
// CHECK-NEXT:     a |= 8;
// CHECK-NEXT:     println!("{}", a);
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
