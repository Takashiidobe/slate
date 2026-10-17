#include <stdio.h>

__attribute__((deprecated)) static int old_api(int x) { return x + 1; }

__attribute__((deprecated("use new_api instead"))) static int
old_api_msg(int x) {
  return x + 2;
}

__attribute__((warn_unused_result)) static int must_check(int x) {
  return x + 3;
}

int main(void) {
  int a = old_api(1);
  int b = old_api_msg(2);
  int c = must_check(3);
  printf("%d %d %d\n", a, b, c);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn old_api(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn old_api_msg(x: i32) -> i32 {
// CHECK-NEXT:     x + 2
// CHECK-NEXT: }
// CHECK-NEXT: fn must_check(x: i32) -> i32 {
// CHECK-NEXT:     x + 3
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i32 = old_api(1 as i32);
// CHECK-NEXT:     let b: i32 = old_api_msg(2 as i32);
// CHECK-NEXT:     let c: i32 = must_check(3 as i32);
// CHECK-NEXT:     println!("{} {} {}", a, b, c);
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
