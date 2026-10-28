#include <stdio.h>

typedef float float32x4_t __attribute__((vector_size(16)));

// @check-fn-begin
static float copy_via_w(float value) {
  float result;
  __asm__ volatile("fmov %s0, %s1" : "=w"(result) : "w"(value));
  return result;
}
// @check-fn-end

// @check-fn-begin
static float32x4_t copy_via_x(float32x4_t value) {
  float32x4_t result;
  __asm__ volatile("orr %0.16b, %1.16b, %1.16b" : "=x"(result) : "x"(value));
  return result;
}
// @check-fn-end

int main(void) {
  float32x4_t value = {2.0f, 3.0f, 5.0f, 7.0f};
  float32x4_t result = copy_via_x(value);
  printf("%.1f %.1f\n", copy_via_w(1.5f), result[3]);
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK-LABEL: {{^}}fn copy_via_w(mut value: f32) -> f32 {
// CHECK-NEXT:     let mut result: f32 = 0 as f32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "fmov {0:s}, {1:s}", lateout(vreg) [[T_0]], in (vreg) unsafe {
// CHECK-NEXT:             std::mem::transmute::< f32, u32 > (value) }, options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe { std::mem::transmute::<u32, f32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     result
// CHECK: {{^}}}
// CHECK-LABEL: {{^}}fn copy_via_x(mut value: std::simd::Simd<f32, 4>) -> std::simd::Simd<f32, 4> {
// CHECK-NEXT:     let mut result: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: core::arch::aarch64::uint8x16_t;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "orr {0:v}.16b, {1:v}.16b, {1:v}.16b", lateout(vreg_low16) [[T_0]], in
// CHECK-NEXT:             (vreg_low16) unsafe { std::mem::transmute::< std::simd::Simd < f32, 4 >,
// CHECK-NEXT:             core::arch::aarch64::uint8x16_t > (value) }, options(nostack,
// CHECK-NEXT:             preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         result = unsafe {
// CHECK-NEXT:             std::mem::transmute::<
// CHECK-NEXT:                 core::arch::aarch64::uint8x16_t,
// CHECK-NEXT:                 std::simd::Simd<f32, 4>,
// CHECK-NEXT:             >([[T_0]])
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     return result;
// CHECK: {{^}}}
// SLATE-FILECHECK-END
