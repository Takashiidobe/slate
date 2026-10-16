#include <limits.h>

void abort(void);
void exit(int);

int main(void) {
  void *x = ((void *)((unsigned int)INT_MAX + 2));
  void *y = ((void *)((unsigned long)LONG_MAX + 2));
  if (x >= ((void *)((unsigned int)INT_MAX + 1)) &&
      x <= ((void *)((unsigned int)INT_MAX + 6)) &&
      y >= ((void *)((unsigned long)LONG_MAX + 1)) &&
      y <= ((void *)((unsigned long)LONG_MAX + 6)))
    exit(0);
  else
    abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: *mut () = ((2147483647 as i32) as u32).wrapping_add((2 as i32) as u32)
// CHECK-NEXT:         as *mut ();
// CHECK-NEXT:     let mut y: *mut () = ((9223372036854775807 as i64) as u64)
// CHECK-NEXT:         .wrapping_add(((2 as i32) as i64) as u64) as *mut ();
// CHECK-NEXT:     if x >= (((2147483647 as i32) as u32).wrapping_add((1 as i32) as u32) as *mut ())
// CHECK-NEXT:         && x <= (((2147483647 as i32) as u32).wrapping_add((6 as i32) as u32) as *mut ())
// CHECK-NEXT:         && y
// CHECK-NEXT:             >= (((9223372036854775807 as i64) as u64)
// CHECK-NEXT:                 .wrapping_add(((1 as i32) as i64) as u64) as *mut ())
// CHECK-NEXT:         && y
// CHECK-NEXT:             <= (((9223372036854775807 as i64) as u64)
// CHECK-NEXT:                 .wrapping_add(((6 as i32) as i64) as u64) as *mut ())
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
// SLATE-FILECHECK-END
