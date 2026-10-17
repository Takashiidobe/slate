#include <stdio.h>

int main() {
  int x = 2;
  int r = 0;
  switch (x) {
  case 1:
    goto one;
  case 2:
    goto two;
  default:
    goto other;
  }
one:
  r = 10;
  goto done;
two:
  r = 20;
  goto done;
other:
  r = 30;
  goto done;
done:
  printf("%d\n", r);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let x: i32 = 2;
// CHECK-NEXT:         r = 0;
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i32 = x;
// CHECK-NEXT:         if [[T_0]] == 1 {
// CHECK-NEXT:             r = 10;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if [[T_0]] == 2 {
// CHECK-NEXT:                 r = 20;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 r = 30;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", r);
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
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
