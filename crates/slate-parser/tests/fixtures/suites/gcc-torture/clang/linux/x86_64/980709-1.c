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
// DEFAULT-NEXT:     fn %2 @pow(%8 __x: f64, %9 __y: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @exit(%10 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 a: volatile f64 [storage=automatic];
// DEFAULT-NEXT:         let %7 c: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64, volatile>(%6, const<f64>(32.0));
// DEFAULT-NEXT:         write<f64>(%7, call<f64, signature=fn(f64, f64) -> f64>(%2, read<f64, volatile>(%6), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(3.0))));
// DEFAULT-NEXT:         call<f64, signature=fn(f64, f64) -> f64>(%2, read<f64, volatile>(%6), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(3.0)));
// DEFAULT-NEXT:         if logical_and<bool>(gt<f64, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%7), const<f64>(0.1)), const<f64>(3.174802)), lt<f64, exceptions=ignore>(sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%7), const<f64>(0.1)), const<f64>(3.174802)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%4, const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
