
#include <stdio.h>

static int cached_value = 0;
static int computed     = 0;

static int compute(void) { return 42; }

int get_value(void) {
  if (!computed) {
    cached_value = compute();
    computed     = 1;
  }
  return cached_value;
}

int main(void) {
  printf("%d\n", get_value());
  printf("%d\n", get_value());
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut cached_value: i32 = 0 as i32;
// CHECK-NEXT: static mut computed: i32 = 0 as i32;
// CHECK-NEXT: fn compute() -> i32 {
// CHECK-NEXT:     42
// CHECK-NEXT: }
// CHECK-NEXT: fn get_value() -> i32 {
// CHECK-NEXT:     if !((unsafe { computed }) != 0) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             cached_value = compute();
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             computed = 1;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { cached_value }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", get_value());
// CHECK-NEXT:     println!("{}", get_value());
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
