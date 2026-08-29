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
  int sse2_out[4];
  signed char ssse3_out[16];
  int avx2_out[8];

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
// LOWERING: #![feature(simd_ffi)]
// LOWERING-NEXT: #![feature(portable_simd)]
// LOWERING-NEXT: #![feature(abi_unadjusted)]
// LOWERING-NEXT: #![feature(link_llvm_intrinsics)]
// LOWERING-NEXT: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __storeu_si128 {
// LOWERING-NEXT:     __v: [i64; 2],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __storeu_si256 {
// LOWERING-NEXT:     __v: [i64; 4],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set_epi32(arg41: i32, arg42: i32, arg43: i32, arg44: i32) -> [i64; 2] {
// LOWERING-NEXT:     let mut __i3: i32 = 0;
// LOWERING-NEXT:     let mut __i2: i32 = 0;
// LOWERING-NEXT:     let mut __i1: i32 = 0;
// LOWERING-NEXT:     let mut __i0: i32 = 0;
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     __i3 = arg41;
// LOWERING-NEXT:     __i2 = arg42;
// LOWERING-NEXT:     __i1 = arg43;
// LOWERING-NEXT:     __i0 = arg44;
// LOWERING-NEXT:     let _v0: i32 = __i0;
// LOWERING-NEXT:     let _v1: i32 = __i1;
// LOWERING-NEXT:     let _v2: i32 = __i2;
// LOWERING-NEXT:     let _v3: i32 = __i3;
// LOWERING-NEXT:     let _v4: [i32; 4] = [_v0, _v1, _v2, _v3];
// LOWERING-NEXT:     let _v5: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>(_v4) };
// LOWERING-NEXT:     *__retval = _v5;
// LOWERING-NEXT:     let _v6: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set1_epi32(arg40: i32) -> [i64; 2] {
// LOWERING-NEXT:     let mut __i: i32 = 0;
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     __i = arg40;
// LOWERING-NEXT:     let _v0: i32 = __i;
// LOWERING-NEXT:     let _v1: i32 = __i;
// LOWERING-NEXT:     let _v2: i32 = __i;
// LOWERING-NEXT:     let _v3: i32 = __i;
// LOWERING-NEXT:     let _v4: [i64; 2] = _mm_set_epi32(_v0, _v1, _v2, _v3);
// LOWERING-NEXT:     *__retval = _v4;
// LOWERING-NEXT:     let _v5: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v5;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_add_epi32(arg38: [i64; 2], arg39: [i64; 2]) -> [i64; 2] {
// LOWERING-NEXT:     let mut __a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut __b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     *__a = arg38;
// LOWERING-NEXT:     *__b = arg39;
// LOWERING-NEXT:     let _v0: [i64; 2] = *__a;
// LOWERING-NEXT:     let _v1: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>(_v0) };
// LOWERING-NEXT:     let _v2: [i64; 2] = *__b;
// LOWERING-NEXT:     let _v3: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>(_v2) };
// LOWERING-NEXT:     let _v4: [u32; 4] = [_v1[0usize] + _v3[0usize], _v1[1usize] + _v3[1usize], _v1[2usize] + _v3[2usize], _v1[3usize] + _v3[3usize]];
// LOWERING-NEXT:     let _v5: [i64; 2] = unsafe { std::mem::transmute::<[u32; 4], [i64; 2]>(_v4) };
// LOWERING-NEXT:     *__retval = _v5;
// LOWERING-NEXT:     let _v6: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_storeu_si128(arg36: *mut [i64; 2], arg37: [i64; 2]) {
// LOWERING-NEXT:     let mut __p: *mut [i64; 2] = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     __p = arg36;
// LOWERING-NEXT:     *__b = arg37;
// LOWERING-NEXT:     let _v0: [i64; 2] = *__b;
// LOWERING-NEXT:     let _v1: *mut [i64; 2] = __p;
// LOWERING-NEXT:     let _v2: *mut __storeu_si128 = _v1 as *mut __storeu_si128;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v2).__v = _v0;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn sse2_probe(arg35: *mut i32) {
// LOWERING-NEXT:     let mut out: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut c: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     out = arg35;
// LOWERING-NEXT:     let _v0: i32 = 5;
// LOWERING-NEXT:     let _v1: [i64; 2] = _mm_set1_epi32(_v0);
// LOWERING-NEXT:     *a = _v1;
// LOWERING-NEXT:     let _v2: i32 = 3;
// LOWERING-NEXT:     let _v3: [i64; 2] = _mm_set1_epi32(_v2);
// LOWERING-NEXT:     *b = _v3;
// LOWERING-NEXT:     let _v4: [i64; 2] = *a;
// LOWERING-NEXT:     let _v5: [i64; 2] = *b;
// LOWERING-NEXT:     let _v6: [i64; 2] = _mm_add_epi32(_v4, _v5);
// LOWERING-NEXT:     *c = _v6;
// LOWERING-NEXT:     let _v7: *mut i32 = out;
// LOWERING-NEXT:     let _v8: *mut [i64; 2] = _v7 as *mut [i64; 2];
// LOWERING-NEXT:     let _v9: [i64; 2] = *c;
// LOWERING-NEXT:     _mm_storeu_si128(_v8, _v9);
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set_epi8(arg19: i8, arg20: i8, arg21: i8, arg22: i8, arg23: i8, arg24: i8, arg25: i8, arg26: i8, arg27: i8, arg28: i8, arg29: i8, arg30: i8, arg31: i8, arg32: i8, arg33: i8, arg34: i8) -> [i64; 2] {
// LOWERING-NEXT:     let mut __b15: i8 = 0;
// LOWERING-NEXT:     let mut __b14: i8 = 0;
// LOWERING-NEXT:     let mut __b13: i8 = 0;
// LOWERING-NEXT:     let mut __b12: i8 = 0;
// LOWERING-NEXT:     let mut __b11: i8 = 0;
// LOWERING-NEXT:     let mut __b10: i8 = 0;
// LOWERING-NEXT:     let mut __b9: i8 = 0;
// LOWERING-NEXT:     let mut __b8: i8 = 0;
// LOWERING-NEXT:     let mut __b7: i8 = 0;
// LOWERING-NEXT:     let mut __b6: i8 = 0;
// LOWERING-NEXT:     let mut __b5: i8 = 0;
// LOWERING-NEXT:     let mut __b4: i8 = 0;
// LOWERING-NEXT:     let mut __b3: i8 = 0;
// LOWERING-NEXT:     let mut __b2: i8 = 0;
// LOWERING-NEXT:     let mut __b1: i8 = 0;
// LOWERING-NEXT:     let mut __b0: i8 = 0;
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     __b15 = arg19;
// LOWERING-NEXT:     __b14 = arg20;
// LOWERING-NEXT:     __b13 = arg21;
// LOWERING-NEXT:     __b12 = arg22;
// LOWERING-NEXT:     __b11 = arg23;
// LOWERING-NEXT:     __b10 = arg24;
// LOWERING-NEXT:     __b9 = arg25;
// LOWERING-NEXT:     __b8 = arg26;
// LOWERING-NEXT:     __b7 = arg27;
// LOWERING-NEXT:     __b6 = arg28;
// LOWERING-NEXT:     __b5 = arg29;
// LOWERING-NEXT:     __b4 = arg30;
// LOWERING-NEXT:     __b3 = arg31;
// LOWERING-NEXT:     __b2 = arg32;
// LOWERING-NEXT:     __b1 = arg33;
// LOWERING-NEXT:     __b0 = arg34;
// LOWERING-NEXT:     let _v0: i8 = __b0;
// LOWERING-NEXT:     let _v1: i8 = __b1;
// LOWERING-NEXT:     let _v2: i8 = __b2;
// LOWERING-NEXT:     let _v3: i8 = __b3;
// LOWERING-NEXT:     let _v4: i8 = __b4;
// LOWERING-NEXT:     let _v5: i8 = __b5;
// LOWERING-NEXT:     let _v6: i8 = __b6;
// LOWERING-NEXT:     let _v7: i8 = __b7;
// LOWERING-NEXT:     let _v8: i8 = __b8;
// LOWERING-NEXT:     let _v9: i8 = __b9;
// LOWERING-NEXT:     let _v10: i8 = __b10;
// LOWERING-NEXT:     let _v11: i8 = __b11;
// LOWERING-NEXT:     let _v12: i8 = __b12;
// LOWERING-NEXT:     let _v13: i8 = __b13;
// LOWERING-NEXT:     let _v14: i8 = __b14;
// LOWERING-NEXT:     let _v15: i8 = __b15;
// LOWERING-NEXT:     let _v16: [i8; 16] = [_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12, _v13, _v14, _v15];
// LOWERING-NEXT:     let _v17: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>(_v16) };
// LOWERING-NEXT:     *__retval = _v17;
// LOWERING-NEXT:     let _v18: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v18;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set1_epi8(arg18: i8) -> [i64; 2] {
// LOWERING-NEXT:     let mut __b: i8 = 0;
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     __b = arg18;
// LOWERING-NEXT:     let _v0: i8 = __b;
// LOWERING-NEXT:     let _v1: i8 = __b;
// LOWERING-NEXT:     let _v2: i8 = __b;
// LOWERING-NEXT:     let _v3: i8 = __b;
// LOWERING-NEXT:     let _v4: i8 = __b;
// LOWERING-NEXT:     let _v5: i8 = __b;
// LOWERING-NEXT:     let _v6: i8 = __b;
// LOWERING-NEXT:     let _v7: i8 = __b;
// LOWERING-NEXT:     let _v8: i8 = __b;
// LOWERING-NEXT:     let _v9: i8 = __b;
// LOWERING-NEXT:     let _v10: i8 = __b;
// LOWERING-NEXT:     let _v11: i8 = __b;
// LOWERING-NEXT:     let _v12: i8 = __b;
// LOWERING-NEXT:     let _v13: i8 = __b;
// LOWERING-NEXT:     let _v14: i8 = __b;
// LOWERING-NEXT:     let _v15: i8 = __b;
// LOWERING-NEXT:     let _v16: [i64; 2] = _mm_set_epi8(_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7, _v8, _v9, _v10, _v11, _v12, _v13, _v14, _v15);
// LOWERING-NEXT:     *__retval = _v16;
// LOWERING-NEXT:     let _v17: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v17;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_setzero_si128() -> [i64; 2] {
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let _v0: [i64; 2] = [0, 0];
// LOWERING-NEXT:     *__retval = _v0;
// LOWERING-NEXT:     let _v1: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v1;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-NEXT: unsafe fn _mm_abs_epi8(arg17: [i64; 2]) -> [i64; 2] {
// LOWERING-NEXT:     let mut __a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     *__a = arg17;
// LOWERING-NEXT:     let _v0: [i64; 2] = *__a;
// LOWERING-NEXT:     let _v1: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>(_v0) };
// LOWERING-NEXT:     let _v2: [i8; 16] = [_v1[0usize].wrapping_abs(), _v1[1usize].wrapping_abs(), _v1[2usize].wrapping_abs(), _v1[3usize].wrapping_abs(), _v1[4usize].wrapping_abs(), _v1[5usize].wrapping_abs(), _v1[6usize].wrapping_abs(), _v1[7usize].wrapping_abs(), _v1[8usize].wrapping_abs(), _v1[9usize].wrapping_abs(), _v1[10usize].wrapping_abs(), _v1[11usize].wrapping_abs(), _v1[12usize].wrapping_abs(), _v1[13usize].wrapping_abs(), _v1[14usize].wrapping_abs(), _v1[15usize].wrapping_abs()];
// LOWERING-NEXT:     let _v3: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>(_v2) };
// LOWERING-NEXT:     *__retval = _v3;
// LOWERING-NEXT:     let _v4: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v4;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-NEXT: unsafe fn _mm_shuffle_epi8(arg15: [i64; 2], arg16: [i64; 2]) -> [i64; 2] {
// LOWERING-NEXT:     let mut __a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut __b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     *__a = arg15;
// LOWERING-NEXT:     *__b = arg16;
// LOWERING-NEXT:     let _v0: [i64; 2] = *__a;
// LOWERING-NEXT:     let _v1: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>(_v0) };
// LOWERING-NEXT:     let _v2: [i64; 2] = *__b;
// LOWERING-NEXT:     let _v3: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>(_v2) };
// LOWERING-NEXT:     let _v4: [i8; 16] = unsafe { __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(std::simd::Simd::from_array(_v1), std::simd::Simd::from_array(_v3)) }.to_array();
// LOWERING-NEXT:     let _v5: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>(_v4) };
// LOWERING-NEXT:     *__retval = _v5;
// LOWERING-NEXT:     let _v6: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "sse3,ssse3")]
// LOWERING-NEXT: unsafe fn ssse3_probe(arg14: *mut i8) {
// LOWERING-NEXT:     let mut out: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut idx: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut abs_a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut shuf: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     out = arg14;
// LOWERING-NEXT:     let _v0: i8 = -1;
// LOWERING-NEXT:     let _v1: [i64; 2] = _mm_set1_epi8(_v0);
// LOWERING-NEXT:     *a = _v1;
// LOWERING-NEXT:     let _v2: [i64; 2] = _mm_setzero_si128();
// LOWERING-NEXT:     *idx = _v2;
// LOWERING-NEXT:     let _v3: [i64; 2] = *a;
// LOWERING-NEXT:     let _v4: [i64; 2] = unsafe { _mm_abs_epi8(_v3) };
// LOWERING-NEXT:     *abs_a = _v4;
// LOWERING-NEXT:     let _v5: [i64; 2] = *abs_a;
// LOWERING-NEXT:     let _v6: [i64; 2] = *idx;
// LOWERING-NEXT:     let _v7: [i64; 2] = unsafe { _mm_shuffle_epi8(_v5, _v6) };
// LOWERING-NEXT:     *shuf = _v7;
// LOWERING-NEXT:     let _v8: *mut i8 = out;
// LOWERING-NEXT:     let _v9: *mut [i64; 2] = _v8 as *mut [i64; 2];
// LOWERING-NEXT:     let _v10: [i64; 2] = *shuf;
// LOWERING-NEXT:     _mm_storeu_si128(_v9, _v10);
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm256_set_epi32(arg6: i32, arg7: i32, arg8: i32, arg9: i32, arg10: i32, arg11: i32, arg12: i32, arg13: i32) -> [i64; 4] {
// LOWERING-NEXT:     let mut __i0: i32 = 0;
// LOWERING-NEXT:     let mut __i1: i32 = 0;
// LOWERING-NEXT:     let mut __i2: i32 = 0;
// LOWERING-NEXT:     let mut __i3: i32 = 0;
// LOWERING-NEXT:     let mut __i4: i32 = 0;
// LOWERING-NEXT:     let mut __i5: i32 = 0;
// LOWERING-NEXT:     let mut __i6: i32 = 0;
// LOWERING-NEXT:     let mut __i7: i32 = 0;
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     __i0 = arg6;
// LOWERING-NEXT:     __i1 = arg7;
// LOWERING-NEXT:     __i2 = arg8;
// LOWERING-NEXT:     __i3 = arg9;
// LOWERING-NEXT:     __i4 = arg10;
// LOWERING-NEXT:     __i5 = arg11;
// LOWERING-NEXT:     __i6 = arg12;
// LOWERING-NEXT:     __i7 = arg13;
// LOWERING-NEXT:     let _v0: i32 = __i7;
// LOWERING-NEXT:     let _v1: i32 = __i6;
// LOWERING-NEXT:     let _v2: i32 = __i5;
// LOWERING-NEXT:     let _v3: i32 = __i4;
// LOWERING-NEXT:     let _v4: i32 = __i3;
// LOWERING-NEXT:     let _v5: i32 = __i2;
// LOWERING-NEXT:     let _v6: i32 = __i1;
// LOWERING-NEXT:     let _v7: i32 = __i0;
// LOWERING-NEXT:     let _v8: [i32; 8] = [_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7];
// LOWERING-NEXT:     let _v9: [i64; 4] = unsafe { std::mem::transmute::<[i32; 8], [i64; 4]>(_v8) };
// LOWERING-NEXT:     *__retval = _v9;
// LOWERING-NEXT:     let _v10: [i64; 4] = *__retval;
// LOWERING-NEXT:     return _v10;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm256_set1_epi32(arg5: i32) -> [i64; 4] {
// LOWERING-NEXT:     let mut __i: i32 = 0;
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     __i = arg5;
// LOWERING-NEXT:     let _v0: i32 = __i;
// LOWERING-NEXT:     let _v1: i32 = __i;
// LOWERING-NEXT:     let _v2: i32 = __i;
// LOWERING-NEXT:     let _v3: i32 = __i;
// LOWERING-NEXT:     let _v4: i32 = __i;
// LOWERING-NEXT:     let _v5: i32 = __i;
// LOWERING-NEXT:     let _v6: i32 = __i;
// LOWERING-NEXT:     let _v7: i32 = __i;
// LOWERING-NEXT:     let _v8: [i64; 4] = unsafe { _mm256_set_epi32(_v0, _v1, _v2, _v3, _v4, _v5, _v6, _v7) };
// LOWERING-NEXT:     *__retval = _v8;
// LOWERING-NEXT:     let _v9: [i64; 4] = *__retval;
// LOWERING-NEXT:     return _v9;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm256_add_epi32(arg3: [i64; 4], arg4: [i64; 4]) -> [i64; 4] {
// LOWERING-NEXT:     let mut __a: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut __b: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     *__a = arg3;
// LOWERING-NEXT:     *__b = arg4;
// LOWERING-NEXT:     let _v0: [i64; 4] = *__a;
// LOWERING-NEXT:     let _v1: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>(_v0) };
// LOWERING-NEXT:     let _v2: [i64; 4] = *__b;
// LOWERING-NEXT:     let _v3: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>(_v2) };
// LOWERING-NEXT:     let _v4: [u32; 8] = [_v1[0usize] + _v3[0usize], _v1[1usize] + _v3[1usize], _v1[2usize] + _v3[2usize], _v1[3usize] + _v3[3usize], _v1[4usize] + _v3[4usize], _v1[5usize] + _v3[5usize], _v1[6usize] + _v3[6usize], _v1[7usize] + _v3[7usize]];
// LOWERING-NEXT:     let _v5: [i64; 4] = unsafe { std::mem::transmute::<[u32; 8], [i64; 4]>(_v4) };
// LOWERING-NEXT:     *__retval = _v5;
// LOWERING-NEXT:     let _v6: [i64; 4] = *__retval;
// LOWERING-NEXT:     return _v6;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm256_storeu_si256(arg1: *mut [i64; 4], arg2: [i64; 4]) {
// LOWERING-NEXT:     let mut __p: *mut [i64; 4] = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __a: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     __p = arg1;
// LOWERING-NEXT:     *__a = arg2;
// LOWERING-NEXT:     let _v0: [i64; 4] = *__a;
// LOWERING-NEXT:     let _v1: *mut [i64; 4] = __p;
// LOWERING-NEXT:     let _v2: *mut __storeu_si256 = _v1 as *mut __storeu_si256;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v2).__v = _v0;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn avx2_probe(arg0: *mut i32) {
// LOWERING-NEXT:     let mut out: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut a: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut b: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut c: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     out = arg0;
// LOWERING-NEXT:     let _v0: i32 = 7;
// LOWERING-NEXT:     let _v1: [i64; 4] = unsafe { _mm256_set1_epi32(_v0) };
// LOWERING-NEXT:     *a = _v1;
// LOWERING-NEXT:     let _v2: i32 = 2;
// LOWERING-NEXT:     let _v3: [i64; 4] = unsafe { _mm256_set1_epi32(_v2) };
// LOWERING-NEXT:     *b = _v3;
// LOWERING-NEXT:     let _v4: [i64; 4] = *a;
// LOWERING-NEXT:     let _v5: [i64; 4] = *b;
// LOWERING-NEXT:     let _v6: [i64; 4] = unsafe { _mm256_add_epi32(_v4, _v5) };
// LOWERING-NEXT:     *c = _v6;
// LOWERING-NEXT:     let _v7: *mut i32 = out;
// LOWERING-NEXT:     let _v8: *mut [i64; 4] = _v7 as *mut [i64; 4];
// LOWERING-NEXT:     let _v9: [i64; 4] = *c;
// LOWERING-NEXT:     unsafe { _mm256_storeu_si256(_v8, _v9) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut sse2_out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut ssse3_out: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-NEXT:     let mut avx2_out: aligned::Aligned<aligned::A16, [i32; 8]> = aligned::Aligned([0; 8]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i32 = sse2_out.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     sse2_probe(_v1);
// LOWERING-NEXT:     let _v2: *mut i8 = ssse3_out.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     unsafe { ssse3_probe(_v2) };
// LOWERING-NEXT:     let _v3: *mut i32 = avx2_out.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     unsafe { avx2_probe(_v3) };
// LOWERING-NEXT:     let _v4: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i64 = 0;
// LOWERING-NEXT:     let _v6: i32 = sse2_out[(_v5 as usize)];
// LOWERING-NEXT:     let _v7: i64 = 1;
// LOWERING-NEXT:     let _v8: i32 = sse2_out[(_v7 as usize)];
// LOWERING-NEXT:     let _v9: i64 = 2;
// LOWERING-NEXT:     let _v10: i32 = sse2_out[(_v9 as usize)];
// LOWERING-NEXT:     let _v11: i64 = 3;
// LOWERING-NEXT:     let _v12: i32 = sse2_out[(_v11 as usize)];
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v4 as *const i8, _v6, _v8, _v10, _v12) };
// LOWERING-NEXT:     let _v14: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v15: i64 = 0;
// LOWERING-NEXT:     let _v16: i8 = ssse3_out[(_v15 as usize)];
// LOWERING-NEXT:     let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:     let _v18: i64 = 15;
// LOWERING-NEXT:     let _v19: i8 = ssse3_out[(_v18 as usize)];
// LOWERING-NEXT:     let _v20: i32 = _v19 as i32;
// LOWERING-NEXT:     let _v21: i32 = unsafe { printf(_v14 as *const i8, _v17, _v20) };
// LOWERING-NEXT:     let _v22: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v23: i64 = 0;
// LOWERING-NEXT:     let _v24: i32 = avx2_out[(_v23 as usize)];
// LOWERING-NEXT:     let _v25: i64 = 1;
// LOWERING-NEXT:     let _v26: i32 = avx2_out[(_v25 as usize)];
// LOWERING-NEXT:     let _v27: i64 = 2;
// LOWERING-NEXT:     let _v28: i32 = avx2_out[(_v27 as usize)];
// LOWERING-NEXT:     let _v29: i64 = 3;
// LOWERING-NEXT:     let _v30: i32 = avx2_out[(_v29 as usize)];
// LOWERING-NEXT:     let _v31: i64 = 4;
// LOWERING-NEXT:     let _v32: i32 = avx2_out[(_v31 as usize)];
// LOWERING-NEXT:     let _v33: i64 = 5;
// LOWERING-NEXT:     let _v34: i32 = avx2_out[(_v33 as usize)];
// LOWERING-NEXT:     let _v35: i64 = 6;
// LOWERING-NEXT:     let _v36: i32 = avx2_out[(_v35 as usize)];
// LOWERING-NEXT:     let _v37: i64 = 7;
// LOWERING-NEXT:     let _v38: i32 = avx2_out[(_v37 as usize)];
// LOWERING-NEXT:     let _v39: i32 = unsafe { printf(_v22 as *const i8, _v24, _v26, _v28, _v30, _v32, _v34, _v36, _v38) };
// LOWERING-NEXT:     let _v40: i32 = 0;
// LOWERING-NEXT:     __retval = _v40;
// LOWERING-NEXT:     let _v41: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v41 as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "unadjusted" {
// LOWERING-NEXT:     #[link_name = "llvm.x86.ssse3.pshuf.b.128"]
// LOWERING-NEXT:     fn __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(_0: std::simd::Simd<i8, 16>, _1: std::simd::Simd<i8, 16>) -> std::simd::Simd<i8, 16>;
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(simd_ffi)]
// REWRITES-NEXT: #![feature(portable_simd)]
// REWRITES-NEXT: #![feature(abi_unadjusted)]
// REWRITES-NEXT: #![feature(link_llvm_intrinsics)]
// REWRITES-NEXT: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __storeu_si128 {
// REWRITES-NEXT:     __v: [i64; 2],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __storeu_si256 {
// REWRITES-NEXT:     __v: [i64; 4],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set_epi32(arg41: i32, arg42: i32, arg43: i32, arg44: i32) -> [i64; 2] {
// REWRITES-NEXT: let mut __i3: i32 = arg41;
// REWRITES-NEXT: let mut __i2: i32 = arg42;
// REWRITES-NEXT: let mut __i1: i32 = arg43;
// REWRITES-NEXT: let mut __i0: i32 = arg44;
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v5: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>([__i0, __i1, __i2, __i3]) };
// REWRITES-NEXT: *__retval = _v5;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set1_epi32(arg40: i32) -> [i64; 2] {
// REWRITES-NEXT: let mut __i: i32 = arg40;
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v4: [i64; 2] = _mm_set_epi32(__i, __i, __i, __i);
// REWRITES-NEXT: *__retval = _v4;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_add_epi32(arg38: [i64; 2], arg39: [i64; 2]) -> [i64; 2] {
// REWRITES-NEXT: let mut __a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut __b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: *__a = arg38;
// REWRITES-NEXT: *__b = arg39;
// REWRITES-NEXT: let _v1: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>(*__a) };
// REWRITES-NEXT: let _v3: [u32; 4] = unsafe { std::mem::transmute::<[i64; 2], [u32; 4]>(*__b) };
// REWRITES-NEXT: let _v5: [i64; 2] = unsafe { std::mem::transmute::<[u32; 4], [i64; 2]>([_v1[0usize] + _v3[0usize], _v1[1usize] + _v3[1usize], _v1[2usize] + _v3[2usize], _v1[3usize] + _v3[3usize]]) };
// REWRITES-NEXT: *__retval = _v5;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_storeu_si128(arg36: *mut [i64; 2], arg37: [i64; 2]) {
// REWRITES-NEXT: let mut __p: *mut [i64; 2] = arg36;
// REWRITES-NEXT: let mut __b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: *__b = arg37;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*(__p as *mut __storeu_si128)).__v = *__b;
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn sse2_probe(arg35: *mut i32) {
// REWRITES-NEXT: let mut out: *mut i32 = arg35;
// REWRITES-NEXT: let mut a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut c: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v0: i32 = 5;
// REWRITES-NEXT: let _v1: [i64; 2] = _mm_set1_epi32(_v0);
// REWRITES-NEXT: *a = _v1;
// REWRITES-NEXT: let _v2: i32 = 3;
// REWRITES-NEXT: let _v3: [i64; 2] = _mm_set1_epi32(_v2);
// REWRITES-NEXT: *b = _v3;
// REWRITES-NEXT: let _v6: [i64; 2] = _mm_add_epi32(*a, *b);
// REWRITES-NEXT: *c = _v6;
// REWRITES-NEXT: _mm_storeu_si128(out as *mut [i64; 2], *c);
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set_epi8(arg19: i8, arg20: i8, arg21: i8, arg22: i8, arg23: i8, arg24: i8, arg25: i8, arg26: i8, arg27: i8, arg28: i8, arg29: i8, arg30: i8, arg31: i8, arg32: i8, arg33: i8, arg34: i8) -> [i64; 2] {
// REWRITES-NEXT: let mut __b15: i8 = arg19;
// REWRITES-NEXT: let mut __b14: i8 = arg20;
// REWRITES-NEXT: let mut __b13: i8 = arg21;
// REWRITES-NEXT: let mut __b12: i8 = arg22;
// REWRITES-NEXT: let mut __b11: i8 = arg23;
// REWRITES-NEXT: let mut __b10: i8 = arg24;
// REWRITES-NEXT: let mut __b9: i8 = arg25;
// REWRITES-NEXT: let mut __b8: i8 = arg26;
// REWRITES-NEXT: let mut __b7: i8 = arg27;
// REWRITES-NEXT: let mut __b6: i8 = arg28;
// REWRITES-NEXT: let mut __b5: i8 = arg29;
// REWRITES-NEXT: let mut __b4: i8 = arg30;
// REWRITES-NEXT: let mut __b3: i8 = arg31;
// REWRITES-NEXT: let mut __b2: i8 = arg32;
// REWRITES-NEXT: let mut __b1: i8 = arg33;
// REWRITES-NEXT: let mut __b0: i8 = arg34;
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v17: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>([__b0, __b1, __b2, __b3, __b4, __b5, __b6, __b7, __b8, __b9, __b10, __b11, __b12, __b13, __b14, __b15]) };
// REWRITES-NEXT: *__retval = _v17;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set1_epi8(arg18: i8) -> [i64; 2] {
// REWRITES-NEXT: let mut __b: i8 = arg18;
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v16: [i64; 2] = _mm_set_epi8(__b, __b, __b, __b, __b, __b, __b, __b, __b, __b, __b, __b, __b, __b, __b, __b);
// REWRITES-NEXT: *__retval = _v16;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_setzero_si128() -> [i64; 2] {
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: *__retval = [0, 0];
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-NEXT: unsafe fn _mm_abs_epi8(arg17: [i64; 2]) -> [i64; 2] {
// REWRITES-NEXT: let mut __a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: *__a = arg17;
// REWRITES-NEXT: let _v1: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>(*__a) };
// REWRITES-NEXT: let _v2: [i8; 16] = [_v1[0usize].wrapping_abs(), _v1[1usize].wrapping_abs(), _v1[2usize].wrapping_abs(), _v1[3usize].wrapping_abs(), _v1[4usize].wrapping_abs(), _v1[5usize].wrapping_abs(), _v1[6usize].wrapping_abs(), _v1[7usize].wrapping_abs(), _v1[8usize].wrapping_abs(), _v1[9usize].wrapping_abs(), _v1[10usize].wrapping_abs(), _v1[11usize].wrapping_abs(), _v1[12usize].wrapping_abs(), _v1[13usize].wrapping_abs(), _v1[14usize].wrapping_abs(), _v1[15usize].wrapping_abs()];
// REWRITES-NEXT: let _v3: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>(_v2) };
// REWRITES-NEXT: *__retval = _v3;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-NEXT: unsafe fn _mm_shuffle_epi8(arg15: [i64; 2], arg16: [i64; 2]) -> [i64; 2] {
// REWRITES-NEXT: let mut __a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut __b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: *__a = arg15;
// REWRITES-NEXT: *__b = arg16;
// REWRITES-NEXT: let _v1: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>(*__a) };
// REWRITES-NEXT: let _v3: [i8; 16] = unsafe { std::mem::transmute::<[i64; 2], [i8; 16]>(*__b) };
// REWRITES-NEXT: let _v4: [i8; 16] = unsafe { __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(std::simd::Simd::from_array(_v1), std::simd::Simd::from_array(_v3)) }.to_array();
// REWRITES-NEXT: let _v5: [i64; 2] = unsafe { std::mem::transmute::<[i8; 16], [i64; 2]>(_v4) };
// REWRITES-NEXT: *__retval = _v5;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "sse3,ssse3")]
// REWRITES-NEXT: unsafe fn ssse3_probe(arg14: *mut i8) {
// REWRITES-NEXT: let mut out: *mut i8 = arg14;
// REWRITES-NEXT: let mut a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut idx: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut abs_a: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut shuf: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v0: i8 = -1;
// REWRITES-NEXT: let _v1: [i64; 2] = _mm_set1_epi8(_v0);
// REWRITES-NEXT: *a = _v1;
// REWRITES-NEXT: let _v2: [i64; 2] = _mm_setzero_si128();
// REWRITES-NEXT: *idx = _v2;
// REWRITES-NEXT: let _v4: [i64; 2] = unsafe { _mm_abs_epi8(*a) };
// REWRITES-NEXT: *abs_a = _v4;
// REWRITES-NEXT: let _v7: [i64; 2] = unsafe { _mm_shuffle_epi8(*abs_a, *idx) };
// REWRITES-NEXT: *shuf = _v7;
// REWRITES-NEXT: _mm_storeu_si128(out as *mut [i64; 2], *shuf);
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm256_set_epi32(arg6: i32, arg7: i32, arg8: i32, arg9: i32, arg10: i32, arg11: i32, arg12: i32, arg13: i32) -> [i64; 4] {
// REWRITES-NEXT: let mut __i0: i32 = arg6;
// REWRITES-NEXT: let mut __i1: i32 = arg7;
// REWRITES-NEXT: let mut __i2: i32 = arg8;
// REWRITES-NEXT: let mut __i3: i32 = arg9;
// REWRITES-NEXT: let mut __i4: i32 = arg10;
// REWRITES-NEXT: let mut __i5: i32 = arg11;
// REWRITES-NEXT: let mut __i6: i32 = arg12;
// REWRITES-NEXT: let mut __i7: i32 = arg13;
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let _v9: [i64; 4] = unsafe { std::mem::transmute::<[i32; 8], [i64; 4]>([__i7, __i6, __i5, __i4, __i3, __i2, __i1, __i0]) };
// REWRITES-NEXT: *__retval = _v9;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm256_set1_epi32(arg5: i32) -> [i64; 4] {
// REWRITES-NEXT: let mut __i: i32 = arg5;
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let _v8: [i64; 4] = unsafe { _mm256_set_epi32(__i, __i, __i, __i, __i, __i, __i, __i) };
// REWRITES-NEXT: *__retval = _v8;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm256_add_epi32(arg3: [i64; 4], arg4: [i64; 4]) -> [i64; 4] {
// REWRITES-NEXT: let mut __a: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut __b: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: *__a = arg3;
// REWRITES-NEXT: *__b = arg4;
// REWRITES-NEXT: let _v1: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>(*__a) };
// REWRITES-NEXT: let _v3: [u32; 8] = unsafe { std::mem::transmute::<[i64; 4], [u32; 8]>(*__b) };
// REWRITES-NEXT: let _v5: [i64; 4] = unsafe { std::mem::transmute::<[u32; 8], [i64; 4]>([_v1[0usize] + _v3[0usize], _v1[1usize] + _v3[1usize], _v1[2usize] + _v3[2usize], _v1[3usize] + _v3[3usize], _v1[4usize] + _v3[4usize], _v1[5usize] + _v3[5usize], _v1[6usize] + _v3[6usize], _v1[7usize] + _v3[7usize]]) };
// REWRITES-NEXT: *__retval = _v5;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm256_storeu_si256(arg1: *mut [i64; 4], arg2: [i64; 4]) {
// REWRITES-NEXT: let mut __p: *mut [i64; 4] = arg1;
// REWRITES-NEXT: let mut __a: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: *__a = arg2;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*(__p as *mut __storeu_si256)).__v = *__a;
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,avx2,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn avx2_probe(arg0: *mut i32) {
// REWRITES-NEXT: let mut out: *mut i32 = arg0;
// REWRITES-NEXT: let mut a: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut b: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut c: aligned::Aligned<aligned::A32, [i64; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let _v0: i32 = 7;
// REWRITES-NEXT: let _v1: [i64; 4] = unsafe { _mm256_set1_epi32(_v0) };
// REWRITES-NEXT: *a = _v1;
// REWRITES-NEXT: let _v2: i32 = 2;
// REWRITES-NEXT: let _v3: [i64; 4] = unsafe { _mm256_set1_epi32(_v2) };
// REWRITES-NEXT: *b = _v3;
// REWRITES-NEXT: let _v6: [i64; 4] = unsafe { _mm256_add_epi32(*a, *b) };
// REWRITES-NEXT: *c = _v6;
// REWRITES-NEXT: unsafe { _mm256_storeu_si256(out as *mut [i64; 4], *c) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut sse2_out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut ssse3_out: aligned::Aligned<aligned::A16, [i8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-NEXT: let mut avx2_out: aligned::Aligned<aligned::A16, [i32; 8]> = aligned::Aligned([0; 8]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i32 = sse2_out.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: sse2_probe(_v1);
// REWRITES-NEXT: let _v2: *mut i8 = ssse3_out.as_mut_ptr() as *mut i8;
// REWRITES-NEXT: unsafe { ssse3_probe(_v2) };
// REWRITES-NEXT: let _v3: *mut i32 = avx2_out.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: unsafe { avx2_probe(_v3) };
// REWRITES-NEXT: let _v4: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i64 = 0;
// REWRITES-NEXT: let _v7: i64 = 1;
// REWRITES-NEXT: let _v9: i64 = 2;
// REWRITES-NEXT: let _v11: i64 = 3;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v4 as *const i8, sse2_out[(_v5 as usize)], sse2_out[(_v7 as usize)], sse2_out[(_v9 as usize)], sse2_out[(_v11 as usize)]) };
// REWRITES-NEXT: let _v14: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v15: i64 = 0;
// REWRITES-NEXT: let _v18: i64 = 15;
// REWRITES-NEXT: let _v21: i32 = unsafe { printf(_v14 as *const i8, ssse3_out[(_v15 as usize)] as i32, ssse3_out[(_v18 as usize)] as i32) };
// REWRITES-NEXT: let _v22: *mut i8 = b"%d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v23: i64 = 0;
// REWRITES-NEXT: let _v25: i64 = 1;
// REWRITES-NEXT: let _v27: i64 = 2;
// REWRITES-NEXT: let _v29: i64 = 3;
// REWRITES-NEXT: let _v31: i64 = 4;
// REWRITES-NEXT: let _v33: i64 = 5;
// REWRITES-NEXT: let _v35: i64 = 6;
// REWRITES-NEXT: let _v37: i64 = 7;
// REWRITES-NEXT: let _v39: i32 = unsafe { printf(_v22 as *const i8, avx2_out[(_v23 as usize)], avx2_out[(_v25 as usize)], avx2_out[(_v27 as usize)], avx2_out[(_v29 as usize)], avx2_out[(_v31 as usize)], avx2_out[(_v33 as usize)], avx2_out[(_v35 as usize)], avx2_out[(_v37 as usize)]) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "unadjusted" {
// REWRITES-NEXT:     #[link_name = "llvm.x86.ssse3.pshuf.b.128"]
// REWRITES-NEXT:     fn __slate_intrinsic_x86_ssse3_pshuf_b_128_929309e755183de6(_0: std::simd::Simd<i8, 16>, _1: std::simd::Simd<i8, 16>) -> std::simd::Simd<i8, 16>;
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
