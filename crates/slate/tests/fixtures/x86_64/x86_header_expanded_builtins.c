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
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let mut t_: core::simd::Simd<f32, 4> = $v;
// CHECK-NEXT:             show(($name) as *const i8, core::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let mut t_: core::simd::Simd<i64, 2> = $v;
// CHECK-NEXT:             show(($name) as *const i8, core::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SHOW__5 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let mut t_: core::simd::Simd<i64, 8> = $v;
// CHECK-NEXT:             show(($name) as *const i8, core::ptr::addr_of_mut!(t_) as *const (), 64i32);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SHOW__7 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let mut t_: core::simd::Simd<f32, 8> = $v;
// CHECK-NEXT:             show(($name) as *const i8, core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SHOW__3 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let mut t_: core::simd::Simd<i64, 4> = $v;
// CHECK-NEXT:             show(($name) as *const i8, core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SHOW__4 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let mut t_: core::simd::Simd<f64, 2> = $v;
// CHECK-NEXT:             show(($name) as *const i8, core::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: macro_rules! SHOW__6 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop {
// CHECK-NEXT:             let mut t_: core::simd::Simd<f64, 4> = $v;
// CHECK-NEXT:             show(($name) as *const i8, core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:             if true {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_cpu_supports([[ARG_0:arg[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v4f64"]
// CHECK-NEXT:     fn __slate_intrinsic_5995f58c42e15c3a(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 4>;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_5ae72a4046d571b1(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_8b85c43cba315222(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v8f32"]
// CHECK-NEXT:     fn __slate_intrinsic_f23ee88b97748596(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 8>;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_ps__1858 {
// CHECK-NEXT:     __v: core::simd::Simd<f32, 4>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_pd__1620 {
// CHECK-NEXT:     __v: core::simd::Simd<f64, 2>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si128 {
// CHECK-NEXT:     __v: core::simd::Simd<i64, 2>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_pd__3129 {
// CHECK-NEXT:     __v: core::simd::Simd<f64, 4>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_ps__3149 {
// CHECK-NEXT:     __v: core::simd::Simd<f32, 8>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si256 {
// CHECK-NEXT:     __v: core::simd::Simd<i64, 4>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_si512 {
// CHECK-NEXT:     __v: core::simd::Simd<i64, 8>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __loadu_ps__4153 {
// CHECK-NEXT:     __v: core::simd::Simd<f32, 16>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_sqrt_ps(__a: core::simd::Simd<f32, 4>) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_8b85c43cba315222(__a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpeq_ss(
// CHECK-NEXT:     mut [[V_0:__v[0-9]+]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_0:__t[0-9]+]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1:__t[0-9]+]] = __b.to_array();
// CHECK-NEXT:                     let [[T_2:__t[0-9]+]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] == [[T_1]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpeq_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] == [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmplt_ss(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1]] = __b.to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmplt_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmple_ss(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1]] = __b.to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] <= [[T_1]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmple_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] <= [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpgt_ss(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         __a,
// CHECK-NEXT:         core::simd::simd_swizzle!(
// CHECK-NEXT:             [[V_0]],
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                         let [[T_1]] = __a.to_array();
// CHECK-NEXT:                         let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_2]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             [4, 1, 2, 3]
// CHECK-NEXT:         ),
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpgt_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __b.to_array();
// CHECK-NEXT:                 let [[T_1]] = __a.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpge_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __b.to_array();
// CHECK-NEXT:                 let [[T_1]] = __a.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] <= [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpneq_ss(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1]] = __b.to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] != [[T_1]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpneq_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] != [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpnlt_ss(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1]] = __b.to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpnlt_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             !{
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpnle_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             !{
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] <= [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpngt_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             !{
// CHECK-NEXT:                 let [[T_0]] = __b.to_array();
// CHECK-NEXT:                 let [[T_1]] = __a.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpnge_ps(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             !{
// CHECK-NEXT:                 let [[T_0]] = __b.to_array();
// CHECK-NEXT:                 let [[T_1]] = __a.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] <= [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpord_ss(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut [[V_1:__v[0-9]+]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] == [[T_1]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & {
// CHECK-NEXT:                         let [[T_3:__t[0-9]+]] = [[V_1]].to_array();
// CHECK-NEXT:                         let [[T_4:__t[0-9]+]] = [[V_1]].to_array();
// CHECK-NEXT:                         let [[T_5:__t[0-9]+]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_3]][__lane] == [[T_4]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_5]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpord_ps(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut [[V_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                 let [[T_1]] = [[V_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] == [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             })
// CHECK-NEXT:                 & {
// CHECK-NEXT:                     let [[T_3]] = [[V_1]].to_array();
// CHECK-NEXT:                     let [[T_4]] = [[V_1]].to_array();
// CHECK-NEXT:                     let [[T_5]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_3]][__lane] == [[T_4]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_5]]
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpunord_ss(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut [[V_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] != [[T_1]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | {
// CHECK-NEXT:                         let [[T_3]] = [[V_1]].to_array();
// CHECK-NEXT:                         let [[T_4]] = [[V_1]].to_array();
// CHECK-NEXT:                         let [[T_5]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_3]][__lane] != [[T_4]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_5]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [4, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpunord_ps(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut [[V_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                 let [[T_1]] = [[V_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] != [[T_1]][__lane]) as i32),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             })
// CHECK-NEXT:                 | {
// CHECK-NEXT:                     let [[T_3]] = [[V_1]].to_array();
// CHECK-NEXT:                     let [[T_4]] = [[V_1]].to_array();
// CHECK-NEXT:                     let [[T_5]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_3]][__lane] != [[T_4]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_5]]
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_loadu_ps(__p: &f32) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::ptr::addr_of!((*((__p as *const f32) as *const __loadu_ps__1858)).__v)
// CHECK-NEXT:             .read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_sqrt_pd(__a: core::simd::Simd<f64, 2>) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_5ae72a4046d571b1(__a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpeq_pd(
// CHECK-NEXT:     mut __a: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] == [[T_1]][__lane]) as i64),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmplt_pd(
// CHECK-NEXT:     mut __a: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i64),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpord_pd(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     mut [[V_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                 let [[T_1]] = [[V_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] == [[T_1]][__lane]) as i64),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             })
// CHECK-NEXT:                 & {
// CHECK-NEXT:                     let [[T_3]] = [[V_1]].to_array();
// CHECK-NEXT:                     let [[T_4]] = [[V_1]].to_array();
// CHECK-NEXT:                     let [[T_5]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_3]][__lane] == [[T_4]][__lane]) as i64),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_5]]
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpunord_pd(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     mut [[V_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             ({
// CHECK-NEXT:                 let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                 let [[T_1]] = [[V_0]].to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] != [[T_1]][__lane]) as i64),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             })
// CHECK-NEXT:                 | {
// CHECK-NEXT:                     let [[T_3]] = [[V_1]].to_array();
// CHECK-NEXT:                     let [[T_4]] = [[V_1]].to_array();
// CHECK-NEXT:                     let [[T_5]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_3]][__lane] != [[T_4]][__lane]) as i64),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_5]]
// CHECK-NEXT:                 },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpneq_pd(
// CHECK-NEXT:     mut __a: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0]] = __a.to_array();
// CHECK-NEXT:                 let [[T_1]] = __b.to_array();
// CHECK-NEXT:                 let [[T_2]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_0]][__lane] != [[T_1]][__lane]) as i64),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmplt_sd(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     mut __b: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1]] = __b.to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i64),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [2, 1]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_cmpord_sd(
// CHECK-NEXT:     mut [[V_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     mut [[V_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         [[V_0]],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_0]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_1]] = [[V_0]].to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] == [[T_1]][__lane]) as i64),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & {
// CHECK-NEXT:                         let [[T_3]] = [[V_1]].to_array();
// CHECK-NEXT:                         let [[T_4]] = [[V_1]].to_array();
// CHECK-NEXT:                         let [[T_5]]: core::simd::Simd<i64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_3]][__lane] == [[T_4]][__lane]) as i64),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_5]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         },
// CHECK-NEXT:         [2, 1]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_loadu_pd(__dp: &f64) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::ptr::addr_of!((*((__dp as *const f64) as *const __loadu_pd__1620)).__v)
// CHECK-NEXT:             .read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_loadu_si128(
// CHECK-NEXT:     __p: *const core::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::ptr::addr_of!((*(__p as *const __loadu_si128)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_set_epi16(
// CHECK-NEXT:     __w7: i16,
// CHECK-NEXT:     __w6: i16,
// CHECK-NEXT:     __w5: i16,
// CHECK-NEXT:     __w4: i16,
// CHECK-NEXT:     __w3: i16,
// CHECK-NEXT:     __w2: i16,
// CHECK-NEXT:     __w1: i16,
// CHECK-NEXT:     __w0: i16,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                         [__w0, __w1, __w2, __w3, __w4, __w5, __w6, __w7],
// CHECK-NEXT:                     );
// CHECK-NEXT:                     core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse2")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_set1_epi16(__w: i16) -> core::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe { _mm_set_epi16(__w, __w, __w, __w, __w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_sqrt_pd(__a: core::simd::Simd<f64, 4>) -> core::simd::Simd<f64, 4> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_5995f58c42e15c3a(__a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_sqrt_ps(__a: core::simd::Simd<f32, 8>) -> core::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_f23ee88b97748596(__a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_loadu_pd(__p: &f64) -> core::simd::Simd<f64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::ptr::addr_of!((*((__p as *const f64) as *const __loadu_pd__3129)).__v)
// CHECK-NEXT:             .read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_loadu_ps(__p: &f32) -> core::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::ptr::addr_of!((*((__p as *const f32) as *const __loadu_ps__3149)).__v)
// CHECK-NEXT:             .read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_loadu_si256(
// CHECK-NEXT:     __p: *const core::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::ptr::addr_of!((*(__p as *const __loadu_si256)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_undefined_si256() -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<f64, 4>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:             core::simd::Simd::splat(0.0f64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_set_pd(
// CHECK-NEXT:     __a: f64,
// CHECK-NEXT:     __b: f64,
// CHECK-NEXT:     __c: f64,
// CHECK-NEXT:     __d: f64,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<f64, 4> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([__d, __c, __b, __a]);
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_set_epi32(
// CHECK-NEXT:     __i0: i32,
// CHECK-NEXT:     __i1: i32,
// CHECK-NEXT:     __i2: i32,
// CHECK-NEXT:     __i3: i32,
// CHECK-NEXT:     __i4: i32,
// CHECK-NEXT:     __i5: i32,
// CHECK-NEXT:     __i6: i32,
// CHECK-NEXT:     __i7: i32,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i32, 8> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                         [__i7, __i6, __i5, __i4, __i3, __i2, __i1, __i0],
// CHECK-NEXT:                     );
// CHECK-NEXT:                     core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
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
// CHECK-NEXT: ) -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i16, 16> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i16, 16>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                         [
// CHECK-NEXT:                             __w00,
// CHECK-NEXT:                             __w01,
// CHECK-NEXT:                             __w02,
// CHECK-NEXT:                             __w03,
// CHECK-NEXT:                             __w04,
// CHECK-NEXT:                             __w05,
// CHECK-NEXT:                             __w06,
// CHECK-NEXT:                             __w07,
// CHECK-NEXT:                             __w08,
// CHECK-NEXT:                             __w09,
// CHECK-NEXT:                             __w10,
// CHECK-NEXT:                             __w11,
// CHECK-NEXT:                             __w12,
// CHECK-NEXT:                             __w13,
// CHECK-NEXT:                             __w14,
// CHECK-NEXT:                             __w15,
// CHECK-NEXT:                         ],
// CHECK-NEXT:                     );
// CHECK-NEXT:                     core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_set1_pd(__w: f64) -> core::simd::Simd<f64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_pd(__w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_set1_epi32(__i: i32) -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi32(__i, __i, __i, __i, __i, __i, __i, __i) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_set1_epi16(__w: i16) -> core::simd::Simd<i64, 4> {
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
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_setzero_si256() -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([0i64, 0i64, 0i64, 0i64]);
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_castps256_ps128(
// CHECK-NEXT:     mut __a: core::simd::Simd<f32, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(__a, __a, [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_castsi256_si128(
// CHECK-NEXT:     mut __a: core::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 2> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(__a, __a, [0, 1]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_castsi128_si256(
// CHECK-NEXT:     mut __a: core::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(__a, core::simd::Simd::splat(0i64), [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_setzero_si512() -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                 [0i64, 0i64, 0i64, 0i64, 0i64, 0i64, 0i64, 0i64],
// CHECK-NEXT:             );
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_undefined_epi32() -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<f64, 8>, core::simd::Simd<i64, 8>>(
// CHECK-NEXT:             core::simd::Simd::splat(0.0f64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_set1_epi32(__s: i32) -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i32, 16> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 16>, core::simd::Simd<i64, 8>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                         [
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                             __s,
// CHECK-NEXT:                         ],
// CHECK-NEXT:                     );
// CHECK-NEXT:                     core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_castsi256_si512(
// CHECK-NEXT:     mut __A: core::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(
// CHECK-NEXT:         __A,
// CHECK-NEXT:         core::simd::Simd::splat(0i64),
// CHECK-NEXT:         [0, 1, 2, 3, 4, 5, 6, 7]
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_castsi512_si128(
// CHECK-NEXT:     mut __A: core::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 2> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(__A, __A, [0, 1]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_castsi512_si256(
// CHECK-NEXT:     mut __A: core::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     return core::simd::simd_swizzle!(__A, __A, [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_loadu_si512(__P: *const ()) -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::ptr::addr_of!((*(__P as *const __loadu_si512)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_mask_loadu_epi32(
// CHECK-NEXT:     __W: core::simd::Simd<i64, 8>,
// CHECK-NEXT:     __U: u16,
// CHECK-NEXT:     __P: *const (),
// CHECK-NEXT: ) -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 16>, core::simd::Simd<i64, 8>>(
// CHECK-NEXT:             core::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 (__P as *const i32) as *const i32,
// CHECK-NEXT:                 core::simd::Mask::<i32, 16>::from_bitmask((__U as u16) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i64, 8>, core::simd::Simd<i32, 16>>(
// CHECK-NEXT:                         __W,
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_maskz_loadu_epi32(
// CHECK-NEXT:     __U: u16,
// CHECK-NEXT:     __P: *const (),
// CHECK-NEXT: ) -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 16>, core::simd::Simd<i64, 8>>(
// CHECK-NEXT:             core::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 (__P as *const i32) as *const i32,
// CHECK-NEXT:                 core::simd::Mask::<i32, 16>::from_bitmask((__U as u16) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i64, 8>, core::simd::Simd<i32, 16>>(
// CHECK-NEXT:                         unsafe { _mm512_setzero_si512() },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_loadu_ps(__p: *const ()) -> core::simd::Simd<f32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::ptr::addr_of!((*(__p as *const __loadu_ps__4153)).__v).read_unaligned()
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_mask_mov_epi32(
// CHECK-NEXT:     __W: core::simd::Simd<i64, 8>,
// CHECK-NEXT:     __U: u16,
// CHECK-NEXT:     __A: core::simd::Simd<i64, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 16>, core::simd::Simd<i64, 8>>(
// CHECK-NEXT:             core::simd::Select::select(
// CHECK-NEXT:                 core::simd::Mask::<i32, 16>::from_bitmask((__U as u16) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i64, 8>, core::simd::Simd<i32, 16>>(
// CHECK-NEXT:                         __A,
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i64, 8>, core::simd::Simd<i32, 16>>(
// CHECK-NEXT:                         __W,
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512vl")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_maskz_loadu_epi32(__U: u8, __P: *const ()) -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:             core::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 (__P as *const core::simd::Simd<i32, 8>) as *const i32,
// CHECK-NEXT:                 core::simd::Mask::<i32, 8>::from_bitmask((__U as u8) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i32, 8>>(
// CHECK-NEXT:                         unsafe { _mm256_setzero_si256() },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_maskz_loadu_epi8(__U: u64, __P: *const ()) -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 64>, core::simd::Simd<i64, 8>>(
// CHECK-NEXT:             core::simd::Simd::load_select_ptr(
// CHECK-NEXT:                 (__P as *const core::simd::Simd<i8, 64>) as *const i8,
// CHECK-NEXT:                 core::simd::Mask::<i8, 64>::from_bitmask((__U as u64) as u64),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i64, 8>, core::simd::Simd<i8, 64>>(
// CHECK-NEXT:                         unsafe { _mm512_setzero_si512() },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_movepi8_mask(__A: core::simd::Simd<i64, 8>) -> u64 {
// CHECK-NEXT:     (core::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i64, 8>, core::simd::Simd<i8, 64>>(
// CHECK-NEXT:                     __A,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             core::simd::Simd::<i8, 64>::default(),
// CHECK-NEXT:         )
// CHECK-NEXT:         .to_bitmask() as u64) as u64
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm512_movm_epi8(__A: u64) -> core::simd::Simd<i64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 64>, core::simd::Simd<i64, 8>>(
// CHECK-NEXT:             core::simd::Select::select(
// CHECK-NEXT:                 core::simd::Mask::<i8, 64>::from_bitmask((__A as u64) as u64),
// CHECK-NEXT:                 core::simd::Simd::splat(-1i8),
// CHECK-NEXT:                 core::simd::Simd::splat(0i8),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512vl,avx512bw")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm256_movm_epi16(__A: u16) -> core::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i16, 16>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:             core::simd::Select::select(
// CHECK-NEXT:                 core::simd::Mask::<i16, 16>::from_bitmask((__A as u16) as u64),
// CHECK-NEXT:                 core::simd::Simd::splat(-1i16),
// CHECK-NEXT:                 core::simd::Simd::splat(0i16),
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512vl,avx512dq")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn _mm_movepi32_mask(__A: core::simd::Simd<i64, 2>) -> u8 {
// CHECK-NEXT:     (core::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                     __A,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             core::simd::Simd::<i32, 4>::default(),
// CHECK-NEXT:         )
// CHECK-NEXT:         .to_bitmask() as u8) as u8
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn show(name: *const i8, data: *const (), bytes: i32) {
// CHECK-NEXT:     let p: *const u8 = data as *const u8;
// CHECK-NEXT:     let mut hash: u32 = 2166136261;
// CHECK-NEXT:     for i in 0..bytes {
// CHECK-NEXT:         hash = (hash ^ (((unsafe { *p.offset(i as isize) }) as i32) as u32))
// CHECK-NEXT:             .wrapping_mul(16777619);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s %08x\n".as_ptr(), name, hash) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse4.1")]
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: unsafe fn sse(fa: &f32, fb: &f32, da: &f64, db: &f64, ia: *const i32) {
// CHECK-NEXT:     let mut a: core::simd::Simd<f32, 4> = unsafe { _mm_loadu_ps(fa) };
// CHECK-NEXT:     let mut b: core::simd::Simd<f32, 4> = unsafe { _mm_loadu_ps(fb) };
// CHECK-NEXT:     let mut c: core::simd::Simd<f64, 2> = unsafe { _mm_loadu_pd(da) };
// CHECK-NEXT:     let mut d: core::simd::Simd<f64, 2> = unsafe { _mm_loadu_pd(db) };
// CHECK-NEXT:     let i: core::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_loadu_si128(ia as *const core::simd::Simd<i64, 2>)
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW!(c"sqrt_ps".as_ptr() as *mut i8, unsafe { _mm_sqrt_ps(a) });
// CHECK-NEXT:     SHOW__4!(c"sqrt_pd".as_ptr() as *mut i8, unsafe { _mm_sqrt_pd(c) });
// CHECK-NEXT:     SHOW!(c"cmpeq_ps".as_ptr() as *mut i8, unsafe { _mm_cmpeq_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmplt_ps".as_ptr() as *mut i8, unsafe { _mm_cmplt_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmple_ps".as_ptr() as *mut i8, unsafe { _mm_cmple_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpgt_ps".as_ptr() as *mut i8, unsafe { _mm_cmpgt_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpge_ps".as_ptr() as *mut i8, unsafe { _mm_cmpge_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpneq_ps".as_ptr() as *mut i8, unsafe { _mm_cmpneq_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpnlt_ps".as_ptr() as *mut i8, unsafe { _mm_cmpnlt_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpnle_ps".as_ptr() as *mut i8, unsafe { _mm_cmpnle_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpngt_ps".as_ptr() as *mut i8, unsafe { _mm_cmpngt_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpnge_ps".as_ptr() as *mut i8, unsafe { _mm_cmpnge_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpord_ps".as_ptr() as *mut i8, unsafe { _mm_cmpord_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpunord_ps".as_ptr() as *mut i8, unsafe { _mm_cmpunord_ps(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpeq_ss".as_ptr() as *mut i8, unsafe { _mm_cmpeq_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmplt_ss".as_ptr() as *mut i8, unsafe { _mm_cmplt_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmple_ss".as_ptr() as *mut i8, unsafe { _mm_cmple_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpgt_ss".as_ptr() as *mut i8, unsafe { _mm_cmpgt_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpneq_ss".as_ptr() as *mut i8, unsafe { _mm_cmpneq_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpnlt_ss".as_ptr() as *mut i8, unsafe { _mm_cmpnlt_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpord_ss".as_ptr() as *mut i8, unsafe { _mm_cmpord_ss(a, b) });
// CHECK-NEXT:     SHOW!(c"cmpunord_ss".as_ptr() as *mut i8, unsafe { _mm_cmpunord_ss(a, b) });
// CHECK-NEXT:     SHOW__4!(c"cmpeq_pd".as_ptr() as *mut i8, unsafe { _mm_cmpeq_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmplt_pd".as_ptr() as *mut i8, unsafe { _mm_cmplt_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmpneq_pd".as_ptr() as *mut i8, unsafe { _mm_cmpneq_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmpord_pd".as_ptr() as *mut i8, unsafe { _mm_cmpord_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmpunord_pd".as_ptr() as *mut i8, unsafe { _mm_cmpunord_pd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmplt_sd".as_ptr() as *mut i8, unsafe { _mm_cmplt_sd(c, d) });
// CHECK-NEXT:     SHOW__4!(c"cmpord_sd".as_ptr() as *mut i8, unsafe { _mm_cmpord_sd(c, d) });
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"insert_epi32".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                             i,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::splat(77i32),
// CHECK-NEXT:                     [0, 1, 6, 3]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"insert_epi16".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                             i,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::splat(-5 as i16),
// CHECK-NEXT:                     [0, 1, 2, 3, 4, 5, 6, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"insert_epi8".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                             i,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::splat(9i8),
// CHECK-NEXT:                     [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 29, 14, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "extract_epi32 {} {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i32, 4>>(i)
// CHECK-NEXT:         }[1],
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i32, 4>>(i)
// CHECK-NEXT:         }[3]
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "extract_epi16 {}",
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i16, 8>>(i)
// CHECK-NEXT:         }[5] as u16) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "extract_epi8 {}",
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i8, 16>>(i)
// CHECK-NEXT:         }[6] as u8) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"blend_epi16".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                             i,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                             unsafe { _mm_set1_epi16(-1 as i16) },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     [8, 1, 10, 3, 4, 13, 6, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"blend_ps".as_ptr() as *mut i8,
// CHECK-NEXT:         core::simd::simd_swizzle!(a, b, [0, 5, 6, 3])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__4!(c"blend_pd".as_ptr() as *mut i8, core::simd::simd_swizzle!(c, d, [0, 3]));
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"srli0".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         core::simd::simd_swizzle!(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             i,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             core::simd::Simd::splat(0u8),
// CHECK-NEXT:                             [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"srli3".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         core::simd::simd_swizzle!(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             i,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             core::simd::Simd::splat(0u8),
// CHECK-NEXT:                             [3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 29, 30, 31]
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"srli15".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         core::simd::simd_swizzle!(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             i,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             core::simd::Simd::splat(0u8),
// CHECK-NEXT:                             [
// CHECK-NEXT:                                 15, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
// CHECK-NEXT:                                 31,
// CHECK-NEXT:                             ]
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"srli16".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         core::simd::simd_swizzle!(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             i,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             core::simd::Simd::splat(0u8),
// CHECK-NEXT:                             [
// CHECK-NEXT:                                 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
// CHECK-NEXT:                                 31,
// CHECK-NEXT:                             ]
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"slli5".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         core::simd::simd_swizzle!(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             i,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             core::simd::Simd::splat(0u8),
// CHECK-NEXT:                             [16, 17, 18, 19, 20, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"slli20".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         core::simd::simd_swizzle!(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             i,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             core::simd::Simd::splat(0u8),
// CHECK-NEXT:                             [
// CHECK-NEXT:                                 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
// CHECK-NEXT:                                 31,
// CHECK-NEXT:                             ]
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: unsafe fn avx(fa: &f32, fb: &f32, da: &f64, ia: *const i32) {
// CHECK-NEXT:     let mut a: core::simd::Simd<f32, 8> = unsafe { _mm256_loadu_ps(fa) };
// CHECK-NEXT:     let mut b: core::simd::Simd<f32, 8> = unsafe { _mm256_loadu_ps(fb) };
// CHECK-NEXT:     let mut c: core::simd::Simd<f64, 4> = unsafe { _mm256_loadu_pd(da) };
// CHECK-NEXT:     let mut i: core::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(ia as *const core::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW__7!(c"sqrt256_ps".as_ptr() as *mut i8, unsafe { _mm256_sqrt_ps(a) });
// CHECK-NEXT:     SHOW__6!(c"sqrt256_pd".as_ptr() as *mut i8, unsafe { _mm256_sqrt_pd(c) });
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_0]] = a.to_array();
// CHECK-NEXT:                     let [[T_1]] = b.to_array();
// CHECK-NEXT:                     let [[T_2]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_0]][__lane] == [[T_1]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_2]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_0".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_3]] = a.to_array();
// CHECK-NEXT:                     let [[T_4]] = b.to_array();
// CHECK-NEXT:                     let [[T_5]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_3]][__lane] < [[T_4]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_5]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_1".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_6:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_7:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_8:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_6]][__lane] <= [[T_7]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_8]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_2".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: core::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: core::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_9:__t[0-9]+]] = [[V_2]].to_array();
// CHECK-NEXT:                     let [[T_10:__t[0-9]+]] = [[V_2]].to_array();
// CHECK-NEXT:                     let [[T_11:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_9]][__lane] != [[T_10]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_11]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | {
// CHECK-NEXT:                         let [[T_12:__t[0-9]+]] = [[V_3]].to_array();
// CHECK-NEXT:                         let [[T_13:__t[0-9]+]] = [[V_3]].to_array();
// CHECK-NEXT:                         let [[T_14:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_12]][__lane] != [[T_13]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_14]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_3".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_15:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_16:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_17:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_15]][__lane] != [[T_16]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_17]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_4".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_18:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_19:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_20:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_18]][__lane] < [[T_19]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_20]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_5".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_21:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_22:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_23:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_21]][__lane] <= [[T_22]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_23]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_6".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: core::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: core::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_24:__t[0-9]+]] = [[V_4]].to_array();
// CHECK-NEXT:                     let [[T_25:__t[0-9]+]] = [[V_4]].to_array();
// CHECK-NEXT:                     let [[T_26:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_24]][__lane] == [[T_25]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_26]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & {
// CHECK-NEXT:                         let [[T_27:__t[0-9]+]] = [[V_5]].to_array();
// CHECK-NEXT:                         let [[T_28:__t[0-9]+]] = [[V_5]].to_array();
// CHECK-NEXT:                         let [[T_29:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_27]][__lane] == [[T_28]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_29]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_7".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: core::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: core::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_30:__t[0-9]+]] = [[V_6]].to_array();
// CHECK-NEXT:                     let [[T_31:__t[0-9]+]] = [[V_7]].to_array();
// CHECK-NEXT:                     let [[T_32:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_30]][__lane] == [[T_31]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_32]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | (({
// CHECK-NEXT:                         let [[T_33:__t[0-9]+]] = [[V_6]].to_array();
// CHECK-NEXT:                         let [[T_34:__t[0-9]+]] = [[V_6]].to_array();
// CHECK-NEXT:                         let [[T_35:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_33]][__lane] != [[T_34]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_35]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_36:__t[0-9]+]] = [[V_7]].to_array();
// CHECK-NEXT:                             let [[T_37:__t[0-9]+]] = [[V_7]].to_array();
// CHECK-NEXT:                             let [[T_38:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                                 core::array::from_fn(
// CHECK-NEXT:                                     |__lane| -(([[T_36]][__lane] != [[T_37]][__lane]) as i32),
// CHECK-NEXT:                                 ),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_38]]
// CHECK-NEXT:                         }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_8".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_39:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_40:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_41:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_39]][__lane] >= [[T_40]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_41]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_9".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_42:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_43:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_44:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_42]][__lane] > [[T_43]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_44]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_10".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 core::simd::Simd::splat(0i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_11".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: core::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: core::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_45:__t[0-9]+]] = [[V_8]].to_array();
// CHECK-NEXT:                     let [[T_46:__t[0-9]+]] = [[V_9]].to_array();
// CHECK-NEXT:                     let [[T_47:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_45]][__lane] != [[T_46]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_47]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & (({
// CHECK-NEXT:                         let [[T_48:__t[0-9]+]] = [[V_8]].to_array();
// CHECK-NEXT:                         let [[T_49:__t[0-9]+]] = [[V_8]].to_array();
// CHECK-NEXT:                         let [[T_50:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_48]][__lane] == [[T_49]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_50]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         & {
// CHECK-NEXT:                             let [[T_51:__t[0-9]+]] = [[V_9]].to_array();
// CHECK-NEXT:                             let [[T_52:__t[0-9]+]] = [[V_9]].to_array();
// CHECK-NEXT:                             let [[T_53:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                                 core::array::from_fn(
// CHECK-NEXT:                                     |__lane| -(([[T_51]][__lane] == [[T_52]][__lane]) as i32),
// CHECK-NEXT:                                 ),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_53]]
// CHECK-NEXT:                         }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_12".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_54:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_55:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_56:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_54]][__lane] >= [[T_55]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_56]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_13".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_57:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_58:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_59:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_57]][__lane] > [[T_58]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_59]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_14".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !core::simd::Simd::splat(0i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_15".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_60:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_61:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_62:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_60]][__lane] == [[T_61]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_62]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_16".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_63:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_64:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_65:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_63]][__lane] < [[T_64]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_65]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_17".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_66:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_67:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_68:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_66]][__lane] <= [[T_67]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_68]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_18".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: core::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: core::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_69:__t[0-9]+]] = [[V_10]].to_array();
// CHECK-NEXT:                     let [[T_70:__t[0-9]+]] = [[V_10]].to_array();
// CHECK-NEXT:                     let [[T_71:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_69]][__lane] != [[T_70]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_71]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | {
// CHECK-NEXT:                         let [[T_72:__t[0-9]+]] = [[V_11]].to_array();
// CHECK-NEXT:                         let [[T_73:__t[0-9]+]] = [[V_11]].to_array();
// CHECK-NEXT:                         let [[T_74:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_72]][__lane] != [[T_73]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_74]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_19".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_75:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_76:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_77:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_75]][__lane] != [[T_76]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_77]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_20".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_19:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_78:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_79:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_80:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_78]][__lane] < [[T_79]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_80]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_21".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_20:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_81:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_82:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_83:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_81]][__lane] <= [[T_82]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_83]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_22".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_21:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: core::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: core::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_84:__t[0-9]+]] = [[V_12]].to_array();
// CHECK-NEXT:                     let [[T_85:__t[0-9]+]] = [[V_12]].to_array();
// CHECK-NEXT:                     let [[T_86:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_84]][__lane] == [[T_85]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_86]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & {
// CHECK-NEXT:                         let [[T_87:__t[0-9]+]] = [[V_13]].to_array();
// CHECK-NEXT:                         let [[T_88:__t[0-9]+]] = [[V_13]].to_array();
// CHECK-NEXT:                         let [[T_89:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_87]][__lane] == [[T_88]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_89]]
// CHECK-NEXT:                     },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_23".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_22:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: core::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: core::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_90:__t[0-9]+]] = [[V_14]].to_array();
// CHECK-NEXT:                     let [[T_91:__t[0-9]+]] = [[V_15]].to_array();
// CHECK-NEXT:                     let [[T_92:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_90]][__lane] == [[T_91]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_92]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     | (({
// CHECK-NEXT:                         let [[T_93:__t[0-9]+]] = [[V_14]].to_array();
// CHECK-NEXT:                         let [[T_94:__t[0-9]+]] = [[V_14]].to_array();
// CHECK-NEXT:                         let [[T_95:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_93]][__lane] != [[T_94]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_95]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_96:__t[0-9]+]] = [[V_15]].to_array();
// CHECK-NEXT:                             let [[T_97:__t[0-9]+]] = [[V_15]].to_array();
// CHECK-NEXT:                             let [[T_98:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                                 core::array::from_fn(
// CHECK-NEXT:                                     |__lane| -(([[T_96]][__lane] != [[T_97]][__lane]) as i32),
// CHECK-NEXT:                                 ),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_98]]
// CHECK-NEXT:                         }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_24".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_23:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_99:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_100:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_101:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_99]][__lane] >= [[T_100]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_101]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_25".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_24:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_102:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_103:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_104:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_102]][__lane] > [[T_103]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_104]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_26".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 core::simd::Simd::splat(0i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_27".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_25:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: core::simd::Simd<f32, 8> = a;
// CHECK-NEXT:         let mut [[V_17:__v[0-9]+]]: core::simd::Simd<f32, 8> = b;
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 ({
// CHECK-NEXT:                     let [[T_105:__t[0-9]+]] = [[V_16]].to_array();
// CHECK-NEXT:                     let [[T_106:__t[0-9]+]] = [[V_17]].to_array();
// CHECK-NEXT:                     let [[T_107:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_105]][__lane] != [[T_106]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_107]]
// CHECK-NEXT:                 })
// CHECK-NEXT:                     & (({
// CHECK-NEXT:                         let [[T_108:__t[0-9]+]] = [[V_16]].to_array();
// CHECK-NEXT:                         let [[T_109:__t[0-9]+]] = [[V_16]].to_array();
// CHECK-NEXT:                         let [[T_110:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_108]][__lane] == [[T_109]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_110]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         & {
// CHECK-NEXT:                             let [[T_111:__t[0-9]+]] = [[V_17]].to_array();
// CHECK-NEXT:                             let [[T_112:__t[0-9]+]] = [[V_17]].to_array();
// CHECK-NEXT:                             let [[T_113:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                                 core::array::from_fn(
// CHECK-NEXT:                                     |__lane| -(([[T_111]][__lane] == [[T_112]][__lane]) as i32),
// CHECK-NEXT:                                 ),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_113]]
// CHECK-NEXT:                         }),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_28".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_26:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_114:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_115:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_116:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_114]][__lane] >= [[T_115]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_116]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_29".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_27:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_117:__t[0-9]+]] = a.to_array();
// CHECK-NEXT:                     let [[T_118:__t[0-9]+]] = b.to_array();
// CHECK-NEXT:                     let [[T_119:__t[0-9]+]]: core::simd::Simd<i32, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_117]][__lane] > [[T_118]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_119]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_30".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<f32, 8>>(
// CHECK-NEXT:                 !core::simd::Simd::splat(0i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_ps_31".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_28:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f64, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<f64, 4>>(
// CHECK-NEXT:                 {
// CHECK-NEXT:                     let [[T_120:__t[0-9]+]] = c.to_array();
// CHECK-NEXT:                     let [[T_121:__t[0-9]+]]: core::simd::Simd<f64, 4> = unsafe {
// CHECK-NEXT:                         _mm256_set1_pd(2.0f64)
// CHECK-NEXT:                     };
// CHECK-NEXT:                     let [[T_122:__t[0-9]+]] = [[T_121]].to_array();
// CHECK-NEXT:                     let [[T_123:__t[0-9]+]]: core::simd::Simd<i64, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_120]][__lane] < [[T_122]][__lane]) as i64),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_123]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp_pd_lt".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 32i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_29:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<f32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: core::simd::Simd<f32, 4> = unsafe { _mm256_castps256_ps128(a) };
// CHECK-NEXT:         t_ = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                 !{
// CHECK-NEXT:                     let [[T_124:__t[0-9]+]] = [[V_18]].to_array();
// CHECK-NEXT:                     let [[T_125:__t[0-9]+]]: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:                         _mm256_castps256_ps128(b)
// CHECK-NEXT:                     };
// CHECK-NEXT:                     let [[T_126:__t[0-9]+]] = [[T_125]].to_array();
// CHECK-NEXT:                     let [[T_127:__t[0-9]+]]: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:                         core::array::from_fn(
// CHECK-NEXT:                             |__lane| -(([[T_124]][__lane] >= [[T_126]][__lane]) as i32),
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     );
// CHECK-NEXT:                     [[T_127]]
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         show(c"cmp128_ps_nge".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 16i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"permute4x64_epi64".as_ptr() as *mut i8,
// CHECK-NEXT:         core::simd::simd_swizzle!(i, [3, 2, 1, 0])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"permute4x64_pd".as_ptr() as *mut i8,
// CHECK-NEXT:         core::simd::simd_swizzle!(c, [2, 3, 0, 1])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"blend_epi32".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i32, 8>>(
// CHECK-NEXT:                             i,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i32, 8>>(
// CHECK-NEXT:                             unsafe { _mm256_set1_epi32(-1i32) },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     [8, 9, 2, 3, 12, 5, 6, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"blend256_epi16".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 16>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i16, 16>>(
// CHECK-NEXT:                             i,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i16, 16>>(
// CHECK-NEXT:                             unsafe { _mm256_set1_epi16(7i16) },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     [16, 17, 18, 19, 4, 5, 6, 7, 24, 25, 26, 27, 12, 13, 14, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"blend256_pd".as_ptr() as *mut i8,
// CHECK-NEXT:         core::simd::simd_swizzle!(c, unsafe { _mm256_set1_pd(9.0f64) }, [4, 1, 6, 3])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"srli256".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 32>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 32>, core::simd::Simd<i8, 32>>(
// CHECK-NEXT:                         core::simd::simd_swizzle!(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 32>, core::simd::Simd<u8, 32>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i8, 32>>(
// CHECK-NEXT:                                             i,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             core::simd::Simd::splat(0u8),
// CHECK-NEXT:                             [
// CHECK-NEXT:                                 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 44, 45, 46, 47,
// CHECK-NEXT:                                 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 60, 61, 62,
// CHECK-NEXT:                                 63,
// CHECK-NEXT:                             ]
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"slli256".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 32>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 32>, core::simd::Simd<i8, 32>>(
// CHECK-NEXT:                         core::simd::simd_swizzle!(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 32>, core::simd::Simd<u8, 32>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i8, 32>>(
// CHECK-NEXT:                                             i,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             core::simd::Simd::splat(0u8),
// CHECK-NEXT:                             [
// CHECK-NEXT:                                 32, 33, 34, 35, 36, 37, 38, 39, 40, 0, 1, 2, 3, 4, 5, 6, 48,
// CHECK-NEXT:                                 49, 50, 51, 52, 53, 54, 55, 56, 16, 17, 18, 19, 20, 21, 22,
// CHECK-NEXT:                             ]
// CHECK-NEXT:                         ),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "extract256_epi32 {}",
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i32, 8>>(i)
// CHECK-NEXT:         }[6]
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { slate_support::printf(c"extract256_epi64 %lld\n".as_ptr(), i[3]) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"insert256_epi32".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i32, 8>>(
// CHECK-NEXT:                             i,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::splat(1234i32),
// CHECK-NEXT:                     [0, 1, 2, 3, 4, 13, 6, 7]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     let wide: core::simd::Simd<i64, 4> = core::simd::simd_swizzle!(
// CHECK-NEXT:         unsafe { _mm256_castsi128_si256(_mm256_castsi256_si128(i)) },
// CHECK-NEXT:         core::simd::simd_swizzle!(core::simd::simd_swizzle!(i, [2, 3]), [0, 1, 0, 0]),
// CHECK-NEXT:         [0, 1, 4, 5]
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(c"cast128_256".as_ptr() as *mut i8, wide);
// CHECK-NEXT:     let undef: core::simd::Simd<i64, 4> = unsafe { _mm256_undefined_si256() };
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"undef_blend".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 8>, core::simd::Simd<i64, 4>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i32, 8>>(
// CHECK-NEXT:                             undef,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 4>, core::simd::Simd<i32, 8>>(
// CHECK-NEXT:                             i,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     [8, 9, 10, 11, 12, 13, 14, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx512f,avx512bw,avx512dq,avx512vl")]
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: unsafe fn avx512(fa: &f32, fb: &f32, ia: *const i32, bytes: *const i8, tail: i32) {
// CHECK-NEXT:     let mut a: core::simd::Simd<f32, 16> = unsafe {
// CHECK-NEXT:         _mm512_loadu_ps((fa as *const f32) as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: core::simd::Simd<f32, 16> = unsafe {
// CHECK-NEXT:         _mm512_loadu_ps((fb as *const f32) as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp512_lt %04x\n".as_ptr(),
// CHECK-NEXT:             ((core::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_0]] = a.to_array();
// CHECK-NEXT:                         let [[T_1]] = b.to_array();
// CHECK-NEXT:                         let [[T_2]]: core::simd::Simd<i32, 16> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_0]][__lane] < [[T_1]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_2]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::<i32, 16>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u16) as u16) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut [[V_0]]: core::simd::Simd<f32, 16> = a;
// CHECK-NEXT:     let mut [[V_1]]: core::simd::Simd<f32, 16> = b;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp512_unord %04x\n".as_ptr(),
// CHECK-NEXT:             ((core::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_3]] = [[V_0]].to_array();
// CHECK-NEXT:                         let [[T_4]] = [[V_0]].to_array();
// CHECK-NEXT:                         let [[T_5]]: core::simd::Simd<i32, 16> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_3]][__lane] != [[T_4]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_5]]
// CHECK-NEXT:                     })
// CHECK-NEXT:                         | {
// CHECK-NEXT:                             let [[T_6]] = [[V_1]].to_array();
// CHECK-NEXT:                             let [[T_7]] = [[V_1]].to_array();
// CHECK-NEXT:                             let [[T_8]]: core::simd::Simd<i32, 16> = core::simd::Simd::from_array(
// CHECK-NEXT:                                 core::array::from_fn(
// CHECK-NEXT:                                     |__lane| -(([[T_6]][__lane] != [[T_7]][__lane]) as i32),
// CHECK-NEXT:                                 ),
// CHECK-NEXT:                             );
// CHECK-NEXT:                             [[T_8]]
// CHECK-NEXT:                         },
// CHECK-NEXT:                     core::simd::Simd::<i32, 16>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u16) as u16) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmp512_nle %04x\n".as_ptr(),
// CHECK-NEXT:             ((core::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     !{
// CHECK-NEXT:                         let [[T_9]] = a.to_array();
// CHECK-NEXT:                         let [[T_10]] = b.to_array();
// CHECK-NEXT:                         let [[T_11]]: core::simd::Simd<i32, 16> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_9]][__lane] <= [[T_10]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_11]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::<i32, 16>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u16) as u16) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"mask_cmp512_eq %04x\n".as_ptr(),
// CHECK-NEXT:             (((core::simd::cmp::SimdPartialOrd::simd_lt(
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_12]] = a.to_array();
// CHECK-NEXT:                         let [[T_13]] = b.to_array();
// CHECK-NEXT:                         let [[T_14]]: core::simd::Simd<i32, 16> = core::simd::Simd::from_array(
// CHECK-NEXT:                             core::array::from_fn(
// CHECK-NEXT:                                 |__lane| -(([[T_12]][__lane] == [[T_13]][__lane]) as i32),
// CHECK-NEXT:                             ),
// CHECK-NEXT:                         );
// CHECK-NEXT:                         [[T_14]]
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::<i32, 16>::default(),
// CHECK-NEXT:                 )
// CHECK-NEXT:                 .to_bitmask() as u16) & 23130) as u16) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let tail_mask: u16 = (1u32 << tail).wrapping_sub(1) as u16;
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"maskz_loadu_epi32".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe { _mm512_maskz_loadu_epi32(tail_mask, ia as *const ()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"mask_loadu_epi32".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             _mm512_mask_loadu_epi32(_mm512_set1_epi32(-3i32), tail_mask, ia as *const ())
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"maskz_loadu256_epi32".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe { _mm256_maskz_loadu_epi32(tail_mask as u8, ia as *const ()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"maskz_loadu_epi8".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe { _mm512_maskz_loadu_epi8(71777214294589695u64, bytes as *const ()) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut v: core::simd::Simd<i64, 8> = unsafe {
// CHECK-NEXT:         _mm512_loadu_si512(bytes as *const ())
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"movepi8_mask %016llx\n".as_ptr(),
// CHECK-NEXT:             _mm512_movepi8_mask(v) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"movepi32_mask %02x\n".as_ptr(),
// CHECK-NEXT:             _mm_movepi32_mask(_mm512_castsi512_si128(v)) as u32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"movm_epi8".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe { _mm512_movm_epi8(9223372041149743107u64) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"movm256_epi16".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe { _mm256_movm_epi16(33825u16) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let undef: core::simd::Simd<i64, 8> = unsafe { _mm512_undefined_epi32() };
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"undef512_mov".as_ptr() as *mut i8,
// CHECK-NEXT:         unsafe { _mm512_mask_mov_epi32(undef, 65535u16, v) }
// CHECK-NEXT:     );
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut t_: core::simd::Simd<i64, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:         unsafe { _mm256_setzero_si256() };
// CHECK-NEXT:         t_ = core::simd::simd_swizzle!(
// CHECK-NEXT:             unsafe { _mm512_castsi256_si512(_mm512_castsi512_si256(v)) },
// CHECK-NEXT:             core::simd::simd_swizzle!(
// CHECK-NEXT:                 core::simd::simd_swizzle!(v, [4, 5, 6, 7]),
// CHECK-NEXT:                 [0, 1, 2, 3, 0, 0, 0, 0]
// CHECK-NEXT:             ),
// CHECK-NEXT:             [0, 1, 2, 3, 8, 9, 10, 11]
// CHECK-NEXT:         );
// CHECK-NEXT:         show(c"cast256_512".as_ptr(), core::ptr::addr_of_mut!(t_) as *const (), 64i32);
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let fa: __SlateAlign16<[f32; 16]> = __SlateAlign16(
// CHECK-NEXT:         [
// CHECK-NEXT:             1i32 as f32,
// CHECK-NEXT:             2i32 as f32,
// CHECK-NEXT:             f32::from_bits(0x7fc00000),
// CHECK-NEXT:             4i32 as f32,
// CHECK-NEXT:             -0.0f32,
// CHECK-NEXT:             6i32 as f32,
// CHECK-NEXT:             7i32 as f32,
// CHECK-NEXT:             8i32 as f32,
// CHECK-NEXT:             9i32 as f32,
// CHECK-NEXT:             f32::from_bits(0x7fc00000),
// CHECK-NEXT:             11i32 as f32,
// CHECK-NEXT:             12i32 as f32,
// CHECK-NEXT:             13i32 as f32,
// CHECK-NEXT:             14i32 as f32,
// CHECK-NEXT:             15i32 as f32,
// CHECK-NEXT:             16i32 as f32,
// CHECK-NEXT:         ],
// CHECK-NEXT:     );
// CHECK-NEXT:     let fb: __SlateAlign16<[f32; 16]> = __SlateAlign16(
// CHECK-NEXT:         [
// CHECK-NEXT:             4i32 as f32,
// CHECK-NEXT:             2i32 as f32,
// CHECK-NEXT:             1i32 as f32,
// CHECK-NEXT:             f32::from_bits(0x7fc00000),
// CHECK-NEXT:             0.0f32,
// CHECK-NEXT:             6i32 as f32,
// CHECK-NEXT:             1i32 as f32,
// CHECK-NEXT:             9i32 as f32,
// CHECK-NEXT:             9i32 as f32,
// CHECK-NEXT:             10i32 as f32,
// CHECK-NEXT:             f32::from_bits(0x7fc00000),
// CHECK-NEXT:             1i32 as f32,
// CHECK-NEXT:             14i32 as f32,
// CHECK-NEXT:             13i32 as f32,
// CHECK-NEXT:             15i32 as f32,
// CHECK-NEXT:             -16i32 as f32,
// CHECK-NEXT:         ],
// CHECK-NEXT:     );
// CHECK-NEXT:     let da: __SlateAlign16<[f64; 4]> = __SlateAlign16(
// CHECK-NEXT:         [1i32 as f64, f32::from_bits(0x7fc00000) as f64, 9i32 as f64, 16i32 as f64],
// CHECK-NEXT:     );
// CHECK-NEXT:     let db: __SlateAlign16<[f64; 4]> = __SlateAlign16(
// CHECK-NEXT:         [1i32 as f64, 3i32 as f64, f32::from_bits(0x7fc00000) as f64, 4i32 as f64],
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut ia: __SlateAlign16<[i32; 16]> = __SlateAlign16([0i32; 16]);
// CHECK-NEXT:     let mut bytes: __SlateAlign16<[i8; 64]> = __SlateAlign16([0i8; 64]);
// CHECK-NEXT:     for i in 0i32..16i32 {
// CHECK-NEXT:         ia.0[i as usize] = i * 16909060 - 7;
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..64i32 {
// CHECK-NEXT:         bytes.0[i as usize] = (i * 37 + 11) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0]] = ia.0.as_mut_ptr() as *const i32;
// CHECK-NEXT:         unsafe { sse(&fa.0[0usize], &fb.0[0usize], &da.0[0usize], &db.0[0usize], [[T_0]]) }
// CHECK-NEXT:     };
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx2") {
// CHECK-NEXT:         {
// CHECK-NEXT:             let [[T_1]] = ia.0.as_mut_ptr() as *const i32;
// CHECK-NEXT:             unsafe { avx(&fa.0[0usize], &fb.0[0usize], &da.0[0usize], [[T_1]]) }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx512bw")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("avx512dq")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("avx512vl") {
// CHECK-NEXT:         let mut tail: i32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_0]]: while tail <= 16 {
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_2]] = ia.0.as_mut_ptr() as *const i32;
// CHECK-NEXT:                 let [[T_3]] = bytes.0.as_mut_ptr() as *const i8;
// CHECK-NEXT:                 unsafe { avx512(&fa.0[0usize], &fb.0[0usize], [[T_2]], [[T_3]], tail) }
// CHECK-NEXT:             };
// CHECK-NEXT:             tail += 5;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
