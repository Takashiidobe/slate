#include <stdio.h>

static int sum_vla(int length, int (*values)[length]) {
  int total = 0;
  for (int index = 0; index < length; ++index) {
    total += (*values)[index];
  }
  return total;
}

int main(void) {
  int result;
  {
    int length = 4;
    int values[length];
    for (int index = 0; index < length; ++index) {
      values[index] = index + 3;
    }
    result = sum_vla(length, &values);
  }
  printf("%d\n", result + 1);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_vla(length: i32, values: *mut [i32; 0]) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for index in 0..length {
// CHECK-NEXT:         total += unsafe { *(values as *mut i32).offset(index as isize) };
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut result: i32 = 0;
// CHECK-NEXT:     let length: i32 = 4;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: u64 = (length as i64) as u64;
// CHECK-NEXT:     let mut values: Vec<i32> = {
// CHECK-NEXT:         let mut __slate_vla: Vec<i32> = Vec::with_capacity([[V_0]] as usize);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_vla.as_mut_ptr().write_bytes(0u8, [[V_0]] as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         __slate_vla
// CHECK-NEXT:     };
// CHECK-NEXT:     for index in 0..length {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(values.as_mut_ptr() as *mut i32).offset(index as isize) = index + 3;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     result = sum_vla(length, values.as_mut_ptr() as *mut [i32; 0]);
// CHECK-NEXT:     println!("{}", result + 1);
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
