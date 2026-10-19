#include <stdio.h>
#include <string.h>

struct __attribute__((packed)) P {
  char        tag;
  long double l;
  unsigned    k : 12, j : 13;
};

struct Inner {
  long double x;
  int         n;
};

struct __attribute__((packed)) Q {
  char         c;
  struct Inner inner;
};

union Empty {};

struct P gp = {'a', 1.5L, 7, 9};

static long double half(long double *p) { return *p / 2; }

int main(void) {
  struct P p = {'b', 2.25L, 3, 4};
  struct Q q;
  memset(&q, 0, sizeof q);
  q.c       = 'q';
  q.inner   = (struct Inner){8.5L, 11};
  p.l      += 1.0L;
  gp.l      = gp.l * 3;
  long double copy;
  memcpy(&copy, &p.l, sizeof copy);
  struct P same = p;
  printf("%zu %zu %zu %zu\n", sizeof(struct P), sizeof(struct Q),
         sizeof(union Empty), __builtin_offsetof(struct P, l));
  printf("%c %.2Lf %u %u\n", p.tag, p.l, p.k, p.j);
  printf("%c %.2Lf %u %u\n", gp.tag, gp.l, gp.k, gp.j);
  printf("%.2Lf %.2Lf\n", copy, same.l);
  struct Inner inner = q.inner;
  printf("%c %.2Lf %d\n", q.c, inner.x, inner.n);
  long double aligned = p.l;
  printf("%.2Lf\n", half(&aligned));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn memcpy(__dest: *mut (), __src: *const (), __n: u64) -> *mut ();
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
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
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80_f80(
// CHECK-NEXT:         _0: *const i8,
// CHECK-NEXT:         _1: LongDouble,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_i32_f80_i32(
// CHECK-NEXT:         _0: *const i8,
// CHECK-NEXT:         _1: i32,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:         _3: i32,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_i32_f80_i32_i32(
// CHECK-NEXT:         _0: *const i8,
// CHECK-NEXT:         _1: i32,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:         _3: i32,
// CHECK-NEXT:         _4: i32,
// CHECK-NEXT:     ) -> i32;
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
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct P {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     l: std::mem::MaybeUninit<[u8; 16]>,
// CHECK-NEXT:     __slate_bits_0: __slate_bits::__SlateBits1U0,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Inner {
// CHECK-NEXT:     x: LongDouble,
// CHECK-NEXT:     n: i32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Q {
// CHECK-NEXT:     c: i8,
// CHECK-NEXT:     inner: std::mem::MaybeUninit<[u8; 32]>,
// CHECK-NEXT: }
// CHECK-NEXT: mod __slate_bits {
// CHECK-NEXT:     #[bitfields::bitfield(
// CHECK-NEXT:         [u8;4],
// CHECK-NEXT:         c_names = true,
// CHECK-NEXT:         new = false,
// CHECK-NEXT:         from_into_bits = false,
// CHECK-NEXT:         from_traits = false,
// CHECK-NEXT:         default = false,
// CHECK-NEXT:         debug = false,
// CHECK-NEXT:         builder = false,
// CHECK-NEXT:         bit_ops = false
// CHECK-NEXT:     )]
// CHECK-NEXT:     pub struct __SlateBits1U0 {
// CHECK-NEXT:         #[bits(12)]
// CHECK-NEXT:         pub k: u32,
// CHECK-NEXT:         #[bits(13)]
// CHECK-NEXT:         pub j: u32,
// CHECK-NEXT:         #[bits(7, access = na)]
// CHECK-NEXT:         pub __slate_pad_2: u8,
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut gp: P = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: P = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].tag = 97i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!([[T_0]].l) as *mut LongDouble)
// CHECK-NEXT:             .write_unaligned(LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]));
// CHECK-NEXT:     }
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_k(7u32);
// CHECK-NEXT:     [[T_0]].__slate_bits_0.__set_j(9u32);
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-NEXT: fn half(p: &LongDouble) -> LongDouble {
// CHECK-NEXT:     (unsafe { *(p as *const LongDouble) }) / __slate_f80_from_i32(2i32)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: Inner = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut p: P = {
// CHECK-NEXT:         let mut [[T_0]]: P = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].tag = 98i8;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (std::ptr::addr_of_mut!([[T_0]].l) as *mut LongDouble)
// CHECK-NEXT:                 .write_unaligned(LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 0, 64]));
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_k(3u32);
// CHECK-NEXT:         [[T_0]].__slate_bits_0.__set_j(4u32);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut q: Q = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             (std::ptr::addr_of_mut!(q) as *mut ()) as *mut u8,
// CHECK-NEXT:             0u8,
// CHECK-NEXT:             33 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     q.c = 113;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(q.inner) as *mut Inner)
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = {
// CHECK-NEXT:                         let mut [[T_1:__t[0-9]+]]: Inner = std::mem::zeroed();
// CHECK-NEXT:                         [[T_1]].x = LongDouble([0, 0, 0, 0, 0, 0, 0, 136, 2, 64]);
// CHECK-NEXT:                         [[T_1]].n = 11i32;
// CHECK-NEXT:                         [[T_1]]
// CHECK-NEXT:                     };
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 },
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: LongDouble = unsafe {
// CHECK-NEXT:         (std::ptr::addr_of!(p.l) as *const LongDouble).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(p.l) as *mut LongDouble)
// CHECK-NEXT:             .write_unaligned([[V_0]] + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(gp.l) as *mut LongDouble)
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 (std::ptr::addr_of!(gp.l) as *const LongDouble).read_unaligned()
// CHECK-NEXT:                     * __slate_f80_from_i32(3i32),
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut copy: LongDouble = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((std::ptr::addr_of_mut!(p.l) as *mut LongDouble) as *const ()) as *const u8,
// CHECK-NEXT:             (std::ptr::addr_of_mut!(copy) as *mut ()) as *mut u8,
// CHECK-NEXT:             16 as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let same: P = p;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 21u64, 33u64, 0u64, 1u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_i32_f80_i32_i32(
// CHECK-NEXT:             c"%c %.2Lf %u %u\n".as_ptr(),
// CHECK-NEXT:             p.tag as i32,
// CHECK-NEXT:             (std::ptr::addr_of!(p.l) as *const LongDouble).read_unaligned(),
// CHECK-NEXT:             p.__slate_bits_0.__get_k() as i32,
// CHECK-NEXT:             p.__slate_bits_0.__get_j() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_i32_f80_i32_i32(
// CHECK-NEXT:             c"%c %.2Lf %u %u\n".as_ptr(),
// CHECK-NEXT:             gp.tag as i32,
// CHECK-NEXT:             (std::ptr::addr_of!(gp.l) as *const LongDouble).read_unaligned(),
// CHECK-NEXT:             (*std::ptr::addr_of!(gp.__slate_bits_0)).__get_k() as i32,
// CHECK-NEXT:             (*std::ptr::addr_of!(gp.__slate_bits_0)).__get_j() as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80_f80(
// CHECK-NEXT:             c"%.2Lf %.2Lf\n".as_ptr(),
// CHECK-NEXT:             copy,
// CHECK-NEXT:             (std::ptr::addr_of!(same.l) as *const LongDouble).read_unaligned(),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let inner: Inner = unsafe {
// CHECK-NEXT:         (std::ptr::addr_of!(q.inner) as *const Inner).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_i32_f80_i32(
// CHECK-NEXT:             c"%c %.2Lf %d\n".as_ptr(),
// CHECK-NEXT:             q.c as i32,
// CHECK-NEXT:             inner.x,
// CHECK-NEXT:             inner.n,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut aligned: LongDouble = unsafe {
// CHECK-NEXT:         (std::ptr::addr_of!(p.l) as *const LongDouble).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"%.2Lf\n".as_ptr(),
// CHECK-NEXT:             half(&(*std::ptr::addr_of_mut!(aligned))),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
