#include <ctype.h>
#include <stdio.h>

int classify(char c) { return isalpha(c); }

int main(void) {
  if (classify('A')) {
    printf("yes\n");
  } else {
    printf("no\n");
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __ctype_b_loc() -> *mut *const u16;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(c: i8) -> i32 {
// CHECK-NEXT:     (((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { *unsafe { __ctype_b_loc() } }.offset((c as i32) as isize) }
// CHECK-NEXT:     }) as u32) as i32) & 1024
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if classify(65 as i8) != 0 {
// CHECK-NEXT:         println!("yes");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("no");
// CHECK-NEXT:     }
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
