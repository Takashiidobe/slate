/* PR target/91472 */
/* Reported by John Paul Adrian Glaubitz <glaubitz@physik.fu-berlin.de> */
/* { dg-require-effective-target double64plus } */

#if __SIZEOF_INT__ >= 4
typedef unsigned int gmp_uint_least32_t;
#else
typedef __UINT_LEAST32_TYPE__ gmp_uint_least32_t;
#endif

union ieee_double_extract {
  struct {
    gmp_uint_least32_t sig  : 1;
    gmp_uint_least32_t exp  : 11;
    gmp_uint_least32_t manh : 20;
    gmp_uint_least32_t manl : 32;
  } s;
  double d;
};

double __attribute__((noipa)) tests_infinity_d(void) {
  union ieee_double_extract x;
  x.s.exp  = 2047;
  x.s.manl = 0;
  x.s.manh = 0;
  x.s.sig  = 0;
  return x.d;
}

int main(void) {
  double x = tests_infinity_d();
  if (x == 0.0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/91472
// CHECK-NEXT: //!
// CHECK-NEXT: //! Reported by John Paul Adrian Glaubitz <glaubitz@physik.fu-berlin.de>
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target double64plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union ieee_double_extract {
// CHECK-NEXT:     s: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     d: f64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(4))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits2U0,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield([u8; 8], c_names = true, new = false, from_into_bits = false, from_traits = false, default = false, debug = false, builder = false, bit_ops = false)]
// CHECK-NEXT:     pub struct __SlateBits2U0 {
// CHECK-NEXT:         #[bits(1)]
// CHECK-NEXT:         pub sig: u32,
// CHECK-NEXT:         #[bits(11)]
// CHECK-NEXT:         pub exp: u32,
// CHECK-NEXT:         #[bits(20)]
// CHECK-NEXT:         pub manh: u32,
// CHECK-NEXT:         #[bits(32)]
// CHECK-NEXT:         pub manl: u32,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn tests_infinity_d() -> f64 {
// CHECK-NEXT:     let mut x: ieee_double_extract = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.s.__slate_bits_0.__set_exp((2047 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.s.__slate_bits_0.__set_manl((0 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.s.__slate_bits_0.__set_manh((0 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         x.s.__slate_bits_0.__set_sig((0 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { x.d }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: f64 = tests_infinity_d();
// CHECK-NEXT:     if x == 0.0f64 {
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
