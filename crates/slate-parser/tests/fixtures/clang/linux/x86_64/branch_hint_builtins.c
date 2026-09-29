// { dg-additional-options "-O1" }
#include <stdio.h>

static int side_effect_calls = 0;

static int bump(int v) {
  side_effect_calls++;
  return v;
}

static int use_expect(int x) {
  if (__builtin_expect(bump(x), 1)) {
    return 1;
  }
  return 0;
}

static int use_expect_with_probability(int x) {
  if (__builtin_expect_with_probability(bump(x), 1, 0.9)) {
    return 1;
  }
  return 0;
}

static int use_unpredictable(int x) {
  if (__builtin_unpredictable(bump(x) > 0)) {
    return 1;
  }
  return 0;
}

int main(void) {
  int a = use_expect(1);
  int b = use_expect_with_probability(1);
  int c = use_unpredictable(1);
  printf("%d %d %d %d\n", a, b, c, side_effect_calls);
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
// DEFAULT-NEXT:     global %[[VALUE_side_effect_calls:[0-9]+]] side_effect_calls: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_bump:[0-9]+]] @bump(%[[VALUE_v:[0-9]+]] v: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_side_effect_calls]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_side_effect_calls]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_v]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect:[0-9]+]] @__builtin_expect(%[[VALUE2:[0-9]+]] <unnamed>: i64, %[[VALUE3:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_use_expect:[0-9]+]] @use_expect(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64) -> i64>(%[[VALUE___builtin_expect]], widen<i64, reason=arg>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_bump]], read<i32>(%[[VALUE_x]]))), widen<i64, reason=arg>(const<i32>(1))), const<i64>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_expect_with_probability:[0-9]+]] @__builtin_expect_with_probability(%[[VALUE4:[0-9]+]] <unnamed>: i64, %[[VALUE5:[0-9]+]] <unnamed>: i64, %[[VALUE6:[0-9]+]] <unnamed>: f64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_use_expect_with_probability:[0-9]+]] @use_expect_with_probability(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64, i64, f64) -> i64>(%[[VALUE___builtin_expect_with_probability]], widen<i64, reason=arg>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_bump]], read<i32>(%[[VALUE_x_2]]))), widen<i64, reason=arg>(const<i32>(1)), const<f64>(0.9)), const<i64>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_unpredictable:[0-9]+]] @__builtin_unpredictable(%[[VALUE7:[0-9]+]] <unnamed>: i64) -> i64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_use_unpredictable:[0-9]+]] @use_unpredictable(%[[VALUE_x_3:[0-9]+]] x: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i64>(call<i64, signature=fn(i64) -> i64>(%[[VALUE___builtin_unpredictable]], from_bool<i64, reason=arg>(gt<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_bump]], read<i32>(%[[VALUE_x_3]])), const<i32>(0)))), const<i64>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 return const<i32>(1);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_use_expect]], const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_use_expect_with_probability]], const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic] = call<i32, signature=fn(i32) -> i32>(%[[VALUE_use_unpredictable]], const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]]), read<i32>(%[[VALUE_c]]), read<i32>(%[[VALUE_side_effect_calls]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
