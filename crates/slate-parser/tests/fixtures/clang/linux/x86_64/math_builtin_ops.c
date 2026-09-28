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
// DEFAULT-NEXT:     global %46 .str46: array<i8, 35> [storage=static] = code_units<array<i8, 35>>([37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 108, 100, 32, 37, 108, 108, 100, 32, 37, 46, 51, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%15 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_sin(%16 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %19 @__builtin_cos(%18 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %21 @__builtin_tan(%20 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %23 @__builtin_log(%22 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %25 @__builtin_log10(%24 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %27 @__builtin_log2(%26 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %30 @__builtin_pow(%28 <unnamed>: f64, %29 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %32 @__builtin_sqrt(%31 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %34 @__builtin_exp(%33 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %36 @__builtin_exp2(%35 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %39 @__builtin_fmod(%37 <unnamed>: f64, %38 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %41 @__builtin_lround(%40 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %43 @__builtin_llround(%42 <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %45 @__builtin_elementwise_exp10(%44 <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 a: volatile f64 [storage=automatic] = const<f64>(0.5);
// DEFAULT-NEXT:         let %3 b: volatile f64 [storage=automatic] = const<f64>(2.0);
// DEFAULT-NEXT:         let %4 c: volatile f64 [storage=automatic] = const<f64>(8.0);
// DEFAULT-NEXT:         let %5 d: volatile f64 [storage=automatic] = const<f64>(5.75);
// DEFAULT-NEXT:         let %6 e: volatile f64 [storage=automatic] = const<f64>(2.0);
// DEFAULT-NEXT:         let %7 f: volatile f64 [storage=automatic] = const<f64>(2.5);
// DEFAULT-NEXT:         let %8 trig: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%17, read<f64, volatile>(%2)), call<f64, signature=fn(f64) -> f64>(%19, read<f64, volatile>(%2))), call<f64, signature=fn(f64) -> f64>(%21, read<f64, volatile>(%2)));
// DEFAULT-NEXT:         let %9 logs: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%23, read<f64, volatile>(%4)), call<f64, signature=fn(f64) -> f64>(%25, const<f64>(100.0))), call<f64, signature=fn(f64) -> f64>(%27, read<f64, volatile>(%4)));
// DEFAULT-NEXT:         let %10 powers: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64, f64) -> f64>(%30, read<f64, volatile>(%3), const<f64>(3.0)), call<f64, signature=fn(f64) -> f64>(%32, read<f64, volatile>(%4))), call<f64, signature=fn(f64) -> f64>(%34, const<f64>(1.0))), call<f64, signature=fn(f64) -> f64>(%36, const<f64>(3.0)));
// DEFAULT-NEXT:         let %11 rem: f64 [storage=automatic] = call<f64, signature=fn(f64, f64) -> f64>(%39, read<f64, volatile>(%5), read<f64, volatile>(%6));
// DEFAULT-NEXT:         let %12 rounded: i64 [storage=automatic] = call<i64, signature=fn(f64) -> i64>(%41, read<f64, volatile>(%7));
// DEFAULT-NEXT:         let %13 rounded_ll: i64 [storage=automatic] = call<i64, signature=fn(f64) -> i64>(%43, read<f64, volatile>(%7));
// DEFAULT-NEXT:         let %14 exp10_val: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%45, const<f64>(2.0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(35)>(%46)), read<f64>(%8), read<f64>(%9), read<f64>(%10), read<f64>(%11), read<i64>(%12), read<i64>(%13), read<f64>(%14));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
