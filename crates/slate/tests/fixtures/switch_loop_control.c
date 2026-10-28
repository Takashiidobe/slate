#include <stdio.h>

int run(void) {
  int out = 0;
  for (int i = 0; i <= 3; i++) {
    switch (i) {
    case 0:
      out += 1;
      break;
    case 1:
      continue;
    case 2:
      out += 20;
      break;
    default:
      out += 100;
      break;
    }
    out += 3;
  }
  return out;
}

int main(void) {
  printf("%d\n", run());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn run() -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     for i in 0..4i32 {
// CHECK-NEXT:         match i {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 out += 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 continue;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 out += 20;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 out += 100;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         out += 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", run());
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
