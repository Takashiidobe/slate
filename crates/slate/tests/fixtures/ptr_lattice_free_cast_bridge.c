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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: use slate_support::malloc;
// CHECK-NEXT: use slate_support::free;
// CHECK-NEXT: fn just_free(p: Box<i32>) {}
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: *mut i32 = (unsafe {
// CHECK-NEXT:         malloc((std::mem::size_of::<i32>() as u64) as usize) as *mut ()
// CHECK-NEXT:     }) as *mut i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *x = 4;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{}", unsafe { * x });
// CHECK-NEXT:     just_free(unsafe { Box::from_raw(x) });
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
