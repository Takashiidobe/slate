#include <stdio.h>
#include <stdlib.h>

void touch(int *y) { *y += 1; }

static int use_and_free(int *y) {
  touch(y);
  int v = *y;
  free(y);
  return v;
}

int main(void) {
  int *y = malloc(sizeof(int));
  *y     = 41;
  printf("%d\n", use_and_free(y));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: fn touch([[V_0:__v[0-9]+]]: *mut i32) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *[[V_0]] = *[[V_0]] + 1;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn use_and_free(y: *mut i32) -> i32 {
// CHECK-NEXT:     touch(y);
// CHECK-NEXT:     let v: i32 = unsafe { *y };
// CHECK-NEXT:     unsafe { free(y as *mut ()) };
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let y: *mut i32 = (unsafe { malloc(4 as u64) }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *y = 41;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", use_and_free(y));
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
