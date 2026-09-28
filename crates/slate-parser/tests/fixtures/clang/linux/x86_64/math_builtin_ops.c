#include <stdio.h>

int main(void) {
  volatile double a    = 0.5;
  volatile double b    = 2.0;
  volatile double c    = 8.0;
  volatile double d    = 5.75;
  volatile double e    = 2.0;
  volatile double f    = 2.5;
  double          trig = __builtin_sin(a) + __builtin_cos(a) + __builtin_tan(a);
  double logs   = __builtin_log(c) + __builtin_log10(100.0) + __builtin_log2(c);
  double powers = __builtin_pow(b, 3.0) + __builtin_sqrt(c) +
                  __builtin_exp(1.0) + __builtin_exp2(3.0);
  double rem    = __builtin_fmod(d, e);
  long   rounded       = __builtin_lround(f);
  long long rounded_ll = __builtin_llround(f);
#if __has_builtin(__builtin_elementwise_exp10)
  double exp10_val = __builtin_elementwise_exp10(2.0);
#else
  double exp10_val = __builtin_pow(10.0, 2.0);
#endif
  printf("%.3f %.3f %.3f %.3f %ld %lld %.3f\n", trig, logs, powers, rem,
         rounded, rounded_ll, exp10_val);
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
// DEFAULT-NEXT:     global %47 .str47: array<i8, 35> [storage=static] = code_units<array<i8, 35>>([37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 108, 100, 32, 37, 108, 108, 100, 32, 37, 46, 51, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %18 @__builtin_sin(%17 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %20 @__builtin_cos(%19 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %22 @__builtin_tan(%21 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %24 @__builtin_log(%23 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %26 @__builtin_log10(%25 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %28 @__builtin_log2(%27 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %31 @__builtin_pow(%29 <unnamed>: f64, %30 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %33 @__builtin_sqrt(%32 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %35 @__builtin_exp(%34 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %37 @__builtin_exp2(%36 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %40 @__builtin_fmod(%38 <unnamed>: f64, %39 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %42 @__builtin_lround(%41 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %44 @__builtin_llround(%43 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %46 @__builtin_elementwise_exp10(%45 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 a: volatile f64 [storage=automatic] = const<f64>(0.5);
// DEFAULT-NEXT:         let %4 b: volatile f64 [storage=automatic] = const<f64>(2.0);
// DEFAULT-NEXT:         let %5 c: volatile f64 [storage=automatic] = const<f64>(8.0);
// DEFAULT-NEXT:         let %6 d: volatile f64 [storage=automatic] = const<f64>(5.75);
// DEFAULT-NEXT:         let %7 e: volatile f64 [storage=automatic] = const<f64>(2.0);
// DEFAULT-NEXT:         let %8 f: volatile f64 [storage=automatic] = const<f64>(2.5);
// DEFAULT-NEXT:         let %9 trig: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%18, read<f64, volatile>(%3)), call<f64, signature=fn(f64) -> f64>(%20, read<f64, volatile>(%3))), call<f64, signature=fn(f64) -> f64>(%22, read<f64, volatile>(%3)));
// DEFAULT-NEXT:         let %10 logs: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%24, read<f64, volatile>(%5)), call<f64, signature=fn(f64) -> f64>(%26, const<f64>(100.0))), call<f64, signature=fn(f64) -> f64>(%28, read<f64, volatile>(%5)));
// DEFAULT-NEXT:         let %11 powers: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64, f64) -> f64>(%31, read<f64, volatile>(%4), const<f64>(3.0)), call<f64, signature=fn(f64) -> f64>(%33, read<f64, volatile>(%5))), call<f64, signature=fn(f64) -> f64>(%35, const<f64>(1.0))), call<f64, signature=fn(f64) -> f64>(%37, const<f64>(3.0)));
// DEFAULT-NEXT:         let %12 rem: f64 [storage=automatic] = call<f64, signature=fn(f64, f64) -> f64>(%40, read<f64, volatile>(%6), read<f64, volatile>(%7));
// DEFAULT-NEXT:         let %13 rounded: i64 [storage=automatic] = call<i64, signature=fn(f64) -> i64>(%42, read<f64, volatile>(%8));
// DEFAULT-NEXT:         let %14 rounded_ll: i64 [storage=automatic] = call<i64, signature=fn(f64) -> i64>(%44, read<f64, volatile>(%8));
// DEFAULT-NEXT:         let %15 exp10_val: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%46, const<f64>(2.0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(35)>(%47)), read<f64>(%9), read<f64>(%10), read<f64>(%11), read<f64>(%12), read<i64>(%13), read<i64>(%14), read<f64>(%15));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
