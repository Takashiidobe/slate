#include <immintrin.h>
#include <stdio.h>

__attribute__((target("avx512f,avx512vl"))) static void
masked_load_probe(int *p, unsigned mask, int *out) {
  __m128i w = _mm_set1_epi32(-1);
  __m128i v = _mm_mask_loadu_epi32(w, (__mmask8)mask, p);
  _mm_storeu_si128((__m128i *)out, v);
}

int main(void) {
  int data[4] = {10, 20, 30, 40};
  int out[4];
  masked_load_probe(data, 0x5u, out);
  printf("%d %d %d %d\n", out[0], out[1], out[2], out[3]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_si128 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 2>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_set_epi32(
// CHECK-NEXT:     __i3: i32,
// CHECK-NEXT:     __i2: i32,
// CHECK-NEXT:     __i1: i32,
// CHECK-NEXT:     __i0: i32,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __i0,
// CHECK-NEXT:                     __i1,
// CHECK-NEXT:                     __i2,
// CHECK-NEXT:                     __i3,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_set1_epi32(__i: i32) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe { _mm_set_epi32(__i, __i, __i, __i) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_storeu_si128(
// CHECK-NEXT:     __p: *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((* (__p as * mut __storeu_si128)).__v)
// CHECK-NEXT:             .write_unaligned(__b);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vl")]
// CHECK-NEXT: unsafe fn _mm_mask_loadu_epi32(
// CHECK-NEXT:     __W: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __U: u8,
// CHECK-NEXT:     __P: *const (),
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 __P as *const i32,
// CHECK-NEXT:                 std::simd::Mask::<i32, 4>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     >(__W)
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f,avx512vl")]
// CHECK-NEXT: unsafe fn masked_load_probe(p: *mut i32, mask: u32, out: *mut i32) {
// CHECK-NEXT:     let w: std::simd::Simd<i64, 2> = unsafe { _mm_set1_epi32(-1i32) };
// CHECK-NEXT:     let v: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_mask_loadu_epi32(w, mask as u8, p as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { _mm_storeu_si128(out as *mut std::simd::Simd<i64, 2>, v) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut data: __SlateAlign16<[i32; 4]> = __SlateAlign16([
// CHECK-NEXT:         10i32, 20i32, 30i32, 40i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut out: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32; 4]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         masked_load_probe(
// CHECK-NEXT:             data.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:             5u32,
// CHECK-NEXT:             out.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{} {} {} {}", out.0[0], out.0[1], out.0[2], out.0[3]);
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
