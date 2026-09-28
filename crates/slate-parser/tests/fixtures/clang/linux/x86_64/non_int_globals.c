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
// DEFAULT-NEXT:     global %2 small: i8 [storage=static] = truncate<i8, reason=assign, fits=always>(const<i32>(12)) [linkage=internal];
// DEFAULT-NEXT:     global %3 byte: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(200))) [linkage=internal];
// DEFAULT-NEXT:     global %4 ratio: f32 [storage=static] = const<f32>(1.5) [linkage=internal];
// DEFAULT-NEXT:     global %5 total: f64 [storage=static] = const<f64>(2.25) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %20 .str20: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @add_char(%7 a: i8, %8 b: i8) -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i8, reason=return, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%7)), widen<i32, reason=promotion>(read<i8>(%8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @scale(%10 value: f32, %11 factor: f32) -> f32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%10), read<f32>(%11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @add_double(%13 a: f64, %14 b: f64) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%13), read<f64>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i8>(%2, call<i8, signature=fn(i8, i8) -> i8>(%6, read<i8>(%2), truncate<i8, reason=arg, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         call<i8, signature=fn(i8, i8) -> i8>(%6, read<i8>(%2), truncate<i8, reason=arg, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<u8>(%3, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%3))), const<i32>(1)))));
// DEFAULT-NEXT:         write<f32>(%4, call<f32, signature=fn(f32, f32) -> f32>(%9, read<f32>(%4), const<f32>(2.0)));
// DEFAULT-NEXT:         call<f32, signature=fn(f32, f32) -> f32>(%9, read<f32>(%4), const<f32>(2.0));
// DEFAULT-NEXT:         write<f64>(%5, call<f64, signature=fn(f64, f64) -> f64>(%12, read<f64>(%5), float_widen<f64, reason=arg>(read<f32>(%4))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%12, read<f64>(%5), float_widen<f64, reason=arg>(read<f32>(%4)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), widen<i32, reason=vararg>(read<i8>(%2)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)), reinterpret<i32, reason=vararg, fits=unknown>(widen<u32, reason=vararg>(read<u8>(%3))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%19)), float_widen<f64, reason=vararg>(read<f32>(%4)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%20)), read<f64>(%5));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
