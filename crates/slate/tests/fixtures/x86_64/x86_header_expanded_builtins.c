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
                                                           const double *db, const int *ia) {
  __m128 a = _mm_loadu_ps(fa), b = _mm_loadu_ps(fb);
  __m128d c = _mm_loadu_pd(da), d = _mm_loadu_pd(db);
  __m128i i = _mm_loadu_si128((const __m128i *)ia);
  SHOW("sqrt_ps", _mm_sqrt_ps(a));
  SHOW("sqrt_pd", _mm_sqrt_pd(c));
  SHOW("cmpeq_ps", _mm_cmpeq_ps(a, b));
  SHOW("cmplt_ps", _mm_cmplt_ps(a, b));
  SHOW("cmple_ps", _mm_cmple_ps(a, b));
  SHOW("cmpgt_ps", _mm_cmpgt_ps(a, b));
  SHOW("cmpge_ps", _mm_cmpge_ps(a, b));
  SHOW("cmpneq_ps", _mm_cmpneq_ps(a, b));
  SHOW("cmpnlt_ps", _mm_cmpnlt_ps(a, b));
  SHOW("cmpnle_ps", _mm_cmpnle_ps(a, b));
  SHOW("cmpngt_ps", _mm_cmpngt_ps(a, b));
  SHOW("cmpnge_ps", _mm_cmpnge_ps(a, b));
  SHOW("cmpord_ps", _mm_cmpord_ps(a, b));
  SHOW("cmpunord_ps", _mm_cmpunord_ps(a, b));
  SHOW("cmpeq_ss", _mm_cmpeq_ss(a, b));
  SHOW("cmplt_ss", _mm_cmplt_ss(a, b));
  SHOW("cmple_ss", _mm_cmple_ss(a, b));
  SHOW("cmpgt_ss", _mm_cmpgt_ss(a, b));
  SHOW("cmpneq_ss", _mm_cmpneq_ss(a, b));
  SHOW("cmpnlt_ss", _mm_cmpnlt_ss(a, b));
  SHOW("cmpord_ss", _mm_cmpord_ss(a, b));
  SHOW("cmpunord_ss", _mm_cmpunord_ss(a, b));
  SHOW("cmpeq_pd", _mm_cmpeq_pd(c, d));
  SHOW("cmplt_pd", _mm_cmplt_pd(c, d));
  SHOW("cmpneq_pd", _mm_cmpneq_pd(c, d));
  SHOW("cmpord_pd", _mm_cmpord_pd(c, d));
  SHOW("cmpunord_pd", _mm_cmpunord_pd(c, d));
  SHOW("cmplt_sd", _mm_cmplt_sd(c, d));
  SHOW("cmpord_sd", _mm_cmpord_sd(c, d));
  SHOW("insert_epi32", _mm_insert_epi32(i, 77, 2));
  SHOW("insert_epi16", _mm_insert_epi16(i, -5, 7));
  SHOW("insert_epi8", _mm_insert_epi8(i, 9, 13));
  printf("extract_epi32 %d %d\n", _mm_extract_epi32(i, 1), _mm_extract_epi32(i, 3));
  printf("extract_epi16 %d\n", _mm_extract_epi16(i, 5));
  printf("extract_epi8 %d\n", _mm_extract_epi8(i, 6));
  SHOW("blend_epi16", _mm_blend_epi16(i, _mm_set1_epi16(-1), 0xa5));
  SHOW("blend_ps", _mm_blend_ps(a, b, 0x6));
  SHOW("blend_pd", _mm_blend_pd(c, d, 0x2));
  SHOW("srli0", _mm_srli_si128(i, 0));
  SHOW("srli3", _mm_srli_si128(i, 3));
  SHOW("srli15", _mm_srli_si128(i, 15));
  SHOW("srli16", _mm_srli_si128(i, 16));
  SHOW("slli5", _mm_slli_si128(i, 5));
  SHOW("slli20", _mm_slli_si128(i, 20));
}

#define CMP256(p) SHOW("cmp_ps_" #p, _mm256_cmp_ps(a, b, p))

__attribute__((target("avx2"), noinline)) static void avx(const float *fa, const float *fb, const double *da,
                                                          const int *ia) {
  __m256 a = _mm256_loadu_ps(fa), b = _mm256_loadu_ps(fb);
  __m256d c = _mm256_loadu_pd(da);
  __m256i i = _mm256_loadu_si256((const __m256i *)ia);
  SHOW("sqrt256_ps", _mm256_sqrt_ps(a));
  SHOW("sqrt256_pd", _mm256_sqrt_pd(c));
  CMP256(0); CMP256(1); CMP256(2); CMP256(3); CMP256(4); CMP256(5); CMP256(6); CMP256(7);
  CMP256(8); CMP256(9); CMP256(10); CMP256(11); CMP256(12); CMP256(13); CMP256(14); CMP256(15);
  CMP256(16); CMP256(17); CMP256(18); CMP256(19); CMP256(20); CMP256(21); CMP256(22); CMP256(23);
  CMP256(24); CMP256(25); CMP256(26); CMP256(27); CMP256(28); CMP256(29); CMP256(30); CMP256(31);
  SHOW("cmp_pd_lt", _mm256_cmp_pd(c, _mm256_set1_pd(2.0), _CMP_LT_OQ));
  SHOW("cmp128_ps_nge", _mm_cmp_ps(_mm256_castps256_ps128(a), _mm256_castps256_ps128(b), _CMP_NGE_UQ));
  SHOW("permute4x64_epi64", _mm256_permute4x64_epi64(i, 0x1b));
  SHOW("permute4x64_pd", _mm256_permute4x64_pd(c, 0x4e));
  SHOW("blend_epi32", _mm256_blend_epi32(i, _mm256_set1_epi32(-1), 0x93));
  SHOW("blend256_epi16", _mm256_blend_epi16(i, _mm256_set1_epi16(7), 0x0f));
  SHOW("blend256_pd", _mm256_blend_pd(c, _mm256_set1_pd(9.0), 0x5));
  SHOW("srli256", _mm256_srli_si256(i, 4));
  SHOW("slli256", _mm256_slli_si256(i, 9));
  printf("extract256_epi32 %d\n", _mm256_extract_epi32(i, 6));
  printf("extract256_epi64 %lld\n", (long long)_mm256_extract_epi64(i, 3));
  SHOW("insert256_epi32", _mm256_insert_epi32(i, 1234, 5));
  __m256i wide = _mm256_inserti128_si256(_mm256_castsi128_si256(_mm256_castsi256_si128(i)),
                                         _mm256_extracti128_si256(i, 1), 1);
  SHOW("cast128_256", wide);
  __m256i undef = _mm256_undefined_si256();
  SHOW("undef_blend", _mm256_blend_epi32(undef, i, 0xff));
}

__attribute__((target("avx512f,avx512bw,avx512dq,avx512vl"), noinline)) static void avx512(
    const float *fa, const float *fb, const int *ia, const signed char *bytes, int tail) {
  __m512 a = _mm512_loadu_ps(fa), b = _mm512_loadu_ps(fb);
  printf("cmp512_lt %04x\n", (unsigned)_mm512_cmp_ps_mask(a, b, _CMP_LT_OQ));
  printf("cmp512_unord %04x\n", (unsigned)_mm512_cmp_ps_mask(a, b, _CMP_UNORD_Q));
  printf("cmp512_nle %04x\n", (unsigned)_mm512_cmp_ps_mask(a, b, _CMP_NLE_UQ));
  printf("mask_cmp512_eq %04x\n", (unsigned)_mm512_mask_cmp_ps_mask(0x5a5a, a, b, _CMP_EQ_OQ));
  __mmask16 tail_mask = (__mmask16)((1u << tail) - 1);
  SHOW("maskz_loadu_epi32", _mm512_maskz_loadu_epi32(tail_mask, ia));
  SHOW("mask_loadu_epi32", _mm512_mask_loadu_epi32(_mm512_set1_epi32(-3), tail_mask, ia));
  SHOW("maskz_loadu256_epi32", _mm256_maskz_loadu_epi32((__mmask8)tail_mask, ia));
  SHOW("maskz_loadu_epi8", _mm512_maskz_loadu_epi8(0x00ff00ff00ff00ffull, bytes));
  __m512i v = _mm512_loadu_si512(bytes);
  printf("movepi8_mask %016llx\n", (unsigned long long)_mm512_movepi8_mask(v));
  printf("movepi32_mask %02x\n", (unsigned)_mm_movepi32_mask(_mm512_castsi512_si128(v)));
  SHOW("movm_epi8", _mm512_movm_epi8(0x8000000100000003ull));
  SHOW("movm256_epi16", _mm256_movm_epi16(0x8421));
  __m512i undef = _mm512_undefined_epi32();
  SHOW("undef512_mov", _mm512_mask_mov_epi32(undef, 0xffff, v));
  SHOW("cast256_512", _mm512_inserti64x4(_mm512_castsi256_si512(_mm512_castsi512_si256(v)),
                                         _mm512_extracti64x4_epi64(v, 1), 1));
}

int main(void) {
  float fa[16] = {1, 2, NAN, 4, -0.0f, 6, 7, 8, 9, NAN, 11, 12, 13, 14, 15, 16};
  float fb[16] = {4, 2, 1, NAN, 0.0f, 6, 1, 9, 9, 10, NAN, 1, 14, 13, 15, -16};
  double da[4] = {1, NAN, 9, 16};
  double db[4] = {1, 3, NAN, 4};
  int ia[16];
  signed char bytes[64];
  for (int i = 0; i < 16; i++)
    ia[i] = i * 0x01020304 - 7;
  for (int i = 0; i < 64; i++)
    bytes[i] = (signed char)(i * 37 + 11);
  sse(fa, fb, da, db, ia);
  if (__builtin_cpu_supports("avx2"))
    avx(fa, fb, da, ia);
  if (__builtin_cpu_supports("avx512bw") && __builtin_cpu_supports("avx512dq") &&
      __builtin_cpu_supports("avx512vl"))
    for (int tail = 0; tail <= 16; tail += 5)
      avx512(fa, fb, ia, bytes, tail);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 2 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__8 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 8 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__5 {
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
// CHECK-NEXT: macro_rules! SHOW__6 {
// CHECK-NEXT:     ($name:expr, $v:expr, $a:expr, $b:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 8 > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         $a; $b; t_ = $v; show(($name) as * const i8, std::ptr::addr_of_mut!(t_) as *
// CHECK-NEXT:         const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__4 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f64, 2 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__9 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 8 > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         unsafe { _mm256_setzero_si256() }; t_ = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 64i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__3 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 4 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__7 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f64, 4 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_cpu_supports([[ARG_0:arg[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_1125dcde3b9f0a9b(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v4f64"]
// CHECK-NEXT:     fn __slate_intrinsic_8ecdd79d9cc7546f(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 4>;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_cdebd328655e3640(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v8f32"]
// CHECK-NEXT:     fn __slate_intrinsic_fa8a70aaf3988c39(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 8>;
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
// CHECK-NEXT: struct __loadu_ps__4153 {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 16>,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_sqrt_ps(__a: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_cdebd328655e3640(__a) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpeq_ss(
// CHECK-NEXT:     mut [[V_0:__v[0-9]+]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1:__v[0-9]+]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i32, 4 >, std::simd::Simd
// CHECK-NEXT:         < f32, 4 >> ({ let [[T_0:__t[0-9]+]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_1:__t[0-9]+]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2:__t[0-9]+]] : std::simd::Simd < f32, 4 > = [[V_1]]; let [[T_3:__t[0-9]+]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4:__t[0-9]+]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] ==
// CHECK-NEXT:         [[T_3]][__lane]) as i32))); [[T_4]] }) }, [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpeq_ps(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] == [[T_3]][__lane]) as i32)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmplt_ss(
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i32, 4 >, std::simd::Simd
// CHECK-NEXT:         < f32, 4 >> ({ let [[T_0]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_1]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2]] : std::simd::Simd < f32, 4 > = [[V_1]]; let [[T_3]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] <
// CHECK-NEXT:         [[T_3]][__lane]) as i32))); [[T_4]] }) }, [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmplt_ps(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] < [[T_3]][__lane]) as i32)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmple_ss(
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i32, 4 >, std::simd::Simd
// CHECK-NEXT:         < f32, 4 >> ({ let [[T_0]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_1]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2]] : std::simd::Simd < f32, 4 > = [[V_1]]; let [[T_3]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] <=
// CHECK-NEXT:         [[T_3]][__lane]) as i32))); [[T_4]] }) }, [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmple_ps(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] <= [[T_3]][__lane]) as i32)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpgt_ss(
// CHECK-NEXT:     mut __a: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let [[V_1]]: std::simd::Simd<f32, 4> = __a;
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         __a, std::simd::simd_swizzle!([[V_0]], unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 4 >, std::simd::Simd < f32, 4 >> ({ let [[T_0]] :
// CHECK-NEXT:         std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_1]] = [[T_0]].to_array(); let [[T_2]] :
// CHECK-NEXT:         std::simd::Simd < f32, 4 > = [[V_1]]; let [[T_3]] = [[T_2]].to_array(); let [[T_4]] :
// CHECK-NEXT:         std::simd::Simd < i32, 4 > = std::simd::Simd::from_array(std::array::from_fn(|
// CHECK-NEXT:         __lane | - (([[T_1]][__lane] < [[T_3]][__lane]) as i32))); [[T_4]] }) }, [4, 1, 2, 3]),
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpgt_ps(
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] < [[T_3]][__lane]) as i32)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpge_ps(
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] <= [[T_3]][__lane]) as i32)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpneq_ss(
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i32, 4 >, std::simd::Simd
// CHECK-NEXT:         < f32, 4 >> ({ let [[T_0]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_1]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2]] : std::simd::Simd < f32, 4 > = [[V_1]]; let [[T_3]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] !=
// CHECK-NEXT:         [[T_3]][__lane]) as i32))); [[T_4]] }) }, [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpneq_ps(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] != [[T_3]][__lane]) as i32)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpnlt_ss(
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i32, 4 >, std::simd::Simd
// CHECK-NEXT:         < f32, 4 >> (! { let [[T_0]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_1]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2]] : std::simd::Simd < f32, 4 > = [[V_1]]; let [[T_3]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] <
// CHECK-NEXT:         [[T_3]][__lane]) as i32))); [[T_4]] }) }, [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpnlt_ps(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             !{
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| -(([[T_1]][__lane] < [[T_3]][__lane]) as i32)),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpnle_ps(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             !{
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_1]][__lane] <= [[T_3]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpngt_ps(
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             !{
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| -(([[T_1]][__lane] < [[T_3]][__lane]) as i32)),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpnge_ps(
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             !{
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_1]][__lane] <= [[T_3]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpord_ss(
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i32, 4 >, std::simd::Simd
// CHECK-NEXT:         < f32, 4 >> (({ let [[T_0]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_1]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_3]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] ==
// CHECK-NEXT:         [[T_3]][__lane]) as i32))); [[T_4]] }) & { let [[T_5:__t[0-9]+]] : std::simd::Simd < f32, 4 > =
// CHECK-NEXT:         [[V_1]]; let [[T_6:__t[0-9]+]] = [[T_5]].to_array(); let [[T_7:__t[0-9]+]] : std::simd::Simd < f32, 4 > = [[V_1]];
// CHECK-NEXT:         let [[T_8:__t[0-9]+]] = [[T_7]].to_array(); let [[T_9:__t[0-9]+]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_6]][__lane] ==
// CHECK-NEXT:         [[T_8]][__lane]) as i32))); [[T_9]] }) }, [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpord_ps(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_1]][__lane] == [[T_3]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             })
// CHECK-NEXT:                 & {
// CHECK-NEXT:                     let [[T_5]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:                     let [[T_6]] = [[T_5]].to_array();
// CHECK-NEXT:                     let [[T_7]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:                     let [[T_8]] = [[T_7]].to_array();
// CHECK-NEXT:                     let [[T_9]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_6]][__lane] == [[T_8]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_9]]
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpunord_ss(
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i32, 4 >, std::simd::Simd
// CHECK-NEXT:         < f32, 4 >> (({ let [[T_0]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_1]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2]] : std::simd::Simd < f32, 4 > = [[V_0]]; let [[T_3]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] !=
// CHECK-NEXT:         [[T_3]][__lane]) as i32))); [[T_4]] }) | { let [[T_5]] : std::simd::Simd < f32, 4 > =
// CHECK-NEXT:         [[V_1]]; let [[T_6]] = [[T_5]].to_array(); let [[T_7]] : std::simd::Simd < f32, 4 > = [[V_1]];
// CHECK-NEXT:         let [[T_8]] = [[T_7]].to_array(); let [[T_9]] : std::simd::Simd < i32, 4 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_6]][__lane] !=
// CHECK-NEXT:         [[T_8]][__lane]) as i32))); [[T_9]] }) }, [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_cmpunord_ps(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_1]][__lane] != [[T_3]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             })
// CHECK-NEXT:                 | {
// CHECK-NEXT:                     let [[T_5]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:                     let [[T_6]] = [[T_5]].to_array();
// CHECK-NEXT:                     let [[T_7]]: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:                     let [[T_8]] = [[T_7]].to_array();
// CHECK-NEXT:                     let [[T_9]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_6]][__lane] != [[T_8]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_9]]
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_loadu_ps(__p: *const f32) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_ps__1858)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_sqrt_pd(__a: std::simd::Simd<f64, 2>) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_1125dcde3b9f0a9b(__a) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_cmpeq_pd(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f64, 2> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f64, 2> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] == [[T_3]][__lane]) as i64)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_cmplt_pd(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f64, 2> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f64, 2> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] < [[T_3]][__lane]) as i64)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_cmpord_pd(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f64, 2> = [[V_0]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f64, 2> = [[V_0]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_1]][__lane] == [[T_3]][__lane]) as i64)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             })
// CHECK-NEXT:                 & {
// CHECK-NEXT:                     let [[T_5]]: std::simd::Simd<f64, 2> = [[V_1]];
// CHECK-NEXT:                     let [[T_6]] = [[T_5]].to_array();
// CHECK-NEXT:                     let [[T_7]]: std::simd::Simd<f64, 2> = [[V_1]];
// CHECK-NEXT:                     let [[T_8]] = [[T_7]].to_array();
// CHECK-NEXT:                     let [[T_9]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_6]][__lane] == [[T_8]][__lane]) as i64)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_9]]
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_cmpunord_pd(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f64, 2> = [[V_0]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f64, 2> = [[V_0]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_1]][__lane] != [[T_3]][__lane]) as i64)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             })
// CHECK-NEXT:                 | {
// CHECK-NEXT:                     let [[T_5]]: std::simd::Simd<f64, 2> = [[V_1]];
// CHECK-NEXT:                     let [[T_6]] = [[T_5]].to_array();
// CHECK-NEXT:                     let [[T_7]]: std::simd::Simd<f64, 2> = [[V_1]];
// CHECK-NEXT:                     let [[T_8]] = [[T_7]].to_array();
// CHECK-NEXT:                     let [[T_9]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_6]][__lane] != [[T_8]][__lane]) as i64)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_9]]
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_cmpneq_pd(
// CHECK-NEXT:     [[V_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >({
// CHECK-NEXT:             let [[T_0]]: std::simd::Simd<f64, 2> = [[V_0]];
// CHECK-NEXT:             let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:             let [[T_2]]: std::simd::Simd<f64, 2> = [[V_1]];
// CHECK-NEXT:             let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:             let [[T_4]]: std::simd::Simd<i64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:                 std::array::from_fn(|__lane| -(([[T_1]][__lane] != [[T_3]][__lane]) as i64)),
// CHECK-NEXT:             );
// CHECK-NEXT:             [[T_4]]
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_cmplt_sd(
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >, std::simd::Simd
// CHECK-NEXT:         < f64, 2 >> ({ let [[T_0]] : std::simd::Simd < f64, 2 > = [[V_0]]; let [[T_1]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2]] : std::simd::Simd < f64, 2 > = [[V_1]]; let [[T_3]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4]] : std::simd::Simd < i64, 2 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] <
// CHECK-NEXT:         [[T_3]][__lane]) as i64))); [[T_4]] }) }, [2, 1]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_cmpord_sd(
// CHECK-NEXT:     mut [[V_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     [[V_1]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]], unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >, std::simd::Simd
// CHECK-NEXT:         < f64, 2 >> (({ let [[T_0]] : std::simd::Simd < f64, 2 > = [[V_0]]; let [[T_1]] = [[T_0]]
// CHECK-NEXT:         .to_array(); let [[T_2]] : std::simd::Simd < f64, 2 > = [[V_0]]; let [[T_3]] = [[T_2]]
// CHECK-NEXT:         .to_array(); let [[T_4]] : std::simd::Simd < i64, 2 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_1]][__lane] ==
// CHECK-NEXT:         [[T_3]][__lane]) as i64))); [[T_4]] }) & { let [[T_5]] : std::simd::Simd < f64, 2 > =
// CHECK-NEXT:         [[V_1]]; let [[T_6]] = [[T_5]].to_array(); let [[T_7]] : std::simd::Simd < f64, 2 > = [[V_1]];
// CHECK-NEXT:         let [[T_8]] = [[T_7]].to_array(); let [[T_9]] : std::simd::Simd < i64, 2 > =
// CHECK-NEXT:         std::simd::Simd::from_array(std::array::from_fn(| __lane | - (([[T_6]][__lane] ==
// CHECK-NEXT:         [[T_8]][__lane]) as i64))); [[T_9]] }) }, [2, 1]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_loadu_pd(__dp: *const f64) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__dp as * const __loadu_pd__1620)).__v).read_unaligned()
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
// CHECK-NEXT: unsafe fn _mm_set_epi16(
// CHECK-NEXT:     __w7: i16,
// CHECK-NEXT:     __w6: i16,
// CHECK-NEXT:     __w5: i16,
// CHECK-NEXT:     __w4: i16,
// CHECK-NEXT:     __w3: i16,
// CHECK-NEXT:     __w2: i16,
// CHECK-NEXT:     __w1: i16,
// CHECK-NEXT:     __w0: i16,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __w0,
// CHECK-NEXT:                     __w1,
// CHECK-NEXT:                     __w2,
// CHECK-NEXT:                     __w3,
// CHECK-NEXT:                     __w4,
// CHECK-NEXT:                     __w5,
// CHECK-NEXT:                     __w6,
// CHECK-NEXT:                     __w7,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: unsafe fn _mm_set1_epi16(__w: i16) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe { _mm_set_epi16(__w, __w, __w, __w, __w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_sqrt_pd(__a: std::simd::Simd<f64, 4>) -> std::simd::Simd<f64, 4> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_8ecdd79d9cc7546f(__a) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_sqrt_ps(__a: std::simd::Simd<f32, 8>) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_fa8a70aaf3988c39(__a) };
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
// CHECK-NEXT: unsafe fn _mm256_undefined_si256() -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<f64, 4>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(std::simd::Simd::splat(0.0f64))
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
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __i7,
// CHECK-NEXT:                     __i6,
// CHECK-NEXT:                     __i5,
// CHECK-NEXT:                     __i4,
// CHECK-NEXT:                     __i3,
// CHECK-NEXT:                     __i2,
// CHECK-NEXT:                     __i1,
// CHECK-NEXT:                     __i0,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set_epi16(
// CHECK-NEXT:     __w15: i16,
// CHECK-NEXT:     __w14: i16,
// CHECK-NEXT:     __w13: i16,
// CHECK-NEXT:     __w12: i16,
// CHECK-NEXT:     __w11: i16,
// CHECK-NEXT:     __w10: i16,
// CHECK-NEXT:     __w09: i16,
// CHECK-NEXT:     __w08: i16,
// CHECK-NEXT:     __w07: i16,
// CHECK-NEXT:     __w06: i16,
// CHECK-NEXT:     __w05: i16,
// CHECK-NEXT:     __w04: i16,
// CHECK-NEXT:     __w03: i16,
// CHECK-NEXT:     __w02: i16,
// CHECK-NEXT:     __w01: i16,
// CHECK-NEXT:     __w00: i16,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i16, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i16, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     __w00,
// CHECK-NEXT:                     __w01,
// CHECK-NEXT:                     __w02,
// CHECK-NEXT:                     __w03,
// CHECK-NEXT:                     __w04,
// CHECK-NEXT:                     __w05,
// CHECK-NEXT:                     __w06,
// CHECK-NEXT:                     __w07,
// CHECK-NEXT:                     __w08,
// CHECK-NEXT:                     __w09,
// CHECK-NEXT:                     __w10,
// CHECK-NEXT:                     __w11,
// CHECK-NEXT:                     __w12,
// CHECK-NEXT:                     __w13,
// CHECK-NEXT:                     __w14,
// CHECK-NEXT:                     __w15,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_pd(__w: f64) -> std::simd::Simd<f64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_pd(__w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_epi32(__i: i32) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi32(__i, __i, __i, __i, __i, __i, __i, __i) };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_epi16(__w: i16) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         _mm256_set_epi16(
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:             __w,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_setzero_si256() -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([0i64, 0i64, 0i64, 0i64]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_castps256_ps128(
// CHECK-NEXT:     mut __a: std::simd::Simd<f32, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, __a, [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_castsi256_si128(
// CHECK-NEXT:     mut __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, __a, [0, 1]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_castsi128_si256(
// CHECK-NEXT:     mut __a: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, std::simd::Simd::splat(0i64), [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_setzero_si512() -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: std::simd::Simd<i64, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 0i64, 0i64, 0i64, 0i64, 0i64, 0i64, 0i64, 0i64,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
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
// CHECK-NEXT: unsafe fn _mm512_castsi256_si512(
// CHECK-NEXT:     mut __A: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(
// CHECK-NEXT:         __A, std::simd::Simd::splat(0i64), [0, 1, 2, 3, 4, 5, 6, 7]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_castsi512_si128(
// CHECK-NEXT:     mut __A: std::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__A, __A, [0, 1]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_castsi512_si256(
// CHECK-NEXT:     mut __A: std::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__A, __A, [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_loadu_si512(__P: *const ()) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__P as * const __loadu_si512)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_mask_loadu_epi32(
// CHECK-NEXT:     __W: std::simd::Simd<i64, 8>,
// CHECK-NEXT:     __U: u16,
// CHECK-NEXT:     __P: *const (),
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 __P as *const i32,
// CHECK-NEXT:                 std::simd::Mask::<i32, 16>::from_bitmask((__U as u16) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 16>,
// CHECK-NEXT:                     >(__W)
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_maskz_loadu_epi32(__U: u16, __P: *const ()) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 __P as *const i32,
// CHECK-NEXT:                 std::simd::Mask::<i32, 16>::from_bitmask((__U as u16) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 16>,
// CHECK-NEXT:                     >(unsafe { _mm512_setzero_si512() })
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_loadu_ps(__p: *const ()) -> std::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::ptr::addr_of!((* (__p as * const __loadu_ps__4153)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_mask_mov_epi32(
// CHECK-NEXT:     __W: std::simd::Simd<i64, 8>,
// CHECK-NEXT:     __U: u16,
// CHECK-NEXT:     __A: std::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Select::select(
// CHECK-NEXT:                 std::simd::Mask::<i32, 16>::from_bitmask((__U as u16) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 16>,
// CHECK-NEXT:                     >(__A)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 16>,
// CHECK-NEXT:                     >(__W)
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vl")]
// CHECK-NEXT: unsafe fn _mm256_maskz_loadu_epi32(__U: u8, __P: *const ()) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i32, 8>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 __P as *const i32,
// CHECK-NEXT:                 std::simd::Mask::<i32, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 4>,
// CHECK-NEXT:                         std::simd::Simd<i32, 8>,
// CHECK-NEXT:                     >(unsafe { _mm256_setzero_si256() })
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _mm512_maskz_loadu_epi8(__U: u64, __P: *const ()) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 64>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 __P as *const i8,
// CHECK-NEXT:                 std::simd::Mask::<i8, 64>::from_bitmask(__U as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i64, 8>,
// CHECK-NEXT:                         std::simd::Simd<i8, 64>,
// CHECK-NEXT:                     >(unsafe { _mm512_setzero_si512() })
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _mm512_movepi8_mask(__A: std::simd::Simd<i64, 8>) -> u64 {
// CHECK-NEXT:     std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 64>,
// CHECK-NEXT:                 >(__A)
// CHECK-NEXT:             },
// CHECK-NEXT:             std::simd::Simd::default(),
// CHECK-NEXT:         )
// CHECK-NEXT:         .to_bitmask() as u64
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _mm512_movm_epi8(__A: u64) -> std::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 64>,
// CHECK-NEXT:             std::simd::Simd<i64, 8>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Select::select(
// CHECK-NEXT:                 std::simd::Mask::<i8, 64>::from_bitmask(__A as u64),
// CHECK-NEXT:                 std::simd::Simd::splat(-1i8),
// CHECK-NEXT:                 std::simd::Simd::splat(0i8),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vl,avx512bw")]
// CHECK-NEXT: unsafe fn _mm256_movm_epi16(__A: u16) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i16, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::Select::select(
// CHECK-NEXT:                 std::simd::Mask::<i16, 16>::from_bitmask((__A as u16) as u64),
// CHECK-NEXT:                 std::simd::Simd::splat(-1i16),
// CHECK-NEXT:                 std::simd::Simd::splat(0i16),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512vl,avx512dq")]
// CHECK-NEXT: unsafe fn _mm_movepi32_mask(__A: std::simd::Simd<i64, 2>) -> u8 {
// CHECK-NEXT:     std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 >(__A)
// CHECK-NEXT:             },
// CHECK-NEXT:             std::simd::Simd::default(),
// CHECK-NEXT:         )
// CHECK-NEXT:         .to_bitmask() as u8
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
// CHECK-NEXT: unsafe fn sse(
// CHECK-NEXT:     fa: *const f32,
// CHECK-NEXT:     fb: *const f32,
// CHECK-NEXT:     da: *const f64,
// CHECK-NEXT:     db: *const f64,
// CHECK-NEXT:     ia: *const i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let mut a: std::simd::Simd<f32, 4> = unsafe { _mm_loadu_ps(fa) };
// CHECK-NEXT:     let mut b: std::simd::Simd<f32, 4> = unsafe { _mm_loadu_ps(fb) };
// CHECK-NEXT:     let mut c: std::simd::Simd<f64, 2> = unsafe { _mm_loadu_pd(da) };
// CHECK-NEXT:     let mut d: std::simd::Simd<f64, 2> = unsafe { _mm_loadu_pd(db) };
// CHECK-NEXT:     let i: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_loadu_si128(ia as *const std::simd::Simd<i64, 2>)
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW!(c"sqrt_ps".as_ptr() as * mut i8, unsafe { _mm_sqrt_ps(a) });
// CHECK-NEXT:     SHOW__4!(c"sqrt_pd".as_ptr() as * mut i8, unsafe { _mm_sqrt_pd(c) });
// CHECK-NEXT:     SHOW!(c"cmpeq_ps".as_ptr() as * mut i8, unsafe { _mm_cmpeq_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmplt_ps".as_ptr() as * mut i8, unsafe { _mm_cmplt_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmple_ps".as_ptr() as * mut i8, unsafe { _mm_cmple_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpgt_ps".as_ptr() as * mut i8, unsafe { _mm_cmpgt_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpge_ps".as_ptr() as * mut i8, unsafe { _mm_cmpge_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpneq_ps".as_ptr() as * mut i8, unsafe { _mm_cmpneq_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpnlt_ps".as_ptr() as * mut i8, unsafe { _mm_cmpnlt_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpnle_ps".as_ptr() as * mut i8, unsafe { _mm_cmpnle_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpngt_ps".as_ptr() as * mut i8, unsafe { _mm_cmpngt_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpnge_ps".as_ptr() as * mut i8, unsafe { _mm_cmpnge_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpord_ps".as_ptr() as * mut i8, unsafe { _mm_cmpord_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpunord_ps".as_ptr() as * mut i8, unsafe { _mm_cmpunord_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpeq_ss".as_ptr() as * mut i8, unsafe { _mm_cmpeq_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmplt_ss".as_ptr() as * mut i8, unsafe { _mm_cmplt_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmple_ss".as_ptr() as * mut i8, unsafe { _mm_cmple_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpgt_ss".as_ptr() as * mut i8, unsafe { _mm_cmpgt_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpneq_ss".as_ptr() as * mut i8, unsafe { _mm_cmpneq_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpnlt_ss".as_ptr() as * mut i8, unsafe { _mm_cmpnlt_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpord_ss".as_ptr() as * mut i8, unsafe { _mm_cmpord_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpunord_ss".as_ptr() as * mut i8, unsafe { _mm_cmpunord_ss(a, b) });
// CHECK-NEXT:     SHOW__4!(c"cmpeq_pd".as_ptr() as * mut i8, unsafe { _mm_cmpeq_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmplt_pd".as_ptr() as * mut i8, unsafe { _mm_cmplt_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmpneq_pd".as_ptr() as * mut i8, unsafe { _mm_cmpneq_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmpord_pd".as_ptr() as * mut i8, unsafe { _mm_cmpord_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmpunord_pd".as_ptr() as * mut i8, unsafe { _mm_cmpunord_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmplt_sd".as_ptr() as * mut i8, unsafe { _mm_cmplt_sd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmpord_sd".as_ptr() as * mut i8, unsafe { _mm_cmpord_sd(c, d) });
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"insert_epi32".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 4 >, std::simd::Simd < i64, 2 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         2 >, std::simd::Simd < i32, 4 >> (i) }, std::simd::Simd::splat(77i32), [0, 1, 6,
// CHECK-NEXT:         3])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"insert_epi16".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 8 >, std::simd::Simd < i64, 2 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         2 >, std::simd::Simd < i16, 8 >> (i) }, std::simd::Simd::splat(- 5i16), [0, 1, 2,
// CHECK-NEXT:         3, 4, 5, 6, 15])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"insert_epi8".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i8, 16 >, std::simd::Simd < i64, 2 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         2 >, std::simd::Simd < i8, 16 >> (i) }, std::simd::Simd::splat(9i8), [0, 1, 2, 3,
// CHECK-NEXT:         4, 5, 6, 7, 8, 9, 10, 11, 12, 29, 14, 15])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "extract_epi32 {} {}", unsafe { std::mem::transmute::< std::simd::Simd < i64, 2
// CHECK-NEXT:         >, std::simd::Simd < i32, 4 >> (i) } [1], unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i32, 4 >> (i) } [3]
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "extract_epi16 {}", (unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >,
// CHECK-NEXT:         std::simd::Simd < i16, 8 >> (i) } [5] as u16) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "extract_epi8 {}", (unsafe { std::mem::transmute::< std::simd::Simd < i64, 2 >,
// CHECK-NEXT:         std::simd::Simd < i8, 16 >> (i) } [6] as u8) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"blend_epi16".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 8 >, std::simd::Simd < i64, 2 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         2 >, std::simd::Simd < i16, 8 >> (i) }, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i16, 8 >> (unsafe {
// CHECK-NEXT:         _mm_set1_epi16(- 1i16) }) }, [8, 1, 10, 3, 4, 13, 6, 15])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"blend_ps".as_ptr() as * mut i8, std::simd::simd_swizzle!(a, b, [0, 5, 6, 3])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__4!(c"blend_pd".as_ptr() as * mut i8, std::simd::simd_swizzle!(c, d, [0, 3]));
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"srli0".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         i8, 16 >, std::simd::Simd < i64, 2 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < u8, 16 >, std::simd::Simd < i8, 16 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i8,
// CHECK-NEXT:         16 >, std::simd::Simd < u8, 16 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i8, 16 >> (i) }) },
// CHECK-NEXT:         std::simd::Simd::splat(0u8), [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
// CHECK-NEXT:         15])) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"srli3".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         i8, 16 >, std::simd::Simd < i64, 2 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < u8, 16 >, std::simd::Simd < i8, 16 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i8,
// CHECK-NEXT:         16 >, std::simd::Simd < u8, 16 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i8, 16 >> (i) }) },
// CHECK-NEXT:         std::simd::Simd::splat(0u8), [3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 29,
// CHECK-NEXT:         30, 31])) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"srli15".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         i8, 16 >, std::simd::Simd < i64, 2 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < u8, 16 >, std::simd::Simd < i8, 16 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i8,
// CHECK-NEXT:         16 >, std::simd::Simd < u8, 16 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i8, 16 >> (i) }) },
// CHECK-NEXT:         std::simd::Simd::splat(0u8), [15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
// CHECK-NEXT:         29, 30, 31])) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"srli16".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         i8, 16 >, std::simd::Simd < i64, 2 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < u8, 16 >, std::simd::Simd < i8, 16 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i8,
// CHECK-NEXT:         16 >, std::simd::Simd < u8, 16 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i8, 16 >> (i) }) },
// CHECK-NEXT:         std::simd::Simd::splat(0u8), [16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
// CHECK-NEXT:         29, 30, 31])) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"slli5".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         i8, 16 >, std::simd::Simd < i64, 2 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < u8, 16 >, std::simd::Simd < i8, 16 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i8,
// CHECK-NEXT:         16 >, std::simd::Simd < u8, 16 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i8, 16 >> (i) }) },
// CHECK-NEXT:         std::simd::Simd::splat(0u8), [16, 17, 18, 19, 20, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
// CHECK-NEXT:         10])) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"slli20".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd <
// CHECK-NEXT:         i8, 16 >, std::simd::Simd < i64, 2 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < u8, 16 >, std::simd::Simd < i8, 16 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i8,
// CHECK-NEXT:         16 >, std::simd::Simd < u8, 16 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 2 >, std::simd::Simd < i8, 16 >> (i) }) },
// CHECK-NEXT:         std::simd::Simd::splat(0u8), [16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28,
// CHECK-NEXT:         29, 30, 31])) }) }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn avx(fa: *const f32, fb: *const f32, da: *const f64, ia: *const i32) {
// CHECK-NEXT:     let a: std::simd::Simd<f32, 8> = unsafe { _mm256_loadu_ps(fa) };
// CHECK-NEXT:     let b: std::simd::Simd<f32, 8> = unsafe { _mm256_loadu_ps(fb) };
// CHECK-NEXT:     let mut c: std::simd::Simd<f64, 4> = unsafe { _mm256_loadu_pd(da) };
// CHECK-NEXT:     let mut i: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(ia as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW__8!(c"sqrt256_ps".as_ptr() as * mut i8, unsafe { _mm256_sqrt_ps(a) });
// CHECK-NEXT:     SHOW__7!(c"sqrt256_pd".as_ptr() as * mut i8, unsafe { _mm256_sqrt_pd(c) });
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_2:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_3:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_0]]: std::simd::Simd<f32, 8> = [[V_2]];
// CHECK-NEXT:                 let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: std::simd::Simd<f32, 8> = [[V_3]];
// CHECK-NEXT:                 let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_1]][__lane] == [[T_3]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_0".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_4:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_5:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_5]]: std::simd::Simd<f32, 8> = [[V_4]];
// CHECK-NEXT:                 let [[T_6]] = [[T_5]].to_array();
// CHECK-NEXT:                 let [[T_7]]: std::simd::Simd<f32, 8> = [[V_5]];
// CHECK-NEXT:                 let [[T_8]] = [[T_7]].to_array();
// CHECK-NEXT:                 let [[T_9]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| -(([[T_6]][__lane] < [[T_8]][__lane]) as i32)),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_9]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_1".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_6:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_7:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_10:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_6]];
// CHECK-NEXT:                 let [[T_11:__t[0-9]+]] = [[T_10]].to_array();
// CHECK-NEXT:                 let [[T_12:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_7]];
// CHECK-NEXT:                 let [[T_13:__t[0-9]+]] = [[T_12]].to_array();
// CHECK-NEXT:                 let [[T_14:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_11]][__lane] <= [[T_13]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_14]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_2".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_8:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_9:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_15:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_8]];
// CHECK-NEXT:                     let [[T_16:__t[0-9]+]] = [[T_15]].to_array();
// CHECK-NEXT:                     let [[T_17:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_8]];
// CHECK-NEXT:                     let [[T_18:__t[0-9]+]] = [[T_17]].to_array();
// CHECK-NEXT:                     let [[T_19:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_16]][__lane] != [[T_18]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_19]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | {
// CHECK-NEXT:                         let [[T_20:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_9]];
// CHECK-NEXT:                         let [[T_21:__t[0-9]+]] = [[T_20]].to_array();
// CHECK-NEXT:                         let [[T_22:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_9]];
// CHECK-NEXT:                         let [[T_23:__t[0-9]+]] = [[T_22]].to_array();
// CHECK-NEXT:                         let [[T_24:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_21]][__lane] != [[T_23]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_24]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_3".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_10:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_11:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_25:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_10]];
// CHECK-NEXT:                 let [[T_26:__t[0-9]+]] = [[T_25]].to_array();
// CHECK-NEXT:                 let [[T_27:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_11]];
// CHECK-NEXT:                 let [[T_28:__t[0-9]+]] = [[T_27]].to_array();
// CHECK-NEXT:                 let [[T_29:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_26]][__lane] != [[T_28]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_29]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_4".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_12:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_13:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_30:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_12]];
// CHECK-NEXT:                     let [[T_31:__t[0-9]+]] = [[T_30]].to_array();
// CHECK-NEXT:                     let [[T_32:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_13]];
// CHECK-NEXT:                     let [[T_33:__t[0-9]+]] = [[T_32]].to_array();
// CHECK-NEXT:                     let [[T_34:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_31]][__lane] < [[T_33]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_34]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_5".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_14:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_15:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_35:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_14]];
// CHECK-NEXT:                     let [[T_36:__t[0-9]+]] = [[T_35]].to_array();
// CHECK-NEXT:                     let [[T_37:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_15]];
// CHECK-NEXT:                     let [[T_38:__t[0-9]+]] = [[T_37]].to_array();
// CHECK-NEXT:                     let [[T_39:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_36]][__lane] <= [[T_38]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_39]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_6".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_16:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_17:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_40:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_16]];
// CHECK-NEXT:                     let [[T_41:__t[0-9]+]] = [[T_40]].to_array();
// CHECK-NEXT:                     let [[T_42:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_16]];
// CHECK-NEXT:                     let [[T_43:__t[0-9]+]] = [[T_42]].to_array();
// CHECK-NEXT:                     let [[T_44:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_41]][__lane] == [[T_43]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_44]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & {
// CHECK-NEXT:                         let [[T_45:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_17]];
// CHECK-NEXT:                         let [[T_46:__t[0-9]+]] = [[T_45]].to_array();
// CHECK-NEXT:                         let [[T_47:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_17]];
// CHECK-NEXT:                         let [[T_48:__t[0-9]+]] = [[T_47]].to_array();
// CHECK-NEXT:                         let [[T_49:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_46]][__lane] == [[T_48]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_49]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_7".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_18:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_19:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_50:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_18]];
// CHECK-NEXT:                     let [[T_51:__t[0-9]+]] = [[T_50]].to_array();
// CHECK-NEXT:                     let [[T_52:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_19]];
// CHECK-NEXT:                     let [[T_53:__t[0-9]+]] = [[T_52]].to_array();
// CHECK-NEXT:                     let [[T_54:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_51]][__lane] == [[T_53]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_54]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | (({
// CHECK-NEXT:                         let [[T_55:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_18]];
// CHECK-NEXT:                         let [[T_56:__t[0-9]+]] = [[T_55]].to_array();
// CHECK-NEXT:                         let [[T_57:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_18]];
// CHECK-NEXT:                         let [[T_58:__t[0-9]+]] = [[T_57]].to_array();
// CHECK-NEXT:                         let [[T_59:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_56]][__lane] != [[T_58]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_59]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_60:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_19]];
// CHECK-NEXT:                             let [[T_61:__t[0-9]+]] = [[T_60]].to_array();
// CHECK-NEXT:                             let [[T_62:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_19]];
// CHECK-NEXT:                             let [[T_63:__t[0-9]+]] = [[T_62]].to_array();
// CHECK-NEXT:                             let [[T_64:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_61]][__lane] != [[T_63]][__lane]) as i32)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_64]]
// CHECK-NEXT:                         }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_8".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_20:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_21:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_65:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_20]];
// CHECK-NEXT:                     let [[T_66:__t[0-9]+]] = [[T_65]].to_array();
// CHECK-NEXT:                     let [[T_67:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_21]];
// CHECK-NEXT:                     let [[T_68:__t[0-9]+]] = [[T_67]].to_array();
// CHECK-NEXT:                     let [[T_69:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_66]][__lane] >= [[T_68]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_69]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_9".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_22:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_23:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_70:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_22]];
// CHECK-NEXT:                     let [[T_71:__t[0-9]+]] = [[T_70]].to_array();
// CHECK-NEXT:                     let [[T_72:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_23]];
// CHECK-NEXT:                     let [[T_73:__t[0-9]+]] = [[T_72]].to_array();
// CHECK-NEXT:                     let [[T_74:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_71]][__lane] > [[T_73]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_74]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_10".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"cmp_ps_11".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 8 >, std::simd::Simd < f32, 8 >>
// CHECK-NEXT:         (std::simd::Simd::splat(0i32)) }, a, b
// CHECK-NEXT:     );
// CHECK-NEXT:     '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_24:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_25:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_75:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_24]];
// CHECK-NEXT:                     let [[T_76:__t[0-9]+]] = [[T_75]].to_array();
// CHECK-NEXT:                     let [[T_77:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_25]];
// CHECK-NEXT:                     let [[T_78:__t[0-9]+]] = [[T_77]].to_array();
// CHECK-NEXT:                     let [[T_79:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_76]][__lane] != [[T_78]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_79]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & (({
// CHECK-NEXT:                         let [[T_80:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_24]];
// CHECK-NEXT:                         let [[T_81:__t[0-9]+]] = [[T_80]].to_array();
// CHECK-NEXT:                         let [[T_82:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_24]];
// CHECK-NEXT:                         let [[T_83:__t[0-9]+]] = [[T_82]].to_array();
// CHECK-NEXT:                         let [[T_84:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_81]][__lane] == [[T_83]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_84]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         & {
// CHECK-NEXT:                             let [[T_85:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_25]];
// CHECK-NEXT:                             let [[T_86:__t[0-9]+]] = [[T_85]].to_array();
// CHECK-NEXT:                             let [[T_87:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_25]];
// CHECK-NEXT:                             let [[T_88:__t[0-9]+]] = [[T_87]].to_array();
// CHECK-NEXT:                             let [[T_89:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_86]][__lane] == [[T_88]][__lane]) as i32)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_89]]
// CHECK-NEXT:                         }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_12".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_26:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_27:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_90:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_26]];
// CHECK-NEXT:                 let [[T_91:__t[0-9]+]] = [[T_90]].to_array();
// CHECK-NEXT:                 let [[T_92:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_27]];
// CHECK-NEXT:                 let [[T_93:__t[0-9]+]] = [[T_92]].to_array();
// CHECK-NEXT:                 let [[T_94:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_91]][__lane] >= [[T_93]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_94]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_13".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_28:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_29:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_95:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_28]];
// CHECK-NEXT:                 let [[T_96:__t[0-9]+]] = [[T_95]].to_array();
// CHECK-NEXT:                 let [[T_97:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_29]];
// CHECK-NEXT:                 let [[T_98:__t[0-9]+]] = [[T_97]].to_array();
// CHECK-NEXT:                 let [[T_99:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_96]][__lane] > [[T_98]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_99]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_14".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"cmp_ps_15".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 8 >, std::simd::Simd < f32, 8 >> (!
// CHECK-NEXT:         std::simd::Simd::splat(0i32)) }, a, b
// CHECK-NEXT:     );
// CHECK-NEXT:     '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_30:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_31:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_100:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_30]];
// CHECK-NEXT:                 let [[T_101:__t[0-9]+]] = [[T_100]].to_array();
// CHECK-NEXT:                 let [[T_102:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_31]];
// CHECK-NEXT:                 let [[T_103:__t[0-9]+]] = [[T_102]].to_array();
// CHECK-NEXT:                 let [[T_104:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_101]][__lane] == [[T_103]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_104]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_16".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_32:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_33:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_105:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_32]];
// CHECK-NEXT:                 let [[T_106:__t[0-9]+]] = [[T_105]].to_array();
// CHECK-NEXT:                 let [[T_107:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_33]];
// CHECK-NEXT:                 let [[T_108:__t[0-9]+]] = [[T_107]].to_array();
// CHECK-NEXT:                 let [[T_109:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_106]][__lane] < [[T_108]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_109]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_17".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_34:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_35:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_110:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_34]];
// CHECK-NEXT:                 let [[T_111:__t[0-9]+]] = [[T_110]].to_array();
// CHECK-NEXT:                 let [[T_112:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_35]];
// CHECK-NEXT:                 let [[T_113:__t[0-9]+]] = [[T_112]].to_array();
// CHECK-NEXT:                 let [[T_114:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_111]][__lane] <= [[T_113]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_114]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_18".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_36:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_37:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_115:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_36]];
// CHECK-NEXT:                     let [[T_116:__t[0-9]+]] = [[T_115]].to_array();
// CHECK-NEXT:                     let [[T_117:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_36]];
// CHECK-NEXT:                     let [[T_118:__t[0-9]+]] = [[T_117]].to_array();
// CHECK-NEXT:                     let [[T_119:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_116]][__lane] != [[T_118]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_119]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | {
// CHECK-NEXT:                         let [[T_120:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_37]];
// CHECK-NEXT:                         let [[T_121:__t[0-9]+]] = [[T_120]].to_array();
// CHECK-NEXT:                         let [[T_122:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_37]];
// CHECK-NEXT:                         let [[T_123:__t[0-9]+]] = [[T_122]].to_array();
// CHECK-NEXT:                         let [[T_124:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_121]][__lane] != [[T_123]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_124]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_19".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_38:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_39:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_125:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_38]];
// CHECK-NEXT:                 let [[T_126:__t[0-9]+]] = [[T_125]].to_array();
// CHECK-NEXT:                 let [[T_127:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_39]];
// CHECK-NEXT:                 let [[T_128:__t[0-9]+]] = [[T_127]].to_array();
// CHECK-NEXT:                 let [[T_129:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_126]][__lane] != [[T_128]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_129]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_20".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_19:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_40:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_41:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_130:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_40]];
// CHECK-NEXT:                     let [[T_131:__t[0-9]+]] = [[T_130]].to_array();
// CHECK-NEXT:                     let [[T_132:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_41]];
// CHECK-NEXT:                     let [[T_133:__t[0-9]+]] = [[T_132]].to_array();
// CHECK-NEXT:                     let [[T_134:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_131]][__lane] < [[T_133]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_134]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_21".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_20:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_42:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_43:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_135:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_42]];
// CHECK-NEXT:                     let [[T_136:__t[0-9]+]] = [[T_135]].to_array();
// CHECK-NEXT:                     let [[T_137:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_43]];
// CHECK-NEXT:                     let [[T_138:__t[0-9]+]] = [[T_137]].to_array();
// CHECK-NEXT:                     let [[T_139:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_136]][__lane] <= [[T_138]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_139]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_22".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_21:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_44:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_45:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_140:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_44]];
// CHECK-NEXT:                     let [[T_141:__t[0-9]+]] = [[T_140]].to_array();
// CHECK-NEXT:                     let [[T_142:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_44]];
// CHECK-NEXT:                     let [[T_143:__t[0-9]+]] = [[T_142]].to_array();
// CHECK-NEXT:                     let [[T_144:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_141]][__lane] == [[T_143]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_144]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & {
// CHECK-NEXT:                         let [[T_145:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_45]];
// CHECK-NEXT:                         let [[T_146:__t[0-9]+]] = [[T_145]].to_array();
// CHECK-NEXT:                         let [[T_147:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_45]];
// CHECK-NEXT:                         let [[T_148:__t[0-9]+]] = [[T_147]].to_array();
// CHECK-NEXT:                         let [[T_149:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_146]][__lane] == [[T_148]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_149]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_23".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_22:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_46:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_47:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_150:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_46]];
// CHECK-NEXT:                     let [[T_151:__t[0-9]+]] = [[T_150]].to_array();
// CHECK-NEXT:                     let [[T_152:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_47]];
// CHECK-NEXT:                     let [[T_153:__t[0-9]+]] = [[T_152]].to_array();
// CHECK-NEXT:                     let [[T_154:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_151]][__lane] == [[T_153]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_154]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | (({
// CHECK-NEXT:                         let [[T_155:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_46]];
// CHECK-NEXT:                         let [[T_156:__t[0-9]+]] = [[T_155]].to_array();
// CHECK-NEXT:                         let [[T_157:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_46]];
// CHECK-NEXT:                         let [[T_158:__t[0-9]+]] = [[T_157]].to_array();
// CHECK-NEXT:                         let [[T_159:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_156]][__lane] != [[T_158]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_159]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_160:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_47]];
// CHECK-NEXT:                             let [[T_161:__t[0-9]+]] = [[T_160]].to_array();
// CHECK-NEXT:                             let [[T_162:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_47]];
// CHECK-NEXT:                             let [[T_163:__t[0-9]+]] = [[T_162]].to_array();
// CHECK-NEXT:                             let [[T_164:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_161]][__lane] != [[T_163]][__lane]) as i32)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_164]]
// CHECK-NEXT:                         }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_24".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_23:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_48:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_49:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_165:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_48]];
// CHECK-NEXT:                     let [[T_166:__t[0-9]+]] = [[T_165]].to_array();
// CHECK-NEXT:                     let [[T_167:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_49]];
// CHECK-NEXT:                     let [[T_168:__t[0-9]+]] = [[T_167]].to_array();
// CHECK-NEXT:                     let [[T_169:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_166]][__lane] >= [[T_168]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_169]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_25".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_24:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_50:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_51:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_170:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_50]];
// CHECK-NEXT:                     let [[T_171:__t[0-9]+]] = [[T_170]].to_array();
// CHECK-NEXT:                     let [[T_172:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_51]];
// CHECK-NEXT:                     let [[T_173:__t[0-9]+]] = [[T_172]].to_array();
// CHECK-NEXT:                     let [[T_174:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_171]][__lane] > [[T_173]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_174]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_26".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"cmp_ps_27".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 8 >, std::simd::Simd < f32, 8 >>
// CHECK-NEXT:         (std::simd::Simd::splat(0i32)) }, a, b
// CHECK-NEXT:     );
// CHECK-NEXT:     '[[SLATE_BREAK_25:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_52:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_53:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_175:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_52]];
// CHECK-NEXT:                     let [[T_176:__t[0-9]+]] = [[T_175]].to_array();
// CHECK-NEXT:                     let [[T_177:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_53]];
// CHECK-NEXT:                     let [[T_178:__t[0-9]+]] = [[T_177]].to_array();
// CHECK-NEXT:                     let [[T_179:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_176]][__lane] != [[T_178]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_179]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & (({
// CHECK-NEXT:                         let [[T_180:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_52]];
// CHECK-NEXT:                         let [[T_181:__t[0-9]+]] = [[T_180]].to_array();
// CHECK-NEXT:                         let [[T_182:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_52]];
// CHECK-NEXT:                         let [[T_183:__t[0-9]+]] = [[T_182]].to_array();
// CHECK-NEXT:                         let [[T_184:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_181]][__lane] == [[T_183]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_184]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         & {
// CHECK-NEXT:                             let [[T_185:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_53]];
// CHECK-NEXT:                             let [[T_186:__t[0-9]+]] = [[T_185]].to_array();
// CHECK-NEXT:                             let [[T_187:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_53]];
// CHECK-NEXT:                             let [[T_188:__t[0-9]+]] = [[T_187]].to_array();
// CHECK-NEXT:                             let [[T_189:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_186]][__lane] == [[T_188]][__lane]) as i32)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_189]]
// CHECK-NEXT:                         }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_28".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_26:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_54:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_55:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_190:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_54]];
// CHECK-NEXT:                 let [[T_191:__t[0-9]+]] = [[T_190]].to_array();
// CHECK-NEXT:                 let [[T_192:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_55]];
// CHECK-NEXT:                 let [[T_193:__t[0-9]+]] = [[T_192]].to_array();
// CHECK-NEXT:                 let [[T_194:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_191]][__lane] >= [[T_193]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_194]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_29".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_27:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_56:__v[0-9]+]]: std::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let [[V_57:__v[0-9]+]]: std::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 8>,
// CHECK-NEXT:                 std::simd::Simd<f32, 8>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_195:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_56]];
// CHECK-NEXT:                 let [[T_196:__t[0-9]+]] = [[T_195]].to_array();
// CHECK-NEXT:                 let [[T_197:__t[0-9]+]]: std::simd::Simd<f32, 8> = [[V_57]];
// CHECK-NEXT:                 let [[T_198:__t[0-9]+]] = [[T_197]].to_array();
// CHECK-NEXT:                 let [[T_199:__t[0-9]+]]: std::simd::Simd<i32, 8> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_196]][__lane] > [[T_198]][__lane]) as i32)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_199]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_30".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"cmp_ps_31".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 8 >, std::simd::Simd < f32, 8 >> (!
// CHECK-NEXT:         std::simd::Simd::splat(0i32)) }, a, b
// CHECK-NEXT:     );
// CHECK-NEXT:     '[[SLATE_BREAK_28:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_58:__v[0-9]+]]: std::simd::Simd<f64, 4> = c;
// CHECK-NEXT:         let [[V_59:__v[0-9]+]]: std::simd::Simd<f64, 4> = unsafe { _mm256_set1_pd(2.0f64) };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i64, 4>,
// CHECK-NEXT:                 std::simd::Simd<f64, 4>,
// CHECK-NEXT:             >({
// CHECK-NEXT:                 let [[T_200:__t[0-9]+]]: std::simd::Simd<f64, 4> = [[V_58]];
// CHECK-NEXT:                 let [[T_201:__t[0-9]+]] = [[T_200]].to_array();
// CHECK-NEXT:                 let [[T_202:__t[0-9]+]]: std::simd::Simd<f64, 4> = [[V_59]];
// CHECK-NEXT:                 let [[T_203:__t[0-9]+]] = [[T_202]].to_array();
// CHECK-NEXT:                 let [[T_204:__t[0-9]+]]: std::simd::Simd<i64, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                     std::array::from_fn(|__lane| {
// CHECK-NEXT:                         -(([[T_201]][__lane] < [[T_203]][__lane]) as i64)
// CHECK-NEXT:                     }),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_204]]
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_pd_lt".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_29:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_60:__v[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { _mm256_castps256_ps128(a) };
// CHECK-NEXT:         let [[V_61:__v[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { _mm256_castps256_ps128(b) };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_205:__t[0-9]+]]: std::simd::Simd<f32, 4> = [[V_60]];
// CHECK-NEXT:                     let [[T_206:__t[0-9]+]] = [[T_205]].to_array();
// CHECK-NEXT:                     let [[T_207:__t[0-9]+]]: std::simd::Simd<f32, 4> = [[V_61]];
// CHECK-NEXT:                     let [[T_208:__t[0-9]+]] = [[T_207]].to_array();
// CHECK-NEXT:                     let [[T_209:__t[0-9]+]]: std::simd::Simd<i32, 4> = std::simd::Simd::from_array(
// CHECK-NEXT:                         std::array::from_fn(|__lane| {
// CHECK-NEXT:                             -(([[T_206]][__lane] >= [[T_208]][__lane]) as i32)
// CHECK-NEXT:                         }),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_209]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp128_ps_nge".as_ptr(), std::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"permute4x64_epi64".as_ptr() as * mut i8, std::simd::simd_swizzle!(i, [3, 2, 1,
// CHECK-NEXT:         0])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__7!(
// CHECK-NEXT:         c"permute4x64_pd".as_ptr() as * mut i8, std::simd::simd_swizzle!(c, [2, 3, 0, 1])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"blend_epi32".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 8 >, std::simd::Simd < i64, 4 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         4 >, std::simd::Simd < i32, 8 >> (i) }, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 4 >, std::simd::Simd < i32, 8 >> (unsafe {
// CHECK-NEXT:         _mm256_set1_epi32(- 1i32) }) }, [8, 9, 2, 3, 12, 5, 6, 15])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"blend256_epi16".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 16 >, std::simd::Simd < i64, 4 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         4 >, std::simd::Simd < i16, 16 >> (i) }, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 4 >, std::simd::Simd < i16, 16 >> (unsafe {
// CHECK-NEXT:         _mm256_set1_epi16(7i16) }) }, [16, 17, 18, 19, 4, 5, 6, 7, 24, 25, 26, 27, 12,
// CHECK-NEXT:         13, 14, 15])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__7!(
// CHECK-NEXT:         c"blend256_pd".as_ptr() as * mut i8, std::simd::simd_swizzle!(c, unsafe {
// CHECK-NEXT:         _mm256_set1_pd(9.0f64) }, [4, 1, 6, 3])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"srli256".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd
// CHECK-NEXT:         < i8, 32 >, std::simd::Simd < i64, 4 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < u8, 32 >, std::simd::Simd < i8, 32 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i8,
// CHECK-NEXT:         32 >, std::simd::Simd < u8, 32 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 4 >, std::simd::Simd < i8, 32 >> (i) }) },
// CHECK-NEXT:         std::simd::Simd::splat(0u8), [4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 44, 45,
// CHECK-NEXT:         46, 47, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 60, 61, 62, 63])) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"slli256".as_ptr() as * mut i8, unsafe { std::mem::transmute::< std::simd::Simd
// CHECK-NEXT:         < i8, 32 >, std::simd::Simd < i64, 4 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < u8, 32 >, std::simd::Simd < i8, 32 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i8,
// CHECK-NEXT:         32 >, std::simd::Simd < u8, 32 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 4 >, std::simd::Simd < i8, 32 >> (i) }) },
// CHECK-NEXT:         std::simd::Simd::splat(0u8), [32, 33, 34, 35, 36, 37, 38, 39, 40, 0, 1, 2, 3, 4,
// CHECK-NEXT:         5, 6, 48, 49, 50, 51, 52, 53, 54, 55, 56, 16, 17, 18, 19, 20, 21, 22])) }) }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "extract256_epi32 {}", unsafe { std::mem::transmute::< std::simd::Simd < i64, 4
// CHECK-NEXT:         >, std::simd::Simd < i32, 8 >> (i) } [6]
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { slate_support::printf(c"extract256_epi64 %lld\n".as_ptr(), i[3]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"insert256_epi32".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 8 >, std::simd::Simd < i64, 4 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         4 >, std::simd::Simd < i32, 8 >> (i) }, std::simd::Simd::splat(1234i32), [0, 1,
// CHECK-NEXT:         2, 3, 4, 13, 6, 7])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let wide: std::simd::Simd<i64, 4> = std::simd::simd_swizzle!(
// CHECK-NEXT:         unsafe { _mm256_castsi128_si256(_mm256_castsi256_si128(i)) },
// CHECK-NEXT:         std::simd::simd_swizzle!(std::simd::simd_swizzle!(i, [2, 3]), [0, 1, 0, 0]), [0,
// CHECK-NEXT:         1, 4, 5]
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(c"cast128_256".as_ptr() as * mut i8, wide);
// CHECK-NEXT:     let undef: std::simd::Simd<i64, 4> = unsafe { _mm256_undefined_si256() };
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"undef_blend".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i32, 8 >, std::simd::Simd < i64, 4 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         4 >, std::simd::Simd < i32, 8 >> (undef) }, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i64, 4 >, std::simd::Simd < i32, 8 >> (i) }, [8, 9, 10, 11, 12,
// CHECK-NEXT:         13, 14, 15])) }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f,avx512bw,avx512dq,avx512vl")]
// CHECK-NEXT: unsafe fn avx512(
// CHECK-NEXT:     fa: *const f32,
// CHECK-NEXT:     fb: *const f32,
// CHECK-NEXT:     ia: *const i32,
// CHECK-NEXT:     bytes: *const i8,
// CHECK-NEXT:     tail: i32,
// CHECK-NEXT: ) {
// CHECK-NEXT:     let a: std::simd::Simd<f32, 16> = unsafe { _mm512_loadu_ps(fa as *const ()) };
// CHECK-NEXT:     let b: std::simd::Simd<f32, 16> = unsafe { _mm512_loadu_ps(fb as *const ()) };
// CHECK-NEXT:     let [[V_0]]: std::simd::Simd<f32, 16> = a;
// CHECK-NEXT:     let [[V_1]]: std::simd::Simd<f32, 16> = b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp512_lt %04x\n".as_ptr(),
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_0]]: std::simd::Simd<f32, 16> = [[V_0]];
// CHECK-NEXT:                         let [[T_1]] = [[T_0]].to_array();
// CHECK-NEXT:                         let [[T_2]]: std::simd::Simd<f32, 16> = [[V_1]];
// CHECK-NEXT:                         let [[T_3]] = [[T_2]].to_array();
// CHECK-NEXT:                         let [[T_4]]: std::simd::Simd<i32, 16> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_1]][__lane] < [[T_3]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_4]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u16) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_2]]: std::simd::Simd<f32, 16> = a;
// CHECK-NEXT:     let [[V_3]]: std::simd::Simd<f32, 16> = b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp512_unord %04x\n".as_ptr(),
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_5]]: std::simd::Simd<f32, 16> = [[V_2]];
// CHECK-NEXT:                         let [[T_6]] = [[T_5]].to_array();
// CHECK-NEXT:                         let [[T_7]]: std::simd::Simd<f32, 16> = [[V_2]];
// CHECK-NEXT:                         let [[T_8]] = [[T_7]].to_array();
// CHECK-NEXT:                         let [[T_9]]: std::simd::Simd<i32, 16> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_6]][__lane] != [[T_8]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_9]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_10]]: std::simd::Simd<f32, 16> = [[V_3]];
// CHECK-NEXT:                             let [[T_11]] = [[T_10]].to_array();
// CHECK-NEXT:                             let [[T_12]]: std::simd::Simd<f32, 16> = [[V_3]];
// CHECK-NEXT:                             let [[T_13]] = [[T_12]].to_array();
// CHECK-NEXT:                             let [[T_14]]: std::simd::Simd<i32, 16> = std::simd::Simd::from_array(
// CHECK-NEXT:                                 std::array::from_fn(|__lane| {
// CHECK-NEXT:                                     -(([[T_11]][__lane] != [[T_13]][__lane]) as i32)
// CHECK-NEXT:                                 }),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_14]]
// CHECK-NEXT:                         },
// CHECK-NEXT:                     std::simd::Simd::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u16) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_62:__v[0-9]+]]: std::simd::Simd<f32, 16> = a;
// CHECK-NEXT:     let [[V_4]]: std::simd::Simd<f32, 16> = b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp512_nle %04x\n".as_ptr(),
// CHECK-NEXT:             (std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     !{
// CHECK-NEXT:                         let [[T_15]]: std::simd::Simd<f32, 16> = [[V_62]];
// CHECK-NEXT:                         let [[T_16]] = [[T_15]].to_array();
// CHECK-NEXT:                         let [[T_17]]: std::simd::Simd<f32, 16> = [[V_4]];
// CHECK-NEXT:                         let [[T_18]] = [[T_17]].to_array();
// CHECK-NEXT:                         let [[T_19]]: std::simd::Simd<i32, 16> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_16]][__lane] <= [[T_18]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_19]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u16) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let [[V_5]]: std::simd::Simd<f32, 16> = a;
// CHECK-NEXT:     let [[V_63:__v[0-9]+]]: std::simd::Simd<f32, 16> = b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"mask_cmp512_eq %04x\n".as_ptr(),
// CHECK-NEXT:             ((std::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_20]]: std::simd::Simd<f32, 16> = [[V_5]];
// CHECK-NEXT:                         let [[T_21]] = [[T_20]].to_array();
// CHECK-NEXT:                         let [[T_22]]: std::simd::Simd<f32, 16> = [[V_63]];
// CHECK-NEXT:                         let [[T_23]] = [[T_22]].to_array();
// CHECK-NEXT:                         let [[T_24]]: std::simd::Simd<i32, 16> = std::simd::Simd::from_array(
// CHECK-NEXT:                             std::array::from_fn(|__lane| {
// CHECK-NEXT:                                 -(([[T_21]][__lane] == [[T_23]][__lane]) as i32)
// CHECK-NEXT:                             }),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_24]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     std::simd::Simd::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u16) & 23130) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let tail_mask: u16 = (1u32 << tail).wrapping_sub(1) as u16;
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"maskz_loadu_epi32".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         _mm512_maskz_loadu_epi32(tail_mask, ia as * const ()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"mask_loadu_epi32".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         _mm512_mask_loadu_epi32(_mm512_set1_epi32(- 3i32), tail_mask, ia as * const ()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"maskz_loadu256_epi32".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         _mm256_maskz_loadu_epi32(tail_mask as u8, ia as * const ()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"maskz_loadu_epi8".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         _mm512_maskz_loadu_epi8(71777214294589695u64, bytes as * const ()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut v: std::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         _mm512_loadu_si512(bytes as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"movepi8_mask %016llx\n".as_ptr(), _mm512_movepi8_mask(v))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"movepi32_mask %02x\n".as_ptr(),
// CHECK-NEXT:             _mm_movepi32_mask(_mm512_castsi512_si128(v)) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"movm_epi8".as_ptr() as * mut i8, unsafe {
// CHECK-NEXT:         _mm512_movm_epi8(9223372041149743107u64) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"movm256_epi16".as_ptr() as * mut i8, unsafe { _mm256_movm_epi16(33825u16) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let undef: std::simd::Simd<i64, 8> = unsafe { _mm512_undefined_epi32() };
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"undef512_mov".as_ptr() as * mut i8, unsafe { _mm512_mask_mov_epi32(undef,
// CHECK-NEXT:         65535u16, v) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__9!(
// CHECK-NEXT:         c"cast256_512".as_ptr() as * mut i8, std::simd::simd_swizzle!(unsafe {
// CHECK-NEXT:         _mm512_castsi256_si512(_mm512_castsi512_si256(v)) },
// CHECK-NEXT:         std::simd::simd_swizzle!(std::simd::simd_swizzle!(v, [4, 5, 6, 7]), [0, 1, 2, 3,
// CHECK-NEXT:         0, 0, 0, 0]), [0, 1, 2, 3, 8, 9, 10, 11])
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut fa: __SlateAlign16<[f32; 16]> = __SlateAlign16([
// CHECK-NEXT:         1i32 as f32,
// CHECK-NEXT:         2i32 as f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         4i32 as f32,
// CHECK-NEXT:         -0.0f32,
// CHECK-NEXT:         6i32 as f32,
// CHECK-NEXT:         7i32 as f32,
// CHECK-NEXT:         8i32 as f32,
// CHECK-NEXT:         9i32 as f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         11i32 as f32,
// CHECK-NEXT:         12i32 as f32,
// CHECK-NEXT:         13i32 as f32,
// CHECK-NEXT:         14i32 as f32,
// CHECK-NEXT:         15i32 as f32,
// CHECK-NEXT:         16i32 as f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut fb: __SlateAlign16<[f32; 16]> = __SlateAlign16([
// CHECK-NEXT:         4i32 as f32,
// CHECK-NEXT:         2i32 as f32,
// CHECK-NEXT:         1i32 as f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         0.0f32,
// CHECK-NEXT:         6i32 as f32,
// CHECK-NEXT:         1i32 as f32,
// CHECK-NEXT:         9i32 as f32,
// CHECK-NEXT:         9i32 as f32,
// CHECK-NEXT:         10i32 as f32,
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         1i32 as f32,
// CHECK-NEXT:         14i32 as f32,
// CHECK-NEXT:         13i32 as f32,
// CHECK-NEXT:         15i32 as f32,
// CHECK-NEXT:         -16i32 as f32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut da: __SlateAlign16<[f64; 4]> = __SlateAlign16([
// CHECK-NEXT:         1i32 as f64,
// CHECK-NEXT:         f32::from_bits(0x7fc00000) as f64,
// CHECK-NEXT:         9i32 as f64,
// CHECK-NEXT:         16i32 as f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut db: __SlateAlign16<[f64; 4]> = __SlateAlign16([
// CHECK-NEXT:         1i32 as f64,
// CHECK-NEXT:         3i32 as f64,
// CHECK-NEXT:         f32::from_bits(0x7fc00000) as f64,
// CHECK-NEXT:         4i32 as f64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut ia: __SlateAlign16<[i32; 16]> = __SlateAlign16([0i32; 16]);
// CHECK-NEXT:     let mut bytes: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     for i in 0i32..16i32 {
// CHECK-NEXT:         ia.0[i as usize] = i * 16909060 - 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..64i32 {
// CHECK-NEXT:         bytes.0[i as usize] = (i * 37 + 11) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         sse(
// CHECK-NEXT:             fa.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:             fb.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:             da.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:             db.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:             ia.0.as_mut_ptr() as *const i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx2") {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             avx(
// CHECK-NEXT:                 fa.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                 fb.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                 da.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:                 ia.0.as_mut_ptr() as *const i32,
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
// CHECK-NEXT:                     ia.0.as_mut_ptr() as *const i32,
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
