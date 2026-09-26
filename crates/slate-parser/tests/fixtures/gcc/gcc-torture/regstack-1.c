void abort(void);
void exit(int);

long double C  = 5;
long double U  = 1;
long double Y2 = 11;
long double Y1 = 17;
long double X, Y, Z, T, R, S;
int         main(void) {
  X  = (C + U) * Y2;
  Y  = C - U - U;
  Z  = C + U + U;
  T  = (C - U) * Y1;
  X  = X - (Z + U);
  R  = Y * Y1;
  S  = Z * Y2;
  T  = T - Y;
  Y  = (U - Y) + R;
  Z  = S - (Z + U + U);
  R  = (Y2 + U) * Y1;
  Y1 = Y2 * Y1;
  R  = R - Y2;
  Y1 = Y1 - 0.5L;
  if (Z != 68. || Y != 49. || X != 58. || Y1 != 186.5 || R != 193. ||
      S != 77. || T != 65. || Y2 != 11.)
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
// DEFAULT-NEXT:     global %2 C: f80 [storage=static] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(5)) [linkage=external];
// DEFAULT-NEXT:     global %3 U: f80 [storage=static] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %4 Y2: f80 [storage=static] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(11)) [linkage=external];
// DEFAULT-NEXT:     global %5 Y1: f80 [storage=static] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(17)) [linkage=external];
// DEFAULT-NEXT:     global %6 X: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 Y: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 Z: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 T: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 R: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 S: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%13 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f80>(%6, mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%2), read<f80>(%3)), read<f80>(%4)));
// DEFAULT-NEXT:         write<f80>(%7, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%2), read<f80>(%3)), read<f80>(%3)));
// DEFAULT-NEXT:         write<f80>(%8, add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%2), read<f80>(%3)), read<f80>(%3)));
// DEFAULT-NEXT:         write<f80>(%9, mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%2), read<f80>(%3)), read<f80>(%5)));
// DEFAULT-NEXT:         write<f80>(%6, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%6), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%8), read<f80>(%3))));
// DEFAULT-NEXT:         write<f80>(%10, mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%7), read<f80>(%5)));
// DEFAULT-NEXT:         write<f80>(%11, mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%8), read<f80>(%4)));
// DEFAULT-NEXT:         write<f80>(%9, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%9), read<f80>(%7)));
// DEFAULT-NEXT:         write<f80>(%7, add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%3), read<f80>(%7)), read<f80>(%10)));
// DEFAULT-NEXT:         write<f80>(%8, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%11), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%8), read<f80>(%3)), read<f80>(%3))));
// DEFAULT-NEXT:         write<f80>(%10, mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%4), read<f80>(%3)), read<f80>(%5)));
// DEFAULT-NEXT:         write<f80>(%5, mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%4), read<f80>(%5)));
// DEFAULT-NEXT:         write<f80>(%10, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%10), read<f80>(%4)));
// DEFAULT-NEXT:         write<f80>(%5, sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%5), const<f80>(0.5)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f80, exceptions=ignore>(read<f80>(%8), float_widen<f80, reason=usual_arith>(const<f64>(68.0))), ne<f80, exceptions=ignore>(read<f80>(%7), float_widen<f80, reason=usual_arith>(const<f64>(49.0)))), ne<f80, exceptions=ignore>(read<f80>(%6), float_widen<f80, reason=usual_arith>(const<f64>(58.0)))), ne<f80, exceptions=ignore>(read<f80>(%5), float_widen<f80, reason=usual_arith>(const<f64>(186.5)))), ne<f80, exceptions=ignore>(read<f80>(%10), float_widen<f80, reason=usual_arith>(const<f64>(193.0)))), ne<f80, exceptions=ignore>(read<f80>(%11), float_widen<f80, reason=usual_arith>(const<f64>(77.0)))), ne<f80, exceptions=ignore>(read<f80>(%9), float_widen<f80, reason=usual_arith>(const<f64>(65.0)))), ne<f80, exceptions=ignore>(read<f80>(%4), float_widen<f80, reason=usual_arith>(const<f64>(11.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
