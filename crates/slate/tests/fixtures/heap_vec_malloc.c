#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int) * 3);
  p[0]   = 1;
  p[1]   = 2;
  p[2]   = 3;
  printf("%d\n", p[0] + p[1] + p[2]);
  free(p);
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let p: *mut i32 = (unsafe { malloc((4 as u64).wrapping_mul(3 as u64)) }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p.offset(0 as isize) } = 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p.offset(1 as isize) } = 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { p.offset(2 as isize) } = 3;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (unsafe { * unsafe { p.offset(0 as isize) } }) + unsafe { * unsafe { p
// CHECK-NEXT:         .offset(1 as isize) } } + unsafe { * unsafe { p.offset(2 as isize) } }
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
