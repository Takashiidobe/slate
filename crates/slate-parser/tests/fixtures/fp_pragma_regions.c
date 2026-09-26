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
// DEFAULT-NEXT:     global %0 file_scope_static: f64 [storage=static] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(3.0)) [linkage=external];
// DEFAULT-NEXT:     global %8 translation_time: f64 [storage=static] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0), const<f64>(3.0)) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @fenv_file_scope(%2 a: f64, %3 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%2), read<f64>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @fenv_nested(%5 a: f64, %6 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 outer: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), read<f64>(%6));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %33: f64 [synthetic] = read<f64>(%7);
// DEFAULT-NEXT:             let %34: f64 [synthetic] = add<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%33), div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%5), read<f64>(%6)));
// DEFAULT-NEXT:             write<f64>(%7, read<f64>(%34));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %35: f64 [synthetic] = read<f64>(%7);
// DEFAULT-NEXT:                 let %36: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%35), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), read<f64>(%6)));
// DEFAULT-NEXT:                 write<f64>(%7, read<f64>(%36));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             let %37: f64 [synthetic] = read<f64>(%7);
// DEFAULT-NEXT:             let %38: f64 [synthetic] = add<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%37), div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%5), read<f64>(%6)));
// DEFAULT-NEXT:             write<f64>(%7, read<f64>(%38));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%7), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), read<f64>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @contract(%10 a: f64, %11 b: f64, %12 c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 off: f64 [storage=automatic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=off>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=off>(read<f64>(%10), read<f64>(%11)), read<f64>(%12));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %39: f64 [synthetic] = read<f64>(%13);
// DEFAULT-NEXT:             let %40: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%39), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%10), read<f64>(%11)), read<f64>(%12)));
// DEFAULT-NEXT:             write<f64>(%13, read<f64>(%40));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f64>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @limited_range(%15 a: complex<f64>, %16 b: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(coerce<f64, f64>, coerce<f64, f64>) -> coerce<f64, f64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %17 full: complex<f64> [storage=automatic] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(read<complex<f64>>(%15), read<complex<f64>>(%16));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             return add<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%17), div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=basic>(read<complex<f64>>(%15), read<complex<f64>>(%16)), read<complex<f64>>(%16)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @statement_expression(%19 a: f64, %20 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %21 inside: f64 [storage=automatic];
// DEFAULT-NEXT:         let %41: f64 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<f64>(%41, div<f64, rounding=environment, exceptions=observable, contract=on>(read<f64>(%19), read<f64>(%20)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<f64>(%21, read<f64>(%41));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%21), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%19), read<f64>(%20)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @float_control(%23 a: f64, %24 b: f64, %25 c: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %26 except: f64 [storage=automatic] = div<f64, rounding=nearest_even, exceptions=observable, contract=on>(read<f64>(%23), read<f64>(%24));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             return add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(add<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%26), mul<f64, rounding=nearest_even, exceptions=ignore, contract=fast>(read<f64>(%23), read<f64>(%24))), read<f64>(%25));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @pushed(%28 a: f64, %29 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=observable, contract=on>(read<f64>(%28), read<f64>(%29));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @popped(%31 a: f64, %32 b: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%31), read<f64>(%32));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
