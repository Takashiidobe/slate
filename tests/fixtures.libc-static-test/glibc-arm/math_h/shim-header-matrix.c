#include <math.h>

extern int slate_oracle_signgam;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_signgam), __typeof__(signgam)), "signgam object type differs from oracle");

static __typeof__(signgam) *const slate_reference_signgam = &signgam;

typedef float slate_oracle_typedef_float_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_float_t, float_t), "typedef float_t differs from oracle");

#ifndef FP_ILOGB0
#error "math.h:FP_ILOGB0 macro is missing from libc-shim"
#endif

#ifndef FP_ILOGBNAN
#error "math.h:FP_ILOGBNAN macro is missing from libc-shim"
#endif

#ifndef FP_INFINITE
#error "math.h:FP_INFINITE macro is missing from libc-shim"
#endif

#ifndef FP_INT_DOWNWARD
#error "math.h:FP_INT_DOWNWARD macro is missing from libc-shim"
#endif

#ifndef FP_INT_TONEAREST
#error "math.h:FP_INT_TONEAREST macro is missing from libc-shim"
#endif

#ifndef FP_INT_TONEARESTFROMZERO
#error "math.h:FP_INT_TONEARESTFROMZERO macro is missing from libc-shim"
#endif

#ifndef FP_INT_TOWARDZERO
#error "math.h:FP_INT_TOWARDZERO macro is missing from libc-shim"
#endif

#ifndef FP_INT_UPWARD
#error "math.h:FP_INT_UPWARD macro is missing from libc-shim"
#endif

#ifndef FP_LLOGB0
#error "math.h:FP_LLOGB0 macro is missing from libc-shim"
#endif

#ifndef FP_LLOGBNAN
#error "math.h:FP_LLOGBNAN macro is missing from libc-shim"
#endif

#ifndef FP_NAN
#error "math.h:FP_NAN macro is missing from libc-shim"
#endif

#ifndef FP_NORMAL
#error "math.h:FP_NORMAL macro is missing from libc-shim"
#endif

#ifndef FP_SUBNORMAL
#error "math.h:FP_SUBNORMAL macro is missing from libc-shim"
#endif

#ifndef FP_ZERO
#error "math.h:FP_ZERO macro is missing from libc-shim"
#endif

#ifndef HUGE_VAL
#error "math.h:HUGE_VAL macro is missing from libc-shim"
#endif

#ifndef HUGE_VALF
#error "math.h:HUGE_VALF macro is missing from libc-shim"
#endif

#ifndef HUGE_VALL
#error "math.h:HUGE_VALL macro is missing from libc-shim"
#endif

#ifndef HUGE_VAL_F32
#error "math.h:HUGE_VAL_F32 macro is missing from libc-shim"
#endif

#ifndef HUGE_VAL_F32X
#error "math.h:HUGE_VAL_F32X macro is missing from libc-shim"
#endif

#ifndef HUGE_VAL_F64
#error "math.h:HUGE_VAL_F64 macro is missing from libc-shim"
#endif

#ifndef INFINITY
#error "math.h:INFINITY macro is missing from libc-shim"
#endif

#ifndef MATH_ERREXCEPT
#error "math.h:MATH_ERREXCEPT macro is missing from libc-shim"
#endif

#ifndef MATH_ERRNO
#error "math.h:MATH_ERRNO macro is missing from libc-shim"
#endif

#ifndef MAXFLOAT
#error "math.h:MAXFLOAT macro is missing from libc-shim"
#endif

#ifndef M_1_PI
#error "math.h:M_1_PI macro is missing from libc-shim"
#endif

#ifndef M_1_PIf
#error "math.h:M_1_PIf macro is missing from libc-shim"
#endif

#ifndef M_1_PIf32
#error "math.h:M_1_PIf32 macro is missing from libc-shim"
#endif

#ifndef M_1_PIf32x
#error "math.h:M_1_PIf32x macro is missing from libc-shim"
#endif

#ifndef M_1_PIf64
#error "math.h:M_1_PIf64 macro is missing from libc-shim"
#endif

#ifndef M_1_PIl
#error "math.h:M_1_PIl macro is missing from libc-shim"
#endif

#ifndef M_2_PI
#error "math.h:M_2_PI macro is missing from libc-shim"
#endif

#ifndef M_2_PIf
#error "math.h:M_2_PIf macro is missing from libc-shim"
#endif

#ifndef M_2_PIf32
#error "math.h:M_2_PIf32 macro is missing from libc-shim"
#endif

#ifndef M_2_PIf32x
#error "math.h:M_2_PIf32x macro is missing from libc-shim"
#endif

#ifndef M_2_PIf64
#error "math.h:M_2_PIf64 macro is missing from libc-shim"
#endif

#ifndef M_2_PIl
#error "math.h:M_2_PIl macro is missing from libc-shim"
#endif

#ifndef M_2_SQRTPI
#error "math.h:M_2_SQRTPI macro is missing from libc-shim"
#endif

#ifndef M_2_SQRTPIf
#error "math.h:M_2_SQRTPIf macro is missing from libc-shim"
#endif

#ifndef M_2_SQRTPIf32
#error "math.h:M_2_SQRTPIf32 macro is missing from libc-shim"
#endif

#ifndef M_2_SQRTPIf32x
#error "math.h:M_2_SQRTPIf32x macro is missing from libc-shim"
#endif

#ifndef M_2_SQRTPIf64
#error "math.h:M_2_SQRTPIf64 macro is missing from libc-shim"
#endif

#ifndef M_2_SQRTPIl
#error "math.h:M_2_SQRTPIl macro is missing from libc-shim"
#endif

#ifndef M_E
#error "math.h:M_E macro is missing from libc-shim"
#endif

#ifndef M_Ef
#error "math.h:M_Ef macro is missing from libc-shim"
#endif

#ifndef M_Ef32
#error "math.h:M_Ef32 macro is missing from libc-shim"
#endif

#ifndef M_Ef32x
#error "math.h:M_Ef32x macro is missing from libc-shim"
#endif

#ifndef M_Ef64
#error "math.h:M_Ef64 macro is missing from libc-shim"
#endif

#ifndef M_El
#error "math.h:M_El macro is missing from libc-shim"
#endif

#ifndef M_LN10
#error "math.h:M_LN10 macro is missing from libc-shim"
#endif

#ifndef M_LN10f
#error "math.h:M_LN10f macro is missing from libc-shim"
#endif

#ifndef M_LN10f32
#error "math.h:M_LN10f32 macro is missing from libc-shim"
#endif

#ifndef M_LN10f32x
#error "math.h:M_LN10f32x macro is missing from libc-shim"
#endif

#ifndef M_LN10f64
#error "math.h:M_LN10f64 macro is missing from libc-shim"
#endif

#ifndef M_LN10l
#error "math.h:M_LN10l macro is missing from libc-shim"
#endif

#ifndef M_LN2
#error "math.h:M_LN2 macro is missing from libc-shim"
#endif

#ifndef M_LN2f
#error "math.h:M_LN2f macro is missing from libc-shim"
#endif

#ifndef M_LN2f32
#error "math.h:M_LN2f32 macro is missing from libc-shim"
#endif

#ifndef M_LN2f32x
#error "math.h:M_LN2f32x macro is missing from libc-shim"
#endif

#ifndef M_LN2f64
#error "math.h:M_LN2f64 macro is missing from libc-shim"
#endif

#ifndef M_LN2l
#error "math.h:M_LN2l macro is missing from libc-shim"
#endif

#ifndef M_LOG10E
#error "math.h:M_LOG10E macro is missing from libc-shim"
#endif

#ifndef M_LOG10Ef
#error "math.h:M_LOG10Ef macro is missing from libc-shim"
#endif

#ifndef M_LOG10Ef32
#error "math.h:M_LOG10Ef32 macro is missing from libc-shim"
#endif

#ifndef M_LOG10Ef32x
#error "math.h:M_LOG10Ef32x macro is missing from libc-shim"
#endif

#ifndef M_LOG10Ef64
#error "math.h:M_LOG10Ef64 macro is missing from libc-shim"
#endif

#ifndef M_LOG10El
#error "math.h:M_LOG10El macro is missing from libc-shim"
#endif

#ifndef M_LOG2E
#error "math.h:M_LOG2E macro is missing from libc-shim"
#endif

#ifndef M_LOG2Ef
#error "math.h:M_LOG2Ef macro is missing from libc-shim"
#endif

#ifndef M_LOG2Ef32
#error "math.h:M_LOG2Ef32 macro is missing from libc-shim"
#endif

#ifndef M_LOG2Ef32x
#error "math.h:M_LOG2Ef32x macro is missing from libc-shim"
#endif

#ifndef M_LOG2Ef64
#error "math.h:M_LOG2Ef64 macro is missing from libc-shim"
#endif

#ifndef M_LOG2El
#error "math.h:M_LOG2El macro is missing from libc-shim"
#endif

#ifndef M_PI
#error "math.h:M_PI macro is missing from libc-shim"
#endif

#ifndef M_PI_2
#error "math.h:M_PI_2 macro is missing from libc-shim"
#endif

#ifndef M_PI_2f
#error "math.h:M_PI_2f macro is missing from libc-shim"
#endif

#ifndef M_PI_2f32
#error "math.h:M_PI_2f32 macro is missing from libc-shim"
#endif

#ifndef M_PI_2f32x
#error "math.h:M_PI_2f32x macro is missing from libc-shim"
#endif

#ifndef M_PI_2f64
#error "math.h:M_PI_2f64 macro is missing from libc-shim"
#endif

#ifndef M_PI_2l
#error "math.h:M_PI_2l macro is missing from libc-shim"
#endif

#ifndef M_PI_4
#error "math.h:M_PI_4 macro is missing from libc-shim"
#endif

#ifndef M_PI_4f
#error "math.h:M_PI_4f macro is missing from libc-shim"
#endif

#ifndef M_PI_4f32
#error "math.h:M_PI_4f32 macro is missing from libc-shim"
#endif

#ifndef M_PI_4f32x
#error "math.h:M_PI_4f32x macro is missing from libc-shim"
#endif

#ifndef M_PI_4f64
#error "math.h:M_PI_4f64 macro is missing from libc-shim"
#endif

#ifndef M_PI_4l
#error "math.h:M_PI_4l macro is missing from libc-shim"
#endif

#ifndef M_PIf
#error "math.h:M_PIf macro is missing from libc-shim"
#endif

#ifndef M_PIf32
#error "math.h:M_PIf32 macro is missing from libc-shim"
#endif

#ifndef M_PIf32x
#error "math.h:M_PIf32x macro is missing from libc-shim"
#endif

#ifndef M_PIf64
#error "math.h:M_PIf64 macro is missing from libc-shim"
#endif

#ifndef M_PIl
#error "math.h:M_PIl macro is missing from libc-shim"
#endif

#ifndef M_SQRT1_2
#error "math.h:M_SQRT1_2 macro is missing from libc-shim"
#endif

#ifndef M_SQRT1_2f
#error "math.h:M_SQRT1_2f macro is missing from libc-shim"
#endif

#ifndef M_SQRT1_2f32
#error "math.h:M_SQRT1_2f32 macro is missing from libc-shim"
#endif

#ifndef M_SQRT1_2f32x
#error "math.h:M_SQRT1_2f32x macro is missing from libc-shim"
#endif

#ifndef M_SQRT1_2f64
#error "math.h:M_SQRT1_2f64 macro is missing from libc-shim"
#endif

#ifndef M_SQRT1_2l
#error "math.h:M_SQRT1_2l macro is missing from libc-shim"
#endif

#ifndef M_SQRT2
#error "math.h:M_SQRT2 macro is missing from libc-shim"
#endif

#ifndef M_SQRT2f
#error "math.h:M_SQRT2f macro is missing from libc-shim"
#endif

#ifndef M_SQRT2f32
#error "math.h:M_SQRT2f32 macro is missing from libc-shim"
#endif

#ifndef M_SQRT2f32x
#error "math.h:M_SQRT2f32x macro is missing from libc-shim"
#endif

#ifndef M_SQRT2f64
#error "math.h:M_SQRT2f64 macro is missing from libc-shim"
#endif

#ifndef M_SQRT2l
#error "math.h:M_SQRT2l macro is missing from libc-shim"
#endif

#ifndef NAN
#error "math.h:NAN macro is missing from libc-shim"
#endif

#ifndef SNAN
#error "math.h:SNAN macro is missing from libc-shim"
#endif

#ifndef SNANF
#error "math.h:SNANF macro is missing from libc-shim"
#endif

#ifndef SNANF32
#error "math.h:SNANF32 macro is missing from libc-shim"
#endif

#ifndef SNANF32X
#error "math.h:SNANF32X macro is missing from libc-shim"
#endif

#ifndef SNANF64
#error "math.h:SNANF64 macro is missing from libc-shim"
#endif

#ifndef SNANL
#error "math.h:SNANL macro is missing from libc-shim"
#endif

#ifndef fpclassify
#error "math.h:fpclassify macro is missing from libc-shim"
#endif

#ifndef iseqsig
#error "math.h:iseqsig macro is missing from libc-shim"
#endif

#ifndef isfinite
#error "math.h:isfinite macro is missing from libc-shim"
#endif

#ifndef isgreater
#error "math.h:isgreater macro is missing from libc-shim"
#endif

#ifndef isgreaterequal
#error "math.h:isgreaterequal macro is missing from libc-shim"
#endif

#ifndef isinf
#error "math.h:isinf macro is missing from libc-shim"
#endif

#ifndef isless
#error "math.h:isless macro is missing from libc-shim"
#endif

#ifndef islessequal
#error "math.h:islessequal macro is missing from libc-shim"
#endif

#ifndef islessgreater
#error "math.h:islessgreater macro is missing from libc-shim"
#endif

#ifndef isnan
#error "math.h:isnan macro is missing from libc-shim"
#endif

#ifndef isnormal
#error "math.h:isnormal macro is missing from libc-shim"
#endif

#ifndef issignaling
#error "math.h:issignaling macro is missing from libc-shim"
#endif

#ifndef issubnormal
#error "math.h:issubnormal macro is missing from libc-shim"
#endif

#ifndef isunordered
#error "math.h:isunordered macro is missing from libc-shim"
#endif

#ifndef iszero
#error "math.h:iszero macro is missing from libc-shim"
#endif

#ifndef math_errhandling
#error "math.h:math_errhandling macro is missing from libc-shim"
#endif

#ifndef signbit
#error "math.h:signbit macro is missing from libc-shim"
#endif

int main(void) { return 0; }
