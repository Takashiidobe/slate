/* { dg-do run } */
/* Copyright (C) 2002  Free Software Foundation.
   by Hans-Peter Nilsson  <hp@bitrange.com>, derived from mzero2.c

   In the MMIX port, negdf2 was bogusly expanding -x into 0 - x.  */

void abort(void);
void exit(int);

double nzerod = -0.0;
float  nzerof = -0.0;
double zerod  = 0.0;
float  zerof  = 0.0;

void   expectd(double, double);
void   expectf(float, float);
double negd(double);
float  negf(float);

int main(void) {
  expectd(negd(zerod), nzerod);
  expectf(negf(zerof), nzerof);
  expectd(negd(nzerod), zerod);
  expectf(negf(nzerof), zerof);
  exit(0);
}

void expectd(double value, double expected) {
  if (value != expected ||
      __builtin_memcmp((void *)&value, (void *)&expected, sizeof(double)) != 0)
    abort();
}

void expectf(float value, float expected) {
  if (value != expected ||
      __builtin_memcmp((void *)&value, (void *)&expected, sizeof(float)) != 0)
    abort();
}

double negd(double v) { return -v; }

float negf(float v) { return -v; }


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
// DEFAULT-NEXT:     global %[[VALUE_nzerod:[0-9]+]] nzerod: f64 [storage=static] = neg<f64>(const<f64>(0.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_nzerof:[0-9]+]] nzerof: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zerod:[0-9]+]] zerod: f64 [storage=static] = const<f64>(0.0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zerof:[0-9]+]] zerof: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_expectd:[0-9]+]] @expectd(%[[VALUE_value:[0-9]+]] value: f64, %[[VALUE_expected:[0-9]+]] expected: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value]]), read<f64>(%[[VALUE_expected]]))
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE1]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp:[0-9]+]], pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%[[VALUE_value]]))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%[[VALUE_expected]]))), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_expectf:[0-9]+]] @expectf(%[[VALUE_value_2:[0-9]+]] value: f32, %[[VALUE_expected_2:[0-9]+]] expected: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_value_2]]), read<f32>(%[[VALUE_expected_2]]))
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f32>>(%[[VALUE_value_2]]))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f32>>(%[[VALUE_expected_2]]))), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_negd:[0-9]+]] @negd(%[[VALUE_v:[0-9]+]] v: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(read<f64>(%[[VALUE_v]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_negf:[0-9]+]] @negf(%[[VALUE_v_2:[0-9]+]] v: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f32>(read<f32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_expectd]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_negd]], read<f64>(%[[VALUE_zerod]])), read<f64>(%[[VALUE_nzerod]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_expectf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_negf]], read<f32>(%[[VALUE_zerof]])), read<f32>(%[[VALUE_nzerof]]));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_expectd]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_negd]], read<f64>(%[[VALUE_nzerod]])), read<f64>(%[[VALUE_zerod]]));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_expectf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_negf]], read<f32>(%[[VALUE_nzerof]])), read<f32>(%[[VALUE_zerof]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp]] @__builtin_memcmp(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE4:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE5:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
