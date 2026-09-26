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
// DEFAULT-NEXT:     global %100 .str100: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @feenableexcept(%53 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @fedisableexcept(%54 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @fegetexcept() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @cos(%55 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @sin(%56 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @sincos(%57 __x: f64, %58 __sinx: ptr<f64>, %59 __cosx: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @exp10(%60 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @fabs(%61 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %8 @drem(%62 __x: f64, %63 __y: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %9 @significand(%64 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %10 @j0(%65 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %11 @j1(%66 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %12 @jn(%67 <unnamed>: i32, %68 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %13 @y0(%69 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %14 @y1(%70 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %15 @yn(%71 <unnamed>: i32, %72 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %16 @lgamma(%73 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %17 @lgamma_r(%74 <unnamed>: f64, %75 __signgamp: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %18 @scalb(%76 __x: f64, %77 __n: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %19 @cosf(%78 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %20 @sinf(%79 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @sincosf(%80 __x: f32, %81 __sinx: ptr<f32>, %82 __cosx: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %22 @exp10f(%83 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %23 @fabsf(%84 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %24 @dremf(%85 __x: f32, %86 __y: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %25 @significandf(%87 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @j0f(%88 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %27 @y0f(%89 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %28 @scalbf(%90 __x: f32, %91 __n: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %29 @cosl(%92 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %30 @sinl(%93 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %31 @sincosl(%94 __x: f80, %95 __sinx: ptr<f80>, %96 __cosx: ptr<f80>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %32 @exp10l(%97 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %33 @fabsl(%98 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %34 @printf(%99 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %35 @gnu_sincos_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %36 sine: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %37 cosine: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %38 sine_f: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:         let %39 cosine_f: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:         let %40 sine_l: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %41 cosine_l: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %42 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%5, const<f64>(0.5), addr_of<ptr<f64>>(%36), addr_of<ptr<f64>>(%37));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%21, const<f32>(0.5), addr_of<ptr<f32>>(%38), addr_of<ptr<f32>>(%39));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<f80>, ptr<f80>) -> void>(%31, const<f80>(0.5), addr_of<ptr<f80>>(%40), addr_of<ptr<f80>>(%41));
// DEFAULT-NEXT:         let %101: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:         let %102: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%101), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%7, sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%36), call<f64, signature=fn(f64) -> f64>(%4, const<f64>(0.5)))), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%42, read<i32>(%102));
// DEFAULT-NEXT:         let %103: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:         let %104: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%103), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%7, sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%37), call<f64, signature=fn(f64) -> f64>(%3, const<f64>(0.5)))), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%42, read<i32>(%104));
// DEFAULT-NEXT:         let %105: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:         let %106: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%105), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%23, sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%38), call<f32, signature=fn(f32) -> f32>(%20, const<f32>(0.5)))), const<f32>(0.0))));
// DEFAULT-NEXT:         write<i32>(%42, read<i32>(%106));
// DEFAULT-NEXT:         let %107: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:         let %108: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%107), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%23, sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%39), call<f32, signature=fn(f32) -> f32>(%19, const<f32>(0.5)))), const<f32>(0.0))));
// DEFAULT-NEXT:         write<i32>(%42, read<i32>(%108));
// DEFAULT-NEXT:         let %109: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:         let %110: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%109), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%33, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%40), call<f80, signature=fn(f80) -> f80>(%30, const<f80>(0.5)))), const<f80>(0))));
// DEFAULT-NEXT:         write<i32>(%42, read<i32>(%110));
// DEFAULT-NEXT:         let %111: i32 [synthetic] = read<i32>(%42);
// DEFAULT-NEXT:         let %112: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%111), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%33, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%41), call<f80, signature=fn(f80) -> f80>(%29, const<f80>(0.5)))), const<f80>(0))));
// DEFAULT-NEXT:         write<i32>(%42, read<i32>(%112));
// DEFAULT-NEXT:         return read<i32>(%42);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @gnu_exponential_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %44 sign: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %45 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %113: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %114: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%113), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%6, const<f64>(2.0)), const<f64>(100.0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%114));
// DEFAULT-NEXT:         let %115: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %116: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%115), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%22, const<f32>(2.0)), const<f32>(100.0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%116));
// DEFAULT-NEXT:         let %117: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %118: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%117), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%32, const<f80>(2)), const<f80>(100))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%118));
// DEFAULT-NEXT:         let %119: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %120: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%119), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, ptr<i32>) -> f64>(%17, const<f64>(0.5), addr_of<ptr<i32>>(%44)), call<f64, signature=fn(f64) -> f64>(%16, const<f64>(0.5)))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%120));
// DEFAULT-NEXT:         let %121: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %122: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%121), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%44), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%122));
// DEFAULT-NEXT:         let %123: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %124: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%123), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%9, const<f64>(12.0)), const<f64>(1.5))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%124));
// DEFAULT-NEXT:         let %125: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %126: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%125), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%25, const<f32>(12.0)), const<f32>(1.5))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%126));
// DEFAULT-NEXT:         let %127: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %128: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%127), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%8, const<f64>(7.0), const<f64>(3.0)), const<f64>(1.0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%128));
// DEFAULT-NEXT:         let %129: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %130: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%129), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%24, const<f32>(7.0), const<f32>(3.0)), const<f32>(1.0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%130));
// DEFAULT-NEXT:         let %131: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %132: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%131), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%18, const<f64>(1.5), const<f64>(3.0)), const<f64>(12.0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%132));
// DEFAULT-NEXT:         let %133: i32 [synthetic] = read<i32>(%45);
// DEFAULT-NEXT:         let %134: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%133), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%28, const<f32>(1.5), const<f32>(3.0)), const<f32>(12.0))));
// DEFAULT-NEXT:         write<i32>(%45, read<i32>(%134));
// DEFAULT-NEXT:         return read<i32>(%45);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @gnu_bessel_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %47 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %135: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %136: bool [synthetic];
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%10, const<f64>(1.0)), call<f64, signature=fn(f64) -> f64>(%10, const<f64>(1.0)))
// DEFAULT-NEXT:             write<bool>(%136, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%10, const<f64>(1.0)), const<f64>(0.7)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%136, const<bool>(false));
// DEFAULT-NEXT:         let %137: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%135), from_bool<i32, reason=promotion>(read<bool>(%136)));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%137));
// DEFAULT-NEXT:         let %138: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %139: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%138), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%11, const<f64>(1.0)), const<f64>(0.4))));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%139));
// DEFAULT-NEXT:         let %140: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %141: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%140), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(i32, f64) -> f64>(%12, const<i32>(2), const<f64>(1.0)), const<f64>(0.1))));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%141));
// DEFAULT-NEXT:         let %142: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %143: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%142), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%13, const<f64>(1.0)), const<f64>(0.08))));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%143));
// DEFAULT-NEXT:         let %144: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %145: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%144), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%14, const<f64>(1.0)), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%145));
// DEFAULT-NEXT:         let %146: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %147: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%146), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(call<f64, signature=fn(i32, f64) -> f64>(%15, const<i32>(2), const<f64>(1.0)), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%147));
// DEFAULT-NEXT:         let %148: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %149: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%148), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%26, const<f32>(1.0)), const<f32>(0.7))));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%149));
// DEFAULT-NEXT:         let %150: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:         let %151: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%150), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%27, const<f32>(1.0)), const<f32>(0.08))));
// DEFAULT-NEXT:         write<i32>(%47, read<i32>(%151));
// DEFAULT-NEXT:         return read<i32>(%47);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %48 @gnu_fenv_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %49 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %152: i32 [synthetic] = read<i32>(%49);
// DEFAULT-NEXT:         let %153: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%152), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(4)), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%49, read<i32>(%153));
// DEFAULT-NEXT:         let %154: i32 [synthetic] = read<i32>(%49);
// DEFAULT-NEXT:         let %155: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%154), from_bool<i32, reason=promotion>(ne<i32>(and<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(4)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%49, read<i32>(%155));
// DEFAULT-NEXT:         let %156: i32 [synthetic] = read<i32>(%49);
// DEFAULT-NEXT:         let %157: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%156), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(4)), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%49, read<i32>(%157));
// DEFAULT-NEXT:         let %158: i32 [synthetic] = read<i32>(%49);
// DEFAULT-NEXT:         let %159: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%158), from_bool<i32, reason=promotion>(eq<i32>(and<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(4)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%49, read<i32>(%159));
// DEFAULT-NEXT:         return read<i32>(%49);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %50 @gnu_constant_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %51 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %160: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %161: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%160), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(3.14159265358979323851)), const<f64>(3.141592653589793))));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%161));
// DEFAULT-NEXT:         let %162: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %163: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%162), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(2.71828182845904523543)), const<f64>(2.718281828459045))));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%163));
// DEFAULT-NEXT:         let %164: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %165: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%164), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.41421356237309504876)), const<f64>(1.4142135623730951))));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%165));
// DEFAULT-NEXT:         let %166: i32 [synthetic] = read<i32>(%51);
// DEFAULT-NEXT:         let %167: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%166), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(0.693147180559945309429)), const<f64>(0.6931471805599453))));
// DEFAULT-NEXT:         write<i32>(%51, read<i32>(%167));
// DEFAULT-NEXT:         return read<i32>(%51);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %52 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%34, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%100)), call<i32, signature=fn() -> i32>(%35), call<i32, signature=fn() -> i32>(%43), call<i32, signature=fn() -> i32>(%46), call<i32, signature=fn() -> i32>(%48), call<i32, signature=fn() -> i32>(%50));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
