// { dg-options "-mfpu=neon" }
#include <stdio.h>

typedef float float32x4_t __attribute__((vector_size(16)));

__attribute__((target("neon")))
// @check-fn-begin
static float32x4_t add_via_w(float32x4_t lhs, float32x4_t rhs) {
  float32x4_t result;
  __asm__ volatile("vadd.f32 %q1, %q0, %q1" : "=w"(result) : "0"(lhs), "w"(rhs));
  return result;
}
// @check-fn-end

int main(void) {
  float32x4_t result = add_via_w((float32x4_t){1.0f, 2.0f, 3.0f, 4.0f},
                                 (float32x4_t){5.0f, 6.0f, 7.0f, 8.0f});
  printf("%.1f %.1f %.1f %.1f\n", result[0], result[1], result[2], result[3]);
  return 0;
}

// SLATE-FILECHECK-BEGIN armv7-unknown-linux-gnueabihf
// CHECK-LABEL: {{^}}unsafe fn add_via_w(
// CHECK-NEXT:     mut lhs: core::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut rhs: core::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> core::simd::Simd<f32, 4> {
// CHECK-NEXT:     let mut result: core::simd::Simd<f32, 4> = unsafe { core::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: core::arch::arm::uint8x16_t;
// CHECK-NEXT:         core::arch::asm!("vadd.f32 {0}, {0}, {0}\n@ {1}", inlateout(qreg) unsafe { core::mem::transmute::<core::simd::Simd<f32, 4>, core::arch::arm::uint8x16_t>(lhs) } => [[T_0]], in(qreg) unsafe { core::mem::transmute::<core::simd::Simd<f32, 4>, core::arch::arm::uint8x16_t>(rhs) }, options(nostack, preserves_flags));
// CHECK-NEXT:         result = unsafe {
// CHECK-NEXT:             core::mem::transmute::<core::arch::arm::uint8x16_t, core::simd::Simd<f32, 4>>([[T_0]])
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     return result;
// CHECK: {{^}}}
// SLATE-FILECHECK-END
