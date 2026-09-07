#include <stdio.h>

typedef float float32x4_t __attribute__((vector_size(16)));

static float copy_via_w(float value) {
  float result;
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("fmov %s0, %s1" : "=w"(result) : "w"(value));
  // @rewrite-end
  // @lowering-end
  return result;
}

static float32x4_t copy_via_x(float32x4_t value) {
  float32x4_t result;
  // @lowering-begin
  // @rewrite-begin
  __asm__ volatile("orr %0.16b, %1.16b, %1.16b" : "=x"(result) : "x"(value));
  // @rewrite-end
  // @lowering-end
  return result;
}

int main(void) {
  float32x4_t value = {2.0f, 3.0f, 5.0f, 7.0f};
  float32x4_t result = copy_via_x(value);
  printf("%.1f %.1f\n", copy_via_w(1.5f), result[3]);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: f32;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("fmov {0:s}, {1:s}", lateout(vreg) {{__v[0-9]+}}, in(vreg) {{arg[0-9]+}});
// LOWERING-AARCH64-GNU-DAG: }
// LOWERING-AARCH64-GNU-DAG: let {{__v[0-9]+}}: core::arch::aarch64::float32x4_t;
// LOWERING-AARCH64-GNU-DAG: unsafe {
// LOWERING-AARCH64-GNU-DAG:     core::arch::asm!("orr {0}.16b, {1}.16b, {1}.16b", lateout(vreg_low16) {{__v[0-9]+}}, in(vreg_low16) unsafe { std::mem::transmute::<[f32; 4], core::arch::aarch64::float32x4_t>({{arg[0-9]+}}) });
// LOWERING-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: f32;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("fmov {0:s}, {1:s}", lateout(vreg) {{__v[0-9]+}}, in(vreg) {{arg[0-9]+}});
// REWRITES-AARCH64-GNU-DAG: }
// REWRITES-AARCH64-GNU-DAG: let {{__v[0-9]+}}: core::arch::aarch64::float32x4_t;
// REWRITES-AARCH64-GNU-DAG: unsafe {
// REWRITES-AARCH64-GNU-DAG:     core::arch::asm!("orr {0}.16b, {1}.16b, {1}.16b", lateout(vreg_low16) {{__v[0-9]+}}, in(vreg_low16) unsafe { std::mem::transmute::<[f32; 4], core::arch::aarch64::float32x4_t>({{arg[0-9]+}}) });
// REWRITES-AARCH64-GNU-DAG: }
// SLATE-FILECHECK-END rewrites
