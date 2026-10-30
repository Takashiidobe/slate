#include <stdio.h>

__attribute__((nonnull)) int use_all(int *a, int *b) { return *a - *b; }

int main(void) {
  int x = 10;
  int y = 4;
  printf("%d\n", use_all(&x, &y));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn use_all(a: &i32, b: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(a as *const i32) }) - unsafe { *(b as *const i32) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 10;
// CHECK-NEXT:     let mut y: i32 = 4;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", use_all(unsafe { & (* std::ptr::addr_of_mut!(x)) }, unsafe { & (*
// CHECK-NEXT:         std::ptr::addr_of_mut!(y)) })
// CHECK-NEXT:     );
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
