/* { dg-do run } */
extern void abort(void);

void test(double f, double i) {
  if (f == __builtin_inf())
    abort();
  if (f == -__builtin_inf())
    abort();
  if (i == -__builtin_inf())
    abort();
  if (i != __builtin_inf())
    abort();

  if (f >= __builtin_inf())
    abort();
  if (f > __builtin_inf())
    abort();
  if (i > __builtin_inf())
    abort();
  if (f <= -__builtin_inf())
    abort();
  if (f < -__builtin_inf())
    abort();
}

void testf(float f, float i) {
  if (f == __builtin_inff())
    abort();
  if (f == -__builtin_inff())
    abort();
  if (i == -__builtin_inff())
    abort();
  if (i != __builtin_inff())
    abort();

  if (f >= __builtin_inff())
    abort();
  if (f > __builtin_inff())
    abort();
  if (i > __builtin_inff())
    abort();
  if (f <= -__builtin_inff())
    abort();
  if (f < -__builtin_inff())
    abort();
}

void testl(long double f, long double i) {
  if (f == __builtin_infl())
    abort();
  if (f == -__builtin_infl())
    abort();
  if (i == -__builtin_infl())
    abort();
  if (i != __builtin_infl())
    abort();

  if (f >= __builtin_infl())
    abort();
  if (f > __builtin_infl())
    abort();
  if (i > __builtin_infl())
    abort();
  if (f <= -__builtin_infl())
    abort();
  if (f < -__builtin_infl())
    abort();
}

int main() {
  test(34.0, __builtin_inf());
  testf(34.0f, __builtin_inff());
  testl(34.0l, __builtin_infl());
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_f:[0-9]+]] f: f64, %[[VALUE_i:[0-9]+]] i: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<f64, exceptions=observable>(read<f64>(%[[VALUE_f]]), call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<f64, exceptions=observable>(read<f64>(%[[VALUE_f]]), neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<f64, exceptions=observable>(read<f64>(%[[VALUE_i]]), neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%[[VALUE_i]]), call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ge<f64, exceptions=observable>(read<f64>(%[[VALUE_f]]), call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if gt<f64, exceptions=observable>(read<f64>(%[[VALUE_f]]), call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if gt<f64, exceptions=observable>(read<f64>(%[[VALUE_i]]), call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if le<f64, exceptions=observable>(read<f64>(%[[VALUE_f]]), neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if lt<f64, exceptions=observable>(read<f64>(%[[VALUE_f]]), neg<f64>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff:[0-9]+]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_testf:[0-9]+]] @testf(%[[VALUE_f_2:[0-9]+]] f: f32, %[[VALUE_i_2:[0-9]+]] i: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<f32, exceptions=observable>(read<f32>(%[[VALUE_f_2]]), call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<f32, exceptions=observable>(read<f32>(%[[VALUE_f_2]]), neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<f32, exceptions=observable>(read<f32>(%[[VALUE_i_2]]), neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%[[VALUE_i_2]]), call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ge<f32, exceptions=observable>(read<f32>(%[[VALUE_f_2]]), call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if gt<f32, exceptions=observable>(read<f32>(%[[VALUE_f_2]]), call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if gt<f32, exceptions=observable>(read<f32>(%[[VALUE_i_2]]), call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if le<f32, exceptions=observable>(read<f32>(%[[VALUE_f_2]]), neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if lt<f32, exceptions=observable>(read<f32>(%[[VALUE_f_2]]), neg<f32>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infl:[0-9]+]] @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_testl:[0-9]+]] @testl(%[[VALUE_f_3:[0-9]+]] f: f80, %[[VALUE_i_3:[0-9]+]] i: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<f80, exceptions=observable>(read<f80>(%[[VALUE_f_3]]), call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<f80, exceptions=observable>(read<f80>(%[[VALUE_f_3]]), neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if eq<f80, exceptions=observable>(read<f80>(%[[VALUE_i_3]]), neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f80, exceptions=observable>(read<f80>(%[[VALUE_i_3]]), call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ge<f80, exceptions=observable>(read<f80>(%[[VALUE_f_3]]), call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if gt<f80, exceptions=observable>(read<f80>(%[[VALUE_f_3]]), call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if gt<f80, exceptions=observable>(read<f80>(%[[VALUE_i_3]]), call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if le<f80, exceptions=observable>(read<f80>(%[[VALUE_f_3]]), neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if lt<f80, exceptions=observable>(read<f80>(%[[VALUE_f_3]]), neg<f80>(call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_test]], const<f64>(34.0), call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_testf]], const<f32>(34.0), call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]));
// DEFAULT-NEXT:         call<void, signature=fn(f80, f80) -> void>(%[[VALUE_testl]], const<f80>(34), call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
