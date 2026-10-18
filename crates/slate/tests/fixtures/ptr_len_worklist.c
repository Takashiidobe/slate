#include <stdio.h>
#include <stdlib.h>

static void fill_values(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] = i * 3;
}

static void forward_fill(int *values, int len) { fill_values(values, len); }

int main(void) {
  int  len    = 5;
  int *values = malloc(len * sizeof(int));
  forward_fill(values, len);
  printf("%d %d\n", values[1], values[4]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn malloc(__size: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: fn fill_values(values: *mut i32, len: i32) {
// CHECK-NEXT:     for i in 0..len {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *values.offset(i as isize) = i * 3;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn forward_fill(values: *mut i32, len: i32) {
// CHECK-NEXT:     fill_values(values, len);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let len: i32 = 5;
// CHECK-NEXT:     let values: *mut i32 = (unsafe { malloc(((len as i64) as u64).wrapping_mul(4)) })
// CHECK-NEXT:         as *mut i32;
// CHECK-NEXT:     forward_fill(values, len);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", unsafe { * values.offset(1 as isize) }, unsafe { * values.offset(4 as
// CHECK-NEXT:         isize) }
// CHECK-NEXT:     );
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
