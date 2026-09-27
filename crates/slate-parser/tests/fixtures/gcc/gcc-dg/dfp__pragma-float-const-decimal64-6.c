/* { dg-do compile } */
/* { dg-options "-std=c99 -pedantic-errors" } */

/* N1312 7.1.1: The FLOAT_CONST_DECIMAL64 pragma.
   C99 6.4.4.2a (New).

   Check that there is a pedantic error for the use of pragma
   STD FLOAT_CONST_DECIMAL64.  */

double a;

void f1 (void)
{
#pragma STDC FLOAT_CONST_DECIMAL64 ON		/* { dg-error "ISO C" } */
  a = 1.0;
}

void f2 (void)
{
#pragma STDC FLOAT_CONST_DECIMAL64 OFF		/* { dg-error "ISO C" } */
  a = 2.0;
}

void f3 (void)
{
#pragma STDC FLOAT_CONST_DECIMAL64 DEFAULT	/* { dg-error "ISO C" } */
  a = 3.0;
}

void f4 (void)
{
  _Pragma ("STDC FLOAT_CONST_DECIMAL64 ON")	/* { dg-error "ISO C" } */
  a = 1.0;
}

void f5 (void)
{
  _Pragma ("STDC FLOAT_CONST_DECIMAL64 OFF")	/* { dg-error "ISO C" } */
  a = 2.0;
}

void f6 (void)
{
  _Pragma ("STDC FLOAT_CONST_DECIMAL64 DEFAULT") /* { dg-error "ISO C" } */
  a = 3.0;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     global %0 a: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%0, const<f64>(1.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @f2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%0, const<f64>(2.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f3() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%0, const<f64>(3.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f4() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%0, const<f64>(1.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f5() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%0, const<f64>(2.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f6() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%0, const<f64>(3.0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
