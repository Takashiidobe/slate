// { dg-do run { target x86_64-*-* } }
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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(simd_ffi)]
// LOWERING-NEXT: #![feature(portable_simd)]
// LOWERING-NEXT: #![feature(abi_unadjusted)]
// LOWERING-NEXT: #![feature(link_llvm_intrinsics)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __storeu_si128 {
// LOWERING-NEXT:     __v: [i64; 2],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __storeu_si256 {
// LOWERING-NEXT:     __v: [i64; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut sse2_out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut ssse3_out: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut avx2_out: aligned::Aligned<aligned::A16, [i32; 8]> = aligned::Aligned([0; 8]);
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = sse2_out.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     sse2_probe({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = ssse3_out.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { ssse3_probe({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i32 = avx2_out.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     unsafe { avx2_probe({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sse2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sse2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sse2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = sse2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = ssse3_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 15;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = ssse3_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 6;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i64 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:             {{__v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sse2_probe({{arg[0-9]+}}: *mut i32) {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_add_epi32({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut [i64; 2] = {{arg[0-9]+}} as *mut [i64; 2];
// LOWERING-NEXT:     _mm_storeu_si128({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-NEXT: unsafe fn ssse3_probe({{arg[0-9]+}}: *mut i8) {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = -1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi8({{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_setzero_si128();
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { _mm_abs_epi8({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { _mm_shuffle_epi8({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut [i64; 2] = {{arg[0-9]+}} as *mut [i64; 2];
// LOWERING-NEXT:     _mm_storeu_si128({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn avx2_probe({{arg[0-9]+}}: *mut i32) {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_set1_epi32({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_set1_epi32({{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_add_epi32({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut [i64; 4] = {{arg[0-9]+}} as *mut [i64; 4];
// LOWERING-NEXT:     unsafe { _mm256_storeu_si256({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 2] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_add_epi32({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u32; 4] = [
// LOWERING-NEXT:         {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[u32; 4], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_storeu_si128({{arg[0-9]+}}: *mut [i64; 2], {{arg[0-9]+}}: [i64; 2]) {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut __storeu_si128 = {{arg[0-9]+}} as *mut __storeu_si128;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).__v = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set_epi32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> [i64; 2] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}];
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set1_epi8({{arg[0-9]+}}: i8) -> [i64; 2] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set_epi8(
// LOWERING-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// LOWERING-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// LOWERING-NEXT:     );
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_setzero_si128() -> [i64; 2] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = [0, 0];
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-NEXT: unsafe fn _mm_abs_epi8({{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [
// LOWERING-NEXT:         {{__v[0-9]+}}[0usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[1usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[2usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[3usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[4usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[5usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[6usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[7usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[8usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[9usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[10usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[11usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[12usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[13usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[14usize].wrapping_abs(),
// LOWERING-NEXT:         {{__v[0-9]+}}[15usize].wrapping_abs(),
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-NEXT: unsafe fn _mm_shuffle_epi8({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe {
// LOWERING-NEXT:         __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(
// LOWERING-NEXT:             std::simd::Simd::from_array({{__v[0-9]+}}),
// LOWERING-NEXT:             std::simd::Simd::from_array({{__v[0-9]+}}),
// LOWERING-NEXT:         )
// LOWERING-NEXT:     }
// LOWERING-NEXT:     .to_array();
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set_epi8(
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-NEXT: ) -> [i64; 2] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [
// LOWERING-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// LOWERING-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm256_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 4] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 4] =
// LOWERING-NEXT:         unsafe { _mm256_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm256_add_epi32({{arg[0-9]+}}: [i64; 4], {{arg[0-9]+}}: [i64; 4]) -> [i64; 4] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: [u32; 8] = [
// LOWERING-NEXT:         {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[4usize] + {{__v[0-9]+}}[4usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[5usize] + {{__v[0-9]+}}[5usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[6usize] + {{__v[0-9]+}}[6usize],
// LOWERING-NEXT:         {{__v[0-9]+}}[7usize] + {{__v[0-9]+}}[7usize],
// LOWERING-NEXT:     ];
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { std::mem::transmute::<[u32; 8], [i64; 4]>({{__v[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm256_storeu_si256({{arg[0-9]+}}: *mut [i64; 4], {{arg[0-9]+}}: [i64; 4]) {
// LOWERING-NEXT:     let {{__v[0-9]+}}: *mut __storeu_si256 = {{arg[0-9]+}} as *mut __storeu_si256;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{__v[0-9]+}}).__v = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm256_set_epi32(
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT: ) -> [i64; 4] {
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i32; 8] = [{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}];
// LOWERING-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { std::mem::transmute::<[i32; 8], [i64; 4]>({{__v[0-9]+}}) };
// LOWERING-NEXT:     return {{__v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "unadjusted" {
// LOWERING-NEXT:     #[link_name = "llvm.x86.ssse3.pshuf.b.128"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(
// LOWERING-NEXT:         _0: std::simd::Simd<i8, 16>,
// LOWERING-NEXT:         _1: std::simd::Simd<i8, 16>,
// LOWERING-NEXT:     ) -> std::simd::Simd<i8, 16>;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(simd_ffi)]
// REWRITES-NEXT: #![feature(portable_simd)]
// REWRITES-NEXT: #![feature(abi_unadjusted)]
// REWRITES-NEXT: #![feature(link_llvm_intrinsics)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __storeu_si128 {
// REWRITES-NEXT:     __v: [i64; 2],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __storeu_si256 {
// REWRITES-NEXT:     __v: [i64; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut sse2_out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT:     let mut ssse3_out: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT:     let mut avx2_out: aligned::Aligned<aligned::A16, [i32; 8]> = aligned::Aligned([0; 8]);
// REWRITES-NEXT:     sse2_probe(sse2_out.as_mut_ptr() as *mut i32);
// REWRITES-NEXT:     unsafe { ssse3_probe(ssse3_out.as_mut_ptr() as *mut i8) };
// REWRITES-NEXT:     unsafe { avx2_probe(avx2_out.as_mut_ptr() as *mut i32) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             sse2_out[0],
// REWRITES-NEXT:             sse2_out[1],
// REWRITES-NEXT:             sse2_out[2],
// REWRITES-NEXT:             sse2_out[3],
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d\n".as_ptr(),
// REWRITES-NEXT:             ssse3_out[0] as i32,
// REWRITES-NEXT:             ssse3_out[15] as i32,
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             avx2_out[0],
// REWRITES-NEXT:             avx2_out[1],
// REWRITES-NEXT:             avx2_out[2],
// REWRITES-NEXT:             avx2_out[3],
// REWRITES-NEXT:             avx2_out[4],
// REWRITES-NEXT:             avx2_out[5],
// REWRITES-NEXT:             avx2_out[6],
// REWRITES-NEXT:             avx2_out[7],
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sse2_probe({{arg[0-9]+}}: *mut i32) {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32(5);
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32(3);
// REWRITES-NEXT:     _mm_storeu_si128({{arg[0-9]+}} as *mut [i64; 2], _mm_add_epi32({{__v[0-9]+}}, {{__v[0-9]+}}));
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-NEXT: unsafe fn ssse3_probe({{arg[0-9]+}}: *mut i8) {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi8(-1);
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_setzero_si128();
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { _mm_abs_epi8({{__v[0-9]+}}) };
// REWRITES-NEXT:     _mm_storeu_si128({{arg[0-9]+}} as *mut [i64; 2], unsafe {
// REWRITES-NEXT:         _mm_shuffle_epi8({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:     });
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn avx2_probe({{arg[0-9]+}}: *mut i32) {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_set1_epi32(7) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_set1_epi32(2) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         _mm256_storeu_si256({{arg[0-9]+}} as *mut [i64; 4], unsafe {
// REWRITES-NEXT:             _mm256_add_epi32({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 2] {
// REWRITES-NEXT:     _mm_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}})
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_add_epi32({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>({{arg[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>({{arg[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<[u32; 4], [i64; 2]>([
// REWRITES-NEXT:             {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// REWRITES-NEXT:         ])
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_storeu_si128({{arg[0-9]+}}: *mut [i64; 2], {{arg[0-9]+}}: [i64; 2]) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*({{arg[0-9]+}} as *mut __storeu_si128)).__v = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set_epi32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> [i64; 2] {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] =
// REWRITES-NEXT:         unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>([{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}]) };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set1_epi8({{arg[0-9]+}}: i8) -> [i64; 2] {
// REWRITES-NEXT:     _mm_set_epi8(
// REWRITES-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// REWRITES-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// REWRITES-NEXT:     )
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_setzero_si128() -> [i64; 2] {
// REWRITES-NEXT:     [0, 0]
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-NEXT: unsafe fn _mm_abs_epi8({{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [
// REWRITES-NEXT:         {{__v[0-9]+}}[0usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[1usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[2usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[3usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[4usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[5usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[6usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[7usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[8usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[9usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[10usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[11usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[12usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[13usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[14usize].wrapping_abs(),
// REWRITES-NEXT:         {{__v[0-9]+}}[15usize].wrapping_abs(),
// REWRITES-NEXT:     ];
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-NEXT: unsafe fn _mm_shuffle_epi8({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe {
// REWRITES-NEXT:         __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(
// REWRITES-NEXT:             std::simd::Simd::from_array({{__v[0-9]+}}),
// REWRITES-NEXT:             std::simd::Simd::from_array({{__v[0-9]+}}),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     }
// REWRITES-NEXT:     .to_array();
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set_epi8(
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-NEXT: ) -> [i64; 2] {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<[i8; 16], [i64; 2]>([
// REWRITES-NEXT:             {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// REWRITES-NEXT:             {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// REWRITES-NEXT:         ])
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm256_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 4] {
// REWRITES-NEXT:     unsafe { _mm256_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm256_add_epi32({{arg[0-9]+}}: [i64; 4], {{arg[0-9]+}}: [i64; 4]) -> [i64; 4] {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>({{arg[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>({{arg[0-9]+}}) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<[u32; 8], [i64; 4]>([
// REWRITES-NEXT:             {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[4usize] + {{__v[0-9]+}}[4usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[5usize] + {{__v[0-9]+}}[5usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[6usize] + {{__v[0-9]+}}[6usize],
// REWRITES-NEXT:             {{__v[0-9]+}}[7usize] + {{__v[0-9]+}}[7usize],
// REWRITES-NEXT:         ])
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm256_storeu_si256({{arg[0-9]+}}: *mut [i64; 4], {{arg[0-9]+}}: [i64; 4]) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         (*({{arg[0-9]+}} as *mut __storeu_si256)).__v = {{arg[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm256_set_epi32(
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT: ) -> [i64; 4] {
// REWRITES-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe {
// REWRITES-NEXT:         std::mem::transmute::<[i32; 8], [i64; 4]>([{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}])
// REWRITES-NEXT:     };
// REWRITES-NEXT:     {{__v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "unadjusted" {
// REWRITES-NEXT:     #[link_name = "llvm.x86.ssse3.pshuf.b.128"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(
// REWRITES-NEXT:         _0: std::simd::Simd<i8, 16>,
// REWRITES-NEXT:         _1: std::simd::Simd<i8, 16>,
// REWRITES-NEXT:     ) -> std::simd::Simd<i8, 16>;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
