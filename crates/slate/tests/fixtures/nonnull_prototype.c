#include <stdio.h>

int use_it(int *a, int *b) __attribute__((nonnull(1, 2)));

int main(void) {
  int x = 6;
  int y = 2;
  printf("%d\n", use_it(&x, &y));
  return 0;
}

int use_it(int *a, int *b) { return *a / *b; }

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 6;
// CHECK-NEXT:     let mut y: i32 = 2;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", use_it(unsafe { & (* std::ptr::addr_of_mut!(x)) }, unsafe { & (*
// CHECK-NEXT:         std::ptr::addr_of_mut!(y)) })
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: fn use_it(a: &i32, b: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(a as *const i32) }) / unsafe { *(b as *const i32) }
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
