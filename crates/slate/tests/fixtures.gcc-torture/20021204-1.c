/* This test was miscompiled when using sibling call optimization,
   because X ? Y : Y - 1 optimization changed X into !X in place
   and haven't reverted it if do_store_flag was successful, so
   when expanding the expression the second time it was
   !X ? Y : Y - 1.  */

extern void abort(void);
extern void exit(int);

void foo(int x) {
  if (x != 1)
    abort();
}

int z;

int main(int argc, char **argv) {
  char *a = "test";
  char *b = a + 2;

  foo(z > 0 ? b - a : b - a - 1);
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This test was miscompiled when using sibling call optimization,
// CHECK-NEXT: //! because X ? Y : Y - 1 optimization changed X into !X in place
// CHECK-NEXT: //! and haven't reverted it if do_store_flag was successful, so
// CHECK-NEXT: //! when expanding the expression the second time it was
// CHECK-NEXT: //! !X ? Y : Y - 1.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32) {
// CHECK-NEXT:     if x != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut z: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main(mut argc: i32, mut argv: *mut *mut i8) -> i32 {
// CHECK-NEXT:     let mut a: *mut i8 = c"test".as_ptr() as *mut i8;
// CHECK-NEXT:     let mut b: *mut i8 = unsafe { a.offset((2 as i32) as isize) };
// CHECK-NEXT:     foo((if (unsafe { z }) > (0 as i32) {
// CHECK-NEXT:         (unsafe { b.offset_from(a as *mut i8) }) as i64
// CHECK-NEXT:     } else {
// CHECK-NEXT:         ((unsafe { b.offset_from(a as *mut i8) }) as i64) - ((1 as i32) as i64)
// CHECK-NEXT:     }) as i32);
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
// CHECK-NEXT:     unsafe { __slate_main(argc as _, argv as _) }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
