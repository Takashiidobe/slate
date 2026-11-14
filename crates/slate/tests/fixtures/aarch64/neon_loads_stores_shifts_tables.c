#include <arm_neon.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static float f32_buf[13] = {1.5f, -0.0f, 3.25f, -7.0f, 0.0f, -2.5f, 1e30f,
                            -1e-40f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f};
static double f64_buf[5] = {-0.0, 2.0, 0.0, -3.5, 4.0};
static uint8_t u8_buf[48];
static int16_t s16_buf[17];
static int32_t s32_buf[9] = {INT32_MIN, INT32_MAX, -1, 7, -5, 3, 2, 1, 0};
static uint64_t u64_buf[3] = {UINT64_MAX, 5, 0x8000000000000000ull};
static int8_t shift_counts[16] = {0, 1, -1, 7, -7, 8, -8, 9, -9, 64, -64, 127, -128, 3, -3, 2};
static int32_t shift32_counts[4] = {-31, 33, -32, 31};
static int16_t shift16_counts[8] = {-16, 16, -15, 15, 0, -1, 17, -17};

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

static int calls;

static const float *next_f32(const float *p) {
    calls++;
    return p;
}

static void loads(void) {
    SHOW("vld1q_f32+1", vld1q_f32(f32_buf + 1));
    SHOW("vld1q_f64+1", vld1q_f64(f64_buf + 1));
    SHOW("vld1q_u8+3", vld1q_u8(u8_buf + 3));
    SHOW("vld1q_s16+1", vld1q_s16(s16_buf + 1));
    SHOW("vld1q_u64+1", vld1q_u64(u64_buf + 1));
    SHOW("vld1_s32+1", vld1_s32(s32_buf + 1));
    SHOW("vld1_u8+5", vld1_u8(u8_buf + 5));
    SHOW("vld1_s16+3", vld1_s16(s16_buf + 3));

    float32x4_t base = vld1q_f32(f32_buf + 8);
    SHOW("vld1q_lane_f32", vld1q_lane_f32(f32_buf + 7, base, 2));
    SHOW("vld1q_lane_s16", vld1q_lane_s16(s16_buf + 5, vld1q_s16(s16_buf), 7));
    SHOW("vld1_lane_s32", vld1_lane_s32(s32_buf, vld1_s32(s32_buf + 2), 1));
    SHOW("vld1q_lane_u64", vld1q_lane_u64(u64_buf + 2, vld1q_u64(u64_buf), 0));

    float32x4x2_t pair = vld2q_f32(next_f32(f32_buf + 1));
    SHOW("vld2q_f32.0", pair.val[0]);
    SHOW("vld2q_f32.1", pair.val[1]);
    uint8x16x2_t bytes = vld2q_u8(u8_buf + 1);
    SHOW("vld2q_u8.0", bytes.val[0]);
    SHOW("vld2q_u8.1", bytes.val[1]);
    int16x8x2_t words = vld2q_s16(s16_buf + 1);
    SHOW("vld2q_s16.0", words.val[0]);
    SHOW("vld2q_s16.1", words.val[1]);
    printf("calls %d\n", calls);
}

static void stores(void) {
    uint8_t out[80];
    memset(out, 0xee, sizeof out);
    vst1q_f32((float *)(out + 4), vld1q_f32(f32_buf));
    vst1q_u8(out + 21, vld1q_u8(u8_buf + 7));
    vst1_s16((int16_t *)(out + 38), vld1_s16(s16_buf + 2));
    vst1q_lane_s32((int32_t *)(out + 48), vld1q_s32(s32_buf), 3);
    vst1q_lane_f64((double *)(out + 56), vld1q_f64(f64_buf + 2), 1);
    vst1_lane_u8(out + 47, vld1_u8(u8_buf + 9), 6);
    vst1q_lane_u64((uint64_t *)(out + 64), vld1q_u64(u64_buf + 1), 1);
    show_bytes("stores", out, sizeof out);

    float f32_out[9];
    memset(f32_out, 0, sizeof f32_out);
    float32x4x2_t pair = {{vld1q_f32(f32_buf), vld1q_f32(f32_buf + 4)}};
    vst2q_f32(f32_out + 1, pair);
    show_bytes("vst2q_f32", f32_out, sizeof f32_out);

    uint8_t u8_out[33];
    memset(u8_out, 0, sizeof u8_out);
    uint8x16x2_t bytes = {{vld1q_u8(u8_buf), vld1q_u8(u8_buf + 16)}};
    vst2q_u8(u8_out + 1, bytes);
    show_bytes("vst2q_u8", u8_out, sizeof u8_out);

    int16_t s16_out[17];
    memset(s16_out, 0, sizeof s16_out);
    int16x8x2_t words = {{vld1q_s16(s16_buf), vld1q_s16(s16_buf + 8)}};
    vst2q_s16(s16_out + 1, words);
    show_bytes("vst2q_s16", s16_out, sizeof s16_out);
}

static void shifts(void) {
    int32x4_t s32 = vld1q_s32(s32_buf);
    uint32x4_t u32 = vreinterpretq_u32_s32(s32);
    int8x16_t s8 = vld1q_s8((const int8_t *)u8_buf + 20);
    uint8x16_t u8 = vld1q_u8(u8_buf + 20);
    int16x8_t s16 = vld1q_s16(s16_buf + 1);
    uint64x2_t u64 = vld1q_u64(u64_buf);

    SHOW("vshrq_n_s32 1", vshrq_n_s32(s32, 1));
    SHOW("vshrq_n_s32 31", vshrq_n_s32(s32, 31));
    SHOW("vshrq_n_s32 32", vshrq_n_s32(s32, 32));
    SHOW("vshrq_n_u32 32", vshrq_n_u32(u32, 32));
    SHOW("vshrq_n_u32 5", vshrq_n_u32(u32, 5));
    SHOW("vshrq_n_s8 8", vshrq_n_s8(s8, 8));
    SHOW("vshrq_n_u8 3", vshrq_n_u8(u8, 3));
    SHOW("vshrq_n_s16 9", vshrq_n_s16(s16, 9));
    SHOW("vshrq_n_u64 64", vshrq_n_u64(u64, 64));
    SHOW("vshrq_n_s64 63", vshrq_n_s64(vreinterpretq_s64_u64(u64), 63));
    SHOW("vshr_n_u16 16", vshr_n_u16(vld1_u16((const uint16_t *)s16_buf), 16));

    SHOW("vshlq_n_s32 0", vshlq_n_s32(s32, 0));
    SHOW("vshlq_n_s32 31", vshlq_n_s32(s32, 31));
    SHOW("vshlq_n_u8 7", vshlq_n_u8(u8, 7));
    SHOW("vshlq_n_s16 4", vshlq_n_s16(s16, 4));
    SHOW("vshlq_n_u64 63", vshlq_n_u64(u64, 63));
    SHOW("vshl_n_s32 3", vshl_n_s32(vld1_s32(s32_buf), 3));

    int8x16_t counts = vld1q_s8(shift_counts);
    SHOW("vshlq_s8", vshlq_s8(s8, counts));
    SHOW("vshlq_u8", vshlq_u8(u8, counts));
    int32x4_t counts32 = vld1q_s32(shift32_counts);
    SHOW("vshlq_s32", vshlq_s32(s32, counts32));
    SHOW("vshlq_u32", vshlq_u32(u32, counts32));
    SHOW("vshlq_s16", vshlq_s16(s16, vld1q_s16(shift16_counts)));
    SHOW("vshl_s32", vshl_s32(vld1_s32(s32_buf), vld1_s32(shift32_counts)));
}

static void tables(void) {
    uint8x16_t t0 = vld1q_u8(u8_buf);
    uint8x16_t t1 = vld1q_u8(u8_buf + 16);
    uint8x16_t t2 = vld1q_u8(u8_buf + 32);
    uint8x16_t t3 = vaddq_u8(t0, t2);
    static const uint8_t idx_bytes[16] = {0, 15, 16, 31, 32, 47, 48, 63, 64, 255, 1, 17, 33, 49, 128, 7};
    uint8x16_t idx = vld1q_u8(idx_bytes);
    SHOW("vqtbl1q_u8", vqtbl1q_u8(t0, idx));
    SHOW("vqtbl1q_s8", vqtbl1q_s8(vreinterpretq_s8_u8(t1), idx));
    uint8x16x2_t two = {{t0, t1}};
    SHOW("vqtbl2q_u8", vqtbl2q_u8(two, idx));
    uint8x16x3_t three = {{t0, t1, t2}};
    SHOW("vqtbl3q_u8", vqtbl3q_u8(three, idx));
    uint8x16x4_t four = {{t0, t1, t2, t3}};
    SHOW("vqtbl4q_u8", vqtbl4q_u8(four, idx));
    SHOW("vqtbl1_u8", vqtbl1_u8(t2, vld1_u8(idx_bytes + 8)));
}

int main(void) {
    for (int i = 0; i < 48; i++)
        u8_buf[i] = (uint8_t)(i * 37 + 11);
    for (int i = 0; i < 17; i++)
        s16_buf[i] = (int16_t)(i * 7919 - 32768);
    loads();
    stores();
    shifts();
    tables();
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! SHOW__5 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < u32, 4 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < u8, 16 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__2 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < f32, 4 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__6 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i32, 4 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__3 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i8, 16 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! SHOW__4 {
// CHECK-NEXT:     ($name:expr, $value:expr) => {
// CHECK-NEXT:         loop { let mut shown : std::simd::Simd < i16, 8 > = $value; show_bytes(($name) as
// CHECK-NEXT:         * const u8, std::ptr::addr_of_mut!(shown) as * const (), 16u64); if true { break;
// CHECK-NEXT:         } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.tbl2.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_2a1c4c8e47a13d34(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.tbl4.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_36863d4e527d9a2a(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_4:arg[0-9]+]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sshl.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_50c4bc9dec513689(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.tbl3.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_50cd27b6c30f4b8b(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_2]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_3]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.ushl.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_5b38bc7c1f4634ce(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.ushl.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_8137799d48c47e0f(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<u32, 4>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<u32, 4>,
// CHECK-NEXT:     ) -> std::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sshl.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_bdcf82ba03381d03(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<i8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.tbl1.v8i8"]
// CHECK-NEXT:     fn __slate_intrinsic_c1588e228482f347(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<u8, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 8>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.tbl1.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_ef2ff56ee74cfc8d(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sshl.v2i32"]
// CHECK-NEXT:     fn __slate_intrinsic_f5147b29ebfa7cca(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i32, 2>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i32, 2>,
// CHECK-NEXT:     ) -> std::simd::Simd<i32, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sshl.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_f683013e940445ae(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i16, 8>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i16, 8>,
// CHECK-NEXT:     ) -> std::simd::Simd<i16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.tbl1.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_f959fc42b554e570(
// CHECK-NEXT:         [[ARG_0]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: std::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> std::simd::Simd<i8, 16>;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct int16x8x2_t {
// CHECK-NEXT:     val: [std::simd::Simd<i16, 8>; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct uint8x16x2_t {
// CHECK-NEXT:     val: [std::simd::Simd<u8, 16>; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct float32x4x2_t {
// CHECK-NEXT:     val: [std::simd::Simd<f32, 4>; 2],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct uint8x16x3_t {
// CHECK-NEXT:     val: [std::simd::Simd<u8, 16>; 3],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct uint8x16x4_t {
// CHECK-NEXT:     val: [std::simd::Simd<u8, 16>; 4],
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vaddq_u8(
// CHECK-NEXT:     __p0: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: std::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u8, 16> = __p0 + __p1;
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vshlq_u8(
// CHECK-NEXT:     __p0: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: std::simd::Simd<i8, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_5b38bc7c1f4634ce(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vshlq_u32(
// CHECK-NEXT:     __p0: std::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: std::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_8137799d48c47e0f(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vshlq_s8(
// CHECK-NEXT:     __p0: std::simd::Simd<i8, 16>,
// CHECK-NEXT:     __p1: std::simd::Simd<i8, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<i8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             __slate_intrinsic_bdcf82ba03381d03(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p0)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p1)
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vshlq_s32(
// CHECK-NEXT:     __p0: std::simd::Simd<i32, 4>,
// CHECK-NEXT:     __p1: std::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_50c4bc9dec513689(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vshlq_s16(
// CHECK-NEXT:     __p0: std::simd::Simd<i16, 8>,
// CHECK-NEXT:     __p1: std::simd::Simd<i16, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<i16, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_f683013e940445ae(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vshl_s32(
// CHECK-NEXT:     __p0: std::simd::Simd<i32, 2>,
// CHECK-NEXT:     __p1: std::simd::Simd<i32, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<i32, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_f5147b29ebfa7cca(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<i32, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<i32, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vqtbl1q_u8(
// CHECK-NEXT:     __p0: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: std::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ef2ff56ee74cfc8d(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vqtbl1q_s8(
// CHECK-NEXT:     __p0: std::simd::Simd<i8, 16>,
// CHECK-NEXT:     __p1: std::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<i8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             __slate_intrinsic_f959fc42b554e570(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p0)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p1)
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vqtbl1_u8(
// CHECK-NEXT:     __p0: std::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: std::simd::Simd<u8, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<u8, 8> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u8, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 8>,
// CHECK-NEXT:             std::simd::Simd<u8, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_c1588e228482f347(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<u8, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 8>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vqtbl2q_u8(
// CHECK-NEXT:     __p0: uint8x16x2_t,
// CHECK-NEXT:     __p1: std::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_2a1c4c8e47a13d34(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[0])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[1])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::memset;
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vqtbl3q_u8(
// CHECK-NEXT:     __p0: uint8x16x3_t,
// CHECK-NEXT:     __p1: std::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_50cd27b6c30f4b8b(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[0])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[1])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[2])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vqtbl4q_u8(
// CHECK-NEXT:     __p0: uint8x16x4_t,
// CHECK-NEXT:     __p1: std::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> std::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<u8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_36863d4e527d9a2a(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[0])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[1])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[2])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0.val[3])
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vreinterpretq_u32_s32(
// CHECK-NEXT:     __p0: std::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i32, 4>, std::simd::Simd<u32, 4>>(__p0)
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vreinterpretq_s8_u8(__p0: std::simd::Simd<u8, 16>) -> std::simd::Simd<i8, 16> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<u8, 16>, std::simd::Simd<i8, 16>>(__p0)
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vreinterpretq_s64_u64(
// CHECK-NEXT:     __p0: std::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> std::simd::Simd<i64, 2> {
// CHECK-NEXT:     let __ret: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<u64, 2>, std::simd::Simd<i64, 2>>(__p0)
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: static mut f32_buf: [f32; 13] = [
// CHECK-NEXT:     1.5f32,
// CHECK-NEXT:     -0.0f32,
// CHECK-NEXT:     3.25f32,
// CHECK-NEXT:     -7.0f32,
// CHECK-NEXT:     0.0f32,
// CHECK-NEXT:     -2.5f32,
// CHECK-NEXT:     1e30f32,
// CHECK-NEXT:     -1e-40f32,
// CHECK-NEXT:     9.0f32,
// CHECK-NEXT:     10.0f32,
// CHECK-NEXT:     11.0f32,
// CHECK-NEXT:     12.0f32,
// CHECK-NEXT:     13.0f32,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut f64_buf: [f64; 5] = [-0.0f64, 2.0f64, 0.0f64, -3.5f64, 4.0f64];
// CHECK-NEXT: static mut u8_buf: [u8; 48] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut s16_buf: [i16; 17] = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut s32_buf: [i32; 9] = [-2147483648, 2147483647, -1, 7, -5, 3, 2, 1, 0];
// CHECK-NEXT: static mut u64_buf: [u64; 3] = [18446744073709551615u64, 5, 9223372036854775808u64];
// CHECK-NEXT: static mut shift_counts: [i8; 16] = [
// CHECK-NEXT:     0,
// CHECK-NEXT:     1,
// CHECK-NEXT:     -1,
// CHECK-NEXT:     7,
// CHECK-NEXT:     -7,
// CHECK-NEXT:     8,
// CHECK-NEXT:     -8,
// CHECK-NEXT:     9,
// CHECK-NEXT:     -9,
// CHECK-NEXT:     64,
// CHECK-NEXT:     -64,
// CHECK-NEXT:     127,
// CHECK-NEXT:     -128,
// CHECK-NEXT:     3,
// CHECK-NEXT:     -3,
// CHECK-NEXT:     2,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut shift32_counts: [i32; 4] = [-31, 33, -32, 31];
// CHECK-NEXT: static mut shift16_counts: [i16; 8] = [-16, 16, -15, 15, 0, -1, 17, -17];
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
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn next_f32(p: *const f32) -> *const f32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-NEXT: fn loads() {
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: std::simd::Simd<f32, 4>;
// CHECK-NEXT:         let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(f32_buf) as *mut f32).add(1) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:         shown = [[V_0]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1q_f32+1".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<f64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: std::simd::Simd<f64, 2>;
// CHECK-NEXT:         let __ret: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<f64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(f64_buf) as *mut f64).add(1) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<f64, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:         shown = [[V_1]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1q_f64+1".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(3) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:         shown = [[V_2]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1q_u8+3".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: std::simd::Simd<i16, 8>;
// CHECK-NEXT:         let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i16, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s16_buf) as *mut i16).add(1) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_3]] = __ret;
// CHECK-NEXT:         shown = [[V_3]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1q_s16+1".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: std::simd::Simd<u64, 2>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u64_buf) as *mut u64).add(1) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u64, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_4]] = __ret;
// CHECK-NEXT:         shown = [[V_4]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1q_u64+1".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: std::simd::Simd<i32, 2>;
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s32_buf) as *mut i32).add(1) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i32, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_5]] = __ret;
// CHECK-NEXT:         shown = [[V_5]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1_s32+1".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u8, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: std::simd::Simd<u8, 8>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<u8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(5) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_6]] = __ret;
// CHECK-NEXT:         shown = [[V_6]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1_u8+5".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i16, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: std::simd::Simd<i16, 4>;
// CHECK-NEXT:         let __ret: std::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i16, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i16, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s16_buf) as *mut i16).add(3) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i16, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_7]] = __ret;
// CHECK-NEXT:         shown = [[V_7]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1_s16+3".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: std::simd::Simd<f32, 4>;
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
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(f32_buf) as *mut f32).add(8) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_8]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let base: std::simd::Simd<f32, 4> = [[V_8]];
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: std::simd::Simd<f32, 4>;
// CHECK-NEXT:         let __s1: std::simd::Simd<f32, 4> = base;
// CHECK-NEXT:         let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < f32, 4 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd < f32, 4 >, std::simd::Simd < i8, 16 >> (__s1) })
// CHECK-NEXT:                         }, std::simd::Simd::splat(unsafe { *
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(f32_buf) as * mut f32).add(7) as *
// CHECK-NEXT:                         const ()) as * mut f32) }), [0, 1, 6, 3]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_9]] = __ret;
// CHECK-NEXT:         shown = [[V_9]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1q_lane_f32".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: std::simd::Simd<i16, 8>;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: std::simd::Simd<i16, 8>;
// CHECK-NEXT:         {
// CHECK-NEXT:             let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i16, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i16, 8>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(s16_buf) as *mut i16) as *const ())
// CHECK-NEXT:                             as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:                             .read_unaligned()
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             };
// CHECK-NEXT:             [[V_11]] = __ret;
// CHECK-NEXT:         }
// CHECK-NEXT:         let __s1: std::simd::Simd<i16, 8> = [[V_11]];
// CHECK-NEXT:         let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i16, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < i16, 8 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd < i16, 8 >, std::simd::Simd < i8, 16 >> (__s1) })
// CHECK-NEXT:                         }, std::simd::Simd::splat(unsafe { *
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(s16_buf) as * mut i16).add(5) as *
// CHECK-NEXT:                         const ()) as * mut i16) }), [0, 1, 2, 3, 4, 5, 6, 15]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_10]] = __ret;
// CHECK-NEXT:         shown = [[V_10]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1q_lane_s16".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_12:__v[0-9]+]]: std::simd::Simd<i32, 2>;
// CHECK-NEXT:         let mut [[V_13:__v[0-9]+]]: std::simd::Simd<i32, 2>;
// CHECK-NEXT:         {
// CHECK-NEXT:             let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     std::simd::Simd<i32, 2>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i32, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(s32_buf) as *mut i32).add(2)
// CHECK-NEXT:                             as *const ()) as *mut std::simd::Simd<i32, 2>)
// CHECK-NEXT:                             .read_unaligned()
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             };
// CHECK-NEXT:             [[V_13]] = __ret;
// CHECK-NEXT:         }
// CHECK-NEXT:         let __s1: std::simd::Simd<i32, 2> = [[V_13]];
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i8, 8 >,
// CHECK-NEXT:                         std::simd::Simd < i32, 2 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd < i32, 2 >, std::simd::Simd < i8, 8 >> (__s1) })
// CHECK-NEXT:                         }, std::simd::Simd::splat(unsafe { *
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(s32_buf) as * mut i32) as * const ())
// CHECK-NEXT:                         as * mut i32) }), [0, 3]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_12]] = __ret;
// CHECK-NEXT:         shown = [[V_12]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1_lane_s32".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_14:__v[0-9]+]]: std::simd::Simd<u64, 2>;
// CHECK-NEXT:         let mut [[V_15:__v[0-9]+]]: std::simd::Simd<u64, 2>;
// CHECK-NEXT:         {
// CHECK-NEXT:             let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<u64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(u64_buf) as *mut u64) as *const ())
// CHECK-NEXT:                             as *mut std::simd::Simd<u64, 2>)
// CHECK-NEXT:                             .read_unaligned()
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             };
// CHECK-NEXT:             [[V_15]] = __ret;
// CHECK-NEXT:         }
// CHECK-NEXT:         let __s1: std::simd::Simd<u64, 2> = [[V_15]];
// CHECK-NEXT:         let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < u64, 2 >> (unsafe { std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd < u64, 2 >, std::simd::Simd < i8, 16 >> (__s1) })
// CHECK-NEXT:                         }, std::simd::Simd::splat(unsafe { *
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(u64_buf) as * mut u64).add(2) as *
// CHECK-NEXT:                         const ()) as * mut u64) }), [2, 1]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_14]] = __ret;
// CHECK-NEXT:         shown = [[V_14]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vld1q_lane_u64".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: float32x4x2_t;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: float32x4x2_t = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_17:__v[0-9]+]]: *mut () = std::ptr::addr_of_mut!(__ret) as *mut ();
// CHECK-NEXT:         let [[V_18:__v[0-9]+]]: *const () = next_f32(
// CHECK-NEXT:             (unsafe { (std::ptr::addr_of_mut!(f32_buf) as *mut f32).add(1) })
// CHECK-NEXT:                 as *const f32,
// CHECK-NEXT:         ) as *const ();
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ([[V_17]] as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 .write_unaligned(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         ([[V_18]] as * mut std::simd::Simd < f32, 4 >).read_unaligned(),
// CHECK-NEXT:                         ([[V_18]] as * mut std::simd::Simd < f32, 4 >).add(1)
// CHECK-NEXT:                         .read_unaligned(), [0, 2, 4, 6]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ([[V_17]] as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 .add(1)
// CHECK-NEXT:                 .write_unaligned(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         ([[V_18]] as * mut std::simd::Simd < f32, 4 >).read_unaligned(),
// CHECK-NEXT:                         ([[V_18]] as * mut std::simd::Simd < f32, 4 >).add(1)
// CHECK-NEXT:                         .read_unaligned(), [1, 3, 5, 7]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_16]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let pair: float32x4x2_t = [[V_16]];
// CHECK-NEXT:     SHOW__2!(c"vld2q_f32.0".as_ptr() as * mut u8, pair.val[0]);
// CHECK-NEXT:     SHOW__2!(c"vld2q_f32.1".as_ptr() as * mut u8, pair.val[1]);
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: uint8x16x2_t;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: uint8x16x2_t = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let [[V_20:__v[0-9]+]]: *mut () = std::ptr::addr_of_mut!(__ret) as *mut ();
// CHECK-NEXT:         let [[V_21:__v[0-9]+]]: *const () = (unsafe {
// CHECK-NEXT:             (std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(1)
// CHECK-NEXT:         }) as *const ();
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ([[V_20]] as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                 .write_unaligned(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         ([[V_21]] as * mut std::simd::Simd < u8, 16 >).read_unaligned(),
// CHECK-NEXT:                         ([[V_21]] as * mut std::simd::Simd < u8, 16 >).add(1)
// CHECK-NEXT:                         .read_unaligned(), [0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22,
// CHECK-NEXT:                         24, 26, 28, 30]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ([[V_20]] as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                 .add(1)
// CHECK-NEXT:                 .write_unaligned(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         ([[V_21]] as * mut std::simd::Simd < u8, 16 >).read_unaligned(),
// CHECK-NEXT:                         ([[V_21]] as * mut std::simd::Simd < u8, 16 >).add(1)
// CHECK-NEXT:                         .read_unaligned(), [1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23,
// CHECK-NEXT:                         25, 27, 29, 31]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         [[V_19]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let bytes: uint8x16x2_t = [[V_19]];
// CHECK-NEXT:     SHOW!(c"vld2q_u8.0".as_ptr() as * mut u8, bytes.val[0]);
// CHECK-NEXT:     SHOW!(c"vld2q_u8.1".as_ptr() as * mut u8, bytes.val[1]);
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: int16x8x2_t;
// CHECK-NEXT:     let mut __ret: int16x8x2_t = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let [[V_23:__v[0-9]+]]: *mut () = std::ptr::addr_of_mut!(__ret) as *mut ();
// CHECK-NEXT:     let [[V_24:__v[0-9]+]]: *const () = (unsafe {
// CHECK-NEXT:         (std::ptr::addr_of_mut!(s16_buf) as *mut i16).add(1)
// CHECK-NEXT:     }) as *const ();
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ([[V_23]] as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     ([[V_24]] as * mut std::simd::Simd < i16, 8 >).read_unaligned(), ([[V_24]]
// CHECK-NEXT:                     as * mut std::simd::Simd < i16, 8 >).add(1).read_unaligned(), [0, 2,
// CHECK-NEXT:                     4, 6, 8, 10, 12, 14]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ([[V_23]] as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:             .add(1)
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     ([[V_24]] as * mut std::simd::Simd < i16, 8 >).read_unaligned(), ([[V_24]]
// CHECK-NEXT:                     as * mut std::simd::Simd < i16, 8 >).add(1).read_unaligned(), [1, 3,
// CHECK-NEXT:                     5, 7, 9, 11, 13, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     [[V_22]] = __ret;
// CHECK-NEXT:     let words: int16x8x2_t = [[V_22]];
// CHECK-NEXT:     SHOW__4!(c"vld2q_s16.0".as_ptr() as * mut u8, words.val[0]);
// CHECK-NEXT:     SHOW__4!(c"vld2q_s16.1".as_ptr() as * mut u8, words.val[1]);
// CHECK-NEXT:     println!("calls {}", unsafe { calls });
// CHECK-NEXT: }
// CHECK-NEXT: fn stores() {
// CHECK-NEXT:     let mut out: [u8; 80] = [0u8; 80];
// CHECK-NEXT:     unsafe { out[0usize..80usize].fill((238i32 as i32) as u8) };
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_0]]: std::simd::Simd<f32, 4>;
// CHECK-NEXT:         let __ret: std::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(f32_buf) as *mut f32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:         let __s1: std::simd::Simd<f32, 4> = [[V_0]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (((out.as_mut_ptr() as *mut u8).add(4) as *mut f32)
// CHECK-NEXT:                 as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 .write_unaligned(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<f32, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__s1)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 });
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_25:__v[0-9]+]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(7) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_25]] = __ret;
// CHECK-NEXT:         let __s1: std::simd::Simd<u8, 16> = [[V_25]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (((out.as_mut_ptr() as *mut u8).add(21) as *mut ())
// CHECK-NEXT:                 as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                 .write_unaligned(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__s1)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 });
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_1]]: std::simd::Simd<i16, 4>;
// CHECK-NEXT:         let __ret: std::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i16, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i16, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s16_buf) as *mut i16).add(2) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i16, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:         let __s1: std::simd::Simd<i16, 4> = [[V_1]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             (((out.as_mut_ptr() as *mut u8).add(38) as *mut i16)
// CHECK-NEXT:                 as *mut std::simd::Simd<i16, 4>)
// CHECK-NEXT:                 .write_unaligned(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         std::simd::Simd<i16, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i16, 4>,
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                         >(__s1)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 });
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_26:__v[0-9]+]]: std::simd::Simd<i32, 4>;
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s32_buf) as *mut i32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_26]] = __ret;
// CHECK-NEXT:         let __s1: std::simd::Simd<i32, 4> = [[V_26]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((out.as_mut_ptr() as *mut u8).add(48) as *mut i32) = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__s1)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }[3];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_2]]: std::simd::Simd<f64, 2>;
// CHECK-NEXT:         let __ret: std::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<f64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<f64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(f64_buf) as *mut f64).add(2) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<f64, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:         let __s1: std::simd::Simd<f64, 2> = [[V_2]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((out.as_mut_ptr() as *mut u8).add(56) as *mut f64) = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     std::simd::Simd<f64, 2>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<f64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__s1)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }[1];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_27:__v[0-9]+]]: std::simd::Simd<u8, 8>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<u8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(9) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_27]] = __ret;
// CHECK-NEXT:         let __s1: std::simd::Simd<u8, 8> = [[V_27]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *(((out.as_mut_ptr() as *mut u8).add(47) as *mut ()) as *mut u8) = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     std::simd::Simd<u8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<u8, 8>,
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     >(__s1)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }[6];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_3]]: std::simd::Simd<u64, 2>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u64_buf) as *mut u64).add(1) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u64, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_3]] = __ret;
// CHECK-NEXT:         let __s1: std::simd::Simd<u64, 2> = [[V_3]];
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *((out.as_mut_ptr() as *mut u8).add(64) as *mut u64) = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<u64, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__s1)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }[1];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     show_bytes(
// CHECK-NEXT:         c"stores".as_ptr(),
// CHECK-NEXT:         out.as_mut_ptr() as *const (),
// CHECK-NEXT:         std::mem::size_of::<[u8; 80]>() as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut f32_out: [f32; 9] = [0 as f32; 9];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             ((f32_out.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (0i32 as i32) as u8,
// CHECK-NEXT:             ((std::mem::size_of::<[f32; 9]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_28:__v[0-9]+]]: std::simd::Simd<f32, 4>;
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
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(f32_buf) as *mut f32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_28]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4]]: std::simd::Simd<f32, 4>;
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
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(f32_buf) as *mut f32).add(4) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_4]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let pair: float32x4x2_t = float32x4x2_t { val: [[[V_28]], [[V_4]]] };
// CHECK-NEXT:     {
// CHECK-NEXT:         let __s1: float32x4x2_t = pair;
// CHECK-NEXT:         let [[V_29:__v[0-9]+]]: *mut () = (unsafe { (f32_out.as_mut_ptr() as *mut f32).add(1) })
// CHECK-NEXT:             as *mut ();
// CHECK-NEXT:         let [[V_5]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(__s1.val[0])
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[V_30:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(__s1.val[1])
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ([[V_29]] as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 .write_unaligned(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < f32, 4 >> ([[V_5]]) }, unsafe {
// CHECK-NEXT:                         std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < f32, 4 >> ([[V_30]]) }, [0, 4, 1, 5]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ([[V_29]] as *mut std::simd::Simd<f32, 4>)
// CHECK-NEXT:                 .add(1)
// CHECK-NEXT:                 .write_unaligned(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < f32, 4 >> ([[V_5]]) }, unsafe {
// CHECK-NEXT:                         std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < f32, 4 >> ([[V_30]]) }, [2, 6, 3, 7]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     show_bytes(
// CHECK-NEXT:         c"vst2q_f32".as_ptr(),
// CHECK-NEXT:         f32_out.as_mut_ptr() as *const (),
// CHECK-NEXT:         std::mem::size_of::<[f32; 9]>() as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut u8_out: [u8; 33] = [0u8; 33];
// CHECK-NEXT:     unsafe { u8_out[0usize..33usize].fill((0i32 as i32) as u8) };
// CHECK-NEXT:     let mut [[V_6]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_6]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_31:__v[0-9]+]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(16) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_31]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let bytes: uint8x16x2_t = uint8x16x2_t {
// CHECK-NEXT:         val: [[[V_6]], [[V_31]]],
// CHECK-NEXT:     };
// CHECK-NEXT:     {
// CHECK-NEXT:         let __s1: uint8x16x2_t = bytes;
// CHECK-NEXT:         let [[V_7]]: *mut () = (unsafe { (u8_out.as_mut_ptr() as *mut u8).add(1) })
// CHECK-NEXT:             as *mut ();
// CHECK-NEXT:         let [[V_32:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(__s1.val[0])
// CHECK-NEXT:         };
// CHECK-NEXT:         let [[V_8]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(__s1.val[1])
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ([[V_7]] as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                 .write_unaligned(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < u8, 16 >> ([[V_32]]) }, unsafe {
// CHECK-NEXT:                         std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < u8, 16 >> ([[V_8]]) }, [0, 16, 1, 17, 2, 18, 3,
// CHECK-NEXT:                         19, 4, 20, 5, 21, 6, 22, 7, 23]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             ([[V_7]] as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                 .add(1)
// CHECK-NEXT:                 .write_unaligned(
// CHECK-NEXT:                     std::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < u8, 16 >> ([[V_32]]) }, unsafe {
// CHECK-NEXT:                         std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                         std::simd::Simd < u8, 16 >> ([[V_8]]) }, [8, 24, 9, 25, 10, 26, 11,
// CHECK-NEXT:                         27, 12, 28, 13, 29, 14, 30, 15, 31]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     show_bytes(
// CHECK-NEXT:         c"vst2q_u8".as_ptr(),
// CHECK-NEXT:         u8_out.as_mut_ptr() as *const (),
// CHECK-NEXT:         std::mem::size_of::<[u8; 33]>() as u64,
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut s16_out: [i16; 17] = [0i16; 17];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::ptr::write_bytes(
// CHECK-NEXT:             ((s16_out.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             (0i32 as i32) as u8,
// CHECK-NEXT:             ((std::mem::size_of::<[i16; 17]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_9]]: std::simd::Simd<i16, 8>;
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
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s16_buf) as *mut i16) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_9]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_33:__v[0-9]+]]: std::simd::Simd<i16, 8>;
// CHECK-NEXT:     let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 (((std::ptr::addr_of_mut!(s16_buf) as *mut i16).add(8) as *const ())
// CHECK-NEXT:                     as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:                     .read_unaligned()
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_33]] = __ret;
// CHECK-NEXT:     let words: int16x8x2_t = int16x8x2_t { val: [[[V_9]], [[V_33]]] };
// CHECK-NEXT:     let __s1: int16x8x2_t = words;
// CHECK-NEXT:     let [[V_10]]: *mut () = (unsafe { (s16_out.as_mut_ptr() as *mut i16).add(1) })
// CHECK-NEXT:         as *mut ();
// CHECK-NEXT:     let [[V_11]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:         >(__s1.val[0])
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_34:__v[0-9]+]]: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd<i16, 8>,
// CHECK-NEXT:             std::simd::Simd<i8, 16>,
// CHECK-NEXT:         >(__s1.val[1])
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ([[V_10]] as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                     std::simd::Simd < i16, 8 >> ([[V_11]]) }, unsafe {
// CHECK-NEXT:                     std::mem::transmute::< std::simd::Simd < i8, 16 >, std::simd::Simd <
// CHECK-NEXT:                     i16, 8 >> ([[V_34]]) }, [0, 8, 1, 9, 2, 10, 3, 11]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         ([[V_10]] as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:             .add(1)
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 std::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe { std::mem::transmute::< std::simd::Simd < i8, 16 >,
// CHECK-NEXT:                     std::simd::Simd < i16, 8 >> ([[V_11]]) }, unsafe {
// CHECK-NEXT:                     std::mem::transmute::< std::simd::Simd < i8, 16 >, std::simd::Simd <
// CHECK-NEXT:                     i16, 8 >> ([[V_34]]) }, [4, 12, 5, 13, 6, 14, 7, 15]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     show_bytes(
// CHECK-NEXT:         c"vst2q_s16".as_ptr(),
// CHECK-NEXT:         s16_out.as_mut_ptr() as *const (),
// CHECK-NEXT:         std::mem::size_of::<[i16; 17]>() as u64,
// CHECK-NEXT:     );
// CHECK-NEXT: }
// CHECK-NEXT: fn shifts() {
// CHECK-NEXT:     let mut [[V_0]]: std::simd::Simd<i32, 4>;
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
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s32_buf) as *mut i32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let s32: std::simd::Simd<i32, 4> = [[V_0]];
// CHECK-NEXT:     let u32: std::simd::Simd<u32, 4> = unsafe { vreinterpretq_u32_s32(s32) };
// CHECK-NEXT:     let mut [[V_25]]: std::simd::Simd<i8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 ((((std::ptr::addr_of_mut!(u8_buf) as *mut u8) as *const i8).add(20)
// CHECK-NEXT:                     as *const ()) as *mut std::simd::Simd<i8, 16>)
// CHECK-NEXT:                     .read_unaligned()
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_25]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let s8: std::simd::Simd<i8, 16> = [[V_25]];
// CHECK-NEXT:     let mut [[V_1]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(20) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let u8: std::simd::Simd<u8, 16> = [[V_1]];
// CHECK-NEXT:     let mut [[V_26]]: std::simd::Simd<i16, 8>;
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
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(s16_buf) as *mut i16).add(1) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_26]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let s16: std::simd::Simd<i16, 8> = [[V_26]];
// CHECK-NEXT:     let mut [[V_2]]: std::simd::Simd<u64, 2>;
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
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u64_buf) as *mut u64) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u64, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let u64: std::simd::Simd<u64, 2> = [[V_2]];
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_27]]: std::simd::Simd<i32, 4>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i32, 4> = s32;
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) >> std::simd::Simd::splat(1i32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_27]] = __ret;
// CHECK-NEXT:         shown = [[V_27]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_s32 1".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_28]]: std::simd::Simd<i32, 4>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i32, 4> = s32;
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) >> std::simd::Simd::splat(31i32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_28]] = __ret;
// CHECK-NEXT:         shown = [[V_28]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_s32 31".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_29]]: std::simd::Simd<i32, 4>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i32, 4> = s32;
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) >> std::simd::Simd::splat(31i32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_29]] = __ret;
// CHECK-NEXT:         shown = [[V_29]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_s32 32".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_30]]: std::simd::Simd<u32, 4>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(std::simd::Simd::splat(0u32))
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_30]] = __ret;
// CHECK-NEXT:         shown = [[V_30]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_u32 32".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_31]]: std::simd::Simd<u32, 4>;
// CHECK-NEXT:         let __s0: std::simd::Simd<u32, 4> = u32;
// CHECK-NEXT:         let __ret: std::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) >> std::simd::Simd::splat(5u32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_31]] = __ret;
// CHECK-NEXT:         shown = [[V_31]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_u32 5".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_32]]: std::simd::Simd<i8, 16>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i8, 16> = s8;
// CHECK-NEXT:         let __ret: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                         std::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__s0)
// CHECK-NEXT:                 }) >> std::simd::Simd::splat(7i8),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_32]] = __ret;
// CHECK-NEXT:         shown = [[V_32]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_s8 8".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_9]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:         let __s0: std::simd::Simd<u8, 16> = u8;
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) >> std::simd::Simd::splat(3u8),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_9]] = __ret;
// CHECK-NEXT:         shown = [[V_9]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_u8 3".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_10]]: std::simd::Simd<i16, 8>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i16, 8> = s16;
// CHECK-NEXT:         let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i16, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) >> std::simd::Simd::splat(9i16),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_10]] = __ret;
// CHECK-NEXT:         shown = [[V_10]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_s16 9".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_34]]: std::simd::Simd<u64, 2>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(std::simd::Simd::splat(0u64))
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_34]] = __ret;
// CHECK-NEXT:         shown = [[V_34]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_u64 64".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_13]]: std::simd::Simd<i64, 2>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i64, 2> = unsafe { vreinterpretq_s64_u64(u64) };
// CHECK-NEXT:         let __ret: std::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i64, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) >> std::simd::Simd::splat(63i64),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_13]] = __ret;
// CHECK-NEXT:         shown = [[V_13]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshrq_n_s64 63".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u16, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_14]]: std::simd::Simd<u16, 4>;
// CHECK-NEXT:         let mut [[V_15]]: std::simd::Simd<u16, 4>;
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
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(s16_buf) as *mut i16) as *const u16)
// CHECK-NEXT:                             as *mut std::simd::Simd<u16, 4>)
// CHECK-NEXT:                             .read_unaligned()
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             };
// CHECK-NEXT:             [[V_15]] = __ret;
// CHECK-NEXT:         }
// CHECK-NEXT:         let __ret: std::simd::Simd<u16, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<u16, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u16, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(std::simd::Simd::splat(0u16))
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_14]] = __ret;
// CHECK-NEXT:         shown = [[V_14]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshr_n_u16 16".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_16]]: std::simd::Simd<i32, 4>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i32, 4> = s32;
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) << std::simd::Simd::splat(0i32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_16]] = __ret;
// CHECK-NEXT:         shown = [[V_16]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshlq_n_s32 0".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_18]]: std::simd::Simd<i32, 4>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i32, 4> = s32;
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 4>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) << std::simd::Simd::splat(31i32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_18]] = __ret;
// CHECK-NEXT:         shown = [[V_18]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshlq_n_s32 31".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_35:__v[0-9]+]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:         let __s0: std::simd::Simd<u8, 16> = u8;
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) << std::simd::Simd::splat(7u8),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_35]] = __ret;
// CHECK-NEXT:         shown = [[V_35]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshlq_n_u8 7".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_20]]: std::simd::Simd<i16, 8>;
// CHECK-NEXT:         let __s0: std::simd::Simd<i16, 8> = s16;
// CHECK-NEXT:         let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i16, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<i16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) << std::simd::Simd::splat(4i16),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_20]] = __ret;
// CHECK-NEXT:         shown = [[V_20]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshlq_n_s16 4".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u64, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_36:__v[0-9]+]]: std::simd::Simd<u64, 2>;
// CHECK-NEXT:         let __s0: std::simd::Simd<u64, 2> = u64;
// CHECK-NEXT:         let __ret: std::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u64, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             std::simd::Simd<u64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<u64, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) << std::simd::Simd::splat(63u64),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_36]] = __ret;
// CHECK-NEXT:         shown = [[V_36]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshlq_n_u64 63".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_22]]: std::simd::Simd<i32, 2>;
// CHECK-NEXT:         let mut [[V_23]]: std::simd::Simd<i32, 2>;
// CHECK-NEXT:         {
// CHECK-NEXT:             let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     std::simd::Simd<i32, 2>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i32, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(s32_buf) as *mut i32) as *const ())
// CHECK-NEXT:                             as *mut std::simd::Simd<i32, 2>)
// CHECK-NEXT:                             .read_unaligned()
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             };
// CHECK-NEXT:             [[V_23]] = __ret;
// CHECK-NEXT:         }
// CHECK-NEXT:         let __s0: std::simd::Simd<i32, 2> = [[V_23]];
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(
// CHECK-NEXT:                     (unsafe {
// CHECK-NEXT:                         std::mem::transmute::<
// CHECK-NEXT:                             std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             std::simd::Simd<i32, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             std::mem::transmute::<
// CHECK-NEXT:                                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:                                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                             >(__s0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     }) << std::simd::Simd::splat(3i32),
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_22]] = __ret;
// CHECK-NEXT:         shown = [[V_22]];
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshl_n_s32 3".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_37:__v[0-9]+]]: std::simd::Simd<i8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 (((std::ptr::addr_of_mut!(shift_counts) as *mut i8) as *const ())
// CHECK-NEXT:                     as *mut std::simd::Simd<i8, 16>)
// CHECK-NEXT:                     .read_unaligned()
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_37]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let counts: std::simd::Simd<i8, 16> = [[V_37]];
// CHECK-NEXT:     SHOW__3!(c"vshlq_s8".as_ptr() as * mut u8, unsafe { vshlq_s8(s8, counts) });
// CHECK-NEXT:     SHOW!(c"vshlq_u8".as_ptr() as * mut u8, unsafe { vshlq_u8(u8, counts) });
// CHECK-NEXT:     let mut [[V_38:__v[0-9]+]]: std::simd::Simd<i32, 4>;
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
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(shift32_counts) as *mut i32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i32, 4>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_38]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let counts32: std::simd::Simd<i32, 4> = [[V_38]];
// CHECK-NEXT:     SHOW__6!(c"vshlq_s32".as_ptr() as * mut u8, unsafe { vshlq_s32(s32, counts32) });
// CHECK-NEXT:     SHOW__5!(c"vshlq_u32".as_ptr() as * mut u8, unsafe { vshlq_u32(u32, counts32) });
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i16, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_39:__v[0-9]+]]: std::simd::Simd<i16, 8>;
// CHECK-NEXT:         let __ret: std::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<i16, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i16, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(shift16_counts) as *mut i16) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i16, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_39]] = __ret;
// CHECK-NEXT:         shown = unsafe { vshlq_s16(s16, [[V_39]]) };
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshlq_s16".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             16u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<i32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_40:__v[0-9]+]]: std::simd::Simd<i32, 2>;
// CHECK-NEXT:         {
// CHECK-NEXT:             let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     std::simd::Simd<i32, 2>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<
// CHECK-NEXT:                         std::simd::Simd<i32, 2>,
// CHECK-NEXT:                         std::simd::Simd<i8, 8>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         (((std::ptr::addr_of_mut!(s32_buf) as *mut i32) as *const ())
// CHECK-NEXT:                             as *mut std::simd::Simd<i32, 2>)
// CHECK-NEXT:                             .read_unaligned()
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             };
// CHECK-NEXT:             [[V_40]] = __ret;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_41:__v[0-9]+]]: std::simd::Simd<i32, 2>;
// CHECK-NEXT:         let __ret: std::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<i32, 2>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<i32, 2>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(shift32_counts) as *mut i32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<i32, 2>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_41]] = __ret;
// CHECK-NEXT:         shown = unsafe { vshl_s32([[V_40]], [[V_41]]) };
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vshl_s32".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn tables() {
// CHECK-NEXT:     let mut [[V_0]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let t0: std::simd::Simd<u8, 16> = [[V_0]];
// CHECK-NEXT:     let mut [[V_25]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(16) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_25]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let t1: std::simd::Simd<u8, 16> = [[V_25]];
// CHECK-NEXT:     let mut [[V_1]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of_mut!(u8_buf) as *mut u8).add(32) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let t2: std::simd::Simd<u8, 16> = [[V_1]];
// CHECK-NEXT:     let t3: std::simd::Simd<u8, 16> = unsafe { vaddq_u8(t0, t2) };
// CHECK-NEXT:     let mut [[V_26]]: std::simd::Simd<u8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 std::simd::Simd<u8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 16>,
// CHECK-NEXT:                     std::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of!(idx_bytes) as *const u8) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 16>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_26]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let idx: std::simd::Simd<u8, 16> = [[V_26]];
// CHECK-NEXT:     SHOW!(c"vqtbl1q_u8".as_ptr() as * mut u8, unsafe { vqtbl1q_u8(t0, idx) });
// CHECK-NEXT:     SHOW__3!(
// CHECK-NEXT:         c"vqtbl1q_s8".as_ptr() as * mut u8, unsafe { vqtbl1q_s8(vreinterpretq_s8_u8(t1),
// CHECK-NEXT:         idx) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let two: uint8x16x2_t = uint8x16x2_t { val: [t0, t1] };
// CHECK-NEXT:     SHOW!(c"vqtbl2q_u8".as_ptr() as * mut u8, unsafe { vqtbl2q_u8(two, idx) });
// CHECK-NEXT:     let three: uint8x16x3_t = uint8x16x3_t { val: [t0, t1, t2] };
// CHECK-NEXT:     SHOW!(c"vqtbl3q_u8".as_ptr() as * mut u8, unsafe { vqtbl3q_u8(three, idx) });
// CHECK-NEXT:     let four: uint8x16x4_t = uint8x16x4_t {
// CHECK-NEXT:         val: [t0, t1, t2, t3],
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW!(c"vqtbl4q_u8".as_ptr() as * mut u8, unsafe { vqtbl4q_u8(four, idx) });
// CHECK-NEXT:     loop {
// CHECK-NEXT:         let mut shown: std::simd::Simd<u8, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_29]]: std::simd::Simd<u8, 8>;
// CHECK-NEXT:         let __ret: std::simd::Simd<u8, 8> = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 std::simd::Simd<u8, 8>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<
// CHECK-NEXT:                     std::simd::Simd<u8, 8>,
// CHECK-NEXT:                     std::simd::Simd<i8, 8>,
// CHECK-NEXT:                 >(unsafe {
// CHECK-NEXT:                     (((std::ptr::addr_of!(idx_bytes) as *const u8).add(8) as *const ())
// CHECK-NEXT:                         as *mut std::simd::Simd<u8, 8>)
// CHECK-NEXT:                         .read_unaligned()
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_29]] = __ret;
// CHECK-NEXT:         shown = unsafe { vqtbl1_u8(t2, [[V_29]]) };
// CHECK-NEXT:         show_bytes(
// CHECK-NEXT:             c"vqtbl1_u8".as_ptr(),
// CHECK-NEXT:             std::ptr::addr_of_mut!(shown) as *const (),
// CHECK-NEXT:             8u64,
// CHECK-NEXT:         );
// CHECK-NEXT:         if true {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: static mut idx_bytes: [u8; 16] = [
// CHECK-NEXT:     0, 15, 16, 31, 32, 47, 48, 63, 64, 255, 1, 17, 33, 49, 128, 7,
// CHECK-NEXT: ];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     for i in 0i32..48i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             u8_buf[i as usize] = (i * 37 + 11) as u8;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     for i in 0i32..17i32 {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             s16_buf[i as usize] = (i * 7919 - 32768) as i16;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     loads();
// CHECK-NEXT:     stores();
// CHECK-NEXT:     shifts();
// CHECK-NEXT:     tables();
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
