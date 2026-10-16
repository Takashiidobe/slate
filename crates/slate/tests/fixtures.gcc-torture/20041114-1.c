/* Verify that

   var <= 0 || ((long unsigned) (unsigned) (var - 1) < MAX_UNSIGNED_INT)

   gets folded to 1.  */

#include <limits.h>

void abort(void);
void link_failure(void);

volatile int v;

void foo(int var) {
  if (!(var <= 0 || ((long unsigned)(unsigned)(var - 1) < UINT_MAX)))
    link_failure();
}

int main(int argc, char **argv) {
  foo(v);
  return 0;
}

#ifndef __OPTIMIZE__
void link_failure(void) { abort(); }
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Verify that
// CHECK-NEXT: //!
// CHECK-NEXT: //! var <= 0 || ((long unsigned) (unsigned) (var - 1) < MAX_UNSIGNED_INT)
// CHECK-NEXT: //!
// CHECK-NEXT: //! gets folded to 1.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut var: i32) {
// CHECK-NEXT:     if !(var <= (0 as i32)
// CHECK-NEXT:         || (((var - (1 as i32)) as u32) as u64)
// CHECK-NEXT:             < (((2147483647 as i32) as u32)
// CHECK-NEXT:                 .wrapping_mul(2 as u32)
// CHECK-NEXT:                 .wrapping_add(1 as u32) as u64))
// CHECK-NEXT:     {
// CHECK-NEXT:         link_failure();
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     foo(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(v)) });
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn link_failure() {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
