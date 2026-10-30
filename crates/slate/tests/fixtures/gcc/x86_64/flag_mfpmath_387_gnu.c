/* { dg-options "-mfpmath=387 -std=gnu23" } */
#include <float.h>
#include <stdio.h>

volatile double huge = 1e308, ten = 10, one = 1, eps = 0x1p-53, dr1 = 1.0000000000000002;
volatile double tiny = 0x1p-600, c1 = 0x1.0000000000001p0, d1 = 0x1.8000000000001p0;
volatile float f = 16777216.0f, fone = 1.0f, fhuge = 3e38f, fa = 1.00000012f;
volatile long long big = (1LL << 53) + 1;
volatile int yes = 1;

static double literal = 0.1, summed = 0.1 + 0.2, overflowed = 1e308 * 10 / 10;
static double rounded = 1.00000000000000011102230246251565404236316680908203125000001;
static float single = 0.1f, single_sum = 0.1f + 0.2f;
static _Complex double complex_sum = 1.5 + 2.5i * 0.1;
static int truncated = (int)(0.1 * 35);

static void take(double x, double *out) { *out = x; }

int main(void) {
  double t = huge * ten;
  float h = f + fone;
  double y = one + eps;
  double stored = huge;
  double assigned;
  double passed;
  double compound = 1e308;
  double as_double = big;
  compound *= ten;
  stored = stored * ten / ten;
  take(huge * ten, &passed);
  printf("%d\n", FLT_EVAL_METHOD);
  printf("static %a %a %g %a %a %a %a %a %d\n", literal, summed, overflowed, rounded,
         (double)single, (double)single_sum, __real__ complex_sum, __imag__ complex_sum, truncated);
  printf("chain %g store %g cast %g arg %g\n", (huge * ten) / ten, t / ten,
         (double)(huge * ten) / ten, passed / ten);
  printf("float %g %g\n", (double)((f + fone) - f), (double)(h - f));
  printf("cmp %d %d %d\n", (one + eps) == one, y == one, huge * ten > 1e308);
  printf("cond %d\n", (tiny * tiny) ? 1 : 0);
  if (tiny * tiny)
    printf("if\n");
  printf("lit %d %d %d %.17g %.17g\n", 0.1f == 0.1, (double)0.1f == 0.1,
         0.1 * 3 == 0.30000000000000004,
         1.00000000000000011102230246251565404236316680908203125000001,
         9007199254740993.0000000001);
  printf("fold %g %g\n", 1e308 * 10 / 10, 1e308 * 10.0);
  printf("arms %g %g %g\n", (yes ? huge * ten : one) / ten, (yes, huge * ten) / ten,
         -(huge * ten) / ten);
  printf("stmt %g\n", ({ huge * ten; }) / ten);
  printf("wide %d %Lg %Lg\n", (int)((tiny * tiny) / tiny / tiny), (long double)(huge * ten),
         (long double)(fhuge * fhuge));
  printf("update %g %g %g\n", compound, stored, (assigned = huge * ten) / ten);
  printf("narrow %g %g %d %d\n", (double)(float)huge, (double)(float)(huge * ten / ten),
         (float)dr1 == dr1, (double)(float)dr1 == dr1);
  printf("widen %g %g\n", (double)(fhuge * 10.f) / 10.f, (fhuge * 10.f) * (double)1 / 10.f);
  printf("vararg %g\n", fhuge * fhuge);
  printf("int %d %g %d %g\n", (double)big == big, (big + 1.0) - big, as_double == big,
         (double)big - big);
  printf("round %a %a\n", c1 * d1, (double)(fa * fa * fa));
  _Complex double z = 1e308 + 1e308 * 1.0i;
  _Complex double zz = (z * ten) / ten;
  printf("complex %g %g %d\n", __real__ zz, __imag__ zz, sizeof(1.0f + 1.0f) == sizeof(float));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-mfpmath=387 -std=gnu23" }
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
// CHECK-NEXT:     safe fn __slate_cf80_add(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_sub(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_mul(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_mul_basic(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div_basic(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_div_improved(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:         __b: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_cf80_neg(
// CHECK-NEXT:         __a: num_complex::Complex<LongDouble>,
// CHECK-NEXT:     ) -> num_complex::Complex<LongDouble>;
// CHECK-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
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
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_i32_f80_f80(
// CHECK-NEXT:         _0: *const i8,
// CHECK-NEXT:         _1: i32,
// CHECK-NEXT:         _2: LongDouble,
// CHECK-NEXT:         _3: LongDouble,
// CHECK-NEXT:     ) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct LongDouble([u8; 10]);
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Add for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_add(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Sub for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_sub(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Mul for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_mul(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Div for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// CHECK-NEXT:         __slate_f80_div(self, __o)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::AddAssign for LongDouble {
// CHECK-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_add(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::SubAssign for LongDouble {
// CHECK-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_sub(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::MulAssign for LongDouble {
// CHECK-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_mul(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::DivAssign for LongDouble {
// CHECK-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *self = __slate_f80_div(*self, __o);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::ops::Neg for LongDouble {
// CHECK-NEXT:     type Output = LongDouble;
// CHECK-NEXT:     fn neg(self) -> LongDouble {
// CHECK-NEXT:         __slate_f80_neg(self)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: impl core::cmp::PartialEq for LongDouble {
// CHECK-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// CHECK-NEXT:         __slate_f80_eq(*self, *__other)
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-EMPTY:
// CHECK-NEXT: static mut huge: f64 = 1e308f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ten: f64 = 10i32 as f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut one: f64 = 1i32 as f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut eps: f64 = 1.1102230246251565e-16f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut dr1: f64 = 1.0000000000000002f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut tiny: f64 = 2.409919865102884e-181f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut c1: f64 = 1.0000000000000002f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut d1: f64 = 1.5000000000000002f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: f32 = 16777216.0f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut fone: f32 = 1.0f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut fhuge: f32 = 3e38f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut fa: f32 = 1.0000001f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut big: i64 = 9007199254740993;
// CHECK-EMPTY:
// CHECK-NEXT: static mut yes: i32 = 1;
// CHECK-EMPTY:
// CHECK-NEXT: static mut literal: f64 = 0.1f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut summed: f64 = 0.30000000000000004f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut overflowed: f64 = f64::INFINITY;
// CHECK-EMPTY:
// CHECK-NEXT: static mut rounded: f64 = 1.0000000000000002f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut single: f32 = 0.1f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut single_sum: f32 = 0.3f32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut complex_sum: num_complex::Complex<f64> = num_complex::Complex {
// CHECK-NEXT:     re: 1.5f64,
// CHECK-NEXT:     im: 0.25f64,
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: static mut truncated: i32 = 3.5f64 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: fn take(x: f64, out: &mut f64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(out as *mut f64) = x;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let t: f64 = __slate_f80_to_f64(
// CHECK-NEXT:         __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge)) })
// CHECK-NEXT:             * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) }),
// CHECK-NEXT:     );
// CHECK-NEXT:     let h: f32 = __slate_f80_to_f32(
// CHECK-NEXT:         __slate_f80_from_f32(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) })
// CHECK-NEXT:             + __slate_f80_from_f32(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fone)) }),
// CHECK-NEXT:     );
// CHECK-NEXT:     let y: f64 = __slate_f80_to_f64(
// CHECK-NEXT:         __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:             + __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(eps)) }),
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut stored: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge)) };
// CHECK-NEXT:     let mut assigned: f64 = 0 as f64;
// CHECK-NEXT:     let mut passed: f64 = 0 as f64;
// CHECK-NEXT:     let mut compound: f64 = 1e308f64;
// CHECK-NEXT:     let as_double: f64 = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(big)) }) as f64;
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: f64 = compound;
// CHECK-NEXT:     compound = __slate_f80_to_f64(
// CHECK-NEXT:         __slate_f80_from_f64([[V_0]])
// CHECK-NEXT:             * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) }),
// CHECK-NEXT:     );
// CHECK-NEXT:     stored = __slate_f80_to_f64(
// CHECK-NEXT:         __slate_f80_from_f64(stored)
// CHECK-NEXT:             * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) })
// CHECK-NEXT:             / __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) }),
// CHECK-NEXT:     );
// CHECK-NEXT:     take(
// CHECK-NEXT:         __slate_f80_to_f64(
// CHECK-NEXT:             __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge)) })
// CHECK-NEXT:                 * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) }),
// CHECK-NEXT:         ),
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(passed)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", 2i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"static %a %a %g %a %a %a %a %a %d\n".as_ptr(),
// CHECK-NEXT:             literal,
// CHECK-NEXT:             summed,
// CHECK-NEXT:             overflowed,
// CHECK-NEXT:             rounded,
// CHECK-NEXT:             single as f64,
// CHECK-NEXT:             single_sum as f64,
// CHECK-NEXT:             complex_sum.re,
// CHECK-NEXT:             complex_sum.im,
// CHECK-NEXT:             truncated,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"chain %g store %g cast %g arg %g\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(huge)))
// CHECK-NEXT:                     * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten)))
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f64(t)
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(huge)))
// CHECK-NEXT:                     * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten)))
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f64(passed)
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"float %g %g\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(f)))
// CHECK-NEXT:                     + __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fone)))
// CHECK-NEXT:                     - __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(f))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f32(h)
// CHECK-NEXT:                     - __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(f))),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "cmp {} {} {}",
// CHECK-NEXT:         (__slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) })
// CHECK-NEXT:             + __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(eps)) })
// CHECK-NEXT:             == __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) }))
// CHECK-NEXT:             as i32,
// CHECK-NEXT:         (y == unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) }) as i32,
// CHECK-NEXT:         (__slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge)) })
// CHECK-NEXT:             * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) })
// CHECK-NEXT:             > __slate_f80_from_f64(1e308f64)) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "cond {}",
// CHECK-NEXT:         if __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(tiny)) })
// CHECK-NEXT:             * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(tiny)) })
// CHECK-NEXT:             != LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
// CHECK-NEXT:         {
// CHECK-NEXT:             1i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             0i32
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     if __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(tiny)) })
// CHECK-NEXT:         * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(tiny)) })
// CHECK-NEXT:         != LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("if");
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"lit %d %d %d %.17g %.17g\n".as_ptr(),
// CHECK-NEXT:             ((0.1f32 as f64) == 0.1f64) as i32,
// CHECK-NEXT:             ((0.1f32 as f64) == 0.1f64) as i32,
// CHECK-NEXT:             (0.30000000000000004f64 == 0.30000000000000004f64) as i32,
// CHECK-NEXT:             1.0000000000000002f64,
// CHECK-NEXT:             9007199254740994.0f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"fold %g %g\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_f64(LongDouble([0, 0, 69, 94, 47, 156, 103, 142, 254, 67])),
// CHECK-NEXT:             __slate_f80_to_f64(LongDouble([0, 64, 214, 53, 59, 131, 1, 178, 1, 68])),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(yes)) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"arms %g %g %g\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 (if std::ptr::read_volatile(std::ptr::addr_of!(yes)) != 0 {
// CHECK-NEXT:                     __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(huge)))
// CHECK-NEXT:                         * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten)))
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(one)))
// CHECK-NEXT:                 }) / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(huge)))
// CHECK-NEXT:                     * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten)))
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 -(__slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(huge)))
// CHECK-NEXT:                     * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))))
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: LongDouble;
// CHECK-NEXT:     [[V_1]] = __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge)) })
// CHECK-NEXT:         * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"stmt %g\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 [[V_1]] / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_i32_f80_f80(
// CHECK-NEXT:             c"wide %d %Lg %Lg\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_i32(
// CHECK-NEXT:                 __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(tiny)))
// CHECK-NEXT:                     * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(tiny)))
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(tiny)))
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(tiny))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(huge)))
// CHECK-NEXT:                 * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fhuge)))
// CHECK-NEXT:                 * __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fhuge))),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: f64 = __slate_f80_to_f64(
// CHECK-NEXT:         __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge)) })
// CHECK-NEXT:             * __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) }),
// CHECK-NEXT:     );
// CHECK-NEXT:     assigned = [[V_2]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"update %g %g %g\n".as_ptr(),
// CHECK-NEXT:             compound,
// CHECK-NEXT:             stored,
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f64([[V_2]])
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"narrow %g %g %d %d\n".as_ptr(),
// CHECK-NEXT:             (std::ptr::read_volatile(std::ptr::addr_of!(huge)) as f32) as f64,
// CHECK-NEXT:             __slate_f80_to_f32(
// CHECK-NEXT:                 __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(huge)))
// CHECK-NEXT:                     * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten)))
// CHECK-NEXT:                     / __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(ten))),
// CHECK-NEXT:             ) as f64,
// CHECK-NEXT:             (((std::ptr::read_volatile(std::ptr::addr_of!(dr1)) as f32) as f64)
// CHECK-NEXT:                 == std::ptr::read_volatile(std::ptr::addr_of!(dr1))) as i32,
// CHECK-NEXT:             (((std::ptr::read_volatile(std::ptr::addr_of!(dr1)) as f32) as f64)
// CHECK-NEXT:                 == std::ptr::read_volatile(std::ptr::addr_of!(dr1))) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"widen %g %g\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fhuge)))
// CHECK-NEXT:                     * __slate_f80_from_f32(10.0f32)
// CHECK-NEXT:                     / __slate_f80_from_f64(10.0f32 as f64),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fhuge)))
// CHECK-NEXT:                     * __slate_f80_from_f32(10.0f32)
// CHECK-NEXT:                     * __slate_f80_from_i32(1i32)
// CHECK-NEXT:                     / __slate_f80_from_f64(10.0f32 as f64),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"vararg %g\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fhuge)))
// CHECK-NEXT:                     * __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fhuge))),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"int %d %g %d %g\n".as_ptr(),
// CHECK-NEXT:             (__slate_f80_from_i64(std::ptr::read_volatile(std::ptr::addr_of!(big)))
// CHECK-NEXT:                 == __slate_f80_from_i64(std::ptr::read_volatile(std::ptr::addr_of!(big))))
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_i64(std::ptr::read_volatile(std::ptr::addr_of!(big)))
// CHECK-NEXT:                     + __slate_f80_from_f64(1.0f64)
// CHECK-NEXT:                     - __slate_f80_from_i64(std::ptr::read_volatile(std::ptr::addr_of!(big))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             (__slate_f80_from_f64(as_double)
// CHECK-NEXT:                 == __slate_f80_from_i64(std::ptr::read_volatile(std::ptr::addr_of!(big))))
// CHECK-NEXT:                 as i32,
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_i64(std::ptr::read_volatile(std::ptr::addr_of!(big)))
// CHECK-NEXT:                     - __slate_f80_from_i64(std::ptr::read_volatile(std::ptr::addr_of!(big))),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"round %a %a\n".as_ptr(),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(c1)))
// CHECK-NEXT:                     * __slate_f80_from_f64(std::ptr::read_volatile(std::ptr::addr_of!(d1))),
// CHECK-NEXT:             ),
// CHECK-NEXT:             __slate_f80_to_f64(
// CHECK-NEXT:                 __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fa)))
// CHECK-NEXT:                     * __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fa)))
// CHECK-NEXT:                     * __slate_f80_from_f32(std::ptr::read_volatile(std::ptr::addr_of!(fa))),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let z: num_complex::Complex<f64> = {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = {
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = __slate_f80_from_f64(1e308f64);
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = {
// CHECK-NEXT:                 let [[T_3:__t[0-9]+]] = __slate_f80_from_f64(1e308f64);
// CHECK-NEXT:                 let [[T_4:__t[0-9]+]] = {
// CHECK-NEXT:                     let [[T_5:__t[0-9]+]] = num_complex::Complex {
// CHECK-NEXT:                         re: 0.0f64,
// CHECK-NEXT:                         im: 1.0f64,
// CHECK-NEXT:                     };
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: __slate_f80_from_f64([[T_5]].re),
// CHECK-NEXT:                         im: __slate_f80_from_f64([[T_5]].im),
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:                 num_complex::Complex {
// CHECK-NEXT:                     re: [[T_3]] * [[T_4]].re,
// CHECK-NEXT:                     im: [[T_3]] * [[T_4]].im,
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_1]] + [[T_2]].re,
// CHECK-NEXT:                 im: [[T_2]].im,
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: __slate_f80_to_f64([[T_0]].re),
// CHECK-NEXT:             im: __slate_f80_to_f64([[T_0]].im),
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let zz: num_complex::Complex<f64> = {
// CHECK-NEXT:         let [[T_6:__t[0-9]+]] = {
// CHECK-NEXT:             let [[T_7:__t[0-9]+]] = {
// CHECK-NEXT:                 let [[T_8:__t[0-9]+]] = {
// CHECK-NEXT:                     let [[T_9:__t[0-9]+]] = z;
// CHECK-NEXT:                     num_complex::Complex {
// CHECK-NEXT:                         re: __slate_f80_from_f64([[T_9]].re),
// CHECK-NEXT:                         im: __slate_f80_from_f64([[T_9]].im),
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:                 let [[T_10:__t[0-9]+]] = __slate_f80_from_f64(unsafe {
// CHECK-NEXT:                     std::ptr::read_volatile(std::ptr::addr_of!(ten))
// CHECK-NEXT:                 });
// CHECK-NEXT:                 num_complex::Complex {
// CHECK-NEXT:                     re: [[T_8]].re * [[T_10]],
// CHECK-NEXT:                     im: [[T_8]].im * [[T_10]],
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:             let [[T_11:__t[0-9]+]] =
// CHECK-NEXT:                 __slate_f80_from_f64(unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ten)) });
// CHECK-NEXT:             num_complex::Complex {
// CHECK-NEXT:                 re: [[T_7]].re / [[T_11]],
// CHECK-NEXT:                 im: [[T_7]].im / [[T_11]],
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         num_complex::Complex {
// CHECK-NEXT:             re: __slate_f80_to_f64([[T_6]].re),
// CHECK-NEXT:             im: __slate_f80_to_f64([[T_6]].im),
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"complex %g %g %d\n".as_ptr(), zz.re, zz.im, true as i32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
