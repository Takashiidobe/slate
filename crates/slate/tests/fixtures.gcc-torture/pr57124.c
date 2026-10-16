/* { dg-require-effective-target int32plus } */
/* { dg-options "-fno-strict-overflow" } */

extern void abort(void);
extern void exit(int);

__attribute__((noinline)) void foo(short unsigned int *p1,
                                   short unsigned int *p2) {
  short unsigned int x1, x4;
  int                x2, x3, x5, x6;
  unsigned int       x7;

  x1 = *p1;
  x2 = (int)x1;
  x3 = x2 * 65536;
  x4 = *p2;
  x5 = (int)x4;
  x6 = x3 + x4;
  x7 = (unsigned int)x6;
  if (x7 <= 268435455U)
    abort();
  exit(0);
}

int main() {
  short unsigned int x, y;
  x = -5;
  y = -10;
  foo(&x, &y);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-options "-fno-strict-overflow" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p1: &u16, mut p2: &u16) {
// CHECK-NEXT:     let mut x1: u16 = unsafe { *(p1 as *const u16) };
// CHECK-NEXT:     let mut x4: u16 = 0 as u16;
// CHECK-NEXT:     let mut x2: i32 = (x1 as u32) as i32;
// CHECK-NEXT:     let mut x3: i32 = x2.wrapping_mul(65536 as i32);
// CHECK-NEXT:     let mut x5: i32 = 0 as i32;
// CHECK-NEXT:     let mut x6: i32 = 0 as i32;
// CHECK-NEXT:     let mut x7: u32 = 0 as u32;
// CHECK-NEXT:     x4 = unsafe { *(p2 as *const u16) };
// CHECK-NEXT:     x5 = (x4 as u32) as i32;
// CHECK-NEXT:     x6 = x3.wrapping_add((x4 as u32) as i32);
// CHECK-NEXT:     x7 = x6 as u32;
// CHECK-NEXT:     if x7 <= (268435455 as u32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: u16 = ((5 as i32).wrapping_neg() as i16) as u16;
// CHECK-NEXT:     let mut y: u16 = ((10 as i32).wrapping_neg() as i16) as u16;
// CHECK-NEXT:     foo(unsafe { &(*std::ptr::addr_of_mut!(x)) }, unsafe {
// CHECK-NEXT:         &(*std::ptr::addr_of_mut!(y))
// CHECK-NEXT:     });
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
