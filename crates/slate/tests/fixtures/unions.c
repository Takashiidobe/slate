#include <stdio.h>

union Pair {
  int left;
  int right;
};

static int pick_left(int value) {
  union Pair p;
  p.left = value;
  return p.left;
}

static int overwrite(int a, int b) {
  union Pair p;
  p.left  = a;
  p.right = b;
  return p.left;
}

int main(void) {
  printf("%d\n", pick_left(7));
  printf("%d\n", overwrite(3, 9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn pick_left(value: i32) -> i32 {
// CHECK-NEXT:     let mut p: Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p.left = value;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { p.left }
// CHECK-NEXT: }
// CHECK-NEXT: fn overwrite(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut p: Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p.left = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p.right = b;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { p.left }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", pick_left(7i32));
// CHECK-NEXT:     println!("{}", overwrite(3i32, 9i32));
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
