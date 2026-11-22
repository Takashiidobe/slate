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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: i32 = 2;
// CHECK-NEXT:     let mut r: i32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: {
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: {
// CHECK-NEXT:             '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: {
// CHECK-NEXT:                 match x {
// CHECK-NEXT:                     1 => {}
// CHECK-NEXT:                     2 => {
// CHECK-NEXT:                         break '[[SLATE_BREAK_2]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                     _ => {
// CHECK-NEXT:                         break '[[SLATE_BREAK_1]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 r = 10;
// CHECK-NEXT:                 break '[[SLATE_BREAK_0]];
// CHECK-NEXT:             }
// CHECK-NEXT:             r = 20;
// CHECK-NEXT:             break '[[SLATE_BREAK_0]];
// CHECK-NEXT:         }
// CHECK-NEXT:         r = 30;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", r);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
