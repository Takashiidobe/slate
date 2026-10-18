#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int) * 2);
  p[0]   = 1;
  p[1]   = 2;
  p      = realloc(p, sizeof(int) * 4);
  p[2]   = 3;
  p[3]   = 4;
  printf("%d\n", p[0] + p[1] + p[2] + p[3]);
  free(p);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn realloc(__ptr: *mut (), __size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut i32 = (unsafe { malloc((4 as u64).wrapping_mul(2 as u64)) })
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.offset(0 as isize) = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.offset(1 as isize) = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     p = (unsafe { realloc(p as *mut (), (4 as u64).wrapping_mul(4 as u64)) })
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.offset(2 as isize) = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *p.offset(3 as isize) = 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (unsafe { * p.offset(0 as isize) }) + unsafe { * p.offset(1 as isize) } +
// CHECK-NEXT:         unsafe { * p.offset(2 as isize) } + unsafe { * p.offset(3 as isize) }
// CHECK-NEXT:     );
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
