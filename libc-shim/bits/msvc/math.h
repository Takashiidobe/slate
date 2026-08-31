#ifndef _SLATE_BITS_MSVC_MATH_H
#define _SLATE_BITS_MSVC_MATH_H

#if !defined(_SLATE_LIBC)
#error "Never include <bits/msvc/math.h> directly; include a public header instead."
#endif

#include <bits/msvc/types.h>

typedef float  float_t;
typedef double double_t;

struct _exception {
  int    type;
  char  *name;
  double arg1;
  double arg2;
  double retval;
};

struct _complex {
  double x;
  double y;
};

#define _DOMAIN    1
#define _SING      2
#define _OVERFLOW  3
#define _UNDERFLOW 4
#define _TLOSS     5
#define _PLOSS     6

extern const double _HUGE;

#define _HUGE_ENUF 1e+300
#define INFINITY   ((float)(_HUGE_ENUF * _HUGE_ENUF))
#define HUGE_VAL   ((double)INFINITY)
#define HUGE_VALF  ((float)INFINITY)
#define HUGE_VALL  ((long double)INFINITY)
#define NAN        ((float)(INFINITY * 0.0F))

#define FP_INFINITE  1
#define FP_NAN       2
#define FP_NORMAL    (-1)
#define FP_SUBNORMAL (-2)
#define FP_ZERO      0
#define FP_ILOGB0    (-0x7fffffff - 1)
#define FP_ILOGBNAN  0x7fffffff

#define MATH_ERRNO       1
#define MATH_ERREXCEPT   2
#define math_errhandling (MATH_ERRNO | MATH_ERREXCEPT)

#define _FE_DIVBYZERO 0x04
#define _FE_INEXACT   0x20
#define _FE_INVALID   0x01
#define _FE_OVERFLOW  0x08
#define _FE_UNDERFLOW 0x10

#define fpclassify(x)                                                          \
  __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x)          __builtin_isfinite(x)
#define isinf(x)             __builtin_isinf(x)
#define isnan(x)             __builtin_isnan(x)
#define isnormal(x)          __builtin_isnormal(x)
#define signbit(x)           __builtin_signbit(x)
#define isgreater(x, y)      __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isless(x, y)         __builtin_isless(x, y)
#define islessequal(x, y)    __builtin_islessequal(x, y)
#define islessgreater(x, y)  __builtin_islessgreater(x, y)
#define isunordered(x, y)    __builtin_isunordered(x, y)

void  _fperrraise(int);
short _dclass(double);
short _ldclass(long double);
short _fdclass(float);
int   _dsign(double);
int   _ldsign(long double);
int   _fdsign(float);
int   _dpcomp(double, double);
int   _ldpcomp(long double, long double);
int   _fdpcomp(float, float);

#define __SLATE_MSVC_REAL_FAMILY(suffix, type)                                 \
  type acos##suffix(type);                                                     \
  type acosh##suffix(type);                                                    \
  type asin##suffix(type);                                                     \
  type asinh##suffix(type);                                                    \
  type atan##suffix(type);                                                     \
  type atan2##suffix(type, type);                                              \
  type atanh##suffix(type);                                                    \
  type cbrt##suffix(type);                                                     \
  type ceil##suffix(type);                                                     \
  type copysign##suffix(type, type);                                           \
  type cos##suffix(type);                                                      \
  type cosh##suffix(type);                                                     \
  type erf##suffix(type);                                                      \
  type erfc##suffix(type);                                                     \
  type exp##suffix(type);                                                      \
  type exp2##suffix(type);                                                     \
  type expm1##suffix(type);                                                    \
  type fabs##suffix(type);                                                     \
  type fdim##suffix(type, type);                                               \
  type floor##suffix(type);                                                    \
  type fma##suffix(type, type, type);                                          \
  type fmax##suffix(type, type);                                               \
  type fmin##suffix(type, type);                                               \
  type fmod##suffix(type, type);                                               \
  type hypot##suffix(type, type);                                              \
  type lgamma##suffix(type);                                                   \
  type log##suffix(type);                                                      \
  type log10##suffix(type);                                                    \
  type log1p##suffix(type);                                                    \
  type log2##suffix(type);                                                     \
  type logb##suffix(type);                                                     \
  type nearbyint##suffix(type);                                                \
  type nextafter##suffix(type, type);                                          \
  type pow##suffix(type, type);                                                \
  type remainder##suffix(type, type);                                          \
  type rint##suffix(type);                                                     \
  type round##suffix(type);                                                    \
  type sin##suffix(type);                                                      \
  type sinh##suffix(type);                                                     \
  type sqrt##suffix(type);                                                     \
  type tan##suffix(type);                                                      \
  type tanh##suffix(type);                                                     \
  type tgamma##suffix(type);                                                   \
  type trunc##suffix(type)

__SLATE_MSVC_REAL_FAMILY(, double);
__SLATE_MSVC_REAL_FAMILY(f, float);
__SLATE_MSVC_REAL_FAMILY(l, long double);

#undef __SLATE_MSVC_REAL_FAMILY

double      frexp(double, int *);
float       frexpf(float, int *);
long double frexpl(long double, int *);
int         ilogb(double);
int         ilogbf(float);
int         ilogbl(long double);
double      ldexp(double, int);
float       ldexpf(float, int);
long double ldexpl(long double, int);
long long   llrint(double);
long long   llrintf(float);
long long   llrintl(long double);
long long   llround(double);
long long   llroundf(float);
long long   llroundl(long double);
long        lrint(double);
long        lrintf(float);
long        lrintl(long double);
long        lround(double);
long        lroundf(float);
long        lroundl(long double);
double      modf(double, double *);
float       modff(float, float *);
long double modfl(long double, long double *);
double      nan(const char *);
float       nanf(const char *);
long double nanl(const char *);
double      nexttoward(double, long double);
float       nexttowardf(float, long double);
long double nexttowardl(long double, long double);
double      remquo(double, double, int *);
float       remquof(float, float, int *);
long double remquol(long double, long double, int *);
double      scalbln(double, long);
float       scalblnf(float, long);
long double scalblnl(long double, long);
double      scalbn(double, int);
float       scalbnf(float, int);
long double scalbnl(long double, int);

int         _matherr(struct _exception *);
int         abs(int);
long        labs(long);
long long   llabs(long long);
double      atof(const char *);
double      _atof_l(const char *, _locale_t);
double      _cabs(struct _complex);
double      _chgsign(double);
float       _chgsignf(float);
long double _chgsignl(long double);
double      _copysign(double, double);
float       _copysignf(float, float);
long double _copysignl(long double, long double);
double      _hypot(double, double);
float       _hypotf(float, float);
long double _hypotl(long double, long double);
float       _logbf(float);
float       _nextafterf(float, float);
int         _finitef(float);
int         _isnanf(float);
int         _fpclassf(float);
int         _set_FMA3_enable(int);
int         _get_FMA3_enable(void);
double      _j0(double);
double      _j1(double);
double      _jn(int, double);
double      _y0(double);
double      _y1(double);
double      _yn(int, double);

#if defined(_USE_MATH_DEFINES)
#define _MATH_DEFINES_DEFINED
#define M_E        2.71828182845904523536
#define M_LOG2E    1.44269504088896340736
#define M_LOG10E   0.434294481903251827651
#define M_LN2      0.693147180559945309417
#define M_LN10     2.30258509299404568402
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616
#define M_1_PI     0.318309886183790671538
#define M_2_PI     0.636619772367581343076
#define M_2_SQRTPI 1.12837916709551257390
#define M_SQRT2    1.41421356237309504880
#define M_SQRT1_2  0.707106781186547524401
#endif

#endif
