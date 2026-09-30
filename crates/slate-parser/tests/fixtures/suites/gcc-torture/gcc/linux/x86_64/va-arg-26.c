#include <stdarg.h>

void abort(void);
void exit(int);

double f(float f1, float f2, float f3, float f4, float f5, float f6, ...) {
  va_list ap;
  double  d;

  va_start(ap, f6);
  d = va_arg(ap, double);
  va_end(ap);
  return d;
}

int main() {
  if (f(1, 2, 3, 4, 5, 6, 7.0) != 7.0)
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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_f1:[0-9]+]] f1: f32, %[[VALUE_f2:[0-9]+]] f2: f32, %[[VALUE_f3:[0-9]+]] f3: f32, %[[VALUE_f4:[0-9]+]] f4: f32, %[[VALUE_f5:[0-9]+]] f5: f32, %[[VALUE_f6:[0-9]+]] f6: f32, ...) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], va_arg<f64>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_d]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<f64, exceptions=observable>(call<f64, signature=fn(f32, f32, f32, f32, f32, f32, ...) -> f64>(%[[VALUE_f]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(2)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(3)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(4)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(5)), int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(6)), const<f64>(7.0)), const<f64>(7.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
