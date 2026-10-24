/* { dg-do compile } */
/* { dg-options "-std=gnu17 -fpermissive -Wdouble-promotion" } */

#include <stddef.h>

/* Some targets do not provide <complex.h> so we define I ourselves.  */
#define I 1.0iF
#define ID ((_Complex double)I)

float f;
double d;
int i;
long double ld;
_Complex float cf;
_Complex double cd;
_Complex long double cld;
size_t s;

extern void unprototyped_fn ();
extern void varargs_fn (int, ...);
extern void double_fn (double);
extern float float_fn (void);

void 
usual_arithmetic_conversions(void) 
{
  float local_f;
  _Complex float local_cf;

  /* Values of type "float" are implicitly converted to "double" or
     "long double" due to use in arithmetic with "double" or "long
     double" operands.  */
  local_f = f + 1.0;         /* { dg-warning "implicit" } */
  local_f = f - d;           /* { dg-warning "implicit" } */
  local_f = 1.0f * 1.0;      /* { dg-warning "implicit" } */
  local_f = 1.0f / d;        /* { dg-warning "implicit" } */

  local_cf = cf + 1.0;       /* { dg-warning "implicit" } */
  local_cf = cf - d;         /* { dg-warning "implicit" } */
  local_cf = cf + 1.0 * ID;  /* { dg-warning "implicit" } */
  local_cf = cf - cd;        /* { dg-warning "implicit" } */
  
  local_f = i ? f : d;       /* { dg-warning "implicit" } */
  i = f == d;                /* { dg-warning "implicit" } */
  i = d != f;                /* { dg-warning "implicit" } */
}

void 
default_argument_promotion (void) 
{
  /* Because there is no prototype, "f" is promoted to "double".  */
  unprototyped_fn (f); /* { dg-warning "implicit" } */
  undeclared_fn (f);   /* { dg-warning "implicit" } */
  /* Because "f" is part of the variable argument list, it is promoted
     to "double".  */
  varargs_fn (1, f);   /* { dg-warning "implicit" } */
}

/* There is no warning when an explicit cast is used to perform the
   conversion.  */

void
casts (void) 
{
  float local_f;
  _Complex float local_cf;

  local_f = (double)f + 1.0;                 /* { dg-bogus "implicit" } */
  local_f = (double)f - d;                   /* { dg-bogus "implicit" } */
  local_f = (double)1.0f + 1.0;              /* { dg-bogus "implicit" } */
  local_f = (double)1.0f - d;                /* { dg-bogus "implicit" } */

  local_cf = (_Complex double)cf + 1.0;      /* { dg-bogus "implicit" } */
  local_cf = (_Complex double)cf - d;        /* { dg-bogus "implicit" } */
  local_cf = (_Complex double)cf + 1.0 * ID; /* { dg-bogus "implicit" } */
  local_cf = (_Complex double)cf - cd;       /* { dg-bogus "implicit" } */

  local_f = i ? (double)f : d;               /* { dg-bogus "implicit" } */
  i = (double)f == d;                        /* { dg-bogus "implicit" } */
  i = d != (double)f;                        /* { dg-bogus "implicit" } */
}

/* There is no warning on conversions that occur in assignment (and
   assignment-like) contexts.  */

void 
assignments (void)
{
  d = f;           /* { dg-bogus "implicit" } */
  double_fn (f);   /* { dg-bogus "implicit" } */
  d = float_fn (); /* { dg-bogus "implicit" } */
}

/* There is no warning in non-evaluated contexts.  */

void
non_evaluated (void)
{
  s = sizeof (f + 1.0);             /* { dg-bogus "implicit" } */
  s = __alignof__ (f + 1.0);        /* { dg-bogus "implicit" } */
  d = (__typeof__(f + 1.0))f;       /* { dg-bogus "implicit" } */
  s = sizeof (i ? f : d);           /* { dg-bogus "implicit" } */
  s = sizeof (unprototyped_fn (f)); /* { dg-bogus "implicit" } */
}

// SLATE-FILECHECK-ARGS -fpermissive
// SLATE-FILECHECK-STD DEFAULT gnu17
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_i:[0-9]+]] i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ld:[0-9]+]] ld: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cf:[0-9]+]] cf: complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cd:[0-9]+]] cd: complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cld:[0-9]+]] cld: complex<f80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: u64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_unprototyped_fn:[0-9]+]] @unprototyped_fn(unprototyped) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_varargs_fn:[0-9]+]] @varargs_fn(%[[VALUE0:[0-9]+]] <unnamed>: i32, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_double_fn:[0-9]+]] @double_fn(%[[VALUE1:[0-9]+]] <unnamed>: f64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_float_fn:[0-9]+]] @float_fn() -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_usual_arithmetic_conversions:[0-9]+]] @usual_arithmetic_conversions() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_local_f:[0-9]+]] local_f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_local_cf:[0-9]+]] local_cf: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_f]])), const<f64>(1.0))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_f]])), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=usual_arith>(const<f32>(1.0)), const<f64>(1.0))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(div<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=usual_arith>(const<f32>(1.0)), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_local_cf]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%[[VALUE_cf]])), const<f64>(1.0))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_local_cf]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%[[VALUE_cf]])), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_local_cf]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%[[VALUE_cf]])), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), complex_convert<complex<f64>, reason=explicit>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_local_cf]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(complex_convert<complex<f64>, reason=usual_arith>(read<complex<f32>>(%[[VALUE_cf]])), read<complex<f64>>(%[[VALUE_cd]]))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(conditional<f64>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_f]])), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], from_bool<i32, reason=assign>(eq<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_f]])), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], from_bool<i32, reason=assign>(ne<f64, exceptions=observable>(read<f64>(%[[VALUE_d]]), float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_f]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_undeclared_fn:[0-9]+]] @undeclared_fn(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_default_argument_promotion:[0-9]+]] @default_argument_promotion() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_unprototyped_fn]], float_widen<f64, reason=vararg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_undeclared_fn]], float_widen<f64, reason=vararg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_varargs_fn]], const<i32>(1), float_widen<f64, reason=vararg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_casts:[0-9]+]] @casts() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_local_f_2:[0-9]+]] local_f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_local_cf_2:[0-9]+]] local_cf: complex<f32> [storage=automatic];
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f_2]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=explicit>(read<f32>(%[[VALUE_f]])), const<f64>(1.0))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f_2]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=explicit>(read<f32>(%[[VALUE_f]])), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f_2]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=explicit>(const<f32>(1.0)), const<f64>(1.0))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f_2]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=explicit>(const<f32>(1.0)), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_local_cf_2]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(complex_convert<complex<f64>, reason=explicit>(read<complex<f32>>(%[[VALUE_cf]])), const<f64>(1.0))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_local_cf_2]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(complex_convert<complex<f64>, reason=explicit>(read<complex<f32>>(%[[VALUE_cf]])), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_local_cf_2]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(add<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(complex_convert<complex<f64>, reason=explicit>(read<complex<f32>>(%[[VALUE_cf]])), mul<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(const<f64>(1.0), complex_convert<complex<f64>, reason=explicit>(aggregate<complex<f32>, zero_fill=false>(index0 = const<f32>(0.0), index1 = const<f32>(1.0)))))));
// DEFAULT-NEXT:         write<complex<f32>>(%[[VALUE_local_cf_2]], complex_convert<complex<f32>, reason=assign, rounding=nearest_even, exceptions=observable>(sub<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(complex_convert<complex<f64>, reason=explicit>(read<complex<f32>>(%[[VALUE_cf]])), read<complex<f64>>(%[[VALUE_cd]]))));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_local_f_2]], float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(conditional<f64>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), float_widen<f64, reason=explicit>(read<f32>(%[[VALUE_f]])), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], from_bool<i32, reason=assign>(eq<f64, exceptions=observable>(float_widen<f64, reason=explicit>(read<f32>(%[[VALUE_f]])), read<f64>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], from_bool<i32, reason=assign>(ne<f64, exceptions=observable>(read<f64>(%[[VALUE_d]]), float_widen<f64, reason=explicit>(read<f32>(%[[VALUE_f]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_assignments:[0-9]+]] @assignments() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], float_widen<f64, reason=assign>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         call<void, signature=fn(f64) -> void>(%[[VALUE_double_fn]], float_widen<f64, reason=arg>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], float_widen<f64, reason=assign>(call<f32, signature=fn() -> f32>(%[[VALUE_float_fn]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_non_evaluated:[0-9]+]] @non_evaluated() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<u64>(%[[VALUE_s]], const<u64>(8));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_s]], const<u64>(8));
// DEFAULT-NEXT:         write<f64>(%[[VALUE_d]], float_widen<f64, reason=explicit>(read<f32>(%[[VALUE_f]])));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_s]], const<u64>(8));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_s]], const<u64>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
