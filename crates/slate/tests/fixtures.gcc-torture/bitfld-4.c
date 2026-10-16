/* When comparisons of bit-fields to unsigned constants got shortened,
   the shortened signed constant was wrongly marked as overflowing,
   leading to a later integer_zerop failure and misoptimization.

   Related to bug tree-optimization/16437 but shows the problem on
   32-bit systems.  */
/* Origin: Joseph Myers <jsm@polyomino.org.uk> */

/* { dg-require-effective-target int32plus } */

extern void abort(void);

struct s {
  int a : 12, b : 20;
};

struct s x = {-123, -456};

int main(void) {
  if (x.a != -123U || x.b != -456U)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! When comparisons of bit-fields to unsigned constants got shortened,
// CHECK-NEXT: //! the shortened signed constant was wrongly marked as overflowing,
// CHECK-NEXT: //! leading to a later integer_zerop failure and misoptimization.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Related to bug tree-optimization/16437 but shows the problem on
// CHECK-NEXT: //! 32-bit systems.
// CHECK-NEXT: //!
// CHECK-NEXT: //! Origin: Joseph Myers <jsm@polyomino.org.uk>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-require-effective-target int32plus }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct s {
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
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub a: i32,
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub b: i32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: s = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: s = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_a(-(123 as i32));
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_b(-(456 as i32));
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((unsafe { (*std::ptr::addr_of!(x.__slate_bits_0)).__get_a() }) as u32)
// CHECK-NEXT:         != (123 as u32).wrapping_neg()
// CHECK-NEXT:         || ((unsafe { (*std::ptr::addr_of!(x.__slate_bits_0)).__get_b() }) as u32)
// CHECK-NEXT:             != (456 as u32).wrapping_neg()
// CHECK-NEXT:     {
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
