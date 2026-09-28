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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @memcmp(%16 <unnamed>: ptr<const void>, %17 <unnamed>: ptr<const void>, %18 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @sin(%19 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @tan(%20 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @atan(%21 <unnamed>: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %6 @sinf(%22 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @tanf(%23 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @atanf(%24 <unnamed>: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %9 @expectd(%11 value: f64, %12 expected: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %29: bool [synthetic];
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(read<f64>(%11), read<f64>(%12))
// DEFAULT-NEXT:             write<bool>(%29, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%29, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%11))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f64>>(%12))), const<u64>(8)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%29)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @expectf(%13 value: f32, %14 expected: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30: bool [synthetic];
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(read<f32>(%13), read<f32>(%14))
// DEFAULT-NEXT:             write<bool>(%30, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%30, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f32>>(%13))), pointer_cast<ptr<const void>, reason=arg>(pointer_cast<ptr<void>, reason=explicit>(addr_of<ptr<f32>>(%14))), const<u64>(4)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%30)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%9, call<f64, signature=fn(f64) -> f64>(%3, const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%9, call<f64, signature=fn(f64) -> f64>(%4, const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%9, call<f64, signature=fn(f64) -> f64>(%5, const<f64>(0.0)), const<f64>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%9, call<f64, signature=fn(f64) -> f64>(%3, neg<f64>(const<f64>(0.0))), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%9, call<f64, signature=fn(f64) -> f64>(%4, neg<f64>(const<f64>(0.0))), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%9, call<f64, signature=fn(f64) -> f64>(%5, neg<f64>(const<f64>(0.0))), neg<f64>(const<f64>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%10, call<f32, signature=fn(f32) -> f32>(%6, const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%10, call<f32, signature=fn(f32) -> f32>(%7, const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%10, call<f32, signature=fn(f32) -> f32>(%8, const<f32>(0.0)), const<f32>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%10, call<f32, signature=fn(f32) -> f32>(%6, neg<f32>(const<f32>(0.0))), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%10, call<f32, signature=fn(f32) -> f32>(%7, neg<f32>(const<f32>(0.0))), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%10, call<f32, signature=fn(f32) -> f32>(%8, neg<f32>(const<f32>(0.0))), neg<f32>(const<f32>(0.0)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
