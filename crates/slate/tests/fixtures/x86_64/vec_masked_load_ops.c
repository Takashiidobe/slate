#include <immintrin.h>
#include <stdio.h>

__attribute__((target("avx512f,avx512vl"))) static void
masked_load_probe(int *p, unsigned mask, int *out) {
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
// LOWERING-X86_64-GNU: #![feature(c_variadic)]
// LOWERING-X86_64-GNU-NEXT: #![allow(
// LOWERING-X86_64-GNU-NEXT:     dead_code,
// LOWERING-X86_64-GNU-NEXT:     unused,
// LOWERING-X86_64-GNU-NEXT:     non_camel_case_types,
// LOWERING-X86_64-GNU-NEXT:     non_snake_case,
// LOWERING-X86_64-GNU-NEXT:     non_upper_case_globals,
// LOWERING-X86_64-GNU-NEXT:     arithmetic_overflow,
// LOWERING-X86_64-GNU-NEXT:     unconditional_panic,
// LOWERING-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-X86_64-GNU-NEXT:     unused_comparisons
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C, packed)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct __storeu_si128 {
// LOWERING-X86_64-GNU-NEXT:     __v: [i64; 2],
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut data: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     let mut out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [10, 20, 30, 40];
// LOWERING-X86_64-GNU-NEXT:     *data = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = data.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u32 = 5;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i32 = out.as_mut_ptr() as *mut i32;
// LOWERING-X86_64-GNU-NEXT:     unsafe { masked_load_probe({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i64 = 3;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = out[({{__v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(
// LOWERING-X86_64-GNU-NEXT:     enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave"
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-NEXT: unsafe fn masked_load_probe({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut i32) {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32({{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{arg[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] =
// LOWERING-X86_64-GNU-NEXT:         unsafe { _mm_mask_loadu_epi32({{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i64; 2] = {{arg[0-9]+}} as *mut [i64; 2];
// LOWERING-X86_64-GNU-NEXT:     _mm_storeu_si128({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[target_feature(
// LOWERING-X86_64-GNU-NEXT:     enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave"
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-NEXT: unsafe fn _mm_mask_loadu_epi32({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: u8, {{arg[0-9]+}}: *mut core::ffi::c_void) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i32; 4] = {{arg[0-9]+}} as *mut [i32; 4];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 4] = unsafe { std::mem::transmute::<[i64; 2], [i32; 4]>({{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [bitint::BInt<1, 1, 1>; 8] = [
// LOWERING-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 0u8 & 1u8) as u128),
// LOWERING-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 1u8 & 1u8) as u128),
// LOWERING-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 2u8 & 1u8) as u128),
// LOWERING-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 3u8 & 1u8) as u128),
// LOWERING-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 4u8 & 1u8) as u128),
// LOWERING-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 5u8 & 1u8) as u128),
// LOWERING-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 6u8 & 1u8) as u128),
// LOWERING-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 7u8 & 1u8) as u128),
// LOWERING-X86_64-GNU-NEXT:     ];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [bitint::BInt<1, 1, 1>; 4] = [{{__v[0-9]+}}[0usize], {{__v[0-9]+}}[1usize], {{__v[0-9]+}}[2usize], {{__v[0-9]+}}[3usize]];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [
// LOWERING-X86_64-GNU-NEXT:         if {{__v[0-9]+}}[0usize].to_i128() != 0 {
// LOWERING-X86_64-GNU-NEXT:             unsafe { *({{__v[0-9]+}} as *const i32).add(0usize) }
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}}[0usize]
// LOWERING-X86_64-GNU-NEXT:         },
// LOWERING-X86_64-GNU-NEXT:         if {{__v[0-9]+}}[1usize].to_i128() != 0 {
// LOWERING-X86_64-GNU-NEXT:             unsafe { *({{__v[0-9]+}} as *const i32).add(1usize) }
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}}[1usize]
// LOWERING-X86_64-GNU-NEXT:         },
// LOWERING-X86_64-GNU-NEXT:         if {{__v[0-9]+}}[2usize].to_i128() != 0 {
// LOWERING-X86_64-GNU-NEXT:             unsafe { *({{__v[0-9]+}} as *const i32).add(2usize) }
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}}[2usize]
// LOWERING-X86_64-GNU-NEXT:         },
// LOWERING-X86_64-GNU-NEXT:         if {{__v[0-9]+}}[3usize].to_i128() != 0 {
// LOWERING-X86_64-GNU-NEXT:             unsafe { *({{__v[0-9]+}} as *const i32).add(3usize) }
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             {{__v[0-9]+}}[3usize]
// LOWERING-X86_64-GNU-NEXT:         },
// LOWERING-X86_64-GNU-NEXT:     ];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_storeu_si128({{arg[0-9]+}}: *mut [i64; 2], {{arg[0-9]+}}: [i64; 2]) {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut __storeu_si128 = {{arg[0-9]+}} as *mut __storeu_si128;
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).__v) }, {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(always)]
// LOWERING-X86_64-GNU-NEXT: fn _mm_set_epi32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> [i64; 2] {
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}];
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>({{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     return {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU: #![feature(c_variadic)]
// REWRITES-X86_64-GNU-NEXT: #![allow(
// REWRITES-X86_64-GNU-NEXT:     dead_code,
// REWRITES-X86_64-GNU-NEXT:     unused,
// REWRITES-X86_64-GNU-NEXT:     non_camel_case_types,
// REWRITES-X86_64-GNU-NEXT:     non_snake_case,
// REWRITES-X86_64-GNU-NEXT:     non_upper_case_globals,
// REWRITES-X86_64-GNU-NEXT:     arithmetic_overflow,
// REWRITES-X86_64-GNU-NEXT:     unconditional_panic,
// REWRITES-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-X86_64-GNU-NEXT:     unused_comparisons
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C, packed)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct __storeu_si128 {
// REWRITES-X86_64-GNU-NEXT:     __v: [i64; 2],
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut data: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     let mut out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-X86_64-GNU-NEXT:     *data = [10, 20, 30, 40];
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         masked_load_probe(
// REWRITES-X86_64-GNU-NEXT:             data.as_mut_ptr() as *mut i32,
// REWRITES-X86_64-GNU-NEXT:             5,
// REWRITES-X86_64-GNU-NEXT:             out.as_mut_ptr() as *mut i32,
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     println!("{} {} {} {}", out[0], out[1], out[2], out[3]);
// REWRITES-X86_64-GNU-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// REWRITES-X86_64-GNU-NEXT:     std::process::exit(0 as i32);
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(
// REWRITES-X86_64-GNU-NEXT:     enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave"
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-NEXT: unsafe fn masked_load_probe({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: u32, {{arg[0-9]+}}: *mut i32) {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = _mm_set1_epi32(-1);
// REWRITES-X86_64-GNU-NEXT:     _mm_storeu_si128({{arg[0-9]+}} as *mut [i64; 2], unsafe {
// REWRITES-X86_64-GNU-NEXT:         _mm_mask_loadu_epi32({{__v[0-9]+}}, {{arg[0-9]+}} as u8, {{arg[0-9]+}} as *mut core::ffi::c_void)
// REWRITES-X86_64-GNU-NEXT:     });
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_set1_epi32({{arg[0-9]+}}: i32) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     _mm_set_epi32({{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}})
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[target_feature(
// REWRITES-X86_64-GNU-NEXT:     enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave"
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-NEXT: unsafe fn _mm_mask_loadu_epi32({{arg[0-9]+}}: [i64; 2], {{arg[0-9]+}}: u8, {{arg[0-9]+}}: *mut core::ffi::c_void) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut [i32; 4] = {{arg[0-9]+}} as *mut [i32; 4];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 4] = unsafe { std::mem::transmute::<[i64; 2], [i32; 4]>({{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [bitint::BInt<1, 1, 1>; 8] = [
// REWRITES-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 0u8 & 1u8) as u128),
// REWRITES-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 1u8 & 1u8) as u128),
// REWRITES-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 2u8 & 1u8) as u128),
// REWRITES-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 3u8 & 1u8) as u128),
// REWRITES-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 4u8 & 1u8) as u128),
// REWRITES-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 5u8 & 1u8) as u128),
// REWRITES-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 6u8 & 1u8) as u128),
// REWRITES-X86_64-GNU-NEXT:         bitint::BInt::<1, 1, 1>::from_u128(({{arg[0-9]+}} >> 7u8 & 1u8) as u128),
// REWRITES-X86_64-GNU-NEXT:     ];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [bitint::BInt<1, 1, 1>; 4] = [{{__v[0-9]+}}[0usize], {{__v[0-9]+}}[1usize], {{__v[0-9]+}}[2usize], {{__v[0-9]+}}[3usize]];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i32; 4] = [
// REWRITES-X86_64-GNU-NEXT:         if {{__v[0-9]+}}[0usize].to_i128() != 0 {
// REWRITES-X86_64-GNU-NEXT:             unsafe { *({{__v[0-9]+}} as *const i32).add(0usize) }
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[0usize]
// REWRITES-X86_64-GNU-NEXT:         },
// REWRITES-X86_64-GNU-NEXT:         if {{__v[0-9]+}}[1usize].to_i128() != 0 {
// REWRITES-X86_64-GNU-NEXT:             unsafe { *({{__v[0-9]+}} as *const i32).add(1usize) }
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[1usize]
// REWRITES-X86_64-GNU-NEXT:         },
// REWRITES-X86_64-GNU-NEXT:         if {{__v[0-9]+}}[2usize].to_i128() != 0 {
// REWRITES-X86_64-GNU-NEXT:             unsafe { *({{__v[0-9]+}} as *const i32).add(2usize) }
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[2usize]
// REWRITES-X86_64-GNU-NEXT:         },
// REWRITES-X86_64-GNU-NEXT:         if {{__v[0-9]+}}[3usize].to_i128() != 0 {
// REWRITES-X86_64-GNU-NEXT:             unsafe { *({{__v[0-9]+}} as *const i32).add(3usize) }
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             {{__v[0-9]+}}[3usize]
// REWRITES-X86_64-GNU-NEXT:         },
// REWRITES-X86_64-GNU-NEXT:     ];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>({{__v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_storeu_si128({{arg[0-9]+}}: *mut [i64; 2], {{arg[0-9]+}}: [i64; 2]) {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut __storeu_si128 = {{arg[0-9]+}} as *mut __storeu_si128;
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_unaligned(unsafe { std::ptr::addr_of_mut!((*{{__v[0-9]+}}).__v) }, {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(always)]
// REWRITES-X86_64-GNU-NEXT: fn _mm_set_epi32({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> [i64; 2] {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i64; 2] =
// REWRITES-X86_64-GNU-NEXT:         unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>([{{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}, {{arg[0-9]+}}]) };
// REWRITES-X86_64-GNU-NEXT:     {{__v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites
