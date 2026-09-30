/* Test complex divide does not have the bug identified in N1496.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */
/* { dg-add-options ieee } */

extern void abort (void);
extern void exit (int);

#define CMPLX(x, y) __builtin_complex ((double) (x), (double) (y))
#define CMPLXF(x, y) __builtin_complex ((float) (x), (float) (y))
#define CMPLXL(x, y) __builtin_complex ((long double) (x), (long double) (y))
#define NAN __builtin_nanf ("")
#define isnan(x) __builtin_isnan (x)

volatile _Complex float num_f = CMPLXF (1, 1);
volatile _Complex float den_f = CMPLXF (0, NAN);
volatile _Complex float res_f, cres_f = CMPLXF (1, 1) / CMPLXF (0, NAN);

volatile _Complex double num_d = CMPLX (1, 1);
volatile _Complex double den_d = CMPLX (0, NAN);
volatile _Complex double res_d, cres_d = CMPLX (1, 1) / CMPLX (0, NAN);

volatile _Complex long double num_ld = CMPLXL (1, 1);
volatile _Complex long double den_ld = CMPLXL (0, NAN);
volatile _Complex long double res_ld, cres_ld = CMPLXL (1, 1) / CMPLXL (0, NAN);

int
main (void)
{
  res_f = num_f / den_f;
  if (!isnan (__real__ res_f) || !isnan (__imag__ res_f)
      || !isnan (__real__ cres_f) || !isnan (__imag__ cres_f))
    abort ();
  res_d = num_d / den_d;
  if (!isnan (__real__ res_d) || !isnan (__imag__ res_d)
      || !isnan (__real__ cres_d) || !isnan (__imag__ cres_d))
    abort ();
  res_ld = num_ld / den_ld;
  if (!isnan (__real__ res_ld) || !isnan (__imag__ res_ld)
      || !isnan (__real__ cres_ld) || !isnan (__imag__ cres_ld))
    abort ();
  exit (0);
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     global %[[VALUE_num_f:[0-9]+]] num_f: volatile complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), index1 = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_den_f:[0-9]+]] den_f: volatile complex<f32> [storage=static] = aggregate<complex<f32>, zero_fill=false>(index0 = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index1 = call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_res_f:[0-9]+]] res_f: volatile complex<f32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_cres_f:[0-9]+]] cres_f: volatile complex<f32> [storage=static] = div<complex<f32>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(aggregate<complex<f32>, zero_fill=false>(index0 = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), index1 = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), aggregate<complex<f32>, zero_fill=false>(index0 = int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index1 = call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]]))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_num_d:[0-9]+]] num_d: volatile complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), index1 = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_den_d:[0-9]+]] den_d: volatile complex<f64> [storage=static] = aggregate<complex<f64>, zero_fill=false>(index0 = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index1 = float_widen<f64, reason=explicit>(call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]]))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_res_d:[0-9]+]] res_d: volatile complex<f64> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_cres_d:[0-9]+]] cres_d: volatile complex<f64> [storage=static] = div<complex<f64>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(aggregate<complex<f64>, zero_fill=false>(index0 = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), index1 = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), aggregate<complex<f64>, zero_fill=false>(index0 = int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index1 = float_widen<f64, reason=explicit>(call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_4]])))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_num_ld:[0-9]+]] num_ld: volatile complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), index1 = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_den_ld:[0-9]+]] den_ld: volatile complex<f80> [storage=static] = aggregate<complex<f80>, zero_fill=false>(index0 = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index1 = float_widen<f80, reason=explicit>(call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]]))))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_res_ld:[0-9]+]] res_ld: volatile complex<f80> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_cres_ld:[0-9]+]] cres_ld: volatile complex<f80> [storage=static] = div<complex<f80>, complex=true, rounding=nearest_even, exceptions=ignore, range=full>(aggregate<complex<f80>, zero_fill=false>(index0 = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)), index1 = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), aggregate<complex<f80>, zero_fill=false>(index0 = int_to_float<f80, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0)), index1 = float_widen<f80, reason=explicit>(call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_6]])))))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf]] @__builtin_nanf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<complex<f32>, volatile>(%[[VALUE_res_f]], div<complex<f32>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f32>, volatile>(%[[VALUE_num_f]]), read<complex<f32>, volatile>(%[[VALUE_den_f]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(not<bool>(float_class<bool, test=nan>(read<f32, volatile>(real(%[[VALUE_res_f]])))), not<bool>(float_class<bool, test=nan>(read<f32, volatile>(imag(%[[VALUE_res_f]]))))), not<bool>(float_class<bool, test=nan>(read<f32, volatile>(real(%[[VALUE_cres_f]]))))), not<bool>(float_class<bool, test=nan>(read<f32, volatile>(imag(%[[VALUE_cres_f]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f64>, volatile>(%[[VALUE_res_d]], div<complex<f64>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f64>, volatile>(%[[VALUE_num_d]]), read<complex<f64>, volatile>(%[[VALUE_den_d]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(not<bool>(float_class<bool, test=nan>(read<f64, volatile>(real(%[[VALUE_res_d]])))), not<bool>(float_class<bool, test=nan>(read<f64, volatile>(imag(%[[VALUE_res_d]]))))), not<bool>(float_class<bool, test=nan>(read<f64, volatile>(real(%[[VALUE_cres_d]]))))), not<bool>(float_class<bool, test=nan>(read<f64, volatile>(imag(%[[VALUE_cres_d]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<complex<f80>, volatile>(%[[VALUE_res_ld]], div<complex<f80>, complex=true, rounding=nearest_even, exceptions=observable, range=full>(read<complex<f80>, volatile>(%[[VALUE_num_ld]]), read<complex<f80>, volatile>(%[[VALUE_den_ld]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(not<bool>(float_class<bool, test=nan>(read<f80, volatile>(real(%[[VALUE_res_ld]])))), not<bool>(float_class<bool, test=nan>(read<f80, volatile>(imag(%[[VALUE_res_ld]]))))), not<bool>(float_class<bool, test=nan>(read<f80, volatile>(real(%[[VALUE_cres_ld]]))))), not<bool>(float_class<bool, test=nan>(read<f80, volatile>(imag(%[[VALUE_cres_ld]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
