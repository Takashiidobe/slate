#include <math.h>

#define TYPE_IS(expression, ...)                                               \
  _Static_assert(                                                             \
      __builtin_types_compatible_p(__typeof__(expression), __VA_ARGS__),       \
      #expression)
#define UNARY(name, type) TYPE_IS(&name, type (*)(type))
#define BINARY(name, type) TYPE_IS(&name, type (*)(type, type))
#define TERNARY(name, type) TYPE_IS(&name, type (*)(type, type, type))

#define CHECK_REAL_FAMILY(suffix, type)                                       \
  UNARY(acos##suffix, type);                                                   \
  UNARY(acosh##suffix, type);                                                  \
  UNARY(asin##suffix, type);                                                   \
  UNARY(asinh##suffix, type);                                                  \
  UNARY(atan##suffix, type);                                                   \
  BINARY(atan2##suffix, type);                                                 \
  UNARY(atanh##suffix, type);                                                  \
  UNARY(cbrt##suffix, type);                                                   \
  UNARY(ceil##suffix, type);                                                   \
  BINARY(copysign##suffix, type);                                              \
  UNARY(cos##suffix, type);                                                    \
  UNARY(cosh##suffix, type);                                                   \
  UNARY(erf##suffix, type);                                                    \
  UNARY(erfc##suffix, type);                                                   \
  UNARY(exp##suffix, type);                                                    \
  UNARY(exp2##suffix, type);                                                   \
  UNARY(expm1##suffix, type);                                                  \
  UNARY(fabs##suffix, type);                                                   \
  BINARY(fdim##suffix, type);                                                  \
  UNARY(floor##suffix, type);                                                  \
  TERNARY(fma##suffix, type);                                                  \
  BINARY(fmax##suffix, type);                                                  \
  BINARY(fmin##suffix, type);                                                  \
  BINARY(fmod##suffix, type);                                                  \
  BINARY(hypot##suffix, type);                                                 \
  UNARY(lgamma##suffix, type);                                                 \
  UNARY(log##suffix, type);                                                    \
  UNARY(log10##suffix, type);                                                  \
  UNARY(log1p##suffix, type);                                                  \
  UNARY(log2##suffix, type);                                                   \
  UNARY(logb##suffix, type);                                                   \
  UNARY(nearbyint##suffix, type);                                              \
  BINARY(nextafter##suffix, type);                                             \
  BINARY(pow##suffix, type);                                                   \
  BINARY(remainder##suffix, type);                                             \
  UNARY(rint##suffix, type);                                                   \
  UNARY(round##suffix, type);                                                  \
  UNARY(sin##suffix, type);                                                    \
  UNARY(sinh##suffix, type);                                                   \
  UNARY(sqrt##suffix, type);                                                   \
  UNARY(tan##suffix, type);                                                    \
  UNARY(tanh##suffix, type);                                                   \
  UNARY(tgamma##suffix, type);                                                 \
  UNARY(trunc##suffix, type)

CHECK_REAL_FAMILY(, double);
CHECK_REAL_FAMILY(f, float);
CHECK_REAL_FAMILY(l, long double);

TYPE_IS(&frexp, double (*)(double, int *));
TYPE_IS(&frexpf, float (*)(float, int *));
TYPE_IS(&frexpl, long double (*)(long double, int *));
TYPE_IS(&ilogb, int (*)(double));
TYPE_IS(&ilogbf, int (*)(float));
TYPE_IS(&ilogbl, int (*)(long double));
TYPE_IS(&ldexp, double (*)(double, int));
TYPE_IS(&ldexpf, float (*)(float, int));
TYPE_IS(&ldexpl, long double (*)(long double, int));
TYPE_IS(&llrint, long long (*)(double));
TYPE_IS(&llrintf, long long (*)(float));
TYPE_IS(&llrintl, long long (*)(long double));
TYPE_IS(&llround, long long (*)(double));
TYPE_IS(&llroundf, long long (*)(float));
TYPE_IS(&llroundl, long long (*)(long double));
TYPE_IS(&lrint, long (*)(double));
TYPE_IS(&lrintf, long (*)(float));
TYPE_IS(&lrintl, long (*)(long double));
TYPE_IS(&lround, long (*)(double));
TYPE_IS(&lroundf, long (*)(float));
TYPE_IS(&lroundl, long (*)(long double));
TYPE_IS(&modf, double (*)(double, double *));
TYPE_IS(&modff, float (*)(float, float *));
TYPE_IS(&modfl, long double (*)(long double, long double *));
TYPE_IS(&nan, double (*)(const char *));
TYPE_IS(&nanf, float (*)(const char *));
TYPE_IS(&nanl, long double (*)(const char *));
TYPE_IS(&nexttoward, double (*)(double, long double));
TYPE_IS(&nexttowardf, float (*)(float, long double));
TYPE_IS(&nexttowardl, long double (*)(long double, long double));
TYPE_IS(&remquo, double (*)(double, double, int *));
TYPE_IS(&remquof, float (*)(float, float, int *));
TYPE_IS(&remquol, long double (*)(long double, long double, int *));
TYPE_IS(&scalbln, double (*)(double, long));
TYPE_IS(&scalblnf, float (*)(float, long));
TYPE_IS(&scalblnl, long double (*)(long double, long));
TYPE_IS(&scalbn, double (*)(double, int));
TYPE_IS(&scalbnf, float (*)(float, int));
TYPE_IS(&scalbnl, long double (*)(long double, int));

TYPE_IS(&_matherr, int (*)(struct _exception *));
TYPE_IS(&abs, int (*)(int));
TYPE_IS(&labs, long (*)(long));
TYPE_IS(&llabs, long long (*)(long long));
TYPE_IS(&atof, double (*)(const char *));
TYPE_IS(&_atof_l, double (*)(const char *, _locale_t));
TYPE_IS(&_cabs, double (*)(struct _complex));
TYPE_IS(&_chgsign, double (*)(double));
TYPE_IS(&_chgsignf, float (*)(float));
TYPE_IS(&_chgsignl, long double (*)(long double));
TYPE_IS(&_copysign, double (*)(double, double));
TYPE_IS(&_copysignf, float (*)(float, float));
TYPE_IS(&_copysignl, long double (*)(long double, long double));
TYPE_IS(&_hypot, double (*)(double, double));
TYPE_IS(&_hypotf, float (*)(float, float));
TYPE_IS(&_hypotl, long double (*)(long double, long double));
TYPE_IS(&_logbf, float (*)(float));
TYPE_IS(&_nextafterf, float (*)(float, float));
TYPE_IS(&_finitef, int (*)(float));
TYPE_IS(&_isnanf, int (*)(float));
TYPE_IS(&_fpclassf, int (*)(float));
TYPE_IS(&_set_FMA3_enable, int (*)(int));
TYPE_IS(&_get_FMA3_enable, int (*)(void));
TYPE_IS(&_j0, double (*)(double));
TYPE_IS(&_j1, double (*)(double));
TYPE_IS(&_jn, double (*)(int, double));
TYPE_IS(&_y0, double (*)(double));
TYPE_IS(&_y1, double (*)(double));
TYPE_IS(&_yn, double (*)(int, double));

int main(void) { return 0; }
