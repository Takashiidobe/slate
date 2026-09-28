#include <stdio.h>

int main(void) {
  volatile double x = 0x1.fffffffffffffp+0;
  double          y = x;
  double          z = -(x * x);
  double          contracted;
  double          uncontracted;

  {
#pragma STDC FP_CONTRACT ON
    contracted = x * y + z;
  }

  {
#pragma STDC FP_CONTRACT OFF
    uncontracted = x * y + z;
  }

  printf("%.20e %.20e\n", contracted, uncontracted);
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
// DEFAULT-NEXT:     global %9 .str9: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 46, 50, 48, 101, 32, 37, 46, 50, 48, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%8 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %3 x: volatile f64 [storage=automatic] = const<f64>(1.9999999999999998);
// DEFAULT-NEXT:         let %4 y: f64 [storage=automatic] = read<f64, volatile>(%3);
// DEFAULT-NEXT:         let %5 z: f64 [storage=automatic] = neg<f64>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64, volatile>(%3), read<f64, volatile>(%3)));
// DEFAULT-NEXT:         let %6 contracted: f64 [storage=automatic];
// DEFAULT-NEXT:         let %7 uncontracted: f64 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<f64>(%6, add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64, volatile>(%3), read<f64>(%4)), read<f64>(%5)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<f64>(%7, add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64, volatile>(%3), read<f64>(%4)), read<f64>(%5)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%9)), read<f64>(%6), read<f64>(%7));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
