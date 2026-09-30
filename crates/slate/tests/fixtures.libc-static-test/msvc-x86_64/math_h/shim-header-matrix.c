#include <math.h>

extern double slate_oracle__atof_l(const char *, _locale_t) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__atof_l), __typeof__(_atof_l)),
    "math.h:_atof_l declaration differs from oracle");

static __typeof__(_atof_l) *const slate_reference__atof_l = &_atof_l;

extern double slate_oracle__cabs(struct _complex) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__cabs), __typeof__(_cabs)),
    "math.h:_cabs declaration differs from oracle");

static __typeof__(_cabs) *const slate_reference__cabs = &_cabs;

extern double slate_oracle__chgsign(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__chgsign), __typeof__(_chgsign)),
    "math.h:_chgsign declaration differs from oracle");

static __typeof__(_chgsign) *const slate_reference__chgsign = &_chgsign;

extern float slate_oracle__chgsignf(float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__chgsignf), __typeof__(_chgsignf)),
    "math.h:_chgsignf declaration differs from oracle");

static __typeof__(_chgsignf) *const slate_reference__chgsignf = &_chgsignf;

extern long double slate_oracle__chgsignl(long double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__chgsignl), __typeof__(_chgsignl)),
    "math.h:_chgsignl declaration differs from oracle");

static __typeof__(_chgsignl) *const slate_reference__chgsignl = &_chgsignl;

extern double slate_oracle__copysign(double, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__copysign), __typeof__(_copysign)),
    "math.h:_copysign declaration differs from oracle");

static __typeof__(_copysign) *const slate_reference__copysign = &_copysign;

extern float slate_oracle__copysignf(float, float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__copysignf), __typeof__(_copysignf)),
    "math.h:_copysignf declaration differs from oracle");

static __typeof__(_copysignf) *const slate_reference__copysignf = &_copysignf;

extern long double slate_oracle__copysignl(long double, long double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__copysignl), __typeof__(_copysignl)),
    "math.h:_copysignl declaration differs from oracle");

static __typeof__(_copysignl) *const slate_reference__copysignl = &_copysignl;

extern short slate_oracle__d_int(double *, short) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__d_int), __typeof__(_d_int)),
    "math.h:_d_int declaration differs from oracle");

static __typeof__(_d_int) *const slate_reference__d_int = &_d_int;

extern short slate_oracle__dclass(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dclass), __typeof__(_dclass)),
    "math.h:_dclass declaration differs from oracle");

static __typeof__(_dclass) *const slate_reference__dclass = &_dclass;

extern short slate_oracle__dexp(double *, double, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dexp), __typeof__(_dexp)),
    "math.h:_dexp declaration differs from oracle");

static __typeof__(_dexp) *const slate_reference__dexp = &_dexp;

extern double slate_oracle__dlog(double, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dlog), __typeof__(_dlog)),
    "math.h:_dlog declaration differs from oracle");

static __typeof__(_dlog) *const slate_reference__dlog = &_dlog;

extern short slate_oracle__dnorm(unsigned short *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dnorm), __typeof__(_dnorm)),
    "math.h:_dnorm declaration differs from oracle");

static __typeof__(_dnorm) *const slate_reference__dnorm = &_dnorm;

extern int slate_oracle__dpcomp(double, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dpcomp), __typeof__(_dpcomp)),
    "math.h:_dpcomp declaration differs from oracle");

static __typeof__(_dpcomp) *const slate_reference__dpcomp = &_dpcomp;

extern double slate_oracle__dpoly(double, const double *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dpoly), __typeof__(_dpoly)),
    "math.h:_dpoly declaration differs from oracle");

static __typeof__(_dpoly) *const slate_reference__dpoly = &_dpoly;

extern short slate_oracle__dscale(double *, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dscale), __typeof__(_dscale)),
    "math.h:_dscale declaration differs from oracle");

static __typeof__(_dscale) *const slate_reference__dscale = &_dscale;

extern int slate_oracle__dsign(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dsign), __typeof__(_dsign)),
    "math.h:_dsign declaration differs from oracle");

static __typeof__(_dsign) *const slate_reference__dsign = &_dsign;

extern double slate_oracle__dsin(double, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dsin), __typeof__(_dsin)),
    "math.h:_dsin declaration differs from oracle");

static __typeof__(_dsin) *const slate_reference__dsin = &_dsin;

extern short slate_oracle__dtest(double *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dtest), __typeof__(_dtest)),
    "math.h:_dtest declaration differs from oracle");

static __typeof__(_dtest) *const slate_reference__dtest = &_dtest;

extern short slate_oracle__dunscale(short *, double *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__dunscale), __typeof__(_dunscale)),
    "math.h:_dunscale declaration differs from oracle");

static __typeof__(_dunscale) *const slate_reference__dunscale = &_dunscale;

extern short slate_oracle__fd_int(float *, short) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fd_int), __typeof__(_fd_int)),
    "math.h:_fd_int declaration differs from oracle");

static __typeof__(_fd_int) *const slate_reference__fd_int = &_fd_int;

extern short slate_oracle__fdclass(float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdclass), __typeof__(_fdclass)),
    "math.h:_fdclass declaration differs from oracle");

static __typeof__(_fdclass) *const slate_reference__fdclass = &_fdclass;

extern short slate_oracle__fdexp(float *, float, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdexp), __typeof__(_fdexp)),
    "math.h:_fdexp declaration differs from oracle");

static __typeof__(_fdexp) *const slate_reference__fdexp = &_fdexp;

extern float slate_oracle__fdlog(float, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdlog), __typeof__(_fdlog)),
    "math.h:_fdlog declaration differs from oracle");

static __typeof__(_fdlog) *const slate_reference__fdlog = &_fdlog;

extern short slate_oracle__fdnorm(unsigned short *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdnorm), __typeof__(_fdnorm)),
    "math.h:_fdnorm declaration differs from oracle");

static __typeof__(_fdnorm) *const slate_reference__fdnorm = &_fdnorm;

extern int slate_oracle__fdpcomp(float, float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdpcomp), __typeof__(_fdpcomp)),
    "math.h:_fdpcomp declaration differs from oracle");

static __typeof__(_fdpcomp) *const slate_reference__fdpcomp = &_fdpcomp;

extern float slate_oracle__fdpoly(float, const float *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdpoly), __typeof__(_fdpoly)),
    "math.h:_fdpoly declaration differs from oracle");

static __typeof__(_fdpoly) *const slate_reference__fdpoly = &_fdpoly;

extern short slate_oracle__fdscale(float *, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdscale), __typeof__(_fdscale)),
    "math.h:_fdscale declaration differs from oracle");

static __typeof__(_fdscale) *const slate_reference__fdscale = &_fdscale;

extern int slate_oracle__fdsign(float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdsign), __typeof__(_fdsign)),
    "math.h:_fdsign declaration differs from oracle");

static __typeof__(_fdsign) *const slate_reference__fdsign = &_fdsign;

extern float slate_oracle__fdsin(float, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdsin), __typeof__(_fdsin)),
    "math.h:_fdsin declaration differs from oracle");

static __typeof__(_fdsin) *const slate_reference__fdsin = &_fdsin;

extern short slate_oracle__fdtest(float *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdtest), __typeof__(_fdtest)),
    "math.h:_fdtest declaration differs from oracle");

static __typeof__(_fdtest) *const slate_reference__fdtest = &_fdtest;

extern short slate_oracle__fdunscale(short *, float *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fdunscale), __typeof__(_fdunscale)),
    "math.h:_fdunscale declaration differs from oracle");

static __typeof__(_fdunscale) *const slate_reference__fdunscale = &_fdunscale;

extern int slate_oracle__finitef(float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__finitef), __typeof__(_finitef)),
    "math.h:_finitef declaration differs from oracle");

static __typeof__(_finitef) *const slate_reference__finitef = &_finitef;

extern int slate_oracle__fpclassf(float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fpclassf), __typeof__(_fpclassf)),
    "math.h:_fpclassf declaration differs from oracle");

static __typeof__(_fpclassf) *const slate_reference__fpclassf = &_fpclassf;

extern void slate_oracle__fperrraise(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__fperrraise), __typeof__(_fperrraise)),
    "math.h:_fperrraise declaration differs from oracle");

static __typeof__(_fperrraise) *const slate_reference__fperrraise = &_fperrraise;

extern int slate_oracle__get_FMA3_enable(void) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__get_FMA3_enable), __typeof__(_get_FMA3_enable)),
    "math.h:_get_FMA3_enable declaration differs from oracle");

static __typeof__(_get_FMA3_enable) *const slate_reference__get_FMA3_enable = &_get_FMA3_enable;

extern double slate_oracle__hypot(double, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__hypot), __typeof__(_hypot)),
    "math.h:_hypot declaration differs from oracle");

static __typeof__(_hypot) *const slate_reference__hypot = &_hypot;

extern float slate_oracle__hypotf(float, float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__hypotf), __typeof__(_hypotf)),
    "math.h:_hypotf declaration differs from oracle");

static __typeof__(_hypotf) *const slate_reference__hypotf = &_hypotf;

extern long double slate_oracle__hypotl(long double, long double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__hypotl), __typeof__(_hypotl)),
    "math.h:_hypotl declaration differs from oracle");

static __typeof__(_hypotl) *const slate_reference__hypotl = &_hypotl;

extern int slate_oracle__isnanf(float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__isnanf), __typeof__(_isnanf)),
    "math.h:_isnanf declaration differs from oracle");

static __typeof__(_isnanf) *const slate_reference__isnanf = &_isnanf;

extern double slate_oracle__j0(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__j0), __typeof__(_j0)),
    "math.h:_j0 declaration differs from oracle");

static __typeof__(_j0) *const slate_reference__j0 = &_j0;

extern double slate_oracle__j1(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__j1), __typeof__(_j1)),
    "math.h:_j1 declaration differs from oracle");

static __typeof__(_j1) *const slate_reference__j1 = &_j1;

extern double slate_oracle__jn(int, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__jn), __typeof__(_jn)),
    "math.h:_jn declaration differs from oracle");

static __typeof__(_jn) *const slate_reference__jn = &_jn;

extern short slate_oracle__ld_int(long double *, short) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ld_int), __typeof__(_ld_int)),
    "math.h:_ld_int declaration differs from oracle");

static __typeof__(_ld_int) *const slate_reference__ld_int = &_ld_int;

extern short slate_oracle__ldclass(long double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldclass), __typeof__(_ldclass)),
    "math.h:_ldclass declaration differs from oracle");

static __typeof__(_ldclass) *const slate_reference__ldclass = &_ldclass;

extern short slate_oracle__ldexp(long double *, long double, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldexp), __typeof__(_ldexp)),
    "math.h:_ldexp declaration differs from oracle");

static __typeof__(_ldexp) *const slate_reference__ldexp = &_ldexp;

extern long double slate_oracle__ldlog(long double, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldlog), __typeof__(_ldlog)),
    "math.h:_ldlog declaration differs from oracle");

static __typeof__(_ldlog) *const slate_reference__ldlog = &_ldlog;

extern int slate_oracle__ldpcomp(long double, long double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldpcomp), __typeof__(_ldpcomp)),
    "math.h:_ldpcomp declaration differs from oracle");

static __typeof__(_ldpcomp) *const slate_reference__ldpcomp = &_ldpcomp;

extern long double slate_oracle__ldpoly(long double, const long double *, int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldpoly), __typeof__(_ldpoly)),
    "math.h:_ldpoly declaration differs from oracle");

static __typeof__(_ldpoly) *const slate_reference__ldpoly = &_ldpoly;

extern short slate_oracle__ldscale(long double *, long) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldscale), __typeof__(_ldscale)),
    "math.h:_ldscale declaration differs from oracle");

static __typeof__(_ldscale) *const slate_reference__ldscale = &_ldscale;

extern int slate_oracle__ldsign(long double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldsign), __typeof__(_ldsign)),
    "math.h:_ldsign declaration differs from oracle");

static __typeof__(_ldsign) *const slate_reference__ldsign = &_ldsign;

extern long double slate_oracle__ldsin(long double, unsigned int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldsin), __typeof__(_ldsin)),
    "math.h:_ldsin declaration differs from oracle");

static __typeof__(_ldsin) *const slate_reference__ldsin = &_ldsin;

extern short slate_oracle__ldtest(long double *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldtest), __typeof__(_ldtest)),
    "math.h:_ldtest declaration differs from oracle");

static __typeof__(_ldtest) *const slate_reference__ldtest = &_ldtest;

extern short slate_oracle__ldunscale(short *, long double *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__ldunscale), __typeof__(_ldunscale)),
    "math.h:_ldunscale declaration differs from oracle");

static __typeof__(_ldunscale) *const slate_reference__ldunscale = &_ldunscale;

extern float slate_oracle__logbf(float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__logbf), __typeof__(_logbf)),
    "math.h:_logbf declaration differs from oracle");

static __typeof__(_logbf) *const slate_reference__logbf = &_logbf;

extern int slate_oracle__matherr(struct _exception *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__matherr), __typeof__(_matherr)),
    "math.h:_matherr declaration differs from oracle");

static __typeof__(_matherr) *const slate_reference__matherr = &_matherr;

extern float slate_oracle__nextafterf(float, float) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__nextafterf), __typeof__(_nextafterf)),
    "math.h:_nextafterf declaration differs from oracle");

static __typeof__(_nextafterf) *const slate_reference__nextafterf = &_nextafterf;

extern int slate_oracle__set_FMA3_enable(int) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__set_FMA3_enable), __typeof__(_set_FMA3_enable)),
    "math.h:_set_FMA3_enable declaration differs from oracle");

static __typeof__(_set_FMA3_enable) *const slate_reference__set_FMA3_enable = &_set_FMA3_enable;

extern double slate_oracle__y0(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__y0), __typeof__(_y0)),
    "math.h:_y0 declaration differs from oracle");

static __typeof__(_y0) *const slate_reference__y0 = &_y0;

extern double slate_oracle__y1(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__y1), __typeof__(_y1)),
    "math.h:_y1 declaration differs from oracle");

static __typeof__(_y1) *const slate_reference__y1 = &_y1;

extern double slate_oracle__yn(int, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle__yn), __typeof__(_yn)),
    "math.h:_yn declaration differs from oracle");

static __typeof__(_yn) *const slate_reference__yn = &_yn;

extern int slate_oracle_abs(int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_abs), __typeof__(abs)),
    "math.h:abs declaration differs from oracle");

static __typeof__(abs) *const slate_reference_abs = &abs;

extern double slate_oracle_acos(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acos), __typeof__(acos)),
    "math.h:acos declaration differs from oracle");

static __typeof__(acos) *const slate_reference_acos = &acos;

extern float slate_oracle_acosf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acosf), __typeof__(acosf)),
    "math.h:acosf declaration differs from oracle");

static __typeof__(acosf) *const slate_reference_acosf = &acosf;

extern double slate_oracle_acosh(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acosh), __typeof__(acosh)),
    "math.h:acosh declaration differs from oracle");

static __typeof__(acosh) *const slate_reference_acosh = &acosh;

extern float slate_oracle_acoshf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acoshf), __typeof__(acoshf)),
    "math.h:acoshf declaration differs from oracle");

static __typeof__(acoshf) *const slate_reference_acoshf = &acoshf;

extern long double slate_oracle_acoshl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acoshl), __typeof__(acoshl)),
    "math.h:acoshl declaration differs from oracle");

static __typeof__(acoshl) *const slate_reference_acoshl = &acoshl;

extern long double slate_oracle_acosl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acosl), __typeof__(acosl)),
    "math.h:acosl declaration differs from oracle");

static __typeof__(acosl) *const slate_reference_acosl = &acosl;

extern double slate_oracle_asin(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asin), __typeof__(asin)),
    "math.h:asin declaration differs from oracle");

static __typeof__(asin) *const slate_reference_asin = &asin;

extern float slate_oracle_asinf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinf), __typeof__(asinf)),
    "math.h:asinf declaration differs from oracle");

static __typeof__(asinf) *const slate_reference_asinf = &asinf;

extern double slate_oracle_asinh(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinh), __typeof__(asinh)),
    "math.h:asinh declaration differs from oracle");

static __typeof__(asinh) *const slate_reference_asinh = &asinh;

extern float slate_oracle_asinhf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinhf), __typeof__(asinhf)),
    "math.h:asinhf declaration differs from oracle");

static __typeof__(asinhf) *const slate_reference_asinhf = &asinhf;

extern long double slate_oracle_asinhl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinhl), __typeof__(asinhl)),
    "math.h:asinhl declaration differs from oracle");

static __typeof__(asinhl) *const slate_reference_asinhl = &asinhl;

extern long double slate_oracle_asinl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinl), __typeof__(asinl)),
    "math.h:asinl declaration differs from oracle");

static __typeof__(asinl) *const slate_reference_asinl = &asinl;

extern double slate_oracle_atan(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan), __typeof__(atan)),
    "math.h:atan declaration differs from oracle");

static __typeof__(atan) *const slate_reference_atan = &atan;

extern double slate_oracle_atan2(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan2), __typeof__(atan2)),
    "math.h:atan2 declaration differs from oracle");

static __typeof__(atan2) *const slate_reference_atan2 = &atan2;

extern float slate_oracle_atan2f(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan2f), __typeof__(atan2f)),
    "math.h:atan2f declaration differs from oracle");

static __typeof__(atan2f) *const slate_reference_atan2f = &atan2f;

extern long double slate_oracle_atan2l(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan2l), __typeof__(atan2l)),
    "math.h:atan2l declaration differs from oracle");

static __typeof__(atan2l) *const slate_reference_atan2l = &atan2l;

extern float slate_oracle_atanf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanf), __typeof__(atanf)),
    "math.h:atanf declaration differs from oracle");

static __typeof__(atanf) *const slate_reference_atanf = &atanf;

extern double slate_oracle_atanh(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanh), __typeof__(atanh)),
    "math.h:atanh declaration differs from oracle");

static __typeof__(atanh) *const slate_reference_atanh = &atanh;

extern float slate_oracle_atanhf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanhf), __typeof__(atanhf)),
    "math.h:atanhf declaration differs from oracle");

static __typeof__(atanhf) *const slate_reference_atanhf = &atanhf;

extern long double slate_oracle_atanhl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanhl), __typeof__(atanhl)),
    "math.h:atanhl declaration differs from oracle");

static __typeof__(atanhl) *const slate_reference_atanhl = &atanhl;

extern long double slate_oracle_atanl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanl), __typeof__(atanl)),
    "math.h:atanl declaration differs from oracle");

static __typeof__(atanl) *const slate_reference_atanl = &atanl;

extern double slate_oracle_atof(const char *) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atof), __typeof__(atof)),
    "math.h:atof declaration differs from oracle");

static __typeof__(atof) *const slate_reference_atof = &atof;

extern double slate_oracle_cbrt(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cbrt), __typeof__(cbrt)),
    "math.h:cbrt declaration differs from oracle");

static __typeof__(cbrt) *const slate_reference_cbrt = &cbrt;

extern float slate_oracle_cbrtf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cbrtf), __typeof__(cbrtf)),
    "math.h:cbrtf declaration differs from oracle");

static __typeof__(cbrtf) *const slate_reference_cbrtf = &cbrtf;

extern long double slate_oracle_cbrtl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cbrtl), __typeof__(cbrtl)),
    "math.h:cbrtl declaration differs from oracle");

static __typeof__(cbrtl) *const slate_reference_cbrtl = &cbrtl;

extern double slate_oracle_ceil(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ceil), __typeof__(ceil)),
    "math.h:ceil declaration differs from oracle");

static __typeof__(ceil) *const slate_reference_ceil = &ceil;

extern float slate_oracle_ceilf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ceilf), __typeof__(ceilf)),
    "math.h:ceilf declaration differs from oracle");

static __typeof__(ceilf) *const slate_reference_ceilf = &ceilf;

extern long double slate_oracle_ceill(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ceill), __typeof__(ceill)),
    "math.h:ceill declaration differs from oracle");

static __typeof__(ceill) *const slate_reference_ceill = &ceill;

extern double slate_oracle_copysign(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_copysign), __typeof__(copysign)),
    "math.h:copysign declaration differs from oracle");

static __typeof__(copysign) *const slate_reference_copysign = &copysign;

extern float slate_oracle_copysignf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_copysignf), __typeof__(copysignf)),
    "math.h:copysignf declaration differs from oracle");

static __typeof__(copysignf) *const slate_reference_copysignf = &copysignf;

extern long double slate_oracle_copysignl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_copysignl), __typeof__(copysignl)),
    "math.h:copysignl declaration differs from oracle");

static __typeof__(copysignl) *const slate_reference_copysignl = &copysignl;

extern double slate_oracle_cos(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cos), __typeof__(cos)),
    "math.h:cos declaration differs from oracle");

static __typeof__(cos) *const slate_reference_cos = &cos;

extern float slate_oracle_cosf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cosf), __typeof__(cosf)),
    "math.h:cosf declaration differs from oracle");

static __typeof__(cosf) *const slate_reference_cosf = &cosf;

extern double slate_oracle_cosh(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cosh), __typeof__(cosh)),
    "math.h:cosh declaration differs from oracle");

static __typeof__(cosh) *const slate_reference_cosh = &cosh;

extern float slate_oracle_coshf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_coshf), __typeof__(coshf)),
    "math.h:coshf declaration differs from oracle");

static __typeof__(coshf) *const slate_reference_coshf = &coshf;

extern long double slate_oracle_coshl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_coshl), __typeof__(coshl)),
    "math.h:coshl declaration differs from oracle");

static __typeof__(coshl) *const slate_reference_coshl = &coshl;

extern long double slate_oracle_cosl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cosl), __typeof__(cosl)),
    "math.h:cosl declaration differs from oracle");

static __typeof__(cosl) *const slate_reference_cosl = &cosl;

extern double slate_oracle_erf(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erf), __typeof__(erf)),
    "math.h:erf declaration differs from oracle");

static __typeof__(erf) *const slate_reference_erf = &erf;

extern double slate_oracle_erfc(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfc), __typeof__(erfc)),
    "math.h:erfc declaration differs from oracle");

static __typeof__(erfc) *const slate_reference_erfc = &erfc;

extern float slate_oracle_erfcf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfcf), __typeof__(erfcf)),
    "math.h:erfcf declaration differs from oracle");

static __typeof__(erfcf) *const slate_reference_erfcf = &erfcf;

extern long double slate_oracle_erfcl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfcl), __typeof__(erfcl)),
    "math.h:erfcl declaration differs from oracle");

static __typeof__(erfcl) *const slate_reference_erfcl = &erfcl;

extern float slate_oracle_erff(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erff), __typeof__(erff)),
    "math.h:erff declaration differs from oracle");

static __typeof__(erff) *const slate_reference_erff = &erff;

extern long double slate_oracle_erfl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfl), __typeof__(erfl)),
    "math.h:erfl declaration differs from oracle");

static __typeof__(erfl) *const slate_reference_erfl = &erfl;

extern double slate_oracle_exp(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp), __typeof__(exp)),
    "math.h:exp declaration differs from oracle");

static __typeof__(exp) *const slate_reference_exp = &exp;

extern double slate_oracle_exp2(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp2), __typeof__(exp2)),
    "math.h:exp2 declaration differs from oracle");

static __typeof__(exp2) *const slate_reference_exp2 = &exp2;

extern float slate_oracle_exp2f(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp2f), __typeof__(exp2f)),
    "math.h:exp2f declaration differs from oracle");

static __typeof__(exp2f) *const slate_reference_exp2f = &exp2f;

extern long double slate_oracle_exp2l(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp2l), __typeof__(exp2l)),
    "math.h:exp2l declaration differs from oracle");

static __typeof__(exp2l) *const slate_reference_exp2l = &exp2l;

extern float slate_oracle_expf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expf), __typeof__(expf)),
    "math.h:expf declaration differs from oracle");

static __typeof__(expf) *const slate_reference_expf = &expf;

extern long double slate_oracle_expl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expl), __typeof__(expl)),
    "math.h:expl declaration differs from oracle");

static __typeof__(expl) *const slate_reference_expl = &expl;

extern double slate_oracle_expm1(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expm1), __typeof__(expm1)),
    "math.h:expm1 declaration differs from oracle");

static __typeof__(expm1) *const slate_reference_expm1 = &expm1;

extern float slate_oracle_expm1f(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expm1f), __typeof__(expm1f)),
    "math.h:expm1f declaration differs from oracle");

static __typeof__(expm1f) *const slate_reference_expm1f = &expm1f;

extern long double slate_oracle_expm1l(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expm1l), __typeof__(expm1l)),
    "math.h:expm1l declaration differs from oracle");

static __typeof__(expm1l) *const slate_reference_expm1l = &expm1l;

extern double slate_oracle_fabs(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fabs), __typeof__(fabs)),
    "math.h:fabs declaration differs from oracle");

static __typeof__(fabs) *const slate_reference_fabs = &fabs;

extern float slate_oracle_fabsf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fabsf), __typeof__(fabsf)),
    "math.h:fabsf declaration differs from oracle");

static __typeof__(fabsf) *const slate_reference_fabsf = &fabsf;

extern long double slate_oracle_fabsl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fabsl), __typeof__(fabsl)),
    "math.h:fabsl declaration differs from oracle");

static __typeof__(fabsl) *const slate_reference_fabsl = &fabsl;

extern double slate_oracle_fdim(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdim), __typeof__(fdim)),
    "math.h:fdim declaration differs from oracle");

static __typeof__(fdim) *const slate_reference_fdim = &fdim;

extern float slate_oracle_fdimf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdimf), __typeof__(fdimf)),
    "math.h:fdimf declaration differs from oracle");

static __typeof__(fdimf) *const slate_reference_fdimf = &fdimf;

extern long double slate_oracle_fdiml(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdiml), __typeof__(fdiml)),
    "math.h:fdiml declaration differs from oracle");

static __typeof__(fdiml) *const slate_reference_fdiml = &fdiml;

extern double slate_oracle_floor(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_floor), __typeof__(floor)),
    "math.h:floor declaration differs from oracle");

static __typeof__(floor) *const slate_reference_floor = &floor;

extern float slate_oracle_floorf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_floorf), __typeof__(floorf)),
    "math.h:floorf declaration differs from oracle");

static __typeof__(floorf) *const slate_reference_floorf = &floorf;

extern long double slate_oracle_floorl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_floorl), __typeof__(floorl)),
    "math.h:floorl declaration differs from oracle");

static __typeof__(floorl) *const slate_reference_floorl = &floorl;

extern double slate_oracle_fma(double, double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fma), __typeof__(fma)),
    "math.h:fma declaration differs from oracle");

static __typeof__(fma) *const slate_reference_fma = &fma;

extern float slate_oracle_fmaf(float, float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaf), __typeof__(fmaf)),
    "math.h:fmaf declaration differs from oracle");

static __typeof__(fmaf) *const slate_reference_fmaf = &fmaf;

extern long double slate_oracle_fmal(long double, long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmal), __typeof__(fmal)),
    "math.h:fmal declaration differs from oracle");

static __typeof__(fmal) *const slate_reference_fmal = &fmal;

extern double slate_oracle_fmax(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmax), __typeof__(fmax)),
    "math.h:fmax declaration differs from oracle");

static __typeof__(fmax) *const slate_reference_fmax = &fmax;

extern float slate_oracle_fmaxf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaxf), __typeof__(fmaxf)),
    "math.h:fmaxf declaration differs from oracle");

static __typeof__(fmaxf) *const slate_reference_fmaxf = &fmaxf;

extern long double slate_oracle_fmaxl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaxl), __typeof__(fmaxl)),
    "math.h:fmaxl declaration differs from oracle");

static __typeof__(fmaxl) *const slate_reference_fmaxl = &fmaxl;

extern double slate_oracle_fmin(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmin), __typeof__(fmin)),
    "math.h:fmin declaration differs from oracle");

static __typeof__(fmin) *const slate_reference_fmin = &fmin;

extern float slate_oracle_fminf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminf), __typeof__(fminf)),
    "math.h:fminf declaration differs from oracle");

static __typeof__(fminf) *const slate_reference_fminf = &fminf;

extern long double slate_oracle_fminl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminl), __typeof__(fminl)),
    "math.h:fminl declaration differs from oracle");

static __typeof__(fminl) *const slate_reference_fminl = &fminl;

extern double slate_oracle_fmod(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmod), __typeof__(fmod)),
    "math.h:fmod declaration differs from oracle");

static __typeof__(fmod) *const slate_reference_fmod = &fmod;

extern float slate_oracle_fmodf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmodf), __typeof__(fmodf)),
    "math.h:fmodf declaration differs from oracle");

static __typeof__(fmodf) *const slate_reference_fmodf = &fmodf;

extern long double slate_oracle_fmodl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmodl), __typeof__(fmodl)),
    "math.h:fmodl declaration differs from oracle");

static __typeof__(fmodl) *const slate_reference_fmodl = &fmodl;

extern double slate_oracle_frexp(double, int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_frexp), __typeof__(frexp)),
    "math.h:frexp declaration differs from oracle");

static __typeof__(frexp) *const slate_reference_frexp = &frexp;

extern float slate_oracle_frexpf(float, int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_frexpf), __typeof__(frexpf)),
    "math.h:frexpf declaration differs from oracle");

static __typeof__(frexpf) *const slate_reference_frexpf = &frexpf;

extern long double slate_oracle_frexpl(long double, int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_frexpl), __typeof__(frexpl)),
    "math.h:frexpl declaration differs from oracle");

static __typeof__(frexpl) *const slate_reference_frexpl = &frexpl;

extern double slate_oracle_hypot(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_hypot), __typeof__(hypot)),
    "math.h:hypot declaration differs from oracle");

static __typeof__(hypot) *const slate_reference_hypot = &hypot;

extern float slate_oracle_hypotf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_hypotf), __typeof__(hypotf)),
    "math.h:hypotf declaration differs from oracle");

static __typeof__(hypotf) *const slate_reference_hypotf = &hypotf;

extern long double slate_oracle_hypotl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_hypotl), __typeof__(hypotl)),
    "math.h:hypotl declaration differs from oracle");

static __typeof__(hypotl) *const slate_reference_hypotl = &hypotl;

extern int slate_oracle_ilogb(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ilogb), __typeof__(ilogb)),
    "math.h:ilogb declaration differs from oracle");

static __typeof__(ilogb) *const slate_reference_ilogb = &ilogb;

extern int slate_oracle_ilogbf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ilogbf), __typeof__(ilogbf)),
    "math.h:ilogbf declaration differs from oracle");

static __typeof__(ilogbf) *const slate_reference_ilogbf = &ilogbf;

extern int slate_oracle_ilogbl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ilogbl), __typeof__(ilogbl)),
    "math.h:ilogbl declaration differs from oracle");

static __typeof__(ilogbl) *const slate_reference_ilogbl = &ilogbl;

extern double slate_oracle_j0(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_j0), __typeof__(j0)),
    "math.h:j0 declaration differs from oracle");

static __typeof__(j0) *const slate_reference_j0 = &j0;

extern double slate_oracle_j1(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_j1), __typeof__(j1)),
    "math.h:j1 declaration differs from oracle");

static __typeof__(j1) *const slate_reference_j1 = &j1;

extern double slate_oracle_jn(int, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_jn), __typeof__(jn)),
    "math.h:jn declaration differs from oracle");

static __typeof__(jn) *const slate_reference_jn = &jn;

extern long slate_oracle_labs(long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_labs), __typeof__(labs)),
    "math.h:labs declaration differs from oracle");

static __typeof__(labs) *const slate_reference_labs = &labs;

extern double slate_oracle_ldexp(double, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldexp), __typeof__(ldexp)),
    "math.h:ldexp declaration differs from oracle");

static __typeof__(ldexp) *const slate_reference_ldexp = &ldexp;

extern float slate_oracle_ldexpf(float, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldexpf), __typeof__(ldexpf)),
    "math.h:ldexpf declaration differs from oracle");

static __typeof__(ldexpf) *const slate_reference_ldexpf = &ldexpf;

extern long double slate_oracle_ldexpl(long double, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldexpl), __typeof__(ldexpl)),
    "math.h:ldexpl declaration differs from oracle");

static __typeof__(ldexpl) *const slate_reference_ldexpl = &ldexpl;

extern double slate_oracle_lgamma(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgamma), __typeof__(lgamma)),
    "math.h:lgamma declaration differs from oracle");

static __typeof__(lgamma) *const slate_reference_lgamma = &lgamma;

extern float slate_oracle_lgammaf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgammaf), __typeof__(lgammaf)),
    "math.h:lgammaf declaration differs from oracle");

static __typeof__(lgammaf) *const slate_reference_lgammaf = &lgammaf;

extern long double slate_oracle_lgammal(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgammal), __typeof__(lgammal)),
    "math.h:lgammal declaration differs from oracle");

static __typeof__(lgammal) *const slate_reference_lgammal = &lgammal;

extern long long slate_oracle_llabs(long long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llabs), __typeof__(llabs)),
    "math.h:llabs declaration differs from oracle");

static __typeof__(llabs) *const slate_reference_llabs = &llabs;

extern long long slate_oracle_llrint(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llrint), __typeof__(llrint)),
    "math.h:llrint declaration differs from oracle");

static __typeof__(llrint) *const slate_reference_llrint = &llrint;

extern long long slate_oracle_llrintf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llrintf), __typeof__(llrintf)),
    "math.h:llrintf declaration differs from oracle");

static __typeof__(llrintf) *const slate_reference_llrintf = &llrintf;

extern long long slate_oracle_llrintl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llrintl), __typeof__(llrintl)),
    "math.h:llrintl declaration differs from oracle");

static __typeof__(llrintl) *const slate_reference_llrintl = &llrintl;

extern long long slate_oracle_llround(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llround), __typeof__(llround)),
    "math.h:llround declaration differs from oracle");

static __typeof__(llround) *const slate_reference_llround = &llround;

extern long long slate_oracle_llroundf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llroundf), __typeof__(llroundf)),
    "math.h:llroundf declaration differs from oracle");

static __typeof__(llroundf) *const slate_reference_llroundf = &llroundf;

extern long long slate_oracle_llroundl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llroundl), __typeof__(llroundl)),
    "math.h:llroundl declaration differs from oracle");

static __typeof__(llroundl) *const slate_reference_llroundl = &llroundl;

extern double slate_oracle_log(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log), __typeof__(log)),
    "math.h:log declaration differs from oracle");

static __typeof__(log) *const slate_reference_log = &log;

extern double slate_oracle_log10(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log10), __typeof__(log10)),
    "math.h:log10 declaration differs from oracle");

static __typeof__(log10) *const slate_reference_log10 = &log10;

extern float slate_oracle_log10f(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log10f), __typeof__(log10f)),
    "math.h:log10f declaration differs from oracle");

static __typeof__(log10f) *const slate_reference_log10f = &log10f;

extern long double slate_oracle_log10l(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log10l), __typeof__(log10l)),
    "math.h:log10l declaration differs from oracle");

static __typeof__(log10l) *const slate_reference_log10l = &log10l;

extern double slate_oracle_log1p(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log1p), __typeof__(log1p)),
    "math.h:log1p declaration differs from oracle");

static __typeof__(log1p) *const slate_reference_log1p = &log1p;

extern float slate_oracle_log1pf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log1pf), __typeof__(log1pf)),
    "math.h:log1pf declaration differs from oracle");

static __typeof__(log1pf) *const slate_reference_log1pf = &log1pf;

extern long double slate_oracle_log1pl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log1pl), __typeof__(log1pl)),
    "math.h:log1pl declaration differs from oracle");

static __typeof__(log1pl) *const slate_reference_log1pl = &log1pl;

extern double slate_oracle_log2(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log2), __typeof__(log2)),
    "math.h:log2 declaration differs from oracle");

static __typeof__(log2) *const slate_reference_log2 = &log2;

extern float slate_oracle_log2f(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log2f), __typeof__(log2f)),
    "math.h:log2f declaration differs from oracle");

static __typeof__(log2f) *const slate_reference_log2f = &log2f;

extern long double slate_oracle_log2l(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log2l), __typeof__(log2l)),
    "math.h:log2l declaration differs from oracle");

static __typeof__(log2l) *const slate_reference_log2l = &log2l;

extern double slate_oracle_logb(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logb), __typeof__(logb)),
    "math.h:logb declaration differs from oracle");

static __typeof__(logb) *const slate_reference_logb = &logb;

extern float slate_oracle_logbf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logbf), __typeof__(logbf)),
    "math.h:logbf declaration differs from oracle");

static __typeof__(logbf) *const slate_reference_logbf = &logbf;

extern long double slate_oracle_logbl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logbl), __typeof__(logbl)),
    "math.h:logbl declaration differs from oracle");

static __typeof__(logbl) *const slate_reference_logbl = &logbl;

extern float slate_oracle_logf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logf), __typeof__(logf)),
    "math.h:logf declaration differs from oracle");

static __typeof__(logf) *const slate_reference_logf = &logf;

extern long double slate_oracle_logl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logl), __typeof__(logl)),
    "math.h:logl declaration differs from oracle");

static __typeof__(logl) *const slate_reference_logl = &logl;

extern long slate_oracle_lrint(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrint), __typeof__(lrint)),
    "math.h:lrint declaration differs from oracle");

static __typeof__(lrint) *const slate_reference_lrint = &lrint;

extern long slate_oracle_lrintf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrintf), __typeof__(lrintf)),
    "math.h:lrintf declaration differs from oracle");

static __typeof__(lrintf) *const slate_reference_lrintf = &lrintf;

extern long slate_oracle_lrintl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrintl), __typeof__(lrintl)),
    "math.h:lrintl declaration differs from oracle");

static __typeof__(lrintl) *const slate_reference_lrintl = &lrintl;

extern long slate_oracle_lround(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lround), __typeof__(lround)),
    "math.h:lround declaration differs from oracle");

static __typeof__(lround) *const slate_reference_lround = &lround;

extern long slate_oracle_lroundf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lroundf), __typeof__(lroundf)),
    "math.h:lroundf declaration differs from oracle");

static __typeof__(lroundf) *const slate_reference_lroundf = &lroundf;

extern long slate_oracle_lroundl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lroundl), __typeof__(lroundl)),
    "math.h:lroundl declaration differs from oracle");

static __typeof__(lroundl) *const slate_reference_lroundl = &lroundl;

extern double slate_oracle_modf(double, double *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_modf), __typeof__(modf)),
    "math.h:modf declaration differs from oracle");

static __typeof__(modf) *const slate_reference_modf = &modf;

extern float slate_oracle_modff(float, float *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_modff), __typeof__(modff)),
    "math.h:modff declaration differs from oracle");

static __typeof__(modff) *const slate_reference_modff = &modff;

extern long double slate_oracle_modfl(long double, long double *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_modfl), __typeof__(modfl)),
    "math.h:modfl declaration differs from oracle");

static __typeof__(modfl) *const slate_reference_modfl = &modfl;

extern double slate_oracle_nan(const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nan), __typeof__(nan)),
    "math.h:nan declaration differs from oracle");

static __typeof__(nan) *const slate_reference_nan = &nan;

extern float slate_oracle_nanf(const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nanf), __typeof__(nanf)),
    "math.h:nanf declaration differs from oracle");

static __typeof__(nanf) *const slate_reference_nanf = &nanf;

extern long double slate_oracle_nanl(const char *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nanl), __typeof__(nanl)),
    "math.h:nanl declaration differs from oracle");

static __typeof__(nanl) *const slate_reference_nanl = &nanl;

extern double slate_oracle_nearbyint(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nearbyint), __typeof__(nearbyint)),
    "math.h:nearbyint declaration differs from oracle");

static __typeof__(nearbyint) *const slate_reference_nearbyint = &nearbyint;

extern float slate_oracle_nearbyintf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nearbyintf), __typeof__(nearbyintf)),
    "math.h:nearbyintf declaration differs from oracle");

static __typeof__(nearbyintf) *const slate_reference_nearbyintf = &nearbyintf;

extern long double slate_oracle_nearbyintl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nearbyintl), __typeof__(nearbyintl)),
    "math.h:nearbyintl declaration differs from oracle");

static __typeof__(nearbyintl) *const slate_reference_nearbyintl = &nearbyintl;

extern double slate_oracle_nextafter(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextafter), __typeof__(nextafter)),
    "math.h:nextafter declaration differs from oracle");

static __typeof__(nextafter) *const slate_reference_nextafter = &nextafter;

extern float slate_oracle_nextafterf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextafterf), __typeof__(nextafterf)),
    "math.h:nextafterf declaration differs from oracle");

static __typeof__(nextafterf) *const slate_reference_nextafterf = &nextafterf;

extern long double slate_oracle_nextafterl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextafterl), __typeof__(nextafterl)),
    "math.h:nextafterl declaration differs from oracle");

static __typeof__(nextafterl) *const slate_reference_nextafterl = &nextafterl;

extern double slate_oracle_nexttoward(double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nexttoward), __typeof__(nexttoward)),
    "math.h:nexttoward declaration differs from oracle");

static __typeof__(nexttoward) *const slate_reference_nexttoward = &nexttoward;

extern float slate_oracle_nexttowardf(float, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nexttowardf), __typeof__(nexttowardf)),
    "math.h:nexttowardf declaration differs from oracle");

static __typeof__(nexttowardf) *const slate_reference_nexttowardf = &nexttowardf;

extern long double slate_oracle_nexttowardl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nexttowardl), __typeof__(nexttowardl)),
    "math.h:nexttowardl declaration differs from oracle");

static __typeof__(nexttowardl) *const slate_reference_nexttowardl = &nexttowardl;

extern double slate_oracle_pow(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pow), __typeof__(pow)),
    "math.h:pow declaration differs from oracle");

static __typeof__(pow) *const slate_reference_pow = &pow;

extern float slate_oracle_powf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_powf), __typeof__(powf)),
    "math.h:powf declaration differs from oracle");

static __typeof__(powf) *const slate_reference_powf = &powf;

extern long double slate_oracle_powl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_powl), __typeof__(powl)),
    "math.h:powl declaration differs from oracle");

static __typeof__(powl) *const slate_reference_powl = &powl;

extern double slate_oracle_remainder(double, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remainder), __typeof__(remainder)),
    "math.h:remainder declaration differs from oracle");

static __typeof__(remainder) *const slate_reference_remainder = &remainder;

extern float slate_oracle_remainderf(float, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remainderf), __typeof__(remainderf)),
    "math.h:remainderf declaration differs from oracle");

static __typeof__(remainderf) *const slate_reference_remainderf = &remainderf;

extern long double slate_oracle_remainderl(long double, long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remainderl), __typeof__(remainderl)),
    "math.h:remainderl declaration differs from oracle");

static __typeof__(remainderl) *const slate_reference_remainderl = &remainderl;

extern double slate_oracle_remquo(double, double, int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remquo), __typeof__(remquo)),
    "math.h:remquo declaration differs from oracle");

static __typeof__(remquo) *const slate_reference_remquo = &remquo;

extern float slate_oracle_remquof(float, float, int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remquof), __typeof__(remquof)),
    "math.h:remquof declaration differs from oracle");

static __typeof__(remquof) *const slate_reference_remquof = &remquof;

extern long double slate_oracle_remquol(long double, long double, int *);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remquol), __typeof__(remquol)),
    "math.h:remquol declaration differs from oracle");

static __typeof__(remquol) *const slate_reference_remquol = &remquol;

extern double slate_oracle_rint(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rint), __typeof__(rint)),
    "math.h:rint declaration differs from oracle");

static __typeof__(rint) *const slate_reference_rint = &rint;

extern float slate_oracle_rintf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rintf), __typeof__(rintf)),
    "math.h:rintf declaration differs from oracle");

static __typeof__(rintf) *const slate_reference_rintf = &rintf;

extern long double slate_oracle_rintl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rintl), __typeof__(rintl)),
    "math.h:rintl declaration differs from oracle");

static __typeof__(rintl) *const slate_reference_rintl = &rintl;

extern double slate_oracle_round(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_round), __typeof__(round)),
    "math.h:round declaration differs from oracle");

static __typeof__(round) *const slate_reference_round = &round;

extern float slate_oracle_roundf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_roundf), __typeof__(roundf)),
    "math.h:roundf declaration differs from oracle");

static __typeof__(roundf) *const slate_reference_roundf = &roundf;

extern long double slate_oracle_roundl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_roundl), __typeof__(roundl)),
    "math.h:roundl declaration differs from oracle");

static __typeof__(roundl) *const slate_reference_roundl = &roundl;

extern double slate_oracle_scalbln(double, long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbln), __typeof__(scalbln)),
    "math.h:scalbln declaration differs from oracle");

static __typeof__(scalbln) *const slate_reference_scalbln = &scalbln;

extern float slate_oracle_scalblnf(float, long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalblnf), __typeof__(scalblnf)),
    "math.h:scalblnf declaration differs from oracle");

static __typeof__(scalblnf) *const slate_reference_scalblnf = &scalblnf;

extern long double slate_oracle_scalblnl(long double, long);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalblnl), __typeof__(scalblnl)),
    "math.h:scalblnl declaration differs from oracle");

static __typeof__(scalblnl) *const slate_reference_scalblnl = &scalblnl;

extern double slate_oracle_scalbn(double, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbn), __typeof__(scalbn)),
    "math.h:scalbn declaration differs from oracle");

static __typeof__(scalbn) *const slate_reference_scalbn = &scalbn;

extern float slate_oracle_scalbnf(float, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbnf), __typeof__(scalbnf)),
    "math.h:scalbnf declaration differs from oracle");

static __typeof__(scalbnf) *const slate_reference_scalbnf = &scalbnf;

extern long double slate_oracle_scalbnl(long double, int);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbnl), __typeof__(scalbnl)),
    "math.h:scalbnl declaration differs from oracle");

static __typeof__(scalbnl) *const slate_reference_scalbnl = &scalbnl;

extern double slate_oracle_sin(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sin), __typeof__(sin)),
    "math.h:sin declaration differs from oracle");

static __typeof__(sin) *const slate_reference_sin = &sin;

extern float slate_oracle_sinf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinf), __typeof__(sinf)),
    "math.h:sinf declaration differs from oracle");

static __typeof__(sinf) *const slate_reference_sinf = &sinf;

extern double slate_oracle_sinh(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinh), __typeof__(sinh)),
    "math.h:sinh declaration differs from oracle");

static __typeof__(sinh) *const slate_reference_sinh = &sinh;

extern float slate_oracle_sinhf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinhf), __typeof__(sinhf)),
    "math.h:sinhf declaration differs from oracle");

static __typeof__(sinhf) *const slate_reference_sinhf = &sinhf;

extern long double slate_oracle_sinhl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinhl), __typeof__(sinhl)),
    "math.h:sinhl declaration differs from oracle");

static __typeof__(sinhl) *const slate_reference_sinhl = &sinhl;

extern long double slate_oracle_sinl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinl), __typeof__(sinl)),
    "math.h:sinl declaration differs from oracle");

static __typeof__(sinl) *const slate_reference_sinl = &sinl;

extern double slate_oracle_sqrt(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sqrt), __typeof__(sqrt)),
    "math.h:sqrt declaration differs from oracle");

static __typeof__(sqrt) *const slate_reference_sqrt = &sqrt;

extern float slate_oracle_sqrtf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sqrtf), __typeof__(sqrtf)),
    "math.h:sqrtf declaration differs from oracle");

static __typeof__(sqrtf) *const slate_reference_sqrtf = &sqrtf;

extern long double slate_oracle_sqrtl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sqrtl), __typeof__(sqrtl)),
    "math.h:sqrtl declaration differs from oracle");

static __typeof__(sqrtl) *const slate_reference_sqrtl = &sqrtl;

extern double slate_oracle_tan(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tan), __typeof__(tan)),
    "math.h:tan declaration differs from oracle");

static __typeof__(tan) *const slate_reference_tan = &tan;

extern float slate_oracle_tanf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanf), __typeof__(tanf)),
    "math.h:tanf declaration differs from oracle");

static __typeof__(tanf) *const slate_reference_tanf = &tanf;

extern double slate_oracle_tanh(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanh), __typeof__(tanh)),
    "math.h:tanh declaration differs from oracle");

static __typeof__(tanh) *const slate_reference_tanh = &tanh;

extern float slate_oracle_tanhf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanhf), __typeof__(tanhf)),
    "math.h:tanhf declaration differs from oracle");

static __typeof__(tanhf) *const slate_reference_tanhf = &tanhf;

extern long double slate_oracle_tanhl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanhl), __typeof__(tanhl)),
    "math.h:tanhl declaration differs from oracle");

static __typeof__(tanhl) *const slate_reference_tanhl = &tanhl;

extern long double slate_oracle_tanl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanl), __typeof__(tanl)),
    "math.h:tanl declaration differs from oracle");

static __typeof__(tanl) *const slate_reference_tanl = &tanl;

extern double slate_oracle_tgamma(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgamma), __typeof__(tgamma)),
    "math.h:tgamma declaration differs from oracle");

static __typeof__(tgamma) *const slate_reference_tgamma = &tgamma;

extern float slate_oracle_tgammaf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgammaf), __typeof__(tgammaf)),
    "math.h:tgammaf declaration differs from oracle");

static __typeof__(tgammaf) *const slate_reference_tgammaf = &tgammaf;

extern long double slate_oracle_tgammal(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgammal), __typeof__(tgammal)),
    "math.h:tgammal declaration differs from oracle");

static __typeof__(tgammal) *const slate_reference_tgammal = &tgammal;

extern double slate_oracle_trunc(double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_trunc), __typeof__(trunc)),
    "math.h:trunc declaration differs from oracle");

static __typeof__(trunc) *const slate_reference_trunc = &trunc;

extern float slate_oracle_truncf(float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_truncf), __typeof__(truncf)),
    "math.h:truncf declaration differs from oracle");

static __typeof__(truncf) *const slate_reference_truncf = &truncf;

extern long double slate_oracle_truncl(long double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_truncl), __typeof__(truncl)),
    "math.h:truncl declaration differs from oracle");

static __typeof__(truncl) *const slate_reference_truncl = &truncl;

extern double slate_oracle_y0(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_y0), __typeof__(y0)),
    "math.h:y0 declaration differs from oracle");

static __typeof__(y0) *const slate_reference_y0 = &y0;

extern double slate_oracle_y1(double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_y1), __typeof__(y1)),
    "math.h:y1 declaration differs from oracle");

static __typeof__(y1) *const slate_reference_y1 = &y1;

extern double slate_oracle_yn(int, double) __attribute__((cdecl));

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_yn), __typeof__(yn)),
    "math.h:yn declaration differs from oracle");

static __typeof__(yn) *const slate_reference_yn = &yn;

extern double slate_oracle_HUGE;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_HUGE), __typeof__(HUGE)), "HUGE object type differs from oracle");

static __typeof__(HUGE) *const slate_reference_HUGE = &HUGE;

extern const union _float_const slate_oracle__Denorm_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Denorm_C), __typeof__(_Denorm_C)), "_Denorm_C object type differs from oracle");

static __typeof__(_Denorm_C) *const slate_reference__Denorm_C = &_Denorm_C;

extern const union _float_const slate_oracle__Eps_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Eps_C), __typeof__(_Eps_C)), "_Eps_C object type differs from oracle");

static __typeof__(_Eps_C) *const slate_reference__Eps_C = &_Eps_C;

extern const union _float_const slate_oracle__FDenorm_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__FDenorm_C), __typeof__(_FDenorm_C)), "_FDenorm_C object type differs from oracle");

static __typeof__(_FDenorm_C) *const slate_reference__FDenorm_C = &_FDenorm_C;

extern const union _float_const slate_oracle__FEps_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__FEps_C), __typeof__(_FEps_C)), "_FEps_C object type differs from oracle");

static __typeof__(_FEps_C) *const slate_reference__FEps_C = &_FEps_C;

extern const union _float_const slate_oracle__FInf_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__FInf_C), __typeof__(_FInf_C)), "_FInf_C object type differs from oracle");

static __typeof__(_FInf_C) *const slate_reference__FInf_C = &_FInf_C;

extern const union _float_const slate_oracle__FNan_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__FNan_C), __typeof__(_FNan_C)), "_FNan_C object type differs from oracle");

static __typeof__(_FNan_C) *const slate_reference__FNan_C = &_FNan_C;

extern const union _float_const slate_oracle__FRteps_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__FRteps_C), __typeof__(_FRteps_C)), "_FRteps_C object type differs from oracle");

static __typeof__(_FRteps_C) *const slate_reference__FRteps_C = &_FRteps_C;

extern const union _float_const slate_oracle__FSnan_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__FSnan_C), __typeof__(_FSnan_C)), "_FSnan_C object type differs from oracle");

static __typeof__(_FSnan_C) *const slate_reference__FSnan_C = &_FSnan_C;

extern const float slate_oracle__FXbig_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__FXbig_C), __typeof__(_FXbig_C)), "_FXbig_C object type differs from oracle");

static __typeof__(_FXbig_C) *const slate_reference__FXbig_C = &_FXbig_C;

extern const float slate_oracle__FZero_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__FZero_C), __typeof__(_FZero_C)), "_FZero_C object type differs from oracle");

static __typeof__(_FZero_C) *const slate_reference__FZero_C = &_FZero_C;

extern const double slate_oracle__HUGE;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__HUGE), __typeof__(_HUGE)), "_HUGE object type differs from oracle");

static __typeof__(_HUGE) *const slate_reference__HUGE = &_HUGE;

extern const union _float_const slate_oracle__Hugeval_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Hugeval_C), __typeof__(_Hugeval_C)), "_Hugeval_C object type differs from oracle");

static __typeof__(_Hugeval_C) *const slate_reference__Hugeval_C = &_Hugeval_C;

extern const union _float_const slate_oracle__Inf_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Inf_C), __typeof__(_Inf_C)), "_Inf_C object type differs from oracle");

static __typeof__(_Inf_C) *const slate_reference__Inf_C = &_Inf_C;

extern const union _float_const slate_oracle__LDenorm_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__LDenorm_C), __typeof__(_LDenorm_C)), "_LDenorm_C object type differs from oracle");

static __typeof__(_LDenorm_C) *const slate_reference__LDenorm_C = &_LDenorm_C;

extern const union _float_const slate_oracle__LEps_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__LEps_C), __typeof__(_LEps_C)), "_LEps_C object type differs from oracle");

static __typeof__(_LEps_C) *const slate_reference__LEps_C = &_LEps_C;

extern const union _float_const slate_oracle__LInf_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__LInf_C), __typeof__(_LInf_C)), "_LInf_C object type differs from oracle");

static __typeof__(_LInf_C) *const slate_reference__LInf_C = &_LInf_C;

extern const union _float_const slate_oracle__LNan_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__LNan_C), __typeof__(_LNan_C)), "_LNan_C object type differs from oracle");

static __typeof__(_LNan_C) *const slate_reference__LNan_C = &_LNan_C;

extern const union _float_const slate_oracle__LRteps_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__LRteps_C), __typeof__(_LRteps_C)), "_LRteps_C object type differs from oracle");

static __typeof__(_LRteps_C) *const slate_reference__LRteps_C = &_LRteps_C;

extern const union _float_const slate_oracle__LSnan_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__LSnan_C), __typeof__(_LSnan_C)), "_LSnan_C object type differs from oracle");

static __typeof__(_LSnan_C) *const slate_reference__LSnan_C = &_LSnan_C;

extern const long double slate_oracle__LXbig_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__LXbig_C), __typeof__(_LXbig_C)), "_LXbig_C object type differs from oracle");

static __typeof__(_LXbig_C) *const slate_reference__LXbig_C = &_LXbig_C;

extern const long double slate_oracle__LZero_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__LZero_C), __typeof__(_LZero_C)), "_LZero_C object type differs from oracle");

static __typeof__(_LZero_C) *const slate_reference__LZero_C = &_LZero_C;

extern const union _float_const slate_oracle__Nan_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Nan_C), __typeof__(_Nan_C)), "_Nan_C object type differs from oracle");

static __typeof__(_Nan_C) *const slate_reference__Nan_C = &_Nan_C;

extern const union _float_const slate_oracle__Rteps_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Rteps_C), __typeof__(_Rteps_C)), "_Rteps_C object type differs from oracle");

static __typeof__(_Rteps_C) *const slate_reference__Rteps_C = &_Rteps_C;

extern const union _float_const slate_oracle__Snan_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Snan_C), __typeof__(_Snan_C)), "_Snan_C object type differs from oracle");

static __typeof__(_Snan_C) *const slate_reference__Snan_C = &_Snan_C;

extern const double slate_oracle__Xbig_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Xbig_C), __typeof__(_Xbig_C)), "_Xbig_C object type differs from oracle");

static __typeof__(_Xbig_C) *const slate_reference__Xbig_C = &_Xbig_C;

extern const double slate_oracle__Zero_C;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle__Zero_C), __typeof__(_Zero_C)), "_Zero_C object type differs from oracle");

static __typeof__(_Zero_C) *const slate_reference__Zero_C = &_Zero_C;

typedef double slate_oracle_typedef_double_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_double_t, double_t), "typedef double_t differs from oracle");

typedef float slate_oracle_typedef_float_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_float_t, float_t), "typedef float_t differs from oracle");

#ifndef DOMAIN
#error "math.h:DOMAIN macro is missing from libc-shim"
#endif

#ifndef FP_ILOGB0
#error "math.h:FP_ILOGB0 macro is missing from libc-shim"
#endif

#ifndef FP_ILOGBNAN
#error "math.h:FP_ILOGBNAN macro is missing from libc-shim"
#endif

#ifndef FP_INFINITE
#error "math.h:FP_INFINITE macro is missing from libc-shim"
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

#ifndef INFINITY
#error "math.h:INFINITY macro is missing from libc-shim"
#endif

#ifndef MATH_ERREXCEPT
#error "math.h:MATH_ERREXCEPT macro is missing from libc-shim"
#endif

#ifndef MATH_ERRNO
#error "math.h:MATH_ERRNO macro is missing from libc-shim"
#endif

#ifndef NAN
#error "math.h:NAN macro is missing from libc-shim"
#endif

#ifndef OVERFLOW
#error "math.h:OVERFLOW macro is missing from libc-shim"
#endif

#ifndef PLOSS
#error "math.h:PLOSS macro is missing from libc-shim"
#endif

#ifndef SING
#error "math.h:SING macro is missing from libc-shim"
#endif

#ifndef TLOSS
#error "math.h:TLOSS macro is missing from libc-shim"
#endif

#ifndef UNDERFLOW
#error "math.h:UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef _C2
#error "math.h:_C2 macro is missing from libc-shim"
#endif

#ifndef _CLASSIFY
#error "math.h:_CLASSIFY macro is missing from libc-shim"
#endif

#ifndef _CLASSIFY2
#error "math.h:_CLASSIFY2 macro is missing from libc-shim"
#endif

#ifndef _CLASS_ARG
#error "math.h:_CLASS_ARG macro is missing from libc-shim"
#endif

#ifndef _COMPLEX_DEFINED
#error "math.h:_COMPLEX_DEFINED macro is missing from libc-shim"
#endif

#ifndef _D0_C
#error "math.h:_D0_C macro is missing from libc-shim"
#endif

#ifndef _D1_C
#error "math.h:_D1_C macro is missing from libc-shim"
#endif

#ifndef _D2_C
#error "math.h:_D2_C macro is missing from libc-shim"
#endif

#ifndef _D3_C
#error "math.h:_D3_C macro is missing from libc-shim"
#endif

#ifndef _DBIAS
#error "math.h:_DBIAS macro is missing from libc-shim"
#endif

#ifndef _DENORM
#error "math.h:_DENORM macro is missing from libc-shim"
#endif

#ifndef _DFRAC
#error "math.h:_DFRAC macro is missing from libc-shim"
#endif

#ifndef _DHUGE_EXP
#error "math.h:_DHUGE_EXP macro is missing from libc-shim"
#endif

#ifndef _DMASK
#error "math.h:_DMASK macro is missing from libc-shim"
#endif

#ifndef _DMAX
#error "math.h:_DMAX macro is missing from libc-shim"
#endif

#ifndef _DOFF
#error "math.h:_DOFF macro is missing from libc-shim"
#endif

#ifndef _DOMAIN
#error "math.h:_DOMAIN macro is missing from libc-shim"
#endif

#ifndef _DSIGN
#error "math.h:_DSIGN macro is missing from libc-shim"
#endif

#ifndef _DSIGN_C
#error "math.h:_DSIGN_C macro is missing from libc-shim"
#endif

#ifndef _F0_C
#error "math.h:_F0_C macro is missing from libc-shim"
#endif

#ifndef _F1_C
#error "math.h:_F1_C macro is missing from libc-shim"
#endif

#ifndef _FBIAS
#error "math.h:_FBIAS macro is missing from libc-shim"
#endif

#ifndef _FE_DIVBYZERO
#error "math.h:_FE_DIVBYZERO macro is missing from libc-shim"
#endif

#ifndef _FE_INEXACT
#error "math.h:_FE_INEXACT macro is missing from libc-shim"
#endif

#ifndef _FE_INVALID
#error "math.h:_FE_INVALID macro is missing from libc-shim"
#endif

#ifndef _FE_OVERFLOW
#error "math.h:_FE_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef _FE_UNDERFLOW
#error "math.h:_FE_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef _FFRAC
#error "math.h:_FFRAC macro is missing from libc-shim"
#endif

#ifndef _FHUGE_EXP
#error "math.h:_FHUGE_EXP macro is missing from libc-shim"
#endif

#ifndef _FINITE
#error "math.h:_FINITE macro is missing from libc-shim"
#endif

#ifndef _FMASK
#error "math.h:_FMASK macro is missing from libc-shim"
#endif

#ifndef _FMAX
#error "math.h:_FMAX macro is missing from libc-shim"
#endif

#ifndef _FOFF
#error "math.h:_FOFF macro is missing from libc-shim"
#endif

#ifndef _FPCOMPARE
#error "math.h:_FPCOMPARE macro is missing from libc-shim"
#endif

#ifndef _FP_EQ
#error "math.h:_FP_EQ macro is missing from libc-shim"
#endif

#ifndef _FP_GT
#error "math.h:_FP_GT macro is missing from libc-shim"
#endif

#ifndef _FP_LT
#error "math.h:_FP_LT macro is missing from libc-shim"
#endif

#ifndef _FRND
#error "math.h:_FRND macro is missing from libc-shim"
#endif

#ifndef _FSIGN
#error "math.h:_FSIGN macro is missing from libc-shim"
#endif

#ifndef _FSIGN_C
#error "math.h:_FSIGN_C macro is missing from libc-shim"
#endif

#ifndef _HUGE_ENUF
#error "math.h:_HUGE_ENUF macro is missing from libc-shim"
#endif

#ifndef _INC_MATH
#error "math.h:_INC_MATH macro is missing from libc-shim"
#endif

#ifndef _INFCODE
#error "math.h:_INFCODE macro is missing from libc-shim"
#endif

#ifndef _L0_C
#error "math.h:_L0_C macro is missing from libc-shim"
#endif

#ifndef _L1_C
#error "math.h:_L1_C macro is missing from libc-shim"
#endif

#ifndef _L2_C
#error "math.h:_L2_C macro is missing from libc-shim"
#endif

#ifndef _L3_C
#error "math.h:_L3_C macro is missing from libc-shim"
#endif

#ifndef _LBIAS
#error "math.h:_LBIAS macro is missing from libc-shim"
#endif

#ifndef _LFRAC
#error "math.h:_LFRAC macro is missing from libc-shim"
#endif

#ifndef _LHUGE_EXP
#error "math.h:_LHUGE_EXP macro is missing from libc-shim"
#endif

#ifndef _LMASK
#error "math.h:_LMASK macro is missing from libc-shim"
#endif

#ifndef _LMAX
#error "math.h:_LMAX macro is missing from libc-shim"
#endif

#ifndef _LOFF
#error "math.h:_LOFF macro is missing from libc-shim"
#endif

#ifndef _LSIGN
#error "math.h:_LSIGN macro is missing from libc-shim"
#endif

#ifndef _LSIGN_C
#error "math.h:_LSIGN_C macro is missing from libc-shim"
#endif

#ifndef _NANCODE
#error "math.h:_NANCODE macro is missing from libc-shim"
#endif

#ifndef _OVERFLOW
#error "math.h:_OVERFLOW macro is missing from libc-shim"
#endif

#ifndef _PLOSS
#error "math.h:_PLOSS macro is missing from libc-shim"
#endif

#ifndef _SING
#error "math.h:_SING macro is missing from libc-shim"
#endif

#ifndef _TLOSS
#error "math.h:_TLOSS macro is missing from libc-shim"
#endif

#ifndef _UNDERFLOW
#error "math.h:_UNDERFLOW macro is missing from libc-shim"
#endif

#ifndef _matherrl
#error "math.h:_matherrl macro is missing from libc-shim"
#endif

#ifndef complex
#error "math.h:complex macro is missing from libc-shim"
#endif

#ifndef fpclassify
#error "math.h:fpclassify macro is missing from libc-shim"
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

#ifndef isunordered
#error "math.h:isunordered macro is missing from libc-shim"
#endif

#ifndef math_errhandling
#error "math.h:math_errhandling macro is missing from libc-shim"
#endif

#ifndef matherr
#error "math.h:matherr macro is missing from libc-shim"
#endif

#ifndef signbit
#error "math.h:signbit macro is missing from libc-shim"
#endif

int main(void) { return 0; }
