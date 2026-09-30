#include <stdio.h>

int main(void) {
  double fma_v    = __builtin_fma(2.0, 3.0, 1.0);
  double hypot_v  = __builtin_hypot(3.0, 4.0);
  double fdim_v   = __builtin_fdim(5.0, 2.0);
  double fdim_neg = __builtin_fdim(2.0, 5.0);
  double cbrt_v   = __builtin_cbrt(27.0);
  double ldexp_v  = __builtin_ldexp(1.0, 4);
  double scalbn_v = __builtin_scalbn(10.0, 3);
  double logb_v   = __builtin_logb(10.0);
  int    ilogb_v  = __builtin_ilogb(10.0);

  printf("%.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f %d\n", fma_v, hypot_v, fdim_v,
         fdim_neg, cbrt_v, ldexp_v, scalbn_v, logb_v, ilogb_v);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 44> [storage=static] = code_units<array<i8, 44>>([37, 46, 54, 102, 32, 37, 46, 54, 102, 32, 37, 46, 54, 102, 32, 37, 46, 54, 102, 32, 37, 46, 54, 102, 32, 37, 46, 54, 102, 32, 37, 46, 54, 102, 32, 37, 46, 54, 102, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fma:[0-9]+]] @__builtin_fma(%[[VALUE0:[0-9]+]] <unnamed>: f64, %[[VALUE1:[0-9]+]] <unnamed>: f64, %[[VALUE2:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_hypot:[0-9]+]] @__builtin_hypot(%[[VALUE3:[0-9]+]] <unnamed>: f64, %[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_fdim:[0-9]+]] @__builtin_fdim(%[[VALUE5:[0-9]+]] <unnamed>: f64, %[[VALUE6:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_cbrt:[0-9]+]] @__builtin_cbrt(%[[VALUE7:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ldexp:[0-9]+]] @__builtin_ldexp(%[[VALUE8:[0-9]+]] <unnamed>: f64, %[[VALUE9:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_scalbn:[0-9]+]] @__builtin_scalbn(%[[VALUE10:[0-9]+]] <unnamed>: f64, %[[VALUE11:[0-9]+]] <unnamed>: i32) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_logb:[0-9]+]] @__builtin_logb(%[[VALUE12:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_ilogb:[0-9]+]] @__builtin_ilogb(%[[VALUE13:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_fma_v:[0-9]+]] fma_v: f64 [storage=automatic] = call<f64, signature=fn(f64, f64, f64) -> f64>(%[[VALUE___builtin_fma]], const<f64>(2.0), const<f64>(3.0), const<f64>(1.0));
// DEFAULT-NEXT:         let %[[VALUE_hypot_v:[0-9]+]] hypot_v: f64 [storage=automatic] = call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_hypot]], const<f64>(3.0), const<f64>(4.0));
// DEFAULT-NEXT:         let %[[VALUE_fdim_v:[0-9]+]] fdim_v: f64 [storage=automatic] = call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fdim]], const<f64>(5.0), const<f64>(2.0));
// DEFAULT-NEXT:         let %[[VALUE_fdim_neg:[0-9]+]] fdim_neg: f64 [storage=automatic] = call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE___builtin_fdim]], const<f64>(2.0), const<f64>(5.0));
// DEFAULT-NEXT:         let %[[VALUE_cbrt_v:[0-9]+]] cbrt_v: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_cbrt]], const<f64>(27.0));
// DEFAULT-NEXT:         let %[[VALUE_ldexp_v:[0-9]+]] ldexp_v: f64 [storage=automatic] = call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_ldexp]], const<f64>(1.0), const<i32>(4));
// DEFAULT-NEXT:         let %[[VALUE_scalbn_v:[0-9]+]] scalbn_v: f64 [storage=automatic] = call<f64, signature=fn(f64, i32) -> f64>(%[[VALUE___builtin_scalbn]], const<f64>(10.0), const<i32>(3));
// DEFAULT-NEXT:         let %[[VALUE_logb_v:[0-9]+]] logb_v: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%[[VALUE___builtin_logb]], const<f64>(10.0));
// DEFAULT-NEXT:         let %[[VALUE_ilogb_v:[0-9]+]] ilogb_v: i32 [storage=automatic] = call<i32, signature=fn(f64) -> i32>(%[[VALUE___builtin_ilogb]], const<f64>(10.0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(44)>(%[[VALUE_str]])), read<f64>(%[[VALUE_fma_v]]), read<f64>(%[[VALUE_hypot_v]]), read<f64>(%[[VALUE_fdim_v]]), read<f64>(%[[VALUE_fdim_neg]]), read<f64>(%[[VALUE_cbrt_v]]), read<f64>(%[[VALUE_ldexp_v]]), read<f64>(%[[VALUE_scalbn_v]]), read<f64>(%[[VALUE_logb_v]]), read<i32>(%[[VALUE_ilogb_v]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
