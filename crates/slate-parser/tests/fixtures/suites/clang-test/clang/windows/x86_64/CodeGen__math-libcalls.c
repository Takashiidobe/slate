
// Test attributes and builtin codegen of math library calls.

void foo(double *d, float f, float *fp, long double *l, int *i, const char *c) {
  f = fmod(f,f);     f = fmodf(f,f);    f = fmodl(f,f);


  atan2(f,f);    atan2f(f,f) ;  atan2l(f, f);


  copysign(f,f); copysignf(f,f);copysignl(f,f);


  fabs(f);       fabsf(f);      fabsl(f);


  frexp(f,i);    frexpf(f,i);   frexpl(f,i);


  ldexp(f,f);    ldexpf(f,f);   ldexpl(f,f);


  modf(f,d);       modff(f,fp);      modfl(f,l);


  nan(c);        nanf(c);       nanl(c);


  pow(f,f);        powf(f,f);       powl(f,f);



  /* math */
  acos(f);       acosf(f);      acosl(f);



  acosh(f);      acoshf(f);     acoshl(f);


  asin(f);       asinf(f);      asinl(f);


  asinh(f);      asinhf(f);     asinhl(f);


  atan(f);       atanf(f);      atanl(f);


  atanh(f);      atanhf(f);     atanhl(f);


  cbrt(f);       cbrtf(f);      cbrtl(f);


  ceil(f);       ceilf(f);      ceill(f);


  cos(f);        cosf(f);       cosl(f);


  cosh(f);       coshf(f);      coshl(f);


  erf(f);        erff(f);       erfl(f);


  erfc(f);       erfcf(f);      erfcl(f);


  exp(f);        expf(f);       expl(f);


  exp2(f);       exp2f(f);      exp2l(f);


  expm1(f);      expm1f(f);     expm1l(f);


  fdim(f,f);       fdimf(f,f);      fdiml(f,f);


  floor(f);      floorf(f);     floorl(f);


  fma(f,f,f);        fmaf(f,f,f);       fmal(f,f,f);


// On GNU or Win, fma never sets errno, so we can convert to the intrinsic.


// Long double is just double on win, so no f80 use/declaration.


  fmax(f,f);       fmaxf(f,f);      fmaxl(f,f);


  fmin(f,f);       fminf(f,f);      fminl(f,f);


  fmaximum_num(*d,*d);       fmaximum_numf(f,f);      fmaximum_numl(*l,*l);


  fminimum_num(*d,*d);       fminimum_numf(f,f);      fminimum_numl(*l,*l);


  hypot(f,f);      hypotf(f,f);     hypotl(f,f);


  ilogb(f);      ilogbf(f);     ilogbl(f);


  lgamma(f);     lgammaf(f);    lgammal(f);


  llrint(f);     llrintf(f);    llrintl(f);


  llround(f);    llroundf(f);   llroundl(f);


  log(f);        logf(f);       logl(f);


  log10(f);      log10f(f);     log10l(f);


  log1p(f);      log1pf(f);     log1pl(f);


  log2(f);       log2f(f);      log2l(f);


  logb(f);       logbf(f);      logbl(f);


  lrint(f);      lrintf(f);     lrintl(f);


  lround(f);     lroundf(f);    lroundl(f);


  nearbyint(f);  nearbyintf(f); nearbyintl(f);


  nextafter(f,f);  nextafterf(f,f); nextafterl(f,f);


  nexttoward(f,f); nexttowardf(f,f);nexttowardl(f,f);


  remainder(f,f);  remainderf(f,f); remainderl(f,f);


  remquo(f,f,i);  remquof(f,f,i); remquol(f,f,i);


  rint(f);       rintf(f);      rintl(f);


  round(f);      roundf(f);     roundl(f);


  scalbln(f,f);    scalblnf(f,f);   scalblnl(f,f);


  scalbn(f,f);     scalbnf(f,f);    scalbnl(f,f);


  sin(f);        sinf(f);       sinl(f);


  sinh(f);       sinhf(f);      sinhl(f);


sincos(f, d, d);       sincosf(f, fp, fp);        sincosl(f, l, l);


  sqrt(f);       sqrtf(f);      sqrtl(f);


  tan(f);        tanf(f);       tanl(f);


  tanh(f);       tanhf(f);      tanhl(f);


  tgamma(f);     tgammaf(f);    tgammal(f);


  trunc(f);      truncf(f);     truncl(f);

};

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wno-implicit-function-declaration

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
// DEFAULT-NEXT:     fn %[[VALUE_fmod:[0-9]+]] @fmod(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmodf:[0-9]+]] @fmodf(%[[VALUE2:[0-9]+]] <unnamed>: f32, %[[VALUE3:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmodl:[0-9]+]] @fmodl(%[[VALUE4:[0-9]+]] <unnamed>: f64, %[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan2:[0-9]+]] @atan2(%[[VALUE6:[0-9]+]] <unnamed>: f64, %[[VALUE7:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan2f:[0-9]+]] @atan2f(%[[VALUE8:[0-9]+]] <unnamed>: f32, %[[VALUE9:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan2l:[0-9]+]] @atan2l(%[[VALUE10:[0-9]+]] <unnamed>: f64, %[[VALUE11:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_copysign:[0-9]+]] @copysign(%[[VALUE12:[0-9]+]] <unnamed>: f64, %[[VALUE13:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_copysignf:[0-9]+]] @copysignf(%[[VALUE14:[0-9]+]] <unnamed>: f32, %[[VALUE15:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_copysignl:[0-9]+]] @copysignl(%[[VALUE16:[0-9]+]] <unnamed>: f64, %[[VALUE17:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE18:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsf:[0-9]+]] @fabsf(%[[VALUE19:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fabsl:[0-9]+]] @fabsl(%[[VALUE20:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_frexp:[0-9]+]] @frexp(%[[VALUE21:[0-9]+]] <unnamed>: f64, %[[VALUE22:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frexpf:[0-9]+]] @frexpf(%[[VALUE23:[0-9]+]] <unnamed>: f32, %[[VALUE24:[0-9]+]] <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_frexpl:[0-9]+]] @frexpl(%[[VALUE25:[0-9]+]] <unnamed>: f64, %[[VALUE26:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ldexp:[0-9]+]] @ldexp(%[[VALUE27:[0-9]+]] <unnamed>: f64, %[[VALUE28:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ldexpf:[0-9]+]] @ldexpf(%[[VALUE29:[0-9]+]] <unnamed>: f32, %[[VALUE30:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ldexpl:[0-9]+]] @ldexpl(%[[VALUE31:[0-9]+]] <unnamed>: f64, %[[VALUE32:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_modf:[0-9]+]] @modf(%[[VALUE33:[0-9]+]] <unnamed>: f64, %[[VALUE34:[0-9]+]] <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_modff:[0-9]+]] @modff(%[[VALUE35:[0-9]+]] <unnamed>: f32, %[[VALUE36:[0-9]+]] <unnamed>: ptr<f32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_modfl:[0-9]+]] @modfl(%[[VALUE37:[0-9]+]] <unnamed>: f64, %[[VALUE38:[0-9]+]] <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nan:[0-9]+]] @nan(%[[VALUE39:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_nanf:[0-9]+]] @nanf(%[[VALUE40:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_nanl:[0-9]+]] @nanl(%[[VALUE41:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_pow:[0-9]+]] @pow(%[[VALUE42:[0-9]+]] <unnamed>: f64, %[[VALUE43:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_powf:[0-9]+]] @powf(%[[VALUE44:[0-9]+]] <unnamed>: f32, %[[VALUE45:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_powl:[0-9]+]] @powl(%[[VALUE46:[0-9]+]] <unnamed>: f64, %[[VALUE47:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acos:[0-9]+]] @acos(%[[VALUE48:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acosf:[0-9]+]] @acosf(%[[VALUE49:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acosl:[0-9]+]] @acosl(%[[VALUE50:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acosh:[0-9]+]] @acosh(%[[VALUE51:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acoshf:[0-9]+]] @acoshf(%[[VALUE52:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_acoshl:[0-9]+]] @acoshl(%[[VALUE53:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asin:[0-9]+]] @asin(%[[VALUE54:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinf:[0-9]+]] @asinf(%[[VALUE55:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinl:[0-9]+]] @asinl(%[[VALUE56:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinh:[0-9]+]] @asinh(%[[VALUE57:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinhf:[0-9]+]] @asinhf(%[[VALUE58:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_asinhl:[0-9]+]] @asinhl(%[[VALUE59:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan:[0-9]+]] @atan(%[[VALUE60:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanf:[0-9]+]] @atanf(%[[VALUE61:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanl:[0-9]+]] @atanl(%[[VALUE62:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanh:[0-9]+]] @atanh(%[[VALUE63:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanhf:[0-9]+]] @atanhf(%[[VALUE64:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanhl:[0-9]+]] @atanhl(%[[VALUE65:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cbrt:[0-9]+]] @cbrt(%[[VALUE66:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_cbrtf:[0-9]+]] @cbrtf(%[[VALUE67:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_cbrtl:[0-9]+]] @cbrtl(%[[VALUE68:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceil:[0-9]+]] @ceil(%[[VALUE69:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceilf:[0-9]+]] @ceilf(%[[VALUE70:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_ceill:[0-9]+]] @ceill(%[[VALUE71:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_cos:[0-9]+]] @cos(%[[VALUE72:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosf:[0-9]+]] @cosf(%[[VALUE73:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosl:[0-9]+]] @cosl(%[[VALUE74:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosh:[0-9]+]] @cosh(%[[VALUE75:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_coshf:[0-9]+]] @coshf(%[[VALUE76:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_coshl:[0-9]+]] @coshl(%[[VALUE77:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erf:[0-9]+]] @erf(%[[VALUE78:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erff:[0-9]+]] @erff(%[[VALUE79:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erfl:[0-9]+]] @erfl(%[[VALUE80:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erfc:[0-9]+]] @erfc(%[[VALUE81:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erfcf:[0-9]+]] @erfcf(%[[VALUE82:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_erfcl:[0-9]+]] @erfcl(%[[VALUE83:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp:[0-9]+]] @exp(%[[VALUE84:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expf:[0-9]+]] @expf(%[[VALUE85:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expl:[0-9]+]] @expl(%[[VALUE86:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp2:[0-9]+]] @exp2(%[[VALUE87:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp2f:[0-9]+]] @exp2f(%[[VALUE88:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp2l:[0-9]+]] @exp2l(%[[VALUE89:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expm1:[0-9]+]] @expm1(%[[VALUE90:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expm1f:[0-9]+]] @expm1f(%[[VALUE91:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expm1l:[0-9]+]] @expm1l(%[[VALUE92:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fdim:[0-9]+]] @fdim(%[[VALUE93:[0-9]+]] <unnamed>: f64, %[[VALUE94:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fdimf:[0-9]+]] @fdimf(%[[VALUE95:[0-9]+]] <unnamed>: f32, %[[VALUE96:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fdiml:[0-9]+]] @fdiml(%[[VALUE97:[0-9]+]] <unnamed>: f64, %[[VALUE98:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_floor:[0-9]+]] @floor(%[[VALUE99:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_floorf:[0-9]+]] @floorf(%[[VALUE100:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_floorl:[0-9]+]] @floorl(%[[VALUE101:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fma:[0-9]+]] @fma(%[[VALUE102:[0-9]+]] <unnamed>: f64, %[[VALUE103:[0-9]+]] <unnamed>: f64, %[[VALUE104:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmaf:[0-9]+]] @fmaf(%[[VALUE105:[0-9]+]] <unnamed>: f32, %[[VALUE106:[0-9]+]] <unnamed>: f32, %[[VALUE107:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmal:[0-9]+]] @fmal(%[[VALUE108:[0-9]+]] <unnamed>: f64, %[[VALUE109:[0-9]+]] <unnamed>: f64, %[[VALUE110:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fmax:[0-9]+]] @fmax(%[[VALUE111:[0-9]+]] <unnamed>: f64, %[[VALUE112:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmaxf:[0-9]+]] @fmaxf(%[[VALUE113:[0-9]+]] <unnamed>: f32, %[[VALUE114:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmaxl:[0-9]+]] @fmaxl(%[[VALUE115:[0-9]+]] <unnamed>: f64, %[[VALUE116:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmin:[0-9]+]] @fmin(%[[VALUE117:[0-9]+]] <unnamed>: f64, %[[VALUE118:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fminf:[0-9]+]] @fminf(%[[VALUE119:[0-9]+]] <unnamed>: f32, %[[VALUE120:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fminl:[0-9]+]] @fminl(%[[VALUE121:[0-9]+]] <unnamed>: f64, %[[VALUE122:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmaximum_num:[0-9]+]] @fmaximum_num(%[[VALUE123:[0-9]+]] <unnamed>: f64, %[[VALUE124:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmaximum_numf:[0-9]+]] @fmaximum_numf(%[[VALUE125:[0-9]+]] <unnamed>: f32, %[[VALUE126:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fmaximum_numl:[0-9]+]] @fmaximum_numl(%[[VALUE127:[0-9]+]] <unnamed>: f64, %[[VALUE128:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fminimum_num:[0-9]+]] @fminimum_num(%[[VALUE129:[0-9]+]] <unnamed>: f64, %[[VALUE130:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fminimum_numf:[0-9]+]] @fminimum_numf(%[[VALUE131:[0-9]+]] <unnamed>: f32, %[[VALUE132:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_fminimum_numl:[0-9]+]] @fminimum_numl(%[[VALUE133:[0-9]+]] <unnamed>: f64, %[[VALUE134:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_hypot:[0-9]+]] @hypot(%[[VALUE135:[0-9]+]] <unnamed>: f64, %[[VALUE136:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hypotf:[0-9]+]] @hypotf(%[[VALUE137:[0-9]+]] <unnamed>: f32, %[[VALUE138:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_hypotl:[0-9]+]] @hypotl(%[[VALUE139:[0-9]+]] <unnamed>: f64, %[[VALUE140:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ilogb:[0-9]+]] @ilogb(%[[VALUE141:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ilogbf:[0-9]+]] @ilogbf(%[[VALUE142:[0-9]+]] <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ilogbl:[0-9]+]] @ilogbl(%[[VALUE143:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lgamma:[0-9]+]] @lgamma(%[[VALUE144:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lgammaf:[0-9]+]] @lgammaf(%[[VALUE145:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lgammal:[0-9]+]] @lgammal(%[[VALUE146:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llrint:[0-9]+]] @llrint(%[[VALUE147:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llrintf:[0-9]+]] @llrintf(%[[VALUE148:[0-9]+]] <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llrintl:[0-9]+]] @llrintl(%[[VALUE149:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llround:[0-9]+]] @llround(%[[VALUE150:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llroundf:[0-9]+]] @llroundf(%[[VALUE151:[0-9]+]] <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_llroundl:[0-9]+]] @llroundl(%[[VALUE152:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log:[0-9]+]] @log(%[[VALUE153:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logf:[0-9]+]] @logf(%[[VALUE154:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logl:[0-9]+]] @logl(%[[VALUE155:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log10:[0-9]+]] @log10(%[[VALUE156:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log10f:[0-9]+]] @log10f(%[[VALUE157:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log10l:[0-9]+]] @log10l(%[[VALUE158:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log1p:[0-9]+]] @log1p(%[[VALUE159:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log1pf:[0-9]+]] @log1pf(%[[VALUE160:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log1pl:[0-9]+]] @log1pl(%[[VALUE161:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log2:[0-9]+]] @log2(%[[VALUE162:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log2f:[0-9]+]] @log2f(%[[VALUE163:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_log2l:[0-9]+]] @log2l(%[[VALUE164:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logb:[0-9]+]] @logb(%[[VALUE165:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logbf:[0-9]+]] @logbf(%[[VALUE166:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_logbl:[0-9]+]] @logbl(%[[VALUE167:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lrint:[0-9]+]] @lrint(%[[VALUE168:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lrintf:[0-9]+]] @lrintf(%[[VALUE169:[0-9]+]] <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lrintl:[0-9]+]] @lrintl(%[[VALUE170:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lround:[0-9]+]] @lround(%[[VALUE171:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lroundf:[0-9]+]] @lroundf(%[[VALUE172:[0-9]+]] <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lroundl:[0-9]+]] @lroundl(%[[VALUE173:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nearbyint:[0-9]+]] @nearbyint(%[[VALUE174:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nearbyintf:[0-9]+]] @nearbyintf(%[[VALUE175:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nearbyintl:[0-9]+]] @nearbyintl(%[[VALUE176:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_nextafter:[0-9]+]] @nextafter(%[[VALUE177:[0-9]+]] <unnamed>: f64, %[[VALUE178:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nextafterf:[0-9]+]] @nextafterf(%[[VALUE179:[0-9]+]] <unnamed>: f32, %[[VALUE180:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nextafterl:[0-9]+]] @nextafterl(%[[VALUE181:[0-9]+]] <unnamed>: f64, %[[VALUE182:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nexttoward:[0-9]+]] @nexttoward(%[[VALUE183:[0-9]+]] <unnamed>: f64, %[[VALUE184:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nexttowardf:[0-9]+]] @nexttowardf(%[[VALUE185:[0-9]+]] <unnamed>: f32, %[[VALUE186:[0-9]+]] <unnamed>: f64) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_nexttowardl:[0-9]+]] @nexttowardl(%[[VALUE187:[0-9]+]] <unnamed>: f64, %[[VALUE188:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remainder:[0-9]+]] @remainder(%[[VALUE189:[0-9]+]] <unnamed>: f64, %[[VALUE190:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remainderf:[0-9]+]] @remainderf(%[[VALUE191:[0-9]+]] <unnamed>: f32, %[[VALUE192:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remainderl:[0-9]+]] @remainderl(%[[VALUE193:[0-9]+]] <unnamed>: f64, %[[VALUE194:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remquo:[0-9]+]] @remquo(%[[VALUE195:[0-9]+]] <unnamed>: f64, %[[VALUE196:[0-9]+]] <unnamed>: f64, %[[VALUE197:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remquof:[0-9]+]] @remquof(%[[VALUE198:[0-9]+]] <unnamed>: f32, %[[VALUE199:[0-9]+]] <unnamed>: f32, %[[VALUE200:[0-9]+]] <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_remquol:[0-9]+]] @remquol(%[[VALUE201:[0-9]+]] <unnamed>: f64, %[[VALUE202:[0-9]+]] <unnamed>: f64, %[[VALUE203:[0-9]+]] <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rint:[0-9]+]] @rint(%[[VALUE204:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rintf:[0-9]+]] @rintf(%[[VALUE205:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_rintl:[0-9]+]] @rintl(%[[VALUE206:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_round:[0-9]+]] @round(%[[VALUE207:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_roundf:[0-9]+]] @roundf(%[[VALUE208:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_roundl:[0-9]+]] @roundl(%[[VALUE209:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_scalbln:[0-9]+]] @scalbln(%[[VALUE210:[0-9]+]] <unnamed>: f64, %[[VALUE211:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalblnf:[0-9]+]] @scalblnf(%[[VALUE212:[0-9]+]] <unnamed>: f32, %[[VALUE213:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalblnl:[0-9]+]] @scalblnl(%[[VALUE214:[0-9]+]] <unnamed>: f64, %[[VALUE215:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbn:[0-9]+]] @scalbn(%[[VALUE216:[0-9]+]] <unnamed>: f64, %[[VALUE217:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbnf:[0-9]+]] @scalbnf(%[[VALUE218:[0-9]+]] <unnamed>: f32, %[[VALUE219:[0-9]+]] <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbnl:[0-9]+]] @scalbnl(%[[VALUE220:[0-9]+]] <unnamed>: f64, %[[VALUE221:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sin:[0-9]+]] @sin(%[[VALUE222:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinf:[0-9]+]] @sinf(%[[VALUE223:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinl:[0-9]+]] @sinl(%[[VALUE224:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinh:[0-9]+]] @sinh(%[[VALUE225:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinhf:[0-9]+]] @sinhf(%[[VALUE226:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinhl:[0-9]+]] @sinhl(%[[VALUE227:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sincos:[0-9]+]] @sincos(%[[VALUE228:[0-9]+]] <unnamed>: f64, %[[VALUE229:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE230:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sincosf:[0-9]+]] @sincosf(%[[VALUE231:[0-9]+]] <unnamed>: f32, %[[VALUE232:[0-9]+]] <unnamed>: ptr<f32>, %[[VALUE233:[0-9]+]] <unnamed>: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sincosl:[0-9]+]] @sincosl(%[[VALUE234:[0-9]+]] <unnamed>: f64, %[[VALUE235:[0-9]+]] <unnamed>: ptr<f64>, %[[VALUE236:[0-9]+]] <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrt:[0-9]+]] @sqrt(%[[VALUE237:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrtf:[0-9]+]] @sqrtf(%[[VALUE238:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sqrtl:[0-9]+]] @sqrtl(%[[VALUE239:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tan:[0-9]+]] @tan(%[[VALUE240:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanf:[0-9]+]] @tanf(%[[VALUE241:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanl:[0-9]+]] @tanl(%[[VALUE242:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanh:[0-9]+]] @tanh(%[[VALUE243:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanhf:[0-9]+]] @tanhf(%[[VALUE244:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanhl:[0-9]+]] @tanhl(%[[VALUE245:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tgamma:[0-9]+]] @tgamma(%[[VALUE246:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tgammaf:[0-9]+]] @tgammaf(%[[VALUE247:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tgammal:[0-9]+]] @tgammal(%[[VALUE248:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_trunc:[0-9]+]] @trunc(%[[VALUE249:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_truncf:[0-9]+]] @truncf(%[[VALUE250:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_truncl:[0-9]+]] @truncl(%[[VALUE251:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_d:[0-9]+]] d: ptr<f64>, %[[VALUE_f:[0-9]+]] f: f32, %[[VALUE_fp:[0-9]+]] fp: ptr<f32>, %[[VALUE_l:[0-9]+]] l: ptr<f64>, %[[VALUE_i:[0-9]+]] i: ptr<i32>, %[[VALUE_c:[0-9]+]] c: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmod]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmodf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmodl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_atan2]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_atan2f]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_atan2l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysign]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_copysignf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_copysignl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabsl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%[[VALUE_frexp]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, ptr<i32>) -> f32>(%[[VALUE_frexpf]], read<f32>(%[[VALUE_f]]), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%[[VALUE_frexpl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_ldexp]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE_ldexpf]], read<f32>(%[[VALUE_f]]), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_ldexpl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%[[VALUE_modf]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, ptr<f32>) -> f32>(%[[VALUE_modff]], read<f32>(%[[VALUE_f]]), read<ptr<f32>>(%[[VALUE_fp]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%[[VALUE_modfl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_l]]));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE_nan]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE_nanf]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE_nanl]], read<ptr<const i8>>(%[[VALUE_c]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_powf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_powl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_acos]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_acosf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_acosl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_acosh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_acoshf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_acoshl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_asin]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_asinf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_asinl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_asinh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_asinhf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_asinhl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_atan]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_atanf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_atanl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_atanh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_atanhf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_atanhl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cbrt]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_cbrtf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cbrtl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceil]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_ceilf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_ceill]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cos]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_cosf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cosl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_cosh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_coshf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_coshl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_erf]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_erff]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_erfl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_erfc]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_erfcf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_erfcl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_expl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp2]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_exp2f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp2l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_expm1]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_expm1f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_expm1l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fdim]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fdimf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fdiml]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_floorf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_floorl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_fma]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32, f32) -> f32>(%[[VALUE_fmaf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE_fmal]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmax]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmaxf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmaxl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmin]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fminf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fminl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmaximum_num]], read<f64>(deref(read<ptr<f64>>(%[[VALUE_d]]))), read<f64>(deref(read<ptr<f64>>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fmaximum_numf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fmaximum_numl]], read<f64>(deref(read<ptr<f64>>(%[[VALUE_l]]))), read<f64>(deref(read<ptr<f64>>(%[[VALUE_l]]))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fminimum_num]], read<f64>(deref(read<ptr<f64>>(%[[VALUE_d]]))), read<f64>(deref(read<ptr<f64>>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_fminimum_numf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_fminimum_numl]], read<f64>(deref(read<ptr<f64>>(%[[VALUE_l]]))), read<f64>(deref(read<ptr<f64>>(%[[VALUE_l]]))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_hypot]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_hypotf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_hypotl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE_ilogb]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%[[VALUE_ilogbf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE_ilogbl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_lgamma]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_lgammaf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_lgammal]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE_llrint]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f32) -> i64>(%[[VALUE_llrintf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE_llrintl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE_llround]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i64, signature=fn(f32) -> i64>(%[[VALUE_llroundf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%[[VALUE_llroundl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_logf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_logl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log10]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_log10f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log10l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log1p]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_log1pf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log1pl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log2]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_log2f]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_log2l]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_logb]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_logbf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_logbl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE_lrint]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%[[VALUE_lrintf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE_lrintl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE_lround]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%[[VALUE_lroundf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%[[VALUE_lroundl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_nearbyint]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_nearbyintf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_nearbyintl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_nextafter]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_nextafterf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_nextafterl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_nexttoward]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f64) -> f32>(%[[VALUE_nexttowardf]], read<f32>(%[[VALUE_f]]), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_nexttowardl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_remainder]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_remainderf]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_remainderl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%[[VALUE_remquo]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32, ptr<i32>) -> f32>(%[[VALUE_remquof]], read<f32>(%[[VALUE_f]]), read<f32>(%[[VALUE_f]]), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%[[VALUE_remquol]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<i32>>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_rint]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_rintf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_rintl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_round]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_roundf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_roundl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_scalbln]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE_scalblnf]], read<f32>(%[[VALUE_f]]), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_scalblnl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_scalbn]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%[[VALUE_scalbnf]], read<f32>(%[[VALUE_f]]), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE_scalbnl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_sinf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sinl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sinh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_sinhf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sinhl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE_sincos]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_d]]), read<ptr<f64>>(%[[VALUE_d]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%[[VALUE_sincosf]], read<f32>(%[[VALUE_f]]), read<ptr<f32>>(%[[VALUE_fp]]), read<ptr<f32>>(%[[VALUE_fp]]));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE_sincosl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])), read<ptr<f64>>(%[[VALUE_l]]), read<ptr<f64>>(%[[VALUE_l]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrt]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_sqrtf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_sqrtl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tan]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_tanf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tanl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tanh]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_tanhf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tanhl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tgamma]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_tgammaf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_tgammal]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_trunc]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%[[VALUE_truncf]], read<f32>(%[[VALUE_f]]));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%[[VALUE_truncl]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
