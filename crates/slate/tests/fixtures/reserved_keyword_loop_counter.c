#include <stdio.h>

int main(void) {
  int try, sum = 0;
  for (try = 0; try < 5; try++)
    sum += try;
  for (int yield = 3; yield < 9; yield += 2)
    sum += yield;
  for (unsigned box = 0; box < 4; box++)
    sum += box * 2;
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for r#try in 0i32..5i32 {
// CHECK-NEXT:         sum += r#try;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut r#yield: i32 = 3;
// CHECK-NEXT:     while r#yield < 9 {
// CHECK-NEXT:         sum += r#yield;
// CHECK-NEXT:         r#yield += 2;
// CHECK-NEXT:     }
// CHECK-NEXT:     for r#box in 0u32..4u32 {
// CHECK-NEXT:         sum = (sum as u32).wrapping_add(r#box.wrapping_mul(2)) as i32;
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
