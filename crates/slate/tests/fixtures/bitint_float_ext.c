#include <stdio.h>

static void ld_to(void) {
  _BitInt(200) a = 1;
  a <<= 150;
  a += 1;
  long double l = a;
  printf("%La\n", l);
  _BitInt(200) b = 1;
  b <<= 100;
  b += 12345;
  printf("%La\n", (long double)b);
  _BitInt(200) c = 0x7fffffffffffffffLL;
  c = c * 2 + 1;
  c <<= 70;
  c += 0x200;
  printf("%La\n", (long double)c);
  printf("%La\n", (long double)-c);
  _BitInt(200) tie = 1;
  tie <<= 100;
  tie += (_BitInt(200))1 << 36;
  printf("%La\n", (long double)tie);
  tie += (_BitInt(200))1 << 37;
  printf("%La\n", (long double)tie);
  unsigned _BitInt(200) u = 3;
  u <<= 190;
  printf("%La\n", (long double)u);
  _BitInt(135) z = 0;
  printf("%La\n", (long double)z);
}

static void ld_from(void) {
  long double values[] = {1.5L, -7.25L, 1e30L, -1e40L, 123456789012345678901234567890.5L, 0.75L};
  for (int i = 0; i < 6; ++i) {
    _BitInt(200) s = (_BitInt(200))values[i];
    unsigned _BitInt(200) u = (unsigned _BitInt(200))(values[i] < 0 ? -values[i] : values[i]);
    printf("%.17g %.17g\n", (double)s, (double)u);
    printf("%d\n", (int)(s % 1000));
  }
}

static void half_quad(void) {
  _BitInt(135) h = 65519;
  printf("%d\n", (int)(float)(_Float16)h);
  _BitInt(135) h2 = 65520;
  printf("%d\n", (int)((_Float16)h2 > 65504));
  _BitInt(200) m = 2049;
  printf("%d\n", (int)(float)(_Float16)m);
  m = ((_BitInt(200))1 << 120) + 1;
  printf("%d\n", (long double)(_Float128)m == (long double)m);
  _Float128 q = (_Float128)m;
  _BitInt(200) back = (_BitInt(200))q;
  printf("%d\n", back == m);
  _Float16 hv = -300.75;
  _BitInt(135) hb = (_BitInt(135))hv;
  printf("%d\n", (int)hb);
}

int main(void) {
  ld_to();
  ld_from();
  half_quad();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(f16)]
// CHECK-NEXT: #![feature(f128)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_libgcc {
// CHECK-NEXT:     #[link(name = "gcc", kind = "static")]
// CHECK-NEXT:     unsafe extern "C" {}
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_powi(__a: LongDouble, __n: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     fn __slate_f80_va_arg(__ap: *mut core::ffi::VaList<'static>) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sqrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cbrt(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_exp2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_expm1(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log2(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log10(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_log1p(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asin(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acos(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atan(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_sinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_cosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_tanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_asinh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_acosh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_atanh(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_nearbyint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fmod(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_remainder(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_pow(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_fdim(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_hypot(__a: LongDouble, __b: LongDouble) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_fma(
// CHECK-NEXT:         __a: LongDouble,
// CHECK-NEXT:         __b: LongDouble,
// CHECK-NEXT:         __c: LongDouble,
// CHECK-NEXT:     ) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// CHECK-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_f128(__a: f128) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// CHECK-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// CHECK-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// CHECK-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// CHECK-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// CHECK-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// CHECK-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// CHECK-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// CHECK-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// CHECK-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// CHECK-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// CHECK-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// CHECK-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// CHECK-NEXT:     safe fn __slate_f80_to_f128(__a: LongDouble) -> f128;
// CHECK-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *const i8, _1: LongDouble) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: impl core::cmp::PartialOrd for LongDouble {
// CHECK-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// CHECK-NEXT:         if __slate_f80_lt(*self, *__other) {
// CHECK-NEXT:             Some(std::cmp::Ordering::Less)
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if __slate_f80_gt(*self, *__other) {
// CHECK-NEXT:                 Some(std::cmp::Ordering::Greater)
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 if __slate_f80_eq(*self, *__other) {
// CHECK-NEXT:                     Some(std::cmp::Ordering::Equal)
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     None
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn ld_to() {
// CHECK-NEXT:     let mut a: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(1 as i128);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = a;
// CHECK-NEXT:     a = [[V_0]].shl_bits(150 as usize);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = a;
// CHECK-NEXT:     a = [[V_1]].wrapping_add(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128));
// CHECK-NEXT:     let l: LongDouble = LongDouble(a.to_f80_bytes());
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { __slate_printf__ri32_pi8_f80(c"%La\n".as_ptr(), l) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut b: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(1 as i128);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = b;
// CHECK-NEXT:     b = [[V_2]].shl_bits(100 as usize);
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = b;
// CHECK-NEXT:     b = [[V_3]]
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(12345 as i128));
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(c"%La\n".as_ptr(), LongDouble(b.to_f80_bytes()))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut c: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(9223372036854775807 as i128);
// CHECK-NEXT:     c = c
// CHECK-NEXT:         .wrapping_mul(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(2 as i128))
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128));
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = c;
// CHECK-NEXT:     c = [[V_4]].shl_bits(70 as usize);
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = c;
// CHECK-NEXT:     c = [[V_5]]
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(512 as i128));
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(c"%La\n".as_ptr(), LongDouble(c.to_f80_bytes()))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%La\n".as_ptr(),
// CHECK-NEXT:             LongDouble(c.wrapping_neg().to_f80_bytes()),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut tie: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(1 as i128);
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = tie;
// CHECK-NEXT:     tie = [[V_6]].shl_bits(100 as usize);
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = tie;
// CHECK-NEXT:     tie = [[V_7]]
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128)
// CHECK-NEXT:                 .shl_bits(36 as usize),
// CHECK-NEXT:         );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(c"%La\n".as_ptr(), LongDouble(tie.to_f80_bytes()))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = tie;
// CHECK-NEXT:     tie = [[V_8]]
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128)
// CHECK-NEXT:                 .shl_bits(37 as usize),
// CHECK-NEXT:         );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(c"%La\n".as_ptr(), LongDouble(tie.to_f80_bytes()))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut u: bitint::BUint<200, 4, 32, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(3 as i128));
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: bitint::BUint<200, 4, 32, bitint::A8> = u;
// CHECK-NEXT:     u = [[V_9]].shl_bits(190 as usize);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(c"%La\n".as_ptr(), LongDouble(u.to_f80_bytes()))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut z: bitint::BInt<135, 3, 24, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         135,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(0 as i128);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(c"%La\n".as_ptr(), LongDouble(z.to_f80_bytes()))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn ld_from() {
// CHECK-NEXT:     let values: [LongDouble; 6] = [
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 232, 1, 192]),
// CHECK-NEXT:         LongDouble([223, 78, 103, 4, 205, 201, 242, 201, 98, 64]),
// CHECK-NEXT:         LongDouble([253, 37, 229, 26, 142, 79, 25, 235, 131, 192]),
// CHECK-NEXT:         LongDouble([119, 240, 185, 97, 251, 135, 116, 199, 95, 64]),
// CHECK-NEXT:         LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 254, 63]),
// CHECK-NEXT:     ];
// CHECK-NEXT:     for i in 0i32..6i32 {
// CHECK-NEXT:         let mut s: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_f80_bytes(values[i as usize].0);
// CHECK-NEXT:         let mut u: bitint::BUint<200, 4, 32, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:             200,
// CHECK-NEXT:             4,
// CHECK-NEXT:             32,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_f80_bytes(
// CHECK-NEXT:             if values[i as usize] < __slate_f80_from_i32(0i32) {
// CHECK-NEXT:                 -values[i as usize]
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 values[i as usize]
// CHECK-NEXT:             }
// CHECK-NEXT:                 .0,
// CHECK-NEXT:         );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%.17g %.17g\n".as_ptr(), s.to_f64(), u.to_f64()) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{}", s.wrapping_rem(< bitint::BInt < 200, 4, 32, bitint::A8
// CHECK-NEXT:             >>::from_i128(1000 as i128)).to_i128() as i32
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn half_quad() {
// CHECK-NEXT:     let mut h: bitint::BInt<135, 3, 24, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         135,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(65519 as i128);
// CHECK-NEXT:     println!("{}", (f16::from_bits(h.to_ieee_bits(10, 5, false) as u16) as f32) as i32);
// CHECK-NEXT:     let mut h2: bitint::BInt<135, 3, 24, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         135,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(65520 as i128);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (f16::from_bits(h2.to_ieee_bits(10, 5, false) as u16) > (65504i32 as f16))
// CHECK-NEXT:         as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut m: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(2049 as i128);
// CHECK-NEXT:     println!("{}", (f16::from_bits(m.to_ieee_bits(10, 5, false) as u16) as f32) as i32);
// CHECK-NEXT:     m = <bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128)
// CHECK-NEXT:         .shl_bits(120 as usize)
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(1 as i128));
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (__slate_f80_from_f128(f128::from_bits(m.to_ieee_bits(112, 15, false))) ==
// CHECK-NEXT:         LongDouble(m.to_f80_bytes())) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let q: f128 = f128::from_bits(m.to_ieee_bits(112, 15, false));
// CHECK-NEXT:     let mut back: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_ieee_bits(q.to_bits() as u128, 112, 15, false);
// CHECK-NEXT:     println!("{}", (back.compare(m) == 0) as i32);
// CHECK-NEXT:     let hv: f16 = -300.75f64 as f16;
// CHECK-NEXT:     let mut hb: bitint::BInt<135, 3, 24, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         135,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_ieee_bits(hv.to_bits() as u128, 10, 5, false);
// CHECK-NEXT:     println!("{}", hb.to_i128() as i32);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     ld_to();
// CHECK-NEXT:     ld_from();
// CHECK-NEXT:     half_quad();
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
