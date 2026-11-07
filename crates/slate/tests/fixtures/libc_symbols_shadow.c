// dg-options: -fno-builtin
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#undef O_RDONLY
#define O_RDONLY 123

int abs(int value) {
  return value + 7;
}

extern int parent_id(void) __asm__("getppid");

int main(void) {
  int (*function)(int) = abs;
  printf("%d %d %d\n", function(3), O_RDONLY, parent_id() > 0);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! dg-options: -fno-builtin
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "getppid"]
// CHECK-NEXT:     fn parent_id() -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: extern "C-unwind" fn abs(value: i32) -> i32 {
// CHECK-NEXT:     value + 7
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let function: Option<unsafe extern "C-unwind" fn(i32) -> i32> = Some(abs);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {}",
// CHECK-NEXT:         unsafe { function.unwrap()(3i32) },
// CHECK-NEXT:         123i32,
// CHECK-NEXT:         ((unsafe { parent_id() }) > 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
