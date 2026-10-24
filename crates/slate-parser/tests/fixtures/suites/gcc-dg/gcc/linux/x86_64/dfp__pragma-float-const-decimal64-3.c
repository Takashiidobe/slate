/* { dg-do compile } */
/* { dg-options "-Wall" } */

/* N1312 7.1.1: The FLOAT_CONST_DECIMAL64 pragma.
   C99 6.4.4.2a (New).  */

/* Check that defining macros whose names are the same as the tokens used
   in the pragma doesn't affect use of the pragma.  */

#define ON YES
#define OFF NO
#define DEFAULT NOPE
#define STDC OFFICIAL
#define FLOAT_CONST_DECIMAL64 NEW_PRAGMA

double a;

void
f1a (void)
{
#pragma STDC FLOAT_CONST_DECIMAL64 ON
  a = 1.0dd + 2.0;
}

void
f1b (void)
{
#pragma STDC FLOAT_CONST_DECIMAL64 OFF
  a = 2.0d + 3.0;
}

void
f1c (void)
{
#pragma STDC FLOAT_CONST_DECIMAL64 DEFAULT
  a = 3.0d + 4.0;
}

/* Check that a macro can be used for the entire pragma.  */

#define PRAGMA(x) _Pragma (#x)
#define DEFAULT_FLOAT_IS_DECIMAL PRAGMA(STDC FLOAT_CONST_DECIMAL64 ON)
#define DEFAULT_FLOAT_IS_BINARY PRAGMA(STDC FLOAT_CONST_DECIMAL64 OFF)

void
f2a (void)
{
  DEFAULT_FLOAT_IS_DECIMAL
  a = 5.0 * 6.0dd;
}

void
f2b (void)
{
  DEFAULT_FLOAT_IS_BINARY
  a = 6.0 * 7.0d;
}

/* _Pragma can be used with macros, including the use of a macro for the
    switch.  */

#undef ON
#undef OFF
#undef DEFAULT
#undef STDC
#undef FLOAT_CONST_DECIMAL64

#define SWITCH ON
#define FLOAT_CONST_DECIMAL64(x) PRAGMA(STDC FLOAT_CONST_DECIMAL64 x)

void
f3a (void)
{
  FLOAT_CONST_DECIMAL64(SWITCH)
  a = 1.0 * 7.0dd;
}

void
f3b (void)
{
  FLOAT_CONST_DECIMAL64(OFF)
  a = 1.0 + 2.0d;
}

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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1a:[0-9]+]] @f1a() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_a]], float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(add<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(1.0), const<d64>(2.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1b:[0-9]+]] @f1b() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_a]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(2.0), const<f64>(3.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1c:[0-9]+]] @f1c() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_a]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(3.0), const<f64>(4.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2a:[0-9]+]] @f2a() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_a]], float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(5.0), const<d64>(6.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2b:[0-9]+]] @f2b() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_a]], mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(6.0), const<f64>(7.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3a:[0-9]+]] @f3a() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_a]], float_convert<f64, reason=assign, rounding=nearest_even, exceptions=observable>(mul<d64, rounding=nearest_even, exceptions=observable, contract=fast>(const<d64>(1.0), const<d64>(7.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3b:[0-9]+]] @f3b() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_a]], add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(const<f64>(1.0), const<f64>(2.0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
