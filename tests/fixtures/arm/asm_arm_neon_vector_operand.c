// { dg-options "-mfpu=neon" }
#include <stdio.h>

typedef float float32x4_t __attribute__((vector_size(16)));

__attribute__((target("neon")))
static float32x4_t add_via_w(float32x4_t lhs, float32x4_t rhs) {
  float32x4_t result;
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("vadd.f32 %q1, %q0, %q1" : "=w"(result) : "0"(lhs), "w"(rhs));
  // @rewrite-end
  // @lowering-end
  return result;
}

int main(void) {
  float32x4_t result = add_via_w((float32x4_t){1.0f, 2.0f, 3.0f, 4.0f},
                                 (float32x4_t){5.0f, 6.0f, 7.0f, 8.0f});
  printf("%.1f %.1f %.1f %.1f\n", result[0], result[1], result[2], result[3]);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-ARMV7-GNU-DAG: let {{__v[0-9]+}}: core::arch::arm::float32x4_t;
// LOWERING-ARMV7-GNU-DAG: unsafe {
// LOWERING-ARMV7-GNU-DAG:     core::arch::asm!("vadd.f32 {0}, {0}, {0}\n/* {1} */", inlateout(qreg) unsafe { std::mem::transmute::<[f32; 4], core::arch::arm::float32x4_t>({{arg[0-9]+}}) } => {{__v[0-9]+}}, in(qreg) unsafe { std::mem::transmute::<[f32; 4], core::arch::arm::float32x4_t>({{arg[0-9]+}}) });
// LOWERING-ARMV7-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-ARMV7-GNU-DAG: let {{__v[0-9]+}}: core::arch::arm::float32x4_t;
// REWRITES-ARMV7-GNU-DAG: unsafe {
// REWRITES-ARMV7-GNU-DAG:     core::arch::asm!("vadd.f32 {0}, {0}, {0}\n/* {1} */", inlateout(qreg) unsafe { std::mem::transmute::<[f32; 4], core::arch::arm::float32x4_t>({{arg[0-9]+}}) } => {{__v[0-9]+}}, in(qreg) unsafe { std::mem::transmute::<[f32; 4], core::arch::arm::float32x4_t>({{arg[0-9]+}}) });
// REWRITES-ARMV7-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
