/* { dg-options "-march=armv8.5-a+crypto+dotprod" } */
#include <arm_neon.h>
#include <arm_acle.h>
#include <stdio.h>
int32x4_t dot(int32x4_t a, int8x16_t b, int8x16_t c) { return vdotq_s32(a, b, c); }
uint32x4_t udot(uint32x4_t a, uint8x16_t b, uint8x16_t c) { return vdotq_u32(a, b, c); }
int32x2_t dot64(int32x2_t a, int8x8_t b, int8x8_t c) { return vdot_s32(a, b, c); }
int32x4_t rdm(int32x4_t a, int32x4_t b, int32x4_t c) { return vqrdmlahq_s32(a, b, c); }
int16x4_t rdms(int16x4_t a, int16x4_t b, int16x4_t c) { return vqrdmlsh_s16(a, b, c); }
uint32x4_t sha(uint32x4_t a, uint32x4_t b, uint32x4_t c) { return vsha256hq_u32(a, b, c); }
uint32x4_t sha2(uint32x4_t a, uint32x4_t b, uint32x4_t c) { return vsha256h2q_u32(a, b, c); }
uint32x4_t shas0(uint32x4_t a, uint32x4_t b) { return vsha256su0q_u32(a, b); }
uint32x4_t shas1(uint32x4_t a, uint32x4_t b, uint32x4_t c) { return vsha256su1q_u32(a, b, c); }
uint8x16_t aes(uint8x16_t a, uint8x16_t b) { return vaesmcq_u8(vaeseq_u8(a, b)); }
uint8x16_t aesd(uint8x16_t a, uint8x16_t b) { return vaesimcq_u8(vaesdq_u8(a, b)); }
float32x4_t cmla(float32x4_t a, float32x4_t b, float32x4_t c) { return vcmlaq_rot90_f32(vcmlaq_f32(a, b, c), b, c); }
float32x4_t r32(float32x4_t a) { return vrnd32xq_f32(a); }
float64x2_t r64(float64x2_t a) { return vrnd64zq_f64(a); }
uint32_t crc(uint32_t a, uint8_t b) { return __crc32b(a, b) ^ __crc32cd(a, 0x1234567890ull); }
uint64x2_t sha512(uint64x2_t a, uint64x2_t b, uint64x2_t c) { return vsha512su1q_u64(vsha512su0q_u64(vsha512h2q_u64(vsha512hq_u64(a, b, c), b, c), a), b, c); }
uint32x4_t sha1s(uint32x4_t a, uint32x4_t b, uint32x4_t c) { return vsha1su1q_u32(vsha1su0q_u32(a, b, c), b); }
uint8x16_t eor3(uint8x16_t a, uint8x16_t b, uint8x16_t c) { return vbcaxq_u8(veor3q_u8(a, b, c), b, c); }
int16x8_t eor3s(int16x8_t a, int16x8_t b, int16x8_t c) { return vbcaxq_s16(veor3q_s16(a, b, c), b, c); }
uint64x2_t rax(uint64x2_t a, uint64x2_t b) { return vrax1q_u64(a, b); }
uint32x4_t sm4(uint32x4_t a, uint32x4_t b) { return vsm4ekeyq_u32(vsm4eq_u32(a, b), b); }
float32x2_t r32z(float32x2_t a) { return vrnd32z_f32(a); }
float64x2_t r64x(float64x2_t a) { return vrnd64xq_f64(a); }
float64x2_t cmla64(float64x2_t a, float64x2_t b, float64x2_t c) { return vcmlaq_rot270_f64(vcmlaq_rot180_f64(a, b, c), b, c); }
uint32_t crcs(uint32_t a, uint16_t h, uint32_t w, uint64_t d) { return __crc32h(a, h) ^ __crc32w(a, w) ^ __crc32d(a, d) ^ __crc32cb(a, (uint8_t)w) ^ __crc32ch(a, h) ^ __crc32cw(a, w); }
int main(void) {
  int8x16_t b = vdupq_n_s8(3), c = vdupq_n_s8(-2);
  int32x4_t d = dot(vdupq_n_s32(1), b, c);
  printf("%d %u %d\n", vgetq_lane_s32(d, 0), vgetq_lane_u32(udot(vdupq_n_u32(1), vdupq_n_u8(200), vdupq_n_u8(3)), 1), vget_lane_s32(dot64(vdup_n_s32(5), vdup_n_s8(7), vdup_n_s8(-1)), 1));
  printf("%d %d\n", vgetq_lane_s32(rdm(vdupq_n_s32(0x7fffff00), vdupq_n_s32(0x40000000), vdupq_n_s32(0x40000000)), 2), vget_lane_s16(rdms(vdup_n_s16(5), vdup_n_s16(0x4000), vdup_n_s16(0x4000)), 3));
  uint32x4_t x = {1, 2, 3, 4}, y = {5, 6, 7, 8}, z = {9, 10, 11, 12};
  uint32x4_t s = shas1(shas0(sha2(sha(x, y, z), y, z), x), y, z);
  printf("%08x %08x %08x %08x\n", vgetq_lane_u32(s, 0), vgetq_lane_u32(s, 1), vgetq_lane_u32(s, 2), vgetq_lane_u32(s, 3));
  uint8x16_t k = vdupq_n_u8(0x5a), p = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  uint8x16_t e = aesd(aes(p, k), k);
  printf("%02x %02x %02x\n", vgetq_lane_u8(e, 0), vgetq_lane_u8(e, 7), vgetq_lane_u8(e, 15));
  float32x4_t f = {1.5f, -2.0f, 3.25f, 4.0f}, g = {0.5f, 2.0f, -1.0f, 3.0f};
  float32x4_t m = cmla(f, g, f);
  printf("%g %g %g %g\n", vgetq_lane_f32(m, 0), vgetq_lane_f32(m, 1), vgetq_lane_f32(m, 2), vgetq_lane_f32(m, 3));
  float32x4_t r = r32((float32x4_t){2.5f, -3.5f, 3e9f, -1e10f});
  float64x2_t q = r64((float64x2_t){-7.9, 1e19});
  printf("%g %g %g %g %g %g\n", vgetq_lane_f32(r, 0), vgetq_lane_f32(r, 1), vgetq_lane_f32(r, 2), vgetq_lane_f32(r, 3), vgetq_lane_f64(q, 0), vgetq_lane_f64(q, 1));
  printf("%08x %08x\n", crc(0xffffffffu, 0x61), crcs(0x12345678u, 0xbeef, 0xdeadbeefu, 0x0123456789abcdefull));
  uint64x2_t u = {0x0123456789abcdefull, 0xfedcba9876543210ull}, v = {3, 0x8000000000000001ull};
  uint64x2_t w5 = sha512(u, v, u), ra = rax(u, v);
  printf("%016llx %016llx %016llx %016llx\n", (unsigned long long)vgetq_lane_u64(w5, 0), (unsigned long long)vgetq_lane_u64(w5, 1), (unsigned long long)vgetq_lane_u64(ra, 0), (unsigned long long)vgetq_lane_u64(ra, 1));
  uint32x4_t s1 = sha1s(x, y, z), s4 = sm4(x, y);
  printf("%08x %08x %08x %08x\n", vgetq_lane_u32(s1, 0), vgetq_lane_u32(s1, 3), vgetq_lane_u32(s4, 0), vgetq_lane_u32(s4, 3));
  uint8x16_t e3 = eor3(p, k, vdupq_n_u8(0x0f));
  int16x8_t e4 = eor3s(vdupq_n_s16(-1), vdupq_n_s16(0x1234), vdupq_n_s16(0x00ff));
  printf("%02x %02x %d\n", vgetq_lane_u8(e3, 0), vgetq_lane_u8(e3, 9), vgetq_lane_s16(e4, 5));
  float32x2_t rz = r32z((float32x2_t){-2.7f, 5e9f});
  float64x2_t rx = r64x((float64x2_t){2.5, -1e300});
  float64x2_t c6 = cmla64((float64x2_t){1.0, 2.0}, (float64x2_t){3.0, -4.0}, (float64x2_t){0.5, 1.5});
  printf("%g %g %g %g %g %g\n", vget_lane_f32(rz, 0), vget_lane_f32(rz, 1), vgetq_lane_f64(rx, 0), vgetq_lane_f64(rx, 1), vgetq_lane_f64(c6, 0), vgetq_lane_f64(c6, 1));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-march=armv8.5-a+crypto+dotprod" }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crc32x"]
// CHECK-NEXT:     fn __slate_intrinsic_17cb5fa68264df63([[ARG_0:arg[0-9]+]]: i32, [[ARG_1:arg[0-9]+]]: i64) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.aesmc"]
// CHECK-NEXT:     fn __slate_intrinsic_1bdba3b3e4577e9d(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha512su0"]
// CHECK-NEXT:     fn __slate_intrinsic_23ef98e96abc68be(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<i64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crc32h"]
// CHECK-NEXT:     fn __slate_intrinsic_272725c78f4b77d8([[ARG_0]]: i32, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha256su1"]
// CHECK-NEXT:     fn __slate_intrinsic_2c662af202905e24(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha256h2"]
// CHECK-NEXT:     fn __slate_intrinsic_33f0a948bd573863(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.vcmla.rot270.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_36f89d27fdf2df25(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crc32b"]
// CHECK-NEXT:     fn __slate_intrinsic_462155345eff895([[ARG_0]]: i32, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.frint32x.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_49fa29ff26a300d3(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crc32cb"]
// CHECK-NEXT:     fn __slate_intrinsic_4abbdc21752fc676([[ARG_0]]: i32, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha512h"]
// CHECK-NEXT:     fn __slate_intrinsic_4b8e530a2846aaba(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<i64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.frint32z.v2f32"]
// CHECK-NEXT:     fn __slate_intrinsic_57e798d407f70bbe(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm4e"]
// CHECK-NEXT:     fn __slate_intrinsic_5cd28da3a50485a8(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.aese"]
// CHECK-NEXT:     fn __slate_intrinsic_63aef91514e459bf(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha512su1"]
// CHECK-NEXT:     fn __slate_intrinsic_649dd0804e4fea83(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<i64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha256su0"]
// CHECK-NEXT:     fn __slate_intrinsic_6ded4177e0d24aa4(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.vcmla.rot180.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_6ea0b2caf71d0c1d(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha512h2"]
// CHECK-NEXT:     fn __slate_intrinsic_7d8a01f19f2a5312(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<i64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.vcmla.rot90.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_7dd279da086bc771(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha256h"]
// CHECK-NEXT:     fn __slate_intrinsic_8c854741aac31f74(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crc32cx"]
// CHECK-NEXT:     fn __slate_intrinsic_9cea499d891e45a3([[ARG_0]]: i32, [[ARG_1]]: i64) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crc32ch"]
// CHECK-NEXT:     fn __slate_intrinsic_a24e9017c6d6c1f5([[ARG_0]]: i32, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sdot.v2i32.v8i8"]
// CHECK-NEXT:     fn __slate_intrinsic_a7aad796512aab4c(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i8, 8>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i8, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm4ekey"]
// CHECK-NEXT:     fn __slate_intrinsic_a9b6fba6c6df43dd(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.frint64x.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_a9f398d160cfef8a(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.aesd"]
// CHECK-NEXT:     fn __slate_intrinsic_aca6159a623cd1(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.eor3u.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_b13b248d124c5e8a(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sqrdmlsh.v4i16"]
// CHECK-NEXT:     fn __slate_intrinsic_b527fd638bf84e7a(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i16, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i16, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i16, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i16, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.rax1"]
// CHECK-NEXT:     fn __slate_intrinsic_be360c333226f3b6(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<i64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.udot.v4i32.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_be44da01249ab491(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<u32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.aesimc"]
// CHECK-NEXT:     fn __slate_intrinsic_beee17ed0b6d97ec(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.bcaxu.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_c9d0e810591991e(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<u8, 16>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.eor3s.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_ce4e4cf1b9b263d0(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i16, 8>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i16, 8>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<i16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crc32cw"]
// CHECK-NEXT:     fn __slate_intrinsic_cf85b123f0c23c23([[ARG_0]]: i32, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crc32w"]
// CHECK-NEXT:     fn __slate_intrinsic_e2d7c1a8b1fbce0c([[ARG_0]]: i32, [[ARG_1]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sdot.v4i32.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_e7e47a2f6288281e(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.bcaxs.v8i16"]
// CHECK-NEXT:     fn __slate_intrinsic_efe0cf1fa806b210(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i16, 8>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i16, 8>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<i16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha1su1"]
// CHECK-NEXT:     fn __slate_intrinsic_f229422c86d3ab14(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sqrdmlah.v4i32"]
// CHECK-NEXT:     fn __slate_intrinsic_f256643ebc3b49ac(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha1su0"]
// CHECK-NEXT:     fn __slate_intrinsic_f4a9ffc6aa0577a9(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.frint64z.v2f64"]
// CHECK-NEXT:     fn __slate_intrinsic_fc21634a0174f846(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     ) -> core::simd::Simd<f64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.vcmla.rot0.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_fefe1fd04d4c8583(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdotq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p2: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_be44da01249ab491(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdotq_s32(
// CHECK-NEXT:     __p0: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     __p2: core::simd::Simd<i8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_e7e47a2f6288281e(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                     __p1,
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                     __p2,
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdot_s32(
// CHECK-NEXT:     __p0: core::simd::Simd<i32, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<i8, 8>,
// CHECK-NEXT:     __p2: core::simd::Simd<i8, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i32, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i32, 2>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_a7aad796512aab4c(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i32, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i32, 2>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                     __p1,
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                     __p2,
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdupq_n_u8(__p0: u8) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                 [
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                 ],
// CHECK-NEXT:             );
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdupq_n_u32(__p0: u32) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([__p0, __p0, __p0, __p0]);
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdupq_n_s8(__p0: i8) -> core::simd::Simd<i8, 16> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                 [
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                     __p0,
// CHECK-NEXT:                 ],
// CHECK-NEXT:             );
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdupq_n_s32(__p0: i32) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([__p0, __p0, __p0, __p0]);
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdupq_n_s16(__p0: i16) -> core::simd::Simd<i16, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                 [__p0, __p0, __p0, __p0, __p0, __p0, __p0, __p0],
// CHECK-NEXT:             );
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdup_n_s8(__p0: i8) -> core::simd::Simd<i8, 8> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i8, 8> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     let __ret: core::simd::Simd<i8, 8> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array(
// CHECK-NEXT:                 [__p0, __p0, __p0, __p0, __p0, __p0, __p0, __p0],
// CHECK-NEXT:             );
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdup_n_s32(__p0: i32) -> core::simd::Simd<i32, 2> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([__p0, __p0]);
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdup_n_s16(__p0: i16) -> core::simd::Simd<i16, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([__p0, __p0, __p0, __p0]);
// CHECK-NEXT:             core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vqrdmlahq_s32(
// CHECK-NEXT:     __p0: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_f256643ebc3b49ac(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vqrdmlsh_s16(
// CHECK-NEXT:     __p0: core::simd::Simd<i16, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<i16, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<i16, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<i16, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_b527fd638bf84e7a(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vcmlaq_f32(
// CHECK-NEXT:     __p0: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_fefe1fd04d4c8583(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vcmlaq_rot90_f32(
// CHECK-NEXT:     __p0: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_7dd279da086bc771(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vaesdq_u8(
// CHECK-NEXT:     __p0: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_aca6159a623cd1(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vaeseq_u8(
// CHECK-NEXT:     __p0: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_63aef91514e459bf(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vaesimcq_u8(__p0: core::simd::Simd<u8, 16>) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_beee17ed0b6d97ec(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vaesmcq_u8(__p0: core::simd::Simd<u8, 16>) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_1bdba3b3e4577e9d(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha1su0q_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_f4a9ffc6aa0577a9(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p2,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha1su1q_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_f229422c86d3ab14(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha256hq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_8c854741aac31f74(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p2,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha256h2q_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_33f0a948bd573863(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p2,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha256su0q_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_6ded4177e0d24aa4(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha256su1q_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p2: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_2c662af202905e24(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p2,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vbcaxq_u8(
// CHECK-NEXT:     __p0: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p2: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_c9d0e810591991e(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vbcaxq_s16(
// CHECK-NEXT:     __p0: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     __p2: core::simd::Simd<i16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_efe0cf1fa806b210(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn veor3q_u8(
// CHECK-NEXT:     __p0: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p1: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     __p2: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_b13b248d124c5e8a(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn veor3q_s16(
// CHECK-NEXT:     __p0: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     __p2: core::simd::Simd<i16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_ce4e4cf1b9b263d0(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vrax1q_u64(
// CHECK-NEXT:     __p0: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_be360c333226f3b6(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha512hq_u64(
// CHECK-NEXT:     __p0: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p2: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_4b8e530a2846aaba(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p2,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha512h2q_u64(
// CHECK-NEXT:     __p0: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p2: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_7d8a01f19f2a5312(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p2,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha512su0q_u64(
// CHECK-NEXT:     __p0: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_23ef98e96abc68be(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha512su1q_u64(
// CHECK-NEXT:     __p0: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __p2: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_649dd0804e4fea83(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p2,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsm4eq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_5cd28da3a50485a8(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsm4ekeyq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_a9b6fba6c6df43dd(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p0,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __p1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vcmlaq_rot180_f64(
// CHECK-NEXT:     __p0: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p2: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_6ea0b2caf71d0c1d(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vcmlaq_rot270_f64(
// CHECK-NEXT:     __p0: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     __p2: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_36f89d27fdf2df25(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vrnd32xq_f32(__p0: core::simd::Simd<f32, 4>) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_49fa29ff26a300d3(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vrnd32z_f32(__p0: core::simd::Simd<f32, 2>) -> core::simd::Simd<f32, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<f32, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f32, 2>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_57e798d407f70bbe(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<f32, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f32, 2>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vrnd64xq_f64(__p0: core::simd::Simd<f64, 2>) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_a9f398d160cfef8a(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vrnd64zq_f64(__p0: core::simd::Simd<f64, 2>) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_fc21634a0174f846(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f64, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn __crc32b(__a: u32, __b: u8) -> u32 {
// CHECK-NEXT:     (unsafe { __slate_intrinsic_462155345eff895(__a as i32, (__b as u32) as i32) })
// CHECK-NEXT:         as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn __crc32h(__a: u32, __b: u16) -> u32 {
// CHECK-NEXT:     (unsafe { __slate_intrinsic_272725c78f4b77d8(__a as i32, (__b as u32) as i32) })
// CHECK-NEXT:         as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn __crc32w(__a: u32, __b: u32) -> u32 {
// CHECK-NEXT:     (unsafe { __slate_intrinsic_e2d7c1a8b1fbce0c(__a as i32, __b as i32) }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn __crc32d(__a: u32, __b: u64) -> u32 {
// CHECK-NEXT:     (unsafe { __slate_intrinsic_17cb5fa68264df63(__a as i32, __b as i64) }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn __crc32cb(__a: u32, __b: u8) -> u32 {
// CHECK-NEXT:     (unsafe { __slate_intrinsic_4abbdc21752fc676(__a as i32, (__b as u32) as i32) })
// CHECK-NEXT:         as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn __crc32ch(__a: u32, __b: u16) -> u32 {
// CHECK-NEXT:     (unsafe { __slate_intrinsic_a24e9017c6d6c1f5(__a as i32, (__b as u32) as i32) })
// CHECK-NEXT:         as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn __crc32cw(__a: u32, __b: u32) -> u32 {
// CHECK-NEXT:     (unsafe { __slate_intrinsic_cf85b123f0c23c23(__a as i32, __b as i32) }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn __crc32cd(__a: u32, __b: u64) -> u32 {
// CHECK-NEXT:     (unsafe { __slate_intrinsic_9cea499d891e45a3(__a as i32, __b as i64) }) as u32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn dot(
// CHECK-NEXT:     a: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     c: core::simd::Simd<i8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     return unsafe { vdotq_s32(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn udot(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     c: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     return unsafe { vdotq_u32(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn dot64(
// CHECK-NEXT:     a: core::simd::Simd<i32, 2>,
// CHECK-NEXT:     b: core::simd::Simd<i8, 8>,
// CHECK-NEXT:     c: core::simd::Simd<i8, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 2> {
// CHECK-NEXT:     return unsafe { vdot_s32(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn rdm(
// CHECK-NEXT:     a: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     c: core::simd::Simd<i32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     return unsafe { vqrdmlahq_s32(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn rdms(
// CHECK-NEXT:     a: core::simd::Simd<i16, 4>,
// CHECK-NEXT:     b: core::simd::Simd<i16, 4>,
// CHECK-NEXT:     c: core::simd::Simd<i16, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<i16, 4> {
// CHECK-NEXT:     return unsafe { vqrdmlsh_s16(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn sha(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     c: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     return unsafe { vsha256hq_u32(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn sha2(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     c: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     return unsafe { vsha256h2q_u32(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn shas0(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     return unsafe { vsha256su0q_u32(a, b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn shas1(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     c: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     return unsafe { vsha256su1q_u32(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn aes(
// CHECK-NEXT:     a: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     b: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     return unsafe { vaesmcq_u8(vaeseq_u8(a, b)) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn aesd(
// CHECK-NEXT:     a: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     b: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     return unsafe { vaesimcq_u8(vaesdq_u8(a, b)) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn cmla(
// CHECK-NEXT:     a: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     c: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe { vcmlaq_rot90_f32(vcmlaq_f32(a, b, c), b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn r32(a: core::simd::Simd<f32, 4>) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     return unsafe { vrnd32xq_f32(a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn r64(a: core::simd::Simd<f64, 2>) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe { vrnd64zq_f64(a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn crc(a: u32, b: u8) -> u32 {
// CHECK-NEXT:     (unsafe { __crc32b(a, b) }) ^ unsafe { __crc32cd(a, 78187493520u64) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn sha512(
// CHECK-NEXT:     a: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     b: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     c: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         vsha512su1q_u64(
// CHECK-NEXT:             vsha512su0q_u64(vsha512h2q_u64(vsha512hq_u64(a, b, c), b, c), a),
// CHECK-NEXT:             b,
// CHECK-NEXT:             c,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn sha1s(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     c: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     return unsafe { vsha1su1q_u32(vsha1su0q_u32(a, b, c), b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn eor3(
// CHECK-NEXT:     a: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     b: core::simd::Simd<u8, 16>,
// CHECK-NEXT:     c: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u8, 16> {
// CHECK-NEXT:     return unsafe { vbcaxq_u8(veor3q_u8(a, b, c), b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn eor3s(
// CHECK-NEXT:     a: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     b: core::simd::Simd<i16, 8>,
// CHECK-NEXT:     c: core::simd::Simd<i16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i16, 8> {
// CHECK-NEXT:     return unsafe { vbcaxq_s16(veor3q_s16(a, b, c), b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn rax(
// CHECK-NEXT:     a: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     b: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     return unsafe { vrax1q_u64(a, b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn sm4(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     return unsafe { vsm4ekeyq_u32(vsm4eq_u32(a, b), b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn r32z(a: core::simd::Simd<f32, 2>) -> core::simd::Simd<f32, 2> {
// CHECK-NEXT:     return unsafe { vrnd32z_f32(a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn r64x(a: core::simd::Simd<f64, 2>) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe { vrnd64xq_f64(a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn cmla64(
// CHECK-NEXT:     a: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     b: core::simd::Simd<f64, 2>,
// CHECK-NEXT:     c: core::simd::Simd<f64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f64, 2> {
// CHECK-NEXT:     return unsafe { vcmlaq_rot270_f64(vcmlaq_rot180_f64(a, b, c), b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn crcs(a: u32, h: u16, w: u32, d: u64) -> u32 {
// CHECK-NEXT:     (unsafe { __crc32h(a, h) }) ^ unsafe { __crc32w(a, w) } ^ unsafe { __crc32d(a, d) }
// CHECK-NEXT:         ^ unsafe { __crc32cb(a, w as u8) } ^ unsafe { __crc32ch(a, h) }
// CHECK-NEXT:         ^ unsafe { __crc32cw(a, w) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: core::simd::Simd<f32, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_5:__slate_compound_[0-9]+]]: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_6:__slate_compound_[0-9]+]]: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_7:__slate_compound_[0-9]+]]: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let b: core::simd::Simd<i8, 16> = unsafe { vdupq_n_s8(3i8) };
// CHECK-NEXT:     let c: core::simd::Simd<i8, 16> = unsafe { vdupq_n_s8(-2i8) };
// CHECK-NEXT:     let d: core::simd::Simd<i32, 4> = unsafe { dot(vdupq_n_s32(1i32), b, c) };
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = d;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32) as i32;
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:             udot(vdupq_n_u32(1u32), vdupq_n_u8(200u8), vdupq_n_u8(3u8))
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u32;
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:             dot64(vdup_n_s32(5i32), vdup_n_s8(7i8), vdup_n_s8(-1i8))
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 2>, core::simd::Simd<i32, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u32) as i32;
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%d %u %d\n".as_ptr(), [[V_0]], [[V_1]], [[V_2]]) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             rdm(
// CHECK-NEXT:                 vdupq_n_s32(2147483392i32),
// CHECK-NEXT:                 vdupq_n_s32(1073741824i32),
// CHECK-NEXT:                 vdupq_n_s32(1073741824i32),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[2] as u32) as i32;
// CHECK-NEXT:         [[V_3]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             rdms(vdup_n_s16(5i16), vdup_n_s16(16384i16), vdup_n_s16(16384i16))
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[3] as u16) as i16;
// CHECK-NEXT:         [[V_4]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {}", [[V_3]], [[V_4]] as i32);
// CHECK-NEXT:     let x: core::simd::Simd<u32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [1u32, 2u32, 3u32, 4u32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let y: core::simd::Simd<u32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [5u32, 6u32, 7u32, 8u32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let z: core::simd::Simd<u32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [9u32, 10u32, 11u32, 12u32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let s: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         shas1(shas0(sha2(sha(x, y, z), y, z), x), y, z)
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32;
// CHECK-NEXT:         [[V_5]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u32;
// CHECK-NEXT:         [[V_6]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[2] as u32;
// CHECK-NEXT:         [[V_7]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[3] as u32;
// CHECK-NEXT:         [[V_8]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%08x %08x %08x %08x\n".as_ptr(), [[V_5]], [[V_6]], [[V_7]], [[V_8]])
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let k: core::simd::Simd<u8, 16> = unsafe { vdupq_n_u8(90u8) };
// CHECK-NEXT:     let p: core::simd::Simd<u8, 16> = core::simd::Simd::from_array(
// CHECK-NEXT:         [
// CHECK-NEXT:             0u8, 1u8, 2u8, 3u8, 4u8, 5u8, 6u8, 7u8, 8u8, 9u8, 10u8, 11u8, 12u8, 13u8,
// CHECK-NEXT:             14u8, 15u8,
// CHECK-NEXT:         ],
// CHECK-NEXT:     );
// CHECK-NEXT:     let e: core::simd::Simd<u8, 16> = unsafe { aesd(aes(p, k), k) };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u8;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u8 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u8, 16> = e;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u8;
// CHECK-NEXT:         [[V_9]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u8;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u8 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u8, 16> = e;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[7] as u8;
// CHECK-NEXT:         [[V_10]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: u8;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u8 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u8, 16> = e;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[15] as u8;
// CHECK-NEXT:         [[V_11]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%02x %02x %02x\n".as_ptr(),
// CHECK-NEXT:             [[V_9]] as i32,
// CHECK-NEXT:             [[V_10]] as i32,
// CHECK-NEXT:             [[V_11]] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let f: core::simd::Simd<f32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [1.5f32, -2.0f32, 3.25f32, 4.0f32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let g: core::simd::Simd<f32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [0.5f32, 2.0f32, -1.0f32, 3.0f32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let m: core::simd::Simd<f32, 4> = unsafe { cmla(f, g, f) };
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = m;
// CHECK-NEXT:         __ret = __s0[0] as f32;
// CHECK-NEXT:         [[V_12]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = m;
// CHECK-NEXT:         __ret = __s0[1] as f32;
// CHECK-NEXT:         [[V_13]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = m;
// CHECK-NEXT:         __ret = __s0[2] as f32;
// CHECK-NEXT:         [[V_14]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = m;
// CHECK-NEXT:         __ret = __s0[3] as f32;
// CHECK-NEXT:         [[V_15]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             [[V_12]] as f64,
// CHECK-NEXT:             [[V_13]] as f64,
// CHECK-NEXT:             [[V_14]] as f64,
// CHECK-NEXT:             [[V_15]] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let r: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         r32(
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = core::simd::Simd::from_array(
// CHECK-NEXT:                     [2.5f32, -3.5f32, 3000000000.0f32, -10000000000.0f32],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let q: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         r64(
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_2]] = core::simd::Simd::from_array([-7.9f64, 1e19f64]);
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = r;
// CHECK-NEXT:         __ret = __s0[0] as f32;
// CHECK-NEXT:         [[V_16]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = r;
// CHECK-NEXT:         __ret = __s0[1] as f32;
// CHECK-NEXT:         [[V_17]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = r;
// CHECK-NEXT:         __ret = __s0[2] as f32;
// CHECK-NEXT:         [[V_18]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = r;
// CHECK-NEXT:         __ret = __s0[3] as f32;
// CHECK-NEXT:         [[V_19]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: f64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f64 = 0 as f64;
// CHECK-NEXT:         let __s0: core::simd::Simd<f64, 2> = q;
// CHECK-NEXT:         __ret = __s0[0] as f64;
// CHECK-NEXT:         [[V_20]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: f64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f64 = 0 as f64;
// CHECK-NEXT:         let __s0: core::simd::Simd<f64, 2> = q;
// CHECK-NEXT:         __ret = __s0[1] as f64;
// CHECK-NEXT:         [[V_21]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%g %g %g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             [[V_16]] as f64,
// CHECK-NEXT:             [[V_17]] as f64,
// CHECK-NEXT:             [[V_18]] as f64,
// CHECK-NEXT:             [[V_19]] as f64,
// CHECK-NEXT:             [[V_20]],
// CHECK-NEXT:             [[V_21]],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%08x %08x\n".as_ptr(),
// CHECK-NEXT:             crc(4294967295u32, 97u8),
// CHECK-NEXT:             crcs(305419896u32, 48879u16, 3735928559u32, 81985529216486895u64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let u: core::simd::Simd<u64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:         [81985529216486895u64, 18364758544493064720u64],
// CHECK-NEXT:     );
// CHECK-NEXT:     let v: core::simd::Simd<u64, 2> = core::simd::Simd::from_array(
// CHECK-NEXT:         [3u64, 9223372036854775809u64],
// CHECK-NEXT:     );
// CHECK-NEXT:     let w5: core::simd::Simd<u64, 2> = unsafe { sha512(u, v, u) };
// CHECK-NEXT:     let ra: core::simd::Simd<u64, 2> = unsafe { rax(u, v) };
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: u64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u64, 2> = w5;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u64;
// CHECK-NEXT:         [[V_22]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: u64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u64, 2> = w5;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u64;
// CHECK-NEXT:         [[V_23]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: u64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u64, 2> = ra;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u64;
// CHECK-NEXT:         [[V_24]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: u64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u64, 2> = ra;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u64;
// CHECK-NEXT:         [[V_25]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%016llx %016llx %016llx %016llx\n".as_ptr(),
// CHECK-NEXT:             [[V_22]],
// CHECK-NEXT:             [[V_23]],
// CHECK-NEXT:             [[V_24]],
// CHECK-NEXT:             [[V_25]],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let s1: core::simd::Simd<u32, 4> = unsafe { sha1s(x, y, z) };
// CHECK-NEXT:     let s4: core::simd::Simd<u32, 4> = unsafe { sm4(x, y) };
// CHECK-NEXT:     let mut [[V_26:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s1;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32;
// CHECK-NEXT:         [[V_26]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_27:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s1;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[3] as u32;
// CHECK-NEXT:         [[V_27]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_28:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s4;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32;
// CHECK-NEXT:         [[V_28]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_29:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s4;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[3] as u32;
// CHECK-NEXT:         [[V_29]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%08x %08x %08x %08x\n".as_ptr(),
// CHECK-NEXT:             [[V_26]],
// CHECK-NEXT:             [[V_27]],
// CHECK-NEXT:             [[V_28]],
// CHECK-NEXT:             [[V_29]],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let e3: core::simd::Simd<u8, 16> = unsafe { eor3(p, k, vdupq_n_u8(15u8)) };
// CHECK-NEXT:     let e4: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:         eor3s(vdupq_n_s16(-1i16), vdupq_n_s16(4660i16), vdupq_n_s16(255i16))
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_30:__v[0-9]+]]: u8;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u8 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u8, 16> = e3;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u8;
// CHECK-NEXT:         [[V_30]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_31:__v[0-9]+]]: u8;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u8 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u8, 16> = e3;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[9] as u8;
// CHECK-NEXT:         [[V_31]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_32:__v[0-9]+]]: i16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i16, 8> = e4;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[5] as u16) as i16;
// CHECK-NEXT:         [[V_32]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%02x %02x %d\n".as_ptr(),
// CHECK-NEXT:             [[V_30]] as i32,
// CHECK-NEXT:             [[V_31]] as i32,
// CHECK-NEXT:             [[V_32]] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let rz: core::simd::Simd<f32, 2> = unsafe {
// CHECK-NEXT:         r32z(
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_3]] = core::simd::Simd::from_array(
// CHECK-NEXT:                     [-2.7f32, 5000000000.0f32],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let rx: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         r64x(
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_4]] = core::simd::Simd::from_array([2.5f64, -1e300f64]);
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_4]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let c6: core::simd::Simd<f64, 2> = unsafe {
// CHECK-NEXT:         cmla64(
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_5]] = core::simd::Simd::from_array([1.0f64, 2.0f64]);
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_5]])
// CHECK-NEXT:             },
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_6]] = core::simd::Simd::from_array([3.0f64, -4.0f64]);
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_6]])
// CHECK-NEXT:             },
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_7]] = core::simd::Simd::from_array([0.5f64, 1.5f64]);
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_7]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_33:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 2> = rz;
// CHECK-NEXT:         __ret = __s0[0] as f32;
// CHECK-NEXT:         [[V_33]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_34:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 2> = rz;
// CHECK-NEXT:         __ret = __s0[1] as f32;
// CHECK-NEXT:         [[V_34]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_35:__v[0-9]+]]: f64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f64 = 0 as f64;
// CHECK-NEXT:         let __s0: core::simd::Simd<f64, 2> = rx;
// CHECK-NEXT:         __ret = __s0[0] as f64;
// CHECK-NEXT:         [[V_35]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_36:__v[0-9]+]]: f64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f64 = 0 as f64;
// CHECK-NEXT:         let __s0: core::simd::Simd<f64, 2> = rx;
// CHECK-NEXT:         __ret = __s0[1] as f64;
// CHECK-NEXT:         [[V_36]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_37:__v[0-9]+]]: f64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f64 = 0 as f64;
// CHECK-NEXT:         let __s0: core::simd::Simd<f64, 2> = c6;
// CHECK-NEXT:         __ret = __s0[0] as f64;
// CHECK-NEXT:         [[V_37]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_38:__v[0-9]+]]: f64;
// CHECK-NEXT:     let mut __ret: f64 = 0 as f64;
// CHECK-NEXT:     let __s0: core::simd::Simd<f64, 2> = c6;
// CHECK-NEXT:     __ret = __s0[1] as f64;
// CHECK-NEXT:     [[V_38]] = __ret;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%g %g %g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             [[V_33]] as f64,
// CHECK-NEXT:             [[V_34]] as f64,
// CHECK-NEXT:             [[V_35]],
// CHECK-NEXT:             [[V_36]],
// CHECK-NEXT:             [[V_37]],
// CHECK-NEXT:             [[V_38]],
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
