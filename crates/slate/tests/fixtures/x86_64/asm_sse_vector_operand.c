#include <stdio.h>

typedef float float32x4_t __attribute__((vector_size(16)));

// @check-fn-begin
static float32x4_t add_via_x(float32x4_t lhs, float32x4_t rhs) {
  float32x4_t result;
  __asm__ volatile("addps %1, %0" : "=x"(result) : "0"(lhs), "x"(rhs));
  return result;
}
// @check-fn-end

int main(void) {
  float32x4_t result = add_via_x((float32x4_t){1.0f, 2.0f, 3.0f, 4.0f},
                                 (float32x4_t){5.0f, 6.0f, 7.0f, 8.0f});
  printf("%.1f %.1f %.1f %.1f\n", result[0], result[1], result[2], result[3]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn add_via_x(
// CHECK-NEXT:     mut lhs: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     mut rhs: std::simd::Simd<f32, 4>,
// CHECK-NEXT: ) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let mut result: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: core::arch::x86_64::__m128i;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addps {0}, {0}\n# {1}", inlateout(xmm_reg) unsafe { std::mem::transmute::<
// CHECK-NEXT:             std::simd::Simd < f32, 4 >, core::arch::x86_64::__m128i > (lhs) } => [[T_0]], in
// CHECK-NEXT:             (xmm_reg) unsafe { std::mem::transmute::< std::simd::Simd < f32, 4 >,
// CHECK-NEXT:             core::arch::x86_64::__m128i > (rhs) }, options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 core::arch::x86_64::__m128i,
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:             >([[T_0]])
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     return result;
// CHECK: {{^}}}
// SLATE-FILECHECK-END
