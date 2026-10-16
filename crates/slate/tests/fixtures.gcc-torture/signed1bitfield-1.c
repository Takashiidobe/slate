/* PR tree-optimization/115154 */
/* This was being miscompiled to `(signed:1)(t*5)`
   being transformed into `-((signed:1)t)` which is undefined.
   Note there is a pattern which removes the negative in some cases
   which works around the issue.  */

struct {
  signed b : 1;
} f;
int                            i = 55;
__attribute__((noinline)) void check(int a) {
  if (!a)
    __builtin_abort();
}
int main() {
  int t  = i != 5;
  t      = t * 5;
  f.b    = t;
  int tt = f.b;
  check(f.b);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/115154
// CHECK-NEXT: //!
// CHECK-NEXT: //! This was being miscompiled to `(signed:1)(t*5)`
// CHECK-NEXT: //! being transformed into `-((signed:1)t)` which is undefined.
// CHECK-NEXT: //! Note there is a pattern which removes the negative in some cases
// CHECK-NEXT: //! which works around the issue.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         u8,
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
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut i: i32 = 55 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn check(mut a: i32) {
// CHECK-NEXT:     if !(a != (0 as i32)) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut t: i32 = ((unsafe { i }) != (5 as i32)) as i32;
// CHECK-NEXT:     t *= 5 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(f.__slate_bits_0)).__set_b(t);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { (*std::ptr::addr_of!(f.__slate_bits_0)).__get_b() };
// CHECK-NEXT:     check(unsafe { (*std::ptr::addr_of!(f.__slate_bits_0)).__get_b() });
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
