#include <stdio.h>

int main(void) {
  int i     = 0;
  int steps = 0;
  while (i < 6) {
    int x = i % 3;
    switch (x) {
    case 0:
    case 1:
    case 2:
      i++;
      steps++;
      continue;
    default:
      break;
    }
    printf("unreachable %d\n", i);
    i++;
    steps++;
  }
  printf("steps=%d\n", steps);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0;
// CHECK-NEXT:     let mut steps: i32 = 0;
// CHECK-NEXT:     while i < 6 {
// CHECK-NEXT:         '[[SLATE_CONTINUE_0:__slate_continue_[0-9]+]]: {
// CHECK-NEXT:             let x: i32 = i % 3;
// CHECK-NEXT:             match x {
// CHECK-NEXT:                 0..=2 => {
// CHECK-NEXT:                     i += 1;
// CHECK-NEXT:                     steps += 1;
// CHECK-NEXT:                     break '[[SLATE_CONTINUE_0]];
// CHECK-NEXT:                 }
// CHECK-NEXT:                 _ => {}
// CHECK-NEXT:             }
// CHECK-NEXT:             println!("unreachable {}", i);
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:             steps += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("steps={}", steps);
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
