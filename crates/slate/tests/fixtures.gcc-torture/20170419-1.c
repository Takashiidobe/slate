/* PR tree-optimization/80426 */
/* Testcase by <ishiura-compiler@ml.kwansei.ac.jp> */

#define INT_MAX 0x7fffffff
#define INT_MIN (-INT_MAX - 1)

int x;

int main(void) {
  volatile int a = 0;
  volatile int b = -INT_MAX;
  int          j;

  for (j = 0; j < 18; j += 1) {
    x = ((a == 0) != (b - (int)(INT_MIN)));
  }

  if (x != 0)
    __builtin_abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/80426
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by <ishiura-compiler@ml.kwansei.ac.jp>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i32 = 0 as i32;
// CHECK-NEXT:     let mut b: i32 = -(2147483647 as i32);
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (18 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             x = ((((unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) }) == (0 as i32))
// CHECK-NEXT:                 as i32)
// CHECK-NEXT:                 != (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) })
// CHECK-NEXT:                     - (-(2147483647 as i32) - (1 as i32))) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         j = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { x }) != (0 as i32) {
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
