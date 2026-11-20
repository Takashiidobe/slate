/* { dg-options "-march=armv8.5-a+crypto+dotprod+fp16" } */
#include <arm_neon.h>
#include <stdio.h>
int32x4_t dotl(int32x4_t a, int8x16_t b, int8x8_t c) { return vdotq_lane_s32(a, b, c, 1); }
uint32x2_t udotl(uint32x2_t a, uint8x8_t b, uint8x16_t c) { return vdot_laneq_u32(a, b, c, 3); }
float32x4_t cmlal(float32x4_t a, float32x4_t b, float32x2_t c) { return vcmlaq_lane_f32(a, b, c, 0); }
float32x4_t cmlal90(float32x4_t a, float32x4_t b, float32x4_t c) { return vcmlaq_rot90_laneq_f32(a, b, c, 1); }
int16_t rdmh(int16_t a, int16_t b, int16_t c) { return vqrdmlahh_s16(a, b, c); }
int32_t rdms(int32_t a, int32_t b, int32_t c) { return vqrdmlshs_s32(a, b, c); }
uint32x4_t sha1(uint32x4_t a, uint32_t e, uint32x4_t w) { return vsha1pq_u32(vsha1mq_u32(vsha1cq_u32(a, e, w), e, w), vsha1h_u32(e), w); }
uint64x2_t xar(uint64x2_t a, uint64x2_t b) { return vxarq_u64(a, b, 17); }
uint32x4_t sm3(uint32x4_t a, uint32x4_t b, uint32x4_t c) {
  uint32x4_t s = vsm3ss1q_u32(a, b, c);
  s = vsm3tt1aq_u32(s, a, b, 1);
  s = vsm3tt1bq_u32(s, a, b, 2);
  s = vsm3tt2aq_u32(s, a, b, 3);
  s = vsm3tt2bq_u32(s, a, b, 0);
  return vsm3partw2q_u32(vsm3partw1q_u32(s, a, b), a, b);
}
float16_t hmax(float16x8_t a) { return vmaxvq_f16(a) + vminvq_f16(a) + vmaxnmvq_f16(a) + vminnmv_f16(vget_low_f16(a)); }
static void p32(const char *n, uint32x4_t v) { printf("%s %08x %08x %08x %08x\n", n, vgetq_lane_u32(v, 0), vgetq_lane_u32(v, 1), vgetq_lane_u32(v, 2), vgetq_lane_u32(v, 3)); }
int main(void) {
  int8_t bs[16], cs[16];
  for (int i = 0; i < 16; i++) { bs[i] = (int8_t)(i * 13 - 100); cs[i] = (int8_t)(i * 7 - 50); }
  int32x4_t acc = {1, -2, 3, -4};
  int32x4_t d = dotl(acc, vld1q_s8(bs), vld1_s8(cs));
  printf("dotl %d %d %d %d\n", vgetq_lane_s32(d, 0), vgetq_lane_s32(d, 1), vgetq_lane_s32(d, 2), vgetq_lane_s32(d, 3));
  uint32x2_t ud = udotl((uint32x2_t){5, 6}, vld1_u8((uint8_t *)bs), vld1q_u8((uint8_t *)cs));
  printf("udotl %u %u\n", vget_lane_u32(ud, 0), vget_lane_u32(ud, 1));
  float32x4_t fa = {1.5f, -2.0f, 0.25f, 4.0f}, fb = {3.0f, 0.5f, -1.0f, 2.0f}, fc = {0.75f, -3.0f, 8.0f, 1.25f};
  float32x4_t c1 = cmlal(fa, fb, vget_low_f32(fc)), c2 = cmlal90(fa, fb, fc);
  printf("cmla %g %g %g %g | %g %g %g %g\n", vgetq_lane_f32(c1, 0), vgetq_lane_f32(c1, 1), vgetq_lane_f32(c1, 2), vgetq_lane_f32(c1, 3), vgetq_lane_f32(c2, 0), vgetq_lane_f32(c2, 1), vgetq_lane_f32(c2, 2), vgetq_lane_f32(c2, 3));
  printf("rdmh %d %d %d\n", rdmh(100, 20000, 30000), rdmh(32767, 32767, 32767), rdmh(-5, -32768, -32768));
  printf("rdms %d %d\n", rdms(7, 0x40000000, 0x40000000), rdms(-2147483647 - 1, 0x7fffffff, 0x7fffffff));
  uint32x4_t a = {0x67452301u, 0xefcdab89u, 0x98badcfeu, 0x10325476u}, w = {1u, 0xdeadbeefu, 0x12345678u, 0xffffffffu};
  p32("sha1", sha1(a, 0xc3d2e1f0u, w));
  printf("sha1h %08x\n", vsha1h_u32(0xc3d2e1f0u));
  uint64x2_t x = xar((uint64x2_t){0x0123456789abcdefull, 0xfedcba9876543210ull}, (uint64x2_t){0x1111111111111111ull, 0x8000000000000001ull});
  printf("xar %016llx %016llx\n", (unsigned long long)vgetq_lane_u64(x, 0), (unsigned long long)vgetq_lane_u64(x, 1));
  p32("sm3", sm3(a, w, (uint32x4_t){3u, 5u, 7u, 9u}));
  float16x8_t h = {1.5, -2.0, 65504.0, 0.25, -0.0, 3.0, -65504.0, 7.0};
  printf("hmax %g\n", (double)hmax(h));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(f16)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-march=armv8.5-a+crypto+dotprod+fp16" }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmaxv.f16.v8f16"]
// CHECK-NEXT:     fn __slate_intrinsic_11a0910a532e4111([[ARG_0:arg[0-9]+]]: core::simd::Simd<f16, 8>) -> f16;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm3partw1"]
// CHECK-NEXT:     fn __slate_intrinsic_17204cc694a9330d(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha1c"]
// CHECK-NEXT:     fn __slate_intrinsic_209e629b34ee54f8(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: i32,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm3ss1"]
// CHECK-NEXT:     fn __slate_intrinsic_344d95088ddbcb1(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmaxnmv.f16.v8f16"]
// CHECK-NEXT:     fn __slate_intrinsic_3aba8cf38bc7f089([[ARG_0]]: core::simd::Simd<f16, 8>) -> f16;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sqrdmlsh.i32"]
// CHECK-NEXT:     fn __slate_intrinsic_4a0b875333232845([[ARG_0]]: i32, [[ARG_1]]: i32, [[ARG_2]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha1m"]
// CHECK-NEXT:     fn __slate_intrinsic_4bca6bd8614f6efd(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: i32,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.xar"]
// CHECK-NEXT:     fn __slate_intrinsic_55f8b913e869080(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i64, 2>,
// CHECK-NEXT:         [[ARG_2]]: i64,
// CHECK-NEXT:     ) -> core::simd::Simd<i64, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm3tt1b"]
// CHECK-NEXT:     fn __slate_intrinsic_761a3128883ae7b(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_3:arg[0-9]+]]: i64,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm3tt2a"]
// CHECK-NEXT:     fn __slate_intrinsic_7809fc30f37b3539(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_3]]: i64,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.vcmla.rot90.v4f32"]
// CHECK-NEXT:     fn __slate_intrinsic_7dd279da086bc771(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha1h"]
// CHECK-NEXT:     fn __slate_intrinsic_83a9937a0d5c590d([[ARG_0]]: i32) -> i32;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.udot.v2i32.v8i8"]
// CHECK-NEXT:     fn __slate_intrinsic_99a88cc426f6c8cc(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<u32, 2>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<u8, 8>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<u8, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<u32, 2>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminnmv.f16.v4f16"]
// CHECK-NEXT:     fn __slate_intrinsic_adaf42af71a0b841([[ARG_0]]: core::simd::Simd<f16, 4>) -> f16;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sqrdmlah.v4i16"]
// CHECK-NEXT:     fn __slate_intrinsic_b08e9d1639b98415(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i16, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i16, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i16, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i16, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm3tt2b"]
// CHECK-NEXT:     fn __slate_intrinsic_c8e814aa1d284575(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_3]]: i64,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm3tt1a"]
// CHECK-NEXT:     fn __slate_intrinsic_d3a28a83b970326a(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_3]]: i64,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminv.f16.v8f16"]
// CHECK-NEXT:     fn __slate_intrinsic_d9be625d140af83b([[ARG_0]]: core::simd::Simd<f16, 8>) -> f16;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sm3partw2"]
// CHECK-NEXT:     fn __slate_intrinsic_e543c06eaaef89d1(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.sdot.v4i32.v16i8"]
// CHECK-NEXT:     fn __slate_intrinsic_e7e47a2f6288281e(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.crypto.sha1p"]
// CHECK-NEXT:     fn __slate_intrinsic_fdefd0576f3ecab4(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:         [[ARG_1]]: i32,
// CHECK-NEXT:         [[ARG_2]]: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<i32, 4>;
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdot_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 2>,
// CHECK-NEXT:     __p1: core::simd::Simd<u8, 8>,
// CHECK-NEXT:     __p2: core::simd::Simd<u8, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<u32, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 2>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_99a88cc426f6c8cc(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<u32, 2>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 2>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<u8, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 8>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<u8, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u8, 8>, core::simd::Simd<i8, 8>>(
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vget_low_f32(mut __p0: core::simd::Simd<f32, 4>) -> core::simd::Simd<f32, 2> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 2> = core::simd::simd_swizzle!(__p0, __p0, [0, 1]);
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vget_low_f16(mut __p0: core::simd::Simd<f16, 8>) -> core::simd::Simd<f16, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 4> = core::simd::simd_swizzle!(
// CHECK-NEXT:         __p0,
// CHECK-NEXT:         __p0,
// CHECK-NEXT:         [0, 1, 2, 3]
// CHECK-NEXT:     );
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha1cq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: u32,
// CHECK-NEXT:     __p2: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_intrinsic_209e629b34ee54f8(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                             __p0,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     __p1 as i32,
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                             __p2,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha1h_u32(__p0: u32) -> u32 {
// CHECK-NEXT:     let __ret: u32 = (unsafe { __slate_intrinsic_83a9937a0d5c590d(__p0 as i32) }) as u32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha1mq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: u32,
// CHECK-NEXT:     __p2: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_intrinsic_4bca6bd8614f6efd(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                             __p0,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     __p1 as i32,
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                             __p2,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsha1pq_u32(
// CHECK-NEXT:     __p0: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     __p1: u32,
// CHECK-NEXT:     __p2: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 __slate_intrinsic_fdefd0576f3ecab4(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                             __p0,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     __p1 as i32,
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                             __p2,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsm3partw1q_u32(
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
// CHECK-NEXT:                                 __slate_intrinsic_17204cc694a9330d(
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsm3partw2q_u32(
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
// CHECK-NEXT:                                 __slate_intrinsic_e543c06eaaef89d1(
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsm3ss1q_u32(
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
// CHECK-NEXT:                                 __slate_intrinsic_344d95088ddbcb1(
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vqrdmlahh_s16(__p0: i16, __p1: i16, __p2: i16) -> i16 {
// CHECK-NEXT:     let __ret: i16 = unsafe {
// CHECK-NEXT:         __slate_intrinsic_b08e9d1639b98415(
// CHECK-NEXT:             core::simd::Simd::splat(__p0),
// CHECK-NEXT:             core::simd::Simd::splat(__p1),
// CHECK-NEXT:             core::simd::Simd::splat(__p2),
// CHECK-NEXT:         )
// CHECK-NEXT:     }[0] as i16;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vqrdmlshs_s32(__p0: i32, __p1: i32, __p2: i32) -> i32 {
// CHECK-NEXT:     let __ret: i32 = (unsafe { __slate_intrinsic_4a0b875333232845(__p0, __p1, __p2) })
// CHECK-NEXT:         as i32;
// CHECK-NEXT:     __ret
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn dotl(
// CHECK-NEXT:     __s0_829: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     __s1_829: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     __s2_829: core::simd::Simd<i8, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: core::simd::Simd<i32, 4>;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: core::simd::Simd<i32, 4>;
// CHECK-NEXT:     let __s0: core::simd::Simd<i32, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i32, 2>>(
// CHECK-NEXT:             __s2_829,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     core::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i32, 2>>(
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     core::mem::transmute::<core::simd::Simd<i32, 2>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                         __s0,
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                 },
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                         [1, 1, 1, 1]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_1]] = __ret;
// CHECK-NEXT:     let __ret_829: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         vdotq_s32(
// CHECK-NEXT:             __s0_829,
// CHECK-NEXT:             __s1_829,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     [[V_1]],
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]] = __ret_829;
// CHECK-NEXT:     return [[V_0]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn udotl(
// CHECK-NEXT:     __s0_238: core::simd::Simd<u32, 2>,
// CHECK-NEXT:     __s1_238: core::simd::Simd<u8, 8>,
// CHECK-NEXT:     __s2_238: core::simd::Simd<u8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 2> {
// CHECK-NEXT:     let mut [[V_0]]: core::simd::Simd<u32, 2>;
// CHECK-NEXT:     let mut [[V_1]]: core::simd::Simd<u32, 2>;
// CHECK-NEXT:     let __s0: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             __s2_238,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<u32, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 2>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                     core::simd::simd_swizzle!(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                         __s0,
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                 },
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                         [3, 3]
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_1]] = __ret;
// CHECK-NEXT:     let __ret_238: core::simd::Simd<u32, 2> = unsafe {
// CHECK-NEXT:         vdot_u32(
// CHECK-NEXT:             __s0_238,
// CHECK-NEXT:             __s1_238,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 2>, core::simd::Simd<u8, 8>>(
// CHECK-NEXT:                     [[V_1]],
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]] = __ret_238;
// CHECK-NEXT:     return [[V_0]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn cmlal(
// CHECK-NEXT:     __s0_184: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __s1_184: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __s2_184: core::simd::Simd<f32, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0]]: core::simd::Simd<f32, 4>;
// CHECK-NEXT:     let mut [[V_1]]: u64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u64, 1> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f32, 2>, core::simd::Simd<u64, 1>>(
// CHECK-NEXT:                 __s2_184,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u64, 1>, core::simd::Simd<i64, 1>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u64;
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: u64;
// CHECK-NEXT:     let mut __ret: u64 = 0;
// CHECK-NEXT:     let __s0: core::simd::Simd<u64, 1> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<f32, 2>, core::simd::Simd<u64, 1>>(
// CHECK-NEXT:             __s2_184,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     __ret = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u64, 1>, core::simd::Simd<i64, 1>>(__s0)
// CHECK-NEXT:     }[0] as u64;
// CHECK-NEXT:     [[V_2]] = __ret;
// CHECK-NEXT:     let __ret_184: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         vcmlaq_f32(
// CHECK-NEXT:             __s0_184,
// CHECK-NEXT:             __s1_184,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([[[V_1]], [[V_2]]]);
// CHECK-NEXT:                         core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]] = __ret_184;
// CHECK-NEXT:     return [[V_0]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn cmlal90(
// CHECK-NEXT:     __s0_212: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __s1_212: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     __s2_212: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_0]]: core::simd::Simd<f32, 4>;
// CHECK-NEXT:     let mut [[V_1]]: u64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                 __s2_212,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u64;
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2]]: u64;
// CHECK-NEXT:     let mut __ret: u64 = 0;
// CHECK-NEXT:     let __s0: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:             __s2_212,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     __ret = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(__s0)
// CHECK-NEXT:     }[1] as u64;
// CHECK-NEXT:     [[V_2]] = __ret;
// CHECK-NEXT:     let __ret_212: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         vcmlaq_rot90_f32(
// CHECK-NEXT:             __s0_212,
// CHECK-NEXT:             __s1_212,
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                     *{
// CHECK-NEXT:                         [[SLATE_COMPOUND_0]] = core::simd::Simd::from_array([[[V_1]], [[V_2]]]);
// CHECK-NEXT:                         core::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]] = __ret_212;
// CHECK-NEXT:     return [[V_0]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn rdmh(a: i16, b: i16, c: i16) -> i16 {
// CHECK-NEXT:     unsafe { vqrdmlahh_s16(a, b, c) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn rdms(a: i32, b: i32, c: i32) -> i32 {
// CHECK-NEXT:     unsafe { vqrdmlshs_s32(a, b, c) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn sha1(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     e: u32,
// CHECK-NEXT:     w: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         vsha1pq_u32(vsha1mq_u32(vsha1cq_u32(a, e, w), e, w), vsha1h_u32(e), w)
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn xar(
// CHECK-NEXT:     __s0: core::simd::Simd<u64, 2>,
// CHECK-NEXT:     __s1: core::simd::Simd<u64, 2>,
// CHECK-NEXT: ) -> core::simd::Simd<u64, 2> {
// CHECK-NEXT:     let mut [[V_0]]: core::simd::Simd<u64, 2>;
// CHECK-NEXT:     let __ret: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i64, 2>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_55f8b913e869080(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u64, 2>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __s0,
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
// CHECK-NEXT:                                                             __s1,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     const { 17 as i64 },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_0]] = __ret;
// CHECK-NEXT:     return [[V_0]];
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn sm3(
// CHECK-NEXT:     a: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     b: core::simd::Simd<u32, 4>,
// CHECK-NEXT:     c: core::simd::Simd<u32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<u32, 4> {
// CHECK-NEXT:     let mut s: core::simd::Simd<u32, 4> = unsafe { vsm3ss1q_u32(a, b, c) };
// CHECK-NEXT:     let mut [[V_0]]: core::simd::Simd<u32, 4>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s;
// CHECK-NEXT:         let __s1: core::simd::Simd<u32, 4> = a;
// CHECK-NEXT:         let __s2: core::simd::Simd<u32, 4> = b;
// CHECK-NEXT:         let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     __slate_intrinsic_d3a28a83b970326a(
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s0,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s1,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s2,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         const { 1 as i64 },
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                 },
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     s = [[V_0]];
// CHECK-NEXT:     let mut [[V_1]]: core::simd::Simd<u32, 4>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s;
// CHECK-NEXT:         let __s1: core::simd::Simd<u32, 4> = a;
// CHECK-NEXT:         let __s2: core::simd::Simd<u32, 4> = b;
// CHECK-NEXT:         let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     __slate_intrinsic_761a3128883ae7b(
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s0,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s1,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s2,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         const { 2 as i64 },
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                 },
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     s = [[V_1]];
// CHECK-NEXT:     let mut [[V_2]]: core::simd::Simd<u32, 4>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = s;
// CHECK-NEXT:         let __s1: core::simd::Simd<u32, 4> = a;
// CHECK-NEXT:         let __s2: core::simd::Simd<u32, 4> = b;
// CHECK-NEXT:         let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     __slate_intrinsic_7809fc30f37b3539(
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s0,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s1,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         unsafe {
// CHECK-NEXT:                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                                 unsafe {
// CHECK-NEXT:                                                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                         unsafe {
// CHECK-NEXT:                                                             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                                 __s2,
// CHECK-NEXT:                                                             )
// CHECK-NEXT:                                                         },
// CHECK-NEXT:                                                     )
// CHECK-NEXT:                                                 },
// CHECK-NEXT:                                             )
// CHECK-NEXT:                                         },
// CHECK-NEXT:                                         const { 3 as i64 },
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                 },
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     s = [[V_2]];
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: core::simd::Simd<u32, 4>;
// CHECK-NEXT:     let __s0: core::simd::Simd<u32, 4> = s;
// CHECK-NEXT:     let __s1: core::simd::Simd<u32, 4> = a;
// CHECK-NEXT:     let __s2: core::simd::Simd<u32, 4> = b;
// CHECK-NEXT:     let __ret: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 __slate_intrinsic_c8e814aa1d284575(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                                             unsafe {
// CHECK-NEXT:                                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u32, 4>>(
// CHECK-NEXT:                                                     unsafe {
// CHECK-NEXT:                                                         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                                             __s0,
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
// CHECK-NEXT:                                                             __s1,
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
// CHECK-NEXT:                                                             __s2,
// CHECK-NEXT:                                                         )
// CHECK-NEXT:                                                     },
// CHECK-NEXT:                                                 )
// CHECK-NEXT:                                             },
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                     const { 0 as i64 },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_3]] = __ret;
// CHECK-NEXT:     s = [[V_3]];
// CHECK-NEXT:     return unsafe { vsm3partw2q_u32(vsm3partw1q_u32(s, a, b), a, b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn hmax(a: core::simd::Simd<f16, 8>) -> f16 {
// CHECK-NEXT:     let mut [[V_0]]: f16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f16 = 0 as f16;
// CHECK-NEXT:         let __s0: core::simd::Simd<f16, 8> = a;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             __slate_intrinsic_11a0910a532e4111(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                 __s0,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as f16;
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1]]: f16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f16 = 0 as f16;
// CHECK-NEXT:         let __s0: core::simd::Simd<f16, 8> = a;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             __slate_intrinsic_d9be625d140af83b(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                 __s0,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as f16;
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2]]: f16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f16 = 0 as f16;
// CHECK-NEXT:         let __s0: core::simd::Simd<f16, 8> = a;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             __slate_intrinsic_3aba8cf38bc7f089(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                 __s0,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         }) as f16;
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3]]: f16;
// CHECK-NEXT:     let mut __ret: f16 = 0 as f16;
// CHECK-NEXT:     let __s0: core::simd::Simd<f16, 4> = unsafe { vget_low_f16(a) };
// CHECK-NEXT:     __ret = (unsafe {
// CHECK-NEXT:         __slate_intrinsic_adaf42af71a0b841(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<f16, 4>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                             __s0,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as f16;
// CHECK-NEXT:     [[V_3]] = __ret;
// CHECK-NEXT:     [[V_0]] + [[V_1]] + [[V_2]] + [[V_3]]
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn p32(n: *const u8, v: core::simd::Simd<u32, 4>) {
// CHECK-NEXT:     let mut [[V_0]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = v;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32;
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = v;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u32;
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 4> = v;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[2] as u32;
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_3]]: u32;
// CHECK-NEXT:     let mut __ret: u32 = 0;
// CHECK-NEXT:     let __s0: core::simd::Simd<u32, 4> = v;
// CHECK-NEXT:     __ret = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u32, 4>, core::simd::Simd<i32, 4>>(__s0)
// CHECK-NEXT:     }[3] as u32;
// CHECK-NEXT:     [[V_3]] = __ret;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s %08x %08x %08x %08x\n".as_ptr(),
// CHECK-NEXT:             n,
// CHECK-NEXT:             [[V_0]],
// CHECK-NEXT:             [[V_1]],
// CHECK-NEXT:             [[V_2]],
// CHECK-NEXT:             [[V_3]],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,fhm,crc,lse,rcpc,dotprod,bti,fcma,jsconv,frintts,aes,sha2,sha3,sm4")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: core::simd::Simd<u32, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: core::simd::Simd<u32, 4> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut bs: [i8; 16] = [0i8; 16];
// CHECK-NEXT:     let mut cs: [i8; 16] = [0i8; 16];
// CHECK-NEXT:     for i in 0i32..16i32 {
// CHECK-NEXT:         bs[i as usize] = (i * 13 - 100) as i8;
// CHECK-NEXT:         cs[i as usize] = (i * 7 - 50) as i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     let acc: core::simd::Simd<i32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [1i32, -2i32, 3i32, -4i32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut [[V_1]]: core::simd::Simd<i8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: core::simd::Simd<i8, 16> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (bs.as_mut_ptr() as *mut core::simd::Simd<i8, 16>).read_unaligned()
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_2]]: core::simd::Simd<i8, 8>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: core::simd::Simd<i8, 8> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (cs.as_mut_ptr() as *mut core::simd::Simd<i8, 8>).read_unaligned()
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_2]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let d: core::simd::Simd<i32, 4> = unsafe { dotl(acc, [[V_1]], [[V_2]]) };
// CHECK-NEXT:     let mut [[V_3]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = d;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32) as i32;
// CHECK-NEXT:         [[V_3]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = d;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u32) as i32;
// CHECK-NEXT:         [[V_4]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = d;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[2] as u32) as i32;
// CHECK-NEXT:         [[V_5]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = d;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[3] as u32) as i32;
// CHECK-NEXT:         [[V_6]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("dotl {} {} {} {}", [[V_3]], [[V_4]], [[V_5]], [[V_6]]);
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: core::simd::Simd<u8, 8>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: core::simd::Simd<u8, 8> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<u8, 8>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 8>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (bs.as_mut_ptr() as *mut core::simd::Simd<u8, 8>)
// CHECK-NEXT:                                 .read_unaligned()
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_7]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: core::simd::Simd<u8, 16>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: core::simd::Simd<u8, 16> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<u8, 16>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<u8, 16>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (cs.as_mut_ptr() as *mut core::simd::Simd<u8, 16>)
// CHECK-NEXT:                                 .read_unaligned()
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_8]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let ud: core::simd::Simd<u32, 2> = unsafe {
// CHECK-NEXT:         udotl(
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = core::simd::Simd::from_array([5u32, 6u32]);
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             },
// CHECK-NEXT:             [[V_7]],
// CHECK-NEXT:             [[V_8]],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 2> = ud;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 2>, core::simd::Simd<i32, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32;
// CHECK-NEXT:         [[V_9]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u32, 2> = ud;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u32, 2>, core::simd::Simd<i32, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u32;
// CHECK-NEXT:         [[V_10]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"udotl %u %u\n".as_ptr(), [[V_9]], [[V_10]]) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let fa: core::simd::Simd<f32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [1.5f32, -2.0f32, 0.25f32, 4.0f32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let fb: core::simd::Simd<f32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [3.0f32, 0.5f32, -1.0f32, 2.0f32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let fc: core::simd::Simd<f32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [0.75f32, -3.0f32, 8.0f32, 1.25f32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let c1: core::simd::Simd<f32, 4> = unsafe { cmlal(fa, fb, vget_low_f32(fc)) };
// CHECK-NEXT:     let c2: core::simd::Simd<f32, 4> = unsafe { cmlal90(fa, fb, fc) };
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = c1;
// CHECK-NEXT:         __ret = __s0[0] as f32;
// CHECK-NEXT:         [[V_11]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = c1;
// CHECK-NEXT:         __ret = __s0[1] as f32;
// CHECK-NEXT:         [[V_12]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = c1;
// CHECK-NEXT:         __ret = __s0[2] as f32;
// CHECK-NEXT:         [[V_13]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = c1;
// CHECK-NEXT:         __ret = __s0[3] as f32;
// CHECK-NEXT:         [[V_14]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = c2;
// CHECK-NEXT:         __ret = __s0[0] as f32;
// CHECK-NEXT:         [[V_15]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_16:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = c2;
// CHECK-NEXT:         __ret = __s0[1] as f32;
// CHECK-NEXT:         [[V_16]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = c2;
// CHECK-NEXT:         __ret = __s0[2] as f32;
// CHECK-NEXT:         [[V_17]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_18:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = c2;
// CHECK-NEXT:         __ret = __s0[3] as f32;
// CHECK-NEXT:         [[V_18]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"cmla %g %g %g %g | %g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             [[V_11]] as f64,
// CHECK-NEXT:             [[V_12]] as f64,
// CHECK-NEXT:             [[V_13]] as f64,
// CHECK-NEXT:             [[V_14]] as f64,
// CHECK-NEXT:             [[V_15]] as f64,
// CHECK-NEXT:             [[V_16]] as f64,
// CHECK-NEXT:             [[V_17]] as f64,
// CHECK-NEXT:             [[V_18]] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "rdmh {} {} {}",
// CHECK-NEXT:         (unsafe { rdmh(100i16, 20000i16, 30000i16) }) as i32,
// CHECK-NEXT:         (unsafe { rdmh(32767i16, 32767i16, 32767i16) }) as i32,
// CHECK-NEXT:         (unsafe { rdmh(-5i16, -32768i16, -32768i16) }) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "rdms {} {}",
// CHECK-NEXT:         unsafe { rdms(7i32, 1073741824i32, 1073741824i32) },
// CHECK-NEXT:         unsafe { rdms(-2147483648i32, 2147483647i32, 2147483647i32) }
// CHECK-NEXT:     );
// CHECK-NEXT:     let a: core::simd::Simd<u32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [1732584193u32, 4023233417u32, 2562383102u32, 271733878u32],
// CHECK-NEXT:     );
// CHECK-NEXT:     let w: core::simd::Simd<u32, 4> = core::simd::Simd::from_array(
// CHECK-NEXT:         [1u32, 3735928559u32, 305419896u32, 4294967295u32],
// CHECK-NEXT:     );
// CHECK-NEXT:     unsafe { p32(c"sha1".as_ptr(), sha1(a, 3285377520u32, w)) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"sha1h %08x\n".as_ptr(), vsha1h_u32(3285377520u32))
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let x: core::simd::Simd<u64, 2> = unsafe {
// CHECK-NEXT:         xar(
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_2]] = core::simd::Simd::from_array(
// CHECK-NEXT:                     [81985529216486895u64, 18364758544493064720u64],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:             },
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_3]] = core::simd::Simd::from_array(
// CHECK-NEXT:                     [1229782938247303441u64, 9223372036854775809u64],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: u64;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u64 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u64, 2> = x;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u64;
// CHECK-NEXT:         [[V_19]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_20:__v[0-9]+]]: u64;
// CHECK-NEXT:     let mut __ret: u64 = 0;
// CHECK-NEXT:     let __s0: core::simd::Simd<u64, 2> = x;
// CHECK-NEXT:     __ret = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u64, 2>, core::simd::Simd<i64, 2>>(__s0)
// CHECK-NEXT:     }[1] as u64;
// CHECK-NEXT:     [[V_20]] = __ret;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"xar %016llx %016llx\n".as_ptr(), [[V_19]], [[V_20]]) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p32(
// CHECK-NEXT:             c"sm3".as_ptr(),
// CHECK-NEXT:             sm3(
// CHECK-NEXT:                 a,
// CHECK-NEXT:                 w,
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_4]] = core::simd::Simd::from_array(
// CHECK-NEXT:                         [3u32, 5u32, 7u32, 9u32],
// CHECK-NEXT:                     );
// CHECK-NEXT:                     core::ptr::addr_of_mut!([[SLATE_COMPOUND_4]])
// CHECK-NEXT:                 },
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let h: core::simd::Simd<f16, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:         [
// CHECK-NEXT:             1.5f64 as f16,
// CHECK-NEXT:             -2.0f64 as f16,
// CHECK-NEXT:             65504.0f64 as f16,
// CHECK-NEXT:             0.25f64 as f16,
// CHECK-NEXT:             -0.0f64 as f16,
// CHECK-NEXT:             3.0f64 as f16,
// CHECK-NEXT:             -65504.0f64 as f16,
// CHECK-NEXT:             7.0f64 as f16,
// CHECK-NEXT:         ],
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"hmax %g\n".as_ptr(), hmax(h) as f64) };
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
