/* { dg-require-effective-target int32plus } */

/* PR tree-optimization/78170.
   Check that sign-extended store to a bitfield
   doesn't overwrite other fields.  */

int a, b, d;

struct S0 {
  int f0;
  int f1;
  int f2;
  int f3;
  int f4;
  int f5 : 15;
  int f6 : 17;
  int f7 : 2;
  int f8 : 30;
} c;

void fn1() {
  d = b = 1;
  for (; b; b = a) {
    struct S0 e = {0, 0, 0, 0, 0, 0, 1, 0, 1};
    c           = e;
    c.f6        = -1;
  }
}

int main() {
  fn1();
  if (c.f7 != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // PR tree-optimization/78170.
// CHECK-NEXT: // Check that sign-extended store to a bitfield
// CHECK-NEXT: // doesn't overwrite other fields.
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 8], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(15)]
// CHECK-NEXT:         pub f5: i32,
// CHECK-NEXT:         #[bits(17)]
// CHECK-NEXT:         pub f6: i32,
// CHECK-NEXT:         #[bits(2)]
// CHECK-NEXT:         pub f7: i32,
// CHECK-NEXT:         #[bits(30)]
// CHECK-NEXT:         pub f8: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S0 {
// CHECK-NEXT:     f0: i32,
// CHECK-NEXT:     f1: i32,
// CHECK-NEXT:     f2: i32,
// CHECK-NEXT:     f3: i32,
// CHECK-NEXT:     f4: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: S0 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn fn1() {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         d = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { b }) != (0 as i32) {
// CHECK-NEXT:         let mut e: S0 = {
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: S0 = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]].f0 = 0 as i32;
// CHECK-NEXT:             [[T_0]].f1 = 0 as i32;
// CHECK-NEXT:             [[T_0]].f2 = 0 as i32;
// CHECK-NEXT:             [[T_0]].f3 = 0 as i32;
// CHECK-NEXT:             [[T_0]].f4 = 0 as i32;
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_f5(0 as i32);
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_f6(1 as i32);
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_f7(0 as i32);
// CHECK-NEXT:             [[T_0]].__slate_bits_0.__set_f8(1 as i32);
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             c = e;
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*std::ptr::addr_of_mut!(c.__slate_bits_0)).__set_f6(-(1 as i32));
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             b = unsafe { a };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     fn1();
// CHECK-NEXT:     if (unsafe { (*std::ptr::addr_of!(c.__slate_bits_0)).__get_f7() }) != (0 as i32) {
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
