#include <stdio.h>

int main() {
  int i   = 0;
  int sum = 0;
loop:
  sum = sum + i;
  i   = i + 1;
  if (i < 5)
    goto loop;
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut sum: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         i = 0;
// CHECK-NEXT:         sum = 0;
// CHECK-NEXT:         loop {
// CHECK-NEXT:             sum += i;
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:             if !(i < 5) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", sum);
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
