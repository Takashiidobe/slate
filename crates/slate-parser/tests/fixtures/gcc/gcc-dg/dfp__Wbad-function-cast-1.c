/* Test operation of -Wbad-function-cast.  */
/* Based on gcc.dg/Wbad-function-cast-1.c.  */

/* { dg-do compile } */
/* { dg-options "-Wbad-function-cast" } */

int if1(void);
char if2(void);
long if3(void);
float rf1(void);
double rf2(void);
_Decimal32 rf3(void);
_Decimal64 rf4(void);
_Decimal128 rf5(void);
_Complex double cf(void);

void
foo(void)
{
  /* Casts to void types are always OK.  */
  (void)rf3();
  (void)rf4();
  (void)rf5();
  (const void)rf3();
  /* Casts to the same type or similar types are OK.  */
  (_Decimal32)rf1();
  (_Decimal64)rf2();
  (_Decimal128)rf3();
  (_Decimal128)rf4();
  (_Decimal128)rf5();
  (float)rf3();
  (double)rf4();
  (long double)rf5();
   /* Casts to types with different TREE_CODE (which is how this
     warning has been defined) are not OK, except for casts to void
     types.  */
  (_Decimal32)if1(); /* { dg-warning "cast from function call of type 'int' to non-matching type '_Decimal32'" } */
  (_Decimal64)if2(); /* { dg-warning "cast from function call of type 'char' to non-matching type '_Decimal64'" } */
  (_Decimal128)if3(); /* { dg-warning "cast from function call of type 'long int' to non-matching type '_Decimal128'" } */
  (int)rf3(); /* { dg-warning "cast from function call of type '_Decimal32' to non-matching type 'int'" } */
  (long)rf4(); /* { dg-warning "cast from function call of type '_Decimal64' to non-matching type 'long int'" } */
  (long int)rf5(); /* { dg-warning "cast from function call of type '_Decimal128' to non-matching type 'long int'" } */
  (_Decimal32)cf(); /* { dg-warning "cast from function call of type 'complex double' to non-matching type '_Decimal32'" } */
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @if1() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @if2() -> i8 [linkage=external];
// DEFAULT-NEXT:     fn %2 @if3() -> i64 [linkage=external];
// DEFAULT-NEXT:     fn %3 @rf1() -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @rf2() -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @rf3() -> d32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @rf4() -> d64 [linkage=external];
// DEFAULT-NEXT:     fn %7 @rf5() -> d128 [linkage=external];
// DEFAULT-NEXT:     fn %8 @cf() -> complex<f64> [linkage=external] [abi=sysv64() -> coerce<f64, f64>];
// DEFAULT-NEXT:     fn %9 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<d32, signature=fn() -> d32>(%5);
// DEFAULT-NEXT:         call<d64, signature=fn() -> d64>(%6);
// DEFAULT-NEXT:         call<d128, signature=fn() -> d128>(%7);
// DEFAULT-NEXT:         call<d32, signature=fn() -> d32>(%5);
// DEFAULT-NEXT:         float_convert<d32, reason=explicit, rounding=nearest_even, exceptions=observable>(call<f32, signature=fn() -> f32>(%3));
// DEFAULT-NEXT:         float_convert<d64, reason=explicit, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn() -> f64>(%4));
// DEFAULT-NEXT:         float_widen<d128, reason=explicit>(call<d32, signature=fn() -> d32>(%5));
// DEFAULT-NEXT:         float_widen<d128, reason=explicit>(call<d64, signature=fn() -> d64>(%6));
// DEFAULT-NEXT:         call<d128, signature=fn() -> d128>(%7);
// DEFAULT-NEXT:         float_convert<f32, reason=explicit, rounding=nearest_even, exceptions=observable>(call<d32, signature=fn() -> d32>(%5));
// DEFAULT-NEXT:         float_convert<f64, reason=explicit, rounding=nearest_even, exceptions=observable>(call<d64, signature=fn() -> d64>(%6));
// DEFAULT-NEXT:         float_convert<f80, reason=explicit, rounding=nearest_even, exceptions=observable>(call<d128, signature=fn() -> d128>(%7));
// DEFAULT-NEXT:         int_to_float<d32, reason=explicit, exact=false, rounding=nearest_even, exceptions=observable>(call<i32, signature=fn() -> i32>(%0));
// DEFAULT-NEXT:         int_to_float<d64, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(call<i8, signature=fn() -> i8>(%1));
// DEFAULT-NEXT:         int_to_float<d128, reason=explicit, exact=true, rounding=nearest_even, exceptions=observable>(call<i64, signature=fn() -> i64>(%2));
// DEFAULT-NEXT:         float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=observable>(call<d32, signature=fn() -> d32>(%5));
// DEFAULT-NEXT:         float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(call<d64, signature=fn() -> d64>(%6));
// DEFAULT-NEXT:         float_to_int<i64, reason=explicit, out_of_range=ub, exceptions=observable>(call<d128, signature=fn() -> d128>(%7));
// DEFAULT-NEXT:         float_convert<d32, reason=explicit, rounding=nearest_even, exceptions=observable>(complex_to_real<f64, reason=explicit>(call<complex<f64>, signature=fn() -> complex<f64>, abi=sysv64() -> coerce<f64, f64>>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
