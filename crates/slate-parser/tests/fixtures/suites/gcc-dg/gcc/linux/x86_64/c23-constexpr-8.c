/* Test C23 constexpr.  Valid code, compilation tests, IEEE arithmetic.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target inff } */

constexpr float fi = __builtin_inf ();
constexpr double di = __builtin_inff ();
constexpr float fn = __builtin_nan ("");
constexpr double dn = __builtin_nanf ("");
constexpr float fns = __builtin_nansf ("");
constexpr double dns = __builtin_nans ("");
constexpr _Complex double cdns = __builtin_nans ("");

void
f0 (void)
{
  (constexpr float) { __builtin_inf () };
  (constexpr double) { __builtin_inff () };
  (constexpr float) { __builtin_nan ("") };
  (constexpr double) { __builtin_nanf ("") };
  (constexpr float) { __builtin_nansf ("") };
  (constexpr double) { __builtin_nans ("") };
  (constexpr _Complex double) { __builtin_nans ("") };
}

// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     global %[[VALUE_fi:[0-9]+]] fi: f32 [storage=static] [const] [constexpr] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf:[0-9]+]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_di:[0-9]+]] di: f64 [storage=static] [const] [constexpr] = float_widen<f64, reason=assign>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff:[0-9]+]])) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_fn:[0-9]+]] fn: f32 [storage=static] [const] [constexpr] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_dn:[0-9]+]] dn: f64 [storage=static] [const] [constexpr] = float_widen<f64, reason=assign>(call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_fns:[0-9]+]] fns: f32 [storage=static] [const] [constexpr] = call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nansf:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_dns:[0-9]+]] dns: f64 [storage=static] [const] [constexpr] = call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nans:[0-9]+]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_4]]))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_cdns:[0-9]+]] cdns: complex<f64> [storage=static] [const] [constexpr] = real_to_complex<complex<f64>, reason=assign>(call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nans]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_5]])))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan]] @__builtin_nan(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf]] @__builtin_nanf(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nansf]] @__builtin_nansf(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nans]] @__builtin_nans(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_f0:[0-9]+]] @f0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<f32>(compound_literal %[[VALUE4:[0-9]+]] [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]])));
// DEFAULT-NEXT:         read<f64>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = float_widen<f64, reason=assign>(call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]])));
// DEFAULT-NEXT:         read<f32>(compound_literal %[[VALUE6:[0-9]+]] [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_6]])))));
// DEFAULT-NEXT:         read<f64>(compound_literal %[[VALUE7:[0-9]+]] [storage=automatic] = float_widen<f64, reason=assign>(call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_7]])))));
// DEFAULT-NEXT:         read<f32>(compound_literal %[[VALUE8:[0-9]+]] [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nansf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_8]]))));
// DEFAULT-NEXT:         read<f64>(compound_literal %[[VALUE9:[0-9]+]] [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nans]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_9]]))));
// DEFAULT-NEXT:         read<complex<f64>>(compound_literal %[[VALUE10:[0-9]+]] [storage=automatic] = real_to_complex<complex<f64>, reason=assign>(call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nans]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_10]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
