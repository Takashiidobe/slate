/* { dg-options "/Zc:preprocessor" } */
#if defined(_M_IX86) || defined(_M_X64)
#include <intrin.h>
#include <immintrin.h>
#include <wmmintrin.h>
#include <stdlib.h>
#include <string.h>

static int lane(__m128i v, int index) {
    int out[4];
    _mm_storeu_si128((__m128i *)out, v);
    return out[index];
}

static void check_cpuid(void) {
    int basic[4];
    int extended[4];
    __cpuid(basic, 0);
    __cpuidex(extended, 0, 0);
    if (basic[0] <= 0 || memcmp(basic, extended, sizeof basic) != 0)
        abort();
}

static int has_feature(int leaf, int reg, int bit) {
    int regs[4];
    __cpuidex(regs, leaf, 0);
    return (regs[reg] >> bit) & 1;
}

static void check_sse(void) {
    __m128i a = _mm_set_epi32(4, 3, 2, 1);
    __m128i b = _mm_set1_epi32(10);
    __m128i sum = _mm_add_epi32(a, b);
    if (lane(sum, 0) != 11 || lane(sum, 3) != 14)
        abort();
    if (_mm_cvtsi128_si32(_mm_xor_si128(sum, sum)) != 0)
        abort();
    __m128i rotated = _mm_shuffle_epi32(a, 0x39);
    if (lane(rotated, 0) != 2 || lane(rotated, 3) != 1)
        abort();
    __m128i shifted = _mm_srli_si128(a, 4);
    if (lane(shifted, 0) != 2 || lane(shifted, 3) != 0)
        abort();
    __m128 f = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.5f);
    if (_mm_cvtss_f32(_mm_mul_ps(f, f)) != 2.25f)
        abort();
    __m128d d = _mm_set_pd(2.0, 0.5);
    if (_mm_cvtsd_f64(_mm_add_pd(d, d)) != 1.0)
        abort();
}

static void check_ssse3_sse41(void) {
    if (!has_feature(1, 2, 9) || !has_feature(1, 2, 19))
        return;
    __m128i bytes = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    __m128i reverse = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    __m128i swapped = _mm_shuffle_epi8(bytes, reverse);
    if (_mm_extract_epi32(swapped, 0) != 0x0c0d0e0f)
        abort();
    if (_mm_extract_epi32(_mm_insert_epi32(bytes, 99, 2), 2) != 99)
        abort();
}

static void check_aes_clmul(void) {
    if (!has_feature(1, 2, 25) || !has_feature(1, 2, 1))
        return;
    __m128i state = _mm_set_epi32(0x01234567, 0x89abcdef, 0x0f1e2d3c, 0x4b5a6978);
    __m128i key = _mm_set1_epi32(0x5a5a5a5a);
    __m128i round = _mm_aesenc_si128(state, key);
    __m128i back = _mm_aesdeclast_si128(_mm_aesenclast_si128(state, key), key);
    if (lane(back, 0) == lane(state, 0) && lane(round, 0) == lane(state, 0))
        abort();
    __m128i x = _mm_set_epi32(0, 0, 0, 3);
    __m128i product = _mm_clmulepi64_si128(x, x, 0x00);
    if (_mm_cvtsi128_si32(product) != 5)
        abort();
}

static void check_sha(void) {
    if (!has_feature(7, 1, 29))
        return;
    __m128i a = _mm_set_epi32(1, 2, 3, 4);
    __m128i b = _mm_set_epi32(5, 6, 7, 8);
    __m128i msg = _mm_sha256msg1_epu32(a, b);
    __m128i rounds = _mm_sha256rnds2_epu32(a, b, msg);
    if (lane(rounds, 0) == 0 && lane(rounds, 1) == 0)
        abort();
}

static void check_carry(void) {
    unsigned int low;
    unsigned char carry = _addcarry_u32(0, 0xffffffffu, 2, &low);
    if (carry != 1 || low != 1)
        abort();
    carry = _subborrow_u32(1, 0, 0, &low);
    if (carry != 1 || low != 0xffffffffu)
        abort();
#ifdef _M_X64
    unsigned __int64 high;
    unsigned __int64 product = _umul128(0xffffffffffffffffull, 3, &high);
    if (product != 0xfffffffffffffffdull || high != 2)
        abort();
    unsigned __int64 sum;
    carry = _addcarry_u64(1, 0xffffffffffffffffull, 1, &sum);
    if (carry != 1 || sum != 1)
        abort();
    carry = _subborrow_u64(0, 1, 2, &sum);
    if (carry != 1 || sum != 0xffffffffffffffffull)
        abort();
#endif
}

int main(void) {
    check_cpuid();
    check_sse();
    check_ssse3_sse41();
    check_aes_clmul();
    check_sha();
    check_carry();
    return 0;
}
#else
int main(void) {
    return 0;
}
#endif

// SLATE-FILECHECK-BEGIN x86_64-pc-windows-msvc i686-pc-windows-msvc aarch64-pc-windows-msvc
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "/Zc:preprocessor" }
// X86_64-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// I686-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// X86_64-PC-WINDOWS-MSVC-NEXT: #[derive(Clone, Copy)]
// I686-PC-WINDOWS-MSVC-NEXT: #[derive(Clone, Copy)]
// X86_64-PC-WINDOWS-MSVC-NEXT: union __m128 {
// I686-PC-WINDOWS-MSVC-NEXT: union __m128 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_f32: [f32; 4],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_f32: [f32; 4],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_u64: [u64; 2],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_u64: [u64; 2],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_i8: [i8; 16],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_i8: [i8; 16],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_i16: [i16; 8],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_i16: [i16; 8],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_i32: [i32; 4],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_i32: [i32; 4],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_i64: [i64; 2],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_i64: [i64; 2],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_u8: [u8; 16],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_u8: [u8; 16],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_u16: [u16; 8],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_u16: [u16; 8],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128_u32: [u32; 4],
// I686-PC-WINDOWS-MSVC-NEXT:     m128_u32: [u32; 4],
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// I686-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// X86_64-PC-WINDOWS-MSVC-NEXT: #[derive(Clone, Copy)]
// I686-PC-WINDOWS-MSVC-NEXT: #[derive(Clone, Copy)]
// X86_64-PC-WINDOWS-MSVC-NEXT: union __m128i {
// I686-PC-WINDOWS-MSVC-NEXT: union __m128i {
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128i_i8: [i8; 16],
// I686-PC-WINDOWS-MSVC-NEXT:     m128i_i8: [i8; 16],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128i_i16: [i16; 8],
// I686-PC-WINDOWS-MSVC-NEXT:     m128i_i16: [i16; 8],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128i_i32: [i32; 4],
// I686-PC-WINDOWS-MSVC-NEXT:     m128i_i32: [i32; 4],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128i_i64: [i64; 2],
// I686-PC-WINDOWS-MSVC-NEXT:     m128i_i64: [i64; 2],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128i_u8: [u8; 16],
// I686-PC-WINDOWS-MSVC-NEXT:     m128i_u8: [u8; 16],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128i_u16: [u16; 8],
// I686-PC-WINDOWS-MSVC-NEXT:     m128i_u16: [u16; 8],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128i_u32: [u32; 4],
// I686-PC-WINDOWS-MSVC-NEXT:     m128i_u32: [u32; 4],
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128i_u64: [u64; 2],
// I686-PC-WINDOWS-MSVC-NEXT:     m128i_u64: [u64; 2],
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// I686-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// X86_64-PC-WINDOWS-MSVC-NEXT: #[derive(Clone, Copy)]
// I686-PC-WINDOWS-MSVC-NEXT: #[derive(Clone, Copy)]
// X86_64-PC-WINDOWS-MSVC-NEXT: struct __m128d {
// I686-PC-WINDOWS-MSVC-NEXT: struct __m128d {
// X86_64-PC-WINDOWS-MSVC-NEXT:     m128d_f64: [f64; 2],
// I686-PC-WINDOWS-MSVC-NEXT:     m128d_f64: [f64; 2],
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: #[repr(C, align(16))]
// X86_64-PC-WINDOWS-MSVC-NEXT: struct __SlateAlign16<T>(T);
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_add_epi32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_add_pd;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_aesdeclast_si128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_aesenc_si128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_aesenclast_si128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_clmulepi64_si128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_cvtsd_f64;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_cvtsi128_si32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_cvtss_f32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_extract_epi32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_insert_epi32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_mul_ps;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_set_epi8;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_set_epi32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_set_pd;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_set_ps;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_set1_epi32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_sha256msg1_epu32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_sha256rnds2_epu32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_shuffle_epi8;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_shuffle_epi32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_srli_si128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_storeu_si128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use core::arch::x86_64::_mm_xor_si128;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_add_epi32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_add_pd;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_aesdeclast_si128;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_aesenc_si128;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_aesenclast_si128;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_clmulepi64_si128;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_cvtsd_f64;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_cvtsi128_si32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_cvtss_f32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_extract_epi32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_insert_epi32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_mul_ps;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_set_epi8;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_set_epi32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_set_pd;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_set_ps;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_set1_epi32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_sha256msg1_epu32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_sha256rnds2_epu32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_shuffle_epi8;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_shuffle_epi32;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_srli_si128;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_storeu_si128;
// I686-PC-WINDOWS-MSVC-NEXT: use core::arch::x86::_mm_xor_si128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__cpuid;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__cpuid;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::__cpuidex;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::__cpuidex;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::_addcarry_u32;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::_addcarry_u32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::_addcarry_u64;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::_subborrow_u32;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::_subborrow_u32;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::_subborrow_u64;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::_umul128;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::abort;
// X86_64-PC-WINDOWS-MSVC-NEXT: use slate_support::memcmp;
// I686-PC-WINDOWS-MSVC-NEXT: use slate_support::memcmp;
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn lane(v: __m128i, index: i32) -> i32 {
// I686-PC-WINDOWS-MSVC-NEXT: fn lane(v: __m128i, index: i32) -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut out: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32; 4]);
// I686-PC-WINDOWS-MSVC-NEXT:     let mut out: [i32; 4] = [0i32; 4];
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _mm_storeu_si128(
// I686-PC-WINDOWS-MSVC-NEXT:         _mm_storeu_si128(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (out.0.as_mut_ptr() as *mut __m128i) as *mut core::arch::x86_64::__m128i,
// X86_64-PC-WINDOWS-MSVC-NEXT:             unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(v) },
// I686-PC-WINDOWS-MSVC-NEXT:             (out.as_mut_ptr() as *mut __m128i) as *mut core::arch::x86::__m128i,
// I686-PC-WINDOWS-MSVC-NEXT:             unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(v) },
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     out.0[index as usize]
// I686-PC-WINDOWS-MSVC-NEXT:     out[index as usize]
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check_cpuid() {
// I686-PC-WINDOWS-MSVC-NEXT: fn check_cpuid() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut basic: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32; 4]);
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut extended: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32; 4]);
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe { __cpuid((basic.0.as_mut_ptr() as *mut i32) as *mut i32, 0i32 as i32) };
// I686-PC-WINDOWS-MSVC-NEXT:     let mut basic: [i32; 4] = [0i32; 4];
// I686-PC-WINDOWS-MSVC-NEXT:     let mut extended: [i32; 4] = [0i32; 4];
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe { __cpuid((basic.as_mut_ptr() as *mut i32) as *mut i32, 0i32 as i32) };
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __cpuidex(
// I686-PC-WINDOWS-MSVC-NEXT:         __cpuidex(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (extended.0.as_mut_ptr() as *mut i32) as *mut i32,
// I686-PC-WINDOWS-MSVC-NEXT:             (extended.as_mut_ptr() as *mut i32) as *mut i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_0:__v[0-9]+]]: bool = basic.0[0] <= 0
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_0:__v[0-9]+]]: bool = basic[0] <= 0
// X86_64-PC-WINDOWS-MSVC-NEXT:         || (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         || (unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             memcmp(
// I686-PC-WINDOWS-MSVC-NEXT:             memcmp(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 (basic.0.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 (extended.0.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 (std::mem::size_of::<[i32; 4]>() as u64) as usize,
// I686-PC-WINDOWS-MSVC-NEXT:                 (basic.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// I686-PC-WINDOWS-MSVC-NEXT:                 (extended.as_mut_ptr() as *const ()) as *const core::ffi::c_void,
// I686-PC-WINDOWS-MSVC-NEXT:                 (std::mem::size_of::<[i32; 4]>() as u32) as usize,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ) as i32
// I686-PC-WINDOWS-MSVC-NEXT:             ) as i32
// X86_64-PC-WINDOWS-MSVC-NEXT:         }) != 0;
// I686-PC-WINDOWS-MSVC-NEXT:         }) != 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn has_feature(leaf: i32, reg: i32, bit: i32) -> i32 {
// I686-PC-WINDOWS-MSVC-NEXT: fn has_feature(leaf: i32, reg: i32, bit: i32) -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut regs: __SlateAlign16<[i32; 4]> = __SlateAlign16([0i32; 4]);
// I686-PC-WINDOWS-MSVC-NEXT:     let mut regs: [i32; 4] = [0i32; 4];
// X86_64-PC-WINDOWS-MSVC-NEXT:     unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         __cpuidex(
// I686-PC-WINDOWS-MSVC-NEXT:         __cpuidex(
// X86_64-PC-WINDOWS-MSVC-NEXT:             (regs.0.as_mut_ptr() as *mut i32) as *mut i32,
// I686-PC-WINDOWS-MSVC-NEXT:             (regs.as_mut_ptr() as *mut i32) as *mut i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             leaf as i32,
// I686-PC-WINDOWS-MSVC-NEXT:             leaf as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:         )
// I686-PC-WINDOWS-MSVC-NEXT:         )
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     regs.0[reg as usize] >> bit & 1
// I686-PC-WINDOWS-MSVC-NEXT:     regs[reg as usize] >> bit & 1
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check_sse() {
// I686-PC-WINDOWS-MSVC-NEXT: fn check_sse() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let a: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let a: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 4i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 4i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 3i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 3i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 2i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 2i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 1i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 1i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let b: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let b: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set1_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 10i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set1_epi32(10i32 as i32))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let sum: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let sum: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_add_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(a) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(b) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_add_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(a) },
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(b) },
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: bool = lane(sum, 0i32) != 11 || lane(sum, 3i32) != 14;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: bool = lane(sum, 0i32) != 11 || lane(sum, 3i32) != 14;
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _mm_cvtsi128_si32(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         _mm_cvtsi128_si32(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                 std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_xor_si128(
// X86_64-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(sum) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(sum) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<__m128i, core::arch::x86::__m128i>(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:                 std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_xor_si128(
// I686-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(sum) },
// I686-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(sum) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 ))
// I686-PC-WINDOWS-MSVC-NEXT:                 ))
// X86_64-PC-WINDOWS-MSVC-NEXT:             })
// I686-PC-WINDOWS-MSVC-NEXT:             })
// X86_64-PC-WINDOWS-MSVC-NEXT:         }) as i32
// I686-PC-WINDOWS-MSVC-NEXT:         }) as i32
// X86_64-PC-WINDOWS-MSVC-NEXT:     }) != 0
// I686-PC-WINDOWS-MSVC-NEXT:     }) != 0
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let rotated: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let rotated: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_shuffle_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(a) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_shuffle_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(a) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 57i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 57i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_1:__v[0-9]+]]: bool = lane(rotated, 0i32) != 2 || lane(rotated, 3i32) != 1;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_1:__v[0-9]+]]: bool = lane(rotated, 0i32) != 2 || lane(rotated, 3i32) != 1;
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_1]] {
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_1]] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let shifted: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let shifted: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_srli_si128(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(a) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_srli_si128(
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(a) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 4i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 4i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_2:__v[0-9]+]]: bool = lane(shifted, 0i32) != 2 || lane(shifted, 3i32) != 0;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_2:__v[0-9]+]]: bool = lane(shifted, 0i32) != 2 || lane(shifted, 3i32) != 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_2]] {
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_2]] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let f: __m128 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let f: __m128 = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128, __m128>(_mm_set_ps(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128, __m128>(_mm_set_ps(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 4.0f32 as f32,
// I686-PC-WINDOWS-MSVC-NEXT:                 4.0f32 as f32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 3.0f32 as f32,
// I686-PC-WINDOWS-MSVC-NEXT:                 3.0f32 as f32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 2.0f32 as f32,
// I686-PC-WINDOWS-MSVC-NEXT:                 2.0f32 as f32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 1.5f32 as f32,
// I686-PC-WINDOWS-MSVC-NEXT:                 1.5f32 as f32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _mm_cvtss_f32(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         _mm_cvtss_f32(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<__m128, core::arch::x86_64::__m128>(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                 std::mem::transmute::<core::arch::x86_64::__m128, __m128>(_mm_mul_ps(
// X86_64-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128, core::arch::x86_64::__m128>(f) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128, core::arch::x86_64::__m128>(f) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<__m128, core::arch::x86::__m128>(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:                 std::mem::transmute::<core::arch::x86::__m128, __m128>(_mm_mul_ps(
// I686-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128, core::arch::x86::__m128>(f) },
// I686-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128, core::arch::x86::__m128>(f) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 ))
// I686-PC-WINDOWS-MSVC-NEXT:                 ))
// X86_64-PC-WINDOWS-MSVC-NEXT:             })
// I686-PC-WINDOWS-MSVC-NEXT:             })
// X86_64-PC-WINDOWS-MSVC-NEXT:         }) as f32
// I686-PC-WINDOWS-MSVC-NEXT:         }) as f32
// X86_64-PC-WINDOWS-MSVC-NEXT:     }) != 2.25f32
// I686-PC-WINDOWS-MSVC-NEXT:     }) != 2.25f32
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let d: __m128d = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let d: __m128d = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128d, __m128d>(_mm_set_pd(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128d, __m128d>(_mm_set_pd(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 2.0f64 as f64,
// I686-PC-WINDOWS-MSVC-NEXT:                 2.0f64 as f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 0.5f64 as f64,
// I686-PC-WINDOWS-MSVC-NEXT:                 0.5f64 as f64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _mm_cvtsd_f64(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         _mm_cvtsd_f64(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<__m128d, core::arch::x86_64::__m128d>(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                 std::mem::transmute::<core::arch::x86_64::__m128d, __m128d>(_mm_add_pd(
// X86_64-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128d, core::arch::x86_64::__m128d>(d) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128d, core::arch::x86_64::__m128d>(d) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<__m128d, core::arch::x86::__m128d>(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:                 std::mem::transmute::<core::arch::x86::__m128d, __m128d>(_mm_add_pd(
// I686-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128d, core::arch::x86::__m128d>(d) },
// I686-PC-WINDOWS-MSVC-NEXT:                     unsafe { std::mem::transmute::<__m128d, core::arch::x86::__m128d>(d) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 ))
// I686-PC-WINDOWS-MSVC-NEXT:                 ))
// X86_64-PC-WINDOWS-MSVC-NEXT:             })
// I686-PC-WINDOWS-MSVC-NEXT:             })
// X86_64-PC-WINDOWS-MSVC-NEXT:         }) as f64
// I686-PC-WINDOWS-MSVC-NEXT:         }) as f64
// X86_64-PC-WINDOWS-MSVC-NEXT:     }) != 1.0f64
// I686-PC-WINDOWS-MSVC-NEXT:     }) != 1.0f64
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check_ssse3_sse41() {
// I686-PC-WINDOWS-MSVC-NEXT: fn check_ssse3_sse41() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: bool = has_feature(1i32, 2i32, 9i32) == 0 || has_feature(1i32, 2i32, 19i32) == 0;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: bool = has_feature(1i32, 2i32, 9i32) == 0 || has_feature(1i32, 2i32, 19i32) == 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         return;
// I686-PC-WINDOWS-MSVC-NEXT:         return;
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let bytes: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let bytes: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set_epi8(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set_epi8(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 15i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 15i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 14i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 14i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 13i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 13i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 12i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 12i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 11i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 11i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 10i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 10i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 9i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 9i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 8i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 8i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 7i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 7i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 6i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 6i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 5i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 5i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 4i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 4i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 3i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 3i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 2i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 2i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 1i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 1i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 0i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 0i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let reverse: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let reverse: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set_epi8(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set_epi8(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 0i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 0i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 1i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 1i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 2i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 2i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 3i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 3i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 4i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 4i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 5i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 5i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 6i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 6i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 7i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 7i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 8i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 8i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 9i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 9i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 10i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 10i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 11i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 11i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 12i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 12i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 13i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 13i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 14i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 14i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 15i8 as core::ffi::c_char,
// I686-PC-WINDOWS-MSVC-NEXT:                 15i8 as core::ffi::c_char,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let swapped: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let swapped: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_shuffle_epi8(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(bytes) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(reverse) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_shuffle_epi8(
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(bytes) },
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(reverse) },
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _mm_extract_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:         _mm_extract_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:             unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(swapped) },
// I686-PC-WINDOWS-MSVC-NEXT:             unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(swapped) },
// X86_64-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:             0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as i32
// I686-PC-WINDOWS-MSVC-NEXT:         ) as i32
// X86_64-PC-WINDOWS-MSVC-NEXT:     }) != 202182159
// I686-PC-WINDOWS-MSVC-NEXT:     }) != 202182159
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _mm_extract_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:         _mm_extract_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:             unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:             unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                 std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                     std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_insert_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                             std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(bytes)
// X86_64-PC-WINDOWS-MSVC-NEXT:                         },
// I686-PC-WINDOWS-MSVC-NEXT:                 std::mem::transmute::<__m128i, core::arch::x86::__m128i>(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:                     std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_insert_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:                         unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(bytes) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                         99i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                         99i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                         2i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                         2i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                     ))
// I686-PC-WINDOWS-MSVC-NEXT:                     ))
// X86_64-PC-WINDOWS-MSVC-NEXT:                 })
// I686-PC-WINDOWS-MSVC-NEXT:                 })
// X86_64-PC-WINDOWS-MSVC-NEXT:             },
// I686-PC-WINDOWS-MSVC-NEXT:             },
// X86_64-PC-WINDOWS-MSVC-NEXT:             2i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:             2i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as i32
// I686-PC-WINDOWS-MSVC-NEXT:         ) as i32
// X86_64-PC-WINDOWS-MSVC-NEXT:     }) != 99
// I686-PC-WINDOWS-MSVC-NEXT:     }) != 99
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check_aes_clmul() {
// I686-PC-WINDOWS-MSVC-NEXT: fn check_aes_clmul() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: bool = has_feature(1i32, 2i32, 25i32) == 0 || has_feature(1i32, 2i32, 1i32) == 0;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: bool = has_feature(1i32, 2i32, 25i32) == 0 || has_feature(1i32, 2i32, 1i32) == 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         return;
// I686-PC-WINDOWS-MSVC-NEXT:         return;
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let state: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let state: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 19088743i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 19088743i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 -1985229329i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 -1985229329i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 253635900i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 253635900i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 1264216440i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 1264216440i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let key: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let key: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set1_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set1_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 1515870810i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 1515870810i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let round: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let round: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_aesenc_si128(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(state) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(key) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_aesenc_si128(
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(state) },
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(key) },
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let back: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let back: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_aesdeclast_si128(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_aesdeclast_si128(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                     std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                         std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(
// I686-PC-WINDOWS-MSVC-NEXT:                     std::mem::transmute::<__m128i, core::arch::x86::__m128i>(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:                         std::mem::transmute::<core::arch::x86::__m128i, __m128i>(
// X86_64-PC-WINDOWS-MSVC-NEXT:                             _mm_aesenclast_si128(
// I686-PC-WINDOWS-MSVC-NEXT:                             _mm_aesenclast_si128(
// X86_64-PC-WINDOWS-MSVC-NEXT:                                 unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:                                 unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                                     std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(
// X86_64-PC-WINDOWS-MSVC-NEXT:                                         state,
// X86_64-PC-WINDOWS-MSVC-NEXT:                                     )
// I686-PC-WINDOWS-MSVC-NEXT:                                     std::mem::transmute::<__m128i, core::arch::x86::__m128i>(state)
// X86_64-PC-WINDOWS-MSVC-NEXT:                                 },
// I686-PC-WINDOWS-MSVC-NEXT:                                 },
// X86_64-PC-WINDOWS-MSVC-NEXT:                                 unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:                                 unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:                                     std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(key)
// I686-PC-WINDOWS-MSVC-NEXT:                                     std::mem::transmute::<__m128i, core::arch::x86::__m128i>(key)
// X86_64-PC-WINDOWS-MSVC-NEXT:                                 },
// I686-PC-WINDOWS-MSVC-NEXT:                                 },
// X86_64-PC-WINDOWS-MSVC-NEXT:                             ),
// I686-PC-WINDOWS-MSVC-NEXT:                             ),
// X86_64-PC-WINDOWS-MSVC-NEXT:                         )
// I686-PC-WINDOWS-MSVC-NEXT:                         )
// X86_64-PC-WINDOWS-MSVC-NEXT:                     })
// I686-PC-WINDOWS-MSVC-NEXT:                     })
// X86_64-PC-WINDOWS-MSVC-NEXT:                 },
// I686-PC-WINDOWS-MSVC-NEXT:                 },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(key) },
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(key) },
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_1]]: bool =
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_1]]: bool =
// X86_64-PC-WINDOWS-MSVC-NEXT:         lane(back, 0i32) == lane(state, 0i32) && lane(round, 0i32) == lane(state, 0i32);
// I686-PC-WINDOWS-MSVC-NEXT:         lane(back, 0i32) == lane(state, 0i32) && lane(round, 0i32) == lane(state, 0i32);
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_1]] {
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_1]] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let x: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let x: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 3i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 3i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let product: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let product: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_clmulepi64_si128(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(x) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(x) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_clmulepi64_si128(
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(x) },
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(x) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 0i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 0i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     if (unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _mm_cvtsi128_si32(unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         _mm_cvtsi128_si32(unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(product)
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<__m128i, core::arch::x86::__m128i>(product)
// X86_64-PC-WINDOWS-MSVC-NEXT:         }) as i32
// I686-PC-WINDOWS-MSVC-NEXT:         }) as i32
// X86_64-PC-WINDOWS-MSVC-NEXT:     }) != 5
// I686-PC-WINDOWS-MSVC-NEXT:     }) != 5
// X86_64-PC-WINDOWS-MSVC-NEXT:     {
// I686-PC-WINDOWS-MSVC-NEXT:     {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check_sha() {
// I686-PC-WINDOWS-MSVC-NEXT: fn check_sha() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     if has_feature(7i32, 1i32, 29i32) == 0 {
// I686-PC-WINDOWS-MSVC-NEXT:     if has_feature(7i32, 1i32, 29i32) == 0 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         return;
// I686-PC-WINDOWS-MSVC-NEXT:         return;
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let a: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let a: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 1i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 1i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 2i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 2i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 3i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 3i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 4i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 4i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let b: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let b: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_set_epi32(
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_set_epi32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 5i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 5i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 6i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 6i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 7i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 7i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:                 8i32 as i32,
// I686-PC-WINDOWS-MSVC-NEXT:                 8i32 as i32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let msg: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let msg: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_sha256msg1_epu32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(a) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(b) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_sha256msg1_epu32(
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(a) },
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(b) },
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let rounds: __m128i = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let rounds: __m128i = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86_64::__m128i, __m128i>(_mm_sha256rnds2_epu32(
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(a) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(b) },
// X86_64-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86_64::__m128i>(msg) },
// I686-PC-WINDOWS-MSVC-NEXT:             std::mem::transmute::<core::arch::x86::__m128i, __m128i>(_mm_sha256rnds2_epu32(
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(a) },
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(b) },
// I686-PC-WINDOWS-MSVC-NEXT:                 unsafe { std::mem::transmute::<__m128i, core::arch::x86::__m128i>(msg) },
// X86_64-PC-WINDOWS-MSVC-NEXT:             ))
// I686-PC-WINDOWS-MSVC-NEXT:             ))
// X86_64-PC-WINDOWS-MSVC-NEXT:         }
// I686-PC-WINDOWS-MSVC-NEXT:         }
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: bool = lane(rounds, 0i32) == 0 && lane(rounds, 1i32) == 0;
// I686-PC-WINDOWS-MSVC-NEXT:     let [[V_0]]: bool = lane(rounds, 0i32) == 0 && lane(rounds, 1i32) == 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// I686-PC-WINDOWS-MSVC-NEXT:     if [[V_0]] {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// X86_64-PC-WINDOWS-MSVC-NEXT: fn check_carry() {
// I686-PC-WINDOWS-MSVC-NEXT: fn check_carry() {
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut low: u32 = 0;
// I686-PC-WINDOWS-MSVC-NEXT:     let mut low: u32 = 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut carry: u8 = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     let mut carry: u8 = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _addcarry_u32(
// I686-PC-WINDOWS-MSVC-NEXT:         _addcarry_u32(
// X86_64-PC-WINDOWS-MSVC-NEXT:             0u8 as u8,
// I686-PC-WINDOWS-MSVC-NEXT:             0u8 as u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             4294967295u32 as u32,
// I686-PC-WINDOWS-MSVC-NEXT:             4294967295u32 as u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             2u32 as u32,
// I686-PC-WINDOWS-MSVC-NEXT:             2u32 as u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(low) as *mut u32,
// I686-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(low) as *mut u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as u8
// I686-PC-WINDOWS-MSVC-NEXT:         ) as u8
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (carry as i32) != 1 || low != 1 {
// I686-PC-WINDOWS-MSVC-NEXT:     if (carry as i32) != 1 || low != 1 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     carry = unsafe {
// I686-PC-WINDOWS-MSVC-NEXT:     carry = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _subborrow_u32(
// I686-PC-WINDOWS-MSVC-NEXT:         _subborrow_u32(
// X86_64-PC-WINDOWS-MSVC-NEXT:             1u8 as u8,
// I686-PC-WINDOWS-MSVC-NEXT:             1u8 as u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0u32 as u32,
// I686-PC-WINDOWS-MSVC-NEXT:             0u32 as u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             0u32 as u32,
// I686-PC-WINDOWS-MSVC-NEXT:             0u32 as u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(low) as *mut u32,
// I686-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(low) as *mut u32,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as u8
// I686-PC-WINDOWS-MSVC-NEXT:         ) as u8
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// I686-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (carry as i32) != 1 || low != 4294967295 {
// I686-PC-WINDOWS-MSVC-NEXT:     if (carry as i32) != 1 || low != 4294967295 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// I686-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// I686-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut high: u64 = 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     let product: u64 = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _umul128(
// X86_64-PC-WINDOWS-MSVC-NEXT:             18446744073709551615u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             3u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(high) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as u64
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if product != 18446744073709551613 || high != 2 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     let mut sum: u64 = 0;
// X86_64-PC-WINDOWS-MSVC-NEXT:     carry = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _addcarry_u64(
// X86_64-PC-WINDOWS-MSVC-NEXT:             1u8 as u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             18446744073709551615u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             1u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(sum) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as u8
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (carry as i32) != 1 || sum != 1 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT:     carry = unsafe {
// X86_64-PC-WINDOWS-MSVC-NEXT:         _subborrow_u64(
// X86_64-PC-WINDOWS-MSVC-NEXT:             0u8 as u8,
// X86_64-PC-WINDOWS-MSVC-NEXT:             1u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             2u64 as u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:             std::ptr::addr_of_mut!(sum) as *mut u64,
// X86_64-PC-WINDOWS-MSVC-NEXT:         ) as u8
// X86_64-PC-WINDOWS-MSVC-NEXT:     };
// X86_64-PC-WINDOWS-MSVC-NEXT:     if (carry as i32) != 1 || sum != 18446744073709551615 {
// X86_64-PC-WINDOWS-MSVC-NEXT:         unsafe { std::process::abort() };
// X86_64-PC-WINDOWS-MSVC-NEXT:     }
// X86_64-PC-WINDOWS-MSVC-NEXT: }
// I686-PC-WINDOWS-MSVC-NEXT: }
// X86_64-PC-WINDOWS-MSVC-EMPTY:
// I686-PC-WINDOWS-MSVC-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-PC-WINDOWS-MSVC-NEXT:     check_cpuid();
// I686-PC-WINDOWS-MSVC-NEXT:     check_cpuid();
// X86_64-PC-WINDOWS-MSVC-NEXT:     check_sse();
// I686-PC-WINDOWS-MSVC-NEXT:     check_sse();
// X86_64-PC-WINDOWS-MSVC-NEXT:     check_ssse3_sse41();
// I686-PC-WINDOWS-MSVC-NEXT:     check_ssse3_sse41();
// X86_64-PC-WINDOWS-MSVC-NEXT:     check_aes_clmul();
// I686-PC-WINDOWS-MSVC-NEXT:     check_aes_clmul();
// X86_64-PC-WINDOWS-MSVC-NEXT:     check_sha();
// I686-PC-WINDOWS-MSVC-NEXT:     check_sha();
// X86_64-PC-WINDOWS-MSVC-NEXT:     check_carry();
// I686-PC-WINDOWS-MSVC-NEXT:     check_carry();
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
