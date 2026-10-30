#include <stdio.h>

static int is_negative(char c) { return c < 0; }

int main(void) {
  char c = (char)200;
  printf("%d %d\n", is_negative(c), (int)c);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const u8, ...) -> i32;
// CHECK-NEXT: }
// X86_64-UNKNOWN-LINUX-GNU-NEXT: fn is_negative(c: i8) -> i32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     ((c as i32) < 0) as i32
// AARCH64-UNKNOWN-LINUX-GNU-NEXT: fn is_negative(c: u8) -> i32 {
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     (((c as u32) as i32) < 0) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     let c: i8 = -56i8;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     println!("{} {}", is_negative(c), c as i32);
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     let c: u8 = 200u8;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     println!("{} {}", is_negative(c), (c as u32) as i32);
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
