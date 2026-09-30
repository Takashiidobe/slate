
// Test attributes and codegen of math builtins.

void foo(double *d, float f, float *fp, long double *l, int *i, const char *c) {
  f = __builtin_fmod(f,f);    f = __builtin_fmodf(f,f);   f =  __builtin_fmodl(f,f);  f = __builtin_fmodf128(f,f);
























  __builtin_atan2(f,f);    __builtin_atan2f(f,f) ;  __builtin_atan2l(f, f); __builtin_atan2f128(f,f);


  __builtin_copysign(f,f); __builtin_copysignf(f,f); __builtin_copysignl(f,f); __builtin_copysignf128(f,f);


  __builtin_fabs(f);       __builtin_fabsf(f);      __builtin_fabsl(f); __builtin_fabsf128(f);


  __builtin_frexp(f,i);    __builtin_frexpf(f,i);   __builtin_frexpl(f,i); __builtin_frexpf128(f,i);


  __builtin_huge_val();    __builtin_huge_valf();   __builtin_huge_vall(); __builtin_huge_valf128();


  __builtin_inf();    __builtin_inff();   __builtin_infl(); __builtin_inff128();


  __builtin_ldexp(f,f);    __builtin_ldexpf(f,f);   __builtin_ldexpl(f,f);  __builtin_ldexpf128(f,f);


  __builtin_modf(f,d);       __builtin_modff(f,fp);      __builtin_modfl(f,l); __builtin_modff128(f,l);


  __builtin_nan(c);        __builtin_nanf(c);       __builtin_nanl(c); __builtin_nanf128(c);


  __builtin_nans(c);        __builtin_nansf(c);       __builtin_nansl(c); __builtin_nansf128(c);


  __builtin_pow(f,f);        __builtin_powf(f,f);       __builtin_powl(f,f); __builtin_powf128(f,f);


  __builtin_powi(f,f);        __builtin_powif(f,f);       __builtin_powil(f,f);


  /* math */
  __builtin_acos(f);       __builtin_acosf(f);      __builtin_acosl(f); __builtin_acosf128(f);


  __builtin_acosh(f);      __builtin_acoshf(f);     __builtin_acoshl(f);  __builtin_acoshf128(f);


  __builtin_asin(f);       __builtin_asinf(f);      __builtin_asinl(f); __builtin_asinf128(f);


  __builtin_asinh(f);      __builtin_asinhf(f);     __builtin_asinhl(f); __builtin_asinhf128(f);


  __builtin_atan(f);       __builtin_atanf(f);      __builtin_atanl(f); __builtin_atanf128(f);


  __builtin_atanh(f);      __builtin_atanhf(f);     __builtin_atanhl(f); __builtin_atanhf128(f);


  __builtin_cbrt(f);       __builtin_cbrtf(f);      __builtin_cbrtl(f); __builtin_cbrtf128(f);


  __builtin_ceil(f);       __builtin_ceilf(f);      __builtin_ceill(f); __builtin_ceilf128(f);


  __builtin_cos(f);        __builtin_cosf(f);       __builtin_cosl(f); __builtin_cosf128(f);


  __builtin_cosh(f);       __builtin_coshf(f);      __builtin_coshl(f); __builtin_coshf128(f);


  __builtin_erf(f);        __builtin_erff(f);       __builtin_erfl(f); __builtin_erff128(f);


__builtin_erfc(f);       __builtin_erfcf(f);      __builtin_erfcl(f); __builtin_erfcf128(f);


__builtin_exp(f);        __builtin_expf(f);       __builtin_expl(f); __builtin_expf128(f);


__builtin_exp2(f);       __builtin_exp2f(f);      __builtin_exp2l(f); __builtin_exp2f128(f);


__builtin_exp10(f);       __builtin_exp10f(f);      __builtin_exp10l(f); __builtin_exp10f128(f);


__builtin_expm1(f);      __builtin_expm1f(f);     __builtin_expm1l(f); __builtin_expm1f128(f);


__builtin_fdim(f,f);       __builtin_fdimf(f,f);      __builtin_fdiml(f,f); __builtin_fdimf128(f,f);


__builtin_floor(f);      __builtin_floorf(f);     __builtin_floorl(f); __builtin_floorf128(f);


__builtin_fma(f,f,f);        __builtin_fmaf(f,f,f);       __builtin_fmal(f,f,f); __builtin_fmaf128(f,f,f);  __builtin_fmaf16(f,f,f);

// NO__ERRONO: declare half @llvm.fma.f16(half, half, half) [[READNONE_INTRINSIC2]]

// On GNU or Win, fma never sets errno, so we can convert to the intrinsic.


// Long double is just double on win, so no f80 use/declaration.

__builtin_fmax(f,f);       __builtin_fmaxf(f,f);      __builtin_fmaxl(f,f); __builtin_fmaxf128(f,f);


__builtin_fmin(f,f);       __builtin_fminf(f,f);      __builtin_fminl(f,f); __builtin_fminf128(f,f);


__builtin_hypot(f,f);      __builtin_hypotf(f,f);     __builtin_hypotl(f,f); __builtin_hypotf128(f,f);


__builtin_ilogb(f);      __builtin_ilogbf(f);     __builtin_ilogbl(f); __builtin_ilogbf128(f);


__builtin_lgamma(f);     __builtin_lgammaf(f);    __builtin_lgammal(f); __builtin_lgammaf128(f);


__builtin_llrint(f);     __builtin_llrintf(f);    __builtin_llrintl(f); __builtin_llrintf128(f);


__builtin_llround(f);    __builtin_llroundf(f);   __builtin_llroundl(f); __builtin_llroundf128(f);


__builtin_log(f);        __builtin_logf(f);       __builtin_logl(f); __builtin_logf128(f);


__builtin_log10(f);      __builtin_log10f(f);     __builtin_log10l(f); __builtin_log10f128(f);


__builtin_log1p(f);      __builtin_log1pf(f);     __builtin_log1pl(f); __builtin_log1pf128(f);


__builtin_log2(f);       __builtin_log2f(f);      __builtin_log2l(f); __builtin_log2f128(f);


__builtin_logb(f);       __builtin_logbf(f);      __builtin_logbl(f); __builtin_logbf128(f);


__builtin_lrint(f);      __builtin_lrintf(f);     __builtin_lrintl(f); __builtin_lrintf128(f);


__builtin_lround(f);     __builtin_lroundf(f);    __builtin_lroundl(f);  __builtin_lroundf128(f);


__builtin_nearbyint(f);  __builtin_nearbyintf(f); __builtin_nearbyintl(f); __builtin_nearbyintf128(f);


__builtin_nextafter(f,f);  __builtin_nextafterf(f,f); __builtin_nextafterl(f,f); __builtin_nextafterf128(f,f);


__builtin_nexttoward(f,f); __builtin_nexttowardf(f,f);__builtin_nexttowardl(f,f); __builtin_nexttowardf128(f,f);


__builtin_remainder(f,f);  __builtin_remainderf(f,f); __builtin_remainderl(f,f); __builtin_remainderf128(f,f);


__builtin_remquo(f,f,i);  __builtin_remquof(f,f,i); __builtin_remquol(f,f,i); __builtin_remquof128(f,f,i);


__builtin_rint(f);       __builtin_rintf(f);      __builtin_rintl(f); __builtin_rintf128(f);


__builtin_round(f);      __builtin_roundf(f);     __builtin_roundl(f); __builtin_roundf128(f);


__builtin_scalbln(f,f);    __builtin_scalblnf(f,f);   __builtin_scalblnl(f,f); __builtin_scalblnf128(f,f);


__builtin_scalbn(f,f);     __builtin_scalbnf(f,f);    __builtin_scalbnl(f,f); __builtin_scalbnf128(f,f);


__builtin_sin(f);        __builtin_sinf(f);       __builtin_sinl(f); __builtin_sinf128(f);


__builtin_sinh(f);       __builtin_sinhf(f);      __builtin_sinhl(f); __builtin_sinhf128(f);


__builtin_sincos(f,d,d); __builtin_sincosf(f,fp,fp); __builtin_sincosl(f,l,l); __builtin_sincosf128(f,l,l);

__builtin_sincospi(f,d,d); __builtin_sincospif(f,fp,fp); __builtin_sincospil(f,l,l);

__builtin_sqrt(f);       __builtin_sqrtf(f);      __builtin_sqrtl(f); __builtin_sqrtf128(f);


__builtin_tan(f);        __builtin_tanf(f);       __builtin_tanl(f); __builtin_tanf128(f);


__builtin_tanh(f);       __builtin_tanhf(f);      __builtin_tanhl(f); __builtin_tanhf128(f);


__builtin_tgamma(f);     __builtin_tgammaf(f);    __builtin_tgammal(f); __builtin_tgammaf128(f);


__builtin_trunc(f);      __builtin_truncf(f);     __builtin_truncl(f); __builtin_truncf128(f);

};

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wno-error=incompatible-pointer-types

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmod:[0-9]+]] @__builtin_fmod(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmodf:[0-9]+]] @__builtin_fmodf(%[[VALUE2:[0-9]+]] <unnamed>: f32, %[[VALUE3:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmodl:[0-9]+]] @__builtin_fmodl(%[[VALUE4:[0-9]+]] <unnamed>: f64, %[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmodf128:[0-9]+]] @__builtin_fmodf128(%[[VALUE6:[0-9]+]] <unnamed>: f128, %[[VALUE7:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atan2:[0-9]+]] @__builtin_atan2(%[[VALUE8:[0-9]+]] <unnamed>: f64, %[[VALUE9:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atan2f:[0-9]+]] @__builtin_atan2f(%[[VALUE10:[0-9]+]] <unnamed>: f32, %[[VALUE11:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atan2l:[0-9]+]] @__builtin_atan2l(%[[VALUE12:[0-9]+]] <unnamed>: f64, %[[VALUE13:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atan2f128:[0-9]+]] @__builtin_atan2f128(%[[VALUE14:[0-9]+]] <unnamed>: f128, %[[VALUE15:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysign:[0-9]+]] @__builtin_copysign(%[[VALUE16:[0-9]+]] <unnamed>: f64, %[[VALUE17:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysignf:[0-9]+]] @__builtin_copysignf(%[[VALUE18:[0-9]+]] <unnamed>: f32, %[[VALUE19:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysignl:[0-9]+]] @__builtin_copysignl(%[[VALUE20:[0-9]+]] <unnamed>: f64, %[[VALUE21:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_copysignf128:[0-9]+]] @__builtin_copysignf128(%[[VALUE22:[0-9]+]] <unnamed>: f128, %[[VALUE23:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabs:[0-9]+]] @__builtin_fabs(%[[VALUE24:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsf:[0-9]+]] @__builtin_fabsf(%[[VALUE25:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsl:[0-9]+]] @__builtin_fabsl(%[[VALUE26:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fabsf128:[0-9]+]] @__builtin_fabsf128(%[[VALUE27:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_frexp:[0-9]+]] @__builtin_frexp(%[[VALUE28:[0-9]+]] <unnamed>: f64, %[[VALUE29:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_frexpf:[0-9]+]] @__builtin_frexpf(%[[VALUE30:[0-9]+]] <unnamed>: f32, %[[VALUE31:[0-9]+]] <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_frexpl:[0-9]+]] @__builtin_frexpl(%[[VALUE32:[0-9]+]] <unnamed>: f64, %[[VALUE33:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_frexpf128:[0-9]+]] @__builtin_frexpf128(%[[VALUE34:[0-9]+]] <unnamed>: f128, %[[VALUE35:[0-9]+]] <unnamed>: ptr<i32>) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_val:[0-9]+]] @__builtin_huge_val() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_valf:[0-9]+]] @__builtin_huge_valf() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_vall:[0-9]+]] @__builtin_huge_vall() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_valf128:[0-9]+]] @__builtin_huge_valf128() -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff:[0-9]+]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infl:[0-9]+]] @__builtin_infl() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff128:[0-9]+]] @__builtin_inff128() -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ldexp:[0-9]+]] @__builtin_ldexp(%[[VALUE36:[0-9]+]] <unnamed>: f64, %[[VALUE37:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ldexpf:[0-9]+]] @__builtin_ldexpf(%[[VALUE38:[0-9]+]] <unnamed>: f32, %[[VALUE39:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ldexpl:[0-9]+]] @__builtin_ldexpl(%[[VALUE40:[0-9]+]] <unnamed>: f64, %[[VALUE41:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ldexpf128:[0-9]+]] @__builtin_ldexpf128(%[[VALUE42:[0-9]+]] <unnamed>: f128, %[[VALUE43:[0-9]+]] <unnamed>: i32) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_modf:[0-9]+]] @__builtin_modf(%[[VALUE44:[0-9]+]] <unnamed>: f64, %[[VALUE45:[0-9]+]] <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_modff:[0-9]+]] @__builtin_modff(%[[VALUE46:[0-9]+]] <unnamed>: f32, %[[VALUE47:[0-9]+]] <unnamed>: ptr<f32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_modfl:[0-9]+]] @__builtin_modfl(%[[VALUE48:[0-9]+]] <unnamed>: f64, %[[VALUE49:[0-9]+]] <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_modff128:[0-9]+]] @__builtin_modff128(%[[VALUE50:[0-9]+]] <unnamed>: f128, %[[VALUE51:[0-9]+]] <unnamed>: ptr<f128>) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan:[0-9]+]] @__builtin_nan(%[[VALUE52:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf:[0-9]+]] @__builtin_nanf(%[[VALUE53:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanl:[0-9]+]] @__builtin_nanl(%[[VALUE54:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf128:[0-9]+]] @__builtin_nanf128(%[[VALUE55:[0-9]+]] <unnamed>: ptr<const i8>) -> f128 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nans:[0-9]+]] @__builtin_nans(%[[VALUE56:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansf:[0-9]+]] @__builtin_nansf(%[[VALUE57:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansl:[0-9]+]] @__builtin_nansl(%[[VALUE58:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansf128:[0-9]+]] @__builtin_nansf128(%[[VALUE59:[0-9]+]] <unnamed>: ptr<const i8>) -> f128 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_pow:[0-9]+]] @__builtin_pow(%[[VALUE60:[0-9]+]] <unnamed>: f64, %[[VALUE61:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powf:[0-9]+]] @__builtin_powf(%[[VALUE62:[0-9]+]] <unnamed>: f32, %[[VALUE63:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powl:[0-9]+]] @__builtin_powl(%[[VALUE64:[0-9]+]] <unnamed>: f64, %[[VALUE65:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powf128:[0-9]+]] @__builtin_powf128(%[[VALUE66:[0-9]+]] <unnamed>: f128, %[[VALUE67:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powi:[0-9]+]] @__builtin_powi(%[[VALUE68:[0-9]+]] <unnamed>: f64, %[[VALUE69:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powif:[0-9]+]] @__builtin_powif(%[[VALUE70:[0-9]+]] <unnamed>: f32, %[[VALUE71:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_powil:[0-9]+]] @__builtin_powil(%[[VALUE72:[0-9]+]] <unnamed>: f64, %[[VALUE73:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_acos:[0-9]+]] @__builtin_acos(%[[VALUE74:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_acosf:[0-9]+]] @__builtin_acosf(%[[VALUE75:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_acosl:[0-9]+]] @__builtin_acosl(%[[VALUE76:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_acosf128:[0-9]+]] @__builtin_acosf128(%[[VALUE77:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_acosh:[0-9]+]] @__builtin_acosh(%[[VALUE78:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_acoshf:[0-9]+]] @__builtin_acoshf(%[[VALUE79:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_acoshl:[0-9]+]] @__builtin_acoshl(%[[VALUE80:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_acoshf128:[0-9]+]] @__builtin_acoshf128(%[[VALUE81:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_asin:[0-9]+]] @__builtin_asin(%[[VALUE82:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_asinf:[0-9]+]] @__builtin_asinf(%[[VALUE83:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_asinl:[0-9]+]] @__builtin_asinl(%[[VALUE84:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_asinf128:[0-9]+]] @__builtin_asinf128(%[[VALUE85:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_asinh:[0-9]+]] @__builtin_asinh(%[[VALUE86:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_asinhf:[0-9]+]] @__builtin_asinhf(%[[VALUE87:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_asinhl:[0-9]+]] @__builtin_asinhl(%[[VALUE88:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_asinhf128:[0-9]+]] @__builtin_asinhf128(%[[VALUE89:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atan:[0-9]+]] @__builtin_atan(%[[VALUE90:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atanf:[0-9]+]] @__builtin_atanf(%[[VALUE91:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atanl:[0-9]+]] @__builtin_atanl(%[[VALUE92:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atanf128:[0-9]+]] @__builtin_atanf128(%[[VALUE93:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atanh:[0-9]+]] @__builtin_atanh(%[[VALUE94:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atanhf:[0-9]+]] @__builtin_atanhf(%[[VALUE95:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atanhl:[0-9]+]] @__builtin_atanhl(%[[VALUE96:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_atanhf128:[0-9]+]] @__builtin_atanhf128(%[[VALUE97:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cbrt:[0-9]+]] @__builtin_cbrt(%[[VALUE98:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cbrtf:[0-9]+]] @__builtin_cbrtf(%[[VALUE99:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cbrtl:[0-9]+]] @__builtin_cbrtl(%[[VALUE100:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cbrtf128:[0-9]+]] @__builtin_cbrtf128(%[[VALUE101:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ceil:[0-9]+]] @__builtin_ceil(%[[VALUE102:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ceilf:[0-9]+]] @__builtin_ceilf(%[[VALUE103:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ceill:[0-9]+]] @__builtin_ceill(%[[VALUE104:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ceilf128:[0-9]+]] @__builtin_ceilf128(%[[VALUE105:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cos:[0-9]+]] @__builtin_cos(%[[VALUE106:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cosf:[0-9]+]] @__builtin_cosf(%[[VALUE107:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cosl:[0-9]+]] @__builtin_cosl(%[[VALUE108:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cosf128:[0-9]+]] @__builtin_cosf128(%[[VALUE109:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cosh:[0-9]+]] @__builtin_cosh(%[[VALUE110:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coshf:[0-9]+]] @__builtin_coshf(%[[VALUE111:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coshl:[0-9]+]] @__builtin_coshl(%[[VALUE112:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_coshf128:[0-9]+]] @__builtin_coshf128(%[[VALUE113:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_erf:[0-9]+]] @__builtin_erf(%[[VALUE114:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_erff:[0-9]+]] @__builtin_erff(%[[VALUE115:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_erfl:[0-9]+]] @__builtin_erfl(%[[VALUE116:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_erff128:[0-9]+]] @__builtin_erff128(%[[VALUE117:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_erfc:[0-9]+]] @__builtin_erfc(%[[VALUE118:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_erfcf:[0-9]+]] @__builtin_erfcf(%[[VALUE119:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_erfcl:[0-9]+]] @__builtin_erfcl(%[[VALUE120:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_erfcf128:[0-9]+]] @__builtin_erfcf128(%[[VALUE121:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp:[0-9]+]] @__builtin_exp(%[[VALUE122:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expf:[0-9]+]] @__builtin_expf(%[[VALUE123:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expl:[0-9]+]] @__builtin_expl(%[[VALUE124:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expf128:[0-9]+]] @__builtin_expf128(%[[VALUE125:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp2:[0-9]+]] @__builtin_exp2(%[[VALUE126:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp2f:[0-9]+]] @__builtin_exp2f(%[[VALUE127:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp2l:[0-9]+]] @__builtin_exp2l(%[[VALUE128:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp2f128:[0-9]+]] @__builtin_exp2f128(%[[VALUE129:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp10:[0-9]+]] @__builtin_exp10(%[[VALUE130:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp10f:[0-9]+]] @__builtin_exp10f(%[[VALUE131:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp10l:[0-9]+]] @__builtin_exp10l(%[[VALUE132:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp10f128:[0-9]+]] @__builtin_exp10f128(%[[VALUE133:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expm1:[0-9]+]] @__builtin_expm1(%[[VALUE134:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expm1f:[0-9]+]] @__builtin_expm1f(%[[VALUE135:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expm1l:[0-9]+]] @__builtin_expm1l(%[[VALUE136:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expm1f128:[0-9]+]] @__builtin_expm1f128(%[[VALUE137:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fdim:[0-9]+]] @__builtin_fdim(%[[VALUE138:[0-9]+]] <unnamed>: f64, %[[VALUE139:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fdimf:[0-9]+]] @__builtin_fdimf(%[[VALUE140:[0-9]+]] <unnamed>: f32, %[[VALUE141:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fdiml:[0-9]+]] @__builtin_fdiml(%[[VALUE142:[0-9]+]] <unnamed>: f64, %[[VALUE143:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fdimf128:[0-9]+]] @__builtin_fdimf128(%[[VALUE144:[0-9]+]] <unnamed>: f128, %[[VALUE145:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_floor:[0-9]+]] @__builtin_floor(%[[VALUE146:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_floorf:[0-9]+]] @__builtin_floorf(%[[VALUE147:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_floorl:[0-9]+]] @__builtin_floorl(%[[VALUE148:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_floorf128:[0-9]+]] @__builtin_floorf128(%[[VALUE149:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fma:[0-9]+]] @__builtin_fma(%[[VALUE150:[0-9]+]] <unnamed>: f64, %[[VALUE151:[0-9]+]] <unnamed>: f64, %[[VALUE152:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmaf:[0-9]+]] @__builtin_fmaf(%[[VALUE153:[0-9]+]] <unnamed>: f32, %[[VALUE154:[0-9]+]] <unnamed>: f32, %[[VALUE155:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmal:[0-9]+]] @__builtin_fmal(%[[VALUE156:[0-9]+]] <unnamed>: f64, %[[VALUE157:[0-9]+]] <unnamed>: f64, %[[VALUE158:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmaf128:[0-9]+]] @__builtin_fmaf128(%[[VALUE159:[0-9]+]] <unnamed>: f128, %[[VALUE160:[0-9]+]] <unnamed>: f128, %[[VALUE161:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmaf16:[0-9]+]] @__builtin_fmaf16(%[[VALUE162:[0-9]+]] <unnamed>: f16, %[[VALUE163:[0-9]+]] <unnamed>: f16, %[[VALUE164:[0-9]+]] <unnamed>: f16) -> f16 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmax:[0-9]+]] @__builtin_fmax(%[[VALUE165:[0-9]+]] <unnamed>: f64, %[[VALUE166:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmaxf:[0-9]+]] @__builtin_fmaxf(%[[VALUE167:[0-9]+]] <unnamed>: f32, %[[VALUE168:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmaxl:[0-9]+]] @__builtin_fmaxl(%[[VALUE169:[0-9]+]] <unnamed>: f64, %[[VALUE170:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmaxf128:[0-9]+]] @__builtin_fmaxf128(%[[VALUE171:[0-9]+]] <unnamed>: f128, %[[VALUE172:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmin:[0-9]+]] @__builtin_fmin(%[[VALUE173:[0-9]+]] <unnamed>: f64, %[[VALUE174:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fminf:[0-9]+]] @__builtin_fminf(%[[VALUE175:[0-9]+]] <unnamed>: f32, %[[VALUE176:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fminl:[0-9]+]] @__builtin_fminl(%[[VALUE177:[0-9]+]] <unnamed>: f64, %[[VALUE178:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fminf128:[0-9]+]] @__builtin_fminf128(%[[VALUE179:[0-9]+]] <unnamed>: f128, %[[VALUE180:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_hypot:[0-9]+]] @__builtin_hypot(%[[VALUE181:[0-9]+]] <unnamed>: f64, %[[VALUE182:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_hypotf:[0-9]+]] @__builtin_hypotf(%[[VALUE183:[0-9]+]] <unnamed>: f32, %[[VALUE184:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_hypotl:[0-9]+]] @__builtin_hypotl(%[[VALUE185:[0-9]+]] <unnamed>: f64, %[[VALUE186:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_hypotf128:[0-9]+]] @__builtin_hypotf128(%[[VALUE187:[0-9]+]] <unnamed>: f128, %[[VALUE188:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ilogb:[0-9]+]] @__builtin_ilogb(%[[VALUE189:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ilogbf:[0-9]+]] @__builtin_ilogbf(%[[VALUE190:[0-9]+]] <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ilogbl:[0-9]+]] @__builtin_ilogbl(%[[VALUE191:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ilogbf128:[0-9]+]] @__builtin_ilogbf128(%[[VALUE192:[0-9]+]] <unnamed>: f128) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lgamma:[0-9]+]] @__builtin_lgamma(%[[VALUE193:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lgammaf:[0-9]+]] @__builtin_lgammaf(%[[VALUE194:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lgammal:[0-9]+]] @__builtin_lgammal(%[[VALUE195:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lgammaf128:[0-9]+]] @__builtin_lgammaf128(%[[VALUE196:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llrint:[0-9]+]] @__builtin_llrint(%[[VALUE197:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llrintf:[0-9]+]] @__builtin_llrintf(%[[VALUE198:[0-9]+]] <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llrintl:[0-9]+]] @__builtin_llrintl(%[[VALUE199:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llrintf128:[0-9]+]] @__builtin_llrintf128(%[[VALUE200:[0-9]+]] <unnamed>: f128) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llround:[0-9]+]] @__builtin_llround(%[[VALUE201:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llroundf:[0-9]+]] @__builtin_llroundf(%[[VALUE202:[0-9]+]] <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llroundl:[0-9]+]] @__builtin_llroundl(%[[VALUE203:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llroundf128:[0-9]+]] @__builtin_llroundf128(%[[VALUE204:[0-9]+]] <unnamed>: f128) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log:[0-9]+]] @__builtin_log(%[[VALUE205:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_logf:[0-9]+]] @__builtin_logf(%[[VALUE206:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_logl:[0-9]+]] @__builtin_logl(%[[VALUE207:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_logf128:[0-9]+]] @__builtin_logf128(%[[VALUE208:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log10:[0-9]+]] @__builtin_log10(%[[VALUE209:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log10f:[0-9]+]] @__builtin_log10f(%[[VALUE210:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log10l:[0-9]+]] @__builtin_log10l(%[[VALUE211:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log10f128:[0-9]+]] @__builtin_log10f128(%[[VALUE212:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log1p:[0-9]+]] @__builtin_log1p(%[[VALUE213:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log1pf:[0-9]+]] @__builtin_log1pf(%[[VALUE214:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log1pl:[0-9]+]] @__builtin_log1pl(%[[VALUE215:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log1pf128:[0-9]+]] @__builtin_log1pf128(%[[VALUE216:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log2:[0-9]+]] @__builtin_log2(%[[VALUE217:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log2f:[0-9]+]] @__builtin_log2f(%[[VALUE218:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log2l:[0-9]+]] @__builtin_log2l(%[[VALUE219:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log2f128:[0-9]+]] @__builtin_log2f128(%[[VALUE220:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_logb:[0-9]+]] @__builtin_logb(%[[VALUE221:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_logbf:[0-9]+]] @__builtin_logbf(%[[VALUE222:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_logbl:[0-9]+]] @__builtin_logbl(%[[VALUE223:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_logbf128:[0-9]+]] @__builtin_logbf128(%[[VALUE224:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lrint:[0-9]+]] @__builtin_lrint(%[[VALUE225:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lrintf:[0-9]+]] @__builtin_lrintf(%[[VALUE226:[0-9]+]] <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lrintl:[0-9]+]] @__builtin_lrintl(%[[VALUE227:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lrintf128:[0-9]+]] @__builtin_lrintf128(%[[VALUE228:[0-9]+]] <unnamed>: f128) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lround:[0-9]+]] @__builtin_lround(%[[VALUE229:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lroundf:[0-9]+]] @__builtin_lroundf(%[[VALUE230:[0-9]+]] <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lroundl:[0-9]+]] @__builtin_lroundl(%[[VALUE231:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lroundf128:[0-9]+]] @__builtin_lroundf128(%[[VALUE232:[0-9]+]] <unnamed>: f128) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nearbyint:[0-9]+]] @__builtin_nearbyint(%[[VALUE233:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nearbyintf:[0-9]+]] @__builtin_nearbyintf(%[[VALUE234:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nearbyintl:[0-9]+]] @__builtin_nearbyintl(%[[VALUE235:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nearbyintf128:[0-9]+]] @__builtin_nearbyintf128(%[[VALUE236:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nextafter:[0-9]+]] @__builtin_nextafter(%[[VALUE237:[0-9]+]] <unnamed>: f64, %[[VALUE238:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nextafterf:[0-9]+]] @__builtin_nextafterf(%[[VALUE239:[0-9]+]] <unnamed>: f32, %[[VALUE240:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nextafterl:[0-9]+]] @__builtin_nextafterl(%[[VALUE241:[0-9]+]] <unnamed>: f64, %[[VALUE242:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nextafterf128:[0-9]+]] @__builtin_nextafterf128(%[[VALUE243:[0-9]+]] <unnamed>: f128, %[[VALUE244:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nexttoward:[0-9]+]] @__builtin_nexttoward(%[[VALUE245:[0-9]+]] <unnamed>: f64, %[[VALUE246:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nexttowardf:[0-9]+]] @__builtin_nexttowardf(%[[VALUE247:[0-9]+]] <unnamed>: f32, %[[VALUE248:[0-9]+]] <unnamed>: f64) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nexttowardl:[0-9]+]] @__builtin_nexttowardl(%[[VALUE249:[0-9]+]] <unnamed>: f64, %[[VALUE250:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nexttowardf128:[0-9]+]] @__builtin_nexttowardf128(%[[VALUE251:[0-9]+]] <unnamed>: f128, %[[VALUE252:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_remainder:[0-9]+]] @__builtin_remainder(%[[VALUE253:[0-9]+]] <unnamed>: f64, %[[VALUE254:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_remainderf:[0-9]+]] @__builtin_remainderf(%[[VALUE255:[0-9]+]] <unnamed>: f32, %[[VALUE256:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_remainderl:[0-9]+]] @__builtin_remainderl(%[[VALUE257:[0-9]+]] <unnamed>: f64, %[[VALUE258:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_remainderf128:[0-9]+]] @__builtin_remainderf128(%[[VALUE259:[0-9]+]] <unnamed>: f128, %[[VALUE260:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_remquo:[0-9]+]] @__builtin_remquo(%[[VALUE261:[0-9]+]] <unnamed>: f64, %[[VALUE262:[0-9]+]] <unnamed>: f64, %[[VALUE263:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_remquof:[0-9]+]] @__builtin_remquof(%[[VALUE264:[0-9]+]] <unnamed>: f32, %[[VALUE265:[0-9]+]] <unnamed>: f32, %[[VALUE266:[0-9]+]] <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_remquol:[0-9]+]] @__builtin_remquol(%[[VALUE267:[0-9]+]] <unnamed>: f64, %[[VALUE268:[0-9]+]] <unnamed>: f64, %[[VALUE269:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_remquof128:[0-9]+]] @__builtin_remquof128(%[[VALUE270:[0-9]+]] <unnamed>: f128, %[[VALUE271:[0-9]+]] <unnamed>: f128, %[[VALUE272:[0-9]+]] <unnamed>: ptr<i32>) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rint:[0-9]+]] @__builtin_rint(%[[VALUE273:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rintf:[0-9]+]] @__builtin_rintf(%[[VALUE274:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rintl:[0-9]+]] @__builtin_rintl(%[[VALUE275:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_rintf128:[0-9]+]] @__builtin_rintf128(%[[VALUE276:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_round:[0-9]+]] @__builtin_round(%[[VALUE277:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_roundf:[0-9]+]] @__builtin_roundf(%[[VALUE278:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_roundl:[0-9]+]] @__builtin_roundl(%[[VALUE279:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_roundf128:[0-9]+]] @__builtin_roundf128(%[[VALUE280:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalbln:[0-9]+]] @__builtin_scalbln(%[[VALUE281:[0-9]+]] <unnamed>: f64, %[[VALUE282:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalblnf:[0-9]+]] @__builtin_scalblnf(%[[VALUE283:[0-9]+]] <unnamed>: f32, %[[VALUE284:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalblnl:[0-9]+]] @__builtin_scalblnl(%[[VALUE285:[0-9]+]] <unnamed>: f64, %[[VALUE286:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalblnf128:[0-9]+]] @__builtin_scalblnf128(%[[VALUE287:[0-9]+]] <unnamed>: f128, %[[VALUE288:[0-9]+]] <unnamed>: i32) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalbn:[0-9]+]] @__builtin_scalbn(%[[VALUE289:[0-9]+]] <unnamed>: f64, %[[VALUE290:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalbnf:[0-9]+]] @__builtin_scalbnf(%[[VALUE291:[0-9]+]] <unnamed>: f32, %[[VALUE292:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalbnl:[0-9]+]] @__builtin_scalbnl(%[[VALUE293:[0-9]+]] <unnamed>: f64, %[[VALUE294:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalbnf128:[0-9]+]] @__builtin_scalbnf128(%[[VALUE295:[0-9]+]] <unnamed>: f128, %[[VALUE296:[0-9]+]] <unnamed>: i32) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sin:[0-9]+]] @__builtin_sin(%[[VALUE297:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sinf:[0-9]+]] @__builtin_sinf(%[[VALUE298:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sinl:[0-9]+]] @__builtin_sinl(%[[VALUE299:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sinf128:[0-9]+]] @__builtin_sinf128(%[[VALUE300:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sinh:[0-9]+]] @__builtin_sinh(%[[VALUE301:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sinhf:[0-9]+]] @__builtin_sinhf(%[[VALUE302:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sinhl:[0-9]+]] @__builtin_sinhl(%[[VALUE303:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sinhf128:[0-9]+]] @__builtin_sinhf128(%[[VALUE304:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sincos:[0-9]+]] @__builtin_sincos(%[[VALUE305:[0-9]+]] <unnamed>: f64, %[[VALUE306:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE307:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sincosf:[0-9]+]] @__builtin_sincosf(%[[VALUE308:[0-9]+]] <unnamed>: f32, %[[VALUE309:[0-9]+]] <unnamed>: ptr<f32>, %[[VALUE310:[0-9]+]] <unnamed>: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sincosl:[0-9]+]] @__builtin_sincosl(%[[VALUE311:[0-9]+]] <unnamed>: f64, %[[VALUE312:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE313:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sincosf128:[0-9]+]] @__builtin_sincosf128(%[[VALUE314:[0-9]+]] <unnamed>: f128, %[[VALUE315:[0-9]+]] <unnamed>: ptr<f128>, %[[VALUE316:[0-9]+]] <unnamed>: ptr<f128>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sincospi:[0-9]+]] @__builtin_sincospi(%[[VALUE317:[0-9]+]] <unnamed>: f64, %[[VALUE318:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE319:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sincospif:[0-9]+]] @__builtin_sincospif(%[[VALUE320:[0-9]+]] <unnamed>: f32, %[[VALUE321:[0-9]+]] <unnamed>: ptr<f32>, %[[VALUE322:[0-9]+]] <unnamed>: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sincospil:[0-9]+]] @__builtin_sincospil(%[[VALUE323:[0-9]+]] <unnamed>: f64, %[[VALUE324:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE325:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrt:[0-9]+]] @__builtin_sqrt(%[[VALUE326:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrtf:[0-9]+]] @__builtin_sqrtf(%[[VALUE327:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrtl:[0-9]+]] @__builtin_sqrtl(%[[VALUE328:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrtf128:[0-9]+]] @__builtin_sqrtf128(%[[VALUE329:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tan:[0-9]+]] @__builtin_tan(%[[VALUE330:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tanf:[0-9]+]] @__builtin_tanf(%[[VALUE331:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tanl:[0-9]+]] @__builtin_tanl(%[[VALUE332:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tanf128:[0-9]+]] @__builtin_tanf128(%[[VALUE333:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tanh:[0-9]+]] @__builtin_tanh(%[[VALUE334:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tanhf:[0-9]+]] @__builtin_tanhf(%[[VALUE335:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tanhl:[0-9]+]] @__builtin_tanhl(%[[VALUE336:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tanhf128:[0-9]+]] @__builtin_tanhf128(%[[VALUE337:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tgamma:[0-9]+]] @__builtin_tgamma(%[[VALUE338:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tgammaf:[0-9]+]] @__builtin_tgammaf(%[[VALUE339:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tgammal:[0-9]+]] @__builtin_tgammal(%[[VALUE340:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tgammaf128:[0-9]+]] @__builtin_tgammaf128(%[[VALUE341:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_trunc:[0-9]+]] @__builtin_trunc(%[[VALUE342:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_truncf:[0-9]+]] @__builtin_truncf(%[[VALUE343:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_truncl:[0-9]+]] @__builtin_truncl(%[[VALUE344:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_truncf128:[0-9]+]] @__builtin_truncf128(%[[VALUE345:[0-9]+]] <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_d:[0-9]+]] d: ptr<f64>, %[[VALUE_f:[0-9]+]] f: f32, %[[VALUE_fp:[0-9]+]] fp: ptr<f32>, %[[VALUE_l:[0-9]+]] l: ptr<f64>, %[[VALUE_i:[0-9]+]] i: ptr<i32>, %[[VALUE_c:[0-9]+]] c: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fmod]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_fmodf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fmodl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_fmodf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_atan2]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_atan2f]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_atan2l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_atan2f128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysign]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_copysignf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_copysignl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_copysignf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_fabs]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_fabsf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_fabsl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_fabsf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%[[VALUE___builtin_frexp]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, ptr<i32>) -> f32>(%[[VALUE___builtin_frexpf]], read<f32>(%[[VALUE_f]]), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%[[VALUE___builtin_frexpl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, ptr<i32>) -> f128>(%[[VALUE___builtin_frexpf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%[[VALUE___builtin_huge_val]]);
// DEFAULT-NEXT:         call<f32, signature=fn() -> f32>(%[[VALUE___builtin_huge_valf]]);
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%[[VALUE___builtin_huge_vall]]);
// DEFAULT-NEXT:         call<f128, signature=fn() -> f128>(%[[VALUE___builtin_huge_valf128]]);
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]);
// DEFAULT-NEXT:         call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]);
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%[[VALUE___builtin_infl]]);
// DEFAULT-NEXT:         call<f128, signature=fn() -> f128>(%[[VALUE___builtin_inff128]]);
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_ldexp]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE___builtin_ldexpf]], read<f32>(%[[VALUE_f]]), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_ldexpl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i32) -> f128>(%[[VALUE___builtin_ldexpf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%[[VALUE___builtin_modf]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, ptr<f32>) -> f32>(%[[VALUE___builtin_modff]], read<f32>(%[[VALUE_f]]), read<ptr<f32>>(%[[VALUE_fp]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%[[VALUE___builtin_modfl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_l]]));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, ptr<f128>) -> f128>(%[[VALUE___builtin_modff128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), pointer_cast<ptr<f128>, reason=arg>(read<ptr<f64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nanl]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f128, signature=fn(ptr<const i8>) -> f128>(%[[VALUE___builtin_nanf128]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nans]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nansf]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nansl]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f128, signature=fn(ptr<const i8>) -> f128>(%[[VALUE___builtin_nansf128]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_pow]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_powf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_powl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_powf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_powi]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE___builtin_powif]], read<f32>(%[[VALUE_f]]), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_powil]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_acos]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_acosf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_acosl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_acosf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_acosh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_acoshf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_acoshl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_acoshf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_asin]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_asinf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_asinl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_asinf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_asinh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_asinhf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_asinhl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_asinhf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_atan]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_atanf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_atanl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_atanf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_atanh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_atanhf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_atanhl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_atanhf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cbrt]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_cbrtf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cbrtl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_cbrtf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_ceil]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_ceilf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_ceill]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_ceilf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cos]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_cosf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cosl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_cosf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cosh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_coshf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_coshl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_coshf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_erf]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_erff]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_erfl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_erff128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_erfc]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_erfcf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_erfcl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_erfcf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_exp]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_expf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_expl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_expf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_exp2]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_exp2f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_exp2l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_exp2f128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_exp10]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_exp10f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_exp10l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_exp10f128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_expm1]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_expm1f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_expm1l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_expm1f128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fdim]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_fdimf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fdiml]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_fdimf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_floor]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_floorf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_floorl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_floorf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE___builtin_fma]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE___builtin_fmaf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE___builtin_fmal]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128, f128) -> f128>(%[[VALUE___builtin_fmaf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f16, signature=fn(f16, f16, f16) -> f16>(%[[VALUE___builtin_fmaf16]], float_narrow<f16, reason=arg, rounding=nearest_even, exceptions=ignore>(read<f32>(%[[VALUE_f]])), float_narrow<f16, reason=arg, rounding=nearest_even, exceptions=ignore>(read<f32>(%[[VALUE_f]])), float_narrow<f16, reason=arg, rounding=nearest_even, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fmax]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_fmaxf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fmaxl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_fmaxf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fmin]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_fminf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fminl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_fminf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_hypot]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_hypotf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_hypotl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_hypotf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE___builtin_ilogb]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%[[VALUE___builtin_ilogbf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE___builtin_ilogbl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f128) -> i32>(%[[VALUE___builtin_ilogbf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_lgamma]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_lgammaf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_lgammal]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_lgammaf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE___builtin_llrint]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f32) -> i64>(%[[VALUE___builtin_llrintf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE___builtin_llrintl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(%[[VALUE___builtin_llrintf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE___builtin_llround]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f32) -> i64>(%[[VALUE___builtin_llroundf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE___builtin_llroundl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(%[[VALUE___builtin_llroundf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_logf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_logl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_logf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log10]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_log10f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log10l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_log10f128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log1p]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_log1pf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log1pl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_log1pf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log2]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_log2f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log2l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_log2f128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_logb]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_logbf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_logbl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_logbf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE___builtin_lrint]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%[[VALUE___builtin_lrintf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE___builtin_lrintl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f128) -> i32>(%[[VALUE___builtin_lrintf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE___builtin_lround]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%[[VALUE___builtin_lroundf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE___builtin_lroundl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f128) -> i32>(%[[VALUE___builtin_lroundf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_nearbyint]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_nearbyintf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_nearbyintl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_nearbyintf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_nextafter]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_nextafterf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_nextafterl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_nextafterf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_nexttoward]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f64) -> f32>(%[[VALUE___builtin_nexttowardf]], read<f32>(%[[VALUE_f]]), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_nexttowardl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_nexttowardf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_remainder]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE___builtin_remainderf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_remainderl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%[[VALUE___builtin_remainderf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%[[VALUE___builtin_remquo]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32, ptr<i32>) -> f32>(%[[VALUE___builtin_remquof]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%[[VALUE___builtin_remquol]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128, ptr<i32>) -> f128>(%[[VALUE___builtin_remquof128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_rint]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_rintf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_rintl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_rintf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_round]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_roundf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_roundl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_roundf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_scalbln]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE___builtin_scalblnf]], read<f32>(%[[VALUE_f]]), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_scalblnl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i32) -> f128>(%[[VALUE___builtin_scalblnf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_scalbn]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE___builtin_scalbnf]], read<f32>(%[[VALUE_f]]), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_scalbnl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i32) -> f128>(%[[VALUE___builtin_scalbnf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sin]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sinf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sinl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_sinf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sinh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sinhf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sinhl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_sinhf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE___builtin_sincos]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_d]]), read<ptr<f64>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%[[VALUE___builtin_sincosf]], read<f32>(%[[VALUE_f]]), read<ptr<f32>>(%[[VALUE_fp]]), read<ptr<f32>>(%[[VALUE_fp]]));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE___builtin_sincosl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_l]]), read<ptr<f64>>(%[[VALUE_l]]));
// DEFAULT-NEXT:         call<void, signature=fn(f128, ptr<f128>, ptr<f128>) -> void>(%[[VALUE___builtin_sincosf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])), pointer_cast<ptr<f128>, reason=arg>(read<ptr<f64>>(%[[VALUE_l]])), pointer_cast<ptr<f128>, reason=arg>(read<ptr<f64>>(%[[VALUE_l]])));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE___builtin_sincospi]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_d]]), read<ptr<f64>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%[[VALUE___builtin_sincospif]], read<f32>(%[[VALUE_f]]), read<ptr<f32>>(%[[VALUE_fp]]), read<ptr<f32>>(%[[VALUE_fp]]));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE___builtin_sincospil]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_l]]), read<ptr<f64>>(%[[VALUE_l]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_sqrtf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrtl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_sqrtf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_tan]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_tanf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_tanl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_tanf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_tanh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_tanhf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_tanhl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_tanhf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_tgamma]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_tgammaf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_tgammal]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_tgammaf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_trunc]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE___builtin_truncf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_truncl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%[[VALUE___builtin_truncf128]], float_widen<f128, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
