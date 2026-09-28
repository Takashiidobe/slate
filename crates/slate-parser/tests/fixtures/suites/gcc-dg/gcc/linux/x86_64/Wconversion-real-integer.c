/* Test for diagnostics for Wconversion between floating-point and
   integers.  */

/* { dg-do compile } */
/* { dg-skip-if "doubles are floats,ints are 16bits" { "avr-*-*" } } */
/* { dg-options "-std=c99 -Wconversion -fno-trapping-math" } */
/* { dg-require-effective-target int32plus } */
/* { dg-require-effective-target double64plus } */
#include <limits.h>

void fsi (signed int x);
void fui (unsigned int x);
void ffloat (float x);
void fdouble (double x);

float  vfloat;
double vdouble;

void h (void)
{
  unsigned int ui = 3;
  int   si = 3;
  unsigned char uc = 3;
  signed char sc = 3;
  float  f = 3;
  double d = 3;

  fsi (3.1f); /* { dg-warning "conversion" } */
  si = 3.1f; /* { dg-warning "conversion" } */
  fsi (3.1);  /* { dg-warning "conversion" } */
  si = 3.1;  /* { dg-warning "conversion" } */
  fsi (d);    /* { dg-warning "conversion" } */
  si = d;    /* { dg-warning "conversion" } */
  fui (-1.0); /* { dg-warning "overflow" } */
  ui = -1.0;   /* { dg-warning "overflow" } */
  ffloat (INT_MAX);  /* { dg-warning "conversion" } */
  vfloat = INT_MAX;  /* { dg-warning "conversion" } */
  ffloat (16777217); /* { dg-warning "conversion" } */
  vfloat = 16777217; /* { dg-warning "conversion" } */
  ffloat (si); /* { dg-warning "conversion" } */
  vfloat = si; /* { dg-warning "conversion" } */
  ffloat (ui); /* { dg-warning "conversion" } */
  vfloat = ui; /* { dg-warning "conversion" } */

  fsi (3);
  si = 3;
  fsi (3.0f);
  si = 3.0f;
  fsi (3.0);
  si = 3.0;
  fsi (16777217.0f);
  si = 16777217.0f;
  fsi ((int) 3.1);
  si = (int) 3.1;
  ffloat (3U);
  vfloat = 3U;
  ffloat (3);
  vfloat = 3;
  ffloat (INT_MIN);
  vfloat = INT_MIN;
  ffloat (uc);
  vfloat = uc;
  ffloat (sc);
  vfloat = sc;

  fdouble (UINT_MAX);
  vdouble = UINT_MAX;
  fdouble (ui);
  vdouble = ui;
  fdouble (si);
  vdouble = si;
}


void fss (signed short x);
void fus (unsigned short x);
void fsc (signed char x);
void fuc (unsigned char x);

void h2 (void)
{
  unsigned short int us;
  short int   ss;
  unsigned char uc;
  signed char sc;
  
  fss (4294967294.0); /* { dg-warning "conversion" } */
  ss = 4294967294.0; /* { dg-warning "conversion" } */
  fss (-4294967294.0);  /* { dg-warning "conversion" } */
  ss = -4294967294.0;  /* { dg-warning "conversion" } */
  fus (4294967294.0); /* { dg-warning "conversion" } */
  us = 4294967294.0; /* { dg-warning "conversion" } */
  fus (-4294967294.0);  /* { dg-warning "conversion" } */
  us = -4294967294.0;  /* { dg-warning "conversion" } */

  fsc (500.0); /* { dg-warning "conversion" } */
  sc = 500.0; /* { dg-warning "conversion" } */
  fsc (-500.0);  /* { dg-warning "conversion" } */
  sc = -500.0;  /* { dg-warning "conversion" } */
  fuc (500.0); /* { dg-warning "conversion" } */
  uc = 500.0; /* { dg-warning "conversion" } */
  fuc (-500.0);  /* { dg-warning "conversion" } */
  uc = -500.0;  /* { dg-warning "conversion" } */

  fss (500.0);
  ss = 500.0;
  fss (-500.0);
  ss = -500.0;
  fus (500.0); 
  us = 500.0; 
  fus (-500.0);   /* { dg-warning "conversion" } */
  us = -500.0;    /* { dg-warning "conversion" } */
}

// SLATE-FILECHECK-STD DEFAULT c99
// SLATE-FILECHECK-ARGS -fno-trapping-math
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
// DEFAULT-NEXT:     global %4 vfloat: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 vdouble: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @fsi(%22 x: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @fui(%23 x: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @ffloat(%24 x: f32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @fdouble(%25 x: f64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @h() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 ui: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(3));
// DEFAULT-NEXT:         let %8 si: i32 [storage=automatic] = const<i32>(3);
// DEFAULT-NEXT:         let %9 uc: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         let %10 sc: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(3));
// DEFAULT-NEXT:         let %11 f: f32 [storage=automatic] = int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3));
// DEFAULT-NEXT:         let %12 d: f64 [storage=automatic] = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(const<f32>(3.1)));
// DEFAULT-NEXT:         write<i32>(%8, float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f32>(3.1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(3.1)));
// DEFAULT-NEXT:         write<i32>(%8, float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(3.1)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(read<f64>(%12)));
// DEFAULT-NEXT:         write<i32>(%8, float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(read<f64>(%12)));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%1, float_to_int<u32, reason=arg, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(1.0))));
// DEFAULT-NEXT:         write<u32>(%7, float_to_int<u32, reason=assign, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(1.0))));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2147483647)));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(2147483647)));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(16777217)));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(16777217)));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%8)));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(read<u32>(%7)));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(read<u32>(%7)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%8, const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(const<f32>(3.0)));
// DEFAULT-NEXT:         write<i32>(%8, float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f32>(3.0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(3.0)));
// DEFAULT-NEXT:         write<i32>(%8, float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(3.0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, float_to_int<i32, reason=arg, out_of_range=ub, exceptions=ignore>(const<f32>(16777216.0)));
// DEFAULT-NEXT:         write<i32>(%8, float_to_int<i32, reason=assign, out_of_range=ub, exceptions=ignore>(const<f32>(16777216.0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(3.1)));
// DEFAULT-NEXT:         write<i32>(%8, float_to_int<i32, reason=explicit, out_of_range=ub, exceptions=ignore>(const<f64>(3.1)));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<u32>(3)));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<u32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(3)));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(sub<i32, overflow=ub>(neg<i32, overflow=ub>(const<i32>(2147483647)), const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(%9)));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u8>(%9)));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%2, int_to_float<f32, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(%10)));
// DEFAULT-NEXT:         write<f32>(%4, int_to_float<f32, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i8>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%3, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))));
// DEFAULT-NEXT:         write<f64>(%5, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%3, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(read<u32>(%7)));
// DEFAULT-NEXT:         write<f64>(%5, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<u32>(%7)));
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%3, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%8)));
// DEFAULT-NEXT:         write<f64>(%5, int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(read<i32>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @fss(%26 x: i16) -> void [linkage=external];
// DEFAULT-NEXT:     fn %14 @fus(%27 x: u16) -> void [linkage=external];
// DEFAULT-NEXT:     fn %15 @fsc(%28 x: i8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %16 @fuc(%29 x: u8) -> void [linkage=external];
// DEFAULT-NEXT:     fn %17 @h2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 us: u16 [storage=automatic];
// DEFAULT-NEXT:         let %19 ss: i16 [storage=automatic];
// DEFAULT-NEXT:         let %20 uc: u8 [storage=automatic];
// DEFAULT-NEXT:         let %21 sc: i8 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%13, float_to_int<i16, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(4294967294.0)));
// DEFAULT-NEXT:         write<i16>(%19, float_to_int<i16, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(4294967294.0)));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%13, float_to_int<i16, reason=arg, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(4294967294.0))));
// DEFAULT-NEXT:         write<i16>(%19, float_to_int<i16, reason=assign, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(4294967294.0))));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%14, float_to_int<u16, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(4294967294.0)));
// DEFAULT-NEXT:         write<u16>(%18, float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(4294967294.0)));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%14, float_to_int<u16, reason=arg, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(4294967294.0))));
// DEFAULT-NEXT:         write<u16>(%18, float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(4294967294.0))));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%15, float_to_int<i8, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(500.0)));
// DEFAULT-NEXT:         write<i8>(%21, float_to_int<i8, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(500.0)));
// DEFAULT-NEXT:         call<void, signature=fn(i8) -> void>(%15, float_to_int<i8, reason=arg, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(500.0))));
// DEFAULT-NEXT:         write<i8>(%21, float_to_int<i8, reason=assign, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(500.0))));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%16, float_to_int<u8, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(500.0)));
// DEFAULT-NEXT:         write<u8>(%20, float_to_int<u8, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(500.0)));
// DEFAULT-NEXT:         call<void, signature=fn(u8) -> void>(%16, float_to_int<u8, reason=arg, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(500.0))));
// DEFAULT-NEXT:         write<u8>(%20, float_to_int<u8, reason=assign, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(500.0))));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%13, float_to_int<i16, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(500.0)));
// DEFAULT-NEXT:         write<i16>(%19, float_to_int<i16, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(500.0)));
// DEFAULT-NEXT:         call<void, signature=fn(i16) -> void>(%13, float_to_int<i16, reason=arg, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(500.0))));
// DEFAULT-NEXT:         write<i16>(%19, float_to_int<i16, reason=assign, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(500.0))));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%14, float_to_int<u16, reason=arg, out_of_range=ub, exceptions=ignore>(const<f64>(500.0)));
// DEFAULT-NEXT:         write<u16>(%18, float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(const<f64>(500.0)));
// DEFAULT-NEXT:         call<void, signature=fn(u16) -> void>(%14, float_to_int<u16, reason=arg, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(500.0))));
// DEFAULT-NEXT:         write<u16>(%18, float_to_int<u16, reason=assign, out_of_range=ub, exceptions=ignore>(neg<f64>(const<f64>(500.0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
