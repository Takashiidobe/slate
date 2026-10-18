#include <stdio.h>

static int weighted_sum(int *items, int len) {
  int total = 0;
  for (int i = 0; i < len; i++) {
    int item  = items[i];
    total    += item * i;
  }
  return total;
}

int main(void) {
  int values[4] = {2, 4, 6, 8};
  printf("%d\n", weighted_sum(values, 4));
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
// CHECK-NEXT: fn weighted_sum(items: *mut i32, len: i32) -> i32 {
// CHECK-NEXT:     let mut total: i32 = 0;
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         let item: i32 = unsafe { *items.offset(i as isize) };
// CHECK-NEXT:         total += item * i;
// CHECK-NEXT:     }
// CHECK-NEXT:     total
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut values: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let __slate_printf_arg0 = weighted_sum(values.0.as_mut_ptr() as *mut i32, 4 as i32);
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
