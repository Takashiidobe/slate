#include <immintrin.h>
#include <stdio.h>
#include <tmmintrin.h>

static void sse2_probe(int out[4]) {
  __m128i a = _mm_set1_epi32(5);
  __m128i b = _mm_set1_epi32(3);
  __m128i c = _mm_add_epi32(a, b);
  _mm_storeu_si128((__m128i *)out, c);
}

__attribute__((target("ssse3"))) static void ssse3_probe(signed char out[16]) {
  __m128i a     = _mm_set1_epi8(-1);
  __m128i idx   = _mm_setzero_si128();
  __m128i abs_a = _mm_abs_epi8(a);
  __m128i shuf  = _mm_shuffle_epi8(abs_a, idx);
  _mm_storeu_si128((__m128i *)out, shuf);
}

__attribute__((target("avx2"))) static void avx2_probe(int out[8]) {
  __m256i a = _mm256_set1_epi32(7);
  __m256i b = _mm256_set1_epi32(2);
  __m256i c = _mm256_add_epi32(a, b);
  _mm256_storeu_si256((__m256i *)out, c);
}

int main(void) {
  int         sse2_out[4];
  signed char ssse3_out[16];
  int         avx2_out[8];

  sse2_probe(sse2_out);
  ssse3_probe(ssse3_out);
  avx2_probe(avx2_out);

  printf("%d %d %d %d\n", sse2_out[0], sse2_out[1], sse2_out[2], sse2_out[3]);
  printf("%d %d\n", ssse3_out[0], ssse3_out[15]);
  printf("%d %d %d %d %d %d %d %d\n", avx2_out[0], avx2_out[1], avx2_out[2],
         avx2_out[3], avx2_out[4], avx2_out[5], avx2_out[6], avx2_out[7]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.ssse3.pshuf.b.128"]
// CHECK-NEXT:     fn __slate_intrinsic_2d3482082e0b2cf1(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 16>;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_si128 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 2>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_si256 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 4>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_add_epi32(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u32, 4>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<u32, 4>,
// CHECK-NEXT:                 >(__a)
// CHECK-NEXT:             })
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<u32, 4>,
// CHECK-NEXT:                     >(__b)
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
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
// CHECK-NEXT: unsafe fn _mm_set_epi8(
// CHECK-NEXT:     __b15: i8,
// CHECK-NEXT:     __b14: i8,
// CHECK-NEXT:     __b13: i8,
// CHECK-NEXT:     __b12: i8,
// CHECK-NEXT:     __b11: i8,
// CHECK-NEXT:     __b10: i8,
// CHECK-NEXT:     __b9: i8,
// CHECK-NEXT:     __b8: i8,
// CHECK-NEXT:     __b7: i8,
// CHECK-NEXT:     __b6: i8,
// CHECK-NEXT:     __b5: i8,
// CHECK-NEXT:     __b4: i8,
// CHECK-NEXT:     __b3: i8,
// CHECK-NEXT:     __b2: i8,
// CHECK-NEXT:     __b1: i8,
// CHECK-NEXT:     __b0: i8,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<i8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __b0,
// CHECK-NEXT:                     __b1,
// CHECK-NEXT:                     __b2,
// CHECK-NEXT:                     __b3,
// CHECK-NEXT:                     __b4,
// CHECK-NEXT:                     __b5,
// CHECK-NEXT:                     __b6,
// CHECK-NEXT:                     __b7,
// CHECK-NEXT:                     __b8,
// CHECK-NEXT:                     __b9,
// CHECK-NEXT:                     __b10,
// CHECK-NEXT:                     __b11,
// CHECK-NEXT:                     __b12,
// CHECK-NEXT:                     __b13,
// CHECK-NEXT:                     __b14,
// CHECK-NEXT:                     __b15,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_set1_epi32(__i: i32) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe { _mm_set_epi32(__i, __i, __i, __i) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_set1_epi8(__b: i8) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         _mm_set_epi8(
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:             __b,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_setzero_si128() -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_2]] = std::simd::Simd::from_array([0i64, 0i64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:         }
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
// CHECK-NEXT: #[target_feature(enable = "ssse3")]
// CHECK-NEXT: unsafe fn _mm_abs_epi8(__a: std::simd::Simd<i64, 2>) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Simd::from_array(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__a)
// CHECK-NEXT:                 }
// CHECK-NEXT:                     .to_array()
// CHECK-NEXT:                     .map(|__lane| __lane.wrapping_abs()),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "ssse3")]
// CHECK-NEXT: unsafe fn _mm_shuffle_epi8(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_2d3482082e0b2cf1(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__a)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__b)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
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
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set_epi32(
// CHECK-NEXT:     __i0: i32,
// CHECK-NEXT:     __i1: i32,
// CHECK-NEXT:     __i2: i32,
// CHECK-NEXT:     __i3: i32,
// CHECK-NEXT:     __i4: i32,
// CHECK-NEXT:     __i5: i32,
// CHECK-NEXT:     __i6: i32,
// CHECK-NEXT:     __i7: i32,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_3]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __i7,
// CHECK-NEXT:                     __i6,
// CHECK-NEXT:                     __i5,
// CHECK-NEXT:                     __i4,
// CHECK-NEXT:                     __i3,
// CHECK-NEXT:                     __i2,
// CHECK-NEXT:                     __i1,
// CHECK-NEXT:                     __i0,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_epi32(__i: i32) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi32(__i, __i, __i, __i, __i, __i, __i, __i) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn _mm256_add_epi32(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u32, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 4>,
// CHECK-NEXT:                     std::simd::Simd<u32, 8>,
// CHECK-NEXT:                 >(__a)
// CHECK-NEXT:             })
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 4>,
// CHECK-NEXT:                         std::simd::Simd<u32, 8>,
// CHECK-NEXT:                     >(__b)
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn sse2_probe(out: *mut i32) {
// CHECK-NEXT:     let a: std::simd::Simd<i64, 2> = unsafe { _mm_set1_epi32(5i32) };
// CHECK-NEXT:     let b: std::simd::Simd<i64, 2> = unsafe { _mm_set1_epi32(3i32) };
// CHECK-NEXT:     let c: std::simd::Simd<i64, 2> = unsafe { _mm_add_epi32(a, b) };
// CHECK-NEXT:     unsafe { _mm_storeu_si128(out as *mut std::simd::Simd<i64, 2>, c) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "ssse3")]
// CHECK-NEXT: unsafe fn ssse3_probe(out: *mut i8) {
// CHECK-NEXT:     let a: std::simd::Simd<i64, 2> = unsafe { _mm_set1_epi8(-1i8) };
// CHECK-NEXT:     let idx: std::simd::Simd<i64, 2> = unsafe { _mm_setzero_si128() };
// CHECK-NEXT:     let abs_a: std::simd::Simd<i64, 2> = unsafe { _mm_abs_epi8(a) };
// CHECK-NEXT:     let shuf: std::simd::Simd<i64, 2> = unsafe { _mm_shuffle_epi8(abs_a, idx) };
// CHECK-NEXT:     unsafe { _mm_storeu_si128(out as *mut std::simd::Simd<i64, 2>, shuf) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn avx2_probe(out: *mut i32) {
// CHECK-NEXT:     let a: std::simd::Simd<i64, 4> = unsafe { _mm256_set1_epi32(7i32) };
// CHECK-NEXT:     let b: std::simd::Simd<i64, 4> = unsafe { _mm256_set1_epi32(2i32) };
// CHECK-NEXT:     let c: std::simd::Simd<i64, 4> = unsafe { _mm256_add_epi32(a, b) };
// CHECK-NEXT:     unsafe { _mm256_storeu_si256(out as *mut std::simd::Simd<i64, 4>, c) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut sse2_out: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32; 4]);
// CHECK-NEXT:     let mut ssse3_out: __SlateAlign16<[i8; 16]> = __SlateAlign16([0i8; 16]);
// CHECK-NEXT:     let mut avx2_out: __SlateAlign16<[i32; 8]> = __SlateAlign16([0i32; 8]);
// CHECK-NEXT:     sse2_probe(sse2_out.0.as_mut_ptr() as *mut i32);
// CHECK-NEXT:     unsafe { ssse3_probe(ssse3_out.0.as_mut_ptr() as *mut i8) };
// CHECK-NEXT:     unsafe { avx2_probe(avx2_out.0.as_mut_ptr() as *mut i32) };
// CHECK-NEXT:     println!("{} {} {} {}", sse2_out.0[0], sse2_out.0[1], sse2_out.0[2], sse2_out.0[3]);
// CHECK-NEXT:     println!("{} {}", ssse3_out.0[0] as i32, ssse3_out.0[15] as i32);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {} {} {}", avx2_out.0[0], avx2_out.0[1], avx2_out.0[2], avx2_out
// CHECK-NEXT:         .0[3], avx2_out.0[4], avx2_out.0[5], avx2_out.0[6], avx2_out.0[7]
// CHECK-NEXT:     );
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
