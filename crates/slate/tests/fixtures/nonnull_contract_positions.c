#include <stdio.h>

__attribute__((nonnull(1, 3))) static int combine(int *left, int scale,
                                                  int *right, int *optional) {
  return *left + scale * *right + (optional ? *optional : 0);
}

__attribute__((nonnull)) static int difference(int *left, int scale,
                                               int *right) {
  return *left - scale * *right;
}

int main(void) {
  int left  = 11;
  int right = 3;
  printf("%d %d\n", combine(&left, 2, &right, NULL),
         difference(&left, 2, &right));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn combine(left: &i32, scale: i32, right: &i32, optional: *mut i32) -> i32 {
// CHECK-NEXT:     *left + scale * *right + if !optional.is_null() { unsafe { *optional } } else { 0 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn difference(left: &i32, scale: i32, right: &i32) -> i32 {
// CHECK-NEXT:     *left - scale * *right
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let left: i32 = 11;
// CHECK-NEXT:     let right: i32 = 3;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}",
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]] = core::ptr::null_mut::<i32>();
// CHECK-NEXT:             combine(&left, 2i32, &right, [[T_0]])
// CHECK-NEXT:         },
// CHECK-NEXT:         difference(&left, 2i32, &right)
// CHECK-NEXT:     );
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
