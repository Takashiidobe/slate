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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 35> [storage=static] = code_units<array<i8, 35>>([37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 46, 51, 102, 32, 37, 108, 100, 32, 37, 108, 108, 100, 32, 37, 46, 51, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sin:[0-9]+]] @__builtin_sin(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cos:[0-9]+]] @__builtin_cos(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_tan:[0-9]+]] @__builtin_tan(%[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log:[0-9]+]] @__builtin_log(%[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log10:[0-9]+]] @__builtin_log10(%[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_log2:[0-9]+]] @__builtin_log2(%[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_pow:[0-9]+]] @__builtin_pow(%[[VALUE6:[0-9]+]] <unnamed>: f64, %[[VALUE7:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_sqrt:[0-9]+]] @__builtin_sqrt(%[[VALUE8:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp:[0-9]+]] @__builtin_exp(%[[VALUE9:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_exp2:[0-9]+]] @__builtin_exp2(%[[VALUE10:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fmod:[0-9]+]] @__builtin_fmod(%[[VALUE11:[0-9]+]] <unnamed>: f64, %[[VALUE12:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_lround:[0-9]+]] @__builtin_lround(%[[VALUE13:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_llround:[0-9]+]] @__builtin_llround(%[[VALUE14:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_elementwise_exp10:[0-9]+]] @__builtin_elementwise_exp10(%[[VALUE15:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: volatile f64 [storage=automatic] = const<f64>(0.5);
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: volatile f64 [storage=automatic] = const<f64>(2.0);
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: volatile f64 [storage=automatic] = const<f64>(8.0);
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: volatile f64 [storage=automatic] = const<f64>(5.75);
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: volatile f64 [storage=automatic] = const<f64>(2.0);
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: volatile f64 [storage=automatic] = const<f64>(2.5);
// DEFAULT-NEXT:         let %[[VALUE_trig:[0-9]+]] trig: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sin]], read<f64, volatile>(%[[VALUE_a]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cos]], read<f64, volatile>(%[[VALUE_a]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_tan]], read<f64, volatile>(%[[VALUE_a]])));
// DEFAULT-NEXT:         let %[[VALUE_logs:[0-9]+]] logs: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log]], read<f64, volatile>(%[[VALUE_c]])), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log10]], const<f64>(100.0))), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_log2]], read<f64, volatile>(%[[VALUE_c]])));
// DEFAULT-NEXT:         let %[[VALUE_powers:[0-9]+]] powers: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_pow]], read<f64, volatile>(%[[VALUE_b]]), const<f64>(3.0)), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_sqrt]], read<f64, volatile>(%[[VALUE_c]]))), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_exp]], const<f64>(1.0))), call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_exp2]], const<f64>(3.0)));
// DEFAULT-NEXT:         let %[[VALUE_rem:[0-9]+]] rem: f64 [storage=automatic] = call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fmod]], read<f64, volatile>(%[[VALUE_d]]), read<f64, volatile>(%[[VALUE_e]]));
// DEFAULT-NEXT:         let %[[VALUE_rounded:[0-9]+]] rounded: i64 [storage=automatic] = call<i64, signature=fn(f64) -> i64>(%[[VALUE___builtin_lround]], read<f64, volatile>(%[[VALUE_f]]));
// DEFAULT-NEXT:         let %[[VALUE_rounded_ll:[0-9]+]] rounded_ll: i64 [storage=automatic] = call<i64, signature=fn(f64) -> i64>(%[[VALUE___builtin_llround]], read<f64, volatile>(%[[VALUE_f]]));
// DEFAULT-NEXT:         let %[[VALUE_exp10_val:[0-9]+]] exp10_val: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_elementwise_exp10]], const<f64>(2.0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(35)>(%[[VALUE_str]])), read<f64>(%[[VALUE_trig]]), read<f64>(%[[VALUE_logs]]), read<f64>(%[[VALUE_powers]]), read<f64>(%[[VALUE_rem]]), read<i64>(%[[VALUE_rounded]]), read<i64>(%[[VALUE_rounded_ll]]), read<f64>(%[[VALUE_exp10_val]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
