#include <stdio.h>

int main(void) {
  int a[5] = {1, 2, 3, 4, 5};
  __builtin_bzero(a, sizeof(a));
  int sum = 0;
  for (int i = 0; i < 5; i++)
    sum += a[i];
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     #[link_name = "bzero"]
// CHECK-NEXT:     fn __builtin_bzero([[V_0:__v[0-9]+]]: *mut (), [[V_1:__v[0-9]+]]: u64);
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 5]> = __SlateAlign16([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     unsafe { __builtin_bzero(a.0.as_mut_ptr() as *mut (), 20 as u64) };
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..5 as i32 {
// CHECK-NEXT:         sum += a.0[i as usize];
// CHECK-NEXT:     }
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
