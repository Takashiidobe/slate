/* { dg-options "-mpc64"  { target { i?86-*-* x86_64-*-* } } } */

extern void abort(void);
extern void exit(int);

void fpEq(double x, double y) {
  if (x != y)
    abort();
}

void fpTest(double x, double y) {
  double result1 = (35.7 * 100.0) / 45.0;
  double result2 = (x * 100.0) / y;
  fpEq(result1, result2);
}

int main() {
  fpTest(35.7, 45.0);
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
// DEFAULT-NEXT:     fn %[[VALUE_fpEq:[0-9]+]] @fpEq(%[[VALUE_x:[0-9]+]] x: f64, %[[VALUE_y:[0-9]+]] y: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(read<f64>(%[[VALUE_x]]), read<f64>(%[[VALUE_y]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fpTest:[0-9]+]] @fpTest(%[[VALUE_x_2:[0-9]+]] x: f64, %[[VALUE_y_2:[0-9]+]] y: f64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_result1:[0-9]+]] result1: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(35.7), const<f64>(100.0)), const<f64>(45.0));
// DEFAULT-NEXT:         let %[[VALUE_result2:[0-9]+]] result2: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_x_2]]), const<f64>(100.0)), read<f64>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_fpEq]], read<f64>(%[[VALUE_result1]]), read<f64>(%[[VALUE_result2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, f64) -> void>(%[[VALUE_fpTest]], const<f64>(35.7), const<f64>(45.0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
