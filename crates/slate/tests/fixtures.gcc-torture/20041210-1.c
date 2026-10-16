/* The FR-V port used to fail this test because the andcc patterns
   wrongly claimed to set the C and V flags.  */
#include <limits.h>

void abort(void);
void exit(int);

int x[4] = {INT_MIN / 2, INT_MAX, 2, 4};

int main() {
  if (x[0] < x[1])
    if ((x[2] & x[3]) < 0)
      abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! The FR-V port used to fail this test because the andcc patterns
// CHECK-NEXT: //! wrongly claimed to set the C and V flags.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:     (-(2147483647 as i32) - (1 as i32)) / (2 as i32),
// CHECK-NEXT:     2147483647 as i32,
// CHECK-NEXT:     2 as i32,
// CHECK-NEXT:     4 as i32,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(x.0) as *mut i32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) < unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(x.0) as *mut i32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:     } {
// CHECK-NEXT:         if (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(x.0) as *mut i32 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) & unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(x.0) as *mut i32 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         } < (0 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
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
