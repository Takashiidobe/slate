#include <immintrin.h>
#include <stdio.h>

__attribute__((noinline)) static __m128i mul_epu32(__m128i a, __m128i b) { return _mm_mul_epu32(a, b); }

__attribute__((target("sse4.1"), noinline)) static __m128i mul_epi32(__m128i a, __m128i b) {
  return _mm_mul_epi32(a, b);
}

__attribute__((target("avx2"), noinline)) static void avx2_products(const int *x, const int *y, unsigned long long *unsigned_out, unsigned long long *signed_out) {
  __m256i a = _mm256_loadu_si256((const __m256i *)x);
  __m256i b = _mm256_loadu_si256((const __m256i *)y);
  _mm256_storeu_si256((__m256i *)unsigned_out, _mm256_mul_epu32(a, b));
  _mm256_storeu_si256((__m256i *)signed_out, _mm256_mul_epi32(a, b));
}

int main(void) {
  unsigned long long out[4], wide_unsigned[4], wide_signed[4];
  __m128i a = _mm_set_epi32(0x7fffffff, (int)0x89abcdef, 7, (int)0xfffffff1);
  __m128i b = _mm_set_epi32(5, (int)0xdeadbeef, 0x12345678, (int)0x80000000);

  _mm_storeu_si128((__m128i *)out, mul_epu32(a, b));
  printf("%llx %llx\n", out[0], out[1]);
  _mm_storeu_si128((__m128i *)out, mul_epi32(a, b));
  printf("%llx %llx\n", out[0], out[1]);

  int x[8] = {(int)0xfffffff1, 7, (int)0x89abcdef, 0x7fffffff, (int)0xffffffff, 3, (int)0xcafebabe, 1};
  int y[8] = {(int)0x80000000, 0x12345678, (int)0xdeadbeef, 5, 0x10000, 9, 0x0badf00d, 2};
  avx2_products(x, y, wide_unsigned, wide_signed);
  printf("%llx %llx %llx %llx\n", wide_unsigned[0], wide_unsigned[1], wide_unsigned[2], wide_unsigned[3]);
  printf("%llx %llx %llx %llx\n", wide_signed[0], wide_signed[1], wide_signed[2], wide_signed[3]);
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
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_mul_epu32(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u64, 2>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             ((unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     >(__a)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }) << std::simd::Simd::splat(32u64) >> std::simd::Simd::splat(32u64))
// CHECK-NEXT:                 * ((unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         std::simd::Simd<u64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(__b)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }) << std::simd::Simd::splat(32u64) >> std::simd::Simd::splat(32u64)),
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
// CHECK-NEXT: unsafe fn _mm_storeu_si128(
// CHECK-NEXT:     __p: *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((* (__p as * mut __storeu_si128)).__v)
// CHECK-NEXT:             .write_unaligned(__b);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse4.1")]
// CHECK-NEXT: unsafe fn _mm_mul_epi32(
// CHECK-NEXT:     __V1: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __V2: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return ((unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i32, 4>>(__V1)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) << std::simd::Simd::splat(32i64) >> std::simd::Simd::splat(32i64))
// CHECK-NEXT:         * ((unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 >(__V2)
// CHECK-NEXT:             })
// CHECK-NEXT:         }) << std::simd::Simd::splat(32i64) >> std::simd::Simd::splat(32i64));
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
// CHECK-NEXT: unsafe fn _mm256_mul_epi32(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return ((unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<i32, 8>>(__a)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) << std::simd::Simd::splat(32i64) >> std::simd::Simd::splat(32i64))
// CHECK-NEXT:         * ((unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<i64, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 4>,
// CHECK-NEXT:                     std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 >(__b)
// CHECK-NEXT:             })
// CHECK-NEXT:         }) << std::simd::Simd::splat(32i64) >> std::simd::Simd::splat(32i64));
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn _mm256_mul_epu32(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u64, 4>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             ((unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 8>,
// CHECK-NEXT:                     std::simd::Simd<u64, 4>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 4>,
// CHECK-NEXT:                         std::simd::Simd<i32, 8>,
// CHECK-NEXT:                     >(__a)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }) << std::simd::Simd::splat(32u64) >> std::simd::Simd::splat(32u64))
// CHECK-NEXT:                 * ((unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i32, 8>,
// CHECK-NEXT:                         std::simd::Simd<u64, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i64, 4>,
// CHECK-NEXT:                             std::simd::Simd<i32, 8>,
// CHECK-NEXT:                         >(__b)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }) << std::simd::Simd::splat(32u64) >> std::simd::Simd::splat(32u64)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn mul_epu32(
// CHECK-NEXT:     a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe { _mm_mul_epu32(a, b) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse4.1")]
// CHECK-NEXT: unsafe fn mul_epi32(
// CHECK-NEXT:     a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe { _mm_mul_epi32(a, b) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn avx2_products(
// CHECK-NEXT:     x: *const i32,
// CHECK-NEXT:     y: *const i32,
// CHECK-NEXT:     unsigned_out: *mut u64,
// CHECK-NEXT:     signed_out: *mut u64,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let a: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(x as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     let b: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(y as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_storeu_si256(
// CHECK-NEXT:             unsigned_out as *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:             _mm256_mul_epu32(a, b),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_storeu_si256(
// CHECK-NEXT:             signed_out as *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:             _mm256_mul_epi32(a, b),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut out: __SlateAlign16<[u64; 4]> = __SlateAlign16([0u64; 4]);
// CHECK-NEXT:     let mut wide_unsigned: __SlateAlign16<[u64; 4]> = __SlateAlign16([0u64; 4]);
// CHECK-NEXT:     let mut wide_signed: __SlateAlign16<[u64; 4]> = __SlateAlign16([0u64; 4]);
// CHECK-NEXT:     let a: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_set_epi32(2147483647i32, -1985229329i32, 7i32, -15i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     let b: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_set_epi32(5i32, -559038737i32, 305419896i32, -2147483648i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             out.0.as_mut_ptr() as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             mul_epu32(a, b),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%llx %llx\n".as_ptr(), out.0[0], out.0[1]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             out.0.as_mut_ptr() as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             mul_epi32(a, b),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%llx %llx\n".as_ptr(), out.0[0], out.0[1]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut x: __SlateAlign16<[i32; 8]> = __SlateAlign16([
// CHECK-NEXT:         -15i32,
// CHECK-NEXT:         7i32,
// CHECK-NEXT:         -1985229329i32,
// CHECK-NEXT:         2147483647i32,
// CHECK-NEXT:         -1i32,
// CHECK-NEXT:         3i32,
// CHECK-NEXT:         -889275714i32,
// CHECK-NEXT:         1i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut y: __SlateAlign16<[i32; 8]> = __SlateAlign16([
// CHECK-NEXT:         -2147483648i32,
// CHECK-NEXT:         305419896i32,
// CHECK-NEXT:         -559038737i32,
// CHECK-NEXT:         5i32,
// CHECK-NEXT:         65536i32,
// CHECK-NEXT:         9i32,
// CHECK-NEXT:         195948557i32,
// CHECK-NEXT:         2i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         avx2_products(
// CHECK-NEXT:             x.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:             y.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:             wide_unsigned.0.as_mut_ptr() as *mut u64,
// CHECK-NEXT:             wide_signed.0.as_mut_ptr() as *mut u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%llx %llx %llx %llx\n".as_ptr(),
// CHECK-NEXT:             wide_unsigned.0[0],
// CHECK-NEXT:             wide_unsigned.0[1],
// CHECK-NEXT:             wide_unsigned.0[2],
// CHECK-NEXT:             wide_unsigned.0[3],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%llx %llx %llx %llx\n".as_ptr(),
// CHECK-NEXT:             wide_signed.0[0],
// CHECK-NEXT:             wide_signed.0[1],
// CHECK-NEXT:             wide_signed.0[2],
// CHECK-NEXT:             wide_signed.0[3],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
