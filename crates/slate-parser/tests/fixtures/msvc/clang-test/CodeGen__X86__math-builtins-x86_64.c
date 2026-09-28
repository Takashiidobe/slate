
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

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
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
// DEFAULT-NEXT:     fn %9 @__builtin_fmod(%7 <unnamed>: f64, %8 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %12 @__builtin_fmodf(%10 <unnamed>: f32, %11 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %15 @__builtin_fmodl(%13 <unnamed>: f64, %14 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %18 @__builtin_fmodf128(%16 <unnamed>: f128, %17 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %21 @__builtin_atan2(%19 <unnamed>: f64, %20 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %24 @__builtin_atan2f(%22 <unnamed>: f32, %23 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_atan2l(%25 <unnamed>: f64, %26 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %30 @__builtin_atan2f128(%28 <unnamed>: f128, %29 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %33 @__builtin_copysign(%31 <unnamed>: f64, %32 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %36 @__builtin_copysignf(%34 <unnamed>: f32, %35 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %39 @__builtin_copysignl(%37 <unnamed>: f64, %38 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %42 @__builtin_copysignf128(%40 <unnamed>: f128, %41 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %44 @__builtin_fabs(%43 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %46 @__builtin_fabsf(%45 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %48 @__builtin_fabsl(%47 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %50 @__builtin_fabsf128(%49 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %53 @__builtin_frexp(%51 <unnamed>: f64, %52 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %56 @__builtin_frexpf(%54 <unnamed>: f32, %55 <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %59 @__builtin_frexpl(%57 <unnamed>: f64, %58 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %62 @__builtin_frexpf128(%60 <unnamed>: f128, %61 <unnamed>: ptr<i32>) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %63 @__builtin_huge_val() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %64 @__builtin_huge_valf() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %65 @__builtin_huge_vall() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %66 @__builtin_huge_valf128() -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %67 @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %68 @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %69 @__builtin_infl() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %70 @__builtin_inff128() -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %73 @__builtin_ldexp(%71 <unnamed>: f64, %72 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %76 @__builtin_ldexpf(%74 <unnamed>: f32, %75 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %79 @__builtin_ldexpl(%77 <unnamed>: f64, %78 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %82 @__builtin_ldexpf128(%80 <unnamed>: f128, %81 <unnamed>: i32) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %85 @__builtin_modf(%83 <unnamed>: f64, %84 <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %88 @__builtin_modff(%86 <unnamed>: f32, %87 <unnamed>: ptr<f32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %91 @__builtin_modfl(%89 <unnamed>: f64, %90 <unnamed>: ptr<f64>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %94 @__builtin_modff128(%92 <unnamed>: f128, %93 <unnamed>: ptr<f128>) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %96 @__builtin_nan(%95 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %98 @__builtin_nanf(%97 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %100 @__builtin_nanl(%99 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %102 @__builtin_nanf128(%101 <unnamed>: ptr<const i8>) -> f128 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %104 @__builtin_nans(%103 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %106 @__builtin_nansf(%105 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %108 @__builtin_nansl(%107 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %110 @__builtin_nansf128(%109 <unnamed>: ptr<const i8>) -> f128 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %113 @__builtin_pow(%111 <unnamed>: f64, %112 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %116 @__builtin_powf(%114 <unnamed>: f32, %115 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %119 @__builtin_powl(%117 <unnamed>: f64, %118 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %122 @__builtin_powf128(%120 <unnamed>: f128, %121 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %125 @__builtin_powi(%123 <unnamed>: f64, %124 <unnamed>: i32) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %128 @__builtin_powif(%126 <unnamed>: f32, %127 <unnamed>: i32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %131 @__builtin_powil(%129 <unnamed>: f64, %130 <unnamed>: i32) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %133 @__builtin_acos(%132 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %135 @__builtin_acosf(%134 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %137 @__builtin_acosl(%136 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %139 @__builtin_acosf128(%138 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %141 @__builtin_acosh(%140 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %143 @__builtin_acoshf(%142 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %145 @__builtin_acoshl(%144 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %147 @__builtin_acoshf128(%146 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %149 @__builtin_asin(%148 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %151 @__builtin_asinf(%150 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %153 @__builtin_asinl(%152 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %155 @__builtin_asinf128(%154 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %157 @__builtin_asinh(%156 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %159 @__builtin_asinhf(%158 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %161 @__builtin_asinhl(%160 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %163 @__builtin_asinhf128(%162 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %165 @__builtin_atan(%164 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %167 @__builtin_atanf(%166 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %169 @__builtin_atanl(%168 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %171 @__builtin_atanf128(%170 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %173 @__builtin_atanh(%172 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %175 @__builtin_atanhf(%174 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %177 @__builtin_atanhl(%176 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %179 @__builtin_atanhf128(%178 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %181 @__builtin_cbrt(%180 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %183 @__builtin_cbrtf(%182 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %185 @__builtin_cbrtl(%184 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %187 @__builtin_cbrtf128(%186 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %189 @__builtin_ceil(%188 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %191 @__builtin_ceilf(%190 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %193 @__builtin_ceill(%192 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %195 @__builtin_ceilf128(%194 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %197 @__builtin_cos(%196 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %199 @__builtin_cosf(%198 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %201 @__builtin_cosl(%200 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %203 @__builtin_cosf128(%202 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %205 @__builtin_cosh(%204 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %207 @__builtin_coshf(%206 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %209 @__builtin_coshl(%208 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %211 @__builtin_coshf128(%210 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %213 @__builtin_erf(%212 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %215 @__builtin_erff(%214 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %217 @__builtin_erfl(%216 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %219 @__builtin_erff128(%218 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %221 @__builtin_erfc(%220 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %223 @__builtin_erfcf(%222 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %225 @__builtin_erfcl(%224 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %227 @__builtin_erfcf128(%226 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %229 @__builtin_exp(%228 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %231 @__builtin_expf(%230 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %233 @__builtin_expl(%232 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %235 @__builtin_expf128(%234 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %237 @__builtin_exp2(%236 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %239 @__builtin_exp2f(%238 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %241 @__builtin_exp2l(%240 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %243 @__builtin_exp2f128(%242 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %245 @__builtin_exp10(%244 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %247 @__builtin_exp10f(%246 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %249 @__builtin_exp10l(%248 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %251 @__builtin_exp10f128(%250 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %253 @__builtin_expm1(%252 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %255 @__builtin_expm1f(%254 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %257 @__builtin_expm1l(%256 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %259 @__builtin_expm1f128(%258 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %262 @__builtin_fdim(%260 <unnamed>: f64, %261 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %265 @__builtin_fdimf(%263 <unnamed>: f32, %264 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %268 @__builtin_fdiml(%266 <unnamed>: f64, %267 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %271 @__builtin_fdimf128(%269 <unnamed>: f128, %270 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %273 @__builtin_floor(%272 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %275 @__builtin_floorf(%274 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %277 @__builtin_floorl(%276 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %279 @__builtin_floorf128(%278 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %283 @__builtin_fma(%280 <unnamed>: f64, %281 <unnamed>: f64, %282 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %287 @__builtin_fmaf(%284 <unnamed>: f32, %285 <unnamed>: f32, %286 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %291 @__builtin_fmal(%288 <unnamed>: f64, %289 <unnamed>: f64, %290 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %295 @__builtin_fmaf128(%292 <unnamed>: f128, %293 <unnamed>: f128, %294 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %299 @__builtin_fmaf16(%296 <unnamed>: f16, %297 <unnamed>: f16, %298 <unnamed>: f16) -> f16 [linkage=external];
// DEFAULT-NEXT:     fn %302 @__builtin_fmax(%300 <unnamed>: f64, %301 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %305 @__builtin_fmaxf(%303 <unnamed>: f32, %304 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %308 @__builtin_fmaxl(%306 <unnamed>: f64, %307 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %311 @__builtin_fmaxf128(%309 <unnamed>: f128, %310 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %314 @__builtin_fmin(%312 <unnamed>: f64, %313 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %317 @__builtin_fminf(%315 <unnamed>: f32, %316 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %320 @__builtin_fminl(%318 <unnamed>: f64, %319 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %323 @__builtin_fminf128(%321 <unnamed>: f128, %322 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %326 @__builtin_hypot(%324 <unnamed>: f64, %325 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %329 @__builtin_hypotf(%327 <unnamed>: f32, %328 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %332 @__builtin_hypotl(%330 <unnamed>: f64, %331 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %335 @__builtin_hypotf128(%333 <unnamed>: f128, %334 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %337 @__builtin_ilogb(%336 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %339 @__builtin_ilogbf(%338 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %341 @__builtin_ilogbl(%340 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %343 @__builtin_ilogbf128(%342 <unnamed>: f128) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %345 @__builtin_lgamma(%344 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %347 @__builtin_lgammaf(%346 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %349 @__builtin_lgammal(%348 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %351 @__builtin_lgammaf128(%350 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %353 @__builtin_llrint(%352 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %355 @__builtin_llrintf(%354 <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %357 @__builtin_llrintl(%356 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %359 @__builtin_llrintf128(%358 <unnamed>: f128) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %361 @__builtin_llround(%360 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %363 @__builtin_llroundf(%362 <unnamed>: f32) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %365 @__builtin_llroundl(%364 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %367 @__builtin_llroundf128(%366 <unnamed>: f128) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %369 @__builtin_log(%368 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %371 @__builtin_logf(%370 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %373 @__builtin_logl(%372 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %375 @__builtin_logf128(%374 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %377 @__builtin_log10(%376 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %379 @__builtin_log10f(%378 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %381 @__builtin_log10l(%380 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %383 @__builtin_log10f128(%382 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %385 @__builtin_log1p(%384 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %387 @__builtin_log1pf(%386 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %389 @__builtin_log1pl(%388 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %391 @__builtin_log1pf128(%390 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %393 @__builtin_log2(%392 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %395 @__builtin_log2f(%394 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %397 @__builtin_log2l(%396 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %399 @__builtin_log2f128(%398 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %401 @__builtin_logb(%400 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %403 @__builtin_logbf(%402 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %405 @__builtin_logbl(%404 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %407 @__builtin_logbf128(%406 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %409 @__builtin_lrint(%408 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %411 @__builtin_lrintf(%410 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %413 @__builtin_lrintl(%412 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %415 @__builtin_lrintf128(%414 <unnamed>: f128) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %417 @__builtin_lround(%416 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %419 @__builtin_lroundf(%418 <unnamed>: f32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %421 @__builtin_lroundl(%420 <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %423 @__builtin_lroundf128(%422 <unnamed>: f128) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %425 @__builtin_nearbyint(%424 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %427 @__builtin_nearbyintf(%426 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %429 @__builtin_nearbyintl(%428 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %431 @__builtin_nearbyintf128(%430 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %434 @__builtin_nextafter(%432 <unnamed>: f64, %433 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %437 @__builtin_nextafterf(%435 <unnamed>: f32, %436 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %440 @__builtin_nextafterl(%438 <unnamed>: f64, %439 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %443 @__builtin_nextafterf128(%441 <unnamed>: f128, %442 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %446 @__builtin_nexttoward(%444 <unnamed>: f64, %445 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %449 @__builtin_nexttowardf(%447 <unnamed>: f32, %448 <unnamed>: f64) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %452 @__builtin_nexttowardl(%450 <unnamed>: f64, %451 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %455 @__builtin_nexttowardf128(%453 <unnamed>: f128, %454 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %458 @__builtin_remainder(%456 <unnamed>: f64, %457 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %461 @__builtin_remainderf(%459 <unnamed>: f32, %460 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %464 @__builtin_remainderl(%462 <unnamed>: f64, %463 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %467 @__builtin_remainderf128(%465 <unnamed>: f128, %466 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %471 @__builtin_remquo(%468 <unnamed>: f64, %469 <unnamed>: f64, %470 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %475 @__builtin_remquof(%472 <unnamed>: f32, %473 <unnamed>: f32, %474 <unnamed>: ptr<i32>) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %479 @__builtin_remquol(%476 <unnamed>: f64, %477 <unnamed>: f64, %478 <unnamed>: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %483 @__builtin_remquof128(%480 <unnamed>: f128, %481 <unnamed>: f128, %482 <unnamed>: ptr<i32>) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %485 @__builtin_rint(%484 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %487 @__builtin_rintf(%486 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %489 @__builtin_rintl(%488 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %491 @__builtin_rintf128(%490 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %493 @__builtin_round(%492 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %495 @__builtin_roundf(%494 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %497 @__builtin_roundl(%496 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %499 @__builtin_roundf128(%498 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %502 @__builtin_scalbln(%500 <unnamed>: f64, %501 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %505 @__builtin_scalblnf(%503 <unnamed>: f32, %504 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %508 @__builtin_scalblnl(%506 <unnamed>: f64, %507 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %511 @__builtin_scalblnf128(%509 <unnamed>: f128, %510 <unnamed>: i32) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %514 @__builtin_scalbn(%512 <unnamed>: f64, %513 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %517 @__builtin_scalbnf(%515 <unnamed>: f32, %516 <unnamed>: i32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %520 @__builtin_scalbnl(%518 <unnamed>: f64, %519 <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %523 @__builtin_scalbnf128(%521 <unnamed>: f128, %522 <unnamed>: i32) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %525 @__builtin_sin(%524 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %527 @__builtin_sinf(%526 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %529 @__builtin_sinl(%528 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %531 @__builtin_sinf128(%530 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %533 @__builtin_sinh(%532 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %535 @__builtin_sinhf(%534 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %537 @__builtin_sinhl(%536 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %539 @__builtin_sinhf128(%538 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %543 @__builtin_sincos(%540 <unnamed>: f64, %541 <unnamed>: ptr<f64>, %542 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %547 @__builtin_sincosf(%544 <unnamed>: f32, %545 <unnamed>: ptr<f32>, %546 <unnamed>: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %551 @__builtin_sincosl(%548 <unnamed>: f64, %549 <unnamed>: ptr<f64>, %550 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %555 @__builtin_sincosf128(%552 <unnamed>: f128, %553 <unnamed>: ptr<f128>, %554 <unnamed>: ptr<f128>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %559 @__builtin_sincospi(%556 <unnamed>: f64, %557 <unnamed>: ptr<f64>, %558 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %563 @__builtin_sincospif(%560 <unnamed>: f32, %561 <unnamed>: ptr<f32>, %562 <unnamed>: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %567 @__builtin_sincospil(%564 <unnamed>: f64, %565 <unnamed>: ptr<f64>, %566 <unnamed>: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %569 @__builtin_sqrt(%568 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %571 @__builtin_sqrtf(%570 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %573 @__builtin_sqrtl(%572 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %575 @__builtin_sqrtf128(%574 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %577 @__builtin_tan(%576 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %579 @__builtin_tanf(%578 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %581 @__builtin_tanl(%580 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %583 @__builtin_tanf128(%582 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %585 @__builtin_tanh(%584 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %587 @__builtin_tanhf(%586 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %589 @__builtin_tanhl(%588 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %591 @__builtin_tanhf128(%590 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %593 @__builtin_tgamma(%592 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %595 @__builtin_tgammaf(%594 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %597 @__builtin_tgammal(%596 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %599 @__builtin_tgammaf128(%598 <unnamed>: f128) -> f128 [linkage=external];
// DEFAULT-NEXT:     fn %601 @__builtin_trunc(%600 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %603 @__builtin_truncf(%602 <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %605 @__builtin_truncl(%604 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %607 @__builtin_truncf128(%606 <unnamed>: f128) -> f128 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %0 @foo(%1 d: ptr<f64>, %2 f: f32, %3 fp: ptr<f32>, %4 l: ptr<f64>, %5 i: ptr<i32>, %6 c: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32>(%2, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%9, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)))));
// DEFAULT-NEXT:         float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%9, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2))));
// DEFAULT-NEXT:         write<f32>(%2, call<f32, signature=fn(f32, f32) -> f32>(%12, read<f32>(%2), read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%12, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         write<f32>(%2, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%15, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)))));
// DEFAULT-NEXT:         float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%15, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2))));
// DEFAULT-NEXT:         write<f32>(%2, float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f128, signature=fn(f128, f128) -> f128>(%18, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)))));
// DEFAULT-NEXT:         float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f128, signature=fn(f128, f128) -> f128>(%18, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%21, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%24, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%27, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%30, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%33, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%36, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%39, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%42, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%44, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%46, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%48, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%50, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%53, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, ptr<i32>) -> f32>(%56, read<f32>(%2), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<i32>) -> f64>(%59, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, ptr<i32>) -> f128>(%62, float_widen<f128, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%63);
// DEFAULT-NEXT:         call<f32, signature=fn() -> f32>(%64);
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%65);
// DEFAULT-NEXT:         call<f128, signature=fn() -> f128>(%66);
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%67);
// DEFAULT-NEXT:         call<f32, signature=fn() -> f32>(%68);
// DEFAULT-NEXT:         call<f64, signature=fn() -> f64>(%69);
// DEFAULT-NEXT:         call<f128, signature=fn() -> f128>(%70);
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%73, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%76, read<f32>(%2), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%79, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i32) -> f128>(%82, float_widen<f128, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%85, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%1));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, ptr<f32>) -> f32>(%88, read<f32>(%2), read<ptr<f32>>(%3));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, ptr<f64>) -> f64>(%91, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%4));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, ptr<f128>) -> f128>(%94, float_widen<f128, reason=arg>(read<f32>(%2)), pointer_cast<ptr<f128>, reason=arg>(read<ptr<f64>>(%4)));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%96, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<const i8>) -> f32>(%98, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%100, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f128, signature=fn(ptr<const i8>) -> f128>(%102, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%104, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f32, signature=fn(ptr<const i8>) -> f32>(%106, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f64, signature=fn(ptr<const i8>) -> f64>(%108, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f128, signature=fn(ptr<const i8>) -> f128>(%110, read<ptr<const i8>>(%6));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%113, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%116, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%119, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%122, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%125, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%128, read<f32>(%2), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%131, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%133, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%135, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%137, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%139, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%141, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%143, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%145, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%147, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%149, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%151, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%153, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%155, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%157, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%159, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%161, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%163, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%165, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%167, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%169, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%171, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%173, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%175, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%177, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%179, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%181, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%183, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%185, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%187, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%189, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%191, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%193, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%195, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%197, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%199, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%201, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%203, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%205, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%207, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%209, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%211, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%213, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%215, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%217, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%219, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%221, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%223, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%225, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%227, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%229, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%231, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%233, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%235, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%237, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%239, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%241, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%243, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%245, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%247, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%249, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%251, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%253, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%255, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%257, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%259, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%262, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%265, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%268, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%271, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%273, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%275, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%277, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%279, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%283, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32, f32) -> f32>(%287, read<f32>(%2), read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, f64) -> f64>(%291, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128, f128) -> f128>(%295, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f16, signature=fn(f16, f16, f16) -> f16>(%299, float_narrow<f16, reason=arg, rounding=nearest_even, exceptions=ignore>(read<f32>(%2)), float_narrow<f16, reason=arg, rounding=nearest_even, exceptions=ignore>(read<f32>(%2)), float_narrow<f16, reason=arg, rounding=nearest_even, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%302, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%305, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%308, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%311, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%314, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%317, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%320, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%323, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%326, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%329, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%332, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%335, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%337, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%339, read<f32>(%2));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%341, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f128) -> i32>(%343, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%345, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%347, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%349, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%351, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%353, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f32) -> i64>(%355, read<f32>(%2));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%357, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(%359, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%361, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f32) -> i64>(%363, read<f32>(%2));
// DEFAULT-NEXT:         call<i64, signature=fn(f64) -> i64>(%365, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i64, signature=fn(f128) -> i64>(%367, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%369, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%371, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%373, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%375, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%377, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%379, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%381, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%383, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%385, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%387, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%389, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%391, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%393, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%395, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%397, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%399, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%401, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%403, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%405, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%407, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%409, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%411, read<f32>(%2));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%413, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f128) -> i32>(%415, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%417, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f32) -> i32>(%419, read<f32>(%2));
// DEFAULT-NEXT:         call<i32, signature=fn(f64) -> i32>(%421, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(f128) -> i32>(%423, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%425, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%427, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%429, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%431, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%434, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%437, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%440, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%443, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%446, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f64) -> f32>(%449, read<f32>(%2), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%452, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%455, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%458, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%461, read<f32>(%2), read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%464, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128) -> f128>(%467, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%471, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32, ptr<i32>) -> f32>(%475, read<f32>(%2), read<f32>(%2), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64, ptr<i32>) -> f64>(%479, float_widen<f64, reason=arg>(read<f32>(%2)), float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, f128, ptr<i32>) -> f128>(%483, float_widen<f128, reason=arg>(read<f32>(%2)), float_widen<f128, reason=arg>(read<f32>(%2)), read<ptr<i32>>(%5));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%485, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%487, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%489, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%491, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%493, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%495, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%497, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%499, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%502, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%505, read<f32>(%2), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%508, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i32) -> f128>(%511, float_widen<f128, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%514, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, i32) -> f32>(%517, read<f32>(%2), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, i32) -> f64>(%520, float_widen<f64, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128, i32) -> f128>(%523, float_widen<f128, reason=arg>(read<f32>(%2)), float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%525, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%527, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%529, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%531, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%533, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%535, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%537, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%539, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%543, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%1), read<ptr<f64>>(%1));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%547, read<f32>(%2), read<ptr<f32>>(%3), read<ptr<f32>>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%551, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%4), read<ptr<f64>>(%4));
// DEFAULT-NEXT:         call<void, signature=fn(f128, ptr<f128>, ptr<f128>) -> void>(%555, float_widen<f128, reason=arg>(read<f32>(%2)), pointer_cast<ptr<f128>, reason=arg>(read<ptr<f64>>(%4)), pointer_cast<ptr<f128>, reason=arg>(read<ptr<f64>>(%4)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%559, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%1), read<ptr<f64>>(%1));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%563, read<f32>(%2), read<ptr<f32>>(%3), read<ptr<f32>>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%567, float_widen<f64, reason=arg>(read<f32>(%2)), read<ptr<f64>>(%4), read<ptr<f64>>(%4));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%569, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%571, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%573, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%575, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%577, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%579, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%581, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%583, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%585, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%587, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%589, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%591, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%593, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%595, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%597, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%599, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%601, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32) -> f32>(%603, read<f32>(%2));
// DEFAULT-NEXT:         call<f64, signature=fn(f64) -> f64>(%605, float_widen<f64, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:         call<f128, signature=fn(f128) -> f128>(%607, float_widen<f128, reason=arg>(read<f32>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
