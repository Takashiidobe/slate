/* { dg-require-effective-target int32plus } */
union U {
  const int a;
  unsigned  b : 24;
};

static union U u = {0x12345678};

/* Constant folding used to fail to account for endianness when folding a
   union.  */

int main(void) {
#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return u.b - 0x345678;
#else
  return u.b - 0x123456;
#endif
#endif
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: /// { dg-require-effective-target int32plus }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union U {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 3], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(24)]
// CHECK-NEXT:         pub b: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: U = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: U = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].a = 305419896 as i32;
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: // Constant folding used to fail to account for endianness when folding a
// CHECK-NEXT: // union.
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     return ((unsafe { (*std::ptr::addr_of!(u.__slate_bits_0)).__get_b() }) as i32)
// CHECK-NEXT:         - (3430008 as i32);
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
