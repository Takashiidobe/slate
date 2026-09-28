
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

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
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
// DEFAULT-NEXT:     fn %9 @fmod(%7 <unnamed>: f64, %8 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %12 @fmodf(%10 <unnamed>: f32, %11 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @fmodl(%13 <unnamed>: f64, %14 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %18 @atan2(%16 <unnamed>: f64, %17 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %21 @atan2f(%19 <unnamed>: f32, %20 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %24 @atan2l(%22 <unnamed>: f64, %23 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %27 @copysign(%25 <unnamed>: f64, %26 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %30 @copysignf(%28 <unnamed>: f32, %29 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %33 @copysignl(%31 <unnamed>: f64, %32 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %35 @fabs(%34 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %37 @fabsf(%36 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %39 @fabsl(%38 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %42 @frexp(%40 <unnamed>: f64, %41 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %45 @frexpf(%43 <unnamed>: f32, %44 <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %48 @frexpl(%46 <unnamed>: f64, %47 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %51 @ldexp(%49 <unnamed>: f64, %50 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %54 @ldexpf(%52 <unnamed>: f32, %53 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %57 @ldexpl(%55 <unnamed>: f64, %56 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %60 @modf(%58 <unnamed>: f64, %59 <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %63 @modff(%61 <unnamed>: f32, %62 <unnamed>: ptr<f32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %66 @modfl(%64 <unnamed>: f64, %65 <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %68 @nan(%67 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %70 @nanf(%69 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %72 @nanl(%71 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %75 @pow(%73 <unnamed>: f64, %74 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %78 @powf(%76 <unnamed>: f32, %77 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %81 @powl(%79 <unnamed>: f64, %80 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %83 @acos(%82 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %85 @acosf(%84 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %87 @acosl(%86 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %89 @acosh(%88 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %91 @acoshf(%90 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %93 @acoshl(%92 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %95 @asin(%94 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %97 @asinf(%96 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %99 @asinl(%98 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %101 @asinh(%100 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %103 @asinhf(%102 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %105 @asinhl(%104 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %107 @atan(%106 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %109 @atanf(%108 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %111 @atanl(%110 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %113 @atanh(%112 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %115 @atanhf(%114 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %117 @atanhl(%116 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %119 @cbrt(%118 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %121 @cbrtf(%120 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %123 @cbrtl(%122 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %125 @ceil(%124 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %127 @ceilf(%126 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %129 @ceill(%128 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %131 @cos(%130 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %133 @cosf(%132 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %135 @cosl(%134 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %137 @cosh(%136 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %139 @coshf(%138 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %141 @coshl(%140 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %143 @erf(%142 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %145 @erff(%144 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %147 @erfl(%146 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %149 @erfc(%148 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %151 @erfcf(%150 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %153 @erfcl(%152 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %155 @exp(%154 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %157 @expf(%156 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %159 @expl(%158 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %161 @exp2(%160 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %163 @exp2f(%162 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %165 @exp2l(%164 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %167 @expm1(%166 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %169 @expm1f(%168 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %171 @expm1l(%170 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %174 @fdim(%172 <unnamed>: f64, %173 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %177 @fdimf(%175 <unnamed>: f32, %176 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %180 @fdiml(%178 <unnamed>: f64, %179 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %182 @floor(%181 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %184 @floorf(%183 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %186 @floorl(%185 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %190 @fma(%187 <unnamed>: f64, %188 <unnamed>: f64, %189 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %194 @fmaf(%191 <unnamed>: f32, %192 <unnamed>: f32, %193 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %198 @fmal(%195 <unnamed>: f64, %196 <unnamed>: f64, %197 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %201 @fmax(%199 <unnamed>: f64, %200 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %204 @fmaxf(%202 <unnamed>: f32, %203 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %207 @fmaxl(%205 <unnamed>: f64, %206 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %210 @fmin(%208 <unnamed>: f64, %209 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %213 @fminf(%211 <unnamed>: f32, %212 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %216 @fminl(%214 <unnamed>: f64, %215 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %219 @fmaximum_num(%217 <unnamed>: f64, %218 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %222 @fmaximum_numf(%220 <unnamed>: f32, %221 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %225 @fmaximum_numl(%223 <unnamed>: f64, %224 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %228 @fminimum_num(%226 <unnamed>: f64, %227 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %231 @fminimum_numf(%229 <unnamed>: f32, %230 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %234 @fminimum_numl(%232 <unnamed>: f64, %233 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %237 @hypot(%235 <unnamed>: f64, %236 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %240 @hypotf(%238 <unnamed>: f32, %239 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %243 @hypotl(%241 <unnamed>: f64, %242 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %245 @ilogb(%244 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %247 @ilogbf(%246 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %249 @ilogbl(%248 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %251 @lgamma(%250 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %253 @lgammaf(%252 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %255 @lgammal(%254 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %257 @llrint(%256 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %259 @llrintf(%258 <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %261 @llrintl(%260 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %263 @llround(%262 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %265 @llroundf(%264 <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %267 @llroundl(%266 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %269 @log(%268 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %271 @logf(%270 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %273 @logl(%272 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %275 @log10(%274 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %277 @log10f(%276 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %279 @log10l(%278 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %281 @log1p(%280 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %283 @log1pf(%282 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %285 @log1pl(%284 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %287 @log2(%286 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %289 @log2f(%288 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %291 @log2l(%290 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %293 @logb(%292 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %295 @logbf(%294 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %297 @logbl(%296 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %299 @lrint(%298 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %301 @lrintf(%300 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %303 @lrintl(%302 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %305 @lround(%304 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %307 @lroundf(%306 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %309 @lroundl(%308 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %311 @nearbyint(%310 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %313 @nearbyintf(%312 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %315 @nearbyintl(%314 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %318 @nextafter(%316 <unnamed>: f64, %317 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %321 @nextafterf(%319 <unnamed>: f32, %320 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %324 @nextafterl(%322 <unnamed>: f64, %323 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %327 @nexttoward(%325 <unnamed>: f64, %326 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %330 @nexttowardf(%328 <unnamed>: f32, %329 <unnamed>: f64) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %333 @nexttowardl(%331 <unnamed>: f64, %332 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %336 @remainder(%334 <unnamed>: f64, %335 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %339 @remainderf(%337 <unnamed>: f32, %338 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %342 @remainderl(%340 <unnamed>: f64, %341 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %346 @remquo(%343 <unnamed>: f64, %344 <unnamed>: f64, %345 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %350 @remquof(%347 <unnamed>: f32, %348 <unnamed>: f32, %349 <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %354 @remquol(%351 <unnamed>: f64, %352 <unnamed>: f64, %353 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %356 @rint(%355 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %358 @rintf(%357 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %360 @rintl(%359 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %362 @round(%361 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %364 @roundf(%363 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %366 @roundl(%365 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %369 @scalbln(%367 <unnamed>: f64, %368 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %372 @scalblnf(%370 <unnamed>: f32, %371 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %375 @scalblnl(%373 <unnamed>: f64, %374 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %378 @scalbn(%376 <unnamed>: f64, %377 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %381 @scalbnf(%379 <unnamed>: f32, %380 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %384 @scalbnl(%382 <unnamed>: f64, %383 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %386 @sin(%385 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %388 @sinf(%387 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %390 @sinl(%389 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %392 @sinh(%391 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %394 @sinhf(%393 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %396 @sinhl(%395 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %400 @sincos(%397 <unnamed>: f64, %398 <unnamed>: ptr<f64>, %399 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %404 @sincosf(%401 <unnamed>: f32, %402 <unnamed>: ptr<f32>, %403 <unnamed>: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %408 @sincosl(%405 <unnamed>: f64, %406 <unnamed>: ptr<f64>, %407 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %410 @sqrt(%409 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %412 @sqrtf(%411 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %414 @sqrtl(%413 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %416 @tan(%415 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %418 @tanf(%417 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %420 @tanl(%419 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %422 @tanh(%421 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %424 @tanhf(%423 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %426 @tanhl(%425 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %428 @tgamma(%427 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %430 @tgammaf(%429 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %432 @tgammal(%431 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %434 @trunc(%433 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %436 @truncf(%435 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %438 @truncl(%437 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %0 @foo(%1 d: ptr<f64>, %2 f: f32, %3 fp: ptr<f32>, %4 l: ptr<f64>, %5 i: ptr<i32>, %6 c: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(%2, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%9, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)))));
// DEFAULT-NEXT:         float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%9, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2))));
// DEFAULT-NEXT:         write<f32>(%2, call<f32, signature=fn(f32, f32) -> f32>(%12, read<f32>(%2), read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%12, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         write<f32>(%2, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%15, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)))));
// DEFAULT-NEXT:         float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%15, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%18, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%21, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%24, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%27, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%30, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%33, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%35, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%37, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%39, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%42, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, ptr<i32>) -> f32>(%45, read<f32>(%2), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%48, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%51, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%54, read<f32>(%2), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%57, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%60, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%1));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, ptr<f32>) -> f32>(%63, read<f32>(%2), read<ptr<f32>>(%3));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%66, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%4));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%68, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<const i8>) -> f32>(%70, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%72, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%75, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%78, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%81, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%83, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%85, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%87, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%89, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%91, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%93, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%95, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%97, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%99, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%101, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%103, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%105, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%107, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%109, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%111, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%113, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%115, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%117, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%119, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%121, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%123, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%125, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%127, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%129, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%131, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%133, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%135, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%137, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%139, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%141, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%143, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%145, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%147, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%149, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%151, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%153, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%155, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%157, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%159, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%161, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%163, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%165, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%167, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%169, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%171, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%174, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%177, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%180, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%182, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%184, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%186, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%190, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32, f32) -> f32>(%194, read<f32>(%2), read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%198, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%201, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%204, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%207, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%210, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%213, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%216, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%219, read<f64>(deref(read<ptr<f64>>(%1))), read<f64>(deref(read<ptr<f64>>(%1))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%222, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%225, read<f64>(deref(read<ptr<f64>>(%4))), read<f64>(deref(read<ptr<f64>>(%4))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%228, read<f64>(deref(read<ptr<f64>>(%1))), read<f64>(deref(read<ptr<f64>>(%1))));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%231, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%234, read<f64>(deref(read<ptr<f64>>(%4))), read<f64>(deref(read<ptr<f64>>(%4))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%237, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%240, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%243, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%245, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%247, read<f32>(%2));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%249, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%251, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%253, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%255, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%257, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f32) -> i64>(%259, read<f32>(%2));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%261, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%263, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f32) -> i64>(%265, read<f32>(%2));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%267, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%269, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%271, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%273, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%275, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%277, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%279, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%281, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%283, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%285, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%287, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%289, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%291, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%293, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%295, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%297, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%299, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%301, read<f32>(%2));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%303, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%305, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%307, read<f32>(%2));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%309, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%311, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%313, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%315, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%318, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%321, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%324, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%327, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f64) -> f32>(%330, read<f32>(%2), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%333, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%336, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%339, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%342, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%346, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32, ptr<i32>) -> f32>(%350, read<f32>(%2), read<f32>(%2), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%354, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%356, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%358, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%360, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%362, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%364, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%366, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%369, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%372, read<f32>(%2), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%375, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%378, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%381, read<f32>(%2), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%384, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%386, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%388, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%390, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%392, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%394, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%396, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%400, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%1), read<ptr<f64>>(%1));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%404, read<f32>(%2), read<ptr<f32>>(%3), read<ptr<f32>>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%408, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%4), read<ptr<f64>>(%4));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%410, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%412, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%414, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%416, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%418, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%420, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%422, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%424, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%426, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%428, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%430, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%432, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%434, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%436, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%438, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
