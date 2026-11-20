/* { dg-options "-march=armv8.2-a+fp16" } */
#include <arm_neon.h>
#include <stdio.h>

static float16x8_t add(float16x8_t a, float16x8_t b) { return vaddq_f16(a, b); }
static float16x8_t mul(float16x8_t a, float16x8_t b) { return vmulq_f16(a, b); }
static float16x8_t fma_(float16x8_t a, float16x8_t b, float16x8_t c) { return vfmaq_f16(a, b, c); }
static float16x8_t mx(float16x8_t a, float16x8_t b) { return vmaxq_f16(a, b); }
static float16x8_t mnm(float16x8_t a, float16x8_t b) { return vminnmq_f16(a, b); }
static float16x8_t ab(float16x8_t a) { return vabsq_f16(a); }
static float16x8_t rn(float16x8_t a) { return vrndnq_f16(a); }
static float16x4_t sq(float16x4_t a) { return vsqrt_f16(a); }
static uint16x8_t gt(float16x8_t a, float16x8_t b) { return vcgtq_f16(a, b); }

static void show(const char *name, float16x8_t v) {
  float16_t out[8];
  vst1q_f16(out, v);
  printf("%s", name);
  for (int i = 0; i < 8; i++)
    printf(" %g", (double)out[i]);
  printf("\n");
}

int main(void) {
  float16_t in[8] = {1.5, -2.25, 0.0, -0.0, 65504.0, 6.0e-8, 2.5, -3.5};
  float16x8_t a = vld1q_f16(in);
  float16x8_t b = vdupq_n_f16(2.0);
  float16x8_t n = vsetq_lane_f16(__builtin_nanf16(""), b, 5);
  show("add", add(a, b));
  show("mul", mul(a, b));
  show("fma", fma_(a, a, b));
  show("max", mx(a, n));
  show("minnm", mnm(a, n));
  show("abs", ab(a));
  show("rndn", rn(a));
  float16x4_t s = sq(vget_low_f16(a));
  printf("sqrt %g %g\n", (double)vget_lane_f16(s, 0), (double)vget_lane_f16(s, 2));
  uint16x8_t g = gt(a, b);
  printf("gt %x %x %x\n", vgetq_lane_u16(g, 0), vgetq_lane_u16(g, 4), vgetq_lane_u16(g, 6));
  float32x4_t w = vcvt_f32_f16(vget_high_f16(a));
  printf("wide %g %g %g %g\n", vgetq_lane_f32(w, 0), vgetq_lane_f32(w, 1), vgetq_lane_f32(w, 2), vgetq_lane_f32(w, 3));
  float16x4_t h = vcvt_f16_f32((float32x4_t){1.0f / 3.0f, 70000.0f, -1e-9f, 2049.0f});
  printf("narrow %g %g %g %g\n", (double)vget_lane_f16(h, 0), (double)vget_lane_f16(h, 1), (double)vget_lane_f16(h, 2), (double)vget_lane_f16(h, 3));
  int32x4_t lanes = vsetq_lane_s32(-7, vdupq_n_s32(1), 3);
  printf("set %d %d\n", vgetq_lane_s32(lanes, 0), vgetq_lane_s32(lanes, 3));
  printf("bits %04x\n", vgetq_lane_u16(vreinterpretq_u16_f16(add(a, b)), 0));
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
// CHECK-NEXT: //! { dg-options "-march=armv8.2-a+fp16" }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fminnm.v8f16"]
// CHECK-NEXT:     fn __slate_intrinsic_8b3ed422042b512c(
// CHECK-NEXT:         [[ARG_0:arg[0-9]+]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:         [[ARG_1:arg[0-9]+]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<f16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.roundeven.v8f16"]
// CHECK-NEXT:     fn __slate_intrinsic_8e22bf589d4bf710(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<f16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.sqrt.v4f16"]
// CHECK-NEXT:     fn __slate_intrinsic_bb251146fde0ebd6(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f16, 4>,
// CHECK-NEXT:     ) -> core::simd::Simd<f16, 4>;
// CHECK-NEXT:     #[link_name = "llvm.fma.v8f16"]
// CHECK-NEXT:     fn __slate_intrinsic_cb52361392f5d6d1(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:         [[ARG_2:arg[0-9]+]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<f16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.aarch64.neon.fmax.v8f16"]
// CHECK-NEXT:     fn __slate_intrinsic_dab3412d98a6e580(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:         [[ARG_1]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<f16, 8>;
// CHECK-NEXT:     #[link_name = "llvm.fabs.v8f16"]
// CHECK-NEXT:     fn __slate_intrinsic_eb9000da2d7b20e9(
// CHECK-NEXT:         [[ARG_0]]: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     ) -> core::simd::Simd<f16, 8>;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vabsq_f16(__p0: core::simd::Simd<f16, 8>) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_eb9000da2d7b20e9(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vaddq_f16(
// CHECK-NEXT:     __p0: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 8> = __p0 + __p1;
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vcgtq_f16(
// CHECK-NEXT:     __p0: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<u16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<u16, 8>>(
// CHECK-NEXT:             {
// CHECK-NEXT:                 let [[T_0:__t[0-9]+]]: core::simd::Simd<f16, 8> = __p0;
// CHECK-NEXT:                 let [[T_1:__t[0-9]+]] = [[T_0]].to_array();
// CHECK-NEXT:                 let [[T_2:__t[0-9]+]]: core::simd::Simd<f16, 8> = __p1;
// CHECK-NEXT:                 let [[T_3:__t[0-9]+]] = [[T_2]].to_array();
// CHECK-NEXT:                 let [[T_4:__t[0-9]+]]: core::simd::Simd<i16, 8> = core::simd::Simd::from_array(
// CHECK-NEXT:                     core::array::from_fn(
// CHECK-NEXT:                         |__lane| -(([[T_1]][__lane] > [[T_3]][__lane]) as i16),
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 );
// CHECK-NEXT:                 [[T_4]]
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vfmaq_f16(
// CHECK-NEXT:     __p0: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     __p2: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_cb52361392f5d6d1(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p2,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vmaxq_f16(
// CHECK-NEXT:     __p0: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_dab3412d98a6e580(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vmulq_f16(
// CHECK-NEXT:     __p0: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 8> = __p0 * __p1;
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdupq_n_s32(__p0: i32) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: core::simd::Simd<i32, 4> = unsafe {
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vget_high_f16(mut __p0: core::simd::Simd<f16, 8>) -> core::simd::Simd<f16, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 4> = core::simd::simd_swizzle!(
// CHECK-NEXT:         __p0,
// CHECK-NEXT:         __p0,
// CHECK-NEXT:         [4, 5, 6, 7]
// CHECK-NEXT:     );
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vcvt_f16_f32(__p0: core::simd::Simd<f32, 4>) -> core::simd::Simd<f16, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<f16, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                     core::simd::num::SimdFloat::cast::<f16>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                         __p0,
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                 },
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vcvt_f32_f16(__p0: core::simd::Simd<f16, 4>) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f32, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f32, 4>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     core::simd::num::SimdFloat::cast::<f32>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<f16, 4>>(
// CHECK-NEXT:                                 unsafe {
// CHECK-NEXT:                                     core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                                         __p0,
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                 },
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vrndnq_f16(__p0: core::simd::Simd<f16, 8>) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_8e22bf589d4bf710(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vminnmq_f16(
// CHECK-NEXT:     __p0: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     __p1: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_8b3ed422042b512c(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p0,
// CHECK-NEXT:                                         )
// CHECK-NEXT:                                     },
// CHECK-NEXT:                                 )
// CHECK-NEXT:                             },
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                             __p1,
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vsqrt_f16(__p0: core::simd::Simd<f16, 4>) -> core::simd::Simd<f16, 4> {
// CHECK-NEXT:     let __ret: core::simd::Simd<f16, 4> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<f16, 4>>(
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i8, 8>>(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         __slate_intrinsic_bb251146fde0ebd6(
// CHECK-NEXT:                             unsafe {
// CHECK-NEXT:                                 core::mem::transmute::<core::simd::Simd<i8, 8>, core::simd::Simd<f16, 4>>(
// CHECK-NEXT:                                     unsafe {
// CHECK-NEXT:                                         core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i8, 8>>(
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
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vreinterpretq_u16_f16(
// CHECK-NEXT:     __p0: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<u16, 8> {
// CHECK-NEXT:     let __ret: core::simd::Simd<u16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<u16, 8>>(__p0)
// CHECK-NEXT:     };
// CHECK-NEXT:     return __ret;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn add(
// CHECK-NEXT:     a: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     b: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     return unsafe { vaddq_f16(a, b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn mul(
// CHECK-NEXT:     a: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     b: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     return unsafe { vmulq_f16(a, b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn fma_(
// CHECK-NEXT:     a: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     b: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     c: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     return unsafe { vfmaq_f16(a, b, c) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn mx(
// CHECK-NEXT:     a: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     b: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     return unsafe { vmaxq_f16(a, b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn mnm(
// CHECK-NEXT:     a: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     b: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     return unsafe { vminnmq_f16(a, b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn ab(a: core::simd::Simd<f16, 8>) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     return unsafe { vabsq_f16(a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn rn(a: core::simd::Simd<f16, 8>) -> core::simd::Simd<f16, 8> {
// CHECK-NEXT:     return unsafe { vrndnq_f16(a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn sq(a: core::simd::Simd<f16, 4>) -> core::simd::Simd<f16, 4> {
// CHECK-NEXT:     return unsafe { vsqrt_f16(a) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn gt(
// CHECK-NEXT:     a: core::simd::Simd<f16, 8>,
// CHECK-NEXT:     b: core::simd::Simd<f16, 8>,
// CHECK-NEXT: ) -> core::simd::Simd<u16, 8> {
// CHECK-NEXT:     return unsafe { vcgtq_f16(a, b) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn show(name: *const u8, __s1: core::simd::Simd<f16, 8>) {
// CHECK-NEXT:     let mut out: [f16; 8] = [0 as f16; 8];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (out.as_mut_ptr() as *mut core::simd::Simd<f16, 8>)
// CHECK-NEXT:             .write_unaligned(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                                 __s1,
// CHECK-NEXT:                             )
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             );
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%s".as_ptr(), name) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { slate_support::printf(c" %g".as_ptr(), out[i as usize] as f64) };
// CHECK-NEXT:         unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("");
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,fp16,crc,lse")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: core::simd::Simd<f32, 4> = unsafe {
// CHECK-NEXT:         core::mem::zeroed()
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut r#in: [f16; 8] = [
// CHECK-NEXT:         1.5f64 as f16,
// CHECK-NEXT:         -2.25f64 as f16,
// CHECK-NEXT:         0.0f64 as f16,
// CHECK-NEXT:         -0.0f64 as f16,
// CHECK-NEXT:         65504.0f64 as f16,
// CHECK-NEXT:         6e-8f64 as f16,
// CHECK-NEXT:         2.5f64 as f16,
// CHECK-NEXT:         -3.5f64 as f16,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: core::simd::Simd<f16, 8>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __ret: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i8, 16>, core::simd::Simd<f16, 8>>(
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i8, 16>>(
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (r#in.as_mut_ptr() as *mut core::simd::Simd<f16, 8>)
// CHECK-NEXT:                                 .read_unaligned()
// CHECK-NEXT:                         },
// CHECK-NEXT:                     )
// CHECK-NEXT:                 },
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let a: core::simd::Simd<f16, 8> = [[V_0]];
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: core::simd::Simd<f16, 8>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __s0: f16 = 2.0f64 as f16;
// CHECK-NEXT:         let __ret: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = core::simd::Simd::from_array(
// CHECK-NEXT:                     [__s0, __s0, __s0, __s0, __s0, __s0, __s0, __s0],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_1]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let b: core::simd::Simd<f16, 8> = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: core::simd::Simd<f16, 8>;
// CHECK-NEXT:     let __s0_871: f16 = f16::from_bits(0x7e00);
// CHECK-NEXT:     let __s1_871: core::simd::Simd<f16, 8> = b;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: core::simd::Simd<i16, 8>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __s0: i16 = unsafe { core::mem::transmute::<f16, i16>(__s0_871) };
// CHECK-NEXT:         let __s1: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f16, 8>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                 __s1_871,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         let __ret: core::simd::Simd<i16, 8> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                             __s1,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::splat(__s0),
// CHECK-NEXT:                     [0, 1, 2, 3, 4, 13, 6, 7]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_3]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let __ret_871: core::simd::Simd<f16, 8> = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i16, 8>, core::simd::Simd<f16, 8>>([[V_3]])
// CHECK-NEXT:     };
// CHECK-NEXT:     [[V_2]] = __ret_871;
// CHECK-NEXT:     let n: core::simd::Simd<f16, 8> = [[V_2]];
// CHECK-NEXT:     unsafe { show(c"add".as_ptr(), add(a, b)) };
// CHECK-NEXT:     unsafe { show(c"mul".as_ptr(), mul(a, b)) };
// CHECK-NEXT:     unsafe { show(c"fma".as_ptr(), fma_(a, a, b)) };
// CHECK-NEXT:     unsafe { show(c"max".as_ptr(), mx(a, n)) };
// CHECK-NEXT:     unsafe { show(c"minnm".as_ptr(), mnm(a, n)) };
// CHECK-NEXT:     unsafe { show(c"abs".as_ptr(), ab(a)) };
// CHECK-NEXT:     unsafe { show(c"rndn".as_ptr(), rn(a)) };
// CHECK-NEXT:     let s: core::simd::Simd<f16, 4> = unsafe { sq(vget_low_f16(a)) };
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: f16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret_847: f16 = 0 as f16;
// CHECK-NEXT:         let __s0_847: core::simd::Simd<f16, 4> = s;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i16;
// CHECK-NEXT:         let mut __ret: i16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0_847,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u16) as i16;
// CHECK-NEXT:         [[V_5]] = __ret;
// CHECK-NEXT:         __ret_847 = unsafe { core::mem::transmute::<i16, f16>([[V_5]]) };
// CHECK-NEXT:         [[V_4]] = __ret_847;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_6:__v[0-9]+]]: f16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret_847: f16 = 0 as f16;
// CHECK-NEXT:         let __s0_847: core::simd::Simd<f16, 4> = s;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i16;
// CHECK-NEXT:         let mut __ret: i16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0_847,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[2] as u16) as i16;
// CHECK-NEXT:         [[V_7]] = __ret;
// CHECK-NEXT:         __ret_847 = unsafe { core::mem::transmute::<i16, f16>([[V_7]]) };
// CHECK-NEXT:         [[V_6]] = __ret_847;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"sqrt %g %g\n".as_ptr(), [[V_4]] as f64, [[V_6]] as f64) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let g: core::simd::Simd<u16, 8> = unsafe { gt(a, b) };
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: u16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u16, 8> = g;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u16, 8>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u16;
// CHECK-NEXT:         [[V_8]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_9:__v[0-9]+]]: u16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u16, 8> = g;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u16, 8>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[4] as u16;
// CHECK-NEXT:         [[V_9]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_10:__v[0-9]+]]: u16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: u16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<u16, 8> = g;
// CHECK-NEXT:         __ret = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<u16, 8>, core::simd::Simd<i16, 8>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[6] as u16;
// CHECK-NEXT:         [[V_10]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"gt %x %x %x\n".as_ptr(),
// CHECK-NEXT:             [[V_8]] as i32,
// CHECK-NEXT:             [[V_9]] as i32,
// CHECK-NEXT:             [[V_10]] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let w: core::simd::Simd<f32, 4> = unsafe { vcvt_f32_f16(vget_high_f16(a)) };
// CHECK-NEXT:     let mut [[V_11:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = w;
// CHECK-NEXT:         __ret = __s0[0] as f32;
// CHECK-NEXT:         [[V_11]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_12:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = w;
// CHECK-NEXT:         __ret = __s0[1] as f32;
// CHECK-NEXT:         [[V_12]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_13:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = w;
// CHECK-NEXT:         __ret = __s0[2] as f32;
// CHECK-NEXT:         [[V_13]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_14:__v[0-9]+]]: f32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: f32 = 0 as f32;
// CHECK-NEXT:         let __s0: core::simd::Simd<f32, 4> = w;
// CHECK-NEXT:         __ret = __s0[3] as f32;
// CHECK-NEXT:         [[V_14]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"wide %g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             [[V_11]] as f64,
// CHECK-NEXT:             [[V_12]] as f64,
// CHECK-NEXT:             [[V_13]] as f64,
// CHECK-NEXT:             [[V_14]] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let h: core::simd::Simd<f16, 4> = unsafe {
// CHECK-NEXT:         vcvt_f16_f32(
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_2]] = core::simd::Simd::from_array(
// CHECK-NEXT:                     [1.0f32 / 3.0f32, 70000.0f32, -1e-9f32, 2049.0f32],
// CHECK-NEXT:                 );
// CHECK-NEXT:                 core::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:             },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_15:__v[0-9]+]]: f16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret_847: f16 = 0 as f16;
// CHECK-NEXT:         let __s0_847: core::simd::Simd<f16, 4> = h;
// CHECK-NEXT:         let mut [[V_16:__v[0-9]+]]: i16;
// CHECK-NEXT:         let mut __ret: i16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0_847,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u16) as i16;
// CHECK-NEXT:         [[V_16]] = __ret;
// CHECK-NEXT:         __ret_847 = unsafe { core::mem::transmute::<i16, f16>([[V_16]]) };
// CHECK-NEXT:         [[V_15]] = __ret_847;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_17:__v[0-9]+]]: f16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret_847: f16 = 0 as f16;
// CHECK-NEXT:         let __s0_847: core::simd::Simd<f16, 4> = h;
// CHECK-NEXT:         let mut [[V_18:__v[0-9]+]]: i16;
// CHECK-NEXT:         let mut __ret: i16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0_847,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[1] as u16) as i16;
// CHECK-NEXT:         [[V_18]] = __ret;
// CHECK-NEXT:         __ret_847 = unsafe { core::mem::transmute::<i16, f16>([[V_18]]) };
// CHECK-NEXT:         [[V_17]] = __ret_847;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_19:__v[0-9]+]]: f16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret_847: f16 = 0 as f16;
// CHECK-NEXT:         let __s0_847: core::simd::Simd<f16, 4> = h;
// CHECK-NEXT:         let mut [[V_20:__v[0-9]+]]: i16;
// CHECK-NEXT:         let mut __ret: i16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0_847,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[2] as u16) as i16;
// CHECK-NEXT:         [[V_20]] = __ret;
// CHECK-NEXT:         __ret_847 = unsafe { core::mem::transmute::<i16, f16>([[V_20]]) };
// CHECK-NEXT:         [[V_19]] = __ret_847;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_21:__v[0-9]+]]: f16;
// CHECK-NEXT:     let mut __ret_847: f16 = 0 as f16;
// CHECK-NEXT:     let __s0_847: core::simd::Simd<f16, 4> = h;
// CHECK-NEXT:     let mut [[V_22:__v[0-9]+]]: i16;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i16 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i16, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<f16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0_847,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i16, 4>, core::simd::Simd<i16, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[3] as u16) as i16;
// CHECK-NEXT:         [[V_22]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     __ret_847 = unsafe { core::mem::transmute::<i16, f16>([[V_22]]) };
// CHECK-NEXT:     [[V_21]] = __ret_847;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"narrow %g %g %g %g\n".as_ptr(),
// CHECK-NEXT:             [[V_15]] as f64,
// CHECK-NEXT:             [[V_17]] as f64,
// CHECK-NEXT:             [[V_19]] as f64,
// CHECK-NEXT:             [[V_21]] as f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut [[V_23:__v[0-9]+]]: core::simd::Simd<i32, 4>;
// CHECK-NEXT:     {
// CHECK-NEXT:         let __s0: i32 = -7;
// CHECK-NEXT:         let __s1: core::simd::Simd<i32, 4> = unsafe { vdupq_n_s32(1i32) };
// CHECK-NEXT:         let __ret: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 core::simd::simd_swizzle!(
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                             __s1,
// CHECK-NEXT:                         )
// CHECK-NEXT:                     },
// CHECK-NEXT:                     core::simd::Simd::splat(__s0),
// CHECK-NEXT:                     [0, 1, 2, 7]
// CHECK-NEXT:                 ),
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         [[V_23]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let lanes: core::simd::Simd<i32, 4> = [[V_23]];
// CHECK-NEXT:     let mut [[V_24:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = lanes;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32) as i32;
// CHECK-NEXT:         [[V_24]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_25:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = lanes;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[3] as u32) as i32;
// CHECK-NEXT:         [[V_25]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("set {} {}", [[V_24]], [[V_25]]);
// CHECK-NEXT:     let mut [[V_26:__v[0-9]+]]: u16;
// CHECK-NEXT:     let mut __ret: u16 = 0;
// CHECK-NEXT:     let __s0: core::simd::Simd<u16, 8> = unsafe { vreinterpretq_u16_f16(add(a, b)) };
// CHECK-NEXT:     __ret = unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<u16, 8>, core::simd::Simd<i16, 8>>(__s0)
// CHECK-NEXT:     }[0] as u16;
// CHECK-NEXT:     [[V_26]] = __ret;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"bits %04x\n".as_ptr(), [[V_26]] as i32) };
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
