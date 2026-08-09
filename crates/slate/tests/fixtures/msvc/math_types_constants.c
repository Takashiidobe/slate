#define _USE_MATH_DEFINES
#include <math.h>
#include <stddef.h>

#ifndef _MATH_DEFINES_DEFINED
#error "_USE_MATH_DEFINES must mark the constants as defined"
#endif

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)

_Static_assert(sizeof(long double) == sizeof(double), "long double size");
_Static_assert(_Alignof(long double) == _Alignof(double),
               "long double alignment");
_Static_assert(__builtin_types_compatible_p(float_t, float), "float_t");
_Static_assert(__builtin_types_compatible_p(double_t, double), "double_t");

_Static_assert(FP_INFINITE == 1, "FP_INFINITE");
_Static_assert(FP_NAN == 2, "FP_NAN");
_Static_assert(FP_NORMAL == -1, "FP_NORMAL");
_Static_assert(FP_SUBNORMAL == -2, "FP_SUBNORMAL");
_Static_assert(FP_ZERO == 0, "FP_ZERO");
_Static_assert(FP_ILOGB0 == (-0x7fffffff - 1), "FP_ILOGB0");
_Static_assert(FP_ILOGBNAN == 0x7fffffff, "FP_ILOGBNAN");
_Static_assert(MATH_ERRNO == 1, "MATH_ERRNO");
_Static_assert(MATH_ERREXCEPT == 2, "MATH_ERREXCEPT");
_Static_assert(math_errhandling == 3, "math_errhandling");

_Static_assert(_DOMAIN == 1, "_DOMAIN");
_Static_assert(_SING == 2, "_SING");
_Static_assert(_OVERFLOW == 3, "_OVERFLOW");
_Static_assert(_UNDERFLOW == 4, "_UNDERFLOW");
_Static_assert(_TLOSS == 5, "_TLOSS");
_Static_assert(_PLOSS == 6, "_PLOSS");
_Static_assert(_FE_DIVBYZERO == 0x04, "_FE_DIVBYZERO");
_Static_assert(_FE_INEXACT == 0x20, "_FE_INEXACT");
_Static_assert(_FE_INVALID == 0x01, "_FE_INVALID");
_Static_assert(_FE_OVERFLOW == 0x08, "_FE_OVERFLOW");
_Static_assert(_FE_UNDERFLOW == 0x10, "_FE_UNDERFLOW");

_Static_assert(sizeof(struct _complex) == 16, "_complex size");
_Static_assert(_Alignof(struct _complex) == 8, "_complex alignment");
_Static_assert(offsetof(struct _complex, x) == 0, "_complex x");
_Static_assert(offsetof(struct _complex, y) == 8, "_complex y");
_Static_assert(sizeof(struct _exception) == 40, "_exception size");
_Static_assert(_Alignof(struct _exception) == 8, "_exception alignment");
_Static_assert(offsetof(struct _exception, type) == 0, "_exception type");
_Static_assert(offsetof(struct _exception, name) == 8, "_exception name");
_Static_assert(offsetof(struct _exception, arg1) == 16, "_exception arg1");
_Static_assert(offsetof(struct _exception, arg2) == 24, "_exception arg2");
_Static_assert(offsetof(struct _exception, retval) == 32,
               "_exception retval");

TYPE_IS(HUGE_VAL, double);
TYPE_IS(HUGE_VALF, float);
TYPE_IS(HUGE_VALL, long double);
TYPE_IS(INFINITY, float);
TYPE_IS(NAN, float);
TYPE_IS(&_HUGE, const double *);
TYPE_IS(fpclassify(0.0f), int);
TYPE_IS(fpclassify(0.0), int);
TYPE_IS(fpclassify(0.0L), int);
TYPE_IS(isfinite(0.0), int);
TYPE_IS(isinf(0.0), int);
TYPE_IS(isnan(0.0), int);
TYPE_IS(isnormal(0.0), int);
TYPE_IS(signbit(0.0), int);
TYPE_IS(isgreater(1.0, 0.0), int);
TYPE_IS(isgreaterequal(1.0, 0.0), int);
TYPE_IS(isless(0.0, 1.0), int);
TYPE_IS(islessequal(0.0, 1.0), int);
TYPE_IS(islessgreater(0.0, 1.0), int);
TYPE_IS(isunordered(0.0, NAN), int);
TYPE_IS(M_E, double);
TYPE_IS(M_LOG2E, double);
TYPE_IS(M_LOG10E, double);
TYPE_IS(M_LN2, double);
TYPE_IS(M_LN10, double);
TYPE_IS(M_PI, double);
TYPE_IS(M_PI_2, double);
TYPE_IS(M_PI_4, double);
TYPE_IS(M_1_PI, double);
TYPE_IS(M_2_PI, double);
TYPE_IS(M_2_SQRTPI, double);
TYPE_IS(M_SQRT2, double);
TYPE_IS(M_SQRT1_2, double);
_Static_assert(M_PI > 3.14 && M_PI < 3.15, "M_PI value");

int main(void) { return 0; }
