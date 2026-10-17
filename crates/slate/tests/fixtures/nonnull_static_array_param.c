#include <stdio.h>

int sum4(int arr[static 4]) {
  int s = 0;
  for (int i = 0; i < 4; i++) {
    s += arr[i];
  }
  return s;
}

int main(void) {
  int values[4] = {1, 2, 3, 4};
  printf("%d\n", sum4(values));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sum4(arr: *mut i32) -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..4 as i32 {
// CHECK-NEXT:         s += unsafe { *unsafe { arr.offset(i as isize) } };
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let __slate_printf_arg0 = sum4(values.0.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
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
