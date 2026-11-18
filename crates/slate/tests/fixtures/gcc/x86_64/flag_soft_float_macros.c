/* { dg-options "-msoft-float" } */
#include <stdio.h>

int main(void) {
#ifdef _SOFT_FLOAT
  int soft = 1;
#else
  int soft = 0;
#endif
#ifdef __SSE__
  int sse = 1;
#else
  int sse = 0;
#endif
  printf("%d %d\n", soft, sse);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-msoft-float" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let soft: i32 = 1;
// CHECK-NEXT:     let sse: i32 = 1;
// CHECK-NEXT:     println!("{} {}", soft, sse);
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
