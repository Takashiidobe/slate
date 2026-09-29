#pragma STDC FENV_ACCESS ON
double file_scope_static = 1.0 / 3.0;

double fenv_file_scope(double a, double b) { return a / b; }

#pragma STDC FENV_ACCESS DEFAULT

double fenv_nested(double a, double b) {
  double outer = a / b;
  {
#pragma STDC FENV_ACCESS ON
    static double translation_time = 2.0 / 3.0;
    outer += a / b;
    {
#pragma STDC FENV_ACCESS OFF
      outer += a / b;
    }
    outer += a / b;
  }
  return outer + a / b;
}

double contract(double a, double b, double c) {
#pragma STDC FP_CONTRACT OFF
  double off = a * b + c;
  {
#pragma STDC FP_CONTRACT DEFAULT
    off += a * b + c;
  }
  return off;
}

_Complex double limited_range(_Complex double a, _Complex double b) {
  _Complex double full = a / b;
  {
#pragma STDC CX_LIMITED_RANGE ON
    return full + a * b / b;
  }
}

double statement_expression(double a, double b) {
  double inside = ({
#pragma STDC FENV_ACCESS ON
    a / b;
  });
  return inside + a / b;
}

double float_control(double a, double b, double c) {
#pragma float_control(except, on)
  double except = a / b;
  {
#pragma float_control(except, off)
#pragma float_control(precise, off)
    return except + a * b + c;
  }
}

#pragma float_control(except, on, push)
double pushed(double a, double b) { return a / b; }
#pragma float_control(pop)
double popped(double a, double b) { return a / b; }

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
// DEFAULT-NEXT:     global %[[VALUE_file_scope_static:[0-9]+]] file_scope_static: f64 [storage=static] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_translation_time:[0-9]+]] translation_time: f64 [storage=static] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0), const<f64>(3.0)) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_fenv_file_scope:[0-9]+]] @fenv_file_scope(%[[VALUE_a:[0-9]+]] a: f64, %[[VALUE_b:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fenv_nested:[0-9]+]] @fenv_nested(%[[VALUE_a_2:[0-9]+]] a: f64, %[[VALUE_b_2:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_outer:[0-9]+]] outer: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_outer]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: f64 [synthetic] = add<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%[[VALUE0]]), div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:             write<f64>(%[[VALUE_outer]], read<f64>(%[[VALUE1]]));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_outer]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE2]]), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:                 write<f64>(%[[VALUE_outer]], read<f64>(%[[VALUE3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_outer]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: f64 [synthetic] = add<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%[[VALUE4]]), div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:             write<f64>(%[[VALUE_outer]], read<f64>(%[[VALUE5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_outer]]), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_2]]), read<f64>(%[[VALUE_b_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_contract:[0-9]+]] @contract(%[[VALUE_a_3:[0-9]+]] a: f64, %[[VALUE_b_3:[0-9]+]] b: f64, %[[VALUE_c:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_off:[0-9]+]] off: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%[[VALUE_a_3]]), read<f64>(%[[VALUE_b_3]])), read<f64>(%[[VALUE_c]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE6:[0-9]+]]: f64 [synthetic] = read<f64>(%[[VALUE_off]]);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE6]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_3]]), read<f64>(%[[VALUE_b_3]])), read<f64>(%[[VALUE_c]])));
// DEFAULT-NEXT:             write<f64>(%[[VALUE_off]], read<f64>(%[[VALUE7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_off]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_limited_range:[0-9]+]] @limited_range(%[[VALUE_a_4:[0-9]+]] a: complex<f64>, %[[VALUE_b_4:[0-9]+]] b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_full:[0-9]+]] full: complex<f64> [storage=automatic] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%[[VALUE_a_4]]), read<complex<f64>>(%[[VALUE_b_4]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_full]]), div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%[[VALUE_a_4]]), read<complex<f64>>(%[[VALUE_b_4]])), read<complex<f64>>(%[[VALUE_b_4]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_statement_expression:[0-9]+]] @statement_expression(%[[VALUE_a_5:[0-9]+]] a: f64, %[[VALUE_b_5:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_inside:[0-9]+]] inside: f64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: f64 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<f64>(%[[VALUE8]], div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a_5]]), read<f64>(%[[VALUE_b_5]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<f64>(%[[VALUE_inside]], read<f64>(%[[VALUE8]]));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_inside]]), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_5]]), read<f64>(%[[VALUE_b_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_float_control:[0-9]+]] @float_control(%[[VALUE_a_6:[0-9]+]] a: f64, %[[VALUE_b_6:[0-9]+]] b: f64, %[[VALUE_c_2:[0-9]+]] c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_except:[0-9]+]] except: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a_6]]), read<f64>(%[[VALUE_b_6]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_except]]), mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%[[VALUE_a_6]]), read<f64>(%[[VALUE_b_6]]))), read<f64>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_pushed:[0-9]+]] @pushed(%[[VALUE_a_7:[0-9]+]] a: f64, %[[VALUE_b_7:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=observable, contract=on>(read<f64>(%[[VALUE_a_7]]), read<f64>(%[[VALUE_b_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_popped:[0-9]+]] @popped(%[[VALUE_a_8:[0-9]+]] a: f64, %[[VALUE_b_8:[0-9]+]] b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%[[VALUE_a_8]]), read<f64>(%[[VALUE_b_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
