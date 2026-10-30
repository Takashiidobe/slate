#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *x = malloc(sizeof(int));
  free(x);
  *x = 4;
  printf("%d %d\n", *x, *x);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc([[ARG_0]]: u64) -> *mut ();
// CHECK-NEXT:     fn free([[ARG_0]]: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: *mut i32 = (unsafe { malloc(4u64) }) as *mut i32;
// CHECK-NEXT:     unsafe { free(x as *mut ()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *x = 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", unsafe { * x }, unsafe { * x });
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
