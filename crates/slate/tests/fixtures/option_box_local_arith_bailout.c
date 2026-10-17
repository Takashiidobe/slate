#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  if (flag) {
    p = malloc(sizeof(int) * 4);
  }
  if (p) {
    p  = p + 1;
    *p = 41;
    printf("%d\n", *p);
    free(p - 1);
  } else {
    printf("no alloc\n");
  }
  return 0;
}

int main(void) {
  compute(1);
  compute(0);
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
// CHECK-NEXT: fn compute(flag: i32) -> i32 {
// CHECK-NEXT:     let mut p: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = (unsafe { malloc((4 as u64).wrapping_mul(4 as u64)) }) as *mut i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if p != std::ptr::null_mut::<i32>() {
// CHECK-NEXT:         p = unsafe { p.offset(1 as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p = 41;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", unsafe { * p });
// CHECK-NEXT:         unsafe { free((unsafe { p.offset(-(1 as isize)) }) as *mut ()) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("no alloc");
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     compute(1 as i32);
// CHECK-NEXT:     compute(0 as i32);
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
