#include <stdio.h>

int main(void) {
  FILE *fp = fopen("slate_perror_fopen_missing.tmp", "r");
  if (fp == NULL) {
    perror("open failed");
    return 1;
  }
  fclose(fp);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn perror([[ARG_0:arg[0-9]+]]: *const i8);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let fp: *mut slate_support::FILE = unsafe {
// CHECK-NEXT:         slate_support::fopen(c"slate_perror_fopen_missing.tmp".as_ptr(), c"r".as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT:     if fp.is_null() {
// CHECK-NEXT:         unsafe { perror(c"open failed".as_ptr()) };
// CHECK-NEXT:         return 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { slate_support::fclose(fp) };
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
