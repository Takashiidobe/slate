/* PR tree-optimization/102622 */
/* Wrong code introduced due to phi-opt
   introducing undefined signed interger overflow
   with one bit signed integer negation. */

struct f {
  signed t : 1;
};
int g(struct f *a, int t) __attribute__((noipa));
int g(struct f *a, int t) {
  if (t)
    a->t = -1;
  else
    a->t = 0;
  int t1 = a->t;
  if (t1)
    return 1;
  return t1;
}

int main(void) {
  struct f a;
  if (!g(&a, 1))
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/102622
// CHECK-NEXT: //!
// CHECK-NEXT: //! Wrong code introduced due to phi-opt
// CHECK-NEXT: //! introducing undefined signed interger overflow
// CHECK-NEXT: //! with one bit signed integer negation.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct f {
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
// CHECK-NEXT:         pub t: i32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn g(mut a: &mut f, mut t: i32) -> i32 {
// CHECK-NEXT:     if t != (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(a as *mut f)).__slate_bits_0.__set_t(-(1 as i32));
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (*(a as *mut f)).__slate_bits_0.__set_t(0 as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut t1: i32 = unsafe { (*(a as *mut f)).__slate_bits_0.__get_t() };
// CHECK-NEXT:     if t1 != (0 as i32) {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     t1
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: f = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     if !(g(unsafe { &mut (*std::ptr::addr_of_mut!(a)) }, 1 as i32) != (0 as i32)) {
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
