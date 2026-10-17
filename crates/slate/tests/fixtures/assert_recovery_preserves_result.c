#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
  int r = (argc == 1) ? 0 : (abort(), 0);
  printf("%d\n", r);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(argc: i32, argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut r: i32 = 0;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     if argc == 1 {
// CHECK-NEXT:         [[V_0]] = 0;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { std::process::abort() };
// CHECK-NEXT:         [[V_0]] = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     r = [[V_0]];
// CHECK-NEXT:     println!("{}", r);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
