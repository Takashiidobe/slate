/* PR middle-end/38422 */

extern void abort(void);

struct S {
  int s : (sizeof(int) * __CHAR_BIT__ - 2);
} s;

void foo(void) { s.s *= 2; }

int main() {
  s.s = 24;
  foo();
  if (s.s != 48)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/38422
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct S {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u32,
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(30)]
// CHECK-NEXT:         pub s: i32,
// CHECK-NEXT:         #[bits(2, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut s: S = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32 = unsafe { (*std::ptr::addr_of!(s.__slate_bits_0)).__get_s() };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] * (2 as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(s.__slate_bits_0)).__set_s([[V_1]]);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(s.__slate_bits_0)).__set_s(24 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if (unsafe { (*std::ptr::addr_of!(s.__slate_bits_0)).__get_s() }) != (48 as i32) {
// CHECK-NEXT:         unsafe { abort() };
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
