#include <stdio.h>

struct pair { int a; int b; };

static int sum(int *values, int count) {
  int total = 0;
  for (int i = 0; i < count; i++)
    total += values[i];
  return total;
}

int main(void) {
  int values[3] = {1, 2, 5};
  struct pair p = {sum(values, 3), sum(values, 3)};
  int arr[2] = {sum(values, 3), sum(values, 3)};
  int z = sum(values, 3);
  int n = 0;
  while (sum(values, 3) > n * 4)
    n++;
  int w = n > 1 ? sum(values, 3) : -sum(values, 3);
  printf("%d %d %d %d %d %d %d\n", z, p.a, p.b, arr[0], arr[1], n, w);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct pair {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..values.len() as i32 {
// CHECK-NEXT:         total += unsafe { values[i as usize] };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let values: [i32; 3] = [1, 2, 5];
// CHECK-NEXT:     let p: pair = pair {
// CHECK-NEXT:         a: sum(&values),
// CHECK-NEXT:         b: sum(&values),
// CHECK-NEXT:     };
// CHECK-NEXT:     let arr: [i32; 2] = [sum(&values), sum(&values)];
// CHECK-NEXT:     let z: i32 = sum(&values);
// CHECK-NEXT:     let mut n: i32 = 0;
// CHECK-NEXT:     while sum(&values) > n * 4 {
// CHECK-NEXT:         n += 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut w: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = if n > 1 {
// CHECK-NEXT:         sum(&values)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         -sum(&values)
// CHECK-NEXT:     };
// CHECK-NEXT:     w = [[V_0]];
// CHECK-NEXT:     println!("{} {} {} {} {} {} {}", z, p.a, p.b, arr[0], arr[1], n, w);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
