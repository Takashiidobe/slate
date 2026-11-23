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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::VecExt as _;
// CHECK-EMPTY:
// CHECK-NEXT: fn compute(flag: i32) -> i32 {
// CHECK-NEXT:     let mut p: Option<Vec<i32>> = None;
// CHECK-NEXT:     if flag != 0 {
// CHECK-NEXT:         p = Vec::try_filled(4, 0);
// CHECK-NEXT:     }
// CHECK-NEXT:     if let Some(p) = p.as_mut() {
// CHECK-NEXT:         p[1] = 41;
// CHECK-NEXT:         println!("{}", p[1]);
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("no alloc");
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     compute(1i32);
// CHECK-NEXT:     compute(0i32);
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
