#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p      = malloc(sizeof(int));
  int  marker = 7;
  marker      = marker + 1;
  *p          = 41;
  printf("%d %d\n", *p, marker);
  free(p);
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: *mut i32 = (unsafe { malloc(4 as u64) }) as *mut i32;
// CHECK-NEXT:     let mut marker: i32 = 7;
// CHECK-NEXT:     marker += 1;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p = 41;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", unsafe { * p }, marker);
// CHECK-NEXT:     unsafe { free(p as *mut ()) };
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
