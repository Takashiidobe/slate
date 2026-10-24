// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

_Static_assert(_Generic(1.0, double: 1, default: 0), "default off");
#pragma STDC FLOAT_CONST_DECIMAL64 ON
_Static_assert(_Generic(1.0, _Decimal64: 1, default: 0), "unsuffixed");
_Static_assert(_Generic(1e3, _Decimal64: 1, default: 0), "exponent");
_Static_assert(_Generic(0x1p3, double: 1, default: 0), "hex stays binary");
_Static_assert(_Generic(1.0d, double: 1, default: 0), "d suffix stays double");
_Static_assert(_Generic(1.0f, float: 1, default: 0), "f suffix");
_Static_assert(_Generic(2.0i, _Complex double: 1, default: 0), "imaginary stays binary");
_Static_assert(_Generic(1, int: 1, default: 0), "integer");

_Decimal64 scaled = 1.5 * 2.0dd;

double f(void) {
#pragma STDC FLOAT_CONST_DECIMAL64 OFF
  double b = 2.0 * 3.0d;
  {
    _Static_assert(_Generic(1.0, double: 1, default: 0), "inherited off");
#pragma STDC FLOAT_CONST_DECIMAL64 ON
    _Static_assert(_Generic(1.0, double: 1, default: 0), "misplaced is ignored");
  }
  {
#pragma STDC FLOAT_CONST_DECIMAL64 ON
    _Decimal64 d = 4.0 * 5.0dd;
    b = b + (double)d;
    {
#pragma STDC FLOAT_CONST_DECIMAL64 DEFAULT
      _Static_assert(_Generic(1.0, double: 1, default: 0), "default is off");
    }
    _Static_assert(_Generic(1.0, _Decimal64: 1, default: 0), "restored on");
    b = b + (double)({
#pragma STDC FLOAT_CONST_DECIMAL64 OFF
      2.0 * 3.0d;
    });
  }
  return b;
}

_Static_assert(_Generic(1.0, _Decimal64: 1, default: 0), "file scope restored");

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_scaled:[0-9]+]] scaled: d64 [storage=static] = mul<d64>(const<d64>(1.5), const<d64>(2.0)) [linkage=external];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_b:[0-9]+]] b: f64 [storage=automatic] = mul<f64>(const<f64>(2.0), const<f64>(3.0));
// IR-NEXT:         {
// IR-NEXT:         }
// IR-NEXT:         {
// IR-NEXT:             let %[[VALUE_d:[0-9]+]] d: d64 [storage=automatic] = mul<d64>(const<d64>(4.0), const<d64>(5.0));
// IR-NEXT:             write<f64>(%[[VALUE_b]], add<f64>(read<f64>(%[[VALUE_b]]), float_convert<f64>(read<d64>(%[[VALUE_d]]))));
// IR-NEXT:             {
// IR-NEXT:             }
// IR-NEXT:             let %[[VALUE0:[0-9]+]]: f64 [synthetic];
// IR-NEXT:             {
// IR-NEXT:                 write<f64>(%[[VALUE0]], mul<f64>(const<f64>(2.0), const<f64>(3.0)));
// IR-NEXT:             }
// IR-NEXT:             write<f64>(%[[VALUE_b]], add<f64>(read<f64>(%[[VALUE_b]]), read<f64>(%[[VALUE0]])));
// IR-NEXT:         }
// IR-NEXT:         return read<f64>(%[[VALUE_b]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
