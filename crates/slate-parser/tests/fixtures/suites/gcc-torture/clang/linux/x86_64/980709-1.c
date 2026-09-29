/* { dg-xfail-if "Can not call system libm.a with -msoft-float" { powerpc-*-aix*
 * rs6000-*-aix* } { "-msoft-float" } { "" } } */
#include <math.h>

void abort(void);
void exit(int);

int main(void) {
  volatile double a;
  double          c;
  a = 32.0;
  c = pow(a, 1.0 / 3.0);
  if (c + 0.1 > 3.174802 && c - 0.1 < 3.174802)
    exit(0);
  else
    abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_pow:[0-9]+]] @pow(%[[VALUE___x:[0-9]+]] __x: f64, %[[VALUE___y:[0-9]+]] __y: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64, volatile>(%[[VALUE_a]], const<f64>(32.0));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_c]], call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64, volatile>(%[[VALUE_a]]), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(3.0))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%[[VALUE_pow]], read<f64, volatile>(%[[VALUE_a]]), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(3.0)));
// DEFAULT-NEXT:         if logical_and<bool>(gt<f64, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_c]]), const<f64>(0.1)), const<f64>(3.174802)), lt<f64, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_c]]), const<f64>(0.1)), const<f64>(3.174802)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
