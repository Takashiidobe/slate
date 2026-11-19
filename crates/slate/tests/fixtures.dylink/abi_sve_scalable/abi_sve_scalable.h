#ifndef ABI_SVE_SCALABLE_H
#define ABI_SVE_SCALABLE_H

#include <arm_sve.h>

typedef svint32_t (*mul_add_fn)(svint32_t, svint32_t, int);
typedef svbool_t (*first_n_fn)(unsigned);
typedef svfloat64_t (*masked_scale_fn)(svbool_t, svfloat64_t, double);
typedef svint32_t (*lane_fn)(svint32_t);
typedef long (*apply_fn)(lane_fn, svint32_t);

#endif
