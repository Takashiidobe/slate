#include <stdio.h>
#include <stdlib.h>

static void just_free(int *p) { free(p); }

int main(void) {
  int *x = malloc(sizeof(int));
  *x     = 4;
  printf("%d\n", *x);
  just_free(x);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: fn just_free(p: *mut i32) {
// CHECK-NEXT:     unsafe { free(p as *mut ()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: *mut i32 = (unsafe { malloc(4 as u64) }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *x = 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { * x });
// CHECK-NEXT:     just_free(x);
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
