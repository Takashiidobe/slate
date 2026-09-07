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
// LOWERING-X86_64-GNU: #![feature(simd_ffi)]
// LOWERING-X86_64-GNU-NEXT: #![feature(portable_simd)]
// LOWERING-X86_64-GNU-NEXT: #![feature(link_llvm_intrinsics)]
// LOWERING-X86_64-GNU-NEXT: #![feature(c_variadic)]
// LOWERING-X86_64-GNU-NEXT: #![allow(
// LOWERING-X86_64-GNU-NEXT:     dead_code,
// LOWERING-X86_64-GNU-NEXT:     unused,
// LOWERING-X86_64-GNU-NEXT:     non_camel_case_types,
// LOWERING-X86_64-GNU-NEXT:     non_snake_case,
// LOWERING-X86_64-GNU-NEXT:     non_upper_case_globals,
// LOWERING-X86_64-GNU-NEXT:     arithmetic_overflow,
// LOWERING-X86_64-GNU-NEXT:     unconditional_panic,
// LOWERING-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-X86_64-GNU-NEXT:     unused_comparisons
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C, packed)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct __storeu_si128 {
// LOWERING-X86_64-GNU-NEXT:     __v: [i64; 2],
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C, packed)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct __storeu_si256 {
// LOWERING-X86_64-GNU-NEXT:     __v: [i64; 4],
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn main() -> std::process::ExitCode {
// LOWERING-X86_64-GNU-NEXT:     let mut sse2_out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     let mut ssse3_out: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-X86_64-GNU-NEXT:     let mut avx2_out: aligned::Aligned<aligned::A16, [i32; 8]> = aligned::Aligned([0; 8]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = sse2_out.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     sse2_probe({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = ssse3_out.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     unsafe { ssse3_probe({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = avx2_out.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe { avx2_probe({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = sse2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = sse2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = sse2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = sse2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = ssse3_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 15;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = ssse3_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 4;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 5;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 6;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 7;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = avx2_out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:         printf(
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     return std::process::ExitCode::SUCCESS;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn sse2_probe({{arg[0-9]+}}: *mut i32) {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_add_epi32({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i64; 2] = {{arg[0-9]+}} as *mut [i64; 2];
// LOWERING-X86_64-GNU-NEXT:     _mm_storeu_si128({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn ssse3_probe({{arg[0-9]+}}: *mut i8) {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = -1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi8({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_setzero_si128();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { _mm_abs_epi8({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { _mm_shuffle_epi8({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i64; 2] = {{arg[0-9]+}} as *mut [i64; 2];
// LOWERING-X86_64-GNU-NEXT:     _mm_storeu_si128({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn avx2_probe({{arg[0-9]+}}: *mut i32) {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_set1_epi32({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_set1_epi32({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_add_epi32({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i64; 4] = {{arg[0-9]+}} as *mut [i64; 4];
// LOWERING-X86_64-GNU-NEXT:     unsafe { _mm256_storeu_si256({{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_add_epi32({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>({{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>({{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 4] = [
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// LOWERING-X86_64-GNU-NEXT:     ];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[u32; 4], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_storeu_si128({{arg[0-9]+}}: *mut [i64; 2], {{arg[0-9]+}}: [i64; 2]) {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut __storeu_si128 = {{arg[0-9]+}} as *mut __storeu_si128;
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).__v) }, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_set_epi32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_set1_epi8({{arg[0-9]+}}: i8) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set_epi8(
// LOWERING-X86_64-GNU-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:     );
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_setzero_si128() -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = [0, 0];
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm_abs_epi8({{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[0usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[1usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[2usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[3usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[4usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[5usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[6usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[7usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[8usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[9usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[10usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[11usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[12usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[13usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[14usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[15usize].wrapping_abs(),
// LOWERING-X86_64-GNU-NEXT:     ];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm_shuffle_epi8({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(
// LOWERING-X86_64-GNU-NEXT:             std::simd::Simd::from_array({{__v[0-9]+}}),
// LOWERING-X86_64-GNU-NEXT:             std::simd::Simd::from_array({{__v[0-9]+}}),
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     .to_array();
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_set_epi8(
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// LOWERING-X86_64-GNU-NEXT: ) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [
// LOWERING-X86_64-GNU-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:     ];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm256_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 4] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] =
// LOWERING-X86_64-GNU-NEXT:         unsafe { _mm256_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm256_add_epi32({{arg[0-9]+}}: [i64; 4], {{arg[0-9]+}}: [i64; 4]) -> [i64; 4] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>({{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>({{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 8] = [
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[4usize] + {{__v[0-9]+}}[4usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[5usize] + {{__v[0-9]+}}[5usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[6usize] + {{__v[0-9]+}}[6usize],
// LOWERING-X86_64-GNU-NEXT:         {{__v[0-9]+}}[7usize] + {{__v[0-9]+}}[7usize],
// LOWERING-X86_64-GNU-NEXT:     ];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { std::mem::transmute::<[u32; 8], [i64; 4]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm256_storeu_si256({{arg[0-9]+}}: *mut [i64; 4], {{arg[0-9]+}}: [i64; 4]) {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut __storeu_si256 = {{arg[0-9]+}} as *mut __storeu_si256;
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).__v) }, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm256_set_epi32(
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-X86_64-GNU-NEXT: ) -> [i64; 4] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 8] = [{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { std::mem::transmute::<[i32; 8], [i64; 4]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "llvm-intrinsic" {
// LOWERING-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.ssse3.pshuf.b.128"]
// LOWERING-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(
// LOWERING-X86_64-GNU-NEXT:         _0: std::simd::Simd<i8, 16>,
// LOWERING-X86_64-GNU-NEXT:         _1: std::simd::Simd<i8, 16>,
// LOWERING-X86_64-GNU-NEXT:     ) -> std::simd::Simd<i8, 16>;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU: #![feature(simd_ffi)]
// REWRITES-X86_64-GNU-NEXT: #![feature(portable_simd)]
// REWRITES-X86_64-GNU-NEXT: #![feature(link_llvm_intrinsics)]
// REWRITES-X86_64-GNU-NEXT: #![feature(c_variadic)]
// REWRITES-X86_64-GNU-NEXT: #![allow(
// REWRITES-X86_64-GNU-NEXT:     dead_code,
// REWRITES-X86_64-GNU-NEXT:     unused,
// REWRITES-X86_64-GNU-NEXT:     non_camel_case_types,
// REWRITES-X86_64-GNU-NEXT:     non_snake_case,
// REWRITES-X86_64-GNU-NEXT:     non_upper_case_globals,
// REWRITES-X86_64-GNU-NEXT:     arithmetic_overflow,
// REWRITES-X86_64-GNU-NEXT:     unconditional_panic,
// REWRITES-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-X86_64-GNU-NEXT:     unused_comparisons
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C, packed)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct __storeu_si128 {
// REWRITES-X86_64-GNU-NEXT:     __v: [i64; 2],
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C, packed)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct __storeu_si256 {
// REWRITES-X86_64-GNU-NEXT:     __v: [i64; 4],
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn main() -> std::process::ExitCode {
// REWRITES-X86_64-GNU-NEXT:     let mut sse2_out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     let mut ssse3_out: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-X86_64-GNU-NEXT:     let mut avx2_out: aligned::Aligned<aligned::A16, [i32; 8]> = aligned::Aligned([0; 8]);
// REWRITES-X86_64-GNU-NEXT:     sse2_probe(sse2_out.as_mut_ptr() as *mut i32);
// REWRITES-X86_64-GNU-NEXT:     unsafe { ssse3_probe(ssse3_out.as_mut_ptr() as *mut i8) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { avx2_probe(avx2_out.as_mut_ptr() as *mut i32) };
// REWRITES-X86_64-GNU-NEXT:     println!(
// REWRITES-X86_64-GNU-NEXT:         "{} {} {} {}",
// REWRITES-X86_64-GNU-NEXT:         sse2_out[0], sse2_out[1], sse2_out[2], sse2_out[3]
// REWRITES-X86_64-GNU-NEXT:     );
// REWRITES-X86_64-GNU-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-X86_64-GNU-NEXT:     println!("{} {}", ssse3_out[0] as i32, ssse3_out[15] as i32);
// REWRITES-X86_64-GNU-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-X86_64-GNU-NEXT:     println!(
// REWRITES-X86_64-GNU-NEXT:         "{} {} {} {} {} {} {} {}",
// REWRITES-X86_64-GNU-NEXT:         avx2_out[0],
// REWRITES-X86_64-GNU-NEXT:         avx2_out[1],
// REWRITES-X86_64-GNU-NEXT:         avx2_out[2],
// REWRITES-X86_64-GNU-NEXT:         avx2_out[3],
// REWRITES-X86_64-GNU-NEXT:         avx2_out[4],
// REWRITES-X86_64-GNU-NEXT:         avx2_out[5],
// REWRITES-X86_64-GNU-NEXT:         avx2_out[6],
// REWRITES-X86_64-GNU-NEXT:         avx2_out[7]
// REWRITES-X86_64-GNU-NEXT:     );
// REWRITES-X86_64-GNU-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-X86_64-GNU-NEXT:     return std::process::ExitCode::SUCCESS;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn sse2_probe({{arg[0-9]+}}: *mut i32) {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32(5);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32(3);
// REWRITES-X86_64-GNU-NEXT:     _mm_storeu_si128({{arg[0-9]+}} as *mut [i64; 2], _mm_add_epi32({{__v[0-9]+}}, {{__v[0-9]+}}));
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn ssse3_probe({{arg[0-9]+}}: *mut i8) {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi8(-1);
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_setzero_si128();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { _mm_abs_epi8({{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     _mm_storeu_si128({{arg[0-9]+}} as *mut [i64; 2], unsafe {
// REWRITES-X86_64-GNU-NEXT:         _mm_shuffle_epi8({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-X86_64-GNU-NEXT:     });
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn avx2_probe({{arg[0-9]+}}: *mut i32) {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_set1_epi32(7) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe { _mm256_set1_epi32(2) };
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         _mm256_storeu_si256({{arg[0-9]+}} as *mut [i64; 4], unsafe {
// REWRITES-X86_64-GNU-NEXT:             _mm256_add_epi32({{__v[0-9]+}}, {{__v[0-9]+}})
// REWRITES-X86_64-GNU-NEXT:         })
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     _mm_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_add_epi32({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>({{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>({{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::mem::transmute::<[u32; 4], [i64; 2]>([
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// REWRITES-X86_64-GNU-NEXT:         ])
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_storeu_si128({{arg[0-9]+}}: *mut [i64; 2], {{arg[0-9]+}}: [i64; 2]) {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut __storeu_si128 = {{arg[0-9]+}} as *mut __storeu_si128;
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).__v) }, {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_set_epi32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] =
// REWRITES-X86_64-GNU-NEXT:         unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>([{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}]) };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_set1_epi8({{arg[0-9]+}}: i8) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     _mm_set_epi8(
// REWRITES-X86_64-GNU-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     )
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_setzero_si128() -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     [0, 0]
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm_abs_epi8({{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = [
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[0usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[1usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[2usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[3usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[4usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[5usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[6usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[7usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[8usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[9usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[10usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[11usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[12usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[13usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[14usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:         {{__v[0-9]+}}[15usize].wrapping_abs(),
// REWRITES-X86_64-GNU-NEXT:     ];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm_shuffle_epi8({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: [i64; 2]) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>({{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 16] = unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(
// REWRITES-X86_64-GNU-NEXT:             std::simd::Simd::from_array({{__v[0-9]+}}),
// REWRITES-X86_64-GNU-NEXT:             std::simd::Simd::from_array({{__v[0-9]+}}),
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     .to_array();
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>({{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_set_epi8(
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i8,
// REWRITES-X86_64-GNU-NEXT: ) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::mem::transmute::<[i8; 16], [i64; 2]>([
// REWRITES-X86_64-GNU-NEXT:             {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:             {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         ])
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm256_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 4] {
// REWRITES-X86_64-GNU-NEXT:     unsafe { _mm256_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm256_add_epi32({{arg[0-9]+}}: [i64; 4], {{arg[0-9]+}}: [i64; 4]) -> [i64; 4] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>({{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>({{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::mem::transmute::<[u32; 8], [i64; 4]>([
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[0usize] + {{__v[0-9]+}}[0usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[1usize] + {{__v[0-9]+}}[1usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[2usize] + {{__v[0-9]+}}[2usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[3usize] + {{__v[0-9]+}}[3usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[4usize] + {{__v[0-9]+}}[4usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[5usize] + {{__v[0-9]+}}[5usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[6usize] + {{__v[0-9]+}}[6usize],
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[7usize] + {{__v[0-9]+}}[7usize],
// REWRITES-X86_64-GNU-NEXT:         ])
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm256_storeu_si256({{arg[0-9]+}}: *mut [i64; 4], {{arg[0-9]+}}: [i64; 4]) {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut __storeu_si256 = {{arg[0-9]+}} as *mut __storeu_si256;
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).__v) }, {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm256_set_epi32(
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-X86_64-GNU-NEXT: ) -> [i64; 4] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 4] = unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::mem::transmute::<[i32; 8], [i64; 4]>([{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}])
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "llvm-intrinsic" {
// REWRITES-X86_64-GNU-NEXT:     #[link_name = "llvm.x86.ssse3.pshuf.b.128"]
// REWRITES-X86_64-GNU-NEXT:     fn __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(
// REWRITES-X86_64-GNU-NEXT:         _0: std::simd::Simd<i8, 16>,
// REWRITES-X86_64-GNU-NEXT:         _1: std::simd::Simd<i8, 16>,
// REWRITES-X86_64-GNU-NEXT:     ) -> std::simd::Simd<i8, 16>;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites
