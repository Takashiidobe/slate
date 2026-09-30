#include <fenv.h>
#include <stdio.h>

int main(void) {
  volatile double x = 3.0;
  volatile double y = 7.0;
  double          before;
  double          contended;
  double          after;

  before = x + y;

  fesetround(FE_DOWNWARD);
  {
#pragma STDC FENV_ACCESS ON
    contended = x / y;
  }
  fesetround(FE_TONEAREST);

  after = x + y;

  printf("%.20e %.20e %.20e\n", before, contended, after);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 19> [storage=static] = code_units<array<i8, 19>>([37, 46, 50, 48, 101, 32, 37, 46, 50, 48, 101, 32, 37, 46, 50, 48, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_fesetround:[0-9]+]] @fesetround(%[[VALUE___rounding_direction:[0-9]+]] __rounding_direction: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: volatile f64 [storage=automatic] = const<f64>(3.0);
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: volatile f64 [storage=automatic] = const<f64>(7.0);
// DEFAULT-NEXT:         let %[[VALUE_before:[0-9]+]] before: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_contended:[0-9]+]] contended: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_after:[0-9]+]] after: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%[[VALUE_before]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64, volatile>(%[[VALUE_x]]), read<f64, volatile>(%[[VALUE_y]])));
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_fesetround]], const<i32>(1024));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<f64>(%[[VALUE_contended]], div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64, volatile>(%[[VALUE_x]]), read<f64, volatile>(%[[VALUE_y]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<i32, signature=fn(i32) -> i32>(%[[VALUE_fesetround]], const<i32>(0));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_after]], add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64, volatile>(%[[VALUE_x]]), read<f64, volatile>(%[[VALUE_y]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(19)>(%[[VALUE_str]])), read<f64>(%[[VALUE_before]]), read<f64>(%[[VALUE_contended]]), read<f64>(%[[VALUE_after]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
