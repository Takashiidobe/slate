#include "dylink.h"
#include "../abi_sve_scalable.h"

EXPORT svint32_t sve_mul_add(svint32_t a, svint32_t b, int bias) {
  return svadd_n_s32_x(svptrue_b32(), svmul_s32_x(svptrue_b32(), a, b), bias);
}

EXPORT svbool_t sve_first_n(unsigned n) { return svwhilelt_b32_u32(0, n); }

EXPORT svfloat64_t sve_masked_scale(svbool_t pg, svfloat64_t v, double k) {
  return svmul_n_f64_m(pg, v, k);
}

EXPORT long sve_apply(lane_fn f, svint32_t v) {
  return svaddv_s32(svptrue_b32(), f(f(v)));
}
