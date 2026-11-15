#include <arm_neon.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static float f32_in[2][4] = {
    {1.5f, -0.0f, 3.25f, -7.0f},
    {0.0f, -2.5f, 1e30f, -1e-40f},
};
static double f64_in[2][2] = {{-0.0, 2.0}, {0.0, -3.5}};
static int8_t s8_in[2][16] = {
    {-128, 127, -1, 0, 5, -6, 7, -8, 100, -100, 64, -64, 1, 2, 3, -127},
    {127, -128, 1, -1, -5, 6, -7, 8, 100, 100, -65, 63, -2, -3, 4, 126},
};
static uint8_t u8_in[16] = {255, 0, 1, 254, 128, 127, 3, 9, 200, 17, 33, 99, 250, 251, 252, 253};
static int16_t s16_in[2][8] = {{-32768, 32767, -1, 0, 1000, -1000, 7, -7},
                               {32767, -32768, 2, -2, -30000, 30000, 0, 1}};
static uint16_t u16_in[2][8] = {{65535, 0, 1, 65534, 32768, 32767, 3, 9},
                                {1, 65535, 65535, 2, 40000, 30000, 0, 9}};
static int32_t s32_in[2][4] = {{INT32_MIN, INT32_MAX, -1, 7}, {-5, INT32_MAX, INT32_MIN, 7}};
static uint32_t u32_in[2][4] = {{UINT32_MAX, 0, 1, 3000000000u}, {1, UINT32_MAX, 2, 1294967296u}};
static uint64_t u64_in[2][2] = {{UINT64_MAX, 5}, {7, 10}};

static void show_f32(const char *name, float32x4_t v) {
    uint32_t bits[4];
    memcpy(bits, &v, sizeof bits);
    printf("%s %08x %08x %08x %08x\n", name, bits[0], bits[1], bits[2], bits[3]);
}

static void show_f64(const char *name, float64x2_t v) {
    uint64_t bits[2];
    memcpy(bits, &v, sizeof bits);
    printf("%s %016llx %016llx\n", name, (unsigned long long)bits[0], (unsigned long long)bits[1]);
}

static void show_bytes(const char *name, const void *data, size_t size) {
    const unsigned char *bytes = data;
    printf("%s ", name);
    for (size_t i = 0; i < size; i++)
        printf("%02x", bytes[i]);
    printf("\n");
}

static void show_scalar_f32(const char *name, float value) {
    uint32_t bits;
    memcpy(&bits, &value, sizeof bits);
    printf("%s %08x\n", name, bits);
}

static void show_scalar_f64(const char *name, double value) {
    uint64_t bits;
    memcpy(&bits, &value, sizeof bits);
    printf("%s %016llx\n", name, (unsigned long long)bits);
}

#define SHOW(name, v) show_bytes(name, &(v), sizeof(v))

int main(void) {
    float32x4_t fa, fb, fc;
    float64x2_t da, db;
    int8x16_t sa, sb;
    uint8x16_t ua;
    int16x8_t ha, hb;
    uint16x8_t wa, wb;
    int32x4_t ia, ib;
    uint32x4_t ja, jb;
    uint64x2_t qa, qb;
    memcpy(&fa, f32_in[0], sizeof fa);
    memcpy(&fb, f32_in[1], sizeof fb);
    memcpy(&da, f64_in[0], sizeof da);
    memcpy(&db, f64_in[1], sizeof db);
    memcpy(&sa, s8_in[0], sizeof sa);
    memcpy(&sb, s8_in[1], sizeof sb);
    memcpy(&ua, u8_in, sizeof ua);
    memcpy(&ha, s16_in[0], sizeof ha);
    memcpy(&hb, s16_in[1], sizeof hb);
    memcpy(&wa, u16_in[0], sizeof wa);
    memcpy(&wb, u16_in[1], sizeof wb);
    memcpy(&ia, s32_in[0], sizeof ia);
    memcpy(&ib, s32_in[1], sizeof ib);
    memcpy(&ja, u32_in[0], sizeof ja);
    memcpy(&jb, u32_in[1], sizeof jb);
    memcpy(&qa, u64_in[0], sizeof qa);
    memcpy(&qb, u64_in[1], sizeof qb);

    float nan_lanes[4] = {NAN, 1.0f, -NAN, 2.0f};
    memcpy(&fc, nan_lanes, sizeof fc);
    double nan_doubles[2] = {NAN, -1.0};
    float64x2_t dn;
    memcpy(&dn, nan_doubles, sizeof dn);

    show_scalar_f32("lane_f32", vgetq_lane_f32(fa, 2));
    show_scalar_f64("lane_f64", vgetq_lane_f64(da, 0));
    printf("lane_s8 %d %d\n", vgetq_lane_s8(sa, 0), vgetq_lane_s8(sa, 15));
    printf("lane_u16 %u\n", vgetq_lane_u16(wa, 0));
    printf("lane_s32 %d\n", vgetq_lane_s32(ia, 0));
    printf("lane_u64 %llu %lld\n", (unsigned long long)vgetq_lane_u64(qa, 0),
           (long long)vgetq_lane_s64(vreinterpretq_s64_u64(qa), 0));
    printf("lane_u32 %u\n", vgetq_lane_u32(ja, 3));

    show_scalar_f32("addv_f32", vaddvq_f32(fa));
    show_scalar_f32("addv_f32_big", vaddvq_f32(fb));
    show_scalar_f64("addv_f64", vaddvq_f64(db));
    printf("addv_u16 %u\n", vaddvq_u16(wa));
    printf("addv_u32 %u\n", vaddvq_u32(ja));
    printf("addv_u64 %llu\n", (unsigned long long)vaddvq_u64(qa));
    printf("addlv_u8 %u\n", vaddlvq_u8(ua));
    printf("addlv_u16 %u\n", vaddlvq_u16(wa));

    show_scalar_f32("maxv_f32", vmaxvq_f32(fa));
    show_scalar_f32("maxv_f32_nan", vmaxvq_f32(fc));
    show_scalar_f32("minv_f32", vminvq_f32(fb));
    show_scalar_f64("maxv_f64", vmaxvq_f64(da));
    show_scalar_f64("minv_f64_nan", vminvq_f64(dn));
    show_scalar_f32("maxnmv_f32_nan", vmaxnmvq_f32(fc));
    show_scalar_f32("minnmv_f32_nan", vminnmvq_f32(fc));
    show_scalar_f64("maxnmv_f64_nan", vmaxnmvq_f64(dn));
    show_scalar_f64("minnmv_f64", vminnmvq_f64(db));
    printf("maxv_s8 %d minv_s8 %d\n", vmaxvq_s8(sa), vminvq_s8(sb));
    printf("maxv_s16 %d minv_s16 %d\n", vmaxvq_s16(ha), vminvq_s16(hb));
    printf("maxv_s32 %d minv_s32 %d\n", vmaxvq_s32(ia), vminvq_s32(ib));
    printf("maxv_u8 %u minv_u8 %u\n", vmaxvq_u8(ua), vminvq_u8(ua));
    printf("maxv_u16 %u minv_u16 %u\n", vmaxvq_u16(wa), vminvq_u16(wb));
    printf("maxv_u32 %u minv_u32 %u\n", vmaxvq_u32(ja), vminvq_u32(jb));

    uint32x4_t select = {0xffffffffu, 0, 0xffff0000u, 0x0000ffffu};
    show_f32("bsl_f32", vbslq_f32(select, fa, fb));
    uint8x16_t bsl_u8 = vbslq_u8(ua, vreinterpretq_u8_s8(sa), vreinterpretq_u8_s8(sb));
    SHOW("bsl_u8", bsl_u8);

    int8x16_t abs_s8 = vabsq_s8(sa);
    SHOW("abs_s8", abs_s8);
    int32x4_t abs_s32 = vabsq_s32(ia);
    SHOW("abs_s32", abs_s32);
    show_f64("abs_f64", vabsq_f64(da));
    show_f32("abs_f32_nan", vabsq_f32(fc));

    uint16x8_t max_u16 = vmaxq_u16(wa, wb);
    SHOW("max_u16", max_u16);
    int32x4_t max_s32 = vmaxq_s32(ia, ib);
    SHOW("max_s32", max_s32);
    int8x16_t min_s8 = vminq_s8(sa, sb);
    SHOW("min_s8", min_s8);
    uint32x4_t min_u32 = vminq_u32(ja, jb);
    SHOW("min_u32", min_u32);
    show_f32("max_f32_nan", vmaxq_f32(fa, fc));
    show_f32("max_f32_zero", vmaxq_f32(fa, fb));
    show_f64("min_f64", vminq_f64(da, db));
    show_f32("maxnm_f32_nan", vmaxnmq_f32(fa, fc));
    show_f32("minnm_f32_nan", vminnmq_f32(fc, fb));
    show_f64("maxnm_f64", vmaxnmq_f64(dn, db));
    show_f64("minnm_f64", vminnmq_f64(da, db));

    int8x16_t qadd_s8 = vqaddq_s8(sa, sb);
    SHOW("qadd_s8", qadd_s8);
    uint32x4_t qadd_u32 = vqaddq_u32(ja, jb);
    SHOW("qadd_u32", qadd_u32);
    int16x8_t qsub_s16 = vqsubq_s16(ha, hb);
    SHOW("qsub_s16", qsub_s16);
    uint64x2_t qsub_u64 = vqsubq_u64(qb, qa);
    SHOW("qsub_u64", qsub_u64);
    uint8x16_t qadd_u8 = vqaddq_u8(ua, ua);
    SHOW("qadd_u8", qadd_u8);

    show_f32("fma_f32", vfmaq_f32(fa, fb, fa));
    show_f64("fma_f64", vfmaq_f64(da, db, da));
    show_f32("sqrt_f32", vsqrtq_f32(vabsq_f32(fa)));
    show_f64("sqrt_f64", vsqrtq_f64(db));

    uint32x4_t cagt_f32 = vcagtq_f32(fa, fb);
    SHOW("cagt_f32", cagt_f32);
    uint32x4_t cagt_nan = vcagtq_f32(fc, fa);
    SHOW("cagt_nan", cagt_nan);
    uint64x2_t cagt_f64 = vcagtq_f64(da, db);
    SHOW("cagt_f64", cagt_f64);
    return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! SHOW {
// CHECK-NEXT:     ($name:expr, $v:expr) => {
// CHECK-NEXT:         show_bytes(($name) as * const u8, core::ptr::addr_of_mut!($v) as * const (),
// CHECK-NEXT:         16u64);
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.faddv.f64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_116292d1f1842b18([[ARG_0:arg[0-9]+]]: core::simd::Simd<f64, 2>) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v8i32"]
// CHECK-NEXT:     fn __slate_intrinsic_1289b355d60cd71c([[ARG_0]]: core::simd::Simd<u32, 8>) -> u32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmaxnmv.f32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_1e45a6a34b0154be([[ARG_0]]: core::simd::Simd<f32, 4>) -> f32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmaxv.f32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_2204a14738b4b36a([[ARG_0]]: core::simd::Simd<f32, 4>) -> f32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminnmv.f32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_242d01703380673e([[ARG_0]]: core::simd::Simd<f32, 4>) -> f32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmin.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_28c80a23aac4bce7(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.umax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_29954df4ca6da281([[ARG_0]]: core::simd::Simd<u32, 4>) -> u32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminnmv.f64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_2a596a503c082fd7([[ARG_0]]: core::simd::Simd<f64, 2>) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.facgt.v2i64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_2be0e100533527f1(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<u64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v2i64"]
// CHECK-NEXT:     fn __slate_intrinsic_2edab781d56c3c29([[ARG_0]]: core::simd::Simd<u64, 2>) -> u64;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_2f1d539bf793407f([[ARG_0]]: core::simd::Simd<u32, 4>) -> u32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmaxv.f64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_3103a600ad0fa3a4([[ARG_0]]: core::simd::Simd<f64, 2>) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmaxnmv.f64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_315618d5a2c41b2d([[ARG_0]]: core::simd::Simd<f64, 2>) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.fabs.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_36002b5d58bcd7c5(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smin.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_3fe3344f40e10f80([[ARG_0]]: core::simd::Simd<i16, 8>) -> i16;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmax.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_46fdfac7c42facdf(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.fabs.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_59393680d105b2b5(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_5ae72a4046d571b1(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminv.f64.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_5b3640b060d44082([[ARG_0]]: core::simd::Simd<f64, 2>) -> f64;
// CHECK-NEXT:     #[link_name = "llvm.fma.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_616387b99512e30e(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.uqadd.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_65db2654d2130ef0(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.umax.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_6960035b61849aa2([[ARG_0]]: core::simd::Simd<u8, 16>) -> u8;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.uqadd.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_73cded43a968d85c(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminnm.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_7b18ec430017e9fb(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.umax.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_803d05ff0d88249f(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u16, 8>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<u16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smax.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_81a13871527c628b([[ARG_0]]: core::simd::Simd<i16, 8>) -> i16;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.umin.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_85398d715a72c131([[ARG_0]]: core::simd::Simd<u8, 16>) -> u8;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_8b85c43cba315222(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.fma.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_8cfacf8a53c42a36(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v16i16"]
// CHECK-NEXT:     fn __slate_intrinsic_913ceccafb28254c([[ARG_0]]: core::simd::Simd<u16, 16>) -> u16;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smin.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_95a0a4e7264639a0([[ARG_0]]: core::simd::Simd<i8, 16>) -> i8;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sqsub.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_98fb02a79bd1fa3b(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i16, 8>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<i16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_9a56b33d8f4248b0([[ARG_0]]: core::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.umin.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_b411a632a4130bd6(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.smin.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_be8db5d650182eda(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smin.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_c5bcd0d7d8d03b90([[ARG_0]]: core::simd::Simd<i32, 4>) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmaxnm.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_c79a9a608d35c7d(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.umin.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_ce0ee746dd35708c([[ARG_0]]: core::simd::Simd<u16, 8>) -> u16;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.umax.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_d1e6a39bbe218dbc([[ARG_0]]: core::simd::Simd<u16, 8>) -> u16;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.smax.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_d723c8c1f3e6449f(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.abs.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_daf735dbaf56441f(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.add.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_dd4a165081cac072([[ARG_0]]: core::simd::Simd<u16, 8>) -> u16;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.facgt.v4i32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_de9d8be20f8589f1(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.faddv.f32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_e035389aa3b494cd([[ARG_0]]: core::simd::Simd<f32, 4>) -> f32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminv.f32.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_e11ba547017d5dc7([[ARG_0]]: core::simd::Simd<f32, 4>) -> f32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminnm.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_e23ea0185fda2baf(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sqadd.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_e4ccc4429e56ef0b(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.abs.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_f1d55301b87d3c55(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.uqsub.v2i64"]
// CHECK-NEXT:     fn __slate_intrinsic_f7bb94eb8d2ce272(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<u64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmaxnm.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_f8589827ae16c8ac(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.smax.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_f97cf353485dbff4([[ARG_0]]: core::simd::Simd<i8, 16>) -> i8;
// CHECK-NEXT:     #[link_name = "llvm.vector.reduce.umin.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_fd617c98b499a8ae([[ARG_0]]: core::simd::Simd<u32, 4>) -> u32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vabsq_s8(__p0: core::simd::Simd<i8, 16>) -> core::simd::Simd<i8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             __slate_intrinsic_daf735dbaf56441f(unsafe {
// CHECK-NEXT:                 core::mem::transmute::<
// CHECK-NEXT:                     core::simd::Simd<i8, 16>,
// CHECK-NEXT:                     core::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(__p0)
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vabsq_f32(__p0: core::simd::Simd<f32, 4>) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_59393680d105b2b5(unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vabsq_s32(__p0: core::simd::Simd<i32, 4>) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<i32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<i32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_f1d55301b87d3c55(unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<i32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i32, 4>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vbslq_u8(
// CHECK-NEXT:     __p0: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p2: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<u8, 16>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 core::mem::transmute::<
// CHECK-NEXT:                     core::simd::Simd<u8, 16>,
// CHECK-NEXT:                     core::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(__p2)
// CHECK-NEXT:             })
// CHECK-NEXT:                 ^ ((unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<u8, 16>,
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p1)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     ^ unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<u8, 16>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p2)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     & unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<u8, 16>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vbslq_f32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 core::mem::transmute::<
// CHECK-NEXT:                     core::simd::Simd<f32, 4>,
// CHECK-NEXT:                     core::simd::Simd<i8, 16>,
// CHECK-NEXT:                 >(__p2)
// CHECK-NEXT:             })
// CHECK-NEXT:                 ^ ((unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p1)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     ^ unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p2)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     & unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<u32, 4>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcagtq_f32(
// CHECK-NEXT:     __p0: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<u32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_de9d8be20f8589f1(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vmaxq_u16(
// CHECK-NEXT:     __p0: core::simd::Simd<u16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<u16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<u16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<u16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u16, 8>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_803d05ff0d88249f(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u16, 8>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u16, 8>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vmaxq_f32(
// CHECK-NEXT:     __p0: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_46fdfac7c42facdf(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vmaxq_s32(
// CHECK-NEXT:     __p0: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<i32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<i32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_d723c8c1f3e6449f(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<i32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<i32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: use slate_support::memcpy;
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<u32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_b411a632a4130bd6(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vminq_s8(
// CHECK-NEXT:     __p0: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     __p1: core::simd::Simd<i8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<i8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             __slate_intrinsic_be8db5d650182eda(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p0)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p1)
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vqaddq_u8(
// CHECK-NEXT:     __p0: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<u8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u8, 16>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_73cded43a968d85c(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u8, 16>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u8, 16>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vqaddq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<u32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_65db2654d2130ef0(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vqaddq_s8(
// CHECK-NEXT:     __p0: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     __p1: core::simd::Simd<i8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<i8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             __slate_intrinsic_e4ccc4429e56ef0b(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p0)
// CHECK-NEXT:                 },
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                     >(__p1)
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vqsubq_u64(
// CHECK-NEXT:     __p0: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<u64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_f7bb94eb8d2ce272(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<u64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<u64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vqsubq_s16(
// CHECK-NEXT:     __p0: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<i16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<i16, 8>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<i16, 8>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_98fb02a79bd1fa3b(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<i16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<i16, 8>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<i16, 8>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<i16, 8>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vmaxnmq_f64(
// CHECK-NEXT:     __p0: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_f8589827ae16c8ac(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vminnmq_f64(
// CHECK-NEXT:     __p0: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_e23ea0185fda2baf(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vmaxnmq_f32(
// CHECK-NEXT:     __p0: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_c79a9a608d35c7d(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vminnmq_f32(
// CHECK-NEXT:     __p0: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_7b18ec430017e9fb(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vfmaq_f32(
// CHECK-NEXT:     __p0: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_8cfacf8a53c42a36(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p2)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vabsq_f64(__p0: core::simd::Simd<f64, 2>) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_36002b5d58bcd7c5(unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vaddlvq_u8(__p0: core::simd::Simd<u8, 16>) -> u16 {
// CHECK-NEXT:     let __ret: u16 = (unsafe {
// CHECK-NEXT:         __slate_intrinsic_913ceccafb28254c(core::simd::num::SimdUint::cast::<u16>(__p0))
// CHECK-NEXT:     }) as u16;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vaddlvq_u16(__p0: core::simd::Simd<u16, 8>) -> u32 {
// CHECK-NEXT:     let __ret: u32 = (unsafe {
// CHECK-NEXT:         __slate_intrinsic_1289b355d60cd71c(core::simd::num::SimdUint::cast::<u32>(__p0))
// CHECK-NEXT:     }) as u32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vaddvq_u32(__p0: core::simd::Simd<u32, 4>) -> u32 {
// CHECK-NEXT:     let __ret: u32 = (unsafe { __slate_intrinsic_2f1d539bf793407f(__p0) }) as u32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vaddvq_u64(__p0: core::simd::Simd<u64, 2>) -> u64 {
// CHECK-NEXT:     let __ret: u64 = (unsafe { __slate_intrinsic_2edab781d56c3c29(__p0) }) as u64;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vaddvq_u16(__p0: core::simd::Simd<u16, 8>) -> u16 {
// CHECK-NEXT:     let __ret: u16 = (unsafe { __slate_intrinsic_dd4a165081cac072(__p0) }) as u16;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vaddvq_f64(__p0: core::simd::Simd<f64, 2>) -> f64 {
// CHECK-NEXT:     let __ret: f64 = (unsafe { __slate_intrinsic_116292d1f1842b18(__p0) }) as f64;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vaddvq_f32(__p0: core::simd::Simd<f32, 4>) -> f32 {
// CHECK-NEXT:     let __ret: f32 = (unsafe { __slate_intrinsic_e035389aa3b494cd(__p0) }) as f32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vcagtq_f64(
// CHECK-NEXT:     __p0: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<u64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_2be0e100533527f1(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vfmaq_f64(
// CHECK-NEXT:     __p0: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p2: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_616387b99512e30e(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p1)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p2)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxnmvq_f64(__p0: core::simd::Simd<f64, 2>) -> f64 {
// CHECK-NEXT:     let __ret: f64 = (unsafe { __slate_intrinsic_315618d5a2c41b2d(__p0) }) as f64;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxnmvq_f32(__p0: core::simd::Simd<f32, 4>) -> f32 {
// CHECK-NEXT:     let __ret: f32 = (unsafe { __slate_intrinsic_1e45a6a34b0154be(__p0) }) as f32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxvq_u8(__p0: core::simd::Simd<u8, 16>) -> u8 {
// CHECK-NEXT:     let __ret: u8 = (unsafe { __slate_intrinsic_6960035b61849aa2(__p0) }) as u8;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxvq_u32(__p0: core::simd::Simd<u32, 4>) -> u32 {
// CHECK-NEXT:     let __ret: u32 = (unsafe { __slate_intrinsic_29954df4ca6da281(__p0) }) as u32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxvq_u16(__p0: core::simd::Simd<u16, 8>) -> u16 {
// CHECK-NEXT:     let __ret: u16 = (unsafe { __slate_intrinsic_d1e6a39bbe218dbc(__p0) }) as u16;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxvq_s8(__p0: core::simd::Simd<i8, 16>) -> i8 {
// CHECK-NEXT:     let __ret: i8 = (unsafe { __slate_intrinsic_f97cf353485dbff4(__p0) }) as i8;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxvq_f64(__p0: core::simd::Simd<f64, 2>) -> f64 {
// CHECK-NEXT:     let __ret: f64 = (unsafe { __slate_intrinsic_3103a600ad0fa3a4(__p0) }) as f64;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxvq_f32(__p0: core::simd::Simd<f32, 4>) -> f32 {
// CHECK-NEXT:     let __ret: f32 = (unsafe { __slate_intrinsic_2204a14738b4b36a(__p0) }) as f32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxvq_s32(__p0: core::simd::Simd<i32, 4>) -> i32 {
// CHECK-NEXT:     let __ret: i32 = (unsafe { __slate_intrinsic_9a56b33d8f4248b0(__p0) }) as i32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vmaxvq_s16(__p0: core::simd::Simd<i16, 8>) -> i16 {
// CHECK-NEXT:     let __ret: i16 = (unsafe { __slate_intrinsic_81a13871527c628b(__p0) }) as i16;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminq_f64(
// CHECK-NEXT:     __p0: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_28c80a23aac4bce7(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             >(__p0)
// CHECK-NEXT:                         })
// CHECK-NEXT:                     },
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                         >(unsafe {
// CHECK-NEXT:                             core::mem::transmute::<
// CHECK-NEXT:                                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                                 core::simd::Simd<i8, 16>,
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
// CHECK-NEXT: unsafe fn vminnmvq_f64(__p0: core::simd::Simd<f64, 2>) -> f64 {
// CHECK-NEXT:     let __ret: f64 = (unsafe { __slate_intrinsic_2a596a503c082fd7(__p0) }) as f64;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminnmvq_f32(__p0: core::simd::Simd<f32, 4>) -> f32 {
// CHECK-NEXT:     let __ret: f32 = (unsafe { __slate_intrinsic_242d01703380673e(__p0) }) as f32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminvq_u8(__p0: core::simd::Simd<u8, 16>) -> u8 {
// CHECK-NEXT:     let __ret: u8 = (unsafe { __slate_intrinsic_85398d715a72c131(__p0) }) as u8;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminvq_u32(__p0: core::simd::Simd<u32, 4>) -> u32 {
// CHECK-NEXT:     let __ret: u32 = (unsafe { __slate_intrinsic_fd617c98b499a8ae(__p0) }) as u32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminvq_u16(__p0: core::simd::Simd<u16, 8>) -> u16 {
// CHECK-NEXT:     let __ret: u16 = (unsafe { __slate_intrinsic_ce0ee746dd35708c(__p0) }) as u16;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminvq_s8(__p0: core::simd::Simd<i8, 16>) -> i8 {
// CHECK-NEXT:     let __ret: i8 = (unsafe { __slate_intrinsic_95a0a4e7264639a0(__p0) }) as i8;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminvq_f64(__p0: core::simd::Simd<f64, 2>) -> f64 {
// CHECK-NEXT:     let __ret: f64 = (unsafe { __slate_intrinsic_5b3640b060d44082(__p0) }) as f64;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminvq_f32(__p0: core::simd::Simd<f32, 4>) -> f32 {
// CHECK-NEXT:     let __ret: f32 = (unsafe { __slate_intrinsic_e11ba547017d5dc7(__p0) }) as f32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminvq_s32(__p0: core::simd::Simd<i32, 4>) -> i32 {
// CHECK-NEXT:     let __ret: i32 = (unsafe { __slate_intrinsic_c5bcd0d7d8d03b90(__p0) }) as i32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vminvq_s16(__p0: core::simd::Simd<i16, 8>) -> i16 {
// CHECK-NEXT:     let __ret: i16 = (unsafe { __slate_intrinsic_3fe3344f40e10f80(__p0) }) as i16;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vreinterpretq_u8_s8(
// CHECK-NEXT:     __p0: core::simd::Simd<i8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(__p0)
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vreinterpretq_s64_u64(
// CHECK-NEXT:     __p0: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<i64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(__p0)
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vsqrtq_f64(__p0: core::simd::Simd<f64, 2>) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f64, 2>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_5ae72a4046d571b1(unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<f64, 2>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<f64, 2>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "neon")]
// CHECK-NEXT: unsafe fn vsqrtq_f32(__p0: core::simd::Simd<f32, 4>) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<
// CHECK-NEXT:             core::simd::Simd<i8, 16>,
// CHECK-NEXT:             core::simd::Simd<f32, 4>,
// CHECK-NEXT:         >(unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<f32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(unsafe {
// CHECK-NEXT:                 __slate_intrinsic_8b85c43cba315222(unsafe {
// CHECK-NEXT:                     core::mem::transmute::<
// CHECK-NEXT:                         core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         core::simd::Simd<f32, 4>,
// CHECK-NEXT:                     >(unsafe {
// CHECK-NEXT:                         core::mem::transmute::<
// CHECK-NEXT:                             core::simd::Simd<f32, 4>,
// CHECK-NEXT:                             core::simd::Simd<i8, 16>,
// CHECK-NEXT:                         >(__p0)
// CHECK-NEXT:                     })
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-NEXT: static mut f32_in: {{\[\[}}f32; 4]; 2] = [
// CHECK-NEXT:     [1.5f32, -0.0f32, 3.25f32, -7.0f32],
// CHECK-NEXT:     [0.0f32, -2.5f32, 1e30f32, -1e-40f32],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut f64_in: {{\[\[}}f64; 2]; 2] = {{\[\[}}-0.0f64, 2.0f64], [0.0f64, -3.5f64]];
// CHECK-NEXT: static mut s8_in: {{\[\[}}i8; 16]; 2] = [
// CHECK-NEXT:     [-128, 127, -1, 0, 5, -6, 7, -8, 100, -100, 64, -64, 1, 2, 3, -127],
// CHECK-NEXT:     [127, -128, 1, -1, -5, 6, -7, 8, 100, 100, -65, 63, -2, -3, 4, 126],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut u8_in: [u8; 16] = [
// CHECK-NEXT:     255, 0, 1, 254, 128, 127, 3, 9, 200, 17, 33, 99, 250, 251, 252, 253,
// CHECK-NEXT: ];
// CHECK-NEXT: static mut s16_in: {{\[\[}}i16; 8]; 2] = [
// CHECK-NEXT:     [-32768, 32767, -1, 0, 1000, -1000, 7, -7],
// CHECK-NEXT:     [32767, -32768, 2, -2, -30000, 30000, 0, 1],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut u16_in: {{\[\[}}u16; 8]; 2] = [
// CHECK-NEXT:     [65535, 0, 1, 65534, 32768, 32767, 3, 9],
// CHECK-NEXT:     [1, 65535, 65535, 2, 40000, 30000, 0, 9],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut s32_in: {{\[\[}}i32; 4]; 2] = [
// CHECK-NEXT:     [-2147483648, 2147483647, -1, 7],
// CHECK-NEXT:     [-5, 2147483647, -2147483648, 7],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut u32_in: {{\[\[}}u32; 4]; 2] = [
// CHECK-NEXT:     [4294967295, 0, 1, 3000000000],
// CHECK-NEXT:     [1, 4294967295, 2, 1294967296],
// CHECK-NEXT: ];
// CHECK-NEXT: static mut u64_in: {{\[\[}}u64; 2]; 2] = {{\[\[}}18446744073709551615u64, 5], [7, 10]];
// CHECK-NEXT: fn show_f32(name: *const u8, mut v: core::simd::Simd<f32, 4>) {
// CHECK-NEXT:     let mut bits: [u32; 4] = [0u32; 4];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(v) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((bits.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((core::mem::size_of::<[u32; 4]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %08x %08x %08x %08x\n".as_ptr(),
// CHECK-NEXT:             name,
// CHECK-NEXT:             bits[0],
// CHECK-NEXT:             bits[1],
// CHECK-NEXT:             bits[2],
// CHECK-NEXT:             bits[3],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn show_f64(name: *const u8, mut v: core::simd::Simd<f64, 2>) {
// CHECK-NEXT:     let mut bits: [u64; 2] = [0u64; 2];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(v) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((bits.as_mut_ptr() as *mut ()) as *mut core::ffi::c_void) as *mut u8,
// CHECK-NEXT:             ((core::mem::size_of::<[u64; 2]>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%s %016llx %016llx\n".as_ptr(), name, bits[0], bits[1])
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn show_bytes(name: *const u8, data: *const (), size: u64) {
// CHECK-NEXT:     let bytes: *const u8 = data as *const u8;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s ".as_ptr(), name) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0..size {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             slate_support::printf(c"%02x".as_ptr(), *bytes.add(i as usize) as i32)
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-NEXT: fn show_scalar_f32(name: *const u8, mut value: f32) {
// CHECK-NEXT:     let mut bits: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(value) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(bits) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             ((core::mem::size_of::<u32>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s %08x\n".as_ptr(), name, bits) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn show_scalar_f64(name: *const u8, mut value: f64) {
// CHECK-NEXT:     let mut bits: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(value) as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(bits) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             ((core::mem::size_of::<u64>() as u64) as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s %016llx\n".as_ptr(), name, bits) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut fa: core::simd::Simd<f32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut fb: core::simd::Simd<f32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut fc: core::simd::Simd<f32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut da: core::simd::Simd<f64, 2> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut db: core::simd::Simd<f64, 2> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut sa: core::simd::Simd<i8, 16> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut sb: core::simd::Simd<i8, 16> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut ua: core::simd::Simd<u8, 16> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut ha: core::simd::Simd<i16, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut hb: core::simd::Simd<i16, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut wa: core::simd::Simd<u16, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut wb: core::simd::Simd<u16, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut ia: core::simd::Simd<i32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut ib: core::simd::Simd<i32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut ja: core::simd::Simd<u32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut jb: core::simd::Simd<u32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut qa: core::simd::Simd<u64, 2> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let mut qb: core::simd::Simd<u64, 2> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(f32_in[0]) as *mut f32) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(fa) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(f32_in[1]) as *mut f32) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(fb) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(f64_in[0]) as *mut f64) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(da) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(f64_in[1]) as *mut f64) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(db) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(s8_in[0]) as *mut i8) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(sa) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(s8_in[1]) as *mut i8) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(sb) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(u8_in) as *mut u8) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(ua) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(s16_in[0]) as *mut i16) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(ha) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(s16_in[1]) as *mut i16) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(hb) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(u16_in[0]) as *mut u16) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(wa) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(u16_in[1]) as *mut u16) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(wb) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(s32_in[0]) as *mut i32) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(ia) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(s32_in[1]) as *mut i32) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(ib) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(u32_in[0]) as *mut u32) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(ja) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(u32_in[1]) as *mut u32) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(jb) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(u64_in[0]) as *mut u64) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(qa) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             (((core::ptr::addr_of_mut!(u64_in[1]) as *mut u64) as *const ())
// CHECK-NEXT:                 as *const core::ffi::c_void) as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(qb) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut nan_lanes: [f32; 4] = [
// CHECK-NEXT:         f32::from_bits(0x7fc00000),
// CHECK-NEXT:         1.0f32,
// CHECK-NEXT:         -f32::from_bits(0x7fc00000),
// CHECK-NEXT:         2.0f32,
// CHECK-NEXT:     ];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((nan_lanes.as_mut_ptr() as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(fc) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut nan_doubles: [f64; 2] = [f32::from_bits(0x7fc00000) as f64, -1.0f64];
// CHECK-NEXT:     let mut dn: core::simd::Simd<f64, 2> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::ptr::copy_nonoverlapping(
// CHECK-NEXT:             ((nan_doubles.as_mut_ptr() as *const ()) as *const core::ffi::c_void)
// CHECK-NEXT:                 as *const u8,
// CHECK-NEXT:             ((core::ptr::addr_of_mut!(dn) as *mut ()) as *mut core::ffi::c_void)
// CHECK-NEXT:                 as *mut u8,
// CHECK-NEXT:             (16u64 as usize) as usize,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = fa;
// CHECK-NEXT:         __ret = __s0[2] as f32;
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     show_scalar_f32(c"lane_f32".as_ptr(), [[V_0]]);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: f64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f64 = 0 as f64;
// CHECK-NEXT:         let __s0: core::simd::Simd<f64, 2> = da;
// CHECK-NEXT:         __ret = __s0[0] as f64;
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     show_scalar_f64(c"lane_f64".as_ptr(), [[V_1]]);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i8;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i8 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i8, 16> = sa;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(__s0)
// CHECK-NEXT:         }[0] as u8) as i8;
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i8;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i8 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i8, 16> = sa;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:                 core::simd::Simd<i8, 16>,
// CHECK-NEXT:             >(__s0)
// CHECK-NEXT:         }[15] as u8) as i8;
// CHECK-NEXT:         [[V_3]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("lane_s8 {} {}", [[V_2]] as i32, [[V_3]] as i32);
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u16, 8> = wa;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u16, 8>,
// CHECK-NEXT:                 core::simd::Simd<i16, 8>,
// CHECK-NEXT:             >(__s0)
// CHECK-NEXT:         }[0] as u16;
// CHECK-NEXT:         [[V_4]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"lane_u16 %u\n".as_ptr(), [[V_4]] as i32) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = ia;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<i32, 4>,
// CHECK-NEXT:                 core::simd::Simd<i32, 4>,
// CHECK-NEXT:             >(__s0)
// CHECK-NEXT:         }[0] as u32) as i32;
// CHECK-NEXT:         [[V_5]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("lane_s32 {}", [[V_5]]);
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u64, 2> = qa;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<u64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(__s0)
// CHECK-NEXT:         }[0] as u64;
// CHECK-NEXT:         [[V_6]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: i64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i64, 2> = unsafe { vreinterpretq_s64_u64(qa) };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<
// CHECK-NEXT:                 core::simd::Simd<i64, 2>,
// CHECK-NEXT:                 core::simd::Simd<i64, 2>,
// CHECK-NEXT:             >(__s0)
// CHECK-NEXT:         }[0] as u64) as i64;
// CHECK-NEXT:         [[V_7]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"lane_u64 %llu %lld\n".as_ptr(), [[V_6]], [[V_7]]) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u32;
// CHECK-NEXT:     let mut __ret: u32 = 0;
// CHECK-NEXT:     let __s0: core::simd::Simd<u32, 4> = ja;
// CHECK-NEXT:     __ret = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(__s0)
// CHECK-NEXT:     }[3] as u32;
// CHECK-NEXT:     [[V_8]] = __ret;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"lane_u32 %u\n".as_ptr(), [[V_8]]) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     show_scalar_f32(c"addv_f32".as_ptr(), unsafe { vaddvq_f32(fa) });
// CHECK-NEXT:     show_scalar_f32(c"addv_f32_big".as_ptr(), unsafe { vaddvq_f32(fb) });
// CHECK-NEXT:     show_scalar_f64(c"addv_f64".as_ptr(), unsafe { vaddvq_f64(db) });
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"addv_u16 %u\n".as_ptr(), vaddvq_u16(wa) as i32) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"addv_u32 %u\n".as_ptr(), vaddvq_u32(ja)) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"addv_u64 %llu\n".as_ptr(), vaddvq_u64(qa)) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"addlv_u8 %u\n".as_ptr(), vaddlvq_u8(ua) as i32) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"addlv_u16 %u\n".as_ptr(), vaddlvq_u16(wa)) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     show_scalar_f32(c"maxv_f32".as_ptr(), unsafe { vmaxvq_f32(fa) });
// CHECK-NEXT:     show_scalar_f32(c"maxv_f32_nan".as_ptr(), unsafe { vmaxvq_f32(fc) });
// CHECK-NEXT:     show_scalar_f32(c"minv_f32".as_ptr(), unsafe { vminvq_f32(fb) });
// CHECK-NEXT:     show_scalar_f64(c"maxv_f64".as_ptr(), unsafe { vmaxvq_f64(da) });
// CHECK-NEXT:     show_scalar_f64(c"minv_f64_nan".as_ptr(), unsafe { vminvq_f64(dn) });
// CHECK-NEXT:     show_scalar_f32(c"maxnmv_f32_nan".as_ptr(), unsafe { vmaxnmvq_f32(fc) });
// CHECK-NEXT:     show_scalar_f32(c"minnmv_f32_nan".as_ptr(), unsafe { vminnmvq_f32(fc) });
// CHECK-NEXT:     show_scalar_f64(c"maxnmv_f64_nan".as_ptr(), unsafe { vmaxnmvq_f64(dn) });
// CHECK-NEXT:     show_scalar_f64(c"minnmv_f64".as_ptr(), unsafe { vminnmvq_f64(db) });
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "maxv_s8 {} minv_s8 {}", (unsafe { vmaxvq_s8(sa) }) as i32, (unsafe {
// CHECK-NEXT:         vminvq_s8(sb) }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "maxv_s16 {} minv_s16 {}", (unsafe { vmaxvq_s16(ha) }) as i32, (unsafe {
// CHECK-NEXT:         vminvq_s16(hb) }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "maxv_s32 {} minv_s32 {}", unsafe { vmaxvq_s32(ia) }, unsafe { vminvq_s32(ib) }
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"maxv_u8 %u minv_u8 %u\n".as_ptr(),
// CHECK-NEXT:             vmaxvq_u8(ua) as i32,
// CHECK-NEXT:             vminvq_u8(ua) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"maxv_u16 %u minv_u16 %u\n".as_ptr(),
// CHECK-NEXT:             vmaxvq_u16(wa) as i32,
// CHECK-NEXT:             vminvq_u16(wb) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"maxv_u32 %u minv_u32 %u\n".as_ptr(),
// CHECK-NEXT:             vmaxvq_u32(ja),
// CHECK-NEXT:             vminvq_u32(jb),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let select: core::simd::Simd<u32, 4> = core::simd::Simd::from_array([
// CHECK-NEXT:         4294967295u32, 0u32, 4294901760u32, 65535u32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     show_f32(c"bsl_f32".as_ptr(), unsafe { vbslq_f32(select, fa, fb) });
// CHECK-NEXT:     let mut bsl_u8: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         vbslq_u8(ua, vreinterpretq_u8_s8(sa), vreinterpretq_u8_s8(sb))
// CHECK-NEXT:     };
// CHECK-NEXT:     SHOW!(c"bsl_u8".as_ptr() as * mut u8, bsl_u8);
// CHECK-NEXT:     let mut abs_s8: core::simd::Simd<i8, 16> = unsafe { vabsq_s8(sa) };
// CHECK-NEXT:     SHOW!(c"abs_s8".as_ptr() as * mut u8, abs_s8);
// CHECK-NEXT:     let mut abs_s32: core::simd::Simd<i32, 4> = unsafe { vabsq_s32(ia) };
// CHECK-NEXT:     SHOW!(c"abs_s32".as_ptr() as * mut u8, abs_s32);
// CHECK-NEXT:     show_f64(c"abs_f64".as_ptr(), unsafe { vabsq_f64(da) });
// CHECK-NEXT:     show_f32(c"abs_f32_nan".as_ptr(), unsafe { vabsq_f32(fc) });
// CHECK-NEXT:     let mut max_u16: core::simd::Simd<u16, 8> = unsafe { vmaxq_u16(wa, wb) };
// CHECK-NEXT:     SHOW!(c"max_u16".as_ptr() as * mut u8, max_u16);
// CHECK-NEXT:     let mut max_s32: core::simd::Simd<i32, 4> = unsafe { vmaxq_s32(ia, ib) };
// CHECK-NEXT:     SHOW!(c"max_s32".as_ptr() as * mut u8, max_s32);
// CHECK-NEXT:     let mut min_s8: core::simd::Simd<i8, 16> = unsafe { vminq_s8(sa, sb) };
// CHECK-NEXT:     SHOW!(c"min_s8".as_ptr() as * mut u8, min_s8);
// CHECK-NEXT:     let mut min_u32: core::simd::Simd<u32, 4> = unsafe { vminq_u32(ja, jb) };
// CHECK-NEXT:     SHOW!(c"min_u32".as_ptr() as * mut u8, min_u32);
// CHECK-NEXT:     show_f32(c"max_f32_nan".as_ptr(), unsafe { vmaxq_f32(fa, fc) });
// CHECK-NEXT:     show_f32(c"max_f32_zero".as_ptr(), unsafe { vmaxq_f32(fa, fb) });
// CHECK-NEXT:     show_f64(c"min_f64".as_ptr(), unsafe { vminq_f64(da, db) });
// CHECK-NEXT:     show_f32(c"maxnm_f32_nan".as_ptr(), unsafe { vmaxnmq_f32(fa, fc) });
// CHECK-NEXT:     show_f32(c"minnm_f32_nan".as_ptr(), unsafe { vminnmq_f32(fc, fb) });
// CHECK-NEXT:     show_f64(c"maxnm_f64".as_ptr(), unsafe { vmaxnmq_f64(dn, db) });
// CHECK-NEXT:     show_f64(c"minnm_f64".as_ptr(), unsafe { vminnmq_f64(da, db) });
// CHECK-NEXT:     let mut qadd_s8: core::simd::Simd<i8, 16> = unsafe { vqaddq_s8(sa, sb) };
// CHECK-NEXT:     SHOW!(c"qadd_s8".as_ptr() as * mut u8, qadd_s8);
// CHECK-NEXT:     let mut qadd_u32: core::simd::Simd<u32, 4> = unsafe { vqaddq_u32(ja, jb) };
// CHECK-NEXT:     SHOW!(c"qadd_u32".as_ptr() as * mut u8, qadd_u32);
// CHECK-NEXT:     let mut qsub_s16: core::simd::Simd<i16, 8> = unsafe { vqsubq_s16(ha, hb) };
// CHECK-NEXT:     SHOW!(c"qsub_s16".as_ptr() as * mut u8, qsub_s16);
// CHECK-NEXT:     let mut qsub_u64: core::simd::Simd<u64, 2> = unsafe { vqsubq_u64(qb, qa) };
// CHECK-NEXT:     SHOW!(c"qsub_u64".as_ptr() as * mut u8, qsub_u64);
// CHECK-NEXT:     let mut qadd_u8: core::simd::Simd<u8, 16> = unsafe { vqaddq_u8(ua, ua) };
// CHECK-NEXT:     SHOW!(c"qadd_u8".as_ptr() as * mut u8, qadd_u8);
// CHECK-NEXT:     show_f32(c"fma_f32".as_ptr(), unsafe { vfmaq_f32(fa, fb, fa) });
// CHECK-NEXT:     show_f64(c"fma_f64".as_ptr(), unsafe { vfmaq_f64(da, db, da) });
// CHECK-NEXT:     show_f32(c"sqrt_f32".as_ptr(), unsafe { vsqrtq_f32(vabsq_f32(fa)) });
// CHECK-NEXT:     show_f64(c"sqrt_f64".as_ptr(), unsafe { vsqrtq_f64(db) });
// CHECK-NEXT:     let mut cagt_f32: core::simd::Simd<u32, 4> = unsafe { vcagtq_f32(fa, fb) };
// CHECK-NEXT:     SHOW!(c"cagt_f32".as_ptr() as * mut u8, cagt_f32);
// CHECK-NEXT:     let mut cagt_nan: core::simd::Simd<u32, 4> = unsafe { vcagtq_f32(fc, fa) };
// CHECK-NEXT:     SHOW!(c"cagt_nan".as_ptr() as * mut u8, cagt_nan);
// CHECK-NEXT:     let mut cagt_f64: core::simd::Simd<u64, 2> = unsafe { vcagtq_f64(da, db) };
// CHECK-NEXT:     SHOW!(c"cagt_f64".as_ptr() as * mut u8, cagt_f64);
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
