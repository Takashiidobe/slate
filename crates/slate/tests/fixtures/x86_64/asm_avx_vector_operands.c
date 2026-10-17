#include <stdio.h>

typedef float v8sf __attribute__((vector_size(32)));
typedef int v16si __attribute__((vector_size(64)));

static float sqrt_via_x(float value) {
  float result;
  __asm__("sqrtss %1, %0" : "=x"(result) : "x"(value));
  return result;
}

__attribute__((target("avx"))) static float avx_sum(void) {
  v8sf left = {1, 2, 3, 4, 5, 6, 7, 8};
  v8sf right = {10, 20, 30, 40, 50, 60, 70, 80};
  v8sf result;
  __asm__("vaddps %2, %1, %0" : "=x"(result) : "x"(left), "x"(right));
  float sum = 0;
  for (int i = 0; i < 8; i++) {
    sum += result[i];
  }
  return sum;
}

__attribute__((target("avx512f"))) static int avx512_masked_sum(void) {
  v16si left = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  v16si right = {100, 100, 100, 100, 100, 100, 100, 100,
                 100, 100, 100, 100, 100, 100, 100, 100};
  unsigned short mask = 0x00f0;
  v16si result;
  __asm__("vpaddd %2, %1, %0%{%3%}%{z%}"
          : "=v"(result)
          : "v"(left), "v"(right), "Yk"(mask));
  int sum = 0;
  for (int i = 0; i < 16; i++) {
    sum += result[i];
  }
  return sum;
}

__attribute__((target("avx512f"))) static unsigned avx512_equal_lanes(void) {
  v16si left = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  v16si right = {1, 0, 3, 0, 5, 0, 7, 0, 9, 0, 11, 0, 13, 0, 15, 0};
  unsigned short mask;
  __asm__("vpcmpeqd %2, %1, %0" : "=k"(mask) : "v"(left), "v"(right));
  return mask;
}

int main(void) {
  printf("%.1f\n", sqrt_via_x(6.25f));
  if (__builtin_cpu_supports("avx")) {
    printf("%.1f\n", avx_sum());
  }
  if (__builtin_cpu_supports("avx512f")) {
    printf("%d %#x\n", avx512_masked_sum(), avx512_equal_lanes());
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_cpu_supports([[V_0:__v[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn sqrt_via_x(mut value: f32) -> f32 {
// CHECK-NEXT:     let mut result: f32 = 0 as f32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "sqrtss {1}, {0}", lateout(xmm_reg) [[T_0]], in (xmm_reg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< f32, u32 > (value) }, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, f32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn avx_sum() -> f32 {
// CHECK-NEXT:     let mut left: std::simd::Simd<f32, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:         (1 as i32) as f32,
// CHECK-NEXT:         (2 as i32) as f32,
// CHECK-NEXT:         (3 as i32) as f32,
// CHECK-NEXT:         (4 as i32) as f32,
// CHECK-NEXT:         (5 as i32) as f32,
// CHECK-NEXT:         (6 as i32) as f32,
// CHECK-NEXT:         (7 as i32) as f32,
// CHECK-NEXT:         (8 as i32) as f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut right: std::simd::Simd<f32, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:         (10 as i32) as f32,
// CHECK-NEXT:         (20 as i32) as f32,
// CHECK-NEXT:         (30 as i32) as f32,
// CHECK-NEXT:         (40 as i32) as f32,
// CHECK-NEXT:         (50 as i32) as f32,
// CHECK-NEXT:         (60 as i32) as f32,
// CHECK-NEXT:         (70 as i32) as f32,
// CHECK-NEXT:         (80 as i32) as f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut result: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: core::arch::x86_64::__m256i;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "vaddps {2}, {1}, {0}", lateout(ymm_reg) [[T_0]], in (ymm_reg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< std::simd::Simd < f32, 8 >,
// CHECK-NEXT:             core::arch::x86_64::__m256i > (left) }, in (ymm_reg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< std::simd::Simd < f32, 8 >,
// CHECK-NEXT:             core::arch::x86_64::__m256i > (right) }, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 core::arch::x86_64::__m256i,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >([[T_0]])
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sum: f32 = (0 as i32) as f32;
// CHECK-NEXT:     for i in 0 as i32..8 as i32 {
// CHECK-NEXT:         sum += result[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn avx512_masked_sum() -> i32 {
// CHECK-NEXT:     let mut left: std::simd::Simd<i32, 16> = std::simd::Simd::from_array([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:         9 as i32,
// CHECK-NEXT:         10 as i32,
// CHECK-NEXT:         11 as i32,
// CHECK-NEXT:         12 as i32,
// CHECK-NEXT:         13 as i32,
// CHECK-NEXT:         14 as i32,
// CHECK-NEXT:         15 as i32,
// CHECK-NEXT:         16 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut right: std::simd::Simd<i32, 16> = std::simd::Simd::from_array([
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:         100 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut mask: u16 = 240;
// CHECK-NEXT:     let mut result: std::simd::Simd<i32, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: core::arch::x86_64::__m512i;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "vpaddd {2}, {1}, {0}{{[{][{]}}{3}}}{{[{][{]}}z}}", lateout(zmm_reg) [[T_0]], in (zmm_reg)
// CHECK-NEXT:             unsafe { std::mem::transmute::< std::simd::Simd < i32, 16 >,
// CHECK-NEXT:             core::arch::x86_64::__m512i > (left) }, in (zmm_reg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< std::simd::Simd < i32, 16 >,
// CHECK-NEXT:             core::arch::x86_64::__m512i > (right) }, in (kreg) mask as u16, options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 core::arch::x86_64::__m512i,
// CHECK-NEXT:                 std::simd::Simd<i32, 16>,
// CHECK-NEXT:             >([[T_0]])
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut sum: i32 = 0;
// CHECK-NEXT:     for i in 0 as i32..16 as i32 {
// CHECK-NEXT:         sum += result[i as usize];
// CHECK-NEXT:     }
// CHECK-NEXT:     sum
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn avx512_equal_lanes() -> u32 {
// CHECK-NEXT:     let mut left: std::simd::Simd<i32, 16> = std::simd::Simd::from_array([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         2 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         4 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         6 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         8 as i32,
// CHECK-NEXT:         9 as i32,
// CHECK-NEXT:         10 as i32,
// CHECK-NEXT:         11 as i32,
// CHECK-NEXT:         12 as i32,
// CHECK-NEXT:         13 as i32,
// CHECK-NEXT:         14 as i32,
// CHECK-NEXT:         15 as i32,
// CHECK-NEXT:         16 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut right: std::simd::Simd<i32, 16> = std::simd::Simd::from_array([
// CHECK-NEXT:         1 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         3 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         5 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         9 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         11 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         13 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         15 as i32,
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut mask: u16 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u16;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "vpcmpeqd {2}, {1}, {0}", lateout(kreg) [[T_0]], in (zmm_reg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< std::simd::Simd < i32, 16 >,
// CHECK-NEXT:             core::arch::x86_64::__m512i > (left) }, in (zmm_reg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< std::simd::Simd < i32, 16 >,
// CHECK-NEXT:             core::arch::x86_64::__m512i > (right) }, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         mask = unsafe { std::mem::transmute::<u16, u16>([[T_0]] as u16) };
// CHECK-NEXT:     }
// CHECK-NEXT:     mask as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.1f\n".as_ptr(), sqrt_via_x(6.25f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx") {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%.1f\n".as_ptr(), (unsafe { avx_sum() }) as f64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx512f") {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d %#x\n".as_ptr(),
// CHECK-NEXT:                 unsafe { avx512_masked_sum() },
// CHECK-NEXT:                 unsafe { avx512_equal_lanes() },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
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
