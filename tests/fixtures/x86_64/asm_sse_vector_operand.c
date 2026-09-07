#include <stdio.h>

typedef float float32x4_t __attribute__((vector_size(16)));

static float32x4_t add_via_x(float32x4_t lhs, float32x4_t rhs) {
  float32x4_t result;
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("addps %1, %0" : "=x"(result) : "0"(lhs), "x"(rhs));
  // @rewrite-end
  // @lowering-end
  return result;
}

int main(void) {
  float32x4_t result = add_via_x((float32x4_t){1.0f, 2.0f, 3.0f, 4.0f},
                                 (float32x4_t){5.0f, 6.0f, 7.0f, 8.0f});
  printf("%.1f %.1f %.1f %.1f\n", result[0], result[1], result[2], result[3]);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-X86_64-GNU-DAG: let {{__v[0-9]+}}: core::arch::x86_64::__m128;
// LOWERING-X86_64-GNU-DAG: unsafe {
// LOWERING-X86_64-GNU-DAG:     core::arch::asm!("addps {0}, {0}\n/* {1} */", inlateout(xmm_reg) unsafe { std::mem::transmute::<[f32; 4], core::arch::x86_64::__m128>({{arg[0-9]+}}) } => {{__v[0-9]+}}, in(xmm_reg) unsafe { std::mem::transmute::<[f32; 4], core::arch::x86_64::__m128>({{arg[0-9]+}}) }, options(att_syntax));
// LOWERING-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-X86_64-GNU-DAG: let {{__v[0-9]+}}: core::arch::x86_64::__m128;
// REWRITES-X86_64-GNU-DAG: unsafe {
// REWRITES-X86_64-GNU-DAG:     core::arch::asm!("addps {0}, {0}\n/* {1} */", inlateout(xmm_reg) unsafe { std::mem::transmute::<[f32; 4], core::arch::x86_64::__m128>({{arg[0-9]+}}) } => {{__v[0-9]+}}, in(xmm_reg) unsafe { std::mem::transmute::<[f32; 4], core::arch::x86_64::__m128>({{arg[0-9]+}}) }, options(att_syntax));
// REWRITES-X86_64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
