/* { dg-options "-mavx512f" } */
#include <immintrin.h>
#include <stdio.h>

__asm__(".text\n"
        ".globl vabi_double4\n"
        "vabi_double4:\n"
        "  addps %xmm0, %xmm0\n"
        "  ret\n"
        ".globl vabi_add8\n"
        "vabi_add8:\n"
        "  vaddps %ymm1, %ymm0, %ymm0\n"
        "  ret\n"
        ".globl __vabi_scale_by16\n"
        "__vabi_scale_by16:\n"
        "  vbroadcastss %xmm1, %zmm1\n"
        "  vmulps %zmm1, %zmm0, %zmm0\n"
        "  ret\n"
        ".globl __vabi_sum8\n"
        "__vabi_sum8:\n"
        "  vextractf64x4 $1, %zmm0, %ymm1\n"
        "  vaddpd %ymm1, %ymm0, %ymm0\n"
        "  vextractf128 $1, %ymm0, %xmm1\n"
        "  vaddpd %xmm1, %xmm0, %xmm0\n"
        "  vhaddpd %xmm0, %xmm0, %xmm0\n"
        "  vzeroupper\n"
        "  ret\n"
        ".globl vabi_ld_double\n"
        "vabi_ld_double:\n"
        "  fldt 8(%rsp)\n"
        "  fadd %st(0), %st(0)\n"
        "  ret\n");

extern __m128 vabi_double4(__m128 x);
extern __m256 vabi_add8(__m256 a, __m256 b);
extern __m512 __vabi_scale_by16(__m512 x, float k);
extern double __vabi_sum8(__m512d x);
extern long double vabi_ld_double(long double x);

static void print_lanes(const char *name, const float *lanes, int count) {
  printf("%s:", name);
  for (int i = 0; i < count; i++)
    printf(" %g", lanes[i]);
  printf("\n");
}

int main(void) {
  if (!__builtin_cpu_supports("avx512f")) {
    printf("no avx512f\n");
    return 0;
  }
  float out4[4];
  _mm_storeu_ps(out4, vabi_double4(_mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f)));
  print_lanes("double4", out4, 4);

  float out8[8];
  _mm256_storeu_ps(out8, vabi_add8(_mm256_set1_ps(1.5f), _mm256_set_ps(8, 7, 6, 5, 4, 3, 2, 1)));
  print_lanes("add8", out8, 8);

  float out16[16];
  _mm512_storeu_ps(out16, __vabi_scale_by16(_mm512_set1_ps(2.0f), 0.25f));
  print_lanes("scale16", out16, 16);

  printf("sum8: %g\n", __vabi_sum8(_mm512_set_pd(8, 7, 6, 5, 4, 3, 2, 1)));
  printf("sum8 long double: %Lg\n", vabi_ld_double((long double)__vabi_sum8(_mm512_set1_pd(0.5))));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-mavx512f" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn vabi_double4([[ARG_0:arg[0-9]+]]: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "vabi_add8"]
// CHECK-NEXT:     fn __vector_abi_vabi_add8(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 8>;
// CHECK-NEXT:     #[link_name = "__vabi_scale_by16"]
// CHECK-NEXT:     fn __vector_abi___vabi_scale_by16(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_1]]: f32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 16>;
// CHECK-NEXT:     #[link_name = "__vabi_sum8"]
// CHECK-NEXT:     fn __vector_abi___vabi_sum8([[ARG_0]]: std::simd::Simd<f64, 8>) -> f64;
// CHECK-NEXT:     fn __builtin_cpu_supports([[ARG_0]]: *const i8) -> bool;
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
// CHECK-NEXT:     fn __slate_printf__ri32_pi8_f80(_0: *const i8, _1: LongDouble) -> i32;
// CHECK-NEXT:     fn __slate_vabi_ld_double__rf80_f80(_0: LongDouble) -> LongDouble;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
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
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_ps__2095 {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 4>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_ps__3282 {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 8>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_ps__4291 {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 16>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_set_ps(__z: f32, __y: f32, __x: f32, __w: f32) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([__w, __x, __y, __z]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_storeu_ps(__p: *mut f32, __a: std::simd::Simd<f32, 4>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((*(__p as *mut __storeu_ps__2095)).__v).write_unaligned(__a);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_storeu_ps(__p: *mut f32, __a: std::simd::Simd<f32, 8>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((*(__p as *mut __storeu_ps__3282)).__v).write_unaligned(__a);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".text\n.globl vabi_double4\nvabi_double4:\n  addps %xmm0, %xmm0\n  ret\n.globl vabi_add8\nvabi_add8:\n  vaddps %ymm1, %ymm0, %ymm0\n  ret\n.globl __vabi_scale_by16\n__vabi_scale_by16:\n  vbroadcastss %xmm1, %zmm1\n  vmulps %zmm1, %zmm0, %zmm0\n  ret\n.globl __vabi_sum8\n__vabi_sum8:\n  vextractf64x4 $1, %zmm0, %ymm1\n  vaddpd %ymm1, %ymm0, %ymm0\n  vextractf128 $1, %ymm0, %xmm1\n  vaddpd %xmm1, %xmm0, %xmm0\n  vhaddpd %xmm0, %xmm0, %xmm0\n  vzeroupper\n  ret\n.globl vabi_ld_double\nvabi_ld_double:\n  fldt 8(%rsp)\n  fadd %st(0), %st(0)\n  ret\n",
// CHECK-NEXT:     options(att_syntax)
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set_ps(
// CHECK-NEXT:     __a: f32,
// CHECK-NEXT:     __b: f32,
// CHECK-NEXT:     __c: f32,
// CHECK-NEXT:     __d: f32,
// CHECK-NEXT:     __e: f32,
// CHECK-NEXT:     __f: f32,
// CHECK-NEXT:     __g: f32,
// CHECK-NEXT:     __h: f32,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] =
// CHECK-NEXT:                 std::simd::Simd::from_array([__h, __g, __f, __e, __d, __c, __b, __a]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_ps(__w: f32) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { _mm256_set_ps(__w, __w, __w, __w, __w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set1_ps(__w: f32) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<f32, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 __w, __w, __w, __w, __w, __w, __w, __w, __w, __w, __w, __w, __w, __w, __w, __w,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set1_pd(__w: f64) -> std::simd::Simd<f64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<f64, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] =
// CHECK-NEXT:                 std::simd::Simd::from_array([__w, __w, __w, __w, __w, __w, __w, __w]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_storeu_ps(__P: *mut (), __A: std::simd::Simd<f32, 16>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((*(__P as *mut __storeu_ps__4291)).__v).write_unaligned(__A);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set_pd(
// CHECK-NEXT:     __A: f64,
// CHECK-NEXT:     __B: f64,
// CHECK-NEXT:     __C: f64,
// CHECK-NEXT:     __D: f64,
// CHECK-NEXT:     __E: f64,
// CHECK-NEXT:     __F: f64,
// CHECK-NEXT:     __G: f64,
// CHECK-NEXT:     __H: f64,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<f64, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] =
// CHECK-NEXT:                 std::simd::Simd::from_array([__H, __G, __F, __E, __D, __C, __B, __A]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn vabi_add8(
// CHECK-NEXT:     [[ARG_0]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     [[ARG_1]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { __vector_abi_vabi_add8([[ARG_0]], [[ARG_1]]) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn __vabi_scale_by16([[ARG_0]]: std::simd::Simd<f32, 16>, [[ARG_1]]: f32) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe { __vector_abi___vabi_scale_by16([[ARG_0]], [[ARG_1]]) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn __vabi_sum8([[ARG_0]]: std::simd::Simd<f64, 8>) -> f64 {
// CHECK-NEXT:     unsafe { __vector_abi___vabi_sum8([[ARG_0]]) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn print_lanes(name: *const i8, lanes: &[f32]) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s:".as_ptr(), name) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0..lanes.len() as i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c" %g".as_ptr(), lanes[i as usize] as f64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if !std::arch::is_x86_feature_detected!("avx512f") {
// CHECK-NEXT:         println!("no avx512f");
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut out4: __SlateAlign16<[f32; 4]> = __SlateAlign16([0 as f32; 4]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_ps(
// CHECK-NEXT:             out4.0.as_mut_ptr() as *mut f32,
// CHECK-NEXT:             vabi_double4(_mm_set_ps(4.0f32, 3.0f32, 2.0f32, 1.0f32)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lanes(c"double4".as_ptr(), &out4.0);
// CHECK-NEXT:     let mut out8: __SlateAlign16<[f32; 8]> = __SlateAlign16([0 as f32; 8]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_storeu_ps(
// CHECK-NEXT:             out8.0.as_mut_ptr() as *mut f32,
// CHECK-NEXT:             vabi_add8(
// CHECK-NEXT:                 _mm256_set1_ps(1.5f32),
// CHECK-NEXT:                 _mm256_set_ps(
// CHECK-NEXT:                     8i32 as f32,
// CHECK-NEXT:                     7i32 as f32,
// CHECK-NEXT:                     6i32 as f32,
// CHECK-NEXT:                     5i32 as f32,
// CHECK-NEXT:                     4i32 as f32,
// CHECK-NEXT:                     3i32 as f32,
// CHECK-NEXT:                     2i32 as f32,
// CHECK-NEXT:                     1i32 as f32,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lanes(c"add8".as_ptr(), &out8.0);
// CHECK-NEXT:     let mut out16: __SlateAlign16<[f32; 16]> = __SlateAlign16([0 as f32; 16]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm512_storeu_ps(
// CHECK-NEXT:             out16.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             __vabi_scale_by16(_mm512_set1_ps(2.0f32), 0.25f32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lanes(c"scale16".as_ptr(), &out16.0);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"sum8: %g\n".as_ptr(),
// CHECK-NEXT:             __vabi_sum8(_mm512_set_pd(
// CHECK-NEXT:                 8i32 as f64,
// CHECK-NEXT:                 7i32 as f64,
// CHECK-NEXT:                 6i32 as f64,
// CHECK-NEXT:                 5i32 as f64,
// CHECK-NEXT:                 4i32 as f64,
// CHECK-NEXT:                 3i32 as f64,
// CHECK-NEXT:                 2i32 as f64,
// CHECK-NEXT:                 1i32 as f64,
// CHECK-NEXT:             )),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_printf__ri32_pi8_f80(
// CHECK-NEXT:             c"sum8 long double: %Lg\n".as_ptr(),
// CHECK-NEXT:             __slate_vabi_ld_double__rf80_f80(__slate_f80_from_f64(__vabi_sum8(_mm512_set1_pd(
// CHECK-NEXT:                 0.5f64,
// CHECK-NEXT:             )))),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
