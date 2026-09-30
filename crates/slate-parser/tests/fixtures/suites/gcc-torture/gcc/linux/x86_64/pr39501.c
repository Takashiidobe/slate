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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_float_min1:[0-9]+]] @float_min1(%[[VALUE_a:[0-9]+]] a: f32, %[[VALUE_b:[0-9]+]] b: f32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f32>(lt<f32, exceptions=observable>(read<f32>(%[[VALUE_a]]), read<f32>(%[[VALUE_b]])), read<f32>(%[[VALUE_a]]), read<f32>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_float_min2:[0-9]+]] @float_min2(%[[VALUE_a_2:[0-9]+]] a: f32, %[[VALUE_b_2:[0-9]+]] b: f32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f32>(le<f32, exceptions=observable>(read<f32>(%[[VALUE_a_2]]), read<f32>(%[[VALUE_b_2]])), read<f32>(%[[VALUE_a_2]]), read<f32>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_float_max1:[0-9]+]] @float_max1(%[[VALUE_a_3:[0-9]+]] a: f32, %[[VALUE_b_3:[0-9]+]] b: f32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f32>(gt<f32, exceptions=observable>(read<f32>(%[[VALUE_a_3]]), read<f32>(%[[VALUE_b_3]])), read<f32>(%[[VALUE_a_3]]), read<f32>(%[[VALUE_b_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_float_max2:[0-9]+]] @float_max2(%[[VALUE_a_4:[0-9]+]] a: f32, %[[VALUE_b_4:[0-9]+]] b: f32) -> f32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f32>(ge<f32, exceptions=observable>(read<f32>(%[[VALUE_a_4]]), read<f32>(%[[VALUE_b_4]])), read<f32>(%[[VALUE_a_4]]), read<f32>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_double_min1:[0-9]+]] @double_min1(%[[VALUE_a_5:[0-9]+]] a: f64, %[[VALUE_b_5:[0-9]+]] b: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(lt<f64, exceptions=observable>(read<f64>(%[[VALUE_a_5]]), read<f64>(%[[VALUE_b_5]])), read<f64>(%[[VALUE_a_5]]), read<f64>(%[[VALUE_b_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_double_min2:[0-9]+]] @double_min2(%[[VALUE_a_6:[0-9]+]] a: f64, %[[VALUE_b_6:[0-9]+]] b: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(le<f64, exceptions=observable>(read<f64>(%[[VALUE_a_6]]), read<f64>(%[[VALUE_b_6]])), read<f64>(%[[VALUE_a_6]]), read<f64>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_double_max1:[0-9]+]] @double_max1(%[[VALUE_a_7:[0-9]+]] a: f64, %[[VALUE_b_7:[0-9]+]] b: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(gt<f64, exceptions=observable>(read<f64>(%[[VALUE_a_7]]), read<f64>(%[[VALUE_b_7]])), read<f64>(%[[VALUE_a_7]]), read<f64>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_double_max2:[0-9]+]] @double_max2(%[[VALUE_a_8:[0-9]+]] a: f64, %[[VALUE_b_8:[0-9]+]] b: f64) -> f64 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return conditional<f64>(ge<f64, exceptions=observable>(read<f64>(%[[VALUE_a_8]]), read<f64>(%[[VALUE_b_8]])), read<f64>(%[[VALUE_a_8]]), read<f64>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min1]], const<f32>(0.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min1]], neg<f32>(const<f32>(1.0)), const<f32>(0.0)), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min1]], const<f32>(0.0), const<f32>(1.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min1]], const<f32>(1.0), const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min1]], neg<f32>(const<f32>(1.0)), const<f32>(1.0)), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min1]], const<f32>(1.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max1]], const<f32>(0.0), neg<f32>(const<f32>(1.0))), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max1]], neg<f32>(const<f32>(1.0)), const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max1]], const<f32>(0.0), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max1]], const<f32>(1.0), const<f32>(0.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max1]], neg<f32>(const<f32>(1.0)), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max1]], const<f32>(1.0), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min2]], const<f32>(0.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min2]], neg<f32>(const<f32>(1.0)), const<f32>(0.0)), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min2]], const<f32>(0.0), const<f32>(1.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min2]], const<f32>(1.0), const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min2]], neg<f32>(const<f32>(1.0)), const<f32>(1.0)), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_min2]], const<f32>(1.0), neg<f32>(const<f32>(1.0))), neg<f32>(const<f32>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max2]], const<f32>(0.0), neg<f32>(const<f32>(1.0))), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max2]], neg<f32>(const<f32>(1.0)), const<f32>(0.0)), const<f32>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max2]], const<f32>(0.0), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max2]], const<f32>(1.0), const<f32>(0.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max2]], neg<f32>(const<f32>(1.0)), const<f32>(1.0)), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f32, exceptions=observable>(call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_float_max2]], const<f32>(1.0), neg<f32>(const<f32>(1.0))), const<f32>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min1]], const<f64>(0.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min1]], neg<f64>(const<f64>(1.0)), const<f64>(0.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min1]], const<f64>(0.0), const<f64>(1.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min1]], const<f64>(1.0), const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min1]], neg<f64>(const<f64>(1.0)), const<f64>(1.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min1]], const<f64>(1.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max1]], const<f64>(0.0), neg<f64>(const<f64>(1.0))), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max1]], neg<f64>(const<f64>(1.0)), const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max1]], const<f64>(0.0), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max1]], const<f64>(1.0), const<f64>(0.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max1]], neg<f64>(const<f64>(1.0)), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max1]], const<f64>(1.0), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min2]], const<f64>(0.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min2]], neg<f64>(const<f64>(1.0)), const<f64>(0.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min2]], const<f64>(0.0), const<f64>(1.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min2]], const<f64>(1.0), const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min2]], neg<f64>(const<f64>(1.0)), const<f64>(1.0)), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_min2]], const<f64>(1.0), neg<f64>(const<f64>(1.0))), neg<f64>(const<f64>(1.0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max2]], const<f64>(0.0), neg<f64>(const<f64>(1.0))), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max2]], neg<f64>(const<f64>(1.0)), const<f64>(0.0)), const<f64>(0.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max2]], const<f64>(0.0), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max2]], const<f64>(1.0), const<f64>(0.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max2]], neg<f64>(const<f64>(1.0)), const<f64>(1.0)), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_double_max2]], const<f64>(1.0), neg<f64>(const<f64>(1.0))), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
