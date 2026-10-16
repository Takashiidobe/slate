/* The bit-field below would have a problem if __INT_MAX__ is too
   small.  */
void abort(void);
void exit(int);

#if __INT_MAX__ < 2147483647
int main(void) { exit(0); }
#else
/* Failed on powerpc due to bad extzvsi pattern.  */

struct ieee {
  unsigned int negative  : 1;
  unsigned int exponent  : 11;
  unsigned int mantissa0 : 20;
  unsigned int mantissa1 : 32;
} x;

unsigned int foo(void) {
  unsigned int exponent;

  exponent = x.exponent;
  if (exponent == 0)
    return 1;
  else if (exponent > 1)
    return 2;
  return 0;
}

int main(void) {
  x.exponent = 1;
  if (foo() != 0)
    abort();
  return 0;
}
#endif

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Failed on powerpc due to bad extzvsi pattern.
// CHECK-NEXT: // The bit-field below would have a problem if __INT_MAX__ is too
// CHECK-NEXT: // small.
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct ieee {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits0U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 8], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits0U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub negative: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub exponent: u32,
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub mantissa0: u32,
// CHECK-NEXT:         #[bits(32)]
// CHECK-NEXT:         pub mantissa1: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: ieee = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() -> u32 {
// CHECK-NEXT:     let mut exponent: u32 =
// CHECK-NEXT:         ((unsafe { (*std::ptr::addr_of!(x.__slate_bits_0)).__get_exponent() }) as i32) as u32;
// CHECK-NEXT:     if exponent == ((0 as i32) as u32) {
// CHECK-NEXT:         return (1 as i32) as u32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if exponent > ((1 as i32) as u32) {
// CHECK-NEXT:             return (2 as i32) as u32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     (0 as i32) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*std::ptr::addr_of_mut!(x.__slate_bits_0)).__set_exponent((1 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if foo() != ((0 as i32) as u32) {
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
