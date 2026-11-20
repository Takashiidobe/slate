#include <stdio.h>

static int sum(int *values, int count) {
  int total = 0;
  for (int i = 0; i < count; i++)
    total += values[i];
  return total;
}

static void report(int code) { printf("code %d\n", code); }

int main(void) {
  int values[3] = {1, 2, 5};
  report(sum(values, 3) == 8 ? 0 : 1);
  int picked = sum(values, 3) > 2 ? sum(values, 3) : 0;
  printf("picked %d\n", picked);
  return sum(values, 3) == 8 ? 0 : 1;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn sum(values: &[i32]) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..values.len() as i32 {
// CHECK-NEXT:         total += unsafe { values[i as usize] };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn report(code: i32) {
// CHECK-NEXT:     println!("code {}", code);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let values: [i32; 3] = [1, 2, 5];
// CHECK-NEXT:     report(if sum(&values) == 8 { 0i32 } else { 1i32 });
// CHECK-NEXT:     let mut picked: i32 = 0;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = if sum(&values) > 2 {
// CHECK-NEXT:         sum(&values)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0
// CHECK-NEXT:     };
// CHECK-NEXT:     picked = [[V_0]];
// CHECK-NEXT:     println!("picked {}", picked);
// CHECK-NEXT:     if sum(&values) == 8 { 0 } else { 1 }
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
