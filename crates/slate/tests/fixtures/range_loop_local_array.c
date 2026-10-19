#include <stdio.h>

static int sum_fixed(void) {
  int a[5]  = {1, 2, 3, 4, 5};
  int total = 0;
  for (int i = 0; i < 5; i++) {
    total += a[i];
  }
  return total;
}

int main(void) {
  printf("%d\n", sum_fixed());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn sum_fixed() -> i32 {
// CHECK-NEXT:     let a: __SlateAlign16<[i32; 5]> = __SlateAlign16([1i32, 2i32, 3i32, 4i32, 5i32]);
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in a.0.iter().copied() {
// CHECK-NEXT:         total += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", sum_fixed());
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
