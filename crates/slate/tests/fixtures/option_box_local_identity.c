#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  int *q = NULL;
  if (flag) {
    p = malloc(sizeof(int));
    q = malloc(sizeof(int));
  }
  if (p == q) {
    printf("same\n");
  } else {
    printf("diff\n");
  }
  if (p) {
    *p = 1;
    printf("%d\n", *p);
    free(p);
  }
  if (q) {
    *q = 2;
    printf("%d\n", *q);
    free(q);
  }
  return 0;
}

int main(void) {
  compute(1);
  compute(0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: fn compute(flag: i32) -> i32 {
// CHECK-NEXT:     let mut p: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     let mut q: *mut i32 = std::ptr::null_mut::<i32>();
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = (unsafe { malloc((std::mem::size_of::<i32>() as u64) as usize) as *mut () })
// CHECK-NEXT:             as *mut i32;
// CHECK-NEXT:         q = (unsafe { malloc((std::mem::size_of::<i32>() as u64) as usize) as *mut () })
// CHECK-NEXT:             as *mut i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if p == q {
// CHECK-NEXT:         println!("same");
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("diff");
// CHECK-NEXT:     }
// CHECK-NEXT:     if !p.is_null() {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *p = 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", unsafe { * p });
// CHECK-NEXT:         unsafe { free((p as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if !q.is_null() {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *q = 2;
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("{}", unsafe { * q });
// CHECK-NEXT:         unsafe { free((q as *mut ()) as *mut core::ffi::c_void) };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     compute(1i32);
// CHECK-NEXT:     compute(0i32);
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
