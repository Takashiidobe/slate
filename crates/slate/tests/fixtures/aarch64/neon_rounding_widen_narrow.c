#include <arm_neon.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static float f32_in[3][4] = {
    {0.5f, -0.5f, 1.5f, -2.5f},
    {2.7f, -2.7f, 1e10f, -3e9f},
    {-0.0f, 0x1p-140f, 8388609.0f, 0.0f},
};
static double f64_in[3][2] = {{2.5, -3.5}, {1e300, -1e19}, {-0.4, 4503599627370497.5}};
static int8_t s8_in[8] = {-128, 127, -1, 0, 5, -6, 100, -100};
static uint8_t u8_in[8] = {255, 0, 1, 254, 128, 127, 3, 200};
static int16_t s16_in[8] = {-32768, 32767, -1, 0, 1000, -1000, 7, -7};
static uint16_t u16_in[4] = {65535, 0, 32768, 9};
static int32_t s32_in[4] = {INT32_MIN, INT32_MAX, -1, 70000};
static uint32_t u32_in[4] = {UINT32_MAX, 0, 1, 3000000000u};
static int64_t s64_in[2] = {INT64_MIN, 0x123456789abcdefll};
static uint64_t u64_in[2] = {UINT64_MAX, 0x8000000180000001ull};

static void show_bytes(const char *name, const void *data, size_t size) {
    const unsigned char *bytes = data;
    printf("%s ", name);
    for (size_t i = 0; i < size; i++)
        printf("%02x", bytes[i]);
    printf("\n");
}

#define SHOW(name, value)                                                                          \
    do {                                                                                           \
        __typeof__(value) shown = (value);                                                         \
        show_bytes(name, &shown, sizeof shown);                                                    \
    } while (0)

static void rounding(void) {
    float nan_bits[4];
    uint32_t nan_raw[4] = {0x7fc00000u, 0xffc00001u, 0x7f800000u, 0xff800000u};
    memcpy(nan_bits, nan_raw, sizeof nan_bits);
    float32x4_t special = vld1q_f32(nan_bits);
    for (int i = 0; i < 3; i++) {
        float32x4_t v = vld1q_f32(f32_in[i]);
        float64x2_t d = vld1q_f64(f64_in[i]);
        SHOW("vrndq_f32", vrndq_f32(v));
        SHOW("vrndmq_f32", vrndmq_f32(v));
        SHOW("vrndnq_f32", vrndnq_f32(v));
        SHOW("vrndpq_f32", vrndpq_f32(v));
        SHOW("vrndaq_f32", vrndaq_f32(v));
        SHOW("vrndxq_f32", vrndxq_f32(v));
        SHOW("vrndiq_f32", vrndiq_f32(v));
        SHOW("vrndq_f64", vrndq_f64(d));
        SHOW("vrndmq_f64", vrndmq_f64(d));
        SHOW("vrndnq_f64", vrndnq_f64(d));
        SHOW("vrndpq_f64", vrndpq_f64(d));
        SHOW("vrnd_f32", vrnd_f32(vget_low_f32(v)));
        SHOW("vrndn_f32", vrndn_f32(vget_high_f32(v)));
        SHOW("vcvtnq_s32_f32", vcvtnq_s32_f32(v));
        SHOW("vcvtnq_u32_f32", vcvtnq_u32_f32(v));
        SHOW("vcvtmq_s32_f32", vcvtmq_s32_f32(v));
        SHOW("vcvtpq_u32_f32", vcvtpq_u32_f32(v));
        SHOW("vcvtaq_s32_f32", vcvtaq_s32_f32(v));
        SHOW("vcvtnq_s64_f64", vcvtnq_s64_f64(d));
        SHOW("vcvtnq_u64_f64", vcvtnq_u64_f64(d));
        SHOW("vcvtmq_s64_f64", vcvtmq_s64_f64(d));
        SHOW("vcvtn_s32_f32", vcvtn_s32_f32(vget_low_f32(v)));
    }
    SHOW("vrndnq special", vrndnq_f32(special));
    SHOW("vrndmq special", vrndmq_f32(special));
    SHOW("vcvtnq_s32 special", vcvtnq_s32_f32(special));
    SHOW("vcvtnq_u32 special", vcvtnq_u32_f32(special));
}

static void widen_narrow(void) {
    int8x8_t s8 = vld1_s8(s8_in);
    uint8x8_t u8 = vld1_u8(u8_in);
    int16x8_t s16 = vld1q_s16(s16_in);
    int32x4_t s32 = vld1q_s32(s32_in);
    uint32x4_t u32 = vld1q_u32(u32_in);
    int64x2_t s64 = vld1q_s64(s64_in);
    uint64x2_t u64 = vld1q_u64(u64_in);

    SHOW("vmovl_s8", vmovl_s8(s8));
    SHOW("vmovl_u8", vmovl_u8(u8));
    SHOW("vmovl_s16", vmovl_s16(vget_low_s16(s16)));
    SHOW("vmovl_u16", vmovl_u16(vld1_u16(u16_in)));
    SHOW("vmovl_s32", vmovl_s32(vget_high_s32(s32)));
    SHOW("vmovl_u32", vmovl_u32(vget_low_u32(u32)));

    SHOW("vmovn_s16", vmovn_s16(s16));
    SHOW("vmovn_u16", vmovn_u16(vreinterpretq_u16_s16(s16)));
    SHOW("vmovn_s32", vmovn_s32(s32));
    SHOW("vmovn_u32", vmovn_u32(u32));
    SHOW("vmovn_s64", vmovn_s64(s64));
    SHOW("vmovn_u64", vmovn_u64(u64));
    SHOW("vmovn_high_s32", vmovn_high_s32(vmovn_s32(s32), s32));

    SHOW("vmull_s8", vmull_s8(s8, vrev64_s8(s8)));
    SHOW("vmull_u8", vmull_u8(u8, u8));
    SHOW("vmull_s16", vmull_s16(vget_low_s16(s16), vget_high_s16(s16)));
    SHOW("vmull_u16", vmull_u16(vld1_u16(u16_in), vld1_u16(u16_in)));
    SHOW("vmull_s32", vmull_s32(vget_low_s32(s32), vget_low_s32(s32)));
    SHOW("vmull_u32", vmull_u32(vget_low_u32(u32), vget_high_u32(u32)));
    SHOW("vmull_high_u32", vmull_high_u32(u32, u32));
}

int main(void) {
    rounding();
    widen_narrow();
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! SHOW__6 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < u32, 4 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__11 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < u8, 8 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 8u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__3 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < u64, 2 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__12 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i8, 8 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 8u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__4 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i64, 2 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__8 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < u16, 8 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__9 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < f32, 2 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 8u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__5 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < f64, 2 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < f32, 4 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__13 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < u16, 4 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 8u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i32, 4 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__10 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i32, 2 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 8u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__14 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i16, 4 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 8u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__15 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < u32, 2 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 8u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__7 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i16, 8 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.roundeven.v2f32"]
// CHECK-NEXT:     fn __slate_intrinsic_25133ab7d009a47f(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: std::simd::Simd<f32, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtnu.v4i32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_258a3120d4338b30(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtpu.v4i32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_330034d7a4a031ee(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtms.v2i64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_3e25e3be5a032b61(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<i64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.trunc.v2f32"]
// CHECK-NEXT:     fn __slate_intrinsic_4b30cb6ec79ab131(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 2>;
// CHECK-NEXT:     #[link_name = "llvm.ceil.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_65326a6c7d0874d1(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.trunc.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_7ec0ecee6d31a6a(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.rint.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_81ef8205ae2308e6(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.trunc.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_b46b02da91b92f95(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtnu.v2i64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_b978d8a6f97188eb(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<u64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.floor.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_bc69c08c6b60d4de(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.round.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_bd88e9df30d9ce8a(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtns.v2i32.v2f32"]
// CHECK-NEXT:     fn __slate_intrinsic_c90f3307c5a127a7(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtns.v2i64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_d21e36a48e75c293(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<i64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.roundeven.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_e4addf0185006c85(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.ceil.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_e7a98300c53d7d64(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtas.v4i32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_ed7fa1d9d0bdff3(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.floor.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_eda9f32bf802a624(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.nearbyint.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_fd94c1a67ab20ad2(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.roundeven.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_fd9e6603cbfa5487(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtms.v4i32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_fec435d8839a20ad(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fcvtns.v4i32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_fecb9ed2e4d63917(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcombine_s16(
// CHECK-NEXT:     mut __p0: std::simd::Simd<i16, 4>,
// CHECK-NEXT:     mut __p1: std::simd::Simd<i16, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i16, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 8> = std::simd::simd_swizzle!(
// CHECK-NEXT:         __p0, __p1, [0, 1, 2, 3, 4, 5, 6, 7]
// CHECK-NEXT:     );
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vget_high_u32(mut __p0: std::simd::Simd<u32, 4>) -> std::simd::Simd<u32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 2> = std::simd::simd_swizzle!(__p0, __p0, [2, 3]);
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vget_high_f32(mut __p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 2> = std::simd::simd_swizzle!(__p0, __p0, [2, 3]);
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vget_high_s32(mut __p0: std::simd::Simd<i32, 4>) -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 2> = std::simd::simd_swizzle!(__p0, __p0, [2, 3]);
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vget_high_s16(mut __p0: std::simd::Simd<i16, 8>) -> std::simd::Simd<i16, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 4> = std::simd::simd_swizzle!(
// CHECK-NEXT:         __p0, __p0, [4, 5, 6, 7]
// CHECK-NEXT:     );
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vget_low_u32(mut __p0: std::simd::Simd<u32, 4>) -> std::simd::Simd<u32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 2> = std::simd::simd_swizzle!(__p0, __p0, [0, 1]);
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vget_low_f32(mut __p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 2> = std::simd::simd_swizzle!(__p0, __p0, [0, 1]);
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vget_low_s32(mut __p0: std::simd::Simd<i32, 4>) -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 2> = std::simd::simd_swizzle!(__p0, __p0, [0, 1]);
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vget_low_s16(mut __p0: std::simd::Simd<i16, 8>) -> std::simd::Simd<i16, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 4> = std::simd::simd_swizzle!(
// CHECK-NEXT:         __p0, __p0, [0, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovl_u8(__p0: std::simd::Simd<u8, 8>) -> std::simd::Simd<u16, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u16, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u16, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u16,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<u8, 8>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 8>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovl_u32(__p0: std::simd::Simd<u32, 2>) -> std::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u64,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<u32, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u32, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovl_u16(__p0: std::simd::Simd<u16, 4>) -> std::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u32,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<u16, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u16, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovl_s8(__p0: std::simd::Simd<i8, 8>) -> std::simd::Simd<i16, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                     i16,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     >(__p0)
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovl_s32(__p0: std::simd::Simd<i32, 2>) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                     i64,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i32, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovl_s16(__p0: std::simd::Simd<i16, 4>) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                     i32,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<i16, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i16, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovn_u32(__p0: std::simd::Simd<u32, 4>) -> std::simd::Simd<u16, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u16, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<u16, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u16, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u16,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<u32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovn_u64(__p0: std::simd::Simd<u64, 2>) -> std::simd::Simd<u32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<u32, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u32, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u32,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<u64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovn_u16(__p0: std::simd::Simd<u16, 8>) -> std::simd::Simd<u8, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u8, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<u8, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u8,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<u16, 8>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u16, 8>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovn_s32(__p0: std::simd::Simd<i32, 4>) -> std::simd::Simd<i16, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<i16, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i16, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                     i16,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovn_s64(__p0: std::simd::Simd<i64, 2>) -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<i32, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                     i32,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovn_s16(__p0: std::simd::Simd<i16, 8>) -> std::simd::Simd<i8, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i8, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                 i8,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i16, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i16, 8>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p0)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmull_u8(
// CHECK-NEXT:     __p0: std::simd::Simd<u8, 8>,
// CHECK-NEXT:     __p1: std::simd::Simd<u8, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<u16, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u16, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u16, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u16,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<u8, 8>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 8>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:                     * std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                         u16,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<u8, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 8>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmull_u32(
// CHECK-NEXT:     __p0: std::simd::Simd<u32, 2>,
// CHECK-NEXT:     __p1: std::simd::Simd<u32, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u64,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<u32, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u32, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:                     * std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                         u64,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<u32, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u32, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmull_u16(
// CHECK-NEXT:     __p0: std::simd::Simd<u16, 4>,
// CHECK-NEXT:     __p1: std::simd::Simd<u16, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                     u32,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<u16, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u16, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:                     * std::simd::num::SimdUint::cast::<
// CHECK-NEXT:                         u32,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<u16, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u16, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmull_s8(
// CHECK-NEXT:     __p0: std::simd::Simd<i8, 8>,
// CHECK-NEXT:     __p1: std::simd::Simd<i8, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i16, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                     i16,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     >(__p0)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     * std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                         i16,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p1)
// CHECK-NEXT:                     }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmull_s32(
// CHECK-NEXT:     __p0: std::simd::Simd<i32, 2>,
// CHECK-NEXT:     __p1: std::simd::Simd<i32, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                     i64,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<i32, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i32, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:                     * std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                         i64,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<i32, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmull_s16(
// CHECK-NEXT:     __p0: std::simd::Simd<i16, 4>,
// CHECK-NEXT:     __p1: std::simd::Simd<i16, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                     i32,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<i16, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i16, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:                     * std::simd::num::SimdInt::cast::<
// CHECK-NEXT:                         i32,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<i16, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i16, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }),
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrev64_s8(mut __p0: std::simd::Simd<i8, 8>) -> std::simd::Simd<i8, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i8, 8> = std::simd::simd_swizzle!(
// CHECK-NEXT:         __p0, __p0, [7, 6, 5, 4, 3, 2, 1, 0]
// CHECK-NEXT:     );
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndq_f64(__p0: std::simd::Simd<f64, 2>) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_7ec0ecee6d31a6a(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndmq_f64(__p0: std::simd::Simd<f64, 2>) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_bc69c08c6b60d4de(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndnq_f64(__p0: std::simd::Simd<f64, 2>) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_fd9e6603cbfa5487(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndpq_f64(__p0: std::simd::Simd<f64, 2>) -> std::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_e7a98300c53d7d64(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtaq_s32_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ed7fa1d9d0bdff3(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtmq_s32_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_fec435d8839a20ad(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtnq_s32_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_fecb9ed2e4d63917(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtn_s32_f32(__p0: std::simd::Simd<f32, 2>) -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<i32, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_c90f3307c5a127a7(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<f32, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtnq_u32_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_258a3120d4338b30(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtpq_u32_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_330034d7a4a031ee(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndq_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_b46b02da91b92f95(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrnd_f32(__p0: std::simd::Simd<f32, 2>) -> std::simd::Simd<f32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<f32, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_4b30cb6ec79ab131(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<f32, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndaq_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_bd88e9df30d9ce8a(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndiq_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_fd94c1a67ab20ad2(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndmq_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_eda9f32bf802a624(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndnq_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_e4addf0185006c85(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndn_f32(__p0: std::simd::Simd<f32, 2>) -> std::simd::Simd<f32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<f32, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_25133ab7d009a47f(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<f32, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndpq_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_65326a6c7d0874d1(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vrndxq_f32(__p0: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_81ef8205ae2308e6(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtmq_s64_f64(__p0: std::simd::Simd<f64, 2>) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_3e25e3be5a032b61(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtnq_s64_f64(__p0: std::simd::Simd<f64, 2>) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_d21e36a48e75c293(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcvtnq_u64_f64(__p0: std::simd::Simd<f64, 2>) -> std::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_b978d8a6f97188eb(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f64, 2>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmovn_high_s32(
// CHECK-NEXT:     __p0: std::simd::Simd<i16, 4>,
// CHECK-NEXT:     __p1: std::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i16, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 8> = unsafe { vcombine_s16(__p0, vmovn_s32(__p1)) };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmull_high_u32(
// CHECK-NEXT:     __p0: std::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: std::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         vmull_u32(vget_high_u32(__p0), vget_high_u32(__p1))
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vreinterpretq_u16_s16(
// CHECK-NEXT:     __p0: std::simd::Simd<i16, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<u16, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u16, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i16, 8>, std::simd::Simd<u16, 8>>(__p0)
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: static mut f32_in: {{\[\[}}f32; 4]; 3] = [
// CHECK-NEXT:     [0.5f32, -0.5f32, 1.5f32, -2.5f32],
// CHECK-NEXT:     [2.7f32, -2.7f32, 10000000000.0f32, -3000000000.0f32],
// CHECK-NEXT:     [-0.0f32, 7.17e-43f32, 8388609.0f32, 0.0f32],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut f64_in: {{\[\[}}f64; 2]; 3] = [
// CHECK-NEXT:     [2.5f64, -3.5f64],
// CHECK-NEXT:     [1e300f64, -1e19f64],
// CHECK-NEXT:     [-0.4f64, 4503599627370498.0f64],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut s8_in: [i8; 8] = [-128, 127, -1, 0, 5, -6, 100, -100];
// CHECK-NEXT: static mut u8_in: [u8; 8] = [255, 0, 1, 254, 128, 127, 3, 200];
// CHECK-NEXT: static mut s16_in: [i16; 8] = [-32768, 32767, -1, 0, 1000, -1000, 7, -7];
// CHECK-NEXT: static mut u16_in: [u16; 4] = [65535, 0, 32768, 9];
// CHECK-NEXT: static mut s32_in: [i32; 4] = [-2147483648, 2147483647, -1, 70000];
// CHECK-NEXT: static mut u32_in: [u32; 4] = [4294967295, 0, 1, 3000000000];
// CHECK-NEXT: static mut s64_in: [i64; 2] = [-9223372036854775808i64, 81985529216486895];
// CHECK-NEXT: static mut u64_in: [u64; 2] = [18446744073709551615u64, 9223372043297226753u64];
// CHECK-NEXT: fn show_bytes(name: *const u8, data: *const (), size: u64) {
// CHECK-NEXT:     let bytes: *const u8 = data as *const u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s ".as_ptr(), name) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0..size {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(c"%02x".as_ptr(), *bytes.add(i as usize) as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-NEXT: fn rounding() {
// CHECK-NEXT:     let mut nan_bits: [f32; 4] = [0 as f32; 4];
// CHECK-NEXT:     let mut nan_raw: [u32; 4] = [2143289344, 4290772993, 2139095040, 4286578688];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((nan_raw.as_mut_ptr() as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((nan_bits.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((std::mem::size_of::<[f32; 4]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: std::simd::Simd<f32, 4>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (nan_bits.as_mut_ptr() as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let special: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:     for i in 0i32..3i32 {
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: std::simd::Simd<f32, 4>;
// CHECK-NEXT:         {
// CHECK-NEXT:             let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(f32_in[i as usize]) as *mut f32)
// CHECK-NEXT:                             as *const ()) as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                             .read_unaligned()
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             };
// CHECK-NEXT:             [[V_1]] = __ret;
// CHECK-NEXT:         }
// CHECK-NEXT:         let v: std::simd::Simd<f32, 4> = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: std::simd::Simd<f64, 2>;
// CHECK-NEXT:         let __ret: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<f64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(f64_in[i as usize]) as *mut f64)
// CHECK-NEXT:                         as *const ()) as *mut std::simd::Simd<f64, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:         let d: std::simd::Simd<f64, 2> = [[V_2]];
// CHECK-NEXT:         SHOW!(c"vrndq_f32".as_ptr() as * mut u8, unsafe { vrndq_f32(v) });
// CHECK-NEXT:         SHOW!(c"vrndmq_f32".as_ptr() as * mut u8, unsafe { vrndmq_f32(v) });
// CHECK-NEXT:         SHOW!(c"vrndnq_f32".as_ptr() as * mut u8, unsafe { vrndnq_f32(v) });
// CHECK-NEXT:         SHOW!(c"vrndpq_f32".as_ptr() as * mut u8, unsafe { vrndpq_f32(v) });
// CHECK-NEXT:         SHOW!(c"vrndaq_f32".as_ptr() as * mut u8, unsafe { vrndaq_f32(v) });
// CHECK-NEXT:         SHOW!(c"vrndxq_f32".as_ptr() as * mut u8, unsafe { vrndxq_f32(v) });
// CHECK-NEXT:         SHOW!(c"vrndiq_f32".as_ptr() as * mut u8, unsafe { vrndiq_f32(v) });
// CHECK-NEXT:         SHOW__5!(c"vrndq_f64".as_ptr() as * mut u8, unsafe { vrndq_f64(d) });
// CHECK-NEXT:         SHOW__5!(c"vrndmq_f64".as_ptr() as * mut u8, unsafe { vrndmq_f64(d) });
// CHECK-NEXT:         SHOW__5!(c"vrndnq_f64".as_ptr() as * mut u8, unsafe { vrndnq_f64(d) });
// CHECK-NEXT:         SHOW__5!(c"vrndpq_f64".as_ptr() as * mut u8, unsafe { vrndpq_f64(d) });
// CHECK-NEXT:         SHOW__9!(c"vrnd_f32".as_ptr() as * mut u8, unsafe { vrnd_f32(vget_low_f32(v)) });
// CHECK-NEXT:         SHOW__9!(
// CHECK-NEXT:             c"vrndn_f32".as_ptr() as * mut u8, unsafe { vrndn_f32(vget_high_f32(v)) }
// CHECK-NEXT:         );
// CHECK-NEXT:         SHOW__2!(c"vcvtnq_s32_f32".as_ptr() as * mut u8, unsafe { vcvtnq_s32_f32(v) });
// CHECK-NEXT:         SHOW__6!(c"vcvtnq_u32_f32".as_ptr() as * mut u8, unsafe { vcvtnq_u32_f32(v) });
// CHECK-NEXT:         SHOW__2!(c"vcvtmq_s32_f32".as_ptr() as * mut u8, unsafe { vcvtmq_s32_f32(v) });
// CHECK-NEXT:         SHOW__6!(c"vcvtpq_u32_f32".as_ptr() as * mut u8, unsafe { vcvtpq_u32_f32(v) });
// CHECK-NEXT:         SHOW__2!(c"vcvtaq_s32_f32".as_ptr() as * mut u8, unsafe { vcvtaq_s32_f32(v) });
// CHECK-NEXT:         SHOW__4!(c"vcvtnq_s64_f64".as_ptr() as * mut u8, unsafe { vcvtnq_s64_f64(d) });
// CHECK-NEXT:         SHOW__3!(c"vcvtnq_u64_f64".as_ptr() as * mut u8, unsafe { vcvtnq_u64_f64(d) });
// CHECK-NEXT:         SHOW__4!(c"vcvtmq_s64_f64".as_ptr() as * mut u8, unsafe { vcvtmq_s64_f64(d) });
// CHECK-NEXT:         SHOW__10!(
// CHECK-NEXT:             c"vcvtn_s32_f32".as_ptr() as * mut u8, unsafe {
// CHECK-NEXT:             vcvtn_s32_f32(vget_low_f32(v)) }
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW!(c"vrndnq special".as_ptr() as * mut u8, unsafe { vrndnq_f32(special) });
// CHECK-NEXT:     SHOW!(c"vrndmq special".as_ptr() as * mut u8, unsafe { vrndmq_f32(special) });
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"vcvtnq_s32 special".as_ptr() as * mut u8, unsafe { vcvtnq_s32_f32(special) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__6!(
// CHECK-NEXT:         c"vcvtnq_u32 special".as_ptr() as * mut u8, unsafe { vcvtnq_u32_f32(special) }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn widen_narrow() {
// CHECK-NEXT:     let mut [[V_0]]: std::simd::Simd<i8, 8>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<i8, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 (((std::ptr::addr_of_mut!(s8_in) as *mut i8) as *const ())
// CHECK-NEXT:                     as *mut std::simd::Simd<i8, 8>)
// CHECK-NEXT:                     .read_unaligned()
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let s8: std::simd::Simd<i8, 8> = [[V_0]];
// CHECK-NEXT:     let mut [[V_1]]: std::simd::Simd<u8, 8>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<u8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_in) as *mut u8) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let u8: std::simd::Simd<u8, 8> = [[V_1]];
// CHECK-NEXT:     let mut [[V_2]]: std::simd::Simd<i16, 8>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i16, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s16_in) as *mut i16) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let s16: std::simd::Simd<i16, 8> = [[V_2]];
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: std::simd::Simd<i32, 4>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s32_in) as *mut i32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_3]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let s32: std::simd::Simd<i32, 4> = [[V_3]];
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: std::simd::Simd<u32, 4>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u32_in) as *mut u32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_4]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let u32: std::simd::Simd<u32, 4> = [[V_4]];
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: std::simd::Simd<i64, 2>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s64_in) as *mut i64) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i64, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_5]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let s64: std::simd::Simd<i64, 2> = [[V_5]];
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: std::simd::Simd<u64, 2>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u64_in) as *mut u64) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u64, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_6]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let u64: std::simd::Simd<u64, 2> = [[V_6]];
// CHECK-NEXT:     SHOW__7!(c"vmovl_s8".as_ptr() as * mut u8, unsafe { vmovl_s8(s8) });
// CHECK-NEXT:     SHOW__8!(c"vmovl_u8".as_ptr() as * mut u8, unsafe { vmovl_u8(u8) });
// CHECK-NEXT:     SHOW__2!(c"vmovl_s16".as_ptr() as * mut u8, unsafe { vmovl_s16(vget_low_s16(s16)) });
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: std::simd::Simd<u16, 4>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u16, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<u16, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u16, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u16_in) as *mut u16) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u16, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_7]] = __ret;
// CHECK-NEXT:         shown = unsafe { vmovl_u16([[V_7]]) };
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vmovl_u16".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__4!(
// CHECK-NEXT:         c"vmovl_s32".as_ptr() as * mut u8, unsafe { vmovl_s32(vget_high_s32(s32)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(c"vmovl_u32".as_ptr() as * mut u8, unsafe { vmovl_u32(vget_low_u32(u32)) });
// CHECK-NEXT:     SHOW__12!(c"vmovn_s16".as_ptr() as * mut u8, unsafe { vmovn_s16(s16) });
// CHECK-NEXT:     SHOW__11!(
// CHECK-NEXT:         c"vmovn_u16".as_ptr() as * mut u8, unsafe { vmovn_u16(vreinterpretq_u16_s16(s16))
// CHECK-NEXT:         }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__14!(c"vmovn_s32".as_ptr() as * mut u8, unsafe { vmovn_s32(s32) });
// CHECK-NEXT:     SHOW__13!(c"vmovn_u32".as_ptr() as * mut u8, unsafe { vmovn_u32(u32) });
// CHECK-NEXT:     SHOW__10!(c"vmovn_s64".as_ptr() as * mut u8, unsafe { vmovn_s64(s64) });
// CHECK-NEXT:     SHOW__15!(c"vmovn_u64".as_ptr() as * mut u8, unsafe { vmovn_u64(u64) });
// CHECK-NEXT:     SHOW__7!(
// CHECK-NEXT:         c"vmovn_high_s32".as_ptr() as * mut u8, unsafe { vmovn_high_s32(vmovn_s32(s32),
// CHECK-NEXT:         s32) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__7!(c"vmull_s8".as_ptr() as * mut u8, unsafe { vmull_s8(s8, vrev64_s8(s8)) });
// CHECK-NEXT:     SHOW__8!(c"vmull_u8".as_ptr() as * mut u8, unsafe { vmull_u8(u8, u8) });
// CHECK-NEXT:     SHOW__2!(
// CHECK-NEXT:         c"vmull_s16".as_ptr() as * mut u8, unsafe { vmull_s16(vget_low_s16(s16),
// CHECK-NEXT:         vget_high_s16(s16)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: std::simd::Simd<u16, 4>;
// CHECK-NEXT:         {
// CHECK-NEXT:             let __ret: std::simd::Simd<u16, 4> = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     std::simd::Simd<u16, 4>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<u16, 4>,
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(u16_in) as *mut u16) as *const ())
// CHECK-NEXT:                             as *mut std::simd::Simd<u16, 4>)
// CHECK-NEXT:                             .read_unaligned()
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             };
// CHECK-NEXT:             [[V_8]] = __ret;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: std::simd::Simd<u16, 4>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u16, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<u16, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u16, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u16_in) as *mut u16) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u16, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_9]] = __ret;
// CHECK-NEXT:         shown = unsafe { vmull_u16([[V_8]], [[V_9]]) };
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vmull_u16".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     SHOW__4!(
// CHECK-NEXT:         c"vmull_s32".as_ptr() as * mut u8, unsafe { vmull_s32(vget_low_s32(s32),
// CHECK-NEXT:         vget_low_s32(s32)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"vmull_u32".as_ptr() as * mut u8, unsafe { vmull_u32(vget_low_u32(u32),
// CHECK-NEXT:         vget_high_u32(u32)) }
// CHECK-NEXT:     );
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"vmull_high_u32".as_ptr() as * mut u8, unsafe { vmull_high_u32(u32, u32) }
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     rounding();
// CHECK-NEXT:     widen_narrow();
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
