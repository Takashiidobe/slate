/* Test for -Wtraditional warnings on conversions by prototypes.
   Note, gcc should omit these warnings in system header files.
   Based on gcc.dg/wtr-conversion-1.c  */

/* { dg-do compile } */
/* { dg-options "-Wtraditional-conversion" } */

extern void foo_i (int);
extern void foo_f (float);
extern void foo_ld (long double);
extern void foo_d32 (_Decimal32);
extern void foo_d64 (_Decimal64);
extern void foo_d128 (_Decimal128);

extern int i;
extern float f;
extern long double ld;
extern _Decimal32 d32;
extern _Decimal64 d64;
extern _Decimal128 d128;

void
testfunc1 ()
{
  foo_i (i);
  foo_i (d32); /* { dg-warning "as integer rather than floating" "prototype conversion warning" } */
  foo_i (d64); /* { dg-warning "as integer rather than floating" "prototype conversion warning" } */
  foo_i (d128); /* { dg-warning "as integer rather than floating" "prototype conversion warning" } */
  foo_d32 (i); /* { dg-warning "as floating rather than integer" "prototype conversion warning" } */
  foo_d32 (f); /* { dg-warning "as '_Decimal32' rather than 'float'" "prototype conversion warning" } */
  foo_d32 (ld); /* { dg-warning "as '_Decimal32' rather than 'long double'" "prototype conversion warning" } */
  foo_d32 (d64); /* { dg-warning "as '_Decimal32' rather than '_Decimal64'" "prototype conversion warning" } */
  foo_d32 (d128); /* { dg-warning "as '_Decimal32' rather than '_Decimal128'" "prototype conversion warning" } */
  foo_d64 (i); /* { dg-warning "as floating rather than integer" "prototype conversion warning" } */
  foo_d64 (f); /* { dg-warning "as '_Decimal64' rather than 'float'" "prototype conversion warning" } */
  foo_d64 (ld); /* { dg-warning "as '_Decimal64' rather than 'long double'" "prototype conversion warning" } */
  foo_d64 (d32); /* { dg-bogus "as '_Decimal64' rather than '_Decimal32'" "prototype conversion warning" } */
  foo_d64 (d128); /* { dg-warning "as '_Decimal64' rather than '_Decimal128'" "prototype conversion warning" } */
  foo_d128 (i); /* { dg-warning "as floating rather than integer" "prototype conversion warning" } */
  foo_d128 (f); /* { dg-warning "as '_Decimal128' rather than 'float'" "prototype conversion warning" } */
  foo_d128 (ld); /* { dg-warning "as '_Decimal128' rather than 'long double'" "prototype conversion warning" } */
  foo_d128 (d32); /* { dg-bogus "as '_Decimal128' rather than '_Decimal32'" "prototype conversion warning" } */
  foo_d128 (d64); /* { dg-bogus "as '_Decimal128' rather than '_Decimal64'" "prototype conversion warning" } */
  foo_d128 (d128); /* { dg-bogus "as '_Decimal128' rather than '_Decimal'" "prototype conversion warning" } */
}
  
# 54 "sys-header.h" 3
/* We are in system headers now, no -Wtraditional warnings should issue.  */

void
testfunc2 ()
{
  foo_i (i);
  foo_i (d32);
  foo_i (d64);
  foo_i (d128);
  foo_d32 (i);
  foo_d32 (f);
  foo_d32 (ld);
  foo_d32 (d32);
  foo_d32 (d64);
  foo_d32 (d128);
  foo_d64 (i);
  foo_d64 (f);
  foo_d64 (ld);
  foo_d64 (d32);
  foo_d64 (d64);
  foo_d64 (d128);
  foo_d128 (i);
  foo_d128 (f);
  foo_d128 (ld);
  foo_d128 (d32);
  foo_d128 (d64);
  foo_d128 (d128);
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
// DEFAULT-NEXT:     extern %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_f:[0-9]+]] f: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_ld:[0-9]+]] ld: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_d32:[0-9]+]] d32: d32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_d64:[0-9]+]] d64: d64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_d128:[0-9]+]] d128: d128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_i:[0-9]+]] @foo_i(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_f:[0-9]+]] @foo_f(%[[VALUE1:[0-9]+]] <unnamed>: f32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_ld:[0-9]+]] @foo_ld(%[[VALUE2:[0-9]+]] <unnamed>: f80) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_d32:[0-9]+]] @foo_d32(%[[VALUE3:[0-9]+]] <unnamed>: d32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_d64:[0-9]+]] @foo_d64(%[[VALUE4:[0-9]+]] <unnamed>: d64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo_d128:[0-9]+]] @foo_d128(%[[VALUE5:[0-9]+]] <unnamed>: d128) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_testfunc1:[0-9]+]] @testfunc1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo_i]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo_i]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d32>(%[[VALUE_d32]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo_i]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_d64]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo_i]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d128]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], float_convert<d32, reason=arg, rounding=nearest_even, exceptions=observable>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], float_convert<d32, reason=arg, rounding=nearest_even, exceptions=observable>(read<f80>(%[[VALUE_ld]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], float_narrow<d32, reason=arg, rounding=nearest_even, exceptions=observable>(read<d64>(%[[VALUE_d64]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], float_narrow<d32, reason=arg, rounding=nearest_even, exceptions=observable>(read<d128>(%[[VALUE_d128]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], int_to_float<d64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], float_convert<d64, reason=arg, rounding=nearest_even, exceptions=observable>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], float_convert<d64, reason=arg, rounding=nearest_even, exceptions=observable>(read<f80>(%[[VALUE_ld]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], float_widen<d64, reason=arg>(read<d32>(%[[VALUE_d32]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], float_narrow<d64, reason=arg, rounding=nearest_even, exceptions=observable>(read<d128>(%[[VALUE_d128]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], int_to_float<d128, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], float_convert<d128, reason=arg, rounding=nearest_even, exceptions=observable>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], float_convert<d128, reason=arg, rounding=nearest_even, exceptions=observable>(read<f80>(%[[VALUE_ld]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], float_widen<d128, reason=arg>(read<d32>(%[[VALUE_d32]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], float_widen<d128, reason=arg>(read<d64>(%[[VALUE_d64]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], read<d128>(%[[VALUE_d128]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_testfunc2:[0-9]+]] @testfunc2() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo_i]], read<i32>(%[[VALUE_i]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo_i]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d32>(%[[VALUE_d32]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo_i]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d64>(%[[VALUE_d64]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_foo_i]], float_to_int<i32, reason=arg, out_of_range=ub, exceptions=observable>(read<d128>(%[[VALUE_d128]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], int_to_float<d32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], float_convert<d32, reason=arg, rounding=nearest_even, exceptions=observable>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], float_convert<d32, reason=arg, rounding=nearest_even, exceptions=observable>(read<f80>(%[[VALUE_ld]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], read<d32>(%[[VALUE_d32]]));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], float_narrow<d32, reason=arg, rounding=nearest_even, exceptions=observable>(read<d64>(%[[VALUE_d64]])));
// DEFAULT-NEXT:         call<void, signature=fn(d32) -> void>(%[[VALUE_foo_d32]], float_narrow<d32, reason=arg, rounding=nearest_even, exceptions=observable>(read<d128>(%[[VALUE_d128]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], int_to_float<d64, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], float_convert<d64, reason=arg, rounding=nearest_even, exceptions=observable>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], float_convert<d64, reason=arg, rounding=nearest_even, exceptions=observable>(read<f80>(%[[VALUE_ld]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], float_widen<d64, reason=arg>(read<d32>(%[[VALUE_d32]])));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], read<d64>(%[[VALUE_d64]]));
// DEFAULT-NEXT:         call<void, signature=fn(d64) -> void>(%[[VALUE_foo_d64]], float_narrow<d64, reason=arg, rounding=nearest_even, exceptions=observable>(read<d128>(%[[VALUE_d128]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], int_to_float<d128, reason=arg, exact=true, rounding=nearest_even, exceptions=observable>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], float_convert<d128, reason=arg, rounding=nearest_even, exceptions=observable>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], float_convert<d128, reason=arg, rounding=nearest_even, exceptions=observable>(read<f80>(%[[VALUE_ld]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], float_widen<d128, reason=arg>(read<d32>(%[[VALUE_d32]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], float_widen<d128, reason=arg>(read<d64>(%[[VALUE_d64]])));
// DEFAULT-NEXT:         call<void, signature=fn(d128) -> void>(%[[VALUE_foo_d128]], read<d128>(%[[VALUE_d128]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
