#include <stdio.h>

int main(void) {
  float  x = 1.5f;
  double y = 2.25;
  printf("%f %.2f %.0f\n", x + 0.5f, y * 2.0, 3.0);
  printf("tail %.3f", 1.25);
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
// DEFAULT-NEXT:     global %6 .str6: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([37, 102, 32, 37, 46, 50, 102, 32, 37, 46, 48, 102, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([116, 97, 105, 108, 32, 37, 46, 51, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%5 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 x: f32 [storage=automatic] = const<f32>(1.5);
// DEFAULT-NEXT:         let %4 y: f64 [storage=automatic] = const<f64>(2.25);
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%6)), float_widen<f64, reason=vararg>(add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%3), const<f32>(0.5))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%4), const<f64>(2.0)), const<f64>(3.0));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%7)), const<f64>(1.25));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
