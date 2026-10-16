/* Ensure that we create VUSE operands also for noreturn functions.  */

#include <stdlib.h>
#include <string.h>

int *pwarn;

void bla(void) __attribute__((noreturn));

void bla(void) {
  if (!*pwarn)
    abort();

  exit(0);
}

int main(void) {
  int warn;

  memset(&warn, 0, sizeof(warn));

  pwarn = &warn;

  warn = 1;

  bla();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Ensure that we create VUSE operands also for noreturn functions.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit(__status: i32);
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut pwarn: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn bla() {
// CHECK-NEXT:     if !((unsafe { *unsafe { pwarn } }) != (0 as i32)) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut warn: i32 = 0 as i32;
// CHECK-NEXT:     unsafe { memset(std::ptr::addr_of_mut!(warn) as *mut (), 0 as i32, 4 as u64) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         pwarn = std::ptr::addr_of_mut!(warn);
// CHECK-NEXT:     }
// CHECK-NEXT:     warn = 1 as i32;
// CHECK-NEXT:     bla();
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
