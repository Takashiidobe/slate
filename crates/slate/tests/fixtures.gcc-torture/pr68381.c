/* { dg-options "-O -fexpensive-optimizations -fno-tree-bit-ccp" } */

__attribute__((noinline, noclone)) int foo(unsigned short x, unsigned short y) {
  int r;
  if (__builtin_mul_overflow(x, y, &r))
    __builtin_abort();
  return r;
}

int main(void) {
  int x = 1;
  int y = 2;
  if (foo(x, y) != x * y)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-O -fexpensive-optimizations -fno-tree-bit-ccp" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: u16, mut y: u16) -> i32 {
// CHECK-NEXT:     let mut r: i32 = 0 as i32;
// CHECK-NEXT:     if {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: i128 = x as i128;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: i128 = y as i128;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: i128 = [[T_0]].wrapping_mul([[T_1]]);
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_2]] as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].checked_mul([[T_1]]).is_none() || (([[T_2]] as i32) as i128) != [[T_2]]
// CHECK-NEXT:     } {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 1 as i32;
// CHECK-NEXT:     let mut y: i32 = 2 as i32;
// CHECK-NEXT:     if foo((x as i16) as u16, (y as i16) as u16) != x * y {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
