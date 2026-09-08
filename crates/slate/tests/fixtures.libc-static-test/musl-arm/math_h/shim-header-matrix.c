#include <math.h>

extern double slate_oracle_acos(double);
extern float slate_oracle_acosf(float);
extern double slate_oracle_acosh(double);
extern float slate_oracle_acoshf(float);
extern long double slate_oracle_acoshl(long double);
extern long double slate_oracle_acosl(long double);
extern double slate_oracle_asin(double);
extern float slate_oracle_asinf(float);
extern double slate_oracle_asinh(double);
extern float slate_oracle_asinhf(float);
extern long double slate_oracle_asinhl(long double);
extern long double slate_oracle_asinl(long double);
extern double slate_oracle_atan(double);
extern double slate_oracle_atan2(double, double);
extern float slate_oracle_atan2f(float, float);
extern long double slate_oracle_atan2l(long double, long double);
extern float slate_oracle_atanf(float);
extern double slate_oracle_atanh(double);
extern float slate_oracle_atanhf(float);
extern long double slate_oracle_atanhl(long double);
extern long double slate_oracle_atanl(long double);
extern double slate_oracle_cbrt(double);
extern float slate_oracle_cbrtf(float);
extern long double slate_oracle_cbrtl(long double);
extern double slate_oracle_ceil(double);
extern float slate_oracle_ceilf(float);
extern long double slate_oracle_ceill(long double);
extern double slate_oracle_copysign(double, double);
extern float slate_oracle_copysignf(float, float);
extern long double slate_oracle_copysignl(long double, long double);
extern double slate_oracle_cos(double);
extern float slate_oracle_cosf(float);
extern double slate_oracle_cosh(double);
extern float slate_oracle_coshf(float);
extern long double slate_oracle_coshl(long double);
extern long double slate_oracle_cosl(long double);
extern double slate_oracle_drem(double, double);
extern float slate_oracle_dremf(float, float);
extern double slate_oracle_erf(double);
extern double slate_oracle_erfc(double);
extern float slate_oracle_erfcf(float);
extern long double slate_oracle_erfcl(long double);
extern float slate_oracle_erff(float);
extern long double slate_oracle_erfl(long double);
extern double slate_oracle_exp(double);
extern double slate_oracle_exp10(double);
extern float slate_oracle_exp10f(float);
extern long double slate_oracle_exp10l(long double);
extern double slate_oracle_exp2(double);
extern float slate_oracle_exp2f(float);
extern long double slate_oracle_exp2l(long double);
extern float slate_oracle_expf(float);
extern long double slate_oracle_expl(long double);
extern double slate_oracle_expm1(double);
extern float slate_oracle_expm1f(float);
extern long double slate_oracle_expm1l(long double);
extern double slate_oracle_fabs(double);
extern float slate_oracle_fabsf(float);
extern long double slate_oracle_fabsl(long double);
extern double slate_oracle_fdim(double, double);
extern float slate_oracle_fdimf(float, float);
extern long double slate_oracle_fdiml(long double, long double);
extern int slate_oracle_finite(double);
extern int slate_oracle_finitef(float);
extern double slate_oracle_floor(double);
extern float slate_oracle_floorf(float);
extern long double slate_oracle_floorl(long double);
extern double slate_oracle_fma(double, double, double);
extern float slate_oracle_fmaf(float, float, float);
extern long double slate_oracle_fmal(long double, long double, long double);
extern double slate_oracle_fmax(double, double);
extern float slate_oracle_fmaxf(float, float);
extern long double slate_oracle_fmaxl(long double, long double);
extern double slate_oracle_fmin(double, double);
extern float slate_oracle_fminf(float, float);
extern long double slate_oracle_fminl(long double, long double);
extern double slate_oracle_fmod(double, double);
extern float slate_oracle_fmodf(float, float);
extern long double slate_oracle_fmodl(long double, long double);
extern double slate_oracle_frexp(double, int *);
extern float slate_oracle_frexpf(float, int *);
extern long double slate_oracle_frexpl(long double, int *);
extern double slate_oracle_hypot(double, double);
extern float slate_oracle_hypotf(float, float);
extern long double slate_oracle_hypotl(long double, long double);
extern int slate_oracle_ilogb(double);
extern int slate_oracle_ilogbf(float);
extern int slate_oracle_ilogbl(long double);
extern double slate_oracle_j0(double);
extern float slate_oracle_j0f(float);
extern double slate_oracle_j1(double);
extern float slate_oracle_j1f(float);
extern double slate_oracle_jn(int, double);
extern float slate_oracle_jnf(int, float);
extern double slate_oracle_ldexp(double, int);
extern float slate_oracle_ldexpf(float, int);
extern long double slate_oracle_ldexpl(long double, int);
extern double slate_oracle_lgamma(double);
extern double slate_oracle_lgamma_r(double, int *);
extern float slate_oracle_lgammaf(float);
extern float slate_oracle_lgammaf_r(float, int *);
extern long double slate_oracle_lgammal(long double);
extern long double slate_oracle_lgammal_r(long double, int *);
extern long long slate_oracle_llrint(double);
extern long long slate_oracle_llrintf(float);
extern long long slate_oracle_llrintl(long double);
extern long long slate_oracle_llround(double);
extern long long slate_oracle_llroundf(float);
extern long long slate_oracle_llroundl(long double);
extern double slate_oracle_log(double);
extern double slate_oracle_log10(double);
extern float slate_oracle_log10f(float);
extern long double slate_oracle_log10l(long double);
extern double slate_oracle_log1p(double);
extern float slate_oracle_log1pf(float);
extern long double slate_oracle_log1pl(long double);
extern double slate_oracle_log2(double);
extern float slate_oracle_log2f(float);
extern long double slate_oracle_log2l(long double);
extern double slate_oracle_logb(double);
extern float slate_oracle_logbf(float);
extern long double slate_oracle_logbl(long double);
extern float slate_oracle_logf(float);
extern long double slate_oracle_logl(long double);
extern long slate_oracle_lrint(double);
extern long slate_oracle_lrintf(float);
extern long slate_oracle_lrintl(long double);
extern long slate_oracle_lround(double);
extern long slate_oracle_lroundf(float);
extern long slate_oracle_lroundl(long double);
extern double slate_oracle_modf(double, double *);
extern float slate_oracle_modff(float, float *);
extern long double slate_oracle_modfl(long double, long double *);
extern double slate_oracle_nan(const char *);
extern float slate_oracle_nanf(const char *);
extern long double slate_oracle_nanl(const char *);
extern double slate_oracle_nearbyint(double);
extern float slate_oracle_nearbyintf(float);
extern long double slate_oracle_nearbyintl(long double);
extern double slate_oracle_nextafter(double, double);
extern float slate_oracle_nextafterf(float, float);
extern long double slate_oracle_nextafterl(long double, long double);
extern double slate_oracle_nexttoward(double, long double);
extern float slate_oracle_nexttowardf(float, long double);
extern long double slate_oracle_nexttowardl(long double, long double);
extern double slate_oracle_pow(double, double);
extern double slate_oracle_pow10(double);
extern float slate_oracle_pow10f(float);
extern long double slate_oracle_pow10l(long double);
extern float slate_oracle_powf(float, float);
extern long double slate_oracle_powl(long double, long double);
extern double slate_oracle_remainder(double, double);
extern float slate_oracle_remainderf(float, float);
extern long double slate_oracle_remainderl(long double, long double);
extern double slate_oracle_remquo(double, double, int *);
extern float slate_oracle_remquof(float, float, int *);
extern long double slate_oracle_remquol(long double, long double, int *);
extern double slate_oracle_rint(double);
extern float slate_oracle_rintf(float);
extern long double slate_oracle_rintl(long double);
extern double slate_oracle_round(double);
extern float slate_oracle_roundf(float);
extern long double slate_oracle_roundl(long double);
extern double slate_oracle_scalb(double, double);
extern float slate_oracle_scalbf(float, float);
extern double slate_oracle_scalbln(double, long);
extern float slate_oracle_scalblnf(float, long);
extern long double slate_oracle_scalblnl(long double, long);
extern double slate_oracle_scalbn(double, int);
extern float slate_oracle_scalbnf(float, int);
extern long double slate_oracle_scalbnl(long double, int);
extern double slate_oracle_significand(double);
extern float slate_oracle_significandf(float);
extern double slate_oracle_sin(double);
extern void slate_oracle_sincos(double, double *, double *);
extern void slate_oracle_sincosf(float, float *, float *);
extern void slate_oracle_sincosl(long double, long double *, long double *);
extern float slate_oracle_sinf(float);
extern double slate_oracle_sinh(double);
extern float slate_oracle_sinhf(float);
extern long double slate_oracle_sinhl(long double);
extern long double slate_oracle_sinl(long double);
extern double slate_oracle_sqrt(double);
extern float slate_oracle_sqrtf(float);
extern long double slate_oracle_sqrtl(long double);
extern double slate_oracle_tan(double);
extern float slate_oracle_tanf(float);
extern double slate_oracle_tanh(double);
extern float slate_oracle_tanhf(float);
extern long double slate_oracle_tanhl(long double);
extern long double slate_oracle_tanl(long double);
extern double slate_oracle_tgamma(double);
extern float slate_oracle_tgammaf(float);
extern long double slate_oracle_tgammal(long double);
extern double slate_oracle_trunc(double);
extern float slate_oracle_truncf(float);
extern long double slate_oracle_truncl(long double);
extern double slate_oracle_y0(double);
extern float slate_oracle_y0f(float);
extern double slate_oracle_y1(double);
extern float slate_oracle_y1f(float);
extern double slate_oracle_yn(int, double);
extern float slate_oracle_ynf(int, float);

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acos), __typeof__(acos)),
    "math.h:acos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acosf), __typeof__(acosf)),
    "math.h:acosf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acosh), __typeof__(acosh)),
    "math.h:acosh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acoshf), __typeof__(acoshf)),
    "math.h:acoshf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acoshl), __typeof__(acoshl)),
    "math.h:acoshl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_acosl), __typeof__(acosl)),
    "math.h:acosl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asin), __typeof__(asin)),
    "math.h:asin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinf), __typeof__(asinf)),
    "math.h:asinf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinh), __typeof__(asinh)),
    "math.h:asinh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinhf), __typeof__(asinhf)),
    "math.h:asinhf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinhl), __typeof__(asinhl)),
    "math.h:asinhl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_asinl), __typeof__(asinl)),
    "math.h:asinl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan), __typeof__(atan)),
    "math.h:atan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan2), __typeof__(atan2)),
    "math.h:atan2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan2f), __typeof__(atan2f)),
    "math.h:atan2f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atan2l), __typeof__(atan2l)),
    "math.h:atan2l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanf), __typeof__(atanf)),
    "math.h:atanf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanh), __typeof__(atanh)),
    "math.h:atanh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanhf), __typeof__(atanhf)),
    "math.h:atanhf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanhl), __typeof__(atanhl)),
    "math.h:atanhl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_atanl), __typeof__(atanl)),
    "math.h:atanl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cbrt), __typeof__(cbrt)),
    "math.h:cbrt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cbrtf), __typeof__(cbrtf)),
    "math.h:cbrtf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cbrtl), __typeof__(cbrtl)),
    "math.h:cbrtl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ceil), __typeof__(ceil)),
    "math.h:ceil declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ceilf), __typeof__(ceilf)),
    "math.h:ceilf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ceill), __typeof__(ceill)),
    "math.h:ceill declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_copysign), __typeof__(copysign)),
    "math.h:copysign declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_copysignf), __typeof__(copysignf)),
    "math.h:copysignf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_copysignl), __typeof__(copysignl)),
    "math.h:copysignl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cos), __typeof__(cos)),
    "math.h:cos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cosf), __typeof__(cosf)),
    "math.h:cosf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cosh), __typeof__(cosh)),
    "math.h:cosh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_coshf), __typeof__(coshf)),
    "math.h:coshf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_coshl), __typeof__(coshl)),
    "math.h:coshl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_cosl), __typeof__(cosl)),
    "math.h:cosl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_drem), __typeof__(drem)),
    "math.h:drem declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_dremf), __typeof__(dremf)),
    "math.h:dremf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erf), __typeof__(erf)),
    "math.h:erf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfc), __typeof__(erfc)),
    "math.h:erfc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfcf), __typeof__(erfcf)),
    "math.h:erfcf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfcl), __typeof__(erfcl)),
    "math.h:erfcl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erff), __typeof__(erff)),
    "math.h:erff declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_erfl), __typeof__(erfl)),
    "math.h:erfl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp), __typeof__(exp)),
    "math.h:exp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp10), __typeof__(exp10)),
    "math.h:exp10 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp10f), __typeof__(exp10f)),
    "math.h:exp10f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp10l), __typeof__(exp10l)),
    "math.h:exp10l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp2), __typeof__(exp2)),
    "math.h:exp2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp2f), __typeof__(exp2f)),
    "math.h:exp2f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_exp2l), __typeof__(exp2l)),
    "math.h:exp2l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expf), __typeof__(expf)),
    "math.h:expf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expl), __typeof__(expl)),
    "math.h:expl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expm1), __typeof__(expm1)),
    "math.h:expm1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expm1f), __typeof__(expm1f)),
    "math.h:expm1f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_expm1l), __typeof__(expm1l)),
    "math.h:expm1l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fabs), __typeof__(fabs)),
    "math.h:fabs declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fabsf), __typeof__(fabsf)),
    "math.h:fabsf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fabsl), __typeof__(fabsl)),
    "math.h:fabsl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdim), __typeof__(fdim)),
    "math.h:fdim declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdimf), __typeof__(fdimf)),
    "math.h:fdimf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fdiml), __typeof__(fdiml)),
    "math.h:fdiml declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_finite), __typeof__(finite)),
    "math.h:finite declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_finitef), __typeof__(finitef)),
    "math.h:finitef declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_floor), __typeof__(floor)),
    "math.h:floor declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_floorf), __typeof__(floorf)),
    "math.h:floorf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_floorl), __typeof__(floorl)),
    "math.h:floorl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fma), __typeof__(fma)),
    "math.h:fma declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaf), __typeof__(fmaf)),
    "math.h:fmaf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmal), __typeof__(fmal)),
    "math.h:fmal declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmax), __typeof__(fmax)),
    "math.h:fmax declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaxf), __typeof__(fmaxf)),
    "math.h:fmaxf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmaxl), __typeof__(fmaxl)),
    "math.h:fmaxl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmin), __typeof__(fmin)),
    "math.h:fmin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminf), __typeof__(fminf)),
    "math.h:fminf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fminl), __typeof__(fminl)),
    "math.h:fminl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmod), __typeof__(fmod)),
    "math.h:fmod declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmodf), __typeof__(fmodf)),
    "math.h:fmodf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_fmodl), __typeof__(fmodl)),
    "math.h:fmodl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_frexp), __typeof__(frexp)),
    "math.h:frexp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_frexpf), __typeof__(frexpf)),
    "math.h:frexpf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_frexpl), __typeof__(frexpl)),
    "math.h:frexpl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_hypot), __typeof__(hypot)),
    "math.h:hypot declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_hypotf), __typeof__(hypotf)),
    "math.h:hypotf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_hypotl), __typeof__(hypotl)),
    "math.h:hypotl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ilogb), __typeof__(ilogb)),
    "math.h:ilogb declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ilogbf), __typeof__(ilogbf)),
    "math.h:ilogbf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ilogbl), __typeof__(ilogbl)),
    "math.h:ilogbl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_j0), __typeof__(j0)),
    "math.h:j0 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_j0f), __typeof__(j0f)),
    "math.h:j0f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_j1), __typeof__(j1)),
    "math.h:j1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_j1f), __typeof__(j1f)),
    "math.h:j1f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_jn), __typeof__(jn)),
    "math.h:jn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_jnf), __typeof__(jnf)),
    "math.h:jnf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldexp), __typeof__(ldexp)),
    "math.h:ldexp declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldexpf), __typeof__(ldexpf)),
    "math.h:ldexpf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ldexpl), __typeof__(ldexpl)),
    "math.h:ldexpl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgamma), __typeof__(lgamma)),
    "math.h:lgamma declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgamma_r), __typeof__(lgamma_r)),
    "math.h:lgamma_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgammaf), __typeof__(lgammaf)),
    "math.h:lgammaf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgammaf_r), __typeof__(lgammaf_r)),
    "math.h:lgammaf_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgammal), __typeof__(lgammal)),
    "math.h:lgammal declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lgammal_r), __typeof__(lgammal_r)),
    "math.h:lgammal_r declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llrint), __typeof__(llrint)),
    "math.h:llrint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llrintf), __typeof__(llrintf)),
    "math.h:llrintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llrintl), __typeof__(llrintl)),
    "math.h:llrintl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llround), __typeof__(llround)),
    "math.h:llround declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llroundf), __typeof__(llroundf)),
    "math.h:llroundf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_llroundl), __typeof__(llroundl)),
    "math.h:llroundl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log), __typeof__(log)),
    "math.h:log declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log10), __typeof__(log10)),
    "math.h:log10 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log10f), __typeof__(log10f)),
    "math.h:log10f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log10l), __typeof__(log10l)),
    "math.h:log10l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log1p), __typeof__(log1p)),
    "math.h:log1p declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log1pf), __typeof__(log1pf)),
    "math.h:log1pf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log1pl), __typeof__(log1pl)),
    "math.h:log1pl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log2), __typeof__(log2)),
    "math.h:log2 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log2f), __typeof__(log2f)),
    "math.h:log2f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_log2l), __typeof__(log2l)),
    "math.h:log2l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logb), __typeof__(logb)),
    "math.h:logb declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logbf), __typeof__(logbf)),
    "math.h:logbf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logbl), __typeof__(logbl)),
    "math.h:logbl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logf), __typeof__(logf)),
    "math.h:logf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_logl), __typeof__(logl)),
    "math.h:logl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrint), __typeof__(lrint)),
    "math.h:lrint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrintf), __typeof__(lrintf)),
    "math.h:lrintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lrintl), __typeof__(lrintl)),
    "math.h:lrintl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lround), __typeof__(lround)),
    "math.h:lround declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lroundf), __typeof__(lroundf)),
    "math.h:lroundf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_lroundl), __typeof__(lroundl)),
    "math.h:lroundl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_modf), __typeof__(modf)),
    "math.h:modf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_modff), __typeof__(modff)),
    "math.h:modff declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_modfl), __typeof__(modfl)),
    "math.h:modfl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nan), __typeof__(nan)),
    "math.h:nan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nanf), __typeof__(nanf)),
    "math.h:nanf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nanl), __typeof__(nanl)),
    "math.h:nanl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nearbyint), __typeof__(nearbyint)),
    "math.h:nearbyint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nearbyintf), __typeof__(nearbyintf)),
    "math.h:nearbyintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nearbyintl), __typeof__(nearbyintl)),
    "math.h:nearbyintl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextafter), __typeof__(nextafter)),
    "math.h:nextafter declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextafterf), __typeof__(nextafterf)),
    "math.h:nextafterf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nextafterl), __typeof__(nextafterl)),
    "math.h:nextafterl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nexttoward), __typeof__(nexttoward)),
    "math.h:nexttoward declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nexttowardf), __typeof__(nexttowardf)),
    "math.h:nexttowardf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_nexttowardl), __typeof__(nexttowardl)),
    "math.h:nexttowardl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pow), __typeof__(pow)),
    "math.h:pow declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pow10), __typeof__(pow10)),
    "math.h:pow10 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pow10f), __typeof__(pow10f)),
    "math.h:pow10f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_pow10l), __typeof__(pow10l)),
    "math.h:pow10l declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_powf), __typeof__(powf)),
    "math.h:powf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_powl), __typeof__(powl)),
    "math.h:powl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remainder), __typeof__(remainder)),
    "math.h:remainder declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remainderf), __typeof__(remainderf)),
    "math.h:remainderf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remainderl), __typeof__(remainderl)),
    "math.h:remainderl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remquo), __typeof__(remquo)),
    "math.h:remquo declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remquof), __typeof__(remquof)),
    "math.h:remquof declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_remquol), __typeof__(remquol)),
    "math.h:remquol declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rint), __typeof__(rint)),
    "math.h:rint declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rintf), __typeof__(rintf)),
    "math.h:rintf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_rintl), __typeof__(rintl)),
    "math.h:rintl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_round), __typeof__(round)),
    "math.h:round declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_roundf), __typeof__(roundf)),
    "math.h:roundf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_roundl), __typeof__(roundl)),
    "math.h:roundl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalb), __typeof__(scalb)),
    "math.h:scalb declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbf), __typeof__(scalbf)),
    "math.h:scalbf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbln), __typeof__(scalbln)),
    "math.h:scalbln declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalblnf), __typeof__(scalblnf)),
    "math.h:scalblnf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalblnl), __typeof__(scalblnl)),
    "math.h:scalblnl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbn), __typeof__(scalbn)),
    "math.h:scalbn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbnf), __typeof__(scalbnf)),
    "math.h:scalbnf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_scalbnl), __typeof__(scalbnl)),
    "math.h:scalbnl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_significand), __typeof__(significand)),
    "math.h:significand declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_significandf), __typeof__(significandf)),
    "math.h:significandf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sin), __typeof__(sin)),
    "math.h:sin declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sincos), __typeof__(sincos)),
    "math.h:sincos declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sincosf), __typeof__(sincosf)),
    "math.h:sincosf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sincosl), __typeof__(sincosl)),
    "math.h:sincosl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinf), __typeof__(sinf)),
    "math.h:sinf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinh), __typeof__(sinh)),
    "math.h:sinh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinhf), __typeof__(sinhf)),
    "math.h:sinhf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinhl), __typeof__(sinhl)),
    "math.h:sinhl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sinl), __typeof__(sinl)),
    "math.h:sinl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sqrt), __typeof__(sqrt)),
    "math.h:sqrt declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sqrtf), __typeof__(sqrtf)),
    "math.h:sqrtf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_sqrtl), __typeof__(sqrtl)),
    "math.h:sqrtl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tan), __typeof__(tan)),
    "math.h:tan declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanf), __typeof__(tanf)),
    "math.h:tanf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanh), __typeof__(tanh)),
    "math.h:tanh declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanhf), __typeof__(tanhf)),
    "math.h:tanhf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanhl), __typeof__(tanhl)),
    "math.h:tanhl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tanl), __typeof__(tanl)),
    "math.h:tanl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgamma), __typeof__(tgamma)),
    "math.h:tgamma declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgammaf), __typeof__(tgammaf)),
    "math.h:tgammaf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_tgammal), __typeof__(tgammal)),
    "math.h:tgammal declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_trunc), __typeof__(trunc)),
    "math.h:trunc declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_truncf), __typeof__(truncf)),
    "math.h:truncf declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_truncl), __typeof__(truncl)),
    "math.h:truncl declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_y0), __typeof__(y0)),
    "math.h:y0 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_y0f), __typeof__(y0f)),
    "math.h:y0f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_y1), __typeof__(y1)),
    "math.h:y1 declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_y1f), __typeof__(y1f)),
    "math.h:y1f declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_yn), __typeof__(yn)),
    "math.h:yn declaration differs from oracle");

_Static_assert(
    __builtin_types_compatible_p(__typeof__(slate_oracle_ynf), __typeof__(ynf)),
    "math.h:ynf declaration differs from oracle");

static __typeof__(acos) *const slate_reference_acos = &acos;
static __typeof__(acosf) *const slate_reference_acosf = &acosf;
static __typeof__(acosh) *const slate_reference_acosh = &acosh;
static __typeof__(acoshf) *const slate_reference_acoshf = &acoshf;
static __typeof__(acoshl) *const slate_reference_acoshl = &acoshl;
static __typeof__(acosl) *const slate_reference_acosl = &acosl;
static __typeof__(asin) *const slate_reference_asin = &asin;
static __typeof__(asinf) *const slate_reference_asinf = &asinf;
static __typeof__(asinh) *const slate_reference_asinh = &asinh;
static __typeof__(asinhf) *const slate_reference_asinhf = &asinhf;
static __typeof__(asinhl) *const slate_reference_asinhl = &asinhl;
static __typeof__(asinl) *const slate_reference_asinl = &asinl;
static __typeof__(atan) *const slate_reference_atan = &atan;
static __typeof__(atan2) *const slate_reference_atan2 = &atan2;
static __typeof__(atan2f) *const slate_reference_atan2f = &atan2f;
static __typeof__(atan2l) *const slate_reference_atan2l = &atan2l;
static __typeof__(atanf) *const slate_reference_atanf = &atanf;
static __typeof__(atanh) *const slate_reference_atanh = &atanh;
static __typeof__(atanhf) *const slate_reference_atanhf = &atanhf;
static __typeof__(atanhl) *const slate_reference_atanhl = &atanhl;
static __typeof__(atanl) *const slate_reference_atanl = &atanl;
static __typeof__(cbrt) *const slate_reference_cbrt = &cbrt;
static __typeof__(cbrtf) *const slate_reference_cbrtf = &cbrtf;
static __typeof__(cbrtl) *const slate_reference_cbrtl = &cbrtl;
static __typeof__(ceil) *const slate_reference_ceil = &ceil;
static __typeof__(ceilf) *const slate_reference_ceilf = &ceilf;
static __typeof__(ceill) *const slate_reference_ceill = &ceill;
static __typeof__(copysign) *const slate_reference_copysign = &copysign;
static __typeof__(copysignf) *const slate_reference_copysignf = &copysignf;
static __typeof__(copysignl) *const slate_reference_copysignl = &copysignl;
static __typeof__(cos) *const slate_reference_cos = &cos;
static __typeof__(cosf) *const slate_reference_cosf = &cosf;
static __typeof__(cosh) *const slate_reference_cosh = &cosh;
static __typeof__(coshf) *const slate_reference_coshf = &coshf;
static __typeof__(coshl) *const slate_reference_coshl = &coshl;
static __typeof__(cosl) *const slate_reference_cosl = &cosl;
static __typeof__(drem) *const slate_reference_drem = &drem;
static __typeof__(dremf) *const slate_reference_dremf = &dremf;
static __typeof__(erf) *const slate_reference_erf = &erf;
static __typeof__(erfc) *const slate_reference_erfc = &erfc;
static __typeof__(erfcf) *const slate_reference_erfcf = &erfcf;
static __typeof__(erfcl) *const slate_reference_erfcl = &erfcl;
static __typeof__(erff) *const slate_reference_erff = &erff;
static __typeof__(erfl) *const slate_reference_erfl = &erfl;
static __typeof__(exp) *const slate_reference_exp = &exp;
static __typeof__(exp10) *const slate_reference_exp10 = &exp10;
static __typeof__(exp10f) *const slate_reference_exp10f = &exp10f;
static __typeof__(exp10l) *const slate_reference_exp10l = &exp10l;
static __typeof__(exp2) *const slate_reference_exp2 = &exp2;
static __typeof__(exp2f) *const slate_reference_exp2f = &exp2f;
static __typeof__(exp2l) *const slate_reference_exp2l = &exp2l;
static __typeof__(expf) *const slate_reference_expf = &expf;
static __typeof__(expl) *const slate_reference_expl = &expl;
static __typeof__(expm1) *const slate_reference_expm1 = &expm1;
static __typeof__(expm1f) *const slate_reference_expm1f = &expm1f;
static __typeof__(expm1l) *const slate_reference_expm1l = &expm1l;
static __typeof__(fabs) *const slate_reference_fabs = &fabs;
static __typeof__(fabsf) *const slate_reference_fabsf = &fabsf;
static __typeof__(fabsl) *const slate_reference_fabsl = &fabsl;
static __typeof__(fdim) *const slate_reference_fdim = &fdim;
static __typeof__(fdimf) *const slate_reference_fdimf = &fdimf;
static __typeof__(fdiml) *const slate_reference_fdiml = &fdiml;
static __typeof__(finite) *const slate_reference_finite = &finite;
static __typeof__(finitef) *const slate_reference_finitef = &finitef;
static __typeof__(floor) *const slate_reference_floor = &floor;
static __typeof__(floorf) *const slate_reference_floorf = &floorf;
static __typeof__(floorl) *const slate_reference_floorl = &floorl;
static __typeof__(fma) *const slate_reference_fma = &fma;
static __typeof__(fmaf) *const slate_reference_fmaf = &fmaf;
static __typeof__(fmal) *const slate_reference_fmal = &fmal;
static __typeof__(fmax) *const slate_reference_fmax = &fmax;
static __typeof__(fmaxf) *const slate_reference_fmaxf = &fmaxf;
static __typeof__(fmaxl) *const slate_reference_fmaxl = &fmaxl;
static __typeof__(fmin) *const slate_reference_fmin = &fmin;
static __typeof__(fminf) *const slate_reference_fminf = &fminf;
static __typeof__(fminl) *const slate_reference_fminl = &fminl;
static __typeof__(fmod) *const slate_reference_fmod = &fmod;
static __typeof__(fmodf) *const slate_reference_fmodf = &fmodf;
static __typeof__(fmodl) *const slate_reference_fmodl = &fmodl;
static __typeof__(frexp) *const slate_reference_frexp = &frexp;
static __typeof__(frexpf) *const slate_reference_frexpf = &frexpf;
static __typeof__(frexpl) *const slate_reference_frexpl = &frexpl;
static __typeof__(hypot) *const slate_reference_hypot = &hypot;
static __typeof__(hypotf) *const slate_reference_hypotf = &hypotf;
static __typeof__(hypotl) *const slate_reference_hypotl = &hypotl;
static __typeof__(ilogb) *const slate_reference_ilogb = &ilogb;
static __typeof__(ilogbf) *const slate_reference_ilogbf = &ilogbf;
static __typeof__(ilogbl) *const slate_reference_ilogbl = &ilogbl;
static __typeof__(j0) *const slate_reference_j0 = &j0;
static __typeof__(j0f) *const slate_reference_j0f = &j0f;
static __typeof__(j1) *const slate_reference_j1 = &j1;
static __typeof__(j1f) *const slate_reference_j1f = &j1f;
static __typeof__(jn) *const slate_reference_jn = &jn;
static __typeof__(jnf) *const slate_reference_jnf = &jnf;
static __typeof__(ldexp) *const slate_reference_ldexp = &ldexp;
static __typeof__(ldexpf) *const slate_reference_ldexpf = &ldexpf;
static __typeof__(ldexpl) *const slate_reference_ldexpl = &ldexpl;
static __typeof__(lgamma) *const slate_reference_lgamma = &lgamma;
static __typeof__(lgamma_r) *const slate_reference_lgamma_r = &lgamma_r;
static __typeof__(lgammaf) *const slate_reference_lgammaf = &lgammaf;
static __typeof__(lgammaf_r) *const slate_reference_lgammaf_r = &lgammaf_r;
static __typeof__(lgammal) *const slate_reference_lgammal = &lgammal;
static __typeof__(lgammal_r) *const slate_reference_lgammal_r = &lgammal_r;
static __typeof__(llrint) *const slate_reference_llrint = &llrint;
static __typeof__(llrintf) *const slate_reference_llrintf = &llrintf;
static __typeof__(llrintl) *const slate_reference_llrintl = &llrintl;
static __typeof__(llround) *const slate_reference_llround = &llround;
static __typeof__(llroundf) *const slate_reference_llroundf = &llroundf;
static __typeof__(llroundl) *const slate_reference_llroundl = &llroundl;
static __typeof__(log) *const slate_reference_log = &log;
static __typeof__(log10) *const slate_reference_log10 = &log10;
static __typeof__(log10f) *const slate_reference_log10f = &log10f;
static __typeof__(log10l) *const slate_reference_log10l = &log10l;
static __typeof__(log1p) *const slate_reference_log1p = &log1p;
static __typeof__(log1pf) *const slate_reference_log1pf = &log1pf;
static __typeof__(log1pl) *const slate_reference_log1pl = &log1pl;
static __typeof__(log2) *const slate_reference_log2 = &log2;
static __typeof__(log2f) *const slate_reference_log2f = &log2f;
static __typeof__(log2l) *const slate_reference_log2l = &log2l;
static __typeof__(logb) *const slate_reference_logb = &logb;
static __typeof__(logbf) *const slate_reference_logbf = &logbf;
static __typeof__(logbl) *const slate_reference_logbl = &logbl;
static __typeof__(logf) *const slate_reference_logf = &logf;
static __typeof__(logl) *const slate_reference_logl = &logl;
static __typeof__(lrint) *const slate_reference_lrint = &lrint;
static __typeof__(lrintf) *const slate_reference_lrintf = &lrintf;
static __typeof__(lrintl) *const slate_reference_lrintl = &lrintl;
static __typeof__(lround) *const slate_reference_lround = &lround;
static __typeof__(lroundf) *const slate_reference_lroundf = &lroundf;
static __typeof__(lroundl) *const slate_reference_lroundl = &lroundl;
static __typeof__(modf) *const slate_reference_modf = &modf;
static __typeof__(modff) *const slate_reference_modff = &modff;
static __typeof__(modfl) *const slate_reference_modfl = &modfl;
static __typeof__(nan) *const slate_reference_nan = &nan;
static __typeof__(nanf) *const slate_reference_nanf = &nanf;
static __typeof__(nanl) *const slate_reference_nanl = &nanl;
static __typeof__(nearbyint) *const slate_reference_nearbyint = &nearbyint;
static __typeof__(nearbyintf) *const slate_reference_nearbyintf = &nearbyintf;
static __typeof__(nearbyintl) *const slate_reference_nearbyintl = &nearbyintl;
static __typeof__(nextafter) *const slate_reference_nextafter = &nextafter;
static __typeof__(nextafterf) *const slate_reference_nextafterf = &nextafterf;
static __typeof__(nextafterl) *const slate_reference_nextafterl = &nextafterl;
static __typeof__(nexttoward) *const slate_reference_nexttoward = &nexttoward;
static __typeof__(nexttowardf) *const slate_reference_nexttowardf = &nexttowardf;
static __typeof__(nexttowardl) *const slate_reference_nexttowardl = &nexttowardl;
static __typeof__(pow) *const slate_reference_pow = &pow;
static __typeof__(pow10) *const slate_reference_pow10 = &pow10;
static __typeof__(pow10f) *const slate_reference_pow10f = &pow10f;
static __typeof__(pow10l) *const slate_reference_pow10l = &pow10l;
static __typeof__(powf) *const slate_reference_powf = &powf;
static __typeof__(powl) *const slate_reference_powl = &powl;
static __typeof__(remainder) *const slate_reference_remainder = &remainder;
static __typeof__(remainderf) *const slate_reference_remainderf = &remainderf;
static __typeof__(remainderl) *const slate_reference_remainderl = &remainderl;
static __typeof__(remquo) *const slate_reference_remquo = &remquo;
static __typeof__(remquof) *const slate_reference_remquof = &remquof;
static __typeof__(remquol) *const slate_reference_remquol = &remquol;
static __typeof__(rint) *const slate_reference_rint = &rint;
static __typeof__(rintf) *const slate_reference_rintf = &rintf;
static __typeof__(rintl) *const slate_reference_rintl = &rintl;
static __typeof__(round) *const slate_reference_round = &round;
static __typeof__(roundf) *const slate_reference_roundf = &roundf;
static __typeof__(roundl) *const slate_reference_roundl = &roundl;
static __typeof__(scalb) *const slate_reference_scalb = &scalb;
static __typeof__(scalbf) *const slate_reference_scalbf = &scalbf;
static __typeof__(scalbln) *const slate_reference_scalbln = &scalbln;
static __typeof__(scalblnf) *const slate_reference_scalblnf = &scalblnf;
static __typeof__(scalblnl) *const slate_reference_scalblnl = &scalblnl;
static __typeof__(scalbn) *const slate_reference_scalbn = &scalbn;
static __typeof__(scalbnf) *const slate_reference_scalbnf = &scalbnf;
static __typeof__(scalbnl) *const slate_reference_scalbnl = &scalbnl;
static __typeof__(significand) *const slate_reference_significand = &significand;
static __typeof__(significandf) *const slate_reference_significandf = &significandf;
static __typeof__(sin) *const slate_reference_sin = &sin;
static __typeof__(sincos) *const slate_reference_sincos = &sincos;
static __typeof__(sincosf) *const slate_reference_sincosf = &sincosf;
static __typeof__(sincosl) *const slate_reference_sincosl = &sincosl;
static __typeof__(sinf) *const slate_reference_sinf = &sinf;
static __typeof__(sinh) *const slate_reference_sinh = &sinh;
static __typeof__(sinhf) *const slate_reference_sinhf = &sinhf;
static __typeof__(sinhl) *const slate_reference_sinhl = &sinhl;
static __typeof__(sinl) *const slate_reference_sinl = &sinl;
static __typeof__(sqrt) *const slate_reference_sqrt = &sqrt;
static __typeof__(sqrtf) *const slate_reference_sqrtf = &sqrtf;
static __typeof__(sqrtl) *const slate_reference_sqrtl = &sqrtl;
static __typeof__(tan) *const slate_reference_tan = &tan;
static __typeof__(tanf) *const slate_reference_tanf = &tanf;
static __typeof__(tanh) *const slate_reference_tanh = &tanh;
static __typeof__(tanhf) *const slate_reference_tanhf = &tanhf;
static __typeof__(tanhl) *const slate_reference_tanhl = &tanhl;
static __typeof__(tanl) *const slate_reference_tanl = &tanl;
static __typeof__(tgamma) *const slate_reference_tgamma = &tgamma;
static __typeof__(tgammaf) *const slate_reference_tgammaf = &tgammaf;
static __typeof__(tgammal) *const slate_reference_tgammal = &tgammal;
static __typeof__(trunc) *const slate_reference_trunc = &trunc;
static __typeof__(truncf) *const slate_reference_truncf = &truncf;
static __typeof__(truncl) *const slate_reference_truncl = &truncl;
static __typeof__(y0) *const slate_reference_y0 = &y0;
static __typeof__(y0f) *const slate_reference_y0f = &y0f;
static __typeof__(y1) *const slate_reference_y1 = &y1;
static __typeof__(y1f) *const slate_reference_y1f = &y1f;
static __typeof__(yn) *const slate_reference_yn = &yn;
static __typeof__(ynf) *const slate_reference_ynf = &ynf;

extern int slate_oracle_signgam;

_Static_assert(__builtin_types_compatible_p(__typeof__(slate_oracle_signgam), __typeof__(signgam)), "signgam object type differs from oracle");

static __typeof__(signgam) *const slate_reference_signgam = &signgam;

typedef double slate_oracle_typedef_double_t;
_Static_assert(__builtin_types_compatible_p(slate_oracle_typedef_double_t, double_t), "typedef double_t differs from oracle");

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

#ifndef HUGE
#error "math.h:HUGE macro is missing from libc-shim"
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

#ifndef M_1_PI
#error "math.h:M_1_PI macro is missing from libc-shim"
#endif

#ifndef M_2_PI
#error "math.h:M_2_PI macro is missing from libc-shim"
#endif

#ifndef M_2_SQRTPI
#error "math.h:M_2_SQRTPI macro is missing from libc-shim"
#endif

#ifndef M_E
#error "math.h:M_E macro is missing from libc-shim"
#endif

#ifndef M_LN10
#error "math.h:M_LN10 macro is missing from libc-shim"
#endif

#ifndef M_LN2
#error "math.h:M_LN2 macro is missing from libc-shim"
#endif

#ifndef M_LOG10E
#error "math.h:M_LOG10E macro is missing from libc-shim"
#endif

#ifndef M_LOG2E
#error "math.h:M_LOG2E macro is missing from libc-shim"
#endif

#ifndef M_PI
#error "math.h:M_PI macro is missing from libc-shim"
#endif

#ifndef M_PI_2
#error "math.h:M_PI_2 macro is missing from libc-shim"
#endif

#ifndef M_PI_4
#error "math.h:M_PI_4 macro is missing from libc-shim"
#endif

#ifndef M_SQRT1_2
#error "math.h:M_SQRT1_2 macro is missing from libc-shim"
#endif

#ifndef M_SQRT2
#error "math.h:M_SQRT2 macro is missing from libc-shim"
#endif

#ifndef NAN
#error "math.h:NAN macro is missing from libc-shim"
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

#ifndef signbit
#error "math.h:signbit macro is missing from libc-shim"
#endif

int main(void) { return 0; }
