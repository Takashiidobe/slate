#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

typedef int32_t v4si __attribute__((vector_size(16)));
typedef uint32_t v4su __attribute__((vector_size(16)));
typedef int64_t v2di __attribute__((vector_size(16)));
typedef uint8_t v16qu __attribute__((vector_size(16)));

static long long fold(const int64_t *lanes, int count) {
  long long h = 17;
  for (int i = 0; i < count; i++) h = h * 1000003 + lanes[i];
  return h;
}

#define PRINT(label, v, n)                                  \
  do {                                                      \
    int64_t lanes[n];                                       \
    for (int i = 0; i < n; i++) lanes[i] = (int64_t)(v)[i]; \
    printf("%s %lld\n", label, fold(lanes, n));             \
  } while (0)

static int calls;

static v4si counted(v4si v) {
  calls++;
  return v;
}

static void generic(void) {
  v4si a = {7, -3, 1000000, -2147483647};
  v4su u = {7, 0xfffffffdu, 1000000, 0x80000001u};
  v2di d = {0x123456789abcdefll, -0x0fedcba987654321ll};
  printf("reduce %d %d %d %d %d %d %d\n", __builtin_reduce_add(a), __builtin_reduce_mul(a),
         __builtin_reduce_and(a), __builtin_reduce_or(a), __builtin_reduce_xor(a),
         __builtin_reduce_max(a), __builtin_reduce_min(a));
  printf("ureduce %u %u %lld\n", __builtin_reduce_max(u), __builtin_reduce_min(u),
         (long long)__builtin_reduce_add(d));
  PRINT("popcount", __builtin_elementwise_popcount(u), 4);
  PRINT("max", __builtin_elementwise_max(a, (v4si){0, 0, 5, 5}), 4);
  PRINT("umax", __builtin_elementwise_max(u, (v4su){8, 8, 8, 8}), 4);
  PRINT("min", __builtin_elementwise_min(a, (v4si){0, 0, 5, 5}), 4);
  v16qu bytes = {0, 1, 2, 250, 251, 252, 253, 254, 255, 9, 10, 11, 12, 13, 14, 15};
  PRINT("bytemax", __builtin_elementwise_max(bytes, (v16qu){128, 128, 128, 128, 128, 128, 128,
                                                             128, 3, 3, 3, 3, 3, 3, 3, 3}),
        16);
  int once = __builtin_reduce_add(counted(a));
  printf("once %d %d\n", once, calls);
}

__attribute__((target("avx2,fma"))) static void avx2(void) {
  __m256i bytes = _mm256_setr_epi8(1, 200, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                   19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 250);
  __m256i other = _mm256_set1_epi8(16);
  __m256i max = _mm256_max_epu8(bytes, other);
  int64_t out[2];
  _mm_storeu_si128((__m128i *)out, _mm256_extracti128_si256(max, 1));
  printf("hi %lld\n", fold(out, 2));
  _mm_storeu_si128((__m128i *)out, _mm256_extracti128_si256(max, 0));
  printf("lo %lld\n", fold(out, 2));
  __m256 floats = _mm256_setr_ps(1, 2, 3, 4, 5, 6, 7, 8);
  __m256 fused = _mm256_fmadd_ps(floats, floats, _mm256_set1_ps(0.5f));
  float f[4];
  _mm_storeu_ps(f, _mm256_extractf128_ps(fused, 1));
  printf("extractf %g %g %g %g\n", f[0], f[1], f[2], f[3]);
  _mm_storeu_ps(f, _mm256_extractf128_ps(fused, 0));
  printf("extractf %g %g %g %g\n", f[0], f[1], f[2], f[3]);
}

__attribute__((target("avx512f,avx512vpopcntdq"))) static void avx512(void) {
  __m512i words = _mm512_setr_epi64(1, 3, 7, -1, 0x5555, 0, 255, 0x8000000000000000ll);
  printf("popcnt %lld\n", _mm512_reduce_add_epi64(_mm512_popcnt_epi64(words)));
  __m512i ints = _mm512_set1_epi32(0x0f0f0f0f);
  __m512i ones = _mm512_set1_epi32(0x33333333);
  __m512i zero = _mm512_setzero_si512();
  __m512i tern = _mm512_ternarylogic_epi32(ints, ones, zero, 0xEA);
  printf("tern %d\n", _mm512_reduce_add_epi32(tern));
  __m512i seq = _mm512_setr_epi64(10, 20, 30, 40, 50, 60, 70, 80);
  int64_t out[4];
  _mm256_storeu_si256((__m256i *)out, _mm512_extracti64x4_epi64(seq, 1));
  printf("x4 %lld\n", fold(out, 4));
  _mm256_storeu_si256((__m256i *)out, _mm512_extracti64x4_epi64(seq, 0));
  printf("x4 %lld\n", fold(out, 4));
  __m512 floats = _mm512_setr_ps(1e8f, 1, -1e8f, 1, 0.1f, 0.2f, 0.3f, 0.4f, 3, 5e7f, 7, -5e7f,
                                 1e-3f, 2e-3f, 4e-3f, 8e-3f);
  __m512 fused = _mm512_fmadd_ps(floats, _mm512_set1_ps(1.5f), _mm512_set1_ps(0.25f));
  printf("fadd %.9g %.9g\n", _mm512_reduce_add_ps(floats), _mm512_reduce_add_ps(fused));
}

int main(void) {
  __builtin_cpu_init();
  generic();
  if (__builtin_cpu_supports("avx2") && __builtin_cpu_supports("fma"))
    avx2();
  else
    printf("no avx2\n");
  if (__builtin_cpu_supports("avx512f") && __builtin_cpu_supports("avx512vpopcntdq"))
    avx512();
  else
    printf("no avx512\n");
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
// CHECK-NEXT:     fn __builtin_cpu_init();
// CHECK-NEXT:     fn __builtin_cpu_supports([[V_0:__v[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_1d26b5d36a7eb498([[ARG_0:arg[0-9]+]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v8i64"]
// CHECK-NEXT:     fn __slate_intrinsic_2fc6264cf7f2e28c([[ARG_0]]: std::simd::Simd<i64, 8>) -> i64;
// CHECK-NEXT:     #[link_name = "llvm.smin.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_37cf22be76d4abf7(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.umin.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_4596d33813d5b1([[ARG_0]]: std::simd::Simd<u32, 4>) -> u32;
// CHECK-NEXT:     #[link_name = "llvm.umax.v32i8"]
// CHECK-NEXT:     fn __slate_intrinsic_48f54180156adfe0(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<u8, 32>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<u8, 32>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 32>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_4e03cac4e2787ee5([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.umax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_6144ba70f3b1eac4([[ARG_0]]: std::simd::Simd<u32, 4>) -> u32;
// CHECK-NEXT:     #[link_name = "llvm.ctpop.v8i64"]
// CHECK-NEXT:     fn __slate_intrinsic_62bf5f4fa3224c9f(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<u64, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<u64, 8>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.pternlog.d.512"]
// CHECK-NEXT:     fn __slate_intrinsic_8cc195c9fbcd8869(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i32, 16>,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: std::simd::Simd<i32, 16>,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 16>;
// CHECK-NEXT:     #[link_name = "llvm.umax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_909e0aaaa733917e(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<u32, 4>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<u32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.xor.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_99076c0ccee93533([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.fma.v16f32"]
// CHECK-NEXT:     fn __slate_intrinsic_99b14e621b649c16(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 16>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v2i64"]
// CHECK-NEXT:     fn __slate_intrinsic_b914587e1d9172a0([[ARG_0]]: std::simd::Simd<i64, 2>) -> i64;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.and.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_ca4871925d5d9dbf([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.umax.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_d5c5fa99040dca20(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.mul.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e1d90ef8d282d235([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.smax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e3673729f976a086(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smin.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e38776902fc0d037([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.fma.v8f32"]
// CHECK-NEXT:     fn __slate_intrinsic_e5b4413adec356b9(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 8>;
// CHECK-NEXT:     #[link_name = "llvm.ctpop.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_ea593a52181dce5d(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<u32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.or.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_f98f1e94ccaaebe7([[ARG_0]]: std::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v16i32"]
// CHECK-NEXT:     fn __slate_intrinsic_f9af23aef3f58c56([[ARG_0]]: std::simd::Simd<i32, 16>) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_ps {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 4>,
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
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_storeu_ps(__p: *mut f32, __a: std::simd::Simd<f32, 4>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((* (__p as * mut __storeu_ps)).__v).write_unaligned(__a);
// CHECK-NEXT:     }
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
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 8> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 __h,
// CHECK-NEXT:                 __g,
// CHECK-NEXT:                 __f,
// CHECK-NEXT:                 __e,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __c,
// CHECK-NEXT:                 __b,
// CHECK-NEXT:                 __a,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
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
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<i8, 32> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 32>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array([
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
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_setr_ps(
// CHECK-NEXT:     __a: f32,
// CHECK-NEXT:     __b: f32,
// CHECK-NEXT:     __c: f32,
// CHECK-NEXT:     __d: f32,
// CHECK-NEXT:     __e: f32,
// CHECK-NEXT:     __f: f32,
// CHECK-NEXT:     __g: f32,
// CHECK-NEXT:     __h: f32,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { _mm256_set_ps(__h, __g, __f, __e, __d, __c, __b, __a) };
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
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_ps(__w: f32) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { _mm256_set_ps(__w, __w, __w, __w, __w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_epi8(__b: i8) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         _mm256_set_epi8(
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
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_setzero_si256() -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_2]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn _mm256_max_epu8(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u8, 32>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             __slate_intrinsic_48f54180156adfe0(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 4>,
// CHECK-NEXT:                         std::simd::Simd<u8, 32>,
// CHECK-NEXT:                     >(__a)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 4>,
// CHECK-NEXT:                         std::simd::Simd<u8, 32>,
// CHECK-NEXT:                     >(__b)
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "fma")]
// CHECK-NEXT: unsafe fn _mm256_fmadd_ps(
// CHECK-NEXT:     __A: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     __B: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     __C: std::simd::Simd<f32, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_e5b4413adec356b9(__A, __B, __C) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_setzero_si512() -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_3]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:                 0 as i64,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set1_ps(__w: f32) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 16> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_4]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_4]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set1_epi32(__s: i32) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_5:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 16> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_5]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                     __s,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_5]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_fmadd_ps(
// CHECK-NEXT:     __A: std::simd::Simd<f32, 16>,
// CHECK-NEXT:     __B: std::simd::Simd<f32, 16>,
// CHECK-NEXT:     __C: std::simd::Simd<f32, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_99b14e621b649c16(__A, __B, __C) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set_epi64(
// CHECK-NEXT:     __A: i64,
// CHECK-NEXT:     __B: i64,
// CHECK-NEXT:     __C: i64,
// CHECK-NEXT:     __D: i64,
// CHECK-NEXT:     __E: i64,
// CHECK-NEXT:     __F: i64,
// CHECK-NEXT:     __G: i64,
// CHECK-NEXT:     __H: i64,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_6:__slate_compound_[0-9]+]]: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_6]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 __H,
// CHECK-NEXT:                 __G,
// CHECK-NEXT:                 __F,
// CHECK-NEXT:                 __E,
// CHECK-NEXT:                 __D,
// CHECK-NEXT:                 __C,
// CHECK-NEXT:                 __B,
// CHECK-NEXT:                 __A,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_6]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_setr_epi64(
// CHECK-NEXT:     e0: i64,
// CHECK-NEXT:     e1: i64,
// CHECK-NEXT:     e2: i64,
// CHECK-NEXT:     e3: i64,
// CHECK-NEXT:     e4: i64,
// CHECK-NEXT:     e5: i64,
// CHECK-NEXT:     e6: i64,
// CHECK-NEXT:     e7: i64,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe { _mm512_set_epi64(e7, e6, e5, e4, e3, e2, e1, e0) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set_ps(
// CHECK-NEXT:     __A: f32,
// CHECK-NEXT:     __B: f32,
// CHECK-NEXT:     __C: f32,
// CHECK-NEXT:     __D: f32,
// CHECK-NEXT:     __E: f32,
// CHECK-NEXT:     __F: f32,
// CHECK-NEXT:     __G: f32,
// CHECK-NEXT:     __H: f32,
// CHECK-NEXT:     __I: f32,
// CHECK-NEXT:     __J: f32,
// CHECK-NEXT:     __K: f32,
// CHECK-NEXT:     __L: f32,
// CHECK-NEXT:     __M: f32,
// CHECK-NEXT:     __N: f32,
// CHECK-NEXT:     __O: f32,
// CHECK-NEXT:     __P: f32,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_7:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 16> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_7]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 __P,
// CHECK-NEXT:                 __O,
// CHECK-NEXT:                 __N,
// CHECK-NEXT:                 __M,
// CHECK-NEXT:                 __L,
// CHECK-NEXT:                 __K,
// CHECK-NEXT:                 __J,
// CHECK-NEXT:                 __I,
// CHECK-NEXT:                 __H,
// CHECK-NEXT:                 __G,
// CHECK-NEXT:                 __F,
// CHECK-NEXT:                 __E,
// CHECK-NEXT:                 __D,
// CHECK-NEXT:                 __C,
// CHECK-NEXT:                 __B,
// CHECK-NEXT:                 __A,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_7]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_setr_ps(
// CHECK-NEXT:     e0: f32,
// CHECK-NEXT:     e1: f32,
// CHECK-NEXT:     e2: f32,
// CHECK-NEXT:     e3: f32,
// CHECK-NEXT:     e4: f32,
// CHECK-NEXT:     e5: f32,
// CHECK-NEXT:     e6: f32,
// CHECK-NEXT:     e7: f32,
// CHECK-NEXT:     e8: f32,
// CHECK-NEXT:     e9: f32,
// CHECK-NEXT:     e10: f32,
// CHECK-NEXT:     e11: f32,
// CHECK-NEXT:     e12: f32,
// CHECK-NEXT:     e13: f32,
// CHECK-NEXT:     e14: f32,
// CHECK-NEXT:     e15: f32,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         _mm512_set_ps(
// CHECK-NEXT:             e15,
// CHECK-NEXT:             e14,
// CHECK-NEXT:             e13,
// CHECK-NEXT:             e12,
// CHECK-NEXT:             e11,
// CHECK-NEXT:             e10,
// CHECK-NEXT:             e9,
// CHECK-NEXT:             e8,
// CHECK-NEXT:             e7,
// CHECK-NEXT:             e6,
// CHECK-NEXT:             e5,
// CHECK-NEXT:             e4,
// CHECK-NEXT:             e3,
// CHECK-NEXT:             e2,
// CHECK-NEXT:             e1,
// CHECK-NEXT:             e0,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_reduce_add_epi64(__W: std::simd::Simd<i64, 8>) -> i64 {
// CHECK-NEXT:     unsafe { __slate_intrinsic_2fc6264cf7f2e28c(__W) }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_reduce_add_epi32(__W: std::simd::Simd<i64, 8>) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_f9af23aef3f58c56(unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i32, 16>>(__W)
// CHECK-NEXT:         })
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_reduce_add_ps(mut [[V_1:__v[0-9]+]]: std::simd::Simd<f32, 16>) -> f32 {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: std::simd::Simd<f32, 8> = std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_1]], [0, 1, 2, 3, 4, 5, 6, 7]
// CHECK-NEXT:     ) + std::simd::simd_swizzle!([[V_1]], [8, 9, 10, 11, 12, 13, 14, 15]);
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: std::simd::Simd<f32, 4> = std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_2]], [0, 1, 2, 3]
// CHECK-NEXT:     ) + std::simd::simd_swizzle!([[V_2]], [4, 5, 6, 7]);
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: std::simd::Simd<f32, 2> = std::simd::simd_swizzle!([[V_3]], [0, 1])
// CHECK-NEXT:         + std::simd::simd_swizzle!([[V_3]], [2, 3]);
// CHECK-NEXT:     -0.0f32
// CHECK-NEXT:         + (std::simd::simd_swizzle!([[V_4]], [0])
// CHECK-NEXT:             + std::simd::simd_swizzle!([[V_4]], [1]))[0]
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vpopcntdq")]
// CHECK-NEXT: unsafe fn _mm512_popcnt_epi64(__A: std::simd::Simd<i64, 8>) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<u64, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             __slate_intrinsic_62bf5f4fa3224c9f(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 8>,
// CHECK-NEXT:                     std::simd::Simd<u64, 8>,
// CHECK-NEXT:                 >(__A)
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: fn fold(lanes: *const i64, count: i32) -> i64 {
// CHECK-NEXT:     let __lanes_view = unsafe {
// CHECK-NEXT:         std::slice::from_raw_parts(lanes as *const i64, count as usize)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut h: i64 = 17;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         h = h * 1000003 + unsafe { __lanes_view[i as usize] };
// CHECK-NEXT:     }
// CHECK-NEXT:     h
// CHECK-NEXT: }
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn counted(v: std::simd::Simd<i32, 4>) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     return v;
// CHECK-NEXT: }
// CHECK-NEXT: fn generic() {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_8:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_9:__slate_compound_[0-9]+]]: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_10:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_11:__slate_compound_[0-9]+]]: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let a: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         7 as i32,
// CHECK-NEXT:         -(3 as i32),
// CHECK-NEXT:         1000000 as i32,
// CHECK-NEXT:         -(2147483647 as i32),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let u: std::simd::Simd<u32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         7 as u32,
// CHECK-NEXT:         4294967293 as u32,
// CHECK-NEXT:         1000000 as u32,
// CHECK-NEXT:         2147483649 as u32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let d: std::simd::Simd<i64, 2> = std::simd::Simd::from_array([
// CHECK-NEXT:         81985529216486895 as i64,
// CHECK-NEXT:         -(1147797409030816545 as i64),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "reduce {} {} {} {} {} {} {}", unsafe { __slate_intrinsic_1d26b5d36a7eb498(a) },
// CHECK-NEXT:         unsafe { __slate_intrinsic_e1d90ef8d282d235(a) }, unsafe {
// CHECK-NEXT:         __slate_intrinsic_ca4871925d5d9dbf(a) }, unsafe {
// CHECK-NEXT:         __slate_intrinsic_f98f1e94ccaaebe7(a) }, unsafe {
// CHECK-NEXT:         __slate_intrinsic_99076c0ccee93533(a) }, unsafe {
// CHECK-NEXT:         __slate_intrinsic_4e03cac4e2787ee5(a) }, unsafe {
// CHECK-NEXT:         __slate_intrinsic_e38776902fc0d037(a) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"ureduce %u %u %lld\n".as_ptr(),
// CHECK-NEXT:             __slate_intrinsic_6144ba70f3b1eac4(u),
// CHECK-NEXT:             __slate_intrinsic_4596d33813d5b1(u),
// CHECK-NEXT:             __slate_intrinsic_b914587e1d9172a0(d),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0 as i64; 4]);
// CHECK-NEXT:         for i in 0 as i32..4 as i32 {
// CHECK-NEXT:             lanes.0[i as usize] = (unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ea593a52181dce5d(u)
// CHECK-NEXT:             }[i as usize] as u64) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"popcount".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4 as i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0 as i64; 4]);
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < 4 {
// CHECK-NEXT:             lanes.0[i as usize] = unsafe {
// CHECK-NEXT:                 __slate_intrinsic_e3673729f976a086(
// CHECK-NEXT:                     a,
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_8]] = std::simd::Simd::from_array([
// CHECK-NEXT:                             0 as i32,
// CHECK-NEXT:                             0 as i32,
// CHECK-NEXT:                             5 as i32,
// CHECK-NEXT:                             5 as i32,
// CHECK-NEXT:                         ]);
// CHECK-NEXT:                         std::ptr::addr_of_mut!([[SLATE_COMPOUND_8]])
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             }[i as usize] as i64;
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"max".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4 as i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0 as i64; 4]);
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: while i < 4 {
// CHECK-NEXT:             lanes.0[i as usize] = (unsafe {
// CHECK-NEXT:                 __slate_intrinsic_909e0aaaa733917e(
// CHECK-NEXT:                     u,
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_9]] = std::simd::Simd::from_array([
// CHECK-NEXT:                             8 as u32,
// CHECK-NEXT:                             8 as u32,
// CHECK-NEXT:                             8 as u32,
// CHECK-NEXT:                             8 as u32,
// CHECK-NEXT:                         ]);
// CHECK-NEXT:                         std::ptr::addr_of_mut!([[SLATE_COMPOUND_9]])
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             }[i as usize] as u64) as i64;
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"umax".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4 as i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0 as i64; 4]);
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: while i < 4 {
// CHECK-NEXT:             lanes.0[i as usize] = unsafe {
// CHECK-NEXT:                 __slate_intrinsic_37cf22be76d4abf7(
// CHECK-NEXT:                     a,
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_10]] = std::simd::Simd::from_array([
// CHECK-NEXT:                             0 as i32,
// CHECK-NEXT:                             0 as i32,
// CHECK-NEXT:                             5 as i32,
// CHECK-NEXT:                             5 as i32,
// CHECK-NEXT:                         ]);
// CHECK-NEXT:                         std::ptr::addr_of_mut!([[SLATE_COMPOUND_10]])
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             }[i as usize] as i64;
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"min".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4 as i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let bytes: std::simd::Simd<u8, 16> = std::simd::Simd::from_array([
// CHECK-NEXT:         0 as u8,
// CHECK-NEXT:         1 as u8,
// CHECK-NEXT:         2 as u8,
// CHECK-NEXT:         ((250 as i32) as i8) as u8,
// CHECK-NEXT:         ((251 as i32) as i8) as u8,
// CHECK-NEXT:         ((252 as i32) as i8) as u8,
// CHECK-NEXT:         ((253 as i32) as i8) as u8,
// CHECK-NEXT:         ((254 as i32) as i8) as u8,
// CHECK-NEXT:         ((255 as i32) as i8) as u8,
// CHECK-NEXT:         9 as u8,
// CHECK-NEXT:         10 as u8,
// CHECK-NEXT:         11 as u8,
// CHECK-NEXT:         12 as u8,
// CHECK-NEXT:         13 as u8,
// CHECK-NEXT:         14 as u8,
// CHECK-NEXT:         15 as u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 16]> = __SlateAlign16([0 as i64; 16]);
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: while i < 16 {
// CHECK-NEXT:             lanes.0[i as usize] = (unsafe {
// CHECK-NEXT:                 __slate_intrinsic_d5c5fa99040dca20(
// CHECK-NEXT:                     bytes,
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_11]] = std::simd::Simd::from_array([
// CHECK-NEXT:                             ((128 as i32) as i8) as u8,
// CHECK-NEXT:                             ((128 as i32) as i8) as u8,
// CHECK-NEXT:                             ((128 as i32) as i8) as u8,
// CHECK-NEXT:                             ((128 as i32) as i8) as u8,
// CHECK-NEXT:                             ((128 as i32) as i8) as u8,
// CHECK-NEXT:                             ((128 as i32) as i8) as u8,
// CHECK-NEXT:                             ((128 as i32) as i8) as u8,
// CHECK-NEXT:                             ((128 as i32) as i8) as u8,
// CHECK-NEXT:                             3 as u8,
// CHECK-NEXT:                             3 as u8,
// CHECK-NEXT:                             3 as u8,
// CHECK-NEXT:                             3 as u8,
// CHECK-NEXT:                             3 as u8,
// CHECK-NEXT:                             3 as u8,
// CHECK-NEXT:                             3 as u8,
// CHECK-NEXT:                             3 as u8,
// CHECK-NEXT:                         ]);
// CHECK-NEXT:                         std::ptr::addr_of_mut!([[SLATE_COMPOUND_11]])
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             }[i as usize] as u64) as i64;
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"bytemax".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 16 as i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let once: i32 = unsafe { __slate_intrinsic_1d26b5d36a7eb498(counted(a)) };
// CHECK-NEXT:     println!("once {} {}", once, unsafe { calls });
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2,fma")]
// CHECK-NEXT: unsafe fn avx2() {
// CHECK-NEXT:     let bytes: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_setr_epi8(
// CHECK-NEXT:             1 as i8,
// CHECK-NEXT:             (200 as i32) as i8,
// CHECK-NEXT:             3 as i8,
// CHECK-NEXT:             4 as i8,
// CHECK-NEXT:             5 as i8,
// CHECK-NEXT:             6 as i8,
// CHECK-NEXT:             7 as i8,
// CHECK-NEXT:             8 as i8,
// CHECK-NEXT:             9 as i8,
// CHECK-NEXT:             10 as i8,
// CHECK-NEXT:             11 as i8,
// CHECK-NEXT:             12 as i8,
// CHECK-NEXT:             13 as i8,
// CHECK-NEXT:             14 as i8,
// CHECK-NEXT:             15 as i8,
// CHECK-NEXT:             16 as i8,
// CHECK-NEXT:             17 as i8,
// CHECK-NEXT:             18 as i8,
// CHECK-NEXT:             19 as i8,
// CHECK-NEXT:             20 as i8,
// CHECK-NEXT:             21 as i8,
// CHECK-NEXT:             22 as i8,
// CHECK-NEXT:             23 as i8,
// CHECK-NEXT:             24 as i8,
// CHECK-NEXT:             25 as i8,
// CHECK-NEXT:             26 as i8,
// CHECK-NEXT:             27 as i8,
// CHECK-NEXT:             28 as i8,
// CHECK-NEXT:             29 as i8,
// CHECK-NEXT:             30 as i8,
// CHECK-NEXT:             31 as i8,
// CHECK-NEXT:             (250 as i32) as i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let other: std::simd::Simd<i64, 4> = unsafe { _mm256_set1_epi8(16 as i8) };
// CHECK-NEXT:     let mut max: std::simd::Simd<i64, 4> = unsafe { _mm256_max_epu8(bytes, other) };
// CHECK-NEXT:     let mut out: __SlateAlign16<[i64; 2]> = __SlateAlign16([0 as i64; 2]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             out.0.as_mut_ptr() as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             std::simd::simd_swizzle!(max, [2, 3]),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"hi %lld\n".as_ptr(), fold(out.0.as_mut_ptr() as *const i64, 2 as i32))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_si128(
// CHECK-NEXT:             out.0.as_mut_ptr() as *mut std::simd::Simd<i64, 2>,
// CHECK-NEXT:             std::simd::simd_swizzle!(max, [0, 1]),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"lo %lld\n".as_ptr(), fold(out.0.as_mut_ptr() as *const i64, 2 as i32))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let floats: std::simd::Simd<f32, 8> = unsafe {
// CHECK-NEXT:         _mm256_setr_ps(
// CHECK-NEXT:             (1 as i32) as f32,
// CHECK-NEXT:             (2 as i32) as f32,
// CHECK-NEXT:             (3 as i32) as f32,
// CHECK-NEXT:             (4 as i32) as f32,
// CHECK-NEXT:             (5 as i32) as f32,
// CHECK-NEXT:             (6 as i32) as f32,
// CHECK-NEXT:             (7 as i32) as f32,
// CHECK-NEXT:             (8 as i32) as f32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut fused: std::simd::Simd<f32, 8> = unsafe {
// CHECK-NEXT:         _mm256_fmadd_ps(floats, floats, _mm256_set1_ps(0.5f32))
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut f: __SlateAlign16<[f32; 4]> = __SlateAlign16([0 as f32; 4]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_ps(
// CHECK-NEXT:             f.0.as_mut_ptr() as *mut f32,
// CHECK-NEXT:             std::simd::simd_swizzle!(fused, [4, 5, 6, 7]),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"extractf %g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             f.0[0] as f64,
// CHECK-NEXT:             f.0[1] as f64,
// CHECK-NEXT:             f.0[2] as f64,
// CHECK-NEXT:             f.0[3] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm_storeu_ps(
// CHECK-NEXT:             f.0.as_mut_ptr() as *mut f32,
// CHECK-NEXT:             std::simd::simd_swizzle!(fused, [0, 1, 2, 3]),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"extractf %g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             f.0[0] as f64,
// CHECK-NEXT:             f.0[1] as f64,
// CHECK-NEXT:             f.0[2] as f64,
// CHECK-NEXT:             f.0[3] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f,avx512vpopcntdq")]
// CHECK-NEXT: unsafe fn avx512() {
// CHECK-NEXT:     let words: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         _mm512_setr_epi64(
// CHECK-NEXT:             1 as i64,
// CHECK-NEXT:             3 as i64,
// CHECK-NEXT:             7 as i64,
// CHECK-NEXT:             -1 as i64,
// CHECK-NEXT:             21845 as i64,
// CHECK-NEXT:             0 as i64,
// CHECK-NEXT:             255 as i64,
// CHECK-NEXT:             (9223372036854775808 as u64) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"popcnt %lld\n".as_ptr(),
// CHECK-NEXT:             _mm512_reduce_add_epi64(_mm512_popcnt_epi64(words)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let ints: std::simd::Simd<i64, 8> = unsafe { _mm512_set1_epi32(252645135 as i32) };
// CHECK-NEXT:     let ones: std::simd::Simd<i64, 8> = unsafe { _mm512_set1_epi32(858993459 as i32) };
// CHECK-NEXT:     let zero: std::simd::Simd<i64, 8> = unsafe { _mm512_setzero_si512() };
// CHECK-NEXT:     let tern: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             __slate_intrinsic_8cc195c9fbcd8869(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 16>,
// CHECK-NEXT:                     >(ints)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 16>,
// CHECK-NEXT:                     >(ones)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 16>,
// CHECK-NEXT:                     >(zero)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 const { 234 as i32 },
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("tern {}", unsafe { _mm512_reduce_add_epi32(tern) });
// CHECK-NEXT:     let mut seq: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         _mm512_setr_epi64(
// CHECK-NEXT:             10 as i64,
// CHECK-NEXT:             20 as i64,
// CHECK-NEXT:             30 as i64,
// CHECK-NEXT:             40 as i64,
// CHECK-NEXT:             50 as i64,
// CHECK-NEXT:             60 as i64,
// CHECK-NEXT:             70 as i64,
// CHECK-NEXT:             80 as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut out: __SlateAlign16<[i64; 4]> = __SlateAlign16([0 as i64; 4]);
// CHECK-NEXT:     unsafe { _mm256_setzero_si256() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_storeu_si256(
// CHECK-NEXT:             out.0.as_mut_ptr() as *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:             std::simd::simd_swizzle!(seq, [4, 5, 6, 7]),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"x4 %lld\n".as_ptr(), fold(out.0.as_mut_ptr() as *const i64, 4 as i32))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { _mm256_setzero_si256() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_storeu_si256(
// CHECK-NEXT:             out.0.as_mut_ptr() as *mut std::simd::Simd<i64, 4>,
// CHECK-NEXT:             std::simd::simd_swizzle!(seq, [0, 1, 2, 3]),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"x4 %lld\n".as_ptr(), fold(out.0.as_mut_ptr() as *const i64, 4 as i32))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let floats: std::simd::Simd<f32, 16> = unsafe {
// CHECK-NEXT:         _mm512_setr_ps(
// CHECK-NEXT:             100000000.0f32,
// CHECK-NEXT:             (1 as i32) as f32,
// CHECK-NEXT:             -100000000.0f32,
// CHECK-NEXT:             (1 as i32) as f32,
// CHECK-NEXT:             0.1f32,
// CHECK-NEXT:             0.2f32,
// CHECK-NEXT:             0.3f32,
// CHECK-NEXT:             0.4f32,
// CHECK-NEXT:             (3 as i32) as f32,
// CHECK-NEXT:             50000000.0f32,
// CHECK-NEXT:             (7 as i32) as f32,
// CHECK-NEXT:             -50000000.0f32,
// CHECK-NEXT:             0.001f32,
// CHECK-NEXT:             0.002f32,
// CHECK-NEXT:             0.004f32,
// CHECK-NEXT:             0.008f32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let fused: std::simd::Simd<f32, 16> = unsafe {
// CHECK-NEXT:         _mm512_fmadd_ps(floats, _mm512_set1_ps(1.5f32), _mm512_set1_ps(0.25f32))
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"fadd %.9g %.9g\n".as_ptr(),
// CHECK-NEXT:             _mm512_reduce_add_ps(floats) as f64,
// CHECK-NEXT:             _mm512_reduce_add_ps(fused) as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     generic();
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx2")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("fma")
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { avx2() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("no avx2");
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx512f")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("avx512vpopcntdq")
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { avx512() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!("no avx512");
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
