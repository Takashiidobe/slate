// { dg-options "-march=armv8.2-a+sve -msve-vector-bits=512" }
#include <arm_sve.h>
#include <stdio.h>

typedef svint32_t fixed_i32 __attribute__((arm_sve_vector_bits(512)));
typedef svfloat64_t fixed_f64 __attribute__((arm_sve_vector_bits(512)));

__asm__(".arch armv8.2-a+sve\n"
        ".text\n"
        ".globl zreg_mul_add\n"
        ".type zreg_mul_add, %function\n"
        "zreg_mul_add:\n"
        "  ptrue p0.s\n"
        "  mul z0.s, p0/m, z0.s, z1.s\n"
        "  mov z2.s, w0\n"
        "  add z0.s, z0.s, z2.s\n"
        "  ret\n"
        ".size zreg_mul_add, .-zreg_mul_add\n"
        ".globl zreg_scale\n"
        ".type zreg_scale, %function\n"
        "zreg_scale:\n"
        "  ptrue p0.d\n"
        "  fmul z0.d, p0/m, z0.d, z1.d\n"
        "  ret\n"
        ".size zreg_scale, .-zreg_scale\n");

fixed_i32 zreg_mul_add(fixed_i32 a, fixed_i32 b, int bias);
fixed_f64 zreg_scale(fixed_f64 a, fixed_f64 b);

int main(void) {
    fixed_i32 a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    fixed_i32 b = {2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5};
    fixed_i32 r = zreg_mul_add(a, b, 100);
    fixed_f64 x = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5};
    fixed_f64 y = zreg_scale(x, x);
    long sum = 0;
    for (int i = 0; i < 16; i++) sum += r[i] * (i + 1);
    printf("%ld %d %d %g %g\n", sum, r[0], r[15], y[0], y[7]);
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
// CHECK-NEXT:     #[link_name = "zreg_mul_add"]
// CHECK-NEXT:     fn __vector_abi_zreg_mul_add(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: core::arch::aarch64::svint32_t,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: core::arch::aarch64::svint32_t,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: i32,
// CHECK-NEXT:     ) -> core::arch::aarch64::svint32_t;
// CHECK-NEXT:     #[link_name = "zreg_scale"]
// CHECK-NEXT:     fn __vector_abi_zreg_scale(
// CHECK-NEXT:         [[ARG_0]]: core::arch::aarch64::svfloat64_t,
// CHECK-NEXT:         [[ARG_1]]: core::arch::aarch64::svfloat64_t,
// CHECK-NEXT:     ) -> core::arch::aarch64::svfloat64_t;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sve")]
// CHECK-NEXT: unsafe fn zreg_mul_add(
// CHECK-NEXT:     [[ARG_0]]: core::simd::Simd<i32, 16>,
// CHECK-NEXT:     [[ARG_1]]: core::simd::Simd<i32, 16>,
// CHECK-NEXT:     [[ARG_2]]: i32,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 16> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         let mut __sve_lanes: [i32; 16] = [0i32; 16];
// CHECK-NEXT:         core::arch::aarch64::svst1_s32(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b32(),
// CHECK-NEXT:             __sve_lanes.as_mut_ptr(),
// CHECK-NEXT:             __vector_abi_zreg_mul_add(
// CHECK-NEXT:                 core::arch::aarch64::svld1_s32(
// CHECK-NEXT:                     core::arch::aarch64::svptrue_b32(),
// CHECK-NEXT:                     [[ARG_0]].as_array().as_ptr(),
// CHECK-NEXT:                 ),
// CHECK-NEXT:                 core::arch::aarch64::svld1_s32(
// CHECK-NEXT:                     core::arch::aarch64::svptrue_b32(),
// CHECK-NEXT:                     [[ARG_1]].as_array().as_ptr(),
// CHECK-NEXT:                 ),
// CHECK-NEXT:                 [[ARG_2]],
// CHECK-NEXT:             ),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::simd::Simd::from_array(__sve_lanes)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "sve")]
// CHECK-NEXT: unsafe fn zreg_scale(
// CHECK-NEXT:     [[ARG_0]]: core::simd::Simd<f64, 8>,
// CHECK-NEXT:     [[ARG_1]]: core::simd::Simd<f64, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 8> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         let mut __sve_lanes: [f64; 8] = [0 as f64; 8];
// CHECK-NEXT:         core::arch::aarch64::svst1_f64(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b64(),
// CHECK-NEXT:             __sve_lanes.as_mut_ptr(),
// CHECK-NEXT:             __vector_abi_zreg_scale(
// CHECK-NEXT:                 core::arch::aarch64::svld1_f64(
// CHECK-NEXT:                     core::arch::aarch64::svptrue_b64(),
// CHECK-NEXT:                     [[ARG_0]].as_array().as_ptr(),
// CHECK-NEXT:                 ),
// CHECK-NEXT:                 core::arch::aarch64::svld1_f64(
// CHECK-NEXT:                     core::arch::aarch64::svptrue_b64(),
// CHECK-NEXT:                     [[ARG_1]].as_array().as_ptr(),
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ),
// CHECK-NEXT:         );
// CHECK-NEXT:         core::simd::Simd::from_array(__sve_lanes)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: core::arch::global_asm!(
// CHECK-NEXT:     ".arch armv8.2-a+sve\n.text\n.globl zreg_mul_add\n.type zreg_mul_add, %function\nzreg_mul_add:\n  ptrue p0.s\n  mul z0.s, p0/m, z0.s, z1.s\n  mov z2.s, w0\n  add z0.s, z0.s, z2.s\n  ret\n.size zreg_mul_add, .-zreg_mul_add\n.globl zreg_scale\n.type zreg_scale, %function\nzreg_scale:\n  ptrue p0.d\n  fmul z0.d, p0/m, z0.d, z1.d\n  ret\n.size zreg_scale, .-zreg_scale\n",
// CHECK-NEXT: );
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: core::simd::Simd<i32, 16> = core::simd::Simd::from_array(
// CHECK-NEXT:         [
// CHECK-NEXT:             1i32, 2i32, 3i32, 4i32, 5i32, 6i32, 7i32, 8i32, 9i32, 10i32, 11i32, 12i32,
// CHECK-NEXT:             13i32, 14i32, 15i32, 16i32,
// CHECK-NEXT:         ],
// CHECK-NEXT:     );
// CHECK-NEXT:     let b: core::simd::Simd<i32, 16> = core::simd::Simd::from_array(
// CHECK-NEXT:         [
// CHECK-NEXT:             2i32, 2i32, 2i32, 2i32, 3i32, 3i32, 3i32, 3i32, 4i32, 4i32, 4i32, 4i32, 5i32,
// CHECK-NEXT:             5i32, 5i32, 5i32,
// CHECK-NEXT:         ],
// CHECK-NEXT:     );
// CHECK-NEXT:     let r: core::simd::Simd<i32, 16> = unsafe { zreg_mul_add(a, b, 100i32) };
// CHECK-NEXT:     let x: core::simd::Simd<f64, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:         [0.5f64, 1.5f64, 2.5f64, 3.5f64, 4.5f64, 5.5f64, 6.5f64, 7.5f64],
// CHECK-NEXT:     );
// CHECK-NEXT:     let y: core::simd::Simd<f64, 8> = unsafe { zreg_scale(x, x) };
// CHECK-NEXT:     let mut sum: i64 = 0;
// CHECK-NEXT:     for i in 0i32..16i32 {
// CHECK-NEXT:         sum += (r[i as usize] * (i + 1)) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %d %d %g %g\n".as_ptr(),
// CHECK-NEXT:             sum,
// CHECK-NEXT:             r[0],
// CHECK-NEXT:             r[15],
// CHECK-NEXT:             y[0],
// CHECK-NEXT:             y[7],
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
