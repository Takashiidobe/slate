#include <stdio.h>

int main() {
  int a = 3;
  int b = -1;
  int r;
  if (a > 0) {
    if (b > 0)
      goto both_pos;
    else
      goto a_pos;
  } else {
    goto other;
  }
both_pos:
  r = 1;
  goto done;
a_pos:
  r = 2;
  goto done;
other:
  r = 3;
  goto done;
done:
  printf("%d\n", r);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let a: i32 = 3;
// CHECK-NEXT:         b = -1;
// CHECK-NEXT:         if a > 0 {
// CHECK-NEXT:             if b > 0 {
// CHECK-NEXT:                 r = 1;
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 r = 2;
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             r = 3;
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
