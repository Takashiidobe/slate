/* { dg-options "-ffast-math" } */

extern void abort(void);
extern void exit(int);

#define min1(a, b) ((a) < (b) ? (a) : (b))
#define max1(a, b) ((a) > (b) ? (a) : (b))

#define min2(a, b) ((a) <= (b) ? (a) : (b))
#define max2(a, b) ((a) >= (b) ? (a) : (b))

#define F(type, n)                                                             \
  type __attribute__((noinline)) type##_##n(type a, type b) { return n(a, b); }

F(float, min1)
F(float, min2)
F(float, max1)
F(float, max2)

F(double, min1)
F(double, min2)
F(double, max1)
F(double, max2)

int main() {
  if (float_min1(0.f, -1.f) != -1.f)
    abort();
  if (float_min1(-1.f, 0.f) != -1.f)
    abort();
  if (float_min1(0.f, 1.f) != 0.f)
    abort();
  if (float_min1(1.f, 0.f) != 0.f)
    abort();
  if (float_min1(-1.f, 1.f) != -1.f)
    abort();
  if (float_min1(1.f, -1.f) != -1.f)
    abort();

  if (float_max1(0.f, -1.f) != 0.f)
    abort();
  if (float_max1(-1.f, 0.f) != 0.f)
    abort();
  if (float_max1(0.f, 1.f) != 1.f)
    abort();
  if (float_max1(1.f, 0.f) != 1.f)
    abort();
  if (float_max1(-1.f, 1.f) != 1.f)
    abort();
  if (float_max1(1.f, -1.f) != 1.f)
    abort();

  if (float_min2(0.f, -1.f) != -1.f)
    abort();
  if (float_min2(-1.f, 0.f) != -1.f)
    abort();
  if (float_min2(0.f, 1.f) != 0.f)
    abort();
  if (float_min2(1.f, 0.f) != 0.f)
    abort();
  if (float_min2(-1.f, 1.f) != -1.f)
    abort();
  if (float_min2(1.f, -1.f) != -1.f)
    abort();

  if (float_max2(0.f, -1.f) != 0.f)
    abort();
  if (float_max2(-1.f, 0.f) != 0.f)
    abort();
  if (float_max2(0.f, 1.f) != 1.f)
    abort();
  if (float_max2(1.f, 0.f) != 1.f)
    abort();
  if (float_max2(-1.f, 1.f) != 1.f)
    abort();
  if (float_max2(1.f, -1.f) != 1.f)
    abort();

  if (double_min1(0., -1.) != -1.)
    abort();
  if (double_min1(-1., 0.) != -1.)
    abort();
  if (double_min1(0., 1.) != 0.)
    abort();
  if (double_min1(1., 0.) != 0.)
    abort();
  if (double_min1(-1., 1.) != -1.)
    abort();
  if (double_min1(1., -1.) != -1.)
    abort();

  if (double_max1(0., -1.) != 0.)
    abort();
  if (double_max1(-1., 0.) != 0.)
    abort();
  if (double_max1(0., 1.) != 1.)
    abort();
  if (double_max1(1., 0.) != 1.)
    abort();
  if (double_max1(-1., 1.) != 1.)
    abort();
  if (double_max1(1., -1.) != 1.)
    abort();

  if (double_min2(0., -1.) != -1.)
    abort();
  if (double_min2(-1., 0.) != -1.)
    abort();
  if (double_min2(0., 1.) != 0.)
    abort();
  if (double_min2(1., 0.) != 0.)
    abort();
  if (double_min2(-1., 1.) != -1.)
    abort();
  if (double_min2(1., -1.) != -1.)
    abort();

  if (double_max2(0., -1.) != 0.)
    abort();
  if (double_max2(-1., 0.) != 0.)
    abort();
  if (double_max2(0., 1.) != 1.)
    abort();
  if (double_max2(1., 0.) != 1.)
    abort();
  if (double_max2(-1., 1.) != 1.)
    abort();
  if (double_max2(1., -1.) != 1.)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%27 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @float_min1(%3 a: f32, %4 b: f32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f32>(lt<f32, exceptions=ignore>(read<f32>(%3), read<f32>(%4)), read<f32>(%3), read<f32>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @float_min2(%6 a: f32, %7 b: f32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f32>(le<f32, exceptions=ignore>(read<f32>(%6), read<f32>(%7)), read<f32>(%6), read<f32>(%7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @float_max1(%9 a: f32, %10 b: f32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f32>(gt<f32, exceptions=ignore>(read<f32>(%9), read<f32>(%10)), read<f32>(%9), read<f32>(%10));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @float_max2(%12 a: f32, %13 b: f32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f32>(ge<f32, exceptions=ignore>(read<f32>(%12), read<f32>(%13)), read<f32>(%12), read<f32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @double_min1(%15 a: f64, %16 b: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(lt<f64, exceptions=ignore>(read<f64>(%15), read<f64>(%16)), read<f64>(%15), read<f64>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @double_min2(%18 a: f64, %19 b: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(le<f64, exceptions=ignore>(read<f64>(%18), read<f64>(%19)), read<f64>(%18), read<f64>(%19));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @double_max1(%21 a: f64, %22 b: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(gt<f64, exceptions=ignore>(read<f64>(%21), read<f64>(%22)), read<f64>(%21), read<f64>(%22));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @double_max2(%24 a: f64, %25 b: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ge<f64, exceptions=ignore>(read<f64>(%24), read<f64>(%25)), read<f64>(%24), read<f64>(%25));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, const<f32>(0.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, neg<f32>(const<f32>(1.0)), const<f32>(0.0)), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, const<f32>(0.0), const<f32>(1.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, const<f32>(1.0), const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, neg<f32>(const<f32>(1.0)), const<f32>(1.0)), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%2, const<f32>(1.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, const<f32>(0.0), neg<f32>(const<f32>(1.0))), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, neg<f32>(const<f32>(1.0)), const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, const<f32>(0.0), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, const<f32>(1.0), const<f32>(0.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, neg<f32>(const<f32>(1.0)), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%8, const<f32>(1.0), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%5, const<f32>(0.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%5, neg<f32>(const<f32>(1.0)), const<f32>(0.0)), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%5, const<f32>(0.0), const<f32>(1.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%5, const<f32>(1.0), const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%5, neg<f32>(const<f32>(1.0)), const<f32>(1.0)), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%5, const<f32>(1.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%11, const<f32>(0.0), neg<f32>(const<f32>(1.0))), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%11, neg<f32>(const<f32>(1.0)), const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%11, const<f32>(0.0), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%11, const<f32>(1.0), const<f32>(0.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%11, neg<f32>(const<f32>(1.0)), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f32, exceptions=ignore>(call<f32, signature=fn(f32, f32) -> f32>(%11, const<f32>(1.0), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%14, const<f64>(0.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%14, neg<f64>(const<f64>(1.0)), const<f64>(0.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%14, const<f64>(0.0), const<f64>(1.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%14, const<f64>(1.0), const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%14, neg<f64>(const<f64>(1.0)), const<f64>(1.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%14, const<f64>(1.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%20, const<f64>(0.0), neg<f64>(const<f64>(1.0))), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%20, neg<f64>(const<f64>(1.0)), const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%20, const<f64>(0.0), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%20, const<f64>(1.0), const<f64>(0.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%20, neg<f64>(const<f64>(1.0)), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%20, const<f64>(1.0), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, const<f64>(0.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, neg<f64>(const<f64>(1.0)), const<f64>(0.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, const<f64>(0.0), const<f64>(1.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, const<f64>(1.0), const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, neg<f64>(const<f64>(1.0)), const<f64>(1.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%17, const<f64>(1.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%23, const<f64>(0.0), neg<f64>(const<f64>(1.0))), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%23, neg<f64>(const<f64>(1.0)), const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%23, const<f64>(0.0), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%23, const<f64>(1.0), const<f64>(0.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%23, neg<f64>(const<f64>(1.0)), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(f64, f64) -> f64>(%23, const<f64>(1.0), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
