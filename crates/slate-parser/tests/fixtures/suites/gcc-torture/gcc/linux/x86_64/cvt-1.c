void abort(void);
void exit(int);

static inline long g1(double x) { return (double)(long)x; }

long g2(double f) { return f; }

double f(long i) {
  if (g1(i) != g2(i))
    abort();
  return g2(i);
}

int main(void) {
  if (f(123456789L) != 123456789L)
    abort();
  if (f(123456789L) != g2(123456789L))
    abort();
  exit(0);
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_g1:[0-9]+]] @g1(%[[VALUE_x:[0-9]+]] x: f64) -> i64 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i64, reason=return, out_of_range=ub, exceptions=observable>(int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_x]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g2:[0-9]+]] @g2(%[[VALUE_f:[0-9]+]] f: f64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return float_to_int<i64, reason=return, out_of_range=ub, exceptions=observable>(read<f64>(%[[VALUE_f]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f_2:[0-9]+]] @f(%[[VALUE_i:[0-9]+]] i: i64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(f64) -> i64>(%[[VALUE_g1]], int_to_float<f64, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(read<i64>(%[[VALUE_i]]))), call<i64, signature=fn(f64) -> i64>(%[[VALUE_g2]], int_to_float<f64, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(read<i64>(%[[VALUE_i]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return int_to_float<f64, reason=return, exact=false, rounding=nearest_even, exceptions=observable>(call<i64, signature=fn(f64) -> i64>(%[[VALUE_g2]], int_to_float<f64, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(read<i64>(%[[VALUE_i]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(i64) -> f64>(%[[VALUE_f_2]], const<i64>(123456789)), int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i64>(123456789)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(i64) -> f64>(%[[VALUE_f_2]], const<i64>(123456789)), int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(call<i64, signature=fn(f64) -> i64>(%[[VALUE_g2]], int_to_float<f64, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i64>(123456789)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
