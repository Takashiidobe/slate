#include <stdio.h>

struct Pair {
  int left;
  int right;
};

static int sum_pair(int a, int b) {
  struct Pair p;
  p.left  = a;
  p.right = b;
  return p.left + p.right;
}

static int overwrite_left(int a, int b) {
  struct Pair p;
  p.left  = a;
  p.right = b;
  p.left  = p.right + 2;
  return p.left;
}

int main(void) {
  printf("%d\n", sum_pair(4, 5));
  printf("%d\n", overwrite_left(3, 8));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_pair(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut p: Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p.left = a;
// CHECK-NEXT:     p.right = b;
// CHECK-NEXT:     p.left + p.right
// CHECK-NEXT: }
// CHECK-NEXT: fn overwrite_left(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut p: Pair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     p.left = a;
// CHECK-NEXT:     p.right = b;
// CHECK-NEXT:     p.left = p.right + 2;
// CHECK-NEXT:     p.left
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sum_pair(4 as i32, 5 as i32));
// CHECK-NEXT:     println!("{}", overwrite_left(3 as i32, 8 as i32));
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
