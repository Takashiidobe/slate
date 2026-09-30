/* PR tree-optimization/107879 */

__attribute__((noipa)) static double foo(double *y) {
  volatile int    ph     = 0;
  volatile double vf     = 1.0;
  double          factor = vf;
  double          x      = -(double)ph * factor;
  if (x == 0)
    *y = 1.0;
  else
    *y = 1.0 / x;
  double w    = 2.0 * x / factor;
  double omww = 1 - w;
  return omww > 0.0 ? omww : 0.0;
}

int main() {
  double y = 42.0;
  if (foo(&y) != 1.0)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_y:[0-9]+]] y: ptr<f64>) -> f64 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ph:[0-9]+]] ph: volatile i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_vf:[0-9]+]] vf: volatile f64 [storage=automatic] = const<f64>(1.0);
// DEFAULT-NEXT:         let %[[VALUE_factor:[0-9]+]] factor: f64 [storage=automatic] = read<f64, volatile>(%[[VALUE_vf]]);
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(neg<f64>(int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32, volatile>(%[[VALUE_ph]]))), read<f64>(%[[VALUE_factor]]));
// DEFAULT-NEXT:         if eq<f64, exceptions=ignore>(read<f64>(%[[VALUE_x]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)))
// DEFAULT-NEXT:             write<f64>(deref(read<ptr<f64>>(%[[VALUE_y]])), const<f64>(1.0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<f64>(deref(read<ptr<f64>>(%[[VALUE_y]])), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), read<f64>(%[[VALUE_x]])));
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0), read<f64>(%[[VALUE_x]])), read<f64>(%[[VALUE_factor]]));
// DEFAULT-NEXT:         let %[[VALUE_omww:[0-9]+]] omww: f64 [storage=automatic] = sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), read<f64>(%[[VALUE_w]]));
// DEFAULT-NEXT:         return conditional<f64>(gt<f64, exceptions=ignore>(read<f64>(%[[VALUE_omww]]), const<f64>(0.0)), read<f64>(%[[VALUE_omww]]), const<f64>(0.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: f64 [storage=automatic] = const<f64>(42.0);
// DEFAULT-NEXT:         if ne<f64, exceptions=ignore>(call<f64, signature=fn(ptr<f64>) -> f64>(%[[VALUE_foo]], addr_of<ptr<f64>>(%[[VALUE_y_2]])), const<f64>(1.0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
