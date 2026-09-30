#include <stdio.h>

static char          small = 12;
static unsigned char byte  = 200;
static float         ratio = 1.5f;
static double        total = 2.25;

static char add_char(char a, char b) { return a + b; }

static float scale(float value, float factor) { return value * factor; }

static double add_double(double a, double b) { return a + b; }

int main(void) {
  small = add_char(small, 3);
  byte  = byte + 1;
  ratio = scale(ratio, 2.0f);
  total = add_double(total, ratio);
  printf("%d\n", small);
  printf("%u\n", byte);
  printf("%f\n", ratio);
  printf("%f\n", total);
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
// DEFAULT-NEXT:     global %[[VALUE_small:[0-9]+]] small: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(12)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_byte:[0-9]+]] byte: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(200))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_ratio:[0-9]+]] ratio: f32 [storage=static] = const<f32>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_total:[0-9]+]] total: f64 [storage=static] = const<f64>(2.25) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_add_char:[0-9]+]] @add_char(%[[VALUE_a:[0-9]+]] a: i8, %[[VALUE_b:[0-9]+]] b: i8) -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_a]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_b]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_scale:[0-9]+]] @scale(%[[VALUE_value:[0-9]+]] value: f32, %[[VALUE_factor:[0-9]+]] factor: f32) -> f32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE_value]]), read<f32>(%[[VALUE_factor]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_add_double:[0-9]+]] @add_double(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_small]], call<i8, signature=fn(i8, i8) -> i8>(%[[VALUE_add_char]], read<i8>(%[[VALUE_small]]), truncate<i8, reason=arg, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         write<u8>(%[[VALUE_byte]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_byte]]))), const<i32>(1)))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_ratio]], call<f32, signature=fn(f32, f32) -> f32>(%[[VALUE_scale]], read<f32>(%[[VALUE_ratio]]), const<f32>(2.0)));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_total]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_add_double]], read<f64>(%[[VALUE_total]]), float_widen<f64, reason=arg>(read<f32>(%[[VALUE_ratio]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), widen<i32, reason=vararg>(read<i8>(%[[VALUE_small]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2]])), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%[[VALUE_byte]]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_3]])), float_widen<f64, reason=vararg>(read<f32>(%[[VALUE_ratio]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_4]])), read<f64>(%[[VALUE_total]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
