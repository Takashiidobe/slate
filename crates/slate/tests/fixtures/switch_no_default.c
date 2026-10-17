#include <stdio.h>

int pick(int x) {
  int out = 5;
  switch (x) {
  case 4:
    out += 4;
    break;
  case 9:
    out += 9;
    break;
  }
  return out;
}

int main(void) {
  printf("%d %d %d\n", pick(4), pick(9), pick(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn pick(x: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 5;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         match x {
// CHECK-NEXT:             4 => {
// CHECK-NEXT:                 out += 4;
// CHECK-NEXT:             }
// CHECK-NEXT:             9 => {
// CHECK-NEXT:                 out += 9;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {}
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {} {}", pick(4 as i32), pick(9 as i32), pick(2 as i32));
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
