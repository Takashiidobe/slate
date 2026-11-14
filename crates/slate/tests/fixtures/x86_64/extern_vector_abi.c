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
        ".globl vabi_scale16\n"
        "vabi_scale16:\n"
        "  vbroadcastss %xmm1, %zmm1\n"
        "  vmulps %zmm1, %zmm0, %zmm0\n"
        "  ret\n"
        ".globl vabi_sum8\n"
        "vabi_sum8:\n"
        "  vextractf64x4 $1, %zmm0, %ymm1\n"
        "  vaddpd %ymm1, %ymm0, %ymm0\n"
        "  vextractf128 $1, %ymm0, %xmm1\n"
        "  vaddpd %xmm1, %xmm0, %xmm0\n"
        "  vhaddpd %xmm0, %xmm0, %xmm0\n"
        "  vzeroupper\n"
        "  ret\n");

extern __m128 vabi_double4(__m128 x);
extern __m256 vabi_add8(__m256 a, __m256 b);
extern __m512 vabi_scale16(__m512 x, float k);
extern double vabi_sum8(__m512d x);

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
  _mm512_storeu_ps(out16, vabi_scale16(_mm512_set1_ps(2.0f), 0.25f));
  print_lanes("scale16", out16, 16);

  printf("sum8: %g\n", vabi_sum8(_mm512_set_pd(8, 7, 6, 5, 4, 3, 2, 1)));
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
// CHECK-NEXT:     fn __slate_vabi_vabi_add8(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 8>;
// CHECK-NEXT:     #[link_name = "vabi_scale16"]
// CHECK-NEXT:     fn __slate_vabi_vabi_scale16(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_1]]: f32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 16>;
// CHECK-NEXT:     #[link_name = "vabi_sum8"]
// CHECK-NEXT:     fn __slate_vabi_vabi_sum8([[ARG_0]]: std::simd::Simd<f64, 8>) -> f64;
// CHECK-NEXT:     fn __builtin_cpu_supports([[ARG_0]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
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
// CHECK-NEXT:     ".text\n.globl vabi_double4\nvabi_double4:\n  addps %xmm0, %xmm0\n  ret\n.globl vabi_add8\nvabi_add8:\n  vaddps %ymm1, %ymm0, %ymm0\n  ret\n.globl vabi_scale16\nvabi_scale16:\n  vbroadcastss %xmm1, %zmm1\n  vmulps %zmm1, %zmm0, %zmm0\n  ret\n.globl vabi_sum8\nvabi_sum8:\n  vextractf64x4 $1, %zmm0, %ymm1\n  vaddpd %ymm1, %ymm0, %ymm0\n  vextractf128 $1, %ymm0, %xmm1\n  vaddpd %xmm1, %xmm0, %xmm0\n  vhaddpd %xmm0, %xmm0, %xmm0\n  vzeroupper\n  ret\n",
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
// CHECK-NEXT:     return unsafe { __slate_vabi_vabi_add8([[ARG_0]], [[ARG_1]]) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn vabi_scale16([[ARG_0]]: std::simd::Simd<f32, 16>, [[ARG_1]]: f32) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe { __slate_vabi_vabi_scale16([[ARG_0]], [[ARG_1]]) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn vabi_sum8([[ARG_0]]: std::simd::Simd<f64, 8>) -> f64 {
// CHECK-NEXT:     unsafe { __slate_vabi_vabi_sum8([[ARG_0]]) }
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
// CHECK-NEXT:             vabi_scale16(_mm512_set1_ps(2.0f32), 0.25f32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     print_lanes(c"scale16".as_ptr(), &out16.0);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"sum8: %g\n".as_ptr(),
// CHECK-NEXT:             vabi_sum8(_mm512_set_pd(
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
