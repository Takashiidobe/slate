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
// DEFAULT-NEXT:     global %135 .str135: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @feenableexcept(%88 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @fedisableexcept(%89 __excepts: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @fegetexcept() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @cos(%90 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %8 @sin(%91 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %12 @sincos(%92 __x: f64, %93 __sinx: ptr<f64>, %94 __cosx: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %14 @exp10(%95 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %16 @fabs(%96 __x: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %19 @drem(%97 __x: f64, %98 __y: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %21 @significand(%99 __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %22 @j0(%100 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %23 @j1(%101 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %24 @jn(%102 <unnamed>: i32, %103 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %25 @y0(%104 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %26 @y1(%105 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %27 @yn(%106 <unnamed>: i32, %107 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %28 @lgamma(%108 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %30 @lgamma_r(%109 <unnamed>: f64, %110 __signgamp: ptr<i32>) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %33 @scalb(%111 __x: f64, %112 __n: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %35 @cosf(%113 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %37 @sinf(%114 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %41 @sincosf(%115 __x: f32, %116 __sinx: ptr<f32>, %117 __cosx: ptr<f32>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %43 @exp10f(%118 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %45 @fabsf(%119 __x: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %48 @dremf(%120 __x: f32, %121 __y: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %50 @significandf(%122 __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %51 @j0f(%123 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %52 @y0f(%124 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %55 @scalbf(%125 __x: f32, %126 __n: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %57 @cosl(%127 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %59 @sinl(%128 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %63 @sincosl(%129 __x: f80, %130 __sinx: ptr<f80>, %131 __cosx: ptr<f80>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %65 @exp10l(%132 __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %67 @fabsl(%133 __x: f80) -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %69 @printf(%134 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %70 @gnu_sincos_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %71 sine: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %72 cosine: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:         let %73 sine_f: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:         let %74 cosine_f: f32 [storage=automatic] = const<f32>(0.0);
// DEFAULT-NEXT:         let %75 sine_l: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %76 cosine_l: f80 [storage=automatic] = const<f80>(0);
// DEFAULT-NEXT:         let %77 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(%12, const<f64>(0.5), addr_of<ptr<f64>>(%71), addr_of<ptr<f64>>(%72));
// DEFAULT-NEXT:         call<void, signature=fn(f32, ptr<f32>, ptr<f32>) -> void>(%41, const<f32>(0.5), addr_of<ptr<f32>>(%73), addr_of<ptr<f32>>(%74));
// DEFAULT-NEXT:         call<void, signature=fn(f80, ptr<f80>, ptr<f80>) -> void>(%63, const<f80>(0.5), addr_of<ptr<f80>>(%75), addr_of<ptr<f80>>(%76));
// DEFAULT-NEXT:         let %136: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:         let %137: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%136), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%16, sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%71), call<f64, signature=fn(f64) -> f64>(%8, const<f64>(0.5)))), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%77, read<i32>(%137));
// DEFAULT-NEXT:         let %138: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:         let %139: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%138), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%16, sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%72), call<f64, signature=fn(f64) -> f64>(%6, const<f64>(0.5)))), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%77, read<i32>(%139));
// DEFAULT-NEXT:         let %140: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:         let %141: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%140), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%45, sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%73), call<f32, signature=fn(f32) -> f32>(%37, const<f32>(0.5)))), const<f32>(0.0))));
// DEFAULT-NEXT:         write<i32>(%77, read<i32>(%141));
// DEFAULT-NEXT:         let %142: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:         let %143: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%142), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%45, sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%74), call<f32, signature=fn(f32) -> f32>(%35, const<f32>(0.5)))), const<f32>(0.0))));
// DEFAULT-NEXT:         write<i32>(%77, read<i32>(%143));
// DEFAULT-NEXT:         let %144: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:         let %145: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%144), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%67, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%75), call<f80, signature=fn(f80) -> f80>(%59, const<f80>(0.5)))), const<f80>(0))));
// DEFAULT-NEXT:         write<i32>(%77, read<i32>(%145));
// DEFAULT-NEXT:         let %146: i32 [synthetic] = read<i32>(%77);
// DEFAULT-NEXT:         let %147: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%146), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%67, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%76), call<f80, signature=fn(f80) -> f80>(%57, const<f80>(0.5)))), const<f80>(0))));
// DEFAULT-NEXT:         write<i32>(%77, read<i32>(%147));
// DEFAULT-NEXT:         return read<i32>(%77);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %78 @gnu_exponential_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %79 sign: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %80 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %148: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %149: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%148), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%14, const<f64>(2.0)), const<f64>(100.0))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%149));
// DEFAULT-NEXT:         let %150: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %151: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%150), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%43, const<f32>(2.0)), const<f32>(100.0))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%151));
// DEFAULT-NEXT:         let %152: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %153: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%152), from_bool<i32, reason=promotion>(eq<f80, exceptions=ignore>(call<f80, signature=fn(f80) -> f80>(%65, const<f80>(2)), const<f80>(100))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%153));
// DEFAULT-NEXT:         let %154: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %155: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%154), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, ptr<i32>) -> f64>(%30, const<f64>(0.5), addr_of<ptr<i32>>(%79)), call<f64, signature=fn(f64) -> f64>(%28, const<f64>(0.5)))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%155));
// DEFAULT-NEXT:         let %156: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %157: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%156), from_bool<i32, reason=promotion>(eq<i32>(read<i32>(%79), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%157));
// DEFAULT-NEXT:         let %158: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %159: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%158), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%21, const<f64>(12.0)), const<f64>(1.5))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%159));
// DEFAULT-NEXT:         let %160: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %161: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%160), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%50, const<f32>(12.0)), const<f32>(1.5))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%161));
// DEFAULT-NEXT:         let %162: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %163: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%162), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%19, const<f64>(7.0), const<f64>(3.0)), const<f64>(1.0))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%163));
// DEFAULT-NEXT:         let %164: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %165: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%164), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%48, const<f32>(7.0), const<f32>(3.0)), const<f32>(1.0))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%165));
// DEFAULT-NEXT:         let %166: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %167: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%166), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%33, const<f64>(1.5), const<f64>(3.0)), const<f64>(12.0))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%167));
// DEFAULT-NEXT:         let %168: i32 [synthetic] = read<i32>(%80);
// DEFAULT-NEXT:         let %169: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%168), from_bool<i32, reason=promotion>(eq<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%55, const<f32>(1.5), const<f32>(3.0)), const<f32>(12.0))));
// DEFAULT-NEXT:         write<i32>(%80, read<i32>(%169));
// DEFAULT-NEXT:         return read<i32>(%80);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %81 @gnu_bessel_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %82 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %170: i32 [synthetic] = read<i32>(%82);
// DEFAULT-NEXT:         let %171: bool [synthetic];
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%22, const<f64>(1.0)), call<f64, signature=fn(f64) -> f64>(%22, const<f64>(1.0)))
// DEFAULT-NEXT:             write<bool>(%171, gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%22, const<f64>(1.0)), const<f64>(0.7)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%171, const<bool>(false));
// DEFAULT-NEXT:         let %172: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%170), from_bool<i32, reason=promotion>(read<bool>(%171)));
// DEFAULT-NEXT:         write<i32>(%82, read<i32>(%172));
// DEFAULT-NEXT:         let %173: i32 [synthetic] = read<i32>(%82);
// DEFAULT-NEXT:         let %174: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%173), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%23, const<f64>(1.0)), const<f64>(0.4))));
// DEFAULT-NEXT:         write<i32>(%82, read<i32>(%174));
// DEFAULT-NEXT:         let %175: i32 [synthetic] = read<i32>(%82);
// DEFAULT-NEXT:         let %176: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%175), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(i32, f64) -> f64>(%24, const<i32>(2), const<f64>(1.0)), const<f64>(0.1))));
// DEFAULT-NEXT:         write<i32>(%82, read<i32>(%176));
// DEFAULT-NEXT:         let %177: i32 [synthetic] = read<i32>(%82);
// DEFAULT-NEXT:         let %178: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%177), from_bool<i32, reason=promotion>(gt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%25, const<f64>(1.0)), const<f64>(0.08))));
// DEFAULT-NEXT:         write<i32>(%82, read<i32>(%178));
// DEFAULT-NEXT:         let %179: i32 [synthetic] = read<i32>(%82);
// DEFAULT-NEXT:         let %180: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%179), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(call<f64, signature=fn(f64) -> f64>(%26, const<f64>(1.0)), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%82, read<i32>(%180));
// DEFAULT-NEXT:         let %181: i32 [synthetic] = read<i32>(%82);
// DEFAULT-NEXT:         let %182: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%181), from_bool<i32, reason=promotion>(lt<f64, exceptions=ignore>(call<f64, signature=fn(i32, f64) -> f64>(%27, const<i32>(2), const<f64>(1.0)), const<f64>(0.0))));
// DEFAULT-NEXT:         write<i32>(%82, read<i32>(%182));
// DEFAULT-NEXT:         let %183: i32 [synthetic] = read<i32>(%82);
// DEFAULT-NEXT:         let %184: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%183), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%51, const<f32>(1.0)), const<f32>(0.7))));
// DEFAULT-NEXT:         write<i32>(%82, read<i32>(%184));
// DEFAULT-NEXT:         let %185: i32 [synthetic] = read<i32>(%82);
// DEFAULT-NEXT:         let %186: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%185), from_bool<i32, reason=promotion>(gt<f32, exceptions=ignore>(call<f32, signature=fn(f32) -> f32>(%52, const<f32>(1.0)), const<f32>(0.08))));
// DEFAULT-NEXT:         write<i32>(%82, read<i32>(%186));
// DEFAULT-NEXT:         return read<i32>(%82);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %83 @gnu_fenv_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %84 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %187: i32 [synthetic] = read<i32>(%84);
// DEFAULT-NEXT:         let %188: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%187), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%1, const<i32>(4)), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%84, read<i32>(%188));
// DEFAULT-NEXT:         let %189: i32 [synthetic] = read<i32>(%84);
// DEFAULT-NEXT:         let %190: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%189), from_bool<i32, reason=promotion>(ne<i32>(and<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(4)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%84, read<i32>(%190));
// DEFAULT-NEXT:         let %191: i32 [synthetic] = read<i32>(%84);
// DEFAULT-NEXT:         let %192: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%191), from_bool<i32, reason=promotion>(ne<i32>(call<i32, signature=fn(i32) -> i32>(%3, const<i32>(4)), neg<i32, overflow=ub>(const<i32>(1)))));
// DEFAULT-NEXT:         write<i32>(%84, read<i32>(%192));
// DEFAULT-NEXT:         let %193: i32 [synthetic] = read<i32>(%84);
// DEFAULT-NEXT:         let %194: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%193), from_bool<i32, reason=promotion>(eq<i32>(and<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(4)), const<i32>(0))));
// DEFAULT-NEXT:         write<i32>(%84, read<i32>(%194));
// DEFAULT-NEXT:         return read<i32>(%84);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %85 @gnu_constant_extensions() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %86 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %195: i32 [synthetic] = read<i32>(%86);
// DEFAULT-NEXT:         let %196: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%195), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(3.14159265358979323851)), const<f64>(3.141592653589793))));
// DEFAULT-NEXT:         write<i32>(%86, read<i32>(%196));
// DEFAULT-NEXT:         let %197: i32 [synthetic] = read<i32>(%86);
// DEFAULT-NEXT:         let %198: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%197), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(2.71828182845904523543)), const<f64>(2.718281828459045))));
// DEFAULT-NEXT:         write<i32>(%86, read<i32>(%198));
// DEFAULT-NEXT:         let %199: i32 [synthetic] = read<i32>(%86);
// DEFAULT-NEXT:         let %200: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%199), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(1.41421356237309504876)), const<f64>(1.4142135623730951))));
// DEFAULT-NEXT:         write<i32>(%86, read<i32>(%200));
// DEFAULT-NEXT:         let %201: i32 [synthetic] = read<i32>(%86);
// DEFAULT-NEXT:         let %202: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%201), from_bool<i32, reason=promotion>(eq<f64, exceptions=ignore>(float_narrow<f64, reason=explicit, rounding=nearest_even, exceptions=ignore>(const<f80>(0.693147180559945309429)), const<f64>(0.6931471805599453))));
// DEFAULT-NEXT:         write<i32>(%86, read<i32>(%202));
// DEFAULT-NEXT:         return read<i32>(%86);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%69, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%135)), call<i32, signature=fn() -> i32>(%70), call<i32, signature=fn() -> i32>(%78), call<i32, signature=fn() -> i32>(%81), call<i32, signature=fn() -> i32>(%83), call<i32, signature=fn() -> i32>(%85));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
