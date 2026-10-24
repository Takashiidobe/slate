#include <stdio.h>

static int classify(int c) {
  switch (c) {
  case 1:
  case 2:
    break;
  case 3:
    goto low;
  case 4:
    goto high;
  default:
    return -1;
  }
  for (;;) {
    switch (c) {
    case 5:
    low:
      return 100;
    case 6:
    high:
      return 200;
    default:
      return -2;
    }
  }
}

int main() {
  printf("%d\n", classify(1));
  printf("%d\n", classify(3));
  printf("%d\n", classify(4));
  printf("%d\n", classify(5));
  printf("%d\n", classify(6));
  printf("%d\n", classify(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(c: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             '[[JOIN_1:__join_[0-9]+]]: {
// CHECK-NEXT:                 let [[T_0:__t[0-9]+]]: i32 = c;
// CHECK-NEXT:                 if [[T_0]] != 1 {
// CHECK-NEXT:                     if [[T_0]] != 2 {
// CHECK-NEXT:                         if [[T_0]] == 3 {
// CHECK-NEXT:                             break '[[JOIN_0]];
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_0]] == 4 {
// CHECK-NEXT:                                 break '[[JOIN_1]];
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 return -1;
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let [[T_1:__t[0-9]+]]: i32 = c;
// CHECK-NEXT:                 if [[T_1]] == 5 {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_1]] != 6 {
// CHECK-NEXT:                         return -2;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return 200;
// CHECK-NEXT:         }
// CHECK-NEXT:         return 100;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", classify(1i32));
// CHECK-NEXT:     println!("{}", classify(3i32));
// CHECK-NEXT:     println!("{}", classify(4i32));
// CHECK-NEXT:     println!("{}", classify(5i32));
// CHECK-NEXT:     println!("{}", classify(6i32));
// CHECK-NEXT:     println!("{}", classify(9i32));
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
