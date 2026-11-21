// { dg-options "-march=armv8.2-a+sve" }
#include <arm_sve.h>
#include <stdio.h>

static svint32_t scaled_sum(svbool_t pg, svint32_t a, svint32_t b) {
    return svmul_n_s32_x(pg, svadd_s32_x(pg, a, b), 3);
}

svint32_t rotate_one(svint32_t a, svint32_t b) { return svext_s32(a, b, 1); }

static void bump(svint32_t *slot, svint32_t by) { *slot = svadd_s32_x(svptrue_b32(), *slot, by); }

int main(void) {
    int in[64], out[64];
    for (int i = 0; i < 64; i++) in[i] = i;
    long total = 0;
    for (int i = 0; i < 64; i += (int)svcntw()) {
        svbool_t pg = svwhilelt_b32_s32(i, 64);
        svprfb(pg, in + i, SV_PLDL1KEEP);
        svint32_t v = svld1_s32(pg, in + i);
        svst1_s32(pg, out + i, scaled_sum(pg, v, svdup_n_s32(1)));
    }
    for (int i = 0; i < 64; i++) total += out[i];
    svint32_t acc;
    acc = svindex_s32(0, 2);
    bump(&acc, svdup_n_s32(5));
    svint32_t rotated = rotate_one(acc, svdup_n_s32(-1));
    long first4 = svaddv_s32(svptrue_pat_b32(SV_VL4), rotated);
    int any = svptest_any(svptrue_b32(), svcmpgt_n_s32(svptrue_b32(), acc, 20));
    printf("%ld %ld %d\n", total, first4, any);
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(stdarch_aarch64_sve)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-march=armv8.2-a+sve" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn scaled_sum(
// CHECK-NEXT:     pg: core::arch::aarch64::svbool_t,
// CHECK-NEXT:     a: core::arch::aarch64::svint32_t,
// CHECK-NEXT:     b: core::arch::aarch64::svint32_t,
// CHECK-NEXT: ) -> core::arch::aarch64::svint32_t {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::arch::aarch64::svmul_n_s32_x(
// CHECK-NEXT:             pg,
// CHECK-NEXT:             core::arch::aarch64::svadd_s32_x(pg, a, b),
// CHECK-NEXT:             3i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn rotate_one(
// CHECK-NEXT:     a: core::arch::aarch64::svint32_t,
// CHECK-NEXT:     b: core::arch::aarch64::svint32_t,
// CHECK-NEXT: ) -> core::arch::aarch64::svint32_t {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         core::arch::aarch64::svext_s32::<{ ((1 as i32) as u64) as i32 }>(a, b)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn bump(
// CHECK-NEXT:     slot: &mut core::arch::aarch64::svint32_t,
// CHECK-NEXT:     by: core::arch::aarch64::svint32_t,
// CHECK-NEXT: ) {
// CHECK-NEXT:     *slot = unsafe {
// CHECK-NEXT:         core::arch::aarch64::svadd_s32_x(core::arch::aarch64::svptrue_b32(), *slot, by)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,sve,fcma")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut r#in: [i32; 64] = [0i32; 64];
// CHECK-NEXT:     let mut out: [i32; 64] = [0i32; 64];
// CHECK-NEXT:     for i in 0i32..64i32 {
// CHECK-NEXT:         r#in[i as usize] = i;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut total: i64 = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut i: i32 = 0;
// CHECK-NEXT:         while i < 64 {
// CHECK-NEXT:             let pg: core::arch::aarch64::svbool_t = unsafe {
// CHECK-NEXT:                 core::arch::aarch64::svwhilelt_b32_s32(i, 64i32)
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::arch::aarch64::svprfb::<{ unsafe { core::mem::transmute::<i32, core::arch::aarch64::svprfop>(((0 as i32) as u32) as i32) } }, _>(
// CHECK-NEXT:                     pg,
// CHECK-NEXT:                     (r#in.as_mut_ptr() as *mut i32).offset(i as isize) as *const (),
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let v: core::arch::aarch64::svint32_t = unsafe {
// CHECK-NEXT:                 core::arch::aarch64::svld1_s32(
// CHECK-NEXT:                     pg,
// CHECK-NEXT:                     (r#in.as_mut_ptr() as *mut i32).offset(i as isize) as *const i32,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::arch::aarch64::svst1_s32(
// CHECK-NEXT:                     pg,
// CHECK-NEXT:                     (out.as_mut_ptr() as *mut i32).offset(i as isize),
// CHECK-NEXT:                     scaled_sum(pg, v, core::arch::aarch64::svdup_n_s32(1i32)),
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             let [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:             i = [[V_0]] + ((unsafe { core::arch::aarch64::svcntw() }) as i32);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..64i32 {
// CHECK-NEXT:         total += out[i as usize] as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut acc: core::arch::aarch64::svint32_t = unsafe {
// CHECK-NEXT:         core::arch::aarch64::svdup_n_s32(0i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     acc = unsafe { core::arch::aarch64::svindex_s32(0i32, 2i32) };
// CHECK-NEXT:     {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = unsafe { core::arch::aarch64::svdup_n_s32(5i32) };
// CHECK-NEXT:         unsafe { bump(&mut acc, [[T_0]]) }
// CHECK-NEXT:     };
// CHECK-NEXT:     let rotated: core::arch::aarch64::svint32_t = unsafe {
// CHECK-NEXT:         rotate_one(acc, core::arch::aarch64::svdup_n_s32(-1i32))
// CHECK-NEXT:     };
// CHECK-NEXT:     let first4: i64 = unsafe {
// CHECK-NEXT:         core::arch::aarch64::svaddv_s32(
// CHECK-NEXT:             core::arch::aarch64::svptrue_pat_b32::<{ unsafe { core::mem::transmute::<i32, core::arch::aarch64::svpattern>(((4 as i32) as u32) as i32) } }>(),
// CHECK-NEXT:             rotated,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let any: i32 = (unsafe {
// CHECK-NEXT:         core::arch::aarch64::svptest_any(
// CHECK-NEXT:             core::arch::aarch64::svptrue_b32(),
// CHECK-NEXT:             core::arch::aarch64::svcmpgt_n_s32(
// CHECK-NEXT:                 core::arch::aarch64::svptrue_b32(),
// CHECK-NEXT:                 acc,
// CHECK-NEXT:                 20i32,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%ld %ld %d\n".as_ptr(), total, first4, any) };
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
