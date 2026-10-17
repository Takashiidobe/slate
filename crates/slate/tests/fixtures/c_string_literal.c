#include <stdio.h>

static int first_plus_last(char *s) { return s[0] + s[10]; }

int main(void) {
  char *message = "write error";
  printf("%d\n", first_plus_last(message));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn first_plus_last(s: *mut i8) -> i32 {
// CHECK-NEXT:     ((unsafe { *unsafe { s.offset(0 as isize) } }) as i32)
// CHECK-NEXT:         + ((unsafe { *unsafe { s.offset(10 as isize) } }) as i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let message: *mut i8 = c"write error".as_ptr() as *mut i8;
// CHECK-NEXT:     println!("{}", first_plus_last(message));
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
