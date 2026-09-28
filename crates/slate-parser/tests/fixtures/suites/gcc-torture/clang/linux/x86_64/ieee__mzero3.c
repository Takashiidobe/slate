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
// DEFAULT-NEXT:     global %2 nzerod: f64 [storage=static] = neg<f64>(const<f64>(0.0)) [linkage=external];
// DEFAULT-NEXT:     global %3 nzerof: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(neg<f64>(const<f64>(0.0))) [linkage=external];
// DEFAULT-NEXT:     global %4 zerod: f64 [storage=static] = const<f64>(0.0) [linkage=external];
// DEFAULT-NEXT:     global %5 zerof: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%17 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %6 @expectd(%11 value: f64, %12 expected: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %28: bool [synthetic];
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%11), read<f64>(%12))
// DEFAULT-NEXT:             write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%28, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%27, pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%11))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%12))), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%28)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @expectf(%13 value: f32, %14 expected: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %29: bool [synthetic];
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(read<f32>(%13), read<f32>(%14))
// DEFAULT-NEXT:             write<bool>(%29, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%29, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%27, pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f32>>(%13))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f32>>(%14))), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%29)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @negd(%15 v: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f64>(read<f64>(%15));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @negf(%16 v: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return neg<f32>(read<f32>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%6, call<f64, signature=fn(f64) -> f64>(%8, read<f64>(%4)), read<f64>(%2));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%7, call<f32, signature=fn(f32) -> f32>(%9, read<f32>(%5)), read<f32>(%3));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%6, call<f64, signature=fn(f64) -> f64>(%8, read<f64>(%2)), read<f64>(%4));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%7, call<f32, signature=fn(f32) -> f32>(%9, read<f32>(%3)), read<f32>(%5));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @__builtin_memcmp(%24 <unnamed>: ptr<const void>, %25 <unnamed>: ptr<const void>, %26 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
