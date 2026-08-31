#include <immintrin.h>
#include <stdio.h>

__attribute__((target("avx512f,avx512vl"))) static void masked_load_probe(
    int *p, unsigned mask, int *out) {
  __m128i w = _mm_set1_epi32(-1);
  __m128i v = _mm_mask_loadu_epi32(w, (__mmask8)mask, p);
  _mm_storeu_si128((__m128i *)out, v);
}

int main(void) {
  int data[4] = {10, 20, 30, 40};
  int out[4];
  masked_load_probe(data, 0x5u, out);
  printf("%d %d %d %d\n", out[0], out[1], out[2], out[3]);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: #[repr(C, packed)]
// LOWERING-NEXT: #[derive(Clone, Copy)]
// LOWERING-NEXT: struct __storeu_si128 {
// LOWERING-NEXT:     __v: [i64; 2],
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set_epi32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> [i64; 2] {
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>({{_v[0-9]+}}) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 2] {
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i64; 2] = _mm_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm_mask_loadu_epi32({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: u8, {{arg[0-9]+}}: *mut core::ffi::c_void) -> [i64; 2] {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut [i32; 4] = {{arg[0-9]+}} as *mut [i32; 4];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = unsafe { std::mem::transmute::<[i64; 2], [i32; 4]>({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: [bitint::BInt<1, 1, 1>; 8] = [bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 0u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 1u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 2u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 3u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 4u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 5u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 6u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 7u8 & 1u8) as u128)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [bitint::BInt<1, 1, 1>; 4] = [{{_v[0-9]+}}[0usize], {{_v[0-9]+}}[1usize], {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[3usize]];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i32; 4] = [if {{_v[0-9]+}}[0usize].to_i128() != 0 { unsafe { *({{_v[0-9]+}} as *const i32).add(0usize) } } else { {{_v[0-9]+}}[0usize] }, if {{_v[0-9]+}}[1usize].to_i128() != 0 { unsafe { *({{_v[0-9]+}} as *const i32).add(1usize) } } else { {{_v[0-9]+}}[1usize] }, if {{_v[0-9]+}}[2usize].to_i128() != 0 { unsafe { *({{_v[0-9]+}} as *const i32).add(2usize) } } else { {{_v[0-9]+}}[2usize] }, if {{_v[0-9]+}}[3usize].to_i128() != 0 { unsafe { *({{_v[0-9]+}} as *const i32).add(3usize) } } else { {{_v[0-9]+}}[3usize] }];
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>({{_v[0-9]+}}) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_storeu_si128({{arg[0-9]+}}: *mut [i64; 2], {{arg[0-9]+}}: [i64; 2]) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut __storeu_si128 = {{arg[0-9]+}} as *mut __storeu_si128;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*{{_v[0-9]+}}).__v = {{arg[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn masked_load_probe({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut i32) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i64; 2] = _mm_set1_epi32({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: u8 = {{arg[0-9]+}} as u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: [i64; 2] = unsafe { _mm_mask_loadu_epi32({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut [i64; 2] = {{arg[0-9]+}} as *mut [i64; 2];
// LOWERING-NEXT:     _mm_storeu_si128({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut data: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     *data = [10, 20, 30, 40];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = data.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = out.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     unsafe { masked_load_probe({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = out[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = out[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = out[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = out[({{_v[0-9]+}} as usize)];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: #[repr(C, packed)]
// REWRITES-NEXT: #[derive(Clone, Copy)]
// REWRITES-NEXT: struct __storeu_si128 {
// REWRITES-NEXT:     __v: [i64; 2],
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set_epi32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> [i64; 2] {
// REWRITES-NEXT: let {{_v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>([{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}]) };
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 2] {
// REWRITES-NEXT: return _mm_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}});
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm_mask_loadu_epi32({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: u8, {{arg[0-9]+}}: *mut core::ffi::c_void) -> [i64; 2] {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut [i32; 4] = {{arg[0-9]+}} as *mut [i32; 4];
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = unsafe { std::mem::transmute::<[i64; 2], [i32; 4]>({{arg[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: [bitint::BInt<1, 1, 1>; 8] = [bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 0u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 1u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 2u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 3u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 4u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 5u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 6u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 7u8 & 1u8) as u128)];
// REWRITES-NEXT: let {{_v[0-9]+}}: [bitint::BInt<1, 1, 1>; 4] = [{{_v[0-9]+}}[0usize], {{_v[0-9]+}}[1usize], {{_v[0-9]+}}[2usize], {{_v[0-9]+}}[3usize]];
// REWRITES-NEXT: let {{_v[0-9]+}}: [i32; 4] = [if {{_v[0-9]+}}[0usize].to_i128() != 0 { unsafe { *({{_v[0-9]+}} as *const i32).add(0usize) } } else { {{_v[0-9]+}}[0usize] }, if {{_v[0-9]+}}[1usize].to_i128() != 0 { unsafe { *({{_v[0-9]+}} as *const i32).add(1usize) } } else { {{_v[0-9]+}}[1usize] }, if {{_v[0-9]+}}[2usize].to_i128() != 0 { unsafe { *({{_v[0-9]+}} as *const i32).add(2usize) } } else { {{_v[0-9]+}}[2usize] }, if {{_v[0-9]+}}[3usize].to_i128() != 0 { unsafe { *({{_v[0-9]+}} as *const i32).add(3usize) } } else { {{_v[0-9]+}}[3usize] }];
// REWRITES-NEXT: let {{_v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>({{_v[0-9]+}}) };
// REWRITES-NEXT: return {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_storeu_si128({{arg[0-9]+}}: *mut [i64; 2], {{arg[0-9]+}}: [i64; 2]) {
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*({{arg[0-9]+}} as *mut __storeu_si128)).__v = {{arg[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn masked_load_probe({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut i32) {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT: let {{_v[0-9]+}}: [i64; 2] = _mm_set1_epi32({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: [i64; 2] = unsafe { _mm_mask_loadu_epi32({{_v[0-9]+}}, {{arg[0-9]+}} as u8, ({{arg[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: _mm_storeu_si128({{arg[0-9]+}} as *mut [i64; 2], {{_v[0-9]+}});
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut data: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: *data = [10, 20, 30, 40];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = data.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: u32 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = out.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: unsafe { masked_load_probe({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i64 = 3;
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, out[({{_v[0-9]+}} as usize)], out[({{_v[0-9]+}} as usize)], out[({{_v[0-9]+}} as usize)], out[({{_v[0-9]+}} as usize)]) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
