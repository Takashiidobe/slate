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
// DEFAULT-NEXT:     global %[[VALUE_C:[0-9]+]] C: f80 [storage=static] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(5)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_U:[0-9]+]] U: f80 [storage=static] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Y2:[0-9]+]] Y2: f80 [storage=static] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(11)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Y1:[0-9]+]] Y1: f80 [storage=static] = int_to_float<f80, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(17)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_X:[0-9]+]] X: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Y:[0-9]+]] Y: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_Z:[0-9]+]] Z: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_T:[0-9]+]] T: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_R:[0-9]+]] R: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_S:[0-9]+]] S: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<f80>(%[[VALUE_X]], mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_C]]), read<f80>(%[[VALUE_U]])), read<f80>(%[[VALUE_Y2]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_Y]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_C]]), read<f80>(%[[VALUE_U]])), read<f80>(%[[VALUE_U]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_Z]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_C]]), read<f80>(%[[VALUE_U]])), read<f80>(%[[VALUE_U]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_T]], mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_C]]), read<f80>(%[[VALUE_U]])), read<f80>(%[[VALUE_Y1]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_X]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_X]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_Z]]), read<f80>(%[[VALUE_U]]))));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_R]], mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_Y]]), read<f80>(%[[VALUE_Y1]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_S]], mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_Z]]), read<f80>(%[[VALUE_Y2]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_T]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_T]]), read<f80>(%[[VALUE_Y]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_Y]], add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_U]]), read<f80>(%[[VALUE_Y]])), read<f80>(%[[VALUE_R]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_Z]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_S]]), add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_Z]]), read<f80>(%[[VALUE_U]])), read<f80>(%[[VALUE_U]]))));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_R]], mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_Y2]]), read<f80>(%[[VALUE_U]])), read<f80>(%[[VALUE_Y1]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_Y1]], mul<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_Y2]]), read<f80>(%[[VALUE_Y1]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_R]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_R]]), read<f80>(%[[VALUE_Y2]])));
// DEFAULT-NEXT:         write<f80>(%[[VALUE_Y1]], sub<f80, rounding=nearest_even, exceptions=ignore, contract=on>(read<f80>(%[[VALUE_Y1]]), const<f80>(0.5)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_Z]]), float_widen<f80, reason=usual_arith>(const<f64>(68.0))), ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_Y]]), float_widen<f80, reason=usual_arith>(const<f64>(49.0)))), ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_X]]), float_widen<f80, reason=usual_arith>(const<f64>(58.0)))), ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_Y1]]), float_widen<f80, reason=usual_arith>(const<f64>(186.5)))), ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_R]]), float_widen<f80, reason=usual_arith>(const<f64>(193.0)))), ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_S]]), float_widen<f80, reason=usual_arith>(const<f64>(77.0)))), ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_T]]), float_widen<f80, reason=usual_arith>(const<f64>(65.0)))), ne<f80, exceptions=ignore>(read<f80>(%[[VALUE_Y2]]), float_widen<f80, reason=usual_arith>(const<f64>(11.0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
