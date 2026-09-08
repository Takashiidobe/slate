#include <math.h>

extern double slate_oracle_acos(double);
extern double slate_oracle_acosh(double);
extern double slate_oracle_acospi(double);
extern double slate_oracle_asin(double);
extern double slate_oracle_asinh(double);
extern double slate_oracle_asinpi(double);
extern double slate_oracle_atan(double);
extern double slate_oracle_atan2(double, double);
extern double slate_oracle_atan2pi(double, double);
extern double slate_oracle_atanh(double);
extern double slate_oracle_atanpi(double);
extern int slate_oracle_canonicalize(double *, const double *);
extern double slate_oracle_cbrt(double);
extern double slate_oracle_ceil(double);
extern double slate_oracle_compoundn(double, long long);
extern double slate_oracle_copysign(double, double);
extern double slate_oracle_cos(double);
extern double slate_oracle_cosh(double);
extern double slate_oracle_cospi(double);
extern double slate_oracle_drem(double, double);
extern double slate_oracle_erf(double);
extern double slate_oracle_erfc(double);
extern double slate_oracle_exp(double);
extern double slate_oracle_exp10(double);
extern double slate_oracle_exp10m1(double);
extern double slate_oracle_exp2(double);
extern double slate_oracle_exp2m1(double);
extern double slate_oracle_expm1(double);
extern double slate_oracle_fabs(double);
extern double slate_oracle_fdim(double, double);
extern int slate_oracle_finite(double);
extern double slate_oracle_floor(double);
extern double slate_oracle_fma(double, double, double);
extern double slate_oracle_fmax(double, double);
extern double slate_oracle_fmaximum(double, double);
extern double slate_oracle_fmaximum_mag(double, double);
extern double slate_oracle_fmaximum_mag_num(double, double);
extern double slate_oracle_fmaximum_num(double, double);
extern double slate_oracle_fmaxmag(double, double);
extern double slate_oracle_fmin(double, double);
extern double slate_oracle_fminimum(double, double);
extern double slate_oracle_fminimum_mag(double, double);
extern double slate_oracle_fminimum_mag_num(double, double);
extern double slate_oracle_fminimum_num(double, double);
extern double slate_oracle_fminmag(double, double);
extern double slate_oracle_fmod(double, double);
extern double slate_oracle_frexp(double, int *);
extern double slate_oracle_fromfp(double, int, unsigned int);
extern double slate_oracle_fromfpx(double, int, unsigned int);
extern double slate_oracle_gamma(double);
extern double slate_oracle_getpayload(const double *);
extern double slate_oracle_hypot(double, double);
extern int slate_oracle_ilogb(double);
extern int slate_oracle_isinf(double);
extern int slate_oracle_isnan(double);
extern double slate_oracle_j0(double);
extern double slate_oracle_j1(double);
extern double slate_oracle_jn(int, double);
extern double slate_oracle_ldexp(double, int);
extern double slate_oracle_lgamma(double);
extern long slate_oracle_llogb(double);
extern long long slate_oracle_llrint(double);
extern long long slate_oracle_llround(double);
extern double slate_oracle_log(double);
extern double slate_oracle_log10(double);
extern double slate_oracle_log10p1(double);
extern double slate_oracle_log1p(double);
extern double slate_oracle_log2(double);
extern double slate_oracle_log2p1(double);
extern double slate_oracle_logb(double);
extern double slate_oracle_logp1(double);
extern long slate_oracle_lrint(double);
extern long slate_oracle_lround(double);
extern double slate_oracle_modf(double, double *);
extern double slate_oracle_nan(const char *);
extern double slate_oracle_nearbyint(double);
extern double slate_oracle_nextafter(double, double);
extern double slate_oracle_nextdown(double);
extern double slate_oracle_nexttoward(double, long double);
extern double slate_oracle_nextup(double);
extern double slate_oracle_pow(double, double);
extern double slate_oracle_pown(double, long long);
extern double slate_oracle_powr(double, double);
extern double slate_oracle_remainder(double, double);
extern double slate_oracle_remquo(double, double, int *);
extern double slate_oracle_rint(double);
extern double slate_oracle_rootn(double, long long);
extern double slate_oracle_round(double);
extern double slate_oracle_roundeven(double);
extern double slate_oracle_rsqrt(double);
extern double slate_oracle_scalb(double, double);
extern double slate_oracle_scalbln(double, long);
extern double slate_oracle_scalbn(double, int);
extern int slate_oracle_setpayload(double *, double);
extern int slate_oracle_setpayloadsig(double *, double);
extern double slate_oracle_significand(double);
extern double slate_oracle_sin(double);
extern void slate_oracle_sincos(double, double *, double *);
extern double slate_oracle_sinh(double);
extern double slate_oracle_sinpi(double);
extern double slate_oracle_sqrt(double);
extern double slate_oracle_tan(double);
extern double slate_oracle_tanh(double);
extern double slate_oracle_tanpi(double);
extern double slate_oracle_tgamma(double);
extern int slate_oracle_totalorder(const double *, const double *);
extern int slate_oracle_totalordermag(const double *, const double *);
extern double slate_oracle_trunc(double);
extern double slate_oracle_ufromfp(double, int, unsigned int);
extern double slate_oracle_ufromfpx(double, int, unsigned int);
extern double slate_oracle_y0(double);
extern double slate_oracle_y1(double);
extern double slate_oracle_yn(int, double);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acos), __typeof__(acos)),
    "math.h:acos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acosh), __typeof__(acosh)),
    "math.h:acosh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acospi), __typeof__(acospi)),
    "math.h:acospi declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asin), __typeof__(asin)),
    "math.h:asin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinh), __typeof__(asinh)),
    "math.h:asinh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinpi), __typeof__(asinpi)),
    "math.h:asinpi declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan), __typeof__(atan)),
    "math.h:atan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan2), __typeof__(atan2)),
    "math.h:atan2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan2pi), __typeof__(atan2pi)),
    "math.h:atan2pi declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanh), __typeof__(atanh)),
    "math.h:atanh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanpi), __typeof__(atanpi)),
    "math.h:atanpi declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_canonicalize), __typeof__(canonicalize)),
    "math.h:canonicalize declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cbrt), __typeof__(cbrt)),
    "math.h:cbrt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ceil), __typeof__(ceil)),
    "math.h:ceil declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_compoundn), __typeof__(compoundn)),
    "math.h:compoundn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_copysign), __typeof__(copysign)),
    "math.h:copysign declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cos), __typeof__(cos)),
    "math.h:cos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cosh), __typeof__(cosh)),
    "math.h:cosh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cospi), __typeof__(cospi)),
    "math.h:cospi declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_drem), __typeof__(drem)),
    "math.h:drem declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erf), __typeof__(erf)),
    "math.h:erf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfc), __typeof__(erfc)),
    "math.h:erfc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp), __typeof__(exp)),
    "math.h:exp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp10), __typeof__(exp10)),
    "math.h:exp10 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp10m1), __typeof__(exp10m1)),
    "math.h:exp10m1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp2), __typeof__(exp2)),
    "math.h:exp2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp2m1), __typeof__(exp2m1)),
    "math.h:exp2m1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expm1), __typeof__(expm1)),
    "math.h:expm1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fabs), __typeof__(fabs)),
    "math.h:fabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdim), __typeof__(fdim)),
    "math.h:fdim declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_finite), __typeof__(finite)),
    "math.h:finite declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_floor), __typeof__(floor)),
    "math.h:floor declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fma), __typeof__(fma)),
    "math.h:fma declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmax), __typeof__(fmax)),
    "math.h:fmax declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaximum), __typeof__(fmaximum)),
    "math.h:fmaximum declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaximum_mag), __typeof__(fmaximum_mag)),
    "math.h:fmaximum_mag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaximum_mag_num), __typeof__(fmaximum_mag_num)),
    "math.h:fmaximum_mag_num declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaximum_num), __typeof__(fmaximum_num)),
    "math.h:fmaximum_num declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaxmag), __typeof__(fmaxmag)),
    "math.h:fmaxmag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmin), __typeof__(fmin)),
    "math.h:fmin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminimum), __typeof__(fminimum)),
    "math.h:fminimum declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminimum_mag), __typeof__(fminimum_mag)),
    "math.h:fminimum_mag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminimum_mag_num), __typeof__(fminimum_mag_num)),
    "math.h:fminimum_mag_num declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminimum_num), __typeof__(fminimum_num)),
    "math.h:fminimum_num declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminmag), __typeof__(fminmag)),
    "math.h:fminmag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmod), __typeof__(fmod)),
    "math.h:fmod declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_frexp), __typeof__(frexp)),
    "math.h:frexp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fromfp), __typeof__(fromfp)),
    "math.h:fromfp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fromfpx), __typeof__(fromfpx)),
    "math.h:fromfpx declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_gamma), __typeof__(gamma)),
    "math.h:gamma declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_getpayload), __typeof__(getpayload)),
    "math.h:getpayload declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_hypot), __typeof__(hypot)),
    "math.h:hypot declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ilogb), __typeof__(ilogb)),
    "math.h:ilogb declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isinf), __typeof__(isinf)),
    "math.h:isinf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_isnan), __typeof__(isnan)),
    "math.h:isnan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_j0), __typeof__(j0)),
    "math.h:j0 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_j1), __typeof__(j1)),
    "math.h:j1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_jn), __typeof__(jn)),
    "math.h:jn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldexp), __typeof__(ldexp)),
    "math.h:ldexp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgamma), __typeof__(lgamma)),
    "math.h:lgamma declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llogb), __typeof__(llogb)),
    "math.h:llogb declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llrint), __typeof__(llrint)),
    "math.h:llrint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llround), __typeof__(llround)),
    "math.h:llround declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log), __typeof__(log)),
    "math.h:log declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log10), __typeof__(log10)),
    "math.h:log10 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log10p1), __typeof__(log10p1)),
    "math.h:log10p1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log1p), __typeof__(log1p)),
    "math.h:log1p declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log2), __typeof__(log2)),
    "math.h:log2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log2p1), __typeof__(log2p1)),
    "math.h:log2p1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logb), __typeof__(logb)),
    "math.h:logb declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logp1), __typeof__(logp1)),
    "math.h:logp1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrint), __typeof__(lrint)),
    "math.h:lrint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lround), __typeof__(lround)),
    "math.h:lround declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_modf), __typeof__(modf)),
    "math.h:modf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nan), __typeof__(nan)),
    "math.h:nan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nearbyint), __typeof__(nearbyint)),
    "math.h:nearbyint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextafter), __typeof__(nextafter)),
    "math.h:nextafter declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextdown), __typeof__(nextdown)),
    "math.h:nextdown declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nexttoward), __typeof__(nexttoward)),
    "math.h:nexttoward declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextup), __typeof__(nextup)),
    "math.h:nextup declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pow), __typeof__(pow)),
    "math.h:pow declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pown), __typeof__(pown)),
    "math.h:pown declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_powr), __typeof__(powr)),
    "math.h:powr declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remainder), __typeof__(remainder)),
    "math.h:remainder declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remquo), __typeof__(remquo)),
    "math.h:remquo declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rint), __typeof__(rint)),
    "math.h:rint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rootn), __typeof__(rootn)),
    "math.h:rootn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_round), __typeof__(round)),
    "math.h:round declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_roundeven), __typeof__(roundeven)),
    "math.h:roundeven declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rsqrt), __typeof__(rsqrt)),
    "math.h:rsqrt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalb), __typeof__(scalb)),
    "math.h:scalb declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbln), __typeof__(scalbln)),
    "math.h:scalbln declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbn), __typeof__(scalbn)),
    "math.h:scalbn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setpayload), __typeof__(setpayload)),
    "math.h:setpayload declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_setpayloadsig), __typeof__(setpayloadsig)),
    "math.h:setpayloadsig declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_significand), __typeof__(significand)),
    "math.h:significand declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sin), __typeof__(sin)),
    "math.h:sin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sincos), __typeof__(sincos)),
    "math.h:sincos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinh), __typeof__(sinh)),
    "math.h:sinh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinpi), __typeof__(sinpi)),
    "math.h:sinpi declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sqrt), __typeof__(sqrt)),
    "math.h:sqrt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tan), __typeof__(tan)),
    "math.h:tan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanh), __typeof__(tanh)),
    "math.h:tanh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanpi), __typeof__(tanpi)),
    "math.h:tanpi declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgamma), __typeof__(tgamma)),
    "math.h:tgamma declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_totalorder), __typeof__(totalorder)),
    "math.h:totalorder declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_totalordermag), __typeof__(totalordermag)),
    "math.h:totalordermag declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_trunc), __typeof__(trunc)),
    "math.h:trunc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ufromfp), __typeof__(ufromfp)),
    "math.h:ufromfp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ufromfpx), __typeof__(ufromfpx)),
    "math.h:ufromfpx declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_y0), __typeof__(y0)),
    "math.h:y0 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_y1), __typeof__(y1)),
    "math.h:y1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_yn), __typeof__(yn)),
    "math.h:yn declaration differs from oracle");

static __typeof__(acos) *const slate_reference_acos = &acos;
static __typeof__(acosh) *const slate_reference_acosh = &acosh;
static __typeof__(acospi) *const slate_reference_acospi = &acospi;
static __typeof__(asin) *const slate_reference_asin = &asin;
static __typeof__(asinh) *const slate_reference_asinh = &asinh;
static __typeof__(asinpi) *const slate_reference_asinpi = &asinpi;
static __typeof__(atan) *const slate_reference_atan = &atan;
static __typeof__(atan2) *const slate_reference_atan2 = &atan2;
static __typeof__(atan2pi) *const slate_reference_atan2pi = &atan2pi;
static __typeof__(atanh) *const slate_reference_atanh = &atanh;
static __typeof__(atanpi) *const slate_reference_atanpi = &atanpi;
static __typeof__(canonicalize) *const slate_reference_canonicalize = &canonicalize;
static __typeof__(cbrt) *const slate_reference_cbrt = &cbrt;
static __typeof__(ceil) *const slate_reference_ceil = &ceil;
static __typeof__(compoundn) *const slate_reference_compoundn = &compoundn;
static __typeof__(copysign) *const slate_reference_copysign = &copysign;
static __typeof__(cos) *const slate_reference_cos = &cos;
static __typeof__(cosh) *const slate_reference_cosh = &cosh;
static __typeof__(cospi) *const slate_reference_cospi = &cospi;
static __typeof__(drem) *const slate_reference_drem = &drem;
static __typeof__(erf) *const slate_reference_erf = &erf;
static __typeof__(erfc) *const slate_reference_erfc = &erfc;
static __typeof__(exp) *const slate_reference_exp = &exp;
static __typeof__(exp10) *const slate_reference_exp10 = &exp10;
static __typeof__(exp10m1) *const slate_reference_exp10m1 = &exp10m1;
static __typeof__(exp2) *const slate_reference_exp2 = &exp2;
static __typeof__(exp2m1) *const slate_reference_exp2m1 = &exp2m1;
static __typeof__(expm1) *const slate_reference_expm1 = &expm1;
static __typeof__(fabs) *const slate_reference_fabs = &fabs;
static __typeof__(fdim) *const slate_reference_fdim = &fdim;
static __typeof__(finite) *const slate_reference_finite = &finite;
static __typeof__(floor) *const slate_reference_floor = &floor;
static __typeof__(fma) *const slate_reference_fma = &fma;
static __typeof__(fmax) *const slate_reference_fmax = &fmax;
static __typeof__(fmaximum) *const slate_reference_fmaximum = &fmaximum;
static __typeof__(fmaximum_mag) *const slate_reference_fmaximum_mag = &fmaximum_mag;
static __typeof__(fmaximum_mag_num) *const slate_reference_fmaximum_mag_num = &fmaximum_mag_num;
static __typeof__(fmaximum_num) *const slate_reference_fmaximum_num = &fmaximum_num;
static __typeof__(fmaxmag) *const slate_reference_fmaxmag = &fmaxmag;
static __typeof__(fmin) *const slate_reference_fmin = &fmin;
static __typeof__(fminimum) *const slate_reference_fminimum = &fminimum;
static __typeof__(fminimum_mag) *const slate_reference_fminimum_mag = &fminimum_mag;
static __typeof__(fminimum_mag_num) *const slate_reference_fminimum_mag_num = &fminimum_mag_num;
static __typeof__(fminimum_num) *const slate_reference_fminimum_num = &fminimum_num;
static __typeof__(fminmag) *const slate_reference_fminmag = &fminmag;
static __typeof__(fmod) *const slate_reference_fmod = &fmod;
static __typeof__(frexp) *const slate_reference_frexp = &frexp;
static __typeof__(fromfp) *const slate_reference_fromfp = &fromfp;
static __typeof__(fromfpx) *const slate_reference_fromfpx = &fromfpx;
static __typeof__(gamma) *const slate_reference_gamma = &gamma;
static __typeof__(getpayload) *const slate_reference_getpayload = &getpayload;
static __typeof__(hypot) *const slate_reference_hypot = &hypot;
static __typeof__(ilogb) *const slate_reference_ilogb = &ilogb;
static __typeof__(isinf) *const slate_reference_isinf = &isinf;
static __typeof__(isnan) *const slate_reference_isnan = &isnan;
static __typeof__(j0) *const slate_reference_j0 = &j0;
static __typeof__(j1) *const slate_reference_j1 = &j1;
static __typeof__(jn) *const slate_reference_jn = &jn;
static __typeof__(ldexp) *const slate_reference_ldexp = &ldexp;
static __typeof__(lgamma) *const slate_reference_lgamma = &lgamma;
static __typeof__(llogb) *const slate_reference_llogb = &llogb;
static __typeof__(llrint) *const slate_reference_llrint = &llrint;
static __typeof__(llround) *const slate_reference_llround = &llround;
static __typeof__(log) *const slate_reference_log = &log;
static __typeof__(log10) *const slate_reference_log10 = &log10;
static __typeof__(log10p1) *const slate_reference_log10p1 = &log10p1;
static __typeof__(log1p) *const slate_reference_log1p = &log1p;
static __typeof__(log2) *const slate_reference_log2 = &log2;
static __typeof__(log2p1) *const slate_reference_log2p1 = &log2p1;
static __typeof__(logb) *const slate_reference_logb = &logb;
static __typeof__(logp1) *const slate_reference_logp1 = &logp1;
static __typeof__(lrint) *const slate_reference_lrint = &lrint;
static __typeof__(lround) *const slate_reference_lround = &lround;
static __typeof__(modf) *const slate_reference_modf = &modf;
static __typeof__(nan) *const slate_reference_nan = &nan;
static __typeof__(nearbyint) *const slate_reference_nearbyint = &nearbyint;
static __typeof__(nextafter) *const slate_reference_nextafter = &nextafter;
static __typeof__(nextdown) *const slate_reference_nextdown = &nextdown;
static __typeof__(nexttoward) *const slate_reference_nexttoward = &nexttoward;
static __typeof__(nextup) *const slate_reference_nextup = &nextup;
static __typeof__(pow) *const slate_reference_pow = &pow;
static __typeof__(pown) *const slate_reference_pown = &pown;
static __typeof__(powr) *const slate_reference_powr = &powr;
static __typeof__(remainder) *const slate_reference_remainder = &remainder;
static __typeof__(remquo) *const slate_reference_remquo = &remquo;
static __typeof__(rint) *const slate_reference_rint = &rint;
static __typeof__(rootn) *const slate_reference_rootn = &rootn;
static __typeof__(round) *const slate_reference_round = &round;
static __typeof__(roundeven) *const slate_reference_roundeven = &roundeven;
static __typeof__(rsqrt) *const slate_reference_rsqrt = &rsqrt;
static __typeof__(scalb) *const slate_reference_scalb = &scalb;
static __typeof__(scalbln) *const slate_reference_scalbln = &scalbln;
static __typeof__(scalbn) *const slate_reference_scalbn = &scalbn;
static __typeof__(setpayload) *const slate_reference_setpayload = &setpayload;
static __typeof__(setpayloadsig) *const slate_reference_setpayloadsig = &setpayloadsig;
static __typeof__(significand) *const slate_reference_significand = &significand;
static __typeof__(sin) *const slate_reference_sin = &sin;
static __typeof__(sincos) *const slate_reference_sincos = &sincos;
static __typeof__(sinh) *const slate_reference_sinh = &sinh;
static __typeof__(sinpi) *const slate_reference_sinpi = &sinpi;
static __typeof__(sqrt) *const slate_reference_sqrt = &sqrt;
static __typeof__(tan) *const slate_reference_tan = &tan;
static __typeof__(tanh) *const slate_reference_tanh = &tanh;
static __typeof__(tanpi) *const slate_reference_tanpi = &tanpi;
static __typeof__(tgamma) *const slate_reference_tgamma = &tgamma;
static __typeof__(totalorder) *const slate_reference_totalorder = &totalorder;
static __typeof__(totalordermag) *const slate_reference_totalordermag = &totalordermag;
static __typeof__(trunc) *const slate_reference_trunc = &trunc;
static __typeof__(ufromfp) *const slate_reference_ufromfp = &ufromfp;
static __typeof__(ufromfpx) *const slate_reference_ufromfpx = &ufromfpx;
static __typeof__(y0) *const slate_reference_y0 = &y0;
static __typeof__(y1) *const slate_reference_y1 = &y1;
static __typeof__(yn) *const slate_reference_yn = &yn;

extern int slate_oracle_signgam;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_signgam), __typeof__(signgam)), "signgam object type differs from oracle");

static __typeof__(signgam) *const slate_reference_signgam = &signgam;

typedef double slate_oracle_typedef_double_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_double_t, double_t), "typedef double_t differs from oracle");

typedef float slate_oracle_typedef_float_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_float_t, float_t), "typedef float_t differs from oracle");

typedef long double slate_oracle_typedef_long_double_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_long_double_t, long_double_t), "typedef long_double_t differs from oracle");

#ifndef FP_FAST_FMA
#error "math.h:FP_FAST_FMA macro is missing from libc-shim"
#endif

#ifndef FP_FAST_FMAF
#error "math.h:FP_FAST_FMAF macro is missing from libc-shim"
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

#ifndef HUGE_VAL_F128
#error "math.h:HUGE_VAL_F128 macro is missing from libc-shim"
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

#ifndef HUGE_VAL_F64X
#error "math.h:HUGE_VAL_F64X macro is missing from libc-shim"
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

#ifndef M_1_PIf128
#error "math.h:M_1_PIf128 macro is missing from libc-shim"
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

#ifndef M_1_PIf64x
#error "math.h:M_1_PIf64x macro is missing from libc-shim"
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

#ifndef M_2_PIf128
#error "math.h:M_2_PIf128 macro is missing from libc-shim"
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

#ifndef M_2_PIf64x
#error "math.h:M_2_PIf64x macro is missing from libc-shim"
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

#ifndef M_2_SQRTPIf128
#error "math.h:M_2_SQRTPIf128 macro is missing from libc-shim"
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

#ifndef M_2_SQRTPIf64x
#error "math.h:M_2_SQRTPIf64x macro is missing from libc-shim"
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

#ifndef M_Ef128
#error "math.h:M_Ef128 macro is missing from libc-shim"
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

#ifndef M_Ef64x
#error "math.h:M_Ef64x macro is missing from libc-shim"
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

#ifndef M_LN10f128
#error "math.h:M_LN10f128 macro is missing from libc-shim"
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

#ifndef M_LN10f64x
#error "math.h:M_LN10f64x macro is missing from libc-shim"
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

#ifndef M_LN2f128
#error "math.h:M_LN2f128 macro is missing from libc-shim"
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

#ifndef M_LN2f64x
#error "math.h:M_LN2f64x macro is missing from libc-shim"
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

#ifndef M_LOG10Ef128
#error "math.h:M_LOG10Ef128 macro is missing from libc-shim"
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

#ifndef M_LOG10Ef64x
#error "math.h:M_LOG10Ef64x macro is missing from libc-shim"
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

#ifndef M_LOG2Ef128
#error "math.h:M_LOG2Ef128 macro is missing from libc-shim"
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

#ifndef M_LOG2Ef64x
#error "math.h:M_LOG2Ef64x macro is missing from libc-shim"
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

#ifndef M_PI_2f128
#error "math.h:M_PI_2f128 macro is missing from libc-shim"
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

#ifndef M_PI_2f64x
#error "math.h:M_PI_2f64x macro is missing from libc-shim"
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

#ifndef M_PI_4f128
#error "math.h:M_PI_4f128 macro is missing from libc-shim"
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

#ifndef M_PI_4f64x
#error "math.h:M_PI_4f64x macro is missing from libc-shim"
#endif

#ifndef M_PI_4l
#error "math.h:M_PI_4l macro is missing from libc-shim"
#endif

#ifndef M_PIf
#error "math.h:M_PIf macro is missing from libc-shim"
#endif

#ifndef M_PIf128
#error "math.h:M_PIf128 macro is missing from libc-shim"
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

#ifndef M_PIf64x
#error "math.h:M_PIf64x macro is missing from libc-shim"
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

#ifndef M_SQRT1_2f128
#error "math.h:M_SQRT1_2f128 macro is missing from libc-shim"
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

#ifndef M_SQRT1_2f64x
#error "math.h:M_SQRT1_2f64x macro is missing from libc-shim"
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

#ifndef M_SQRT2f128
#error "math.h:M_SQRT2f128 macro is missing from libc-shim"
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

#ifndef M_SQRT2f64x
#error "math.h:M_SQRT2f64x macro is missing from libc-shim"
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

#ifndef SNANF128
#error "math.h:SNANF128 macro is missing from libc-shim"
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

#ifndef SNANF64X
#error "math.h:SNANF64X macro is missing from libc-shim"
#endif

#ifndef SNANL
#error "math.h:SNANL macro is missing from libc-shim"
#endif

#ifndef fpclassify
#error "math.h:fpclassify macro is missing from libc-shim"
#endif

#ifndef iscanonical
#error "math.h:iscanonical macro is missing from libc-shim"
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
