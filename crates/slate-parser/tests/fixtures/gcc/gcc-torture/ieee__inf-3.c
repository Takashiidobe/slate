/* { dg-do run } */
extern void abort(void);

void test(double f, double i) {
  if (f == __builtin_huge_val())
    abort();
  if (f == -__builtin_huge_val())
    abort();
  if (i == -__builtin_huge_val())
    abort();
  if (i != __builtin_huge_val())
    abort();

  if (f >= __builtin_huge_val())
    abort();
  if (f > __builtin_huge_val())
    abort();
  if (i > __builtin_huge_val())
    abort();
  if (f <= -__builtin_huge_val())
    abort();
  if (f < -__builtin_huge_val())
    abort();
}

void testf(float f, float i) {
  if (f == __builtin_huge_valf())
    abort();
  if (f == -__builtin_huge_valf())
    abort();
  if (i == -__builtin_huge_valf())
    abort();
  if (i != __builtin_huge_valf())
    abort();

  if (f >= __builtin_huge_valf())
    abort();
  if (f > __builtin_huge_valf())
    abort();
  if (i > __builtin_huge_valf())
    abort();
  if (f <= -__builtin_huge_valf())
    abort();
  if (f < -__builtin_huge_valf())
    abort();
}

void testl(long double f, long double i) {
  if (f == __builtin_huge_vall())
    abort();
  if (f == -__builtin_huge_vall())
    abort();
  if (i == -__builtin_huge_vall())
    abort();
  if (i != __builtin_huge_vall())
    abort();

  if (f >= __builtin_huge_vall())
    abort();
  if (f > __builtin_huge_vall())
    abort();
  if (i > __builtin_huge_vall())
    abort();
  if (f <= -__builtin_huge_vall())
    abort();
  if (f < -__builtin_huge_vall())
    abort();
}

int main() {
  test(34.0, __builtin_huge_val());
  testf(34.0f, __builtin_huge_valf());
  testl(34.0l, __builtin_huge_vall());
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @test(%2 f: f64, %3 i: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%2), call<f64, signature=fn() -> f64>(__builtin_huge_val))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%2), neg<f64>(call<f64, signature=fn() -> f64>(__builtin_huge_val)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%3), neg<f64>(call<f64, signature=fn() -> f64>(__builtin_huge_val)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%3), call<f64, signature=fn() -> f64>(__builtin_huge_val))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ge<f64, exceptions=ignore>(read<f64>(%2), call<f64, signature=fn() -> f64>(__builtin_huge_val))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<f64, exceptions=ignore>(read<f64>(%2), call<f64, signature=fn() -> f64>(__builtin_huge_val))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<f64, exceptions=ignore>(read<f64>(%3), call<f64, signature=fn() -> f64>(__builtin_huge_val))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if le<f64, exceptions=ignore>(read<f64>(%2), neg<f64>(call<f64, signature=fn() -> f64>(__builtin_huge_val)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if lt<f64, exceptions=ignore>(read<f64>(%2), neg<f64>(call<f64, signature=fn() -> f64>(__builtin_huge_val)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @testf(%5 f: f32, %6 i: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<f32, exceptions=ignore>(read<f32>(%5), call<f32, signature=fn() -> f32>(__builtin_huge_valf))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if eq<f32, exceptions=ignore>(read<f32>(%5), neg<f32>(call<f32, signature=fn() -> f32>(__builtin_huge_valf)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if eq<f32, exceptions=ignore>(read<f32>(%6), neg<f32>(call<f32, signature=fn() -> f32>(__builtin_huge_valf)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(read<f32>(%6), call<f32, signature=fn() -> f32>(__builtin_huge_valf))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ge<f32, exceptions=ignore>(read<f32>(%5), call<f32, signature=fn() -> f32>(__builtin_huge_valf))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<f32, exceptions=ignore>(read<f32>(%5), call<f32, signature=fn() -> f32>(__builtin_huge_valf))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<f32, exceptions=ignore>(read<f32>(%6), call<f32, signature=fn() -> f32>(__builtin_huge_valf))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if le<f32, exceptions=ignore>(read<f32>(%5), neg<f32>(call<f32, signature=fn() -> f32>(__builtin_huge_valf)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if lt<f32, exceptions=ignore>(read<f32>(%5), neg<f32>(call<f32, signature=fn() -> f32>(__builtin_huge_valf)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @testl(%8 f: f80, %9 i: f80) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<f80, exceptions=ignore>(read<f80>(%8), call<f80, signature=fn() -> f80>(__builtin_huge_vall))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if eq<f80, exceptions=ignore>(read<f80>(%8), neg<f80>(call<f80, signature=fn() -> f80>(__builtin_huge_vall)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if eq<f80, exceptions=ignore>(read<f80>(%9), neg<f80>(call<f80, signature=fn() -> f80>(__builtin_huge_vall)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<f80, exceptions=ignore>(read<f80>(%9), call<f80, signature=fn() -> f80>(__builtin_huge_vall))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ge<f80, exceptions=ignore>(read<f80>(%8), call<f80, signature=fn() -> f80>(__builtin_huge_vall))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<f80, exceptions=ignore>(read<f80>(%8), call<f80, signature=fn() -> f80>(__builtin_huge_vall))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<f80, exceptions=ignore>(read<f80>(%9), call<f80, signature=fn() -> f80>(__builtin_huge_vall))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if le<f80, exceptions=ignore>(read<f80>(%8), neg<f80>(call<f80, signature=fn() -> f80>(__builtin_huge_vall)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if lt<f80, exceptions=ignore>(read<f80>(%8), neg<f80>(call<f80, signature=fn() -> f80>(__builtin_huge_vall)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%1, const<f64>(34.0), call<f64, signature=fn() -> f64>(__builtin_huge_val));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%4, const<f32>(34.0), call<f32, signature=fn() -> f32>(__builtin_huge_valf));
// DEFAULT-NEXT:         call<void, signature=fn(f80, f80) -> void>(%7, const<f80>(34), call<f80, signature=fn() -> f80>(__builtin_huge_vall));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
