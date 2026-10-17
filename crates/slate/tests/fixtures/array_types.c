#include <stdio.h>

static int sum_char_array(void) {
  char values[3];
  values[0] = 2;
  values[1] = 5;
  values[2] = values[0] + values[1];
  return values[2];
}

static double pick_double_array(int index) {
  double values[3];
  values[0] = 1.25;
  values[1] = 2.50;
  values[2] = values[0] + values[1];
  return values[index];
}

int main(void) {
  printf("%d\n", sum_char_array());
  printf("%.2f\n", pick_double_array(2));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sum_char_array() -> i32 {
// CHECK-NEXT:     let mut values: [i8; 3] = [0 as i8; 3];
// CHECK-NEXT:     values[0] = 2;
// CHECK-NEXT:     values[1] = 5;
// CHECK-NEXT:     values[2] = ((values[0] as i32) + (values[1] as i32)) as i8;
// CHECK-NEXT:     values[2] as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn pick_double_array(index: i32) -> f64 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[f64; 3]> = __SlateAlign16([0 as f64; 3]);
// CHECK-NEXT:     values.0[0] = 1.25f64;
// CHECK-NEXT:     values.0[1] = 2.5f64;
// CHECK-NEXT:     values.0[2] = values.0[0] + values.0[1];
// CHECK-NEXT:     values.0[index as usize]
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sum_char_array());
// CHECK-NEXT:     unsafe { printf(c"%.2f\n".as_ptr(), pick_double_array(2 as i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
