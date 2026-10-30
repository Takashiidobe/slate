#include <stdio.h>
enum FuzzEnum {
  FuzzZero,
  FuzzOne,
  FuzzFive = 5,
  FuzzSix,
  FuzzNegative = -2,
  FuzzNegativeNext
};
union FuzzPair {
  int left;
  int right;
};
static int helper(int a, int b) {
  int c = a + b;
  return c;
}
static int loop_sum(int n) {
  int total = 0;
  for (int i = 1; i <= n; i++) {
    total += i;
  }
  return total;
}
static int array_pick(int index) {
  int values[3];
  values[0] = 6;
  values[1] = 1;
  values[2] = values[0] + values[1];
  return values[index];
}
static int union_pick(int a, int b) {
  union FuzzPair p;
  p.left  = a;
  p.right = b;
  return p.left;
}
int main(void) {
  printf("%d\n", helper(7, 2));
  printf("%d\n", loop_sum(2));
  printf("%d\n", array_pick(1));
  printf("%d\n", union_pick(8, 9));
  printf("%d\n", FuzzSix);
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
// CHECK-NEXT: union FuzzPair {
// CHECK-NEXT:     left: i32,
// CHECK-NEXT:     right: i32,
// CHECK-NEXT: }
// CHECK-NEXT: fn helper(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let c: i32 = a + b;
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-NEXT: fn loop_sum(n: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     let mut i: i32 = 1;
// CHECK-NEXT:     while i <= n {
// CHECK-NEXT:         total += i;
// CHECK-NEXT:         i += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn array_pick(index: i32) -> i32 {
// CHECK-NEXT:     let mut values: [i32; 3] = [0i32; 3];
// CHECK-NEXT:     values[0] = 6;
// CHECK-NEXT:     values[1] = 1;
// CHECK-NEXT:     values[2] = values[0] + values[1];
// CHECK-NEXT:     values[index as usize]
// CHECK-NEXT: }
// CHECK-NEXT: fn union_pick(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     let mut p: FuzzPair = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p.left = a;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p.right = b;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { p.left }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", helper(7i32, 2i32));
// CHECK-NEXT:     println!("{}", loop_sum(2i32));
// CHECK-NEXT:     println!("{}", array_pick(1i32));
// CHECK-NEXT:     println!("{}", union_pick(8i32, 9i32));
// CHECK-NEXT:     println!("{}", 6i32);
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
