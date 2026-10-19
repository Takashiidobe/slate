#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

typedef int32_t v4si __attribute__((vector_size(16)));
typedef int32_t v8si __attribute__((vector_size(32)));
typedef int32_t v2si __attribute__((vector_size(8)));
typedef uint64_t v4du __attribute__((vector_size(32)));
typedef int8_t v16qi __attribute__((vector_size(16)));
typedef uint8_t v16qu __attribute__((vector_size(16)));
typedef int16_t v16hi __attribute__((vector_size(32)));
typedef uint16_t v16hu __attribute__((vector_size(32)));
typedef int16_t v4hi __attribute__((vector_size(8)));
typedef float v4sf __attribute__((vector_size(16)));
typedef double v4df __attribute__((vector_size(32)));

static long long fold(const int64_t *lanes, int count) {
  long long h = 17;
  for (int i = 0; i < count; i++) h = h * 1000003 + lanes[i];
  return h;
}

#define PRINT(label, v, n)                                     \
  do {                                                         \
    int64_t lanes[n];                                          \
    for (int i = 0; i < n; i++) lanes[i] = (int64_t)(v)[i];    \
    printf("%s %lld\n", label, fold(lanes, n));                \
  } while (0)

static v2si low_half(v4si v) { return __builtin_shufflevector(v, v, 0, 1); }

static v8si widen_undef(v4si v) {
  v8si w = __builtin_shufflevector(v, v, 0, 1, 2, 3, -1, -1, -1, -1);
  return __builtin_shufflevector(w, w, 0, 1, 2, 3, 3, 2, 1, 0);
}

static v4si interleave(v4si a, v4si b) { return __builtin_shufflevector(a, b, 0, 4, 1, 5); }

static v8si concat(v4si a, v4si b) { return __builtin_shufflevector(a, b, 7, 6, 5, 4, 3, 2, 1, 0); }

static v4si reverse(v4si v) { return __builtin_shufflevector(v, v, 3, 2, 1, 0); }

__attribute__((target("avx2"))) static void intrinsics(void) {
  __m256i bytes = _mm256_setr_epi64x(0x0102030405060708ll, -0x1112131415161718ll,
                                     0x2122232425262728ll, -0x3132333435363738ll);
  __m128i low = _mm256_castsi256_si128(bytes);
  __m256i words = _mm256_cvtepi8_epi16(low);
  __m256i masked = _mm256_andnot_si256(bytes, _mm256_set1_epi64x(0x00ff00ff00ff00ffll));
  __m256 floats = _mm256_setr_ps(1.5f, -2.25f, 3.0f, 4.75f, 5.5f, 6.0f, -7.0f, 8.25f);
  __m128 low_floats = _mm256_castps256_ps128(floats);
  int64_t out[4];
  _mm256_storeu_si256((__m256i *)out, words);
  printf("cvt %lld\n", fold(out, 4));
  _mm256_storeu_si256((__m256i *)out, masked);
  printf("andnot %lld\n", fold(out, 4));
  float f[4];
  _mm_storeu_ps(f, low_floats);
  printf("castps %.2f %.2f %.2f %.2f\n", f[0], f[1], f[2], f[3]);
}

int main(void) {
  v4si a = {1, -2, 3, -4};
  v4si b = {100, 200, 300, 400};
  PRINT("low", low_half(a), 2);
  PRINT("widen", widen_undef(a), 4);
  PRINT("interleave", interleave(a, b), 4);
  PRINT("concat", concat(a, b), 8);
  PRINT("reverse", reverse(b), 4);

  v4du bits = {0, 1, 0x8000000000000000ull, 0xffffffff00000000ull};
  PRINT("not", ~bits, 4);
  PRINT("neg", -a, 4);
  PRINT("notneg", ~(-a), 4);

  v16qi signed_bytes = {-128, -1, 0, 1, 127, 64, -64, 5, 6, 7, 8, 9, 10, 11, 12, -13};
  v16qu unsigned_bytes = (v16qu)signed_bytes;
  PRINT("sext", __builtin_convertvector(signed_bytes, v16hi), 16);
  PRINT("zext", __builtin_convertvector(unsigned_bytes, v16hu), 16);
  PRINT("trunc", __builtin_convertvector((v4si){70000, -70000, 32767, -32769}, v4hi), 4);

  v4sf floats = __builtin_convertvector(a, v4sf);
  v4si back = __builtin_convertvector(floats * (v4sf){1.5f, 1.5f, 1.5f, 1.5f}, v4si);
  v4df doubles = __builtin_convertvector(floats, v4df);
  printf("float %.2f %.2f %.2f %.2f\n", floats[0], floats[1], floats[2], floats[3]);
  PRINT("back", back, 4);
  printf("double %.3f %.3f\n", doubles[1] / 3, doubles[3] / 3);

  if (__builtin_cpu_supports("avx2"))
    intrinsics();
  else
    printf("cvt -465638526377578752\nandnot 6292942421676408185\ncastps 1.50 -2.25 3.00 4.75\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_cpu_supports([[V_0:__v[0-9]+]]: *const i8) -> bool;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_ps {
// CHECK-NEXT:     __v: std::simd::Simd<f32, 4>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __storeu_si256 {
// CHECK-NEXT:     __v: std::simd::Simd<i64, 4>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sse")]
// CHECK-NEXT: unsafe fn _mm_storeu_ps(__p: *mut f32, __a: std::simd::Simd<f32, 4>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((*(__p as *mut __storeu_ps)).__v).write_unaligned(__a);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_storeu_si256(__p: *mut std::simd::Simd<i64, 4>, __a: std::simd::Simd<i64, 4>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::addr_of_mut!((*(__p as *mut __storeu_si256)).__v).write_unaligned(__a);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] =
// CHECK-NEXT:                 std::simd::Simd::from_array([__h, __g, __f, __e, __d, __c, __b, __a]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set_epi64x(__a: i64, __b: i64, __c: i64, __d: i64) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<i64, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array([__d, __c, __b, __a]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
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
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_setr_epi64x(__a: i64, __b: i64, __c: i64, __d: i64) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi64x(__d, __c, __b, __a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_set1_epi64x(__q: i64) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi64x(__q, __q, __q, __q) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_castps256_ps128(mut __a: std::simd::Simd<f32, 8>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, __a, [0, 1, 2, 3]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx")]
// CHECK-NEXT: unsafe fn _mm256_castsi256_si128(mut __a: std::simd::Simd<i64, 4>) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, __a, [0, 1]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn _mm256_andnot_si256(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 4>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<u64, 4>, std::simd::Simd<i64, 4>>(
// CHECK-NEXT:             !unsafe {
// CHECK-NEXT:                 std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<u64, 4>>(__a)
// CHECK-NEXT:             } & unsafe {
// CHECK-NEXT:                 std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<u64, 4>>(__b)
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn _mm256_cvtepi8_epi16(__V: std::simd::Simd<i64, 2>) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     // This function always performs a signed extension, but __v16qi is a char
// CHECK-NEXT:     // which may be signed or unsigned, so use __v16qs.
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i16, 16>, std::simd::Simd<i64, 4>>(
// CHECK-NEXT:             std::simd::num::SimdInt::cast::<i16>(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<std::simd::Simd<i8, 16>, std::simd::Simd<i8, 16>>(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i8, 16>>(__V)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn fold(lanes: *const i64, count: i32) -> i64 {
// CHECK-NEXT:     let __lanes_view = unsafe { std::slice::from_raw_parts(lanes as *const i64, count as usize) };
// CHECK-NEXT:     let mut h: i64 = 17;
// CHECK-NEXT:     for i in 0..count {
// CHECK-NEXT:         h = h * 1000003 + unsafe { __lanes_view[i as usize] };
// CHECK-NEXT:     }
// CHECK-NEXT:     h
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn low_half(mut v: std::simd::Simd<i32, 4>) -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(v, v, [0, 1]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn widen_undef(mut v: std::simd::Simd<i32, 4>) -> std::simd::Simd<i32, 8> {
// CHECK-NEXT:     let mut w: std::simd::Simd<i32, 8> = std::simd::simd_swizzle!(v, v, [0, 1, 2, 3, 0, 0, 0, 0]);
// CHECK-NEXT:     return std::simd::simd_swizzle!(w, w, [0, 1, 2, 3, 3, 2, 1, 0]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn interleave(
// CHECK-NEXT:     mut a: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     mut b: std::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(a, b, [0, 4, 1, 5]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn concat(
// CHECK-NEXT:     mut a: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     mut b: std::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i32, 8> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(a, b, [7, 6, 5, 4, 3, 2, 1, 0]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn reverse(mut v: std::simd::Simd<i32, 4>) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(v, v, [3, 2, 1, 0]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx2")]
// CHECK-NEXT: unsafe fn intrinsics() {
// CHECK-NEXT:     let bytes: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         _mm256_setr_epi64x(
// CHECK-NEXT:             72623859790382856i64,
// CHECK-NEXT:             -1230066625199609624i64,
// CHECK-NEXT:             2387509390608836392i64,
// CHECK-NEXT:             -3544952156018063160i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let low: std::simd::Simd<i64, 2> = unsafe { _mm256_castsi256_si128(bytes) };
// CHECK-NEXT:     let words: std::simd::Simd<i64, 4> = unsafe { _mm256_cvtepi8_epi16(low) };
// CHECK-NEXT:     let masked: std::simd::Simd<i64, 4> =
// CHECK-NEXT:         unsafe { _mm256_andnot_si256(bytes, _mm256_set1_epi64x(71777214294589695i64)) };
// CHECK-NEXT:     let floats: std::simd::Simd<f32, 8> = unsafe {
// CHECK-NEXT:         _mm256_setr_ps(
// CHECK-NEXT:             1.5f32, -2.25f32, 3.0f32, 4.75f32, 5.5f32, 6.0f32, -7.0f32, 8.25f32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let low_floats: std::simd::Simd<f32, 4> = unsafe { _mm256_castps256_ps128(floats) };
// CHECK-NEXT:     let mut out: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:     unsafe { _mm256_storeu_si256(out.0.as_mut_ptr() as *mut std::simd::Simd<i64, 4>, words) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"cvt %lld\n".as_ptr(),
// CHECK-NEXT:             fold(out.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     unsafe { _mm256_storeu_si256(out.0.as_mut_ptr() as *mut std::simd::Simd<i64, 4>, masked) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"andnot %lld\n".as_ptr(),
// CHECK-NEXT:             fold(out.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut f: __SlateAlign16<[f32; 4]> = __SlateAlign16([0 as f32; 4]);
// CHECK-NEXT:     unsafe { _mm_storeu_ps(f.0.as_mut_ptr() as *mut f32, low_floats) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"castps %.2f %.2f %.2f %.2f\n".as_ptr(),
// CHECK-NEXT:             f.0[0] as f64,
// CHECK-NEXT:             f.0[1] as f64,
// CHECK-NEXT:             f.0[2] as f64,
// CHECK-NEXT:             f.0[3] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let a: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([1i32, -2i32, 3i32, -4i32]);
// CHECK-NEXT:     let b: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([100i32, 200i32, 300i32, 400i32]);
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 2]> = __SlateAlign16([0i64; 2]);
// CHECK-NEXT:         for i in 0i32..2i32 {
// CHECK-NEXT:             lanes.0[i as usize] = low_half(a)[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"low".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 2i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:         for i in 0i32..4i32 {
// CHECK-NEXT:             lanes.0[i as usize] = widen_undef(a)[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"widen".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:         for i in 0i32..4i32 {
// CHECK-NEXT:             lanes.0[i as usize] = interleave(a, b)[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"interleave".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 8]> = __SlateAlign16([0i64; 8]);
// CHECK-NEXT:         for i in 0i32..8i32 {
// CHECK-NEXT:             lanes.0[i as usize] = concat(a, b)[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"concat".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 8i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:         for i in 0i32..4i32 {
// CHECK-NEXT:             lanes.0[i as usize] = reverse(b)[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"reverse".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let bits: std::simd::Simd<u64, 4> =
// CHECK-NEXT:         std::simd::Simd::from_array([0u64, 1u64, 9223372036854775808u64, 18446744069414584320u64]);
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:         for i in 0i32..4i32 {
// CHECK-NEXT:             lanes.0[i as usize] = (!bits)[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"not".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:         for i in 0i32..4i32 {
// CHECK-NEXT:             lanes.0[i as usize] = (-a)[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"neg".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:         for i in 0i32..4i32 {
// CHECK-NEXT:             lanes.0[i as usize] = (!(-a))[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"notneg".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let signed_bytes: std::simd::Simd<i8, 16> = std::simd::Simd::from_array([
// CHECK-NEXT:         -128i8, -1i8, 0i8, 1i8, 127i8, 64i8, -64i8, 5i8, 6i8, 7i8, 8i8, 9i8, 10i8, 11i8, 12i8,
// CHECK-NEXT:         -13i8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let unsigned_bytes: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i8, 16>, std::simd::Simd<u8, 16>>(signed_bytes)
// CHECK-NEXT:     };
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 16]> = __SlateAlign16([0i64; 16]);
// CHECK-NEXT:         for i in 0i32..16i32 {
// CHECK-NEXT:             lanes.0[i as usize] =
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<i16>(signed_bytes)[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"sext".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 16i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 16]> = __SlateAlign16([0i64; 16]);
// CHECK-NEXT:         for i in 0i32..16i32 {
// CHECK-NEXT:             lanes.0[i as usize] =
// CHECK-NEXT:                 (std::simd::num::SimdUint::cast::<u16>(unsigned_bytes)[i as usize] as u64) as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"zext".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 16i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < 4 {
// CHECK-NEXT:             lanes.0[i as usize] = std::simd::num::SimdInt::cast::<i16>(unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_2]] =
// CHECK-NEXT:                         std::simd::Simd::from_array([70000i32, -70000i32, 32767i32, -32769i32]);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             })[i as usize] as i64;
// CHECK-NEXT:             i += 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"trunc".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let floats: std::simd::Simd<f32, 4> = std::simd::num::SimdInt::cast::<f32>(a);
// CHECK-NEXT:     let back: std::simd::Simd<i32, 4> = std::simd::num::SimdFloat::cast::<i32>(
// CHECK-NEXT:         floats
// CHECK-NEXT:             * unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_3]] =
// CHECK-NEXT:                         std::simd::Simd::from_array([1.5f32, 1.5f32, 1.5f32, 1.5f32]);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             },
// CHECK-NEXT:     );
// CHECK-NEXT:     let doubles: std::simd::Simd<f64, 4> = std::simd::num::SimdFloat::cast::<f64>(floats);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"float %.2f %.2f %.2f %.2f\n".as_ptr(),
// CHECK-NEXT:             floats[0] as f64,
// CHECK-NEXT:             floats[1] as f64,
// CHECK-NEXT:             floats[2] as f64,
// CHECK-NEXT:             floats[3] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut lanes: __SlateAlign16<[i64; 4]> = __SlateAlign16([0i64; 4]);
// CHECK-NEXT:         for i in 0i32..4i32 {
// CHECK-NEXT:             lanes.0[i as usize] = back[i as usize] as i64;
// CHECK-NEXT:         }
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%s %lld\n".as_ptr(),
// CHECK-NEXT:                 c"back".as_ptr() as *mut i8,
// CHECK-NEXT:                 fold(lanes.0.as_mut_ptr() as *const i64, 4i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if !(0i32 != 0i32) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"double %.3f %.3f\n".as_ptr(),
// CHECK-NEXT:             doubles[1] / (3i32 as f64),
// CHECK-NEXT:             doubles[3] / (3i32 as f64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     if std::arch::is_x86_feature_detected!("avx2") {
// CHECK-NEXT:         unsafe { intrinsics() };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "cvt -465638526377578752\nandnot 6292942421676408185\ncastps 1.50 -2.25 3.00 4.75"
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
