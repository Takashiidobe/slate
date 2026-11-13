#include <immintrin.h>
#include <stdio.h>

__attribute__((target("avx512f,avx512vl,avx512bw"), noinline)) static void selects(
    unsigned mask, const long long *a, const long long *b, long long *q512, int *d256, short *w128, char *b128) {
  __m512i qa = _mm512_loadu_si512(a);
  __m512i qb = _mm512_loadu_si512(b);
  _mm512_storeu_si512(q512, _mm512_mask_sub_epi64(qa, (__mmask8)mask, qa, qb));
  __m256i da = _mm256_loadu_si256((const __m256i *)a);
  __m256i db = _mm256_loadu_si256((const __m256i *)b);
  _mm256_storeu_si256((__m256i *)d256, _mm256_mask_add_epi32(da, (__mmask8)mask, da, db));
  __m128i wa = _mm_loadu_si128((const __m128i *)a);
  __m128i wb = _mm_loadu_si128((const __m128i *)b);
  _mm_storeu_si128((__m128i *)w128, _mm_mask_add_epi16(wa, (__mmask8)mask, wa, wb));
  _mm_storeu_si128((__m128i *)b128, _mm_maskz_add_epi8(mask, wa, wb));
}

int main(void) {
  long long a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  long long b[8] = {100, 200, 300, 400, 500, 600, 700, 800};
  long long q[8];
  int d[8];
  short w[8];
  char c[16];
  if (!__builtin_cpu_supports("avx512vl") || !__builtin_cpu_supports("avx512bw")) {
    printf("skip\n");
    return 0;
  }
  for (unsigned mask = 0; mask < 256; mask += 37) {
    selects(mask, a, b, q, d, w, c);
    printf("%u:", mask);
    for (int i = 0; i < 8; i++)
      printf(" %lld %d %d", q[i], d[i], w[i]);
    for (int i = 0; i < 16; i++)
      printf(" %d", c[i]);
    printf("\n");
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_cpu_supports([[ARG_0:arg[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
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
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si512 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 8>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_si512 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 8>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_add_epi8(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 >(__a)
// CHECK-NEXT:             })
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     >(__b)
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_add_epi16(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u16, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<u16, 8>,
// CHECK-NEXT:                 >(__a)
// CHECK-NEXT:             })
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<u16, 8>,
// CHECK-NEXT:                     >(__b)
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_loadu_si128(
// CHECK-NEXT:     __p: *const std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_si128)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_setzero_si128() -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([0i64, 0i64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
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
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_sub_epi64(
// CHECK-NEXT:     __A: std::simd::Simd<i64, 8>,
// CHECK-NEXT:     __B: std::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u64, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 8>,
// CHECK-NEXT:                     std::simd::Simd<u64, 8>,
// CHECK-NEXT:                 >(__A)
// CHECK-NEXT:             })
// CHECK-NEXT:                 - unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<u64, 8>,
// CHECK-NEXT:                     >(__B)
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_mask_sub_epi64(
// CHECK-NEXT:     __W: std::simd::Simd<i64, 8>,
// CHECK-NEXT:     __U: u8,
// CHECK-NEXT:     __A: std::simd::Simd<i64, 8>,
// CHECK-NEXT:     __B: std::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return std::simd::Select::select(
// CHECK-NEXT:         std::simd::Mask::<i64, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:         unsafe { _mm512_sub_epi64(__A, __B) },
// CHECK-NEXT:         __W,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_loadu_si512(__P: *const ()) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__P as * const __loadu_si512)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_storeu_si512(__P: *mut (), __A: std::simd::Simd<i64, 8>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((* (__P as * mut __storeu_si512)).__v)
// CHECK-NEXT:             .write_unaligned(__A);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vl")]
// CHECK-NEXT: unsafe fn _mm256_mask_add_epi32(
// CHECK-NEXT:     __W: std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __U: u8,
// CHECK-NEXT:     __A: std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __B: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Select::select(
// CHECK-NEXT:                 std::simd::Mask::<i32, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 4>,
// CHECK-NEXT:                         std::simd::Simd<i32, 8>,
// CHECK-NEXT:                     >(unsafe { _mm256_add_epi32(__A, __B) })
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 4>,
// CHECK-NEXT:                         std::simd::Simd<i32, 8>,
// CHECK-NEXT:                     >(__W)
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vl,avx512bw")]
// CHECK-NEXT: unsafe fn _mm_maskz_add_epi8(
// CHECK-NEXT:     __U: u16,
// CHECK-NEXT:     __A: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __B: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Select::select(
// CHECK-NEXT:                 std::simd::Mask::<i8, 16>::from_bitmask((__U as u16) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(unsafe { _mm_add_epi8(__A, __B) })
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(unsafe { _mm_setzero_si128() })
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vl,avx512bw")]
// CHECK-NEXT: unsafe fn _mm_mask_add_epi16(
// CHECK-NEXT:     __W: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __U: u8,
// CHECK-NEXT:     __A: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __B: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Select::select(
// CHECK-NEXT:                 std::simd::Mask::<i16, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i16, 8>,
// CHECK-NEXT:                     >(unsafe { _mm_add_epi16(__A, __B) })
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i16, 8>,
// CHECK-NEXT:                     >(__W)
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f,avx512vl,avx512bw")]
// CHECK-NEXT: unsafe fn selects(
// CHECK-NEXT:     mask: u32,
// CHECK-NEXT:     a: *const i64,
// CHECK-NEXT:     b: *const i64,
// CHECK-NEXT:     q512: *mut i64,
// CHECK-NEXT:     d256: *mut i32,
// CHECK-NEXT:     w128: *mut i16,
// CHECK-NEXT:     b128: *mut i8,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let qa: std::simd::Simd<i64, 8> = unsafe { _mm512_loadu_si512(a as *const ()) };
// CHECK-NEXT:     let qb: std::simd::Simd<i64, 8> = unsafe { _mm512_loadu_si512(b as *const ()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm512_storeu_si512(
// CHECK-NEXT:             q512 as *mut (),
// CHECK-NEXT:             _mm512_mask_sub_epi64(qa, mask as u8, qa, qb),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let da: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(a as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     let db: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(b as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_storeu_si256(
// CHECK-NEXT:             d256 as *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:             _mm256_mask_add_epi32(da, mask as u8, da, db),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let wa: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_loadu_si128(a as *const std::simd::Simd<i64, 2>)
// CHECK-NEXT:     };
// CHECK-NEXT:     let wb: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_loadu_si128(b as *const std::simd::Simd<i64, 2>)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             w128 as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             _mm_mask_add_epi16(wa, mask as u8, wa, wb),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             b128 as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             _mm_maskz_add_epi8(mask as u16, wa, wb),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i64; 8]> = __SlateAlign16([
// CHECK-NEXT:         1i64, 2i64, 3i64, 4i64, 5i64, 6i64, 7i64, 8i64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut b: __SlateAlign16<[i64; 8]> = __SlateAlign16([
// CHECK-NEXT:         100i64, 200i64, 300i64, 400i64, 500i64, 600i64, 700i64, 800i64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut q: __SlateAlign16<[i64; 8]> = __SlateAlign16([0i64; 8]);
// CHECK-NEXT:     let mut d: __SlateAlign16<[i32; 8]> = __SlateAlign16([0i32; 8]);
// CHECK-NEXT:     let mut w: __SlateAlign16<[i16; 8]> = __SlateAlign16([0i16; 8]);
// CHECK-NEXT:     let mut c: __SlateAlign16<[i8; 16]> = __SlateAlign16([0i8; 16]);
// CHECK-NEXT:     if !std::arch::is_x86_feature_detected!("avx512vl")
// CHECK-NEXT:         || !std::arch::is_x86_feature_detected!("avx512bw")
// CHECK-NEXT:     {
// CHECK-NEXT:         println!("skip");
// CHECK-NEXT:         return 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut mask: u32 = 0;
// CHECK-NEXT:     while mask < 256 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             selects(
// CHECK-NEXT:                 mask,
// CHECK-NEXT:                 a.0.as_mut_ptr() as *const i64,
// CHECK-NEXT:                 b.0.as_mut_ptr() as *const i64,
// CHECK-NEXT:                 q.0.as_mut_ptr() as *mut i64,
// CHECK-NEXT:                 d.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:                 w.0.as_mut_ptr() as *mut i16,
// CHECK-NEXT:                 c.0.as_mut_ptr() as *mut i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c"%u:".as_ptr(), mask) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         for i in 0i32..8i32 {
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 slate_support::printf(
// CHECK-NEXT:                     c" %lld %d %d".as_ptr(),
// CHECK-NEXT:                     q.0[i as usize],
// CHECK-NEXT:                     d.0[i as usize],
// CHECK-NEXT:                     w.0[i as usize] as i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         }
// CHECK-NEXT:         for i in 0i32..16i32 {
// CHECK-NEXT:             print!(" {}", c.0[i as usize] as i32);
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         }
// CHECK-NEXT:         println!("");
// CHECK-NEXT:         mask = mask.wrapping_add(37);
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
