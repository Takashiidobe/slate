/* Signed left-shift is implementation-defined in C89 (and see
   DR#081), not undefined.  Bug 7284 from Al Grant (AlGrant at
   myrealbox.com).  */

/* { dg-require-effective-target int32plus } */
/* { dg-options "-std=c89" } */

extern void abort(void);
extern void exit(int);

int f(int n) { return (n << 24) / (1 << 23); }

volatile int x = 128;

int main(void) {
  if (f(x) != -256)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Signed left-shift is implementation-defined in C89 (and see
// CHECK-NEXT: //! DR#081), not undefined.  Bug 7284 from Al Grant (AlGrant at
// CHECK-NEXT: //! myrealbox.com).
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target int32plus }
// CHECK-NEXT: //
// CHECK-NEXT: // { dg-options "-std=c89" }
// CHECK-NEXT: fn f(mut n: i32) -> i32 {
// CHECK-NEXT:     (n << (24 as i32)) / ((1 as i32) << (23 as i32))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = 128 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if f(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) }) != -(256 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
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
