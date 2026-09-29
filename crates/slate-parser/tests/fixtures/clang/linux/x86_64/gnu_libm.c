#define _GNU_SOURCE
#include <complex.h>
#include <fenv.h>
#include <math.h>
#include <stdio.h>

static int gnu_sincos_extensions(void) {
  double      sine     = 0.0;
  double      cosine   = 0.0;
  float       sine_f   = 0.0f;
  float       cosine_f = 0.0f;
  long double sine_l   = 0.0L;
  long double cosine_l = 0.0L;
  int         total    = 0;

  sincos(0.5, &sine, &cosine);
  sincosf(0.5f, &sine_f, &cosine_f);
  sincosl(0.5L, &sine_l, &cosine_l);

  total += fabs(sine - sin(0.5)) == 0.0;
  total += fabs(cosine - cos(0.5)) == 0.0;
  total += fabsf(sine_f - sinf(0.5f)) == 0.0f;
  total += fabsf(cosine_f - cosf(0.5f)) == 0.0f;
  total += fabsl(sine_l - sinl(0.5L)) == 0.0L;
  total += fabsl(cosine_l - cosl(0.5L)) == 0.0L;
  return total;
}

static int gnu_exponential_extensions(void) {
  int sign  = 0;
  int total = 0;

  total += exp10(2.0) == 100.0;
  total += exp10f(2.0f) == 100.0f;
  total += exp10l(2.0L) == 100.0L;
  total += lgamma_r(0.5, &sign) == lgamma(0.5);
  total += sign == 1;
  total += significand(12.0) == 1.5;
  total += significandf(12.0f) == 1.5f;
  total += drem(7.0, 3.0) == 1.0;
  total += dremf(7.0f, 3.0f) == 1.0f;
  total += scalb(1.5, 3.0) == 12.0;
  total += scalbf(1.5f, 3.0f) == 12.0f;
  return total;
}

static int gnu_bessel_extensions(void) {
  int total = 0;

  total += j0(1.0) == j0(1.0) && j0(1.0) > 0.7;
  total += j1(1.0) > 0.4;
  total += jn(2, 1.0) > 0.1;
  total += y0(1.0) > 0.08;
  total += y1(1.0) < 0.0;
  total += yn(2, 1.0) < 0.0;
  total += j0f(1.0f) > 0.7f;
  total += y0f(1.0f) > 0.08f;
  return total;
}

static int gnu_fenv_extensions(void) {
  int total = 0;

  total += feenableexcept(FE_DIVBYZERO) != -1;
  total += (fegetexcept() & FE_DIVBYZERO) != 0;
  total += fedisableexcept(FE_DIVBYZERO) != -1;
  total += (fegetexcept() & FE_DIVBYZERO) == 0;
  return total;
}

static int gnu_constant_extensions(void) {
  int total = 0;

  total += (double)M_PIl == M_PI;
  total += (double)M_El == M_E;
  total += (double)M_SQRT2l == M_SQRT2;
  total += (double)M_LN2l == M_LN2;
  return total;
}

int main(void) {
  printf("%d %d %d %d %d\n", gnu_sincos_extensions(),
         gnu_exponential_extensions(), gnu_bessel_extensions(),
         gnu_fenv_extensions(), gnu_constant_extensions());
  return 0;
}




// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_feenableexcept:[0-9]+]] @feenableexcept(%[[VALUE___excepts:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fedisableexcept:[0-9]+]] @fedisableexcept(%[[VALUE___excepts_2:[0-9]+]] __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fegetexcept:[0-9]+]] @fegetexcept() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cos:[0-9]+]] @cos(%[[VALUE___x:[0-9]+]] __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sin:[0-9]+]] @sin(%[[VALUE___x_2:[0-9]+]] __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sincos:[0-9]+]] @sincos(%[[VALUE___x_3:[0-9]+]] __x: f64, %[[VALUE___sinx:[0-9]+]] __sinx: ptr<f64>, %[[VALUE___cosx:[0-9]+]] __cosx: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10:[0-9]+]] @exp10(%[[VALUE___x_4:[0-9]+]] __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabs:[0-9]+]] @fabs(%[[VALUE___x_5:[0-9]+]] __x: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_drem:[0-9]+]] @drem(%[[VALUE___x_6:[0-9]+]] __x: f64, %[[VALUE___y:[0-9]+]] __y: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_significand:[0-9]+]] @significand(%[[VALUE___x_7:[0-9]+]] __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_j0:[0-9]+]] @j0(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_j1:[0-9]+]] @j1(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_jn:[0-9]+]] @jn(%[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_y0:[0-9]+]] @y0(%[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_y1:[0-9]+]] @y1(%[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_yn:[0-9]+]] @yn(%[[VALUE6:[0-9]+]] <unnamed>: i32, %[[VALUE7:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lgamma:[0-9]+]] @lgamma(%[[VALUE8:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_lgamma_r:[0-9]+]] @lgamma_r(%[[VALUE9:[0-9]+]] <unnamed>: f64, %[[VALUE___signgamp:[0-9]+]] __signgamp: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalb:[0-9]+]] @scalb(%[[VALUE___x_8:[0-9]+]] __x: f64, %[[VALUE___n:[0-9]+]] __n: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosf:[0-9]+]] @cosf(%[[VALUE___x_9:[0-9]+]] __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinf:[0-9]+]] @sinf(%[[VALUE___x_10:[0-9]+]] __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sincosf:[0-9]+]] @sincosf(%[[VALUE___x_11:[0-9]+]] __x: f32, %[[VALUE___sinx_2:[0-9]+]] __sinx: ptr<f32>, %[[VALUE___cosx_2:[0-9]+]] __cosx: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10f:[0-9]+]] @exp10f(%[[VALUE___x_12:[0-9]+]] __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabsf:[0-9]+]] @fabsf(%[[VALUE___x_13:[0-9]+]] __x: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_dremf:[0-9]+]] @dremf(%[[VALUE___x_14:[0-9]+]] __x: f32, %[[VALUE___y_2:[0-9]+]] __y: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_significandf:[0-9]+]] @significandf(%[[VALUE___x_15:[0-9]+]] __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_j0f:[0-9]+]] @j0f(%[[VALUE10:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_y0f:[0-9]+]] @y0f(%[[VALUE11:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_scalbf:[0-9]+]] @scalbf(%[[VALUE___x_16:[0-9]+]] __x: f32, %[[VALUE___n_2:[0-9]+]] __n: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cosl:[0-9]+]] @cosl(%[[VALUE___x_17:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinl:[0-9]+]] @sinl(%[[VALUE___x_18:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sincosl:[0-9]+]] @sincosl(%[[VALUE___x_19:[0-9]+]] __x: f80, %[[VALUE___sinx_3:[0-9]+]] __sinx: ptr<f80>, %[[VALUE___cosx_3:[0-9]+]] __cosx: ptr<f80>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_exp10l:[0-9]+]] @exp10l(%[[VALUE___x_20:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fabsl:[0-9]+]] @fabsl(%[[VALUE___x_21:[0-9]+]] __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_gnu_sincos_extensions:[0-9]+]] @gnu_sincos_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_sine:[0-9]+]] sine: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_cosine:[0-9]+]] cosine: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_sine_f:[0-9]+]] sine_f: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_cosine_f:[0-9]+]] cosine_f: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:         let %[[VALUE_sine_l:[0-9]+]] sine_l: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %[[VALUE_cosine_l:[0-9]+]] cosine_l: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE_sincos]], const<f64>(0.5), addr_of<ptr<f64>>(%[[VALUE_sine]]), addr_of<ptr<f64>>(%[[VALUE_cosine]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%[[VALUE_sincosf]], const<f32>(0.5), addr_of<ptr<f32>>(%[[VALUE_sine_f]]), addr_of<ptr<f32>>(%[[VALUE_cosine_f]]));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<f80>, ptr<f80>) -> void>(%[[VALUE_sincosl]], const<f80>(0.5), addr_of<ptr<f80>>(%[[VALUE_sine_l]]), addr_of<ptr<f80>>(%[[VALUE_cosine_l]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_sine]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], const<f64>(0.5)))), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_fabs]], sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_cosine]]), call<f64, signature=fn(f64) -> f64>(%[[VALUE_cos]], const<f64>(0.5)))), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE_sine_f]]), call<f32, signature=fn(f32) -> f32>(%[[VALUE_sinf]], const<f32>(0.5)))), const<f32>(0.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_fabsf]], sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE_cosine_f]]), call<f32, signature=fn(f32) -> f32>(%[[VALUE_cosf]], const<f32>(0.5)))), const<f32>(0.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_sine_l]]), call<f80, signature=fn(f80) -> f80>(%[[VALUE_sinl]], const<f80>(0.5)))), const<f80>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_fabsl]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_cosine_l]]), call<f80, signature=fn(f80) -> f80>(%[[VALUE_cosl]], const<f80>(0.5)))), const<f80>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_exponential_extensions:[0-9]+]] @gnu_exponential_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_sign:[0-9]+]] sign: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_total_2:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp10]], const<f64>(2.0)), const<f64>(100.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_exp10f]], const<f32>(2.0)), const<f32>(100.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%[[VALUE_exp10l]], const<f80>(2)), const<f80>(100))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, ptr<i32>) -> f64>(%[[VALUE_lgamma_r]], const<f64>(0.5), addr_of<ptr<i32>>(%[[VALUE_sign]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE_lgamma]], const<f64>(0.5)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%[[VALUE_sign]]), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE34]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_significand]], const<f64>(12.0)), const<f64>(1.5))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_significandf]], const<f32>(12.0)), const<f32>(1.5))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE38]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_drem]], const<f64>(7.0), const<f64>(3.0)), const<f64>(1.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE39]]));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_dremf]], const<f32>(7.0), const<f32>(3.0)), const<f32>(1.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE42]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_scalb]], const<f64>(1.5), const<f64>(3.0)), const<f64>(12.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE43]]));
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE44]]), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_scalbf]], const<f32>(1.5), const<f32>(3.0)), const<f32>(12.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_2]], read<i32>(%[[VALUE45]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_bessel_extensions:[0-9]+]] @gnu_bessel_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_total_3:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_j0]], const<f64>(1.0)), call<f64, signature=fn(f64) -> f64>(%[[VALUE_j0]], const<f64>(1.0)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE47]], gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_j0]], const<f64>(1.0)), const<f64>(0.7)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE47]], const<bool>(false));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE47]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE48]]));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE49]]), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_j1]], const<f64>(1.0)), const<f64>(0.4))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE51]]), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(i32, f64) -> f64>(%[[VALUE_jn]], const<i32>(2), const<f64>(1.0)), const<f64>(0.1))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE52]]));
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE53]]), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_y0]], const<f64>(1.0)), const<f64>(0.08))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE54]]));
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE55]]), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_y1]], const<f64>(1.0)), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE56]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE57]]), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(call<f64, signature=fn(i32, f64) -> f64>(%[[VALUE_yn]], const<i32>(2), const<f64>(1.0)), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE58]]));
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_j0f]], const<f32>(1.0)), const<f32>(0.7))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE60]]));
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE61]]), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%[[VALUE_y0f]], const<f32>(1.0)), const<f32>(0.08))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_3]], read<i32>(%[[VALUE62]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_fenv_extensions:[0-9]+]] @gnu_fenv_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_total_4:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE63]]), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_feenableexcept]], const<i32>(4)), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE64]]));
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE65]]), from_bool<i32, reason=promotion>(ne<i32>(and<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_fegetexcept]]), const<i32>(4)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE66]]));
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE67]]), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_fedisableexcept]], const<i32>(4)), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE68]]));
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE69]]), from_bool<i32, reason=promotion>(eq<i32>(and<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_fegetexcept]]), const<i32>(4)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_4]], read<i32>(%[[VALUE70]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gnu_constant_extensions:[0-9]+]] @gnu_constant_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_total_5:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_5]]);
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE71]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(3.14159265358979323851)), const<f64>(3.141592653589793))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_5]], read<i32>(%[[VALUE72]]));
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_5]]);
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE73]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(2.71828182845904523543)), const<f64>(2.718281828459045))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_5]], read<i32>(%[[VALUE74]]));
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_5]]);
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE75]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.41421356237309504876)), const<f64>(1.4142135623730951))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_5]], read<i32>(%[[VALUE76]]));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total_5]]);
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE77]]), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(0.693147180559945309429)), const<f64>(0.6931471805599453))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_total_5]], read<i32>(%[[VALUE78]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str]])), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_sincos_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_exponential_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_bessel_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_fenv_extensions]]), call<i32, signature=fn() -> i32>(%[[VALUE_gnu_constant_extensions]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
