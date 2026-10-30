#include <stdio.h>

__attribute__((cold)) static int cold_path(int x) { return x * 2; }

__attribute__((always_inline)) static int always_inlined(int x) {
  return x + 1;
}

__attribute__((noinline)) static int never_inlined(int x) { return x - 1; }

int main(void) {
  int a = cold_path(3);
  int b = always_inlined(a);
  int c = never_inlined(b);
  printf("%d %d %d\n", a, b, c);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn cold_path(x: i32) -> i32 {
// CHECK-NEXT:     x * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn always_inlined(x: i32) -> i32 {
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn never_inlined(x: i32) -> i32 {
// CHECK-NEXT:     x - 1
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i32 = cold_path(3i32);
// CHECK-NEXT:     let b: i32 = always_inlined(a);
// CHECK-NEXT:     let c: i32 = never_inlined(b);
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
