#include <limits.h>

extern void                   abort(void);
int __attribute__((noinline)) foo(unsigned int u) {
  return (int)(u + 4) < (int)u;
}

int main(int argc, char *argv[]) {
  unsigned int u = INT_MAX;

  if (foo(u) == 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut u: u32) -> i32 {
// CHECK-NEXT:     ((u.wrapping_add((4 as i32) as u32) as i32) < (u as i32)) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut u: u32 = (2147483647 as i32) as u32;
// CHECK-NEXT:     if foo(u) == (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
