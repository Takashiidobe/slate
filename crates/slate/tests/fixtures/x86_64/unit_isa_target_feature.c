/* { dg-options "-mavx2 -mfma" } */
#include <immintrin.h>
#include <stdio.h>

static int gather_sum(const int *table, __m256i index) {
    __m256i v = _mm256_i32gather_epi32(table, index, 4);
    __m128i lo = _mm256_castsi256_si128(v);
    __m128i hi = _mm256_extracti128_si256(v, 1);
    __m128i sum = _mm_add_epi32(lo, hi);
    sum = _mm_hadd_epi32(sum, sum);
    sum = _mm_hadd_epi32(sum, sum);
    return _mm_cvtsi128_si32(sum);
}

static float fused(float a, float b, float c) {
    __m256 r = _mm256_fmadd_ps(_mm256_set1_ps(a), _mm256_set1_ps(b), _mm256_set1_ps(c));
    return _mm256_cvtss_f32(r);
}

static int (*pick(void))(const int *, __m256i) { return gather_sum; }

int main(void) {
    int table[16];
    for (int i = 0; i < 16; i++)
        table[i] = i * i;
    __m256i index = _mm256_setr_epi32(1, 3, 5, 7, 9, 11, 13, 15);
    printf("%d\n", gather_sum(table, index));
    printf("%d\n", pick()(table, _mm256_set1_epi32(2)));
    printf("%.1f\n", fused(1.5f, 2.0f, 0.25f));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-mavx2 -mfma" }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.avx2.gather.d.d.256"]
// CHECK-NEXT:     fn __slate_intrinsic_49f65fe8515ab83a(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: std::simd::Simd<i32, 8>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: *const (),
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: std::simd::Simd<i32, 8>,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: std::simd::Simd<i32, 8>,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: i8,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 8>;
// CHECK-NEXT:     #[link_name = "llvm.x86.ssse3.phadd.d.128"]
// CHECK-NEXT:     fn __slate_intrinsic_db88d798f7ef4ebd(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.fma.v8f32"]
// CHECK-NEXT:     fn __slate_intrinsic_e5b4413adec356b9(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 8>;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave,sse2")]
// CHECK-NEXT: unsafe fn _mm_add_epi32(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<u32, 4>, std::simd::Simd<i64, 2>>(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<u32, 4>>(__a)
// CHECK-NEXT:             }) + unsafe {
// CHECK-NEXT:                 std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<u32, 4>>(__b)
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave,sse2")]
// CHECK-NEXT: unsafe fn _mm_cvtsi128_si32(__a: std::simd::Simd<i64, 2>) -> i32 {
// CHECK-NEXT:     let __b: std::simd::Simd<i32, 4> =
// CHECK-NEXT:         unsafe { std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i32, 4>>(__a) };
// CHECK-NEXT:     __b[0]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn _mm_hadd_epi32(
// CHECK-NEXT:     __a: std::simd::Simd<i64, 2>,
// CHECK-NEXT:     __b: std::simd::Simd<i64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i32, 4>, std::simd::Simd<i64, 2>>(unsafe {
// CHECK-NEXT:             __slate_intrinsic_db88d798f7ef4ebd(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i32, 4>>(__a)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<std::simd::Simd<i64, 2>, std::simd::Simd<i32, 4>>(__b)
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn _mm256_cvtss_f32(__a: std::simd::Simd<f32, 8>) -> f32 {
// CHECK-NEXT:     __a[0]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn _mm256_undefined_si256() -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<f64, 4>, std::simd::Simd<i64, 4>>(
// CHECK-NEXT:             std::simd::Simd::splat(0.0f64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
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
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
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
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i32, 8>, std::simd::Simd<i64, 4>>(unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] =
// CHECK-NEXT:                     std::simd::Simd::from_array([__i7, __i6, __i5, __i4, __i3, __i2, __i1, __i0]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn _mm256_setr_epi32(
// CHECK-NEXT:     __i0: i32,
// CHECK-NEXT:     __i1: i32,
// CHECK-NEXT:     __i2: i32,
// CHECK-NEXT:     __i3: i32,
// CHECK-NEXT:     __i4: i32,
// CHECK-NEXT:     __i5: i32,
// CHECK-NEXT:     __i6: i32,
// CHECK-NEXT:     __i7: i32,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi32(__i7, __i6, __i5, __i4, __i3, __i2, __i1, __i0) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn _mm256_set1_ps(__w: f32) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { _mm256_set_ps(__w, __w, __w, __w, __w, __w, __w, __w) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn _mm256_set1_epi32(__i: i32) -> std::simd::Simd<i64, 4> {
// CHECK-NEXT:     return unsafe { _mm256_set_epi32(__i, __i, __i, __i, __i, __i, __i, __i) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn _mm256_castsi256_si128(mut __a: std::simd::Simd<i64, 4>) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     return std::simd::simd_swizzle!(__a, __a, [0, 1]);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn _mm256_fmadd_ps(
// CHECK-NEXT:     __A: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     __B: std::simd::Simd<f32, 8>,
// CHECK-NEXT:     __C: std::simd::Simd<f32, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 8> {
// CHECK-NEXT:     return unsafe { __slate_intrinsic_e5b4413adec356b9(__A, __B, __C) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.gather_sum")]
// CHECK-NEXT: unsafe fn gather_sum(table: *const i32, index: std::simd::Simd<i64, 4>) -> i32 {
// CHECK-NEXT:     let mut v: std::simd::Simd<i64, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i32, 8>, std::simd::Simd<i64, 4>>(unsafe {
// CHECK-NEXT:             __slate_intrinsic_49f65fe8515ab83a(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<i32, 8>>(
// CHECK-NEXT:                         _mm256_undefined_si256(),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:                 table as *const (),
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<i32, 8>>(index)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<std::simd::Simd<i64, 4>, std::simd::Simd<i32, 8>>(
// CHECK-NEXT:                         _mm256_set1_epi32(-1i32),
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:                 const { (4 as i32) as i8 },
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     let lo: std::simd::Simd<i64, 2> = unsafe { _mm256_castsi256_si128(v) };
// CHECK-NEXT:     let hi: std::simd::Simd<i64, 2> = std::simd::simd_swizzle!(v, [2, 3]);
// CHECK-NEXT:     let mut sum: std::simd::Simd<i64, 2> = unsafe { _mm_add_epi32(lo, hi) };
// CHECK-NEXT:     sum = unsafe { _mm_hadd_epi32(sum, sum) };
// CHECK-NEXT:     sum = unsafe { _mm_hadd_epi32(sum, sum) };
// CHECK-NEXT:     unsafe { _mm_cvtsi128_si32(sum) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn fused(a: f32, b: f32, c: f32) -> f32 {
// CHECK-NEXT:     let r: std::simd::Simd<f32, 8> =
// CHECK-NEXT:         unsafe { _mm256_fmadd_ps(_mm256_set1_ps(a), _mm256_set1_ps(b), _mm256_set1_ps(c)) };
// CHECK-NEXT:     unsafe { _mm256_cvtss_f32(r) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn pick() -> Option<unsafe fn(*const i32, std::simd::Simd<i64, 4>) -> i32> {
// CHECK-NEXT:     Some(gather_sum)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "avx,avx2,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut table: __SlateAlign16<[i32; 16]> = __SlateAlign16([0i32; 16]);
// CHECK-NEXT:     for i in 0i32..16i32 {
// CHECK-NEXT:         table.0[i as usize] = i * i;
// CHECK-NEXT:     }
// CHECK-NEXT:     let index: std::simd::Simd<i64, 4> =
// CHECK-NEXT:         unsafe { _mm256_setr_epi32(1i32, 3i32, 5i32, 7i32, 9i32, 11i32, 13i32, 15i32) };
// CHECK-NEXT:     let __slate_printf_arg0 = unsafe { gather_sum(table.0.as_mut_ptr() as *const i32, index) };
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     let __slate_printf_arg0 =
// CHECK-NEXT:         unsafe { pick().unwrap()(table.0.as_mut_ptr() as *const i32, _mm256_set1_epi32(2i32)) };
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     unsafe { slate_support::printf(c"%.1f\n".as_ptr(), fused(1.5f32, 2.0f32, 0.25f32) as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
