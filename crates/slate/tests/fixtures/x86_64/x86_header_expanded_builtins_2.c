#include <immintrin.h>
#include <math.h>
#include <stdio.h>

static void show(const char *name, const void *data, int bytes) {
  const unsigned char *p = data;
  unsigned hash = 2166136261u;
  for (int i = 0; i < bytes; i++)
    hash = (hash ^ p[i]) * 16777619u;
  printf("%s %08x\n", name, hash);
}

#define SHOW(name, v) do { __typeof__(v) t_ = (v); show(name, &t_, sizeof t_); } while (0)

__attribute__((target("sse4.1"), noinline)) static void sse(const float *fa, const float *fb, const double *da,
                                                           const signed char *bytes) {
  __m128 a = _mm_loadu_ps(fa), b = _mm_loadu_ps(fb);
  __m128d c = _mm_loadu_pd(da);
  __m128i x = _mm_loadu_si128((const __m128i *)bytes), y = _mm_loadu_si128((const __m128i *)(bytes + 16));
  SHOW("shuffle_ps", _mm_shuffle_ps(a, b, 0x1b));
  SHOW("shuffle_ps2", _mm_shuffle_ps(a, b, 0xe4));
  SHOW("shuffle_pd", _mm_shuffle_pd(c, _mm_set1_pd(5.5), 0x1));
  SHOW("alignr0", _mm_alignr_epi8(x, y, 0));
  SHOW("alignr5", _mm_alignr_epi8(x, y, 5));
  SHOW("alignr16", _mm_alignr_epi8(x, y, 16));
  SHOW("alignr20", _mm_alignr_epi8(x, y, 20));
  SHOW("alignr32", _mm_alignr_epi8(x, y, 32));
  SHOW("round_near", _mm_round_ps(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
  SHOW("round_floor", _mm_floor_ps(a));
  SHOW("round_ceil", _mm_ceil_pd(c));
  SHOW("round_trunc", _mm_round_ps(a, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
  SHOW("round_cur", _mm_round_ps(a, _MM_FROUND_CUR_DIRECTION));
  SHOW("round_ss", _mm_round_ss(a, b, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
  SHOW("floor_sd", _mm_floor_sd(c, _mm_set1_pd(-2.5)));
}

__attribute__((target("avx2"), noinline)) static void avx(const float *fa, const float *fb, const double *da,
                                                          const signed char *bytes) {
  __m256 a = _mm256_loadu_ps(fa), b = _mm256_loadu_ps(fb);
  __m256d c = _mm256_loadu_pd(da);
  __m256i x = _mm256_loadu_si256((const __m256i *)bytes), y = _mm256_loadu_si256((const __m256i *)(bytes + 32));
  SHOW("shuffle256_ps", _mm256_shuffle_ps(a, b, 0x9c));
  SHOW("shuffle256_pd", _mm256_shuffle_pd(c, _mm256_set1_pd(7.0), 0x6));
  SHOW("alignr256_3", _mm256_alignr_epi8(x, y, 3));
  SHOW("alignr256_17", _mm256_alignr_epi8(x, y, 17));
  SHOW("perm2x128_21", _mm256_permute2x128_si256(x, y, 0x21));
  SHOW("perm2x128_08", _mm256_permute2x128_si256(x, y, 0x08));
  SHOW("perm2x128_83", _mm256_permute2x128_si256(x, y, 0x83));
  SHOW("perm2f128_ps", _mm256_permute2f128_ps(a, b, 0x30));
  SHOW("perm2f128_pd", _mm256_permute2f128_pd(c, c, 0x01));
  SHOW("round256_pd", _mm256_round_pd(c, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC));
  SHOW("round256_ps", _mm256_round_ps(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));
}

__attribute__((target("avx512f,avx512bw,avx512dq,avx512vl"), noinline)) static void avx512(
    const float *fa, const float *fb, const double *da, const signed char *bytes, int tail) {
  __m512i x = _mm512_loadu_si512(bytes), y = _mm512_loadu_si512(bytes + 64);
  printf("cmp_epi8_lt %016llx\n", (unsigned long long)_mm512_cmp_epi8_mask(x, y, _MM_CMPINT_LT));
  printf("cmp_epi8_ne %016llx\n", (unsigned long long)_mm512_cmp_epi8_mask(x, y, _MM_CMPINT_NE));
  printf("cmp_epu8_le %016llx\n", (unsigned long long)_mm512_cmp_epu8_mask(x, y, _MM_CMPINT_LE));
  printf("cmp_epu8_nlt %016llx\n", (unsigned long long)_mm512_cmp_epu8_mask(x, y, _MM_CMPINT_NLT));
  printf("cmpeq_epi8 %016llx\n", (unsigned long long)_mm512_mask_cmpeq_epi8_mask(0xf0f0f0f0f0f0f0f0ull, x, x));
  printf("cmp_epi32_gt %02x\n", (unsigned)_mm256_cmp_epi32_mask(_mm512_castsi512_si256(x),
                                                                 _mm512_castsi512_si256(y), _MM_CMPINT_NLE));
  printf("cmp_epu16_false %08x\n", (unsigned)_mm512_cmp_epu16_mask(x, y, 3));
  printf("cmp_epi64_true %02x\n", (unsigned)_mm512_cmp_epi64_mask(x, y, 7));
  __m512 a = _mm512_loadu_ps(fa), b = _mm512_loadu_ps(fb);
  __m512d c = _mm512_loadu_pd(da);
  SHOW("shuffle512_ps", _mm512_shuffle_ps(a, b, 0x4e));
  SHOW("fmaddsub_pd", _mm512_fmaddsub_pd(c, c, _mm512_set1_pd(0.25)));
  SHOW("mask_fmaddsub_pd", _mm512_mask_fmaddsub_pd(c, 0x5a, c, _mm512_set1_pd(1.0)));
  SHOW("maskz_fmaddsub_ps", _mm512_maskz_fmaddsub_ps(0x0ff0, a, b, a));
  SHOW("mask3_fmaddsub_pd", _mm512_mask3_fmaddsub_pd(c, c, _mm512_set1_pd(3.0), 0x0f));
  printf("reduce_max_pd %g\n", _mm512_reduce_max_pd(c));
  printf("reduce_min_ps %g\n", _mm512_reduce_min_ps(_mm512_cvtepi32_ps(x)));
  SHOW("cvtph_ps", _mm512_cvtph_ps(_mm512_castsi512_si256(x)));
  int table[64];
  for (int i = 0; i < 64; i++)
    table[i] = i * i - 100;
  __m512i index = _mm512_and_si512(_mm512_loadu_si512(bytes), _mm512_set1_epi32(63));
  SHOW("gather_epi32", _mm512_i32gather_epi32(index, table, 4));
  SHOW("mask_gather_ps", _mm512_mask_i32gather_ps(a, (__mmask16)0x3c3c, index, (const float *)table, 4));
  int scattered[64] = {0};
  _mm512_i32scatter_epi32(scattered, _mm512_set_epi32(60, 56, 52, 48, 44, 40, 36, 32, 28, 24, 20, 16, 12, 8, 4, 0),
                          index, 4);
  show("scatter_epi32", scattered, sizeof scattered);
  long long stored[8] = {0};
  _mm512_mask_storeu_epi64(stored, (__mmask8)((1u << (tail / 2)) - 1), _mm512_set1_epi64(-9));
  show("mask_storeu_epi64", stored, sizeof stored);
  signed char stored8[64] = {0};
  _mm512_mask_storeu_epi8(stored8, 0x8000000000000001ull >> (tail % 3), x);
  show("mask_storeu_epi8", stored8, sizeof stored8);
  int stored32[8] = {0};
  _mm256_mask_storeu_epi32(stored32, (__mmask8)tail, _mm512_castsi512_si256(y));
  show("mask_storeu256_epi32", stored32, sizeof stored32);
}

int main(void) {
  float fa[16] = {1.5f, -2.5f, NAN, 4.49f, -0.0f, 6.5f, 7.75f, -8.25f, 9, NAN, 11.5f, 12, 13, -14.5f, 15, 16};
  float fb[16] = {4, 2.5f, 1, NAN, 0.0f, -6, 1, 9.5f, 9, 10, NAN, 1, 14, 13, 15, -16};
  double da[8] = {1.5, -2.5, 9.25, 16.75, -0.5, 3, 7.5, -8};
  signed char bytes[128];
  for (int i = 0; i < 128; i++)
    bytes[i] = (signed char)(i * 53 + 7);
  sse(fa, fb, da, bytes);
  if (__builtin_cpu_supports("avx2"))
    avx(fa, fb, da, bytes);
  if (__builtin_cpu_supports("avx512bw") && __builtin_cpu_supports("avx512dq") &&
      __builtin_cpu_supports("avx512vl"))
    for (int tail = 0; tail <= 16; tail += 5)
      avx512(fa, fb, da, bytes, tail);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! SHOW__7 {
// CHECK-NEXT:     ($name:expr, $v:expr, $x:expr, $y:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 2 > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >, std::simd::Simd < i8,
// CHECK-NEXT:         16 >> ($x) }; unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >,
// CHECK-NEXT:         std::simd::Simd < i8, 16 >> ($y) }; t_ = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__5 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 8 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__8 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 8 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 64i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 4 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__3 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f64, 2 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__6 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f64, 4 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 16 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 64i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__4 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f64, 8 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 64i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_cpu_supports([[ARG_0:arg[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.sse41.round.sd"]
// CHECK-NEXT:     fn __slate_intrinsic_283f4de954b09d3f(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx.round.pd.256"]
// CHECK-NEXT:     fn __slate_intrinsic_29b5ef18c643ff00(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 4>,
// CHECK-NEXT:         [[ARG_1]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 4>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.scatter.dpi.512"]
// CHECK-NEXT:     fn __slate_intrinsic_303cc290a2814231(
// CHECK-NEXT:         [[ARG_0]]: *mut (),
// CHECK-NEXT:         [[ARG_1]]: i16,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i32, 16>,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: std::simd::Simd<i32, 16>,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     #[link_name = "llvm.x86.sse41.round.ps"]
// CHECK-NEXT:     fn __slate_intrinsic_53c06c204073bc(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.vfmaddsub.ps.512"]
// CHECK-NEXT:     fn __slate_intrinsic_617bca97bebbbdec(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_3]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 16>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.vfmaddsub.pd.512"]
// CHECK-NEXT:     fn __slate_intrinsic_6c6d806fde32d802(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:         [[ARG_3]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 8>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.mask.vcvtph2ps.512"]
// CHECK-NEXT:     fn __slate_intrinsic_6e773f80963d7fd9(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i16, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_2]]: u16,
// CHECK-NEXT:         [[ARG_3]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 16>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.gather.dps.512"]
// CHECK-NEXT:     fn __slate_intrinsic_7cd5682a6f9641e8(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:         [[ARG_1]]: *const (),
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i32, 16>,
// CHECK-NEXT:         [[ARG_3]]: i16,
// CHECK-NEXT:         [[ARG_4]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 16>;
// CHECK-NEXT:     #[link_name = "llvm.x86.sse41.round.pd"]
// CHECK-NEXT:     fn __slate_intrinsic_8f9dc76ce5609ed9(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.fmax.v8f64"]
// CHECK-NEXT:     fn __slate_intrinsic_9dd14e6b812661cd([[ARG_0]]: std::simd::Simd<f64, 8>) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.x86.sse41.round.ss"]
// CHECK-NEXT:     fn __slate_intrinsic_9f7ded4d7a204b4a(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_2]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx.round.ps.256"]
// CHECK-NEXT:     fn __slate_intrinsic_d2ac07a07f438c2d(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:         [[ARG_1]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 8>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.fmin.v16f32"]
// CHECK-NEXT:     fn __slate_intrinsic_e10e78e3c8296e57([[ARG_0]]: std::simd::Simd<f32, 16>) -> f32;
// CHECK-NEXT:     #[link_name = "llvm.x86.avx512.gather.dpi.512"]
// CHECK-NEXT:     fn __slate_intrinsic_fde483b058db6b4(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 16>,
// CHECK-NEXT:         [[ARG_1]]: *const (),
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i32, 16>,
// CHECK-NEXT:         [[ARG_3]]: i16,
// CHECK-NEXT:         [[ARG_4]]: i32,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 16>;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_ps__1858 {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 4>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_pd__1620 {
// CHECK-NEXT:     __v: std::simd::Simd<f64, 2>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si128 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 2>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_pd__3129 {
// CHECK-NEXT:     __v: std::simd::Simd<f64, 4>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_ps__3149 {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 8>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si256 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 4>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si512 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 8>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_pd__4144 {
// CHECK-NEXT:     __v: std::simd::Simd<f64, 8>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_ps__4153 {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 16>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_loadu_ps(__p: *const f32) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_ps__1858)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_loadu_pd(__dp: *const f64) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__dp as * const __loadu_pd__1620)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_set1_pd(__w: f64) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([__w, __w]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
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
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_loadu_pd(__p: *const f64) -> std::simd::Simd<f64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_pd__3129)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_loadu_ps(__p: *const f32) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_ps__3149)).__v).read_unaligned()
// CHECK-NEXT:     };
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
// CHECK-NEXT: unsafe fn _mm256_set_pd(
// CHECK-NEXT:     __a: f64,
// CHECK-NEXT:     __b: f64,
// CHECK-NEXT:     __c: f64,
// CHECK-NEXT:     __d: f64,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<f64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([__d, __c, __b, __a]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_pd(__w: f64) -> std::simd::Simd<f64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_pd(__w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_undefined_epi32() -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<f64, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(std::simd::Simd::splat(0.0f64))
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_setzero_ps() -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<f32, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:                 0.0f32,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set1_pd(__w: f64) -> std::simd::Simd<f64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<f64, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:                 __w,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set1_epi32(__s: i32) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i32, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
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
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set1_epi64(__d: i64) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i64, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:                 __d,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_castsi512_si256(
// CHECK-NEXT:     mut __A: std::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__A, __A, [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_and_si512(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 8>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 8>,
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
// CHECK-NEXT:                 >(__a)
// CHECK-NEXT:             })
// CHECK-NEXT:                 & unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<u64, 8>,
// CHECK-NEXT:                     >(__b)
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_fmaddsub_pd(
// CHECK-NEXT:     [[V_0:__v[0-9]+]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:     [[V_1:__v[0-9]+]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:     [[V_2:__v[0-9]+]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 8> {
// CHECK-NEXT:     return std::simd::Select::select(
// CHECK-NEXT:         std::simd::Mask::<i64, 8>::from_bitmask(255u64),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_intrinsic_6c6d806fde32d802([[V_0]], [[V_1]], [[V_2]], const { 4 as i32 })
// CHECK-NEXT:         },
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_mask_fmaddsub_pd(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:     __U: u8,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:     [[V_2]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 8> {
// CHECK-NEXT:     return std::simd::Select::select(
// CHECK-NEXT:         std::simd::Mask::<i64, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_intrinsic_6c6d806fde32d802([[V_0]], [[V_1]], [[V_2]], const { 4 as i32 })
// CHECK-NEXT:         },
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_mask3_fmaddsub_pd(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:     [[V_2]]: std::simd::Simd<f64, 8>,
// CHECK-NEXT:     __U: u8,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 8> {
// CHECK-NEXT:     return std::simd::Select::select(
// CHECK-NEXT:         std::simd::Mask::<i64, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_intrinsic_6c6d806fde32d802([[V_0]], [[V_1]], [[V_2]], const { 4 as i32 })
// CHECK-NEXT:         },
// CHECK-NEXT:         [[V_2]],
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_maskz_fmaddsub_ps(
// CHECK-NEXT:     __U: u16,
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT:     [[V_2]]: std::simd::Simd<f32, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return std::simd::Select::select(
// CHECK-NEXT:         std::simd::Mask::<i32, 16>::from_bitmask((__U as u16) as u64),
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             __slate_intrinsic_617bca97bebbbdec([[V_0]], [[V_1]], [[V_2]], const { 4 as i32 })
// CHECK-NEXT:         },
// CHECK-NEXT:         std::simd::Simd::splat(0.0f32),
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_cvtepi32_ps(__A: std::simd::Simd<i64, 8>) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return std::simd::num::SimdInt::cast::<
// CHECK-NEXT:         f32,
// CHECK-NEXT:     >(unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i32, 16>>(__A)
// CHECK-NEXT:     });
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_cvtph_ps(__A: std::simd::Simd<i64, 4>) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         __slate_intrinsic_6e773f80963d7fd9(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 4>,
// CHECK-NEXT:                     std::simd::Simd<i16, 16>,
// CHECK-NEXT:                 >(__A)
// CHECK-NEXT:             },
// CHECK-NEXT:             _mm512_setzero_ps(),
// CHECK-NEXT:             65535u16,
// CHECK-NEXT:             const { 4 as i32 },
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
// CHECK-NEXT: unsafe fn _mm512_loadu_pd(__p: *const ()) -> std::simd::Simd<f64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_pd__4144)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_loadu_ps(__p: *const ()) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_ps__4153)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_mask_storeu_epi64(__P: *mut (), __U: u8, __A: std::simd::Simd<i64, 8>) {
// CHECK-NEXT:     std::simd::Simd::store_select_ptr(
// CHECK-NEXT:         __A,
// CHECK-NEXT:         __P as *mut i64,
// CHECK-NEXT:         std::simd::Mask::<i64, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_set_epi32(
// CHECK-NEXT:     __A: i32,
// CHECK-NEXT:     __B: i32,
// CHECK-NEXT:     __C: i32,
// CHECK-NEXT:     __D: i32,
// CHECK-NEXT:     __E: i32,
// CHECK-NEXT:     __F: i32,
// CHECK-NEXT:     __G: i32,
// CHECK-NEXT:     __H: i32,
// CHECK-NEXT:     __I: i32,
// CHECK-NEXT:     __J: i32,
// CHECK-NEXT:     __K: i32,
// CHECK-NEXT:     __L: i32,
// CHECK-NEXT:     __M: i32,
// CHECK-NEXT:     __N: i32,
// CHECK-NEXT:     __O: i32,
// CHECK-NEXT:     __P: i32,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i32, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __P,
// CHECK-NEXT:                     __O,
// CHECK-NEXT:                     __N,
// CHECK-NEXT:                     __M,
// CHECK-NEXT:                     __L,
// CHECK-NEXT:                     __K,
// CHECK-NEXT:                     __J,
// CHECK-NEXT:                     __I,
// CHECK-NEXT:                     __H,
// CHECK-NEXT:                     __G,
// CHECK-NEXT:                     __F,
// CHECK-NEXT:                     __E,
// CHECK-NEXT:                     __D,
// CHECK-NEXT:                     __C,
// CHECK-NEXT:                     __B,
// CHECK-NEXT:                     __A,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_reduce_max_pd(__V: std::simd::Simd<f64, 8>) -> f64 {
// CHECK-NEXT:     unsafe { __slate_intrinsic_9dd14e6b812661cd(__V) }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_reduce_min_ps(__V: std::simd::Simd<f32, 16>) -> f32 {
// CHECK-NEXT:     unsafe { __slate_intrinsic_e10e78e3c8296e57(__V) }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vl")]
// CHECK-NEXT: unsafe fn _mm256_mask_storeu_epi32(__P: *mut (), __U: u8, __A: std::simd::Simd<i64, 4>) {
// CHECK-NEXT:     std::simd::Simd::store_select_ptr(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<i32, 8>>(__A)
// CHECK-NEXT:         },
// CHECK-NEXT:         __P as *mut i32,
// CHECK-NEXT:         std::simd::Mask::<i32, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _mm512_mask_storeu_epi8(__P: *mut (), __U: u64, __A: std::simd::Simd<i64, 8>) {
// CHECK-NEXT:     std::simd::Simd::store_select_ptr(
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(__A)
// CHECK-NEXT:         },
// CHECK-NEXT:         __P as *mut i8,
// CHECK-NEXT:         std::simd::Mask::<i8, 64>::from_bitmask(__U as u64),
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn show(name: *const i8, data: *const (), bytes: i32) {
// CHECK-NEXT:     let p: *const u8 = data as *const u8;
// CHECK-NEXT:     let mut hash: u32 = 2166136261;
// CHECK-NEXT:     for i in 0..bytes {
// CHECK-NEXT:         hash = (hash ^ (((unsafe { *p.offset(i as isize) }) as i32) as u32))
// CHECK-NEXT:             .wrapping_mul(16777619);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s %08x\n".as_ptr(), name, hash) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse4.1")]
// CHECK-NEXT: unsafe fn sse(fa: *const f32, fb: *const f32, da: *const f64, bytes: *const i8) {
// CHECK-NEXT:     let mut a: std::simd::Simd<f32, 4> = unsafe { _mm_loadu_ps(fa) };
// CHECK-NEXT:     let mut b: std::simd::Simd<f32, 4> = unsafe { _mm_loadu_ps(fb) };
// CHECK-NEXT:     let mut c: std::simd::Simd<f64, 2> = unsafe { _mm_loadu_pd(da) };
// CHECK-NEXT:     let x: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_loadu_si128(bytes as *const std::simd::Simd<i64, 2>)
// CHECK-NEXT:     };
// CHECK-NEXT:     let y: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_loadu_si128(bytes.add(16) as *const std::simd::Simd<i64, 2>)
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"shuffle_ps".as_ptr() as * mut i8, std::simd::simd_swizzle!(a, b, [3, 2, 5, 4])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"shuffle_ps2".as_ptr() as * mut i8, std::simd::simd_swizzle!(a, b, [0, 1, 6, 7])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"shuffle_pd".as_ptr() as * mut i8, std::simd::simd_swizzle!(c, unsafe {
// CHECK-NEXT:         _mm_set1_pd(5.5f64) }, [1, 2])
// CHECK-NEXT:     );
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i8, 16>>(x)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i8, 16>>(y)
// CHECK-NEXT:         };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     [[V_4]], [[V_3]], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"alignr0".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i8, 16>>(x)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i8, 16>>(y)
// CHECK-NEXT:         };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     [[V_6]], [[V_5]], [5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
// CHECK-NEXT:                     20]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"alignr5".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i8, 16>>(x)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i8, 16>>(y)
// CHECK-NEXT:         };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     [[V_8]], [[V_7]], [16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
// CHECK-NEXT:                     30, 31]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"alignr16".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i8, 16>>(x)
// CHECK-NEXT:         };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     [[V_9]], std::simd::Simd::splat(0i8), [4, 5, 6, 7, 8, 9, 10, 11, 12,
// CHECK-NEXT:                     13, 14, 15, 16, 17, 18, 19]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"alignr20".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__7!(
// CHECK-NEXT:         c"alignr32".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd
// CHECK-NEXT:         < i8, 16 >, std::simd::Simd < i64, 2 >> (std::simd::Simd::splat(0i8)) }, x, y
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"round_near".as_ptr() as * mut i8, unsafe { __slate_intrinsic_53c06c204073bc(a,
// CHECK-NEXT:         const { (0 as i32) | (8 as i32) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"round_floor".as_ptr() as * mut i8, unsafe { __slate_intrinsic_53c06c204073bc(a,
// CHECK-NEXT:         const { (0 as i32) | (1 as i32) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"round_ceil".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         __slate_intrinsic_8f9dc76ce5609ed9(c, const { (0 as i32) | (2 as i32) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"round_trunc".as_ptr() as * mut i8, unsafe { __slate_intrinsic_53c06c204073bc(a,
// CHECK-NEXT:         const { (3 as i32) | (8 as i32) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"round_cur".as_ptr() as * mut i8, unsafe { __slate_intrinsic_53c06c204073bc(a,
// CHECK-NEXT:         const { 4 as i32 }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"round_ss".as_ptr() as * mut i8, unsafe { __slate_intrinsic_9f7ded4d7a204b4a(a,
// CHECK-NEXT:         b, const { (2 as i32) | (8 as i32) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"floor_sd".as_ptr() as * mut i8, unsafe { __slate_intrinsic_283f4de954b09d3f(c,
// CHECK-NEXT:         _mm_set1_pd(- 2.5f64), const { (0 as i32) | (1 as i32) }) }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn avx(fa: *const f32, fb: *const f32, da: *const f64, bytes: *const i8) {
// CHECK-NEXT:     let mut a: std::simd::Simd<f32, 8> = unsafe { _mm256_loadu_ps(fa) };
// CHECK-NEXT:     let mut b: std::simd::Simd<f32, 8> = unsafe { _mm256_loadu_ps(fb) };
// CHECK-NEXT:     let mut c: std::simd::Simd<f64, 4> = unsafe { _mm256_loadu_pd(da) };
// CHECK-NEXT:     let x: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(bytes as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     let y: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(bytes.add(32) as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"shuffle256_ps".as_ptr() as * mut i8, std::simd::simd_swizzle!(a, b, [0, 3, 9,
// CHECK-NEXT:         10, 4, 7, 13, 14])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"shuffle256_pd".as_ptr() as * mut i8, std::simd::simd_swizzle!(c, unsafe {
// CHECK-NEXT:         _mm256_set1_pd(7.0f64) }, [0, 5, 3, 6])
// CHECK-NEXT:     );
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_2]]: std::simd::Simd<i8, 32> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<i8, 32>>(x)
// CHECK-NEXT:         };
// CHECK-NEXT:         let mut [[V_3]]: std::simd::Simd<i8, 32> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<i8, 32>>(y)
// CHECK-NEXT:         };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 32>,
// CHECK-NEXT:                 std::simd::Simd<i64, 4>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     [[V_3]], [[V_2]], [3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 32, 33, 34,
// CHECK-NEXT:                     19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 48, 49, 50]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"alignr256_3".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: std::simd::Simd<i8, 32> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<i8, 32>>(x)
// CHECK-NEXT:         };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 32>,
// CHECK-NEXT:                 std::simd::Simd<i64, 4>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     [[V_10]], std::simd::Simd::splat(0i8), [1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
// CHECK-NEXT:                     11, 12, 13, 14, 15, 32, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
// CHECK-NEXT:                     28, 29, 30, 31, 48]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"alignr256_17".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: std::simd::Simd<i64, 4> = x;
// CHECK-NEXT:         let mut [[V_7]]: std::simd::Simd<i64, 4> = y;
// CHECK-NEXT:         t_ = std::simd::simd_swizzle!([[V_11]], [[V_7]], [2, 3, 4, 5]);
// CHECK-NEXT:         show(c"perm2x128_21".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: std::simd::Simd<i64, 4> = x;
// CHECK-NEXT:         t_ = std::simd::simd_swizzle!(std::simd::Simd::splat(0i64), [[V_12]], [0, 1, 4, 5]);
// CHECK-NEXT:         show(c"perm2x128_08".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<i64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: std::simd::Simd<i64, 4> = y;
// CHECK-NEXT:         t_ = std::simd::simd_swizzle!([[V_13]], std::simd::Simd::splat(0i64), [2, 3, 4, 5]);
// CHECK-NEXT:         show(c"perm2x128_83".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = std::simd::simd_swizzle!([[V_14]], [[V_15]], [0, 1, 2, 3, 12, 13, 14, 15]);
// CHECK-NEXT:         show(c"perm2f128_ps".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: std::simd::Simd<f64, 4> = c;
// CHECK-NEXT:         t_ = std::simd::simd_swizzle!([[V_16]], [[V_16]], [2, 3, 4, 5]);
// CHECK-NEXT:         show(c"perm2f128_pd".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"round256_pd".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         __slate_intrinsic_29b5ef18c643ff00(c, const { (1 as i32) | (8 as i32) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"round256_ps".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         __slate_intrinsic_d2ac07a07f438c2d(a, const { (0 as i32) | (8 as i32) }) }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f,avx512bw,avx512dq,avx512vl")]
// CHECK-NEXT: unsafe fn avx512(
// CHECK-NEXT:     fa: *const f32,
// CHECK-NEXT:     fb: *const f32,
// CHECK-NEXT:     da: *const f64,
// CHECK-NEXT:     bytes: *const i8,
// CHECK-NEXT:     tail: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let x: std::simd::Simd<i64, 8> = unsafe { _mm512_loadu_si512(bytes as *const ()) };
// CHECK-NEXT:     let y: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         _mm512_loadu_si512(bytes.add(64) as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_0]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(x)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_1]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(y)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp_epi8_lt %016llx\n".as_ptr(),
// CHECK-NEXT:             std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_0:__t[0-9]+]]: std::simd::Simd<i8, 64> = [[V_0]];
// CHECK-NEXT:                         let [[T_1:__t[0-9]+]] = [[T_0]].to_array();
// CHECK-NEXT:                         let [[T_2:__t[0-9]+]]: std::simd::Simd<i8, 64> = [[V_1]];
// CHECK-NEXT:                         let [[T_3:__t[0-9]+]] = [[T_2]].to_array();
// CHECK-NEXT:                         let [[T_4:__t[0-9]+]]: std::simd::Simd<i8, 64> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_1]][__lane] < [[T_3]][__lane]) as i8)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_4]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::<i8, 64>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_2]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(x)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_3]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(y)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp_epi8_ne %016llx\n".as_ptr(),
// CHECK-NEXT:             std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_5:__t[0-9]+]]: std::simd::Simd<i8, 64> = [[V_2]];
// CHECK-NEXT:                         let [[T_6:__t[0-9]+]] = [[T_5]].to_array();
// CHECK-NEXT:                         let [[T_7:__t[0-9]+]]: std::simd::Simd<i8, 64> = [[V_3]];
// CHECK-NEXT:                         let [[T_8:__t[0-9]+]] = [[T_7]].to_array();
// CHECK-NEXT:                         let [[T_9:__t[0-9]+]]: std::simd::Simd<i8, 64> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_6]][__lane] != [[T_8]][__lane]) as i8)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_9]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::<i8, 64>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_4]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(x)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_10]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(y)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp_epu8_le %016llx\n".as_ptr(),
// CHECK-NEXT:             std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_10:__t[0-9]+]]: std::simd::Simd<u8, 64> = unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i8, 64>,
// CHECK-NEXT:                                 std::simd::Simd<u8, 64>,
// CHECK-NEXT:                             >([[V_4]])
// CHECK-NEXT:                         };
// CHECK-NEXT:                         let [[T_11:__t[0-9]+]] = [[T_10]].to_array();
// CHECK-NEXT:                         let [[T_12:__t[0-9]+]]: std::simd::Simd<u8, 64> = unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i8, 64>,
// CHECK-NEXT:                                 std::simd::Simd<u8, 64>,
// CHECK-NEXT:                             >([[V_10]])
// CHECK-NEXT:                         };
// CHECK-NEXT:                         let [[T_13:__t[0-9]+]] = [[T_12]].to_array();
// CHECK-NEXT:                         let [[T_14:__t[0-9]+]]: std::simd::Simd<i8, 64> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_11]][__lane] <= [[T_13]][__lane]) as i8)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_14]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::<i8, 64>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_5]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(x)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_6]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(y)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp_epu8_nlt %016llx\n".as_ptr(),
// CHECK-NEXT:             std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_15:__t[0-9]+]]: std::simd::Simd<u8, 64> = unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i8, 64>,
// CHECK-NEXT:                                 std::simd::Simd<u8, 64>,
// CHECK-NEXT:                             >([[V_5]])
// CHECK-NEXT:                         };
// CHECK-NEXT:                         let [[T_16:__t[0-9]+]] = [[T_15]].to_array();
// CHECK-NEXT:                         let [[T_17:__t[0-9]+]]: std::simd::Simd<u8, 64> = unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i8, 64>,
// CHECK-NEXT:                                 std::simd::Simd<u8, 64>,
// CHECK-NEXT:                             >([[V_6]])
// CHECK-NEXT:                         };
// CHECK-NEXT:                         let [[T_18:__t[0-9]+]] = [[T_17]].to_array();
// CHECK-NEXT:                         let [[T_19:__t[0-9]+]]: std::simd::Simd<i8, 64> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_16]][__lane] >= [[T_18]][__lane]) as i8)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_19]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::<i8, 64>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_11]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(x)
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_7]]: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i64, 8>, std::simd::Simd<i8, 64>>(x)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmpeq_epi8 %016llx\n".as_ptr(),
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_20:__t[0-9]+]]: std::simd::Simd<i8, 64> = [[V_11]];
// CHECK-NEXT:                         let [[T_21:__t[0-9]+]] = [[T_20]].to_array();
// CHECK-NEXT:                         let [[T_22:__t[0-9]+]]: std::simd::Simd<i8, 64> = [[V_7]];
// CHECK-NEXT:                         let [[T_23:__t[0-9]+]] = [[T_22]].to_array();
// CHECK-NEXT:                         let [[T_24:__t[0-9]+]]: std::simd::Simd<i8, 64> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_21]][__lane] == [[T_23]][__lane]) as i8)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_24]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::<i8, 64>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u64) & 17361641481138401520,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_8]]: std::simd::Simd<i32, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:             std::simd::Simd<i32, 8>,
// CHECK-NEXT:         >(unsafe { _mm512_castsi512_si256(x) })
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_12]]: std::simd::Simd<i32, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:             std::simd::Simd<i32, 8>,
// CHECK-NEXT:         >(unsafe { _mm512_castsi512_si256(y) })
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp_epi32_gt %02x\n".as_ptr(),
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_25:__t[0-9]+]]: std::simd::Simd<i32, 8> = [[V_8]];
// CHECK-NEXT:                         let [[T_26:__t[0-9]+]] = [[T_25]].to_array();
// CHECK-NEXT:                         let [[T_27:__t[0-9]+]]: std::simd::Simd<i32, 8> = [[V_12]];
// CHECK-NEXT:                         let [[T_28:__t[0-9]+]] = [[T_27]].to_array();
// CHECK-NEXT:                         let [[T_29:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_26]][__lane] > [[T_28]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_29]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::<i32, 8>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u8) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp_epu16_false %08x\n".as_ptr(),
// CHECK-NEXT:             std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     std::simd::Simd::splat(0i16),
// CHECK-NEXT:                     std::simd::Simd::<i16, 32>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp_epi64_true %02x\n".as_ptr(),
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     !std::simd::Simd::splat(0i64),
// CHECK-NEXT:                     std::simd::Simd::<i64, 8>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u8) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut a: std::simd::Simd<f32, 16> = unsafe { _mm512_loadu_ps(fa as *const ()) };
// CHECK-NEXT:     let mut b: std::simd::Simd<f32, 16> = unsafe { _mm512_loadu_ps(fb as *const ()) };
// CHECK-NEXT:     let c: std::simd::Simd<f64, 8> = unsafe { _mm512_loadu_pd(da as *const ()) };
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"shuffle512_ps".as_ptr() as * mut i8, std::simd::simd_swizzle!(a, b, [2, 3, 16,
// CHECK-NEXT:         17, 6, 7, 20, 21, 10, 11, 24, 25, 14, 15, 28, 29])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__4!(
// CHECK-NEXT:         c"fmaddsub_pd".as_ptr() as * mut i8, unsafe { _mm512_fmaddsub_pd(c, c,
// CHECK-NEXT:         _mm512_set1_pd(0.25f64)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__4!(
// CHECK-NEXT:         c"mask_fmaddsub_pd".as_ptr() as * mut i8, unsafe { _mm512_mask_fmaddsub_pd(c,
// CHECK-NEXT:         90u8, c, _mm512_set1_pd(1.0f64)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"maskz_fmaddsub_ps".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         _mm512_maskz_fmaddsub_ps(4080u16, a, b, a) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__4!(
// CHECK-NEXT:         c"mask3_fmaddsub_pd".as_ptr() as * mut i8, unsafe { _mm512_mask3_fmaddsub_pd(c,
// CHECK-NEXT:         c, _mm512_set1_pd(3.0f64), 15u8) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"reduce_max_pd %g\n".as_ptr(), _mm512_reduce_max_pd(c))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"reduce_min_ps %g\n".as_ptr(),
// CHECK-NEXT:             _mm512_reduce_min_ps(_mm512_cvtepi32_ps(x)) as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"cvtph_ps".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         _mm512_cvtph_ps(_mm512_castsi512_si256(x)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut table: __SlateAlign16<[i32; 64]> = __SlateAlign16([0i32; 64]);
// CHECK-NEXT:     for i in 0i32..64i32 {
// CHECK-NEXT:         table.0[i as usize] = i * i - 100;
// CHECK-NEXT:     }
// CHECK-NEXT:     let index: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         _mm512_and_si512(
// CHECK-NEXT:             _mm512_loadu_si512(bytes as *const ()),
// CHECK-NEXT:             _mm512_set1_epi32(63i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW__8!(
// CHECK-NEXT:         c"gather_epi32".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 16 >, std::simd::Simd < i64, 8 >> (unsafe {
// CHECK-NEXT:         __slate_intrinsic_fde483b058db6b4(unsafe { std::mem::transmute::< std::simd::Simd
// CHECK-NEXT:         < i64, 8 >, std::simd::Simd < i32, 16 >> (_mm512_undefined_epi32()) }, table.0
// CHECK-NEXT:         .as_mut_ptr() as * const (), unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         i64, 8 >, std::simd::Simd < i32, 16 >> (index) }, - 1i16, const { 4 as i32 }) })
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"mask_gather_ps".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         __slate_intrinsic_7cd5682a6f9641e8(a, table.0.as_mut_ptr() as * const (), unsafe
// CHECK-NEXT:         { std::mem::transmute::< std::simd::Simd < f32, 16 >, std::simd::Simd < i32, 16
// CHECK-NEXT:         >> (unsafe { std::mem::transmute::< std::simd::Simd < i64, 8 >, std::simd::Simd <
// CHECK-NEXT:         f32, 16 >> (index) }) }, 15420i16, const { 4 as i32 }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut scattered: __SlateAlign16<[i32; 64]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_30:__t[0-9]+]]: [i32; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_30]][0] = 0i32;
// CHECK-NEXT:         [[T_30]]
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         __slate_intrinsic_303cc290a2814231(
// CHECK-NEXT:             scattered.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             -1i16,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 8>,
// CHECK-NEXT:                     std::simd::Simd<i32, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     _mm512_set_epi32(
// CHECK-NEXT:                         60i32,
// CHECK-NEXT:                         56i32,
// CHECK-NEXT:                         52i32,
// CHECK-NEXT:                         48i32,
// CHECK-NEXT:                         44i32,
// CHECK-NEXT:                         40i32,
// CHECK-NEXT:                         36i32,
// CHECK-NEXT:                         32i32,
// CHECK-NEXT:                         28i32,
// CHECK-NEXT:                         24i32,
// CHECK-NEXT:                         20i32,
// CHECK-NEXT:                         16i32,
// CHECK-NEXT:                         12i32,
// CHECK-NEXT:                         8i32,
// CHECK-NEXT:                         4i32,
// CHECK-NEXT:                         0i32,
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 8>,
// CHECK-NEXT:                     std::simd::Simd<i32, 16>,
// CHECK-NEXT:                 >(index)
// CHECK-NEXT:             },
// CHECK-NEXT:             const { (4 as i32) as i32 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"scatter_epi32".as_ptr(),
// CHECK-NEXT:         scattered.0.as_mut_ptr() as *const (),
// CHECK-NEXT:         std::mem::size_of::<[i32; 64]>() as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut stored: __SlateAlign16<[i64; 8]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_31:__t[0-9]+]]: [i64; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_31]][0] = 0i64;
// CHECK-NEXT:         [[T_31]]
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm512_mask_storeu_epi64(
// CHECK-NEXT:             stored.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             (1u32 << tail / 2).wrapping_sub(1) as u8,
// CHECK-NEXT:             _mm512_set1_epi64(-9i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"mask_storeu_epi64".as_ptr(),
// CHECK-NEXT:         stored.0.as_mut_ptr() as *const (),
// CHECK-NEXT:         std::mem::size_of::<[i64; 8]>() as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut stored8: __SlateAlign16<[i8; 64]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_32:__t[0-9]+]]: [i8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_32]][0] = 0i8;
// CHECK-NEXT:         [[T_32]]
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm512_mask_storeu_epi8(
// CHECK-NEXT:             stored8.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             9223372036854775809u64 >> tail % 3,
// CHECK-NEXT:             x,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"mask_storeu_epi8".as_ptr(),
// CHECK-NEXT:         stored8.0.as_mut_ptr() as *const (),
// CHECK-NEXT:         std::mem::size_of::<[i8; 64]>() as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut stored32: __SlateAlign16<[i32; 8]> = __SlateAlign16({
// CHECK-NEXT:         let mut [[T_33:__t[0-9]+]]: [i32; 8] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_33]][0] = 0i32;
// CHECK-NEXT:         [[T_33]]
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _mm256_mask_storeu_epi32(
// CHECK-NEXT:             stored32.0.as_mut_ptr() as *mut (),
// CHECK-NEXT:             tail as u8,
// CHECK-NEXT:             _mm512_castsi512_si256(y),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"mask_storeu256_epi32".as_ptr(),
// CHECK-NEXT:         stored32.0.as_mut_ptr() as *const (),
// CHECK-NEXT:         std::mem::size_of::<[i32; 8]>() as i32,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut fa: __SlateAlign16<[f32; 16]> = __SlateAlign16([
// CHECK-NEXT:         1.5f32,
// CHECK-NEXT:         -2.5f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         4.49f32,
// CHECK-NEXT:         -0.0f32,
// CHECK-NEXT:         6.5f32,
// CHECK-NEXT:         7.75f32,
// CHECK-NEXT:         -8.25f32,
// CHECK-NEXT:         9i32 as f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         11.5f32,
// CHECK-NEXT:         12i32 as f32,
// CHECK-NEXT:         13i32 as f32,
// CHECK-NEXT:         -14.5f32,
// CHECK-NEXT:         15i32 as f32,
// CHECK-NEXT:         16i32 as f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut fb: __SlateAlign16<[f32; 16]> = __SlateAlign16([
// CHECK-NEXT:         4i32 as f32,
// CHECK-NEXT:         2.5f32,
// CHECK-NEXT:         1i32 as f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         0.0f32,
// CHECK-NEXT:         -6i32 as f32,
// CHECK-NEXT:         1i32 as f32,
// CHECK-NEXT:         9.5f32,
// CHECK-NEXT:         9i32 as f32,
// CHECK-NEXT:         10i32 as f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         1i32 as f32,
// CHECK-NEXT:         14i32 as f32,
// CHECK-NEXT:         13i32 as f32,
// CHECK-NEXT:         15i32 as f32,
// CHECK-NEXT:         -16i32 as f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut da: __SlateAlign16<[f64; 8]> = __SlateAlign16([
// CHECK-NEXT:         1.5f64,
// CHECK-NEXT:         -2.5f64,
// CHECK-NEXT:         9.25f64,
// CHECK-NEXT:         16.75f64,
// CHECK-NEXT:         -0.5f64,
// CHECK-NEXT:         3i32 as f64,
// CHECK-NEXT:         7.5f64,
// CHECK-NEXT:         -8i32 as f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut bytes: __SlateAlign16<[i8; 128]> = __SlateAlign16([0i8; 128]);
// CHECK-NEXT:     for i in 0i32..128i32 {
// CHECK-NEXT:         bytes.0[i as usize] = (i * 53 + 7) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sse(
// CHECK-NEXT:             fa.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:             fb.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:             da.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:             bytes.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx2") {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             avx(
// CHECK-NEXT:                 fa.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                 fb.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                 da.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:                 bytes.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx512bw")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("avx512dq")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("avx512vl")
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut tail: i32 = 0;
// CHECK-NEXT:         while tail <= 16 {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 avx512(
// CHECK-NEXT:                     fa.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                     fb.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                     da.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:                     bytes.0.as_mut_ptr() as *const i8,
// CHECK-NEXT:                     tail,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             tail += 5;
// CHECK-NEXT:         }
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
