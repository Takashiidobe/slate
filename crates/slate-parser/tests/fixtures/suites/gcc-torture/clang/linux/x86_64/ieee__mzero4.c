/* { dg-do run } */
/* Copyright (C) 2003  Free Software Foundation.
   by Roger Sayle <roger@eyesopen.com>, derived from mzero3.c

   Constant folding of sin(-0.0), tan(-0.0) and atan(-0.0) should
   all return -0.0, for both double and float forms.  */

void                  abort(void);
typedef __SIZE_TYPE__ size_t;
extern int            memcmp(const void *, const void *, size_t);

double sin(double);
double tan(double);
double atan(double);

float sinf(float);
float tanf(float);
float atanf(float);

void expectd(double, double);
void expectf(float, float);

void expectd(double value, double expected) {
  if (value != expected ||
      memcmp((void *)&value, (void *)&expected, sizeof(double)) != 0)
    abort();
}

void expectf(float value, float expected) {
  if (value != expected ||
      memcmp((void *)&value, (void *)&expected, sizeof(float)) != 0)
    abort();
}

int main() {
  expectd(sin(0.0), 0.0);
  expectd(tan(0.0), 0.0);
  expectd(atan(0.0), 0.0);

  expectd(sin(-0.0), -0.0);
  expectd(tan(-0.0), -0.0);
  expectd(atan(-0.0), -0.0);

  expectf(sinf(0.0f), 0.0f);
  expectf(tanf(0.0f), 0.0f);
  expectf(atanf(0.0f), 0.0f);

  expectf(sinf(-0.0f), -0.0f);
  expectf(tanf(-0.0f), -0.0f);
  expectf(atanf(-0.0f), -0.0f);

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sin:[0-9]+]] @sin(%[[VALUE3:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tan:[0-9]+]] @tan(%[[VALUE4:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atan:[0-9]+]] @atan(%[[VALUE5:[0-9]+]] <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sinf:[0-9]+]] @sinf(%[[VALUE6:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_tanf:[0-9]+]] @tanf(%[[VALUE7:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_atanf:[0-9]+]] @atanf(%[[VALUE8:[0-9]+]] <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expectd:[0-9]+]] @expectd(%[[VALUE_value:[0-9]+]] value: f64, %[[VALUE_expected:[0-9]+]] expected: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_value]]), read<f64>(%[[VALUE_expected]]))
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE9]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%[[VALUE_value]]))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%[[VALUE_expected]]))), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE9]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_expectf:[0-9]+]] @expectf(%[[VALUE_value_2:[0-9]+]] value: f32, %[[VALUE_expected_2:[0-9]+]] expected: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(read<f32>(%[[VALUE_value_2]]), read<f32>(%[[VALUE_expected_2]]))
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f32>>(%[[VALUE_value_2]]))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f32>>(%[[VALUE_expected_2]]))), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE10]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_expectd]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_expectd]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_tan]], const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_expectd]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_atan]], const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_expectd]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_sin]], neg<f64>(const<f64>(0.0))), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_expectd]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_tan]], neg<f64>(const<f64>(0.0))), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_expectd]], call<f64, signature=fn(f64) -> f64>(%[[VALUE_atan]], neg<f64>(const<f64>(0.0))), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_expectf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_sinf]], const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_expectf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_tanf]], const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_expectf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_atanf]], const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_expectf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_sinf]], neg<f32>(const<f32>(0.0))), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_expectf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_tanf]], neg<f32>(const<f32>(0.0))), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_expectf]], call<f32, signature=fn(f32) -> f32>(%[[VALUE_atanf]], neg<f32>(const<f32>(0.0))), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
