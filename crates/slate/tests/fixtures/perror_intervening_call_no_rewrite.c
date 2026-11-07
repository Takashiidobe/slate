#include <stdio.h>

int main(void) {
  int rc = remove("slate_perror_intervening_missing.tmp");
  fflush(stdout);
  if (rc < 0) {
    perror("remove failed");
    return 1;
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush([[ARG_0:arg[0-9]+]]: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::remove;
// CHECK-NEXT: use slate_support::perror;
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let rc: i32 = unsafe {
// CHECK-NEXT:         remove(
// CHECK-NEXT:             c"slate_perror_intervening_missing.tmp".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(slate_support::stdout()) };
// CHECK-NEXT:     if rc < 0 {
// CHECK-NEXT:         unsafe { perror(c"remove failed".as_ptr() as *const core::ffi::c_char) };
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
