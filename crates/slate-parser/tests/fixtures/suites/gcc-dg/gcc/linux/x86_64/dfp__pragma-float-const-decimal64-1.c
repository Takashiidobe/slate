/* { dg-do compile } */
/* { dg-options "-Wall" } */

/* N1312 7.1.1: The FLOAT_CONST_DECIMAL64 pragma.
   C99 6.4.4.2a (New).

   Verify that the pragma has the expected result by using unsuffixed
   float constants as operands in expressions that would mix binary and
   decimal operands if the pragma had no effect, or the wrong effect.  */

#pragma STDC FLOAT_CONST_DECIMAL64 ON
double a = 1.0 * 2.0dd;

double
f1 (void)
{
#pragma STDC FLOAT_CONST_DECIMAL64 OFF
  double b = 2.0 * 3.0d;

  {
    double c = 3.0 * 4.0d;
    b = b + c;
  }

  {
#pragma STDC FLOAT_CONST_DECIMAL64 ON
    double d = 4.0 * 5.0dd;

    b = b + d;
  }

  {
     /* Default is OFF.  */
#pragma STDC FLOAT_CONST_DECIMAL64 DEFAULT
     double e = 5.0 * 6.0d;
     b = b + e;
  }

  return b;
}

double
f2 (void)
{
  /* Use value from outer scope, which is ON.  */
  double b = 2.0 * 3.0dd;

  {
#pragma STDC FLOAT_CONST_DECIMAL64 OFF
    double c = 3.0 * 4.0d;

    {
#pragma STDC FLOAT_CONST_DECIMAL64 ON
      double d = 4.0 * 5.0dd;

      {
#pragma STDC FLOAT_CONST_DECIMAL64 DEFAULT
	double e = 5.0 * 6.0d;

	{
#pragma STDC FLOAT_CONST_DECIMAL64 ON
	  double f = 6.0 * 7.0dd;

	  b = a + b + c + d + e + f;
	}
      }
    }
  }
  return b;
}

/* Use previous value from this scope, which is ON.  */
double f = 6.0 * 7.0dd;

double
f3 (void)
{
#pragma STDC FLOAT_CONST_DECIMAL64 OFF
  double b = 2.0 * 3.0d;

  return b + f;
}

/* Return to the state from this scope, which is ON.  */
double g = 7.0 + 8.0dd;

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: f64 [storage=static] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=ignore>(mul<d64, rounding=nearest_even, exceptions=ignore, contract=fast>(const<d64>(1.0), const<d64>(2.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: f64 [storage=static] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=ignore>(mul<d64, rounding=nearest_even, exceptions=ignore, contract=fast>(const<d64>(6.0), const<d64>(7.0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: f64 [storage=static] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=ignore>(add<d64, rounding=nearest_even, exceptions=ignore, contract=fast>(const<d64>(7.0), const<d64>(8.0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(2.0), const<f64>(3.0));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_c:[0-9]+]] c: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(3.0), const<f64>(4.0));
// DEFAULT-NEXT:             write<f64>(%[[VALUE_b]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_b]]), read<f64>(%[[VALUE_c]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_d:[0-9]+]] d: f64 [storage=automatic] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(4.0), const<d64>(5.0)));
// DEFAULT-NEXT:             write<f64>(%[[VALUE_b]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_b]]), read<f64>(%[[VALUE_d]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_e:[0-9]+]] e: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(5.0), const<f64>(6.0));
// DEFAULT-NEXT:             write<f64>(%[[VALUE_b]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_b]]), read<f64>(%[[VALUE_e]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_b]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_2:[0-9]+]] b: f64 [storage=automatic] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(2.0), const<d64>(3.0)));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_c_2:[0-9]+]] c: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(3.0), const<f64>(4.0));
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_d_2:[0-9]+]] d: f64 [storage=automatic] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(4.0), const<d64>(5.0)));
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_e_2:[0-9]+]] e: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(5.0), const<f64>(6.0));
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_f_2:[0-9]+]] f: f64 [storage=automatic] = float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(6.0), const<d64>(7.0)));
// DEFAULT-NEXT:                         write<f64>(%[[VALUE_b_2]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_a]]), read<f64>(%[[VALUE_b_2]])), read<f64>(%[[VALUE_c_2]])), read<f64>(%[[VALUE_d_2]])), read<f64>(%[[VALUE_e_2]])), read<f64>(%[[VALUE_f_2]])));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_b_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3() -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_b_3:[0-9]+]] b: f64 [storage=automatic] = mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(2.0), const<f64>(3.0));
// DEFAULT-NEXT:         return add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE_b_3]]), read<f64>(%[[VALUE_f]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
