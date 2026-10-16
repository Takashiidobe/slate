/* Tests that the may_alias attribute works as expected.
   Author: Osku Salerma <osku@iki.fi> Apr 2002.  */

extern void abort(void);
extern void exit(int);

typedef short __attribute__((__may_alias__)) short_a;

int main(void) {
  int      a = 0x12345678;
  short_a *b = (short_a *)&a;

  b[1] = 0;

  if (a == 0x12345678)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Tests that the may_alias attribute works as expected.
// CHECK-NEXT: //! Author: Osku Salerma <osku@iki.fi> Apr 2002.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 305419896 as i32;
// CHECK-NEXT:     let mut b: *mut i16 = std::ptr::addr_of_mut!(a) as *mut i16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe { b.offset((1 as i32) as isize) } = (0 as i32) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     if a == (305419896 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
