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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// LOWERING-NEXT: fn _mm_set_epi32(arg9: i32, arg10: i32, arg11: i32, arg12: i32) -> [i64; 2] {
// LOWERING-NEXT:     let mut __i3: i32 = 0;
// LOWERING-NEXT:     let mut __i2: i32 = 0;
// LOWERING-NEXT:     let mut __i1: i32 = 0;
// LOWERING-NEXT:     let mut __i0: i32 = 0;
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     __i3 = arg9;
// LOWERING-NEXT:     __i2 = arg10;
// LOWERING-NEXT:     __i1 = arg11;
// LOWERING-NEXT:     __i0 = arg12;
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
// LOWERING-NEXT: fn _mm_set1_epi32(arg8: i32) -> [i64; 2] {
// LOWERING-NEXT:     let mut __i: i32 = 0;
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     __i = arg8;
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
// LOWERING-NEXT: #[target_feature(enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn _mm_mask_loadu_epi32(arg5: [i64; 2], arg6: u8, arg7: *mut core::ffi::c_void) -> [i64; 2] {
// LOWERING-NEXT:     let mut __W: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut __U: u8 = 0;
// LOWERING-NEXT:     let mut __P: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     *__W = arg5;
// LOWERING-NEXT:     __U = arg6;
// LOWERING-NEXT:     __P = arg7;
// LOWERING-NEXT:     let _v0: *mut core::ffi::c_void = __P;
// LOWERING-NEXT:     let _v1: *mut [i32; 4] = _v0 as *mut [i32; 4];
// LOWERING-NEXT:     let _v2: [i64; 2] = *__W;
// LOWERING-NEXT:     let _v3: [i32; 4] = unsafe { std::mem::transmute::<[i64; 2], [i32; 4]>(_v2) };
// LOWERING-NEXT:     let _v4: u8 = __U;
// LOWERING-NEXT:     let _v5: [bitint::BInt<1, 1, 1>; 8] = [bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 0u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 1u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 2u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 3u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 4u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 5u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 6u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 7u8 & 1u8) as u128)];
// LOWERING-NEXT:     let _v6: [bitint::BInt<1, 1, 1>; 4] = [_v5[0usize], _v5[1usize], _v5[2usize], _v5[3usize]];
// LOWERING-NEXT:     let _v7: [i32; 4] = [if _v6[0usize].to_i128() != 0 { unsafe { *(_v1 as *const i32).add(0usize) } } else { _v3[0usize] }, if _v6[1usize].to_i128() != 0 { unsafe { *(_v1 as *const i32).add(1usize) } } else { _v3[1usize] }, if _v6[2usize].to_i128() != 0 { unsafe { *(_v1 as *const i32).add(2usize) } } else { _v3[2usize] }, if _v6[3usize].to_i128() != 0 { unsafe { *(_v1 as *const i32).add(3usize) } } else { _v3[3usize] }];
// LOWERING-NEXT:     let _v8: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>(_v7) };
// LOWERING-NEXT:     *__retval = _v8;
// LOWERING-NEXT:     let _v9: [i64; 2] = *__retval;
// LOWERING-NEXT:     return _v9;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[inline(always)]
// LOWERING-NEXT: fn _mm_storeu_si128(arg3: *mut [i64; 2], arg4: [i64; 2]) {
// LOWERING-NEXT:     let mut __p: *mut [i64; 2] = std::ptr::null_mut();
// LOWERING-NEXT:     let mut __b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     __p = arg3;
// LOWERING-NEXT:     *__b = arg4;
// LOWERING-NEXT:     let _v0: [i64; 2] = *__b;
// LOWERING-NEXT:     let _v1: *mut [i64; 2] = __p;
// LOWERING-NEXT:     let _v2: *mut __storeu_si128 = _v1 as *mut __storeu_si128;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         (*_v2).__v = _v0;
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: #[target_feature(enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// LOWERING-NEXT: unsafe fn masked_load_probe(arg0: *mut i32, arg1: u32, arg2: *mut i32) {
// LOWERING-NEXT:     let mut p: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut mask: u32 = 0;
// LOWERING-NEXT:     let mut out: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut w: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     let mut v: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// LOWERING-NEXT:     p = arg0;
// LOWERING-NEXT:     mask = arg1;
// LOWERING-NEXT:     out = arg2;
// LOWERING-NEXT:     let _v0: i32 = -1;
// LOWERING-NEXT:     let _v1: [i64; 2] = _mm_set1_epi32(_v0);
// LOWERING-NEXT:     *w = _v1;
// LOWERING-NEXT:     let _v2: [i64; 2] = *w;
// LOWERING-NEXT:     let _v3: u32 = mask;
// LOWERING-NEXT:     let _v4: u8 = _v3 as u8;
// LOWERING-NEXT:     let _v5: *mut i32 = p;
// LOWERING-NEXT:     let _v6: *mut core::ffi::c_void = _v5 as *mut core::ffi::c_void;
// LOWERING-NEXT:     let _v7: [i64; 2] = unsafe { _mm_mask_loadu_epi32(_v2, _v4, _v6 as *mut core::ffi::c_void) };
// LOWERING-NEXT:     *v = _v7;
// LOWERING-NEXT:     let _v8: *mut i32 = out;
// LOWERING-NEXT:     let _v9: *mut [i64; 2] = _v8 as *mut [i64; 2];
// LOWERING-NEXT:     let _v10: [i64; 2] = *v;
// LOWERING-NEXT:     _mm_storeu_si128(_v9, _v10);
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut data: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let mut out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     *data = [10, 20, 30, 40];
// LOWERING-NEXT:     let _v1: *mut i32 = data.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let _v2: u32 = 5;
// LOWERING-NEXT:     let _v3: *mut i32 = out.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     unsafe { masked_load_probe(_v1, _v2, _v3) };
// LOWERING-NEXT:     let _v4: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: i64 = 0;
// LOWERING-NEXT:     let _v6: i32 = out[(_v5 as usize)];
// LOWERING-NEXT:     let _v7: i64 = 1;
// LOWERING-NEXT:     let _v8: i32 = out[(_v7 as usize)];
// LOWERING-NEXT:     let _v9: i64 = 2;
// LOWERING-NEXT:     let _v10: i32 = out[(_v9 as usize)];
// LOWERING-NEXT:     let _v11: i64 = 3;
// LOWERING-NEXT:     let _v12: i32 = out[(_v11 as usize)];
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v4 as *const i8, _v6, _v8, _v10, _v12) };
// LOWERING-NEXT:     let _v14: i32 = 0;
// LOWERING-NEXT:     __retval = _v14;
// LOWERING-NEXT:     let _v15: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v15 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
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
// REWRITES-NEXT: fn _mm_set_epi32(arg9: i32, arg10: i32, arg11: i32, arg12: i32) -> [i64; 2] {
// REWRITES-NEXT: let mut __i3: i32 = arg9;
// REWRITES-NEXT: let mut __i2: i32 = arg10;
// REWRITES-NEXT: let mut __i1: i32 = arg11;
// REWRITES-NEXT: let mut __i0: i32 = arg12;
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v5: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>([__i0, __i1, __i2, __i3]) };
// REWRITES-NEXT: *__retval = _v5;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_set1_epi32(arg8: i32) -> [i64; 2] {
// REWRITES-NEXT: let mut __i: i32 = arg8;
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v4: [i64; 2] = _mm_set_epi32(__i, __i, __i, __i);
// REWRITES-NEXT: *__retval = _v4;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn _mm_mask_loadu_epi32(arg5: [i64; 2], arg6: u8, arg7: *mut core::ffi::c_void) -> [i64; 2] {
// REWRITES-NEXT: let mut __W: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut __U: u8 = 0;
// REWRITES-NEXT: let mut __P: *mut core::ffi::c_void = std::ptr::null_mut();
// REWRITES-NEXT: let mut __retval: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: *__W = arg5;
// REWRITES-NEXT: __U = arg6;
// REWRITES-NEXT: __P = arg7;
// REWRITES-NEXT: let _v1: *mut [i32; 4] = __P as *mut [i32; 4];
// REWRITES-NEXT: let _v3: [i32; 4] = unsafe { std::mem::transmute::<[i64; 2], [i32; 4]>(*__W) };
// REWRITES-NEXT: let _v4: u8 = __U;
// REWRITES-NEXT: let _v5: [bitint::BInt<1, 1, 1>; 8] = [bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 0u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 1u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 2u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 3u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 4u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 5u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 6u8 & 1u8) as u128), bitint::BInt::<1, 1, 1>::from_u128((_v4 >> 7u8 & 1u8) as u128)];
// REWRITES-NEXT: let _v6: [bitint::BInt<1, 1, 1>; 4] = [_v5[0usize], _v5[1usize], _v5[2usize], _v5[3usize]];
// REWRITES-NEXT: let _v7: [i32; 4] = [if _v6[0usize].to_i128() != 0 { unsafe { *(_v1 as *const i32).add(0usize) } } else { _v3[0usize] }, if _v6[1usize].to_i128() != 0 { unsafe { *(_v1 as *const i32).add(1usize) } } else { _v3[1usize] }, if _v6[2usize].to_i128() != 0 { unsafe { *(_v1 as *const i32).add(2usize) } } else { _v3[2usize] }, if _v6[3usize].to_i128() != 0 { unsafe { *(_v1 as *const i32).add(3usize) } } else { _v3[3usize] }];
// REWRITES-NEXT: let _v8: [i64; 2] = unsafe { std::mem::transmute::<[i32; 4], [i64; 2]>(_v7) };
// REWRITES-NEXT: *__retval = _v8;
// REWRITES-NEXT: return *__retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[inline(always)]
// REWRITES-NEXT: fn _mm_storeu_si128(arg3: *mut [i64; 2], arg4: [i64; 2]) {
// REWRITES-NEXT: let mut __p: *mut [i64; 2] = arg3;
// REWRITES-NEXT: let mut __b: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: *__b = arg4;
// REWRITES-NEXT: unsafe {
// REWRITES-NEXT:         (*(__p as *mut __storeu_si128)).__v = *__b;
// REWRITES-NEXT: }
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: #[target_feature(enable = "avx,avx2,avx512f,avx512vl,f16c,fma,popcnt,sse3,sse4.1,sse4.2,ssse3,xsave")]
// REWRITES-NEXT: unsafe fn masked_load_probe(arg0: *mut i32, arg1: u32, arg2: *mut i32) {
// REWRITES-NEXT: let mut p: *mut i32 = arg0;
// REWRITES-NEXT: let mut mask: u32 = arg1;
// REWRITES-NEXT: let mut out: *mut i32 = arg2;
// REWRITES-NEXT: let mut w: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let mut v: aligned::Aligned<aligned::A16, [i64; 2]> = aligned::Aligned([0; 2]);
// REWRITES-NEXT: let _v0: i32 = -1;
// REWRITES-NEXT: let _v1: [i64; 2] = _mm_set1_epi32(_v0);
// REWRITES-NEXT: *w = _v1;
// REWRITES-NEXT: let _v7: [i64; 2] = unsafe { _mm_mask_loadu_epi32(*w, mask as u8, (p as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: *v = _v7;
// REWRITES-NEXT: _mm_storeu_si128(out as *mut [i64; 2], *v);
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut data: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: let mut out: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: *data = [10, 20, 30, 40];
// REWRITES-NEXT: let _v1: *mut i32 = data.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: let _v2: u32 = 5;
// REWRITES-NEXT: let _v3: *mut i32 = out.as_mut_ptr() as *mut i32;
// REWRITES-NEXT: unsafe { masked_load_probe(_v1, _v2, _v3) };
// REWRITES-NEXT: let _v4: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: i64 = 0;
// REWRITES-NEXT: let _v7: i64 = 1;
// REWRITES-NEXT: let _v9: i64 = 2;
// REWRITES-NEXT: let _v11: i64 = 3;
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v4 as *const i8, out[(_v5 as usize)], out[(_v7 as usize)], out[(_v9 as usize)], out[(_v11 as usize)]) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
