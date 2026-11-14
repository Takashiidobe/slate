#include <immintrin.h>
#include <stdio.h>

static void show(const char *name, const void *data, int bytes) {
  const unsigned char *p = data;
  unsigned hash = 2166136261u;
  for (int i = 0; i < bytes; i++)
    hash = (hash ^ p[i]) * 16777619u;
  printf("%s %08x\n", name, hash);
}

#define SHOW(name, v) do { __typeof__(v) t_ = (v); show(name, &t_, sizeof t_); } while (0)

__attribute__((target("avx2"), noinline)) static void avx(const float *fa, const double *da, const short *words) {
  __m128i w = _mm_loadu_si128((const __m128i *)words);
  __m256i w256 = _mm256_loadu_si256((const __m256i *)words);
  SHOW("shufflelo", _mm_shufflelo_epi16(w, 0x1b));
  SHOW("shufflehi", _mm_shufflehi_epi16(w, 0xb1));
  SHOW("shufflelo256", _mm256_shufflelo_epi16(w256, 0x4e));
  SHOW("shufflehi256", _mm256_shufflehi_epi16(w256, 0x93));
  SHOW("permute_ps", _mm_permute_ps(_mm_loadu_ps(fa), 0x39));
  SHOW("permute256_ps", _mm256_permute_ps(_mm256_loadu_ps(fa), 0xc6));
  SHOW("permute_pd", _mm_permute_pd(_mm_loadu_pd(da), 0x1));
  SHOW("permute256_pd", _mm256_permute_pd(_mm256_loadu_pd(da), 0x5));
}

__attribute__((target("avx512f,avx512bw,avx512dq"), noinline)) static void avx512(const float *fa, const double *da,
                                                                                  const short *words, unsigned seed) {
  SHOW("shufflelo512", _mm512_shufflelo_epi16(_mm512_loadu_si512(words), 0xe1));
  SHOW("shufflehi512", _mm512_shufflehi_epi16(_mm512_loadu_si512(words), 0x2d));
  SHOW("permute512_ps", _mm512_permute_ps(_mm512_loadu_ps(fa), 0x72));
  SHOW("permute512_pd", _mm512_permute_pd(_mm512_loadu_pd(da), 0xa6));
  __mmask16 a = (__mmask16)(seed * 2654435761u), b = (__mmask16)(seed * 40503u + 7);
  printf("kand %04x\n", (unsigned)_kand_mask16(a, b));
  printf("kandn %04x\n", (unsigned)_kandn_mask16(a, b));
  printf("kor %04x\n", (unsigned)_kor_mask16(a, b));
  printf("kxor %04x\n", (unsigned)_kxor_mask16(a, b));
  printf("kxnor %04x\n", (unsigned)_kxnor_mask16(a, b));
  printf("knot %04x\n", (unsigned)_knot_mask16(a));
  printf("kadd %04x\n", (unsigned)_kadd_mask16(a, b));
  printf("kunpackb %04x\n", (unsigned)_mm512_kunpackb(a, b));
  __mmask32 c = seed * 2246822519u;
  __mmask64 d = (unsigned long long)seed * 0x9e3779b97f4a7c15ull;
  printf("kunpackw %08x\n", (unsigned)_mm512_kunpackw(c, c ^ 0x5555aaaa));
  printf("kunpackd %016llx\n", (unsigned long long)_mm512_kunpackd(d, ~d));
  printf("knot64 %016llx\n", (unsigned long long)_knot_mask64(d));
  __mmask32 loaded = _load_mask32(&c);
  _store_mask64(&d, _cvtu64_mask64((unsigned long long)_cvtmask32_u32(loaded) << 3));
  printf("kmov %08x %016llx %02x\n", _cvtmask32_u32(loaded), (unsigned long long)d, _cvtmask8_u32((__mmask8)seed));
  printf("kshiftli16 %04x\n", (unsigned)_kshiftli_mask16(a, 3));
  printf("kshiftri32 %08x\n", (unsigned)_kshiftri_mask32(c, 7));
  printf("kshiftri32_wide %08x\n", (unsigned)_kshiftri_mask32(c, 40));
  printf("kshiftli8 %02x\n", (unsigned)_kshiftli_mask8((__mmask8)seed, 5));
  printf("kshiftri64 %016llx\n", (unsigned long long)_kshiftri_mask64(d, 61));
}

int main(void) {
  float fa[16] = {1.5f, -2.5f, 3.25f, 4.49f, -0.0f, 6.5f, 7.75f, -8.25f, 9, 10, 11.5f, 12, 13, -14.5f, 15, 16};
  double da[8] = {1.5, -2.5, 9.25, 16.75, -0.5, 3, 7.5, -8};
  short words[32];
  for (int i = 0; i < 32; i++)
    words[i] = (short)(i * 4099 - 777);
  if (__builtin_cpu_supports("avx2"))
    avx(fa, da, words);
  if (__builtin_cpu_supports("avx512bw") && __builtin_cpu_supports("avx512dq"))
    for (unsigned seed = 1; seed < 40; seed += 13)
      avx512(fa, da, words, seed);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 2 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__4 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 8 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 32i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < i64, 8 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 64i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__5 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 4 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__6 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f64, 2 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 16i32); if true { break; } }
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
// CHECK-NEXT: macro_rules! SHOW__8 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f32, 16 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 64i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__9 {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         loop { let mut t_ : std::simd::Simd < f64, 8 > = $v; show(($name) as * const i8,
// CHECK-NEXT:         std::ptr::addr_of_mut!(t_) as * const (), 64i32); if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_cpu_supports([[ARG_0:arg[0-9]+]]: *const i8) -> bool;
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
// CHECK-NEXT: unsafe fn _mm512_knot(__M: u16) -> u16 {
// CHECK-NEXT:     !__M
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_kand(__A: u16, __B: u16) -> u16 {
// CHECK-NEXT:     __A & __B
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_kandn(__A: u16, __B: u16) -> u16 {
// CHECK-NEXT:     !__A & __B
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_kor(__A: u16, __B: u16) -> u16 {
// CHECK-NEXT:     __A | __B
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_kunpackb(__A: u16, __B: u16) -> u16 {
// CHECK-NEXT:     __A << 8u16 | __B & 255
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_kxnor(__A: u16, __B: u16) -> u16 {
// CHECK-NEXT:     !(__A ^ __B)
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f")]
// CHECK-NEXT: unsafe fn _mm512_kxor(__A: u16, __B: u16) -> u16 {
// CHECK-NEXT:     __A ^ __B
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _knot_mask64(__M: u64) -> u64 {
// CHECK-NEXT:     !__M
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _cvtmask32_u32(__A: u32) -> u32 {
// CHECK-NEXT:     __A
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _cvtu64_mask64(__A: u64) -> u64 {
// CHECK-NEXT:     __A
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _load_mask32(__A: &u32) -> u32 {
// CHECK-NEXT:     unsafe { *(__A as *const u32) }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _store_mask64(__A: &mut u64, __B: u64) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(__A as *mut u64) = __B;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _mm512_kunpackd(__A: u64, __B: u64) -> u64 {
// CHECK-NEXT:     __A << 32u64 | __B & 4294967295
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512bw")]
// CHECK-NEXT: unsafe fn _mm512_kunpackw(__A: u32, __B: u32) -> u32 {
// CHECK-NEXT:     __A << 16u32 | __B & 65535
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512dq")]
// CHECK-NEXT: unsafe fn _kadd_mask16(__A: u16, __B: u16) -> u16 {
// CHECK-NEXT:     __A.wrapping_add(__B)
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512dq")]
// CHECK-NEXT: unsafe fn _cvtmask8_u32(__A: u8) -> u32 {
// CHECK-NEXT:     __A as u32
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
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn avx(fa: *const f32, da: *const f64, words: *const i16) {
// CHECK-NEXT:     let w: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         _mm_loadu_si128(words as *const std::simd::Simd<i64, 2>)
// CHECK-NEXT:     };
// CHECK-NEXT:     let w256: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_loadu_si256(words as *const std::simd::Simd<i64, 4>)
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"shufflelo".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 8 >, std::simd::Simd < i64, 2 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         2 >, std::simd::Simd < i16, 8 >> (w) }, [3, 2, 1, 0, 4, 5, 6, 7])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW!(
// CHECK-NEXT:         c"shufflehi".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 8 >, std::simd::Simd < i64, 2 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         2 >, std::simd::Simd < i16, 8 >> (w) }, [0, 1, 2, 3, 5, 4, 7, 6])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"shufflelo256".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 16 >, std::simd::Simd < i64, 4 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         4 >, std::simd::Simd < i16, 16 >> (w256) }, [2, 3, 0, 1, 4, 5, 6, 7, 10, 11, 8,
// CHECK-NEXT:         9, 12, 13, 14, 15])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"shufflehi256".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 16 >, std::simd::Simd < i64, 4 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         4 >, std::simd::Simd < i16, 16 >> (w256) }, [0, 1, 2, 3, 7, 4, 5, 6, 8, 9, 10,
// CHECK-NEXT:         11, 15, 12, 13, 14])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__5!(
// CHECK-NEXT:         c"permute_ps".as_ptr() as * mut i8, std::simd::simd_swizzle!(unsafe {
// CHECK-NEXT:         _mm_loadu_ps(fa) }, [1, 2, 3, 0])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__4!(
// CHECK-NEXT:         c"permute256_ps".as_ptr() as * mut i8, std::simd::simd_swizzle!(unsafe {
// CHECK-NEXT:         _mm256_loadu_ps(fa) }, [2, 1, 0, 3, 6, 5, 4, 7])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"permute_pd".as_ptr() as * mut i8, std::simd::simd_swizzle!(unsafe {
// CHECK-NEXT:         _mm_loadu_pd(da) }, [1, 0])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__7!(
// CHECK-NEXT:         c"permute256_pd".as_ptr() as * mut i8, std::simd::simd_swizzle!(unsafe {
// CHECK-NEXT:         _mm256_loadu_pd(da) }, [1, 0, 3, 2])
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "avx512f,avx512bw,avx512dq")]
// CHECK-NEXT: unsafe fn avx512(fa: *const f32, da: *const f64, words: *const i16, seed: u32) {
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"shufflelo512".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 32 >, std::simd::Simd < i64, 8 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         8 >, std::simd::Simd < i16, 32 >> (unsafe { _mm512_loadu_si512(words as * const
// CHECK-NEXT:         ()) }) }, [1, 0, 2, 3, 4, 5, 6, 7, 9, 8, 10, 11, 12, 13, 14, 15, 17, 16, 18, 19,
// CHECK-NEXT:         20, 21, 22, 23, 25, 24, 26, 27, 28, 29, 30, 31])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"shufflehi512".as_ptr() as * mut i8, unsafe { std::mem::transmute::<
// CHECK-NEXT:         std::simd::Simd < i16, 32 >, std::simd::Simd < i64, 8 >>
// CHECK-NEXT:         (std::simd::simd_swizzle!(unsafe { std::mem::transmute::< std::simd::Simd < i64,
// CHECK-NEXT:         8 >, std::simd::Simd < i16, 32 >> (unsafe { _mm512_loadu_si512(words as * const
// CHECK-NEXT:         ()) }) }, [0, 1, 2, 3, 5, 7, 6, 4, 8, 9, 10, 11, 13, 15, 14, 12, 16, 17, 18, 19,
// CHECK-NEXT:         21, 23, 22, 20, 24, 25, 26, 27, 29, 31, 30, 28])) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__8!(
// CHECK-NEXT:         c"permute512_ps".as_ptr() as * mut i8, std::simd::simd_swizzle!(unsafe {
// CHECK-NEXT:         _mm512_loadu_ps(fa as * const ()) }, [2, 0, 3, 1, 6, 4, 7, 5, 10, 8, 11, 9, 14,
// CHECK-NEXT:         12, 15, 13])
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__9!(
// CHECK-NEXT:         c"permute512_pd".as_ptr() as * mut i8, std::simd::simd_swizzle!(unsafe {
// CHECK-NEXT:         _mm512_loadu_pd(da as * const ()) }, [0, 1, 3, 2, 4, 5, 6, 7])
// CHECK-NEXT:     );
// CHECK-NEXT:     let a: u16 = seed.wrapping_mul(2654435761) as u16;
// CHECK-NEXT:     let b: u16 = seed.wrapping_mul(40503).wrapping_add(7) as u16;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"kand %04x\n".as_ptr(), _mm512_kand(a, b) as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"kandn %04x\n".as_ptr(), _mm512_kandn(a, b) as u32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"kor %04x\n".as_ptr(), _mm512_kor(a, b) as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"kxor %04x\n".as_ptr(), _mm512_kxor(a, b) as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"kxnor %04x\n".as_ptr(), _mm512_kxnor(a, b) as u32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"knot %04x\n".as_ptr(), _mm512_knot(a) as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"kadd %04x\n".as_ptr(), _kadd_mask16(a, b) as u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"kunpackb %04x\n".as_ptr(), _mm512_kunpackb(a, b) as u32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut c: u32 = seed.wrapping_mul(2246822519);
// CHECK-NEXT:     let mut d: u64 = (seed as u64).wrapping_mul(11400714819323198485);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"kunpackw %08x\n".as_ptr(),
// CHECK-NEXT:             _mm512_kunpackw(c, c ^ 1431677610),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"kunpackd %016llx\n".as_ptr(), _mm512_kunpackd(d, !d))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"knot64 %016llx\n".as_ptr(), _knot_mask64(d)) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let loaded: u32 = unsafe { _load_mask32(&(*std::ptr::addr_of_mut!(c))) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         _store_mask64(
// CHECK-NEXT:             &mut (*std::ptr::addr_of_mut!(d)),
// CHECK-NEXT:             _cvtu64_mask64((_cvtmask32_u32(loaded) as u64) << 3i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"kmov %08x %016llx %02x\n".as_ptr(),
// CHECK-NEXT:             _cvtmask32_u32(loaded),
// CHECK-NEXT:             d,
// CHECK-NEXT:             _cvtmask8_u32(seed as u8),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"kshiftli16 %04x\n".as_ptr(), ((a as u16) << 3u16) as u32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"kshiftri32 %08x\n".as_ptr(), (c as u32) >> 7u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"kshiftri32_wide %08x\n".as_ptr(), 0u32) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"kshiftli8 %02x\n".as_ptr(), ((seed as u8) << 5u8) as u32)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"kshiftri64 %016llx\n".as_ptr(), (d as u64) >> 61u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut fa: __SlateAlign16<[f32; 16]> = __SlateAlign16([
// CHECK-NEXT:         1.5f32,
// CHECK-NEXT:         -2.5f32,
// CHECK-NEXT:         3.25f32,
// CHECK-NEXT:         4.49f32,
// CHECK-NEXT:         -0.0f32,
// CHECK-NEXT:         6.5f32,
// CHECK-NEXT:         7.75f32,
// CHECK-NEXT:         -8.25f32,
// CHECK-NEXT:         9i32 as f32,
// CHECK-NEXT:         10i32 as f32,
// CHECK-NEXT:         11.5f32,
// CHECK-NEXT:         12i32 as f32,
// CHECK-NEXT:         13i32 as f32,
// CHECK-NEXT:         -14.5f32,
// CHECK-NEXT:         15i32 as f32,
// CHECK-NEXT:         16i32 as f32,
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
// CHECK-NEXT:     let mut words: __SlateAlign16<[i16; 32]> = __SlateAlign16([0i16; 32]);
// CHECK-NEXT:     for i in 0i32..32i32 {
// CHECK-NEXT:         words.0[i as usize] = (i * 4099 - 777) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx2") {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             avx(
// CHECK-NEXT:                 fa.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                 da.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:                 words.0.as_mut_ptr() as *const i16,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx512bw")
// CHECK-NEXT:         && std::arch::is_x86_feature_detected!("avx512dq")
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut seed: u32 = 1;
// CHECK-NEXT:         while seed < 40 {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 avx512(
// CHECK-NEXT:                     fa.0.as_mut_ptr() as *const f32,
// CHECK-NEXT:                     da.0.as_mut_ptr() as *const f64,
// CHECK-NEXT:                     words.0.as_mut_ptr() as *const i16,
// CHECK-NEXT:                     seed,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             seed = seed.wrapping_add(13);
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
