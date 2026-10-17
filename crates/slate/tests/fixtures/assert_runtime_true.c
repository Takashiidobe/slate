#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv) {
  printf("before\n");
  assert(argc == 1);
  printf("after\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __assert_fail(
// CHECK-NEXT:         __assertion: *const i8,
// CHECK-NEXT:         __file: *const i8,
// CHECK-NEXT:         __line: u32,
// CHECK-NEXT:         __function: *const i8,
// CHECK-NEXT:     );
// CHECK-NEXT:     fn __assert_single_arg([[V_0:__v[0-9]+]]: bool) -> bool;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(argc: i32, argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     println!("before");
// CHECK-NEXT:     1 as u64;
// CHECK-NEXT:     if argc == 1 {
// CHECK-NEXT:         0 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __assert_fail(
// CHECK-NEXT:                 c"argc == 1".as_ptr(),
// CHECK-NEXT:                 c"{{.*}}tests/fixtures/assert_runtime_true.c"
// CHECK-NEXT:                     .as_ptr(),
// CHECK-NEXT:                 6 as u32,
// CHECK-NEXT:                 c"int main(int, char **)".as_ptr(),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("after");
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
