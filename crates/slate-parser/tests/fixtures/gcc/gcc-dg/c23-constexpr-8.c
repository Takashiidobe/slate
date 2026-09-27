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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %0 fi: f32 [storage=static] [const] [constexpr] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn() -> f64>(%8)) [linkage=internal];
// DEFAULT-NEXT:     global %1 di: f64 [storage=static] [const] [constexpr] = float_widen<f64, reason=assign>(call<f32, signature=fn() -> f32>(%9)) [linkage=internal];
// DEFAULT-NEXT:     global %12 .str12: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %2 fn: f32 [storage=static] [const] [constexpr] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(call<f64, signature=fn(ptr<const i8>) -> f64>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%12)))) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %3 dn: f64 [storage=static] [const] [constexpr] = float_widen<f64, reason=assign>(call<f32, signature=fn(ptr<const i8>) -> f32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%15)))) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %4 fns: f32 [storage=static] [const] [constexpr] = call<f32, signature=fn(ptr<const i8>) -> f32>(%17, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%18))) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %5 dns: f64 [storage=static] [const] [constexpr] = call<f64, signature=fn(ptr<const i8>) -> f64>(%20, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%21))) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %6 cdns: complex<f64> [storage=static] [const] [constexpr] = real_to_complex<complex<f64>, reason=assign>(call<f64, signature=fn(ptr<const i8>) -> f64>(%20, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%22)))) [linkage=internal];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %27 .str27: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %29 .str29: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %8 @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %9 @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %11 @__builtin_nan(%10 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %14 @__builtin_nanf(%13 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %17 @__builtin_nansf(%16 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %20 @__builtin_nans(%19 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %7 @f0() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         read<f32>(compound_literal %23 [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn() -> f64>(%8)));
// DEFAULT-NEXT:         read<f64>(compound_literal %24 [storage=automatic] = float_widen<f64, reason=assign>(call<f32, signature=fn() -> f32>(%9)));
// DEFAULT-NEXT:         read<f32>(compound_literal %26 [storage=automatic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=observable>(call<f64, signature=fn(ptr<const i8>) -> f64>(%11, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%25)))));
// DEFAULT-NEXT:         read<f64>(compound_literal %28 [storage=automatic] = float_widen<f64, reason=assign>(call<f32, signature=fn(ptr<const i8>) -> f32>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%27)))));
// DEFAULT-NEXT:         read<f32>(compound_literal %30 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%17, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%29))));
// DEFAULT-NEXT:         read<f64>(compound_literal %32 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%20, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%31))));
// DEFAULT-NEXT:         read<complex<f64>>(compound_literal %34 [storage=automatic] = real_to_complex<complex<f64>, reason=assign>(call<f64, signature=fn(ptr<const i8>) -> f64>(%20, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%33)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
