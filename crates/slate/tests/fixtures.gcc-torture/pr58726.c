/* PR rtl-optimization/58726 */

int a, c;
union {
  int f1;
  int f2 : 1;
} b;

short foo(short p) { return p < 0 ? p : a; }

int main() {
  if (sizeof(short) * __CHAR_BIT__ != 16 || sizeof(int) * __CHAR_BIT__ != 32)
    return 0;
  b.f1 = 56374;
  unsigned short d;
  int            e = b.f2;
  d                = e == 0 ? b.f1 : 0;
  c                = foo(d);
  if (c != (short)56374)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/58726
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = unsafe { std::mem::zeroed() };
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
// CHECK-NEXT:         pub f2: i32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_1: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     f1: i32,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: i16) -> i16 {
// CHECK-NEXT:     (if (p as i32) < (0 as i32) {
// CHECK-NEXT:         p as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { a }
// CHECK-NEXT:     }) as i16
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (2 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((16 as i32) as i64) as u64)
// CHECK-NEXT:         || (4 as u64).wrapping_mul(((8 as i32) as i64) as u64) != (((32 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b.f1 = 56374 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut d: u16 = 0 as u16;
// CHECK-NEXT:     let mut e: i32 = unsafe { (*std::ptr::addr_of!(b.__slate_bits_0)).__get_f2() };
// CHECK-NEXT:     d = ((if e == (0 as i32) {
// CHECK-NEXT:         unsafe { b.f1 }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     }) as i16) as u16;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = foo(d as i16) as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { c }) != (((56374 as i32) as i16) as i32) {
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
