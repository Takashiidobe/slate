#include <stdio.h>
#include <stdlib.h>

static int consume_values(int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    values[i] += 1;
    sum       += values[i];
  }
  free(values);
  return sum;
}

static int forward_consume(int *values, int len) {
  return consume_values(values, len);
}

int main(void) {
  int  len    = 4;
  int *values = malloc(len * sizeof(int));
  for (int i = 0; i < len; ++i)
    values[i] = i * 2;
  int sum = forward_consume(values, len);
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT:     fn free(__ptr: *mut ());
// CHECK-NEXT: }
// CHECK-NEXT: fn consume_values(values: *mut i32, len: i32) -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         let [[V_0:__v[0-9]+]]: *mut i32 = unsafe { values.offset(i as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *[[V_0]] = (unsafe { *[[V_0]] }) + 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         sum += unsafe { *unsafe { values.offset(i as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { free(values as *mut ()) };
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: fn forward_consume(values: *mut i32, len: i32) -> i32 {
// CHECK-NEXT:     consume_values(values, len)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let len: i32 = 4;
// CHECK-NEXT:     let values: *mut i32 = (unsafe { malloc(((len as i64) as u64).wrapping_mul(4)) })
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { values.offset(i as isize) } = i * 2;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let sum: i32 = forward_consume(values, len);
// CHECK-NEXT:     println!("{}", sum);
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
