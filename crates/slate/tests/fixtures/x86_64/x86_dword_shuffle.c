#include <immintrin.h>
#include <stdio.h>

__attribute__((noinline)) static void sse2_shuffles(const int *in, int *out) {
  __m128i v = _mm_loadu_si128((const __m128i *)in);
  _mm_storeu_si128((__m128i *)out, _mm_shuffle_epi32(v, _MM_SHUFFLE(0, 1, 2, 3)));
  _mm_storeu_si128((__m128i *)(out + 4), _mm_shuffle_epi32(v, _MM_SHUFFLE(3, 3, 0, 0)));
  _mm_storeu_si128((__m128i *)(out + 8), _mm_shuffle_epi32(v, 0x31));
}

__attribute__((target("avx2"), noinline)) static void avx2_shuffle(const int *in, int *out) {
  __m256i v = _mm256_loadu_si256((const __m256i *)in);
  _mm256_storeu_si256((__m256i *)out, _mm256_shuffle_epi32(v, _MM_SHUFFLE(1, 0, 3, 2)));
}

int main(void) {
  int in[8] = {10, 20, 30, 40, 50, 60, 70, 80};
  int sse[12], avx[8];
  sse2_shuffles(in, sse);
  avx2_shuffle(in, avx);
  for (int i = 0; i < 12; i++)
    printf("%d ", sse[i]);
  printf("\n");
  for (int i = 0; i < 8; i++)
    printf("%d ", avx[i]);
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si128 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 2>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_si128 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 2>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si256 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 4>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_si256 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 4>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_loadu_si128(
// CHECK-NEXT:     __p: *const std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_si128)).__v).read_unaligned()
// CHECK-NEXT:     };
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
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_loadu_si256(
// CHECK-NEXT:     __p: *const std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_si256)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_storeu_si256(
// CHECK-NEXT:     __p: *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((* (__p as * mut __storeu_si256)).__v)
// CHECK-NEXT:             .write_unaligned(__a);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn sse2_shuffles(r#in: *const i32, out: *mut i32) {
// CHECK-NEXT:     let v: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_loadu_si128(r#in as *const std::simd::Simd<i64, 2>)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             out as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >,
// CHECK-NEXT:                         std::simd::Simd < i32, 4 >> (v) }, [3, 2, 1, 0]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             out.add(4) as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >,
// CHECK-NEXT:                         std::simd::Simd < i32, 4 >> (v) }, [0, 0, 3, 3]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             out.add(8) as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >,
// CHECK-NEXT:                         std::simd::Simd < i32, 4 >> (v) }, [1, 0, 3, 0]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn avx2_shuffle(r#in: *const i32, out: *mut i32) {
// CHECK-NEXT:     let v: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(r#in as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_storeu_si256(
// CHECK-NEXT:             out as *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 8>,
// CHECK-NEXT:                     std::simd::Simd<i64, 4>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i64, 4 >,
// CHECK-NEXT:                         std::simd::Simd < i32, 8 >> (v) }, [2, 3, 0, 1, 6, 7, 4, 5]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r#in: __SlateAlign16<[i32; 8]> = __SlateAlign16([
// CHECK-NEXT:         10i32, 20i32, 30i32, 40i32, 50i32, 60i32, 70i32, 80i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut sse: __SlateAlign16<[i32; 12]> = __SlateAlign16([0i32; 12]);
// CHECK-NEXT:     let mut avx: __SlateAlign16<[i32; 8]> = __SlateAlign16([0i32; 8]);
// CHECK-NEXT:     sse2_shuffles(r#in.0.as_mut_ptr() as *const i32, sse.0.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         avx2_shuffle(r#in.0.as_mut_ptr() as *const i32, avx.0.as_mut_ptr() as *mut i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     for i in 0i32..12i32 {
// CHECK-NEXT:         print!("{} ", sse.0[i as usize]);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         print!("{} ", avx.0[i as usize]);
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
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
