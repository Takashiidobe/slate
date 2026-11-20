/* { dg-options "-march=armv8.2-a+dotprod" } */
#include <arm_neon.h>
#include <stdio.h>

static int32x4_t dot(int32x4_t acc, int8x16_t a, int8x16_t b) {
  __asm__("sdot %0.4s, %1.16b, %2.16b" : "+w"(acc) : "w"(a), "w"(b));
  return acc;
}

static unsigned crc(unsigned c, unsigned char v) {
  __asm__("crc32b %w0, %w0, %w1" : "+r"(c) : "r"(v));
  return c;
}

static unsigned (*pick(void))(unsigned, unsigned char) { return crc; }

int main(void) {
  int32x4_t r = dot(vdupq_n_s32(1), vdupq_n_s8(2), vdupq_n_s8(-3));
  unsigned c = 0xffffffffu;
  for (unsigned char v = 1; v < 5; v++)
    c = pick()(c, v);
  printf("%d %d %08x\n", vgetq_lane_s32(r, 0), vgetq_lane_s32(r, 3), c);
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-march=armv8.2-a+dotprod" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,dotprod,neon")]
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: unsafe fn vdupq_n_s8(__p0: i8) -> core::simd::Simd<i8, 16> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: core::simd::Simd<i8, 16> = unsafe {
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
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,dotprod,neon")]
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
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,dotprod")]
// CHECK-NEXT: unsafe fn dot(
// CHECK-NEXT:     mut acc: core::simd::Simd<i32, 4>,
// CHECK-NEXT:     mut a: core::simd::Simd<i8, 16>,
// CHECK-NEXT:     mut b: core::simd::Simd<i8, 16>,
// CHECK-NEXT: ) -> core::simd::Simd<i32, 4> {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: core::arch::aarch64::uint8x16_t;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "sdot {0:v}.4s, {1:v}.16b, {2:v}.16b",
// CHECK-NEXT:             inlateout(vreg) unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i32, 4>, core::arch::aarch64::uint8x16_t>(
// CHECK-NEXT:                     acc,
// CHECK-NEXT:                 )
// CHECK-NEXT:             } => [[T_0]],
// CHECK-NEXT:             in(vreg) unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::arch::aarch64::uint8x16_t>(
// CHECK-NEXT:                     a,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             in(vreg) unsafe {
// CHECK-NEXT:                 core::mem::transmute::<core::simd::Simd<i8, 16>, core::arch::aarch64::uint8x16_t>(
// CHECK-NEXT:                     b,
// CHECK-NEXT:                 )
// CHECK-NEXT:             },
// CHECK-NEXT:             options(pure, nomem, nostack, preserves_flags),
// CHECK-NEXT:         );
// CHECK-NEXT:         acc = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::arch::aarch64::uint8x16_t, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 [[T_0]],
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     return acc;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,dotprod")]
// CHECK-NEXT: #[unsafe(link_section = ".text.slate_distinct.main.crc")]
// CHECK-NEXT: unsafe extern "C-unwind" fn crc(mut c: u32, mut v: u8) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "crc32b {0:w}, {0:w}, {1:w}",
// CHECK-NEXT:             inlateout(reg) c as u32 => [[T_0]],
// CHECK-NEXT:             in(reg) (v as u8) as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, preserves_flags),
// CHECK-NEXT:         );
// CHECK-NEXT:         c = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     c
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,dotprod")]
// CHECK-NEXT: unsafe fn pick() -> Option<unsafe extern "C-unwind" fn(u32, u8) -> u32> {
// CHECK-NEXT:     Some(crc)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[target_feature(enable = "rdm,crc,lse,dotprod")]
// CHECK-NEXT: unsafe fn __slate_main() -> i32 {
// CHECK-NEXT:     let r: core::simd::Simd<i32, 4> = unsafe {
// CHECK-NEXT:         dot(vdupq_n_s32(1i32), vdupq_n_s8(2i8), vdupq_n_s8(-3i8))
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: u32 = 4294967295;
// CHECK-NEXT:     let mut v: u8 = 1;
// CHECK-NEXT:     while (v as i32) < 5 {
// CHECK-NEXT:         c = unsafe { pick().unwrap()(c, v) };
// CHECK-NEXT:         v = ((v as i32) + 1) as u8;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i32;
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut __ret: i32 = 0;
// CHECK-NEXT:         let __s0: core::simd::Simd<i32, 4> = r;
// CHECK-NEXT:         __ret = (unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(
// CHECK-NEXT:                 __s0,
// CHECK-NEXT:             )
// CHECK-NEXT:         }[0] as u32) as i32;
// CHECK-NEXT:         [[V_0]] = __ret;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i32;
// CHECK-NEXT:     let mut __ret: i32 = 0;
// CHECK-NEXT:     let __s0: core::simd::Simd<i32, 4> = r;
// CHECK-NEXT:     __ret = (unsafe {
// CHECK-NEXT:         core::mem::transmute::<core::simd::Simd<i32, 4>, core::simd::Simd<i32, 4>>(__s0)
// CHECK-NEXT:     }[3] as u32) as i32;
// CHECK-NEXT:     [[V_1]] = __ret;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"%d %d %08x\n".as_ptr(), [[V_0]], [[V_1]], c) };
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
