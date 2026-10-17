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

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn combine(left: &i32, scale: i32, right: &i32, optional: *mut i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(left as *const i32) }) + scale * unsafe { *(right as *const i32) }
// CHECK-NEXT:         + if optional != std::ptr::null_mut::<i32>() { unsafe { *optional } } else { 0 }
// CHECK-NEXT: }
// CHECK-NEXT: fn difference(left: &i32, scale: i32, right: &i32) -> i32 {
// CHECK-NEXT:     (unsafe { *(left as *const i32) }) - scale * unsafe { *(right as *const i32) }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut left: i32 = 11;
// CHECK-NEXT:     let mut right: i32 = 3;
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", combine(unsafe { & (* std::ptr::addr_of_mut!(left)) }, 2 as i32, unsafe
// CHECK-NEXT:         { & (* std::ptr::addr_of_mut!(right)) }, std::ptr::null_mut::< i32 > ()),
// CHECK-NEXT:         difference(unsafe { & (* std::ptr::addr_of_mut!(left)) }, 2 as i32, unsafe { & (*
// CHECK-NEXT:         std::ptr::addr_of_mut!(right)) })
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
