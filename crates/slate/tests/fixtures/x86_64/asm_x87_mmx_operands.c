#include <stdio.h>

static long double root(long double x) {
  long double r;
  __asm__("fsqrt" : "=t"(r) : "0"(x));
  return r;
}

static long double arctan2(long double y, long double x) {
  long double r;
  __asm__ __volatile__("fpatan" : "=t"(r) : "0"(x), "u"(y) : "st(1)");
  return r;
}

static void sincos(double x, double *s, double *c) {
  __asm__("fsincos" : "=t"(*c), "=u"(*s) : "0"(x));
}

static int below(long double a, long double b) {
  int flag;
  __asm__("fucomi %%st(1), %%st" : "=@ccb"(flag) : "t"(a), "u"(b));
  return flag;
}

static float square(float x) {
  float r;
  __asm__("fld %1\n\tfmul %%st(0), %%st" : "=&t"(r) : "f"(x));
  return r;
}

static long double sum3(long double a, long double b, long double c) {
  long double r;
  __asm__("fadd %2, %%st\n\tfadd %3, %%st" : "=t"(r) : "0"(a), "f"(c), "u"(b));
  return r;
}

static double from_int(int i) {
  double r;
  __asm__("fildl %1" : "=t"(r) : "m"(i));
  return r;
}

static long double scale(long double x) {
  __asm__("fadd %%st(0), %%st" : "+t"(x));
  return x;
}

static unsigned long long add_words(unsigned long long a, unsigned long long b) {
  __asm__("paddw %1, %0" : "=y"(a) : "y"(b), "0"(a));
  __asm__ __volatile__("emms");
  return a;
}

static int add_dwords(int a, int b) {
  __asm__("paddd %1, %0" : "+y"(a) : "y"(b));
  __asm__ __volatile__("emms");
  return a;
}

int main(void) {
  double s;
  double c;
  sincos(0.5, &s, &c);
  printf("%.12Lf\n", root(2.0L));
  printf("%.12Lf\n", arctan2(1.0L, 1.0L));
  printf("%.12f %.12f\n", s, c);
  printf("%d %d\n", below(1.0L, 2.0L), below(3.0L, 2.0L));
  printf("%.3f\n", square(1.5f));
  printf("%.3Lf\n", sum3(1.0L, 2.0L, 3.0L));
  printf("%.1f\n", from_int(-42));
  printf("%.3Lf\n", scale(2.25L));
  printf("%llx\n", add_words(0x0001000200030004ULL, 0x001000200030ffffULL));
  printf("%d\n", add_dwords(5, -7));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
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
// CHECK-NEXT: fn root(x: LongDouble) -> LongDouble {
// CHECK-NEXT:     let mut r: LongDouble = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: LongDouble = x;
// CHECK-NEXT:         let mut [[T_1:__t[0-9]+]]: LongDouble = std::mem::zeroed();
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fldt ({0:r})\nfsqrt\nfstpt ({1:r})", in (reg) std::ptr::addr_of!([[T_0]]), in
// CHECK-NEXT:             (reg) std::ptr::addr_of_mut!([[T_1]]), lateout("st(0)") _, lateout("st(1)") _,
// CHECK-NEXT:             lateout("st(2)") _, lateout("st(3)") _, lateout("st(4)") _, lateout("st(5)")
// CHECK-NEXT:             _, lateout("st(6)") _, lateout("st(7)") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = [[T_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn arctan2(y: LongDouble, x: LongDouble) -> LongDouble {
// CHECK-NEXT:     let mut r: LongDouble = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: LongDouble = x;
// CHECK-NEXT:         let mut [[T_1]]: LongDouble = std::mem::zeroed();
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: LongDouble = y;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fldt ({2:r})\nfldt ({0:r})\nfpatan\nfstpt ({1:r})", in (reg)
// CHECK-NEXT:             std::ptr::addr_of!([[T_0]]), in (reg) std::ptr::addr_of_mut!([[T_1]]), in (reg)
// CHECK-NEXT:             std::ptr::addr_of!([[T_2]]), lateout("st(0)") _, lateout("st(1)") _,
// CHECK-NEXT:             lateout("st(2)") _, lateout("st(3)") _, lateout("st(4)") _, lateout("st(5)")
// CHECK-NEXT:             _, lateout("st(6)") _, lateout("st(7)") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = [[T_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn sincos(x: f64, s: &mut f64, c: &mut f64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: f64 = x;
// CHECK-NEXT:         let mut [[T_1]]: f64 = std::mem::zeroed();
// CHECK-NEXT:         let mut [[T_2]]: f64 = std::mem::zeroed();
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fldl ({0:r})\nfsincos\nfstpl ({1:r})\nfstpl ({2:r})", in (reg)
// CHECK-NEXT:             std::ptr::addr_of!([[T_0]]), in (reg) std::ptr::addr_of_mut!([[T_1]]), in (reg)
// CHECK-NEXT:             std::ptr::addr_of_mut!([[T_2]]), lateout("st(0)") _, lateout("st(1)") _,
// CHECK-NEXT:             lateout("st(2)") _, lateout("st(3)") _, lateout("st(4)") _, lateout("st(5)")
// CHECK-NEXT:             _, lateout("st(6)") _, lateout("st(7)") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         *(c as *mut f64) = [[T_1]];
// CHECK-NEXT:         *(s as *mut f64) = [[T_2]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn below(a: LongDouble, b: LongDouble) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u8;
// CHECK-NEXT:         let [[T_1]]: LongDouble = a;
// CHECK-NEXT:         let [[T_2]]: LongDouble = b;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fldt ({2:r})\nfldt ({1:r})\nfucomi %st(1), %st\nsetb {0}\nfstp %st(0)\nfstp %st(0)",
// CHECK-NEXT:             lateout(reg_byte) [[T_0]], in (reg) std::ptr::addr_of!([[T_1]]), in (reg)
// CHECK-NEXT:             std::ptr::addr_of!([[T_2]]), lateout("st(0)") _, lateout("st(1)") _,
// CHECK-NEXT:             lateout("st(2)") _, lateout("st(3)") _, lateout("st(4)") _, lateout("st(5)")
// CHECK-NEXT:             _, lateout("st(6)") _, lateout("st(7)") _, options(pure, readonly, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn square(x: f32) -> f32 {
// CHECK-NEXT:     let mut r: f32 = 0 as f32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_0]]: f32 = std::mem::zeroed();
// CHECK-NEXT:         let [[T_1]]: f32 = x;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "flds ({1:r})\nfld %st\n\tfmul %st(0), %st\nfstps ({0:r})\nfstp %st(0)", in
// CHECK-NEXT:             (reg) std::ptr::addr_of_mut!([[T_0]]), in (reg) std::ptr::addr_of!([[T_1]]),
// CHECK-NEXT:             lateout("st(0)") _, lateout("st(1)") _, lateout("st(2)") _, lateout("st(3)")
// CHECK-NEXT:             _, lateout("st(4)") _, lateout("st(5)") _, lateout("st(6)") _,
// CHECK-NEXT:             lateout("st(7)") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = [[T_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn sum3(a: LongDouble, b: LongDouble, c: LongDouble) -> LongDouble {
// CHECK-NEXT:     let mut r: LongDouble = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: LongDouble = a;
// CHECK-NEXT:         let mut [[T_1]]: LongDouble = std::mem::zeroed();
// CHECK-NEXT:         let [[T_2]]: LongDouble = c;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: LongDouble = b;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fldt ({2:r})\nfldt ({3:r})\nfldt ({0:r})\nfadd %st(2), %st\n\tfadd %st(1), %st\nfstpt ({1:r})\nfstp %st(0)\nfstp %st(0)",
// CHECK-NEXT:             in (reg) std::ptr::addr_of!([[T_0]]), in (reg) std::ptr::addr_of_mut!([[T_1]]), in
// CHECK-NEXT:             (reg) std::ptr::addr_of!([[T_2]]), in (reg) std::ptr::addr_of!([[T_3]]),
// CHECK-NEXT:             lateout("st(0)") _, lateout("st(1)") _, lateout("st(2)") _, lateout("st(3)")
// CHECK-NEXT:             _, lateout("st(4)") _, lateout("st(5)") _, lateout("st(6)") _,
// CHECK-NEXT:             lateout("st(7)") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = [[T_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn from_int(mut i: i32) -> f64 {
// CHECK-NEXT:     let mut r: f64 = 0 as f64;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let mut [[T_0]]: f64 = std::mem::zeroed();
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fildl ({1:r})\nfstpl ({0:r})", in (reg) std::ptr::addr_of_mut!([[T_0]]), in
// CHECK-NEXT:             (reg) std::ptr::addr_of!(i), lateout("st(0)") _, lateout("st(1)") _,
// CHECK-NEXT:             lateout("st(2)") _, lateout("st(3)") _, lateout("st(4)") _, lateout("st(5)")
// CHECK-NEXT:             _, lateout("st(6)") _, lateout("st(7)") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = [[T_0]];
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn scale(mut x: LongDouble) -> LongDouble {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: LongDouble = x;
// CHECK-NEXT:         let mut [[T_1]]: LongDouble = std::mem::zeroed();
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fldt ({0:r})\nfadd %st(0), %st\nfstpt ({1:r})", in (reg)
// CHECK-NEXT:             std::ptr::addr_of!([[T_0]]), in (reg) std::ptr::addr_of_mut!([[T_1]]),
// CHECK-NEXT:             lateout("st(0)") _, lateout("st(1)") _, lateout("st(2)") _, lateout("st(3)")
// CHECK-NEXT:             _, lateout("st(4)") _, lateout("st(5)") _, lateout("st(6)") _,
// CHECK-NEXT:             lateout("st(7)") _, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = [[T_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn add_words(mut a: u64, b: u64) -> u64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64 = a;
// CHECK-NEXT:         let mut [[T_1]]: u64 = std::mem::zeroed();
// CHECK-NEXT:         let [[T_2]]: u64 = b;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq ({0:r}), %mm0\nmovq ({2:r}), %mm1\npaddw %mm1, %mm0\nmovq %mm0, ({1:r})",
// CHECK-NEXT:             in (reg) std::ptr::addr_of!([[T_0]]), in (reg) std::ptr::addr_of_mut!([[T_1]]), in
// CHECK-NEXT:             (reg) std::ptr::addr_of!([[T_2]]), lateout("mm0") _, lateout("mm1") _,
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         a = [[T_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("emms", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn add_dwords(mut a: i32, b: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: i32 = a;
// CHECK-NEXT:         let mut [[T_1]]: i32 = std::mem::zeroed();
// CHECK-NEXT:         let [[T_2]]: i32 = b;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movd ({0:r}), %mm0\nmovd ({2:r}), %mm1\npaddd %mm1, %mm0\nmovd %mm0, ({1:r})",
// CHECK-NEXT:             in (reg) std::ptr::addr_of!([[T_0]]), in (reg) std::ptr::addr_of_mut!([[T_1]]), in
// CHECK-NEXT:             (reg) std::ptr::addr_of!([[T_2]]), lateout("mm0") _, lateout("mm1") _,
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         a = [[T_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("emms", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut s: f64 = 0 as f64;
// CHECK-NEXT:     let mut c: f64 = 0 as f64;
// CHECK-NEXT:     sincos(
// CHECK-NEXT:         0.5f64,
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(s)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(c)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.12Lf\n".as_ptr(),
// CHECK-NEXT:             root(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64])),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.12Lf\n".as_ptr(),
// CHECK-NEXT:             arctan2(
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.12f %.12f\n".as_ptr(), s, c) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {}", below(LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]), LongDouble([0, 0,
// CHECK-NEXT:         0, 0, 0, 0, 0, 128, 0, 64])), below(LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0,
// CHECK-NEXT:         64]), LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]))
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { printf(c"%.3f\n".as_ptr(), square(1.5f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.3Lf\n".as_ptr(),
// CHECK-NEXT:             sum3(
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]),
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]),
// CHECK-NEXT:                 LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.1f\n".as_ptr(), from_int(-42i32)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.3Lf\n".as_ptr(),
// CHECK-NEXT:             scale(LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 0, 64])),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%llx\n".as_ptr(), add_words(281483566841860u64, 4503737069535231u64))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!("{}", add_dwords(5i32, - 7i32));
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
