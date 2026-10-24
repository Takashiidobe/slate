#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ATTRIBUTE_TARGET_POPCNT __attribute__((target("popcnt")))
#define ATTRIBUTE_TARGET_AVX2 __attribute__((target("avx2,fma")))
#define ATTRIBUTE_TARGET_AVX512 __attribute__((target("avx512f, no-sse4a")))

static long long popcount_scalar(const uint64_t *words, size_t count) {
  long long bits = 0;
  for (size_t i = 0; i < count; i++) {
    uint64_t word = words[i];
    while (word) {
      word &= word - 1;
      bits++;
    }
  }
  return bits;
}

ATTRIBUTE_TARGET_POPCNT
static long long popcount_hw(const uint64_t *words, size_t count) {
  long long bits = 0;
  for (size_t i = 0; i < count; i++) bits += __builtin_popcountll(words[i]);
  return bits;
}

static void add_scalar(int32_t *out, const int32_t *a, const int32_t *b, size_t count) {
  for (size_t i = 0; i < count; i++) out[i] = a[i] + b[i];
}

ATTRIBUTE_TARGET_AVX2
static void add_avx2(int32_t *out, const int32_t *a, const int32_t *b, size_t count) {
  size_t i = 0;
  for (; i + 8 <= count; i += 8) {
    __m256i va = _mm256_loadu_si256((const __m256i *)(a + i));
    __m256i vb = _mm256_loadu_si256((const __m256i *)(b + i));
    _mm256_storeu_si256((__m256i *)(out + i), _mm256_add_epi32(va, vb));
  }
  for (; i < count; i++) out[i] = a[i] + b[i];
}

ATTRIBUTE_TARGET_AVX2
static void reverse_bytes_avx2(uint8_t *bytes) {
  __m256i mask = _mm256_setr_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                                  15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
  __m256i v = _mm256_loadu_si256((const __m256i *)bytes);
  _mm256_storeu_si256((__m256i *)bytes, _mm256_shuffle_epi8(v, mask));
}

static void reverse_bytes_scalar(uint8_t *bytes) {
  for (int lane = 0; lane < 32; lane += 16)
    for (int i = 0; i < 8; i++) {
      uint8_t t = bytes[lane + i];
      bytes[lane + i] = bytes[lane + 15 - i];
      bytes[lane + 15 - i] = t;
    }
}

ATTRIBUTE_TARGET_AVX512
static void add_avx512(int32_t *out, const int32_t *a, const int32_t *b, size_t count) {
  size_t i = 0;
  for (; i + 16 <= count; i += 16) {
    __m512i va = _mm512_loadu_si512((const void *)(a + i));
    __m512i vb = _mm512_loadu_si512((const void *)(b + i));
    _mm512_storeu_si512((void *)(out + i), _mm512_add_epi32(va, vb));
  }
  for (; i < count; i++) out[i] = a[i] + b[i];
}

typedef void add_fn(int32_t *, const int32_t *, const int32_t *, size_t);

static add_fn *select_add(void) {
  if (__builtin_cpu_supports("avx512f")) return add_avx512;
  if (__builtin_cpu_supports("avx2") && __builtin_cpu_supports("fma")) return add_avx2;
  return add_scalar;
}

int main(void) {
  __builtin_cpu_init();

  uint64_t words[37];
  for (size_t i = 0; i < 37; i++) words[i] = 0x9e3779b97f4a7c15ull * (i + 1) ^ (i << 40);
  long long bits = __builtin_cpu_supports("popcnt") ? popcount_hw(words, 37)
                                                    : popcount_scalar(words, 37);
  printf("bits %lld %lld\n", bits, popcount_scalar(words, 37));

  int32_t a[45], b[45], out[45];
  for (int i = 0; i < 45; i++) {
    a[i] = i * 7 - 100;
    b[i] = 1000 - i * i;
  }
  add_fn *add = select_add();
  memset(out, 0, sizeof(out));
  add(out, a, b, 45);
  long long sum = 0;
  for (int i = 0; i < 45; i++) sum = sum * 31 + out[i];
  printf("add %lld %d %d\n", sum, out[0], out[44]);

  uint8_t bytes[32];
  for (int i = 0; i < 32; i++) bytes[i] = (uint8_t)(i * 3);
  if (__builtin_cpu_supports("avx2"))
    reverse_bytes_avx2(bytes);
  else
    reverse_bytes_scalar(bytes);
  for (int i = 0; i < 32; i++) printf("%d%c", bytes[i], i == 31 ? '\n' : ' ');
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
// CHECK-NEXT:     fn memset(__s: *mut (), __c: i32, __n: u64) -> *mut ();
// CHECK-NEXT:     fn __builtin_cpu_supports([[V_0:__v[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT:     fn __builtin_cpu_init();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.avx2.pshuf.b"]
// CHECK-NEXT:     fn __slate_intrinsic_43a5f143669e69c1(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: std::simd::Simd<u8, 32>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<u8, 32>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 32>;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
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
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set_epi8(
// CHECK-NEXT:     __b31: i8,
// CHECK-NEXT:     __b30: i8,
// CHECK-NEXT:     __b29: i8,
// CHECK-NEXT:     __b28: i8,
// CHECK-NEXT:     __b27: i8,
// CHECK-NEXT:     __b26: i8,
// CHECK-NEXT:     __b25: i8,
// CHECK-NEXT:     __b24: i8,
// CHECK-NEXT:     __b23: i8,
// CHECK-NEXT:     __b22: i8,
// CHECK-NEXT:     __b21: i8,
// CHECK-NEXT:     __b20: i8,
// CHECK-NEXT:     __b19: i8,
// CHECK-NEXT:     __b18: i8,
// CHECK-NEXT:     __b17: i8,
// CHECK-NEXT:     __b16: i8,
// CHECK-NEXT:     __b15: i8,
// CHECK-NEXT:     __b14: i8,
// CHECK-NEXT:     __b13: i8,
// CHECK-NEXT:     __b12: i8,
// CHECK-NEXT:     __b11: i8,
// CHECK-NEXT:     __b10: i8,
// CHECK-NEXT:     __b09: i8,
// CHECK-NEXT:     __b08: i8,
// CHECK-NEXT:     __b07: i8,
// CHECK-NEXT:     __b06: i8,
// CHECK-NEXT:     __b05: i8,
// CHECK-NEXT:     __b04: i8,
// CHECK-NEXT:     __b03: i8,
// CHECK-NEXT:     __b02: i8,
// CHECK-NEXT:     __b01: i8,
// CHECK-NEXT:     __b00: i8,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i8, 32> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 32>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __b00,
// CHECK-NEXT:                     __b01,
// CHECK-NEXT:                     __b02,
// CHECK-NEXT:                     __b03,
// CHECK-NEXT:                     __b04,
// CHECK-NEXT:                     __b05,
// CHECK-NEXT:                     __b06,
// CHECK-NEXT:                     __b07,
// CHECK-NEXT:                     __b08,
// CHECK-NEXT:                     __b09,
// CHECK-NEXT:                     __b10,
// CHECK-NEXT:                     __b11,
// CHECK-NEXT:                     __b12,
// CHECK-NEXT:                     __b13,
// CHECK-NEXT:                     __b14,
// CHECK-NEXT:                     __b15,
// CHECK-NEXT:                     __b16,
// CHECK-NEXT:                     __b17,
// CHECK-NEXT:                     __b18,
// CHECK-NEXT:                     __b19,
// CHECK-NEXT:                     __b20,
// CHECK-NEXT:                     __b21,
// CHECK-NEXT:                     __b22,
// CHECK-NEXT:                     __b23,
// CHECK-NEXT:                     __b24,
// CHECK-NEXT:                     __b25,
// CHECK-NEXT:                     __b26,
// CHECK-NEXT:                     __b27,
// CHECK-NEXT:                     __b28,
// CHECK-NEXT:                     __b29,
// CHECK-NEXT:                     __b30,
// CHECK-NEXT:                     __b31,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_setr_epi8(
// CHECK-NEXT:     __b31: i8,
// CHECK-NEXT:     __b30: i8,
// CHECK-NEXT:     __b29: i8,
// CHECK-NEXT:     __b28: i8,
// CHECK-NEXT:     __b27: i8,
// CHECK-NEXT:     __b26: i8,
// CHECK-NEXT:     __b25: i8,
// CHECK-NEXT:     __b24: i8,
// CHECK-NEXT:     __b23: i8,
// CHECK-NEXT:     __b22: i8,
// CHECK-NEXT:     __b21: i8,
// CHECK-NEXT:     __b20: i8,
// CHECK-NEXT:     __b19: i8,
// CHECK-NEXT:     __b18: i8,
// CHECK-NEXT:     __b17: i8,
// CHECK-NEXT:     __b16: i8,
// CHECK-NEXT:     __b15: i8,
// CHECK-NEXT:     __b14: i8,
// CHECK-NEXT:     __b13: i8,
// CHECK-NEXT:     __b12: i8,
// CHECK-NEXT:     __b11: i8,
// CHECK-NEXT:     __b10: i8,
// CHECK-NEXT:     __b09: i8,
// CHECK-NEXT:     __b08: i8,
// CHECK-NEXT:     __b07: i8,
// CHECK-NEXT:     __b06: i8,
// CHECK-NEXT:     __b05: i8,
// CHECK-NEXT:     __b04: i8,
// CHECK-NEXT:     __b03: i8,
// CHECK-NEXT:     __b02: i8,
// CHECK-NEXT:     __b01: i8,
// CHECK-NEXT:     __b00: i8,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         _mm256_set_epi8(
// CHECK-NEXT:             __b00,
// CHECK-NEXT:             __b01,
// CHECK-NEXT:             __b02,
// CHECK-NEXT:             __b03,
// CHECK-NEXT:             __b04,
// CHECK-NEXT:             __b05,
// CHECK-NEXT:             __b06,
// CHECK-NEXT:             __b07,
// CHECK-NEXT:             __b08,
// CHECK-NEXT:             __b09,
// CHECK-NEXT:             __b10,
// CHECK-NEXT:             __b11,
// CHECK-NEXT:             __b12,
// CHECK-NEXT:             __b13,
// CHECK-NEXT:             __b14,
// CHECK-NEXT:             __b15,
// CHECK-NEXT:             __b16,
// CHECK-NEXT:             __b17,
// CHECK-NEXT:             __b18,
// CHECK-NEXT:             __b19,
// CHECK-NEXT:             __b20,
// CHECK-NEXT:             __b21,
// CHECK-NEXT:             __b22,
// CHECK-NEXT:             __b23,
// CHECK-NEXT:             __b24,
// CHECK-NEXT:             __b25,
// CHECK-NEXT:             __b26,
// CHECK-NEXT:             __b27,
// CHECK-NEXT:             __b28,
// CHECK-NEXT:             __b29,
// CHECK-NEXT:             __b30,
// CHECK-NEXT:             __b31,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn _mm256_shuffle_epi8(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 32>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 32>,
// CHECK-NEXT:                 std::simd::Simd<i8, 32>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_43a5f143669e69c1(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 32>,
// CHECK-NEXT:                             std::simd::Simd<u8, 32>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i64, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 32>,
// CHECK-NEXT:                             >(__a)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 32>,
// CHECK-NEXT:                             std::simd::Simd<u8, 32>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i64, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 32>,
// CHECK-NEXT:                             >(__b)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_add_epi32(
// CHECK-NEXT:     __A: std::simd::Simd<i64, 8>,
// CHECK-NEXT:     __B: std::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u32, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 8>,
// CHECK-NEXT:                     std::simd::Simd<u32, 16>,
// CHECK-NEXT:                 >(__A)
// CHECK-NEXT:             })
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<u32, 16>,
// CHECK-NEXT:                     >(__B)
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
// CHECK-NEXT: fn popcount_scalar(words: &[u64]) -> i64 {
// CHECK-NEXT:     let mut bits: i64 = 0;
// CHECK-NEXT:     let mut i: u64 = 0;
// CHECK-NEXT:     while i < (words.len() as u64) {
// CHECK-NEXT:         let mut word: u64 = unsafe { words[i as usize] };
// CHECK-NEXT:         while word != 0 {
// CHECK-NEXT:             word &= word.wrapping_sub(1);
// CHECK-NEXT:             bits += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     bits
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "popcnt")]
// CHECK-NEXT: unsafe fn popcount_hw(words: *const u64, count: u64) -> i64 {
// CHECK-NEXT:     let mut bits: i64 = 0;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         bits += (unsafe { *words.add(i as usize) }.count_ones() as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     bits
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add_scalar")]
// CHECK-NEXT: extern "C-unwind" fn add_scalar(
// CHECK-NEXT:     out: *mut i32,
// CHECK-NEXT:     a: *const i32,
// CHECK-NEXT:     b: *const i32,
// CHECK-NEXT:     count: u64,
// CHECK-NEXT: ) {
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *out.add(i as usize) = *a.add(i as usize) + *b.add(i as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2,fma")]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add_avx2")]
// CHECK-NEXT: unsafe extern "C-unwind" fn add_avx2(
// CHECK-NEXT:     out: *mut i32,
// CHECK-NEXT:     a: *const i32,
// CHECK-NEXT:     b: *const i32,
// CHECK-NEXT:     count: u64,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut i: u64 = 0;
// CHECK-NEXT:     while i.wrapping_add(8) <= count {
// CHECK-NEXT:         let va: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:             _mm256_loadu_si256(a.add(i as usize) as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:         };
// CHECK-NEXT:         let vb: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:             _mm256_loadu_si256(b.add(i as usize) as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             _mm256_storeu_si256(
// CHECK-NEXT:                 out.add(i as usize) as *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:                 _mm256_add_epi32(va, vb),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         i = i.wrapping_add(8);
// CHECK-NEXT:     }
// CHECK-NEXT:     while i < count {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *out.add(i as usize) = *a.add(i as usize) + *b.add(i as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2,fma")]
// CHECK-NEXT: unsafe fn reverse_bytes_avx2(bytes: *mut u8) {
// CHECK-NEXT:     let mask: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_setr_epi8(
// CHECK-NEXT:             15i8,
// CHECK-NEXT:             14i8,
// CHECK-NEXT:             13i8,
// CHECK-NEXT:             12i8,
// CHECK-NEXT:             11i8,
// CHECK-NEXT:             10i8,
// CHECK-NEXT:             9i8,
// CHECK-NEXT:             8i8,
// CHECK-NEXT:             7i8,
// CHECK-NEXT:             6i8,
// CHECK-NEXT:             5i8,
// CHECK-NEXT:             4i8,
// CHECK-NEXT:             3i8,
// CHECK-NEXT:             2i8,
// CHECK-NEXT:             1i8,
// CHECK-NEXT:             0i8,
// CHECK-NEXT:             15i8,
// CHECK-NEXT:             14i8,
// CHECK-NEXT:             13i8,
// CHECK-NEXT:             12i8,
// CHECK-NEXT:             11i8,
// CHECK-NEXT:             10i8,
// CHECK-NEXT:             9i8,
// CHECK-NEXT:             8i8,
// CHECK-NEXT:             7i8,
// CHECK-NEXT:             6i8,
// CHECK-NEXT:             5i8,
// CHECK-NEXT:             4i8,
// CHECK-NEXT:             3i8,
// CHECK-NEXT:             2i8,
// CHECK-NEXT:             1i8,
// CHECK-NEXT:             0i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let v: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(bytes as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_storeu_si256(
// CHECK-NEXT:             bytes as *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:             _mm256_shuffle_epi8(v, mask),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn reverse_bytes_scalar(bytes: *mut u8) {
// CHECK-NEXT:     let mut lane: i32 = 0;
// CHECK-NEXT:     while lane < 32 {
// CHECK-NEXT:         for i in 0i32..8i32 {
// CHECK-NEXT:             let t: u8 = unsafe { *bytes.offset((lane + i) as isize) };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *bytes.offset((lane + i) as isize) = *bytes
// CHECK-NEXT:                     .offset((lane + 15 - i) as isize);
// CHECK-NEXT:             }
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *bytes.offset((lane + 15 - i) as isize) = t;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         lane += 16;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.add_avx512")]
// CHECK-NEXT: unsafe extern "C-unwind" fn add_avx512(
// CHECK-NEXT:     out: *mut i32,
// CHECK-NEXT:     a: *const i32,
// CHECK-NEXT:     b: *const i32,
// CHECK-NEXT:     count: u64,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut i: u64 = 0;
// CHECK-NEXT:     while i.wrapping_add(16) <= count {
// CHECK-NEXT:         let va: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:             _mm512_loadu_si512(a.add(i as usize) as *const ())
// CHECK-NEXT:         };
// CHECK-NEXT:         let vb: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:             _mm512_loadu_si512(b.add(i as usize) as *const ())
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             _mm512_storeu_si512(out.add(i as usize) as *mut (), _mm512_add_epi32(va, vb))
// CHECK-NEXT:         };
// CHECK-NEXT:         i = i.wrapping_add(16);
// CHECK-NEXT:     }
// CHECK-NEXT:     while i < count {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *out.add(i as usize) = *a.add(i as usize) + *b.add(i as usize);
// CHECK-NEXT:         }
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn select_add() -> Option<
// CHECK-NEXT:     unsafe extern "C-unwind" fn(*mut i32, *const i32, *const i32, u64),
// CHECK-NEXT: > {
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx512f") {
// CHECK-NEXT:         return Some(add_avx512);
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx2")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("fma")
// CHECK-NEXT:     {
// CHECK-NEXT:         return Some(add_avx2);
// CHECK-NEXT:     }
// CHECK-NEXT:     Some(add_scalar)
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut words: __SlateAlign16<[u64; 37]> = __SlateAlign16([0u64; 37]);
// CHECK-NEXT:     for i in 0u64..37u64 {
// CHECK-NEXT:         words.0[i as usize] = 11400714819323198485u64.wrapping_mul(i.wrapping_add(1))
// CHECK-NEXT:             ^ i << 40i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut bits: i64 = 0;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i64;
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("popcnt") {
// CHECK-NEXT:         [[V_1]] = unsafe { popcount_hw(words.0.as_mut_ptr() as *const u64, 37u64) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_1]] = popcount_scalar(&words.0);
// CHECK-NEXT:     }
// CHECK-NEXT:     bits = [[V_1]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"bits %lld %lld\n".as_ptr(), bits, popcount_scalar(&words.0)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut a: __SlateAlign16<[i32; 45]> = __SlateAlign16([0i32; 45]);
// CHECK-NEXT:     let mut b: __SlateAlign16<[i32; 45]> = __SlateAlign16([0i32; 45]);
// CHECK-NEXT:     let mut out: __SlateAlign16<[i32; 45]> = __SlateAlign16([0i32; 45]);
// CHECK-NEXT:     for i in 0i32..45i32 {
// CHECK-NEXT:         a.0[i as usize] = i * 7 - 100;
// CHECK-NEXT:         b.0[i as usize] = 1000 - i * i;
// CHECK-NEXT:     }
// CHECK-NEXT:     let add: Option<
// CHECK-NEXT:         unsafe extern "C-unwind" fn(*mut i32, *const i32, *const i32, u64),
// CHECK-NEXT:     > = select_add();
// CHECK-NEXT:     unsafe { std::ptr::write_bytes(out.0.as_mut_ptr() as *mut u8, 0u8, 180 as usize) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         add
// CHECK-NEXT:             .unwrap()(
// CHECK-NEXT:             out.0.as_mut_ptr() as *mut i32,
// CHECK-NEXT:             a.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:             b.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:             45u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut sum: i64 = 0;
// CHECK-NEXT:     for i in 0i32..45i32 {
// CHECK-NEXT:         sum = sum * 31 + (out.0[i as usize] as i64);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"add %lld %d %d\n".as_ptr(), sum, out.0[0], out.0[44]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut bytes: __SlateAlign16<[u8; 32]> = __SlateAlign16([0u8; 32]);
// CHECK-NEXT:     for i in 0i32..32i32 {
// CHECK-NEXT:         bytes.0[i as usize] = ((i * 3) as i8) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx2") {
// CHECK-NEXT:         unsafe { reverse_bytes_avx2(bytes.0.as_mut_ptr() as *mut u8) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         reverse_bytes_scalar(bytes.0.as_mut_ptr() as *mut u8);
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..32i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%d%c".as_ptr(),
// CHECK-NEXT:                 (bytes.0[i as usize] as u32) as i32,
// CHECK-NEXT:                 if i == 31 { 10i32 } else { 32i32 },
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
