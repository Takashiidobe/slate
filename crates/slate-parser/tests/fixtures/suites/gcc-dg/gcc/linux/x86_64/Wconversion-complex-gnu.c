/* PR c/48956: Test for diagnostics for implicit conversions involving complex
   types.  See also Wconversion-complex-c99.c.

   These tests cover integer complex values (which are GNU extensions).  */

/* { dg-do compile } */
/* { dg-skip-if "doubles are floats,ints are 16bits" { "avr-*-*" } } */
/* { dg-options " -std=gnu99 -Wconversion " } */
/* { dg-require-effective-target int32plus } */
/* { dg-require-effective-target double64plus } */

#include <limits.h>

void fsi (int);
void fui (unsigned);
void ffloat (float);
int vsi;
unsigned int vui;
float vfloat;

void fsic (int _Complex);
void fuic (unsigned _Complex);
void ffloatc (float _Complex);
int _Complex vsic;
unsigned _Complex vuic;
float _Complex vfloatc;

/* Check implicit conversions of float complex-domain values to integer
   complex-domain types.  */
void
var_float_to_int (void)
{
  double _Complex doublec = 0.;

  fsic (doublec); /* { dg-warning "conversion" } */
  fuic (doublec); /* { dg-warning "conversion" } */

  vsic = doublec; /* { dg-warning "conversion" } */
  vuic = doublec; /* { dg-warning "conversion" } */
}

/* Check implicit conversions of integer complex-domain values to integer
   real-domain types.  */
void
var_complex_to_real (void)
{
  int _Complex ic = 0;
  unsigned _Complex uc = 0;
  unsigned long long _Complex ullc = 0;

  fsic (ic);
  fuic (uc);
  vsic = ic;
  vuic = uc;

  fsi (ic); /* { dg-warning "conversion" } */
  vsi = ic; /* { dg-warning "conversion" } */
  fui (uc); /* { dg-warning "conversion" } */
  vui = uc; /* { dg-warning "conversion" } */

  fuic (ullc); /* { dg-warning "conversion" } */
  vuic = ullc; /* { dg-warning "conversion" } */

  fui (ic); /* { dg-warning "conversion" } */
  vui = ic; /* { dg-warning "conversion" } */
}

/* Check implicit conversions of float complex-domain constants to integer
   types.  */
void
const_float_to_int (void)
{
  fsic (1. - 1.i);
  fuic (1. + 1.i);
  vsic = 1. - 1.i;
  vuic = 1. + 1.i;

  fsic (0.5 + 0.i); /* { dg-warning "conversion" } */
  vsic = 0.5 + 0.i; /* { dg-warning "conversion" } */
  fuic (0.5 + 0.i); /* { dg-warning "conversion" } */
}

/* Check implicit conversions of integer complex-domain constants to integer
   types.  */
void
const_complex_int_to_real_int (void)
{
  fsi (-1 + 0i);
  fui (1 + 0i);
  vsi = -1 + 0i;
  vui = 1 + 0i;

  fui (1 + 1i); /* { dg-warning "conversion" } */
  vui = 1 + 1i; /* { dg-warning "conversion" } */

  fui (UINT_MAX + 1ull + 0i); /* { dg-warning "conversion" } */
  vui = UINT_MAX + 1ull + 0i; /* { dg-warning "conversion" } */

  ffloat (UINT_MAX + 0i); /* { dg-warning "conversion" } */
  vfloat = UINT_MAX + 0i; /* { dg-warning "conversion" } */
}

void
const_complex_int_narrowing (void)
{
  fsic (1 - 1i);
  fuic (1 + 1i);
  vsic = 1 - 1i;
  vuic = 1 + 1i;

  fuic (UINT_MAX + 1ull + 1i); /* { dg-warning "conversion" } */
  fuic ((UINT_MAX + 1ull) * 1i); /* { dg-warning "conversion" } */
  fuic ((UINT_MAX + 1ull) + (UINT_MAX + 1ull) * 1i); /* { dg-warning "conversion" } */

  vuic = (UINT_MAX + 1ull) * 1i; /* { dg-warning "conversion" } */
  vuic = (UINT_MAX + 1ull) + 1i; /* { dg-warning "conversion" } */
  vuic = (UINT_MAX + 1ull) + (UINT_MAX + 1ull) * 1i; /* { dg-warning "conversion" } */

  ffloatc (UINT_MAX * 1i); /* { dg-warning "conversion" } */
  ffloatc (UINT_MAX + 1i); /* { dg-warning "conversion" } */
  ffloatc (UINT_MAX + UINT_MAX * 1i); /* { dg-warning "conversion" } */

  vfloatc = UINT_MAX * 1i; /* { dg-warning "conversion" } */
  vfloatc = UINT_MAX + 1i; /* { dg-warning "conversion" } */
  vfloatc = UINT_MAX + UINT_MAX * 1i; /* { dg-warning "conversion" } */
}

// SLATE-FILECHECK-STD DEFAULT gnu99
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
// DEFAULT-NEXT:     global %[[VALUE_vsi:[0-9]+]] vsi: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vui:[0-9]+]] vui: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vfloat:[0-9]+]] vfloat: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vsic:[0-9]+]] vsic: complex<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vuic:[0-9]+]] vuic: complex<u32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vfloatc:[0-9]+]] vfloatc: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fsi:[0-9]+]] @fsi(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fui:[0-9]+]] @fui(%[[VALUE1:[0-9]+]] <unnamed>: u32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_ffloat:[0-9]+]] @ffloat(%[[VALUE2:[0-9]+]] <unnamed>: f32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fsic:[0-9]+]] @fsic(%[[VALUE3:[0-9]+]] <unnamed>: complex<i32>) -> void [linkage=external] [abi=sysv64(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_fuic:[0-9]+]] @fuic(%[[VALUE4:[0-9]+]] <unnamed>: complex<u32>) -> void [linkage=external] [abi=sysv64(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_ffloatc:[0-9]+]] @ffloatc(%[[VALUE5:[0-9]+]] <unnamed>: complex<f32>) -> void [linkage=external] [abi=sysv64(native_c) -> void];
// DEFAULT-NEXT:     fn %[[VALUE_var_float_to_int:[0-9]+]] @var_float_to_int() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_doublec:[0-9]+]] doublec: complex<f64> [storage=automatic] = real_to_complex<complex<f64>, reason=assign>(const<f64>(0.0));
// DEFAULT-NEXT:         call<void, signature=fn(complex<i32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fsic]], complex_convert<complex<i32>, reason=arg, out_of_range=ub, exceptions=observable>(read<complex<f64>>(%[[VALUE_doublec]])));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], complex_convert<complex<u32>, reason=arg, out_of_range=ub, exceptions=observable>(read<complex<f64>>(%[[VALUE_doublec]])));
// DEFAULT-NEXT:         write<complex<i32>>(%[[VALUE_vsic]], complex_convert<complex<i32>, reason=assign, out_of_range=ub, exceptions=observable>(read<complex<f64>>(%[[VALUE_doublec]])));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_vuic]], complex_convert<complex<u32>, reason=assign, out_of_range=ub, exceptions=observable>(read<complex<f64>>(%[[VALUE_doublec]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_var_complex_to_real:[0-9]+]] @var_complex_to_real() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ic:[0-9]+]] ic: complex<i32> [storage=automatic] = real_to_complex<complex<i32>, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_uc:[0-9]+]] uc: complex<u32> [storage=automatic] = real_to_complex<complex<u32>, reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_ullc:[0-9]+]] ullc: complex<u64> [storage=automatic] = real_to_complex<complex<u64>, reason=assign>(reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<i32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fsic]], read<complex<i32>>(%[[VALUE_ic]]));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], read<complex<u32>>(%[[VALUE_uc]]));
// DEFAULT-NEXT:         write<complex<i32>>(%[[VALUE_vsic]], read<complex<i32>>(%[[VALUE_ic]]));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_vuic]], read<complex<u32>>(%[[VALUE_uc]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_fsi]], complex_to_real<i32, reason=arg>(read<complex<i32>>(%[[VALUE_ic]])));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_vsi]], complex_to_real<i32, reason=assign>(read<complex<i32>>(%[[VALUE_ic]])));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fui]], complex_to_real<u32, reason=arg>(read<complex<u32>>(%[[VALUE_uc]])));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_vui]], complex_to_real<u32, reason=assign>(read<complex<u32>>(%[[VALUE_uc]])));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], complex_convert<complex<u32>, reason=arg, fits=unknown>(read<complex<u64>>(%[[VALUE_ullc]])));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_vuic]], complex_convert<complex<u32>, reason=assign, fits=unknown>(read<complex<u64>>(%[[VALUE_ullc]])));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fui]], reinterpret<u32, reason=arg, fits=unknown>(complex_to_real<i32, reason=arg>(read<complex<i32>>(%[[VALUE_ic]]))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_vui]], reinterpret<u32, reason=assign, fits=unknown>(complex_to_real<i32, reason=assign>(read<complex<i32>>(%[[VALUE_ic]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_const_float_to_int:[0-9]+]] @const_float_to_int() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(complex<i32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fsic]], complex_convert<complex<i32>, reason=arg, out_of_range=ub, exceptions=observable>(sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], complex_convert<complex<u32>, reason=arg, out_of_range=ub, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)))));
// DEFAULT-NEXT:         write<complex<i32>>(%[[VALUE_vsic]], complex_convert<complex<i32>, reason=assign, out_of_range=ub, exceptions=observable>(sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)))));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_vuic]], complex_convert<complex<u32>, reason=assign, out_of_range=ub, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(1.0)))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<i32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fsic]], complex_convert<complex<i32>, reason=arg, out_of_range=ub, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(0.5), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.0)))));
// DEFAULT-NEXT:         write<complex<i32>>(%[[VALUE_vsic]], complex_convert<complex<i32>, reason=assign, out_of_range=ub, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(0.5), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.0)))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], complex_convert<complex<u32>, reason=arg, out_of_range=ub, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(0.5), aggregate<complex<f64>, zero_fill=false>(index0 = const<f64>(0.0), index1 = const<f64>(0.0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_const_complex_int_to_real_int:[0-9]+]] @const_complex_int_to_real_int() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_fsi]], complex_to_real<i32, reason=arg>(add<complex<i32>, complex=true, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0)))));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fui]], reinterpret<u32, reason=arg, fits=unknown>(complex_to_real<i32, reason=arg>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0))))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_vsi]], complex_to_real<i32, reason=assign>(add<complex<i32>, complex=true, overflow=ub>(neg<i32, overflow=ub>(const<i32>(1)), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0)))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_vui]], reinterpret<u32, reason=assign, fits=unknown>(complex_to_real<i32, reason=assign>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fui]], reinterpret<u32, reason=arg, fits=unknown>(complex_to_real<i32, reason=arg>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_vui]], reinterpret<u32, reason=assign, fits=unknown>(complex_to_real<i32, reason=assign>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(u32) -> void>(%[[VALUE_fui]], truncate<u32, reason=arg, fits=unknown>(complex_to_real<u64, reason=arg>(add<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), complex_convert<complex<u64>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0)))))));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_vui]], truncate<u32, reason=assign, fits=unknown>(complex_to_real<u64, reason=assign>(add<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), complex_convert<complex<u64>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0)))))));
// DEFAULT-NEXT:         call<void, signature=fn(f32) -> void>(%[[VALUE_ffloat]], int_to_float<f32, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(complex_to_real<u32, reason=arg>(add<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), complex_convert<complex<u32>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0)))))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_vfloat]], int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(complex_to_real<u32, reason=assign>(add<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), complex_convert<complex<u32>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_const_complex_int_narrowing:[0-9]+]] @const_complex_int_narrowing() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(complex<i32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fsic]], sub<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], complex_convert<complex<u32>, reason=arg, fits=unknown>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))));
// DEFAULT-NEXT:         write<complex<i32>>(%[[VALUE_vsic]], sub<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_vuic]], complex_convert<complex<u32>, reason=assign, fits=unknown>(add<complex<i32>, complex=true, overflow=ub>(const<i32>(1), aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], complex_convert<complex<u32>, reason=arg, fits=unknown>(add<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), complex_convert<complex<u64>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], complex_convert<complex<u32>, reason=arg, fits=unknown>(mul<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), complex_convert<complex<u64>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<u32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_fuic]], complex_convert<complex<u32>, reason=arg, fits=unknown>(add<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), mul<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), complex_convert<complex<u64>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_vuic]], complex_convert<complex<u32>, reason=assign, fits=unknown>(mul<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), complex_convert<complex<u64>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_vuic]], complex_convert<complex<u32>, reason=assign, fits=unknown>(add<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), complex_convert<complex<u64>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         write<complex<u32>>(%[[VALUE_vuic]], complex_convert<complex<u32>, reason=assign, fits=unknown>(add<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), mul<complex<u64>, complex=true, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=usual_arith>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1))), const<u64>(1)), complex_convert<complex<u64>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_ffloatc]], complex_convert<complex<f32>, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(mul<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), complex_convert<complex<u32>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_ffloatc]], complex_convert<complex<f32>, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(add<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), complex_convert<complex<u32>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         call<void, signature=fn(complex<f32>) -> void, abi=sysv64(native_c) -> void>(%[[VALUE_ffloatc]], complex_convert<complex<f32>, reason=arg, exact=false, rounding=nearest_even, exceptions=observable>(add<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), mul<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), complex_convert<complex<u32>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_vfloatc]], complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(mul<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), complex_convert<complex<u32>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_vfloatc]], complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(add<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), complex_convert<complex<u32>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1))))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_vfloatc]], complex_convert<complex<f32>, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(add<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), mul<complex<u32>, complex=true, overflow=wrap>(add<u32, overflow=wrap>(mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483647)), const<u32>(2)), const<u32>(1)), complex_convert<complex<u32>, reason=usual_arith, fits=unknown>(aggregate<complex<i32>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(1)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
