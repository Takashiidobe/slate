
#if (__SIZEOF_DOUBLE__ == 8)
double d = 1024.0 - 1.0 / 32768.0;
#else
double d = 1024.0 - 1.0 / 16384.0;
#endif

extern double floor(double);
extern float  floorf(float);
extern void   abort();

int main() {

  double df = floor(d);
  float  f1 = (float)floor(d);

  if ((int)df != 1023 || (int)f1 != 1023)
    abort();

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
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] = sub<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(const<f64>(1024.0), div<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(const<f64>(1.0), const<f64>(32768.0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_floor:[0-9]+]] @floor(%[[VALUE0:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_floorf:[0-9]+]] @floorf(%[[VALUE1:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_df:[0-9]+]] df: f64 [storage=automatic] = call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_d]]));
// DEFAULT-NEXT:         let %[[VALUE_f1:[0-9]+]] f1: f32 [storage=automatic] = float_narrow<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn(f64) -> f64>(%[[VALUE_floor]], read<f64>(%[[VALUE_d]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_df]])), const<i32>(1023)), ne<i32>(float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(read<f32>(%[[VALUE_f1]])), const<i32>(1023)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
