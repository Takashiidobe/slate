#include <stdio.h>

int main(void) {
  int rc = remove("slate_perror_call_guard_missing.tmp");
  if (rc < 0) {
    perror("remove failed");
    return 1;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn remove([[ARG_0:arg[0-9]+]]: *const i8) -> i32;
// CHECK-NEXT:     fn perror([[ARG_0]]: *const i8);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let rc: i32 = unsafe { remove(c"slate_perror_call_guard_missing.tmp".as_ptr()) };
// CHECK-NEXT:     if rc < 0 {
// CHECK-NEXT:         unsafe { perror(c"remove failed".as_ptr()) };
// CHECK-NEXT:         return 1;
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
