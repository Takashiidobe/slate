// { dg-options "-march=armv8.2-a+sve -msve-vector-bits=512" }
#include <arm_sve.h>
#include <stdio.h>

typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(512)));
typedef svfloat64_t fixed_f64 __attribute__((arm_sve_vector_bits(512)));
typedef svuint8_t fixed_u8 __attribute__((arm_sve_vector_bits(512)));

static fixed_i32 table = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 8, 9, 7, 9, 3};

static svint32_t widen(fixed_i32 v) { return v; }

static fixed_i32 narrow(svint32_t v) { return v; }

int main(void) {
    svint32_t live = svadd_n_s32_x(svptrue_b32(), widen(table), 10);
    fixed_i32 back = narrow(live);
    fixed_f64 halves = svdiv_n_f64_x(svptrue_b64(), svdup_n_f64(7.0), 2.0);
    fixed_u8 bytes = svindex_u8(250, 1);
    long sum = 0;
    for (int i = 0; i < 16; i++) sum += back[i] * (i + 1);
    printf("%ld %g %g %u %u %u\n", sum, halves[0], halves[7], bytes[0], bytes[5], bytes[63]);
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(stdarch_aarch64_sve)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-march=armv8.2-a+sve -msve-vector-bits=512" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut table: core::simd::Simd<i32, 16> = core::simd::Simd::from_array([
// CHECK-NEXT:     3i32, 1i32, 4i32, 1i32, 5i32, 9i32, 2i32, 6i32, 5i32, 3i32, 5i32, 8i32, 9i32, 7i32, 9i32, 3i32,
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn widen(mut v: core::simd::Simd<i32, 16>) -> core::arch::aarch64::svint32_t {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::arch::aarch64::svld1_s32(core::arch::aarch64::svptrue_b32(), v.as_array().as_ptr())
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn narrow(v: core::arch::aarch64::svint32_t) -> core::simd::Simd<i32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         let mut __sve_lanes: [i32; 16] = [0i32; 16];
// CHECK-NEXT:         core::arch::aarch64::svst1_s32(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b32(),
// CHECK-NEXT:             __sve_lanes.as_mut_ptr(),
// CHECK-NEXT:             v,
// CHECK-NEXT:         );
// CHECK-NEXT:         core::simd::Simd::from_array(__sve_lanes)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let live: core::arch::aarch64::svint32_t = unsafe {
// CHECK-NEXT:         core::arch::aarch64::svadd_n_s32_x(core::arch::aarch64::svptrue_b32(), widen(table), 10i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     let back: core::simd::Simd<i32, 16> = unsafe { narrow(live) };
// CHECK-NEXT:     let halves: core::simd::Simd<f64, 8> = unsafe {
// CHECK-NEXT:         let mut __sve_lanes: [f64; 8] = [0 as f64; 8];
// CHECK-NEXT:         core::arch::aarch64::svst1_f64(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b64(),
// CHECK-NEXT:             __sve_lanes.as_mut_ptr(),
// CHECK-NEXT:             core::arch::aarch64::svdiv_n_f64_x(
// CHECK-NEXT:                 core::arch::aarch64::svptrue_b64(),
// CHECK-NEXT:                 core::arch::aarch64::svdup_n_f64(7.0f64),
// CHECK-NEXT:                 2.0f64,
// CHECK-NEXT:             ),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::simd::Simd::from_array(__sve_lanes)
// CHECK-NEXT:     };
// CHECK-NEXT:     let bytes: core::simd::Simd<u8, 64> = unsafe {
// CHECK-NEXT:         let mut __sve_lanes: [u8; 64] = [0u8; 64];
// CHECK-NEXT:         core::arch::aarch64::svst1_u8(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b8(),
// CHECK-NEXT:             __sve_lanes.as_mut_ptr(),
// CHECK-NEXT:             core::arch::aarch64::svindex_u8(250u8, 1u8),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::simd::Simd::from_array(__sve_lanes)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut sum: i64 = 0;
// CHECK-NEXT:     for i in 0i32..16i32 {
// CHECK-NEXT:         sum += (back[i as usize] * (i + 1)) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %g %g %u %u %u\n".as_ptr(),
// CHECK-NEXT:             sum,
// CHECK-NEXT:             halves[0],
// CHECK-NEXT:             halves[7],
// CHECK-NEXT:             bytes[0] as i32,
// CHECK-NEXT:             bytes[5] as i32,
// CHECK-NEXT:             bytes[63] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
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
