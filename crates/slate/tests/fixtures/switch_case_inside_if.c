#include <stdio.h>

static int lookup(int status, int found) {
  switch (status) {
  case 12:
    return 3;
  case 0:
    if (!found)
    default:
      return 2;
  }
  return 0;
}

static int skip_into_else(int x, int y) {
  int out = 0;
  switch (x) {
  case 1:
    if (y > 0) {
      out += 10;
    } else {
    case 2:
      out += 20;
    }
    out += 1;
    break;
  case 3:
    out += 30;
  }
  return out;
}

int main(void) {
  for (int status = -1; status <= 13; status++)
    printf("%d:%d,%d ", status, lookup(status, 0), lookup(status, 1));
  printf("\n");
  for (int x = 0; x <= 4; x++)
    printf("%d:%d,%d ", x, skip_into_else(x, 1), skip_into_else(x, -1));
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn lookup(status: i32, found: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i32 = status;
// CHECK-NEXT:         if [[T_0]] == 12 {
// CHECK-NEXT:             return 3;
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if [[T_0]] == 0 {
// CHECK-NEXT:                 if found != 0 {
// CHECK-NEXT:                     return 0;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         return 2;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn skip_into_else(x: i32, y: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             '[[JOIN_1:__join_[0-9]+]]: {
// CHECK-NEXT:                 out = 0;
// CHECK-NEXT:                 let [[T_0]]: i32 = x;
// CHECK-NEXT:                 if [[T_0]] == 1 {
// CHECK-NEXT:                     if y > 0 {
// CHECK-NEXT:                         out += 10;
// CHECK-NEXT:                         break '[[JOIN_1]];
// CHECK-NEXT:                     }
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_0]] != 2 {
// CHECK-NEXT:                         if [[T_0]] == 3 {
// CHECK-NEXT:                             out += 30;
// CHECK-NEXT:                             break '[[JOIN_0]];
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             break '[[JOIN_0]];
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 out += 20;
// CHECK-NEXT:             }
// CHECK-NEXT:             out += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         return out;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for status in -1..14i32 {
// CHECK-NEXT:         print!("{}:{},{} ", status, lookup(status, 0i32), lookup(status, 1i32));
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     for x in 0..5i32 {
// CHECK-NEXT:         print!("{}:{},{} ", x, skip_into_else(x, 1i32), skip_into_else(x, - 1i32));
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
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
