#include <stdio.h>

static int sum_array(void) {
  int values[3];
  values[0] = 4;
  values[1] = 5;
  values[2] = values[0] + values[1];
  return values[2];
}

static int pick_index(int index) {
  int values[3];
  values[0] = 10;
  values[1] = 20;
  values[2] = 30;
  return values[index];
}

int main(void) {
  printf("%d\n", sum_array());
  printf("%d\n", pick_index(1));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn sum_array() -> i32 {
// CHECK-NEXT:     let mut values: [i32; 3] = [0i32; 3];
// CHECK-NEXT:     values[0] = 4;
// CHECK-NEXT:     values[1] = 5;
// CHECK-NEXT:     values[2] = values[0] + values[1];
// CHECK-NEXT:     values[2]
// CHECK-NEXT: }
// CHECK-NEXT: fn pick_index(index: i32) -> i32 {
// CHECK-NEXT:     let mut values: [i32; 3] = [0i32; 3];
// CHECK-NEXT:     values[0] = 10;
// CHECK-NEXT:     values[1] = 20;
// CHECK-NEXT:     values[2] = 30;
// CHECK-NEXT:     values[index as usize]
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sum_array());
// CHECK-NEXT:     println!("{}", pick_index(1i32));
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
