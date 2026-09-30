/* { dg-do compile } */
/* { dg-require-effective-target inf } */
/* { dg-options "-O1 -fno-trapping-math -fno-finite-math-only -fdump-tree-optimized" } */
  
extern void f(int);
extern void link_error ();

extern float x;
extern double y;
extern long double z;

int
main ()
{
  double nan = __builtin_nan ("");
  float nanf = __builtin_nanf ("");
  long double nanl = __builtin_nanl ("");

  double pinf = __builtin_inf ();
  float pinff = __builtin_inff ();
  long double pinfl = __builtin_infl ();

  if (__builtin_finite (pinf))
    link_error ();
  if (__builtin_finitef (pinff))
    link_error ();
  if (__builtin_finitel (pinfl))
    link_error ();

  if (__builtin_finite (nan))
    link_error ();
  if (__builtin_finitef (nanf))
    link_error ();
  if (__builtin_finitel (nanl))
    link_error ();

  if (!__builtin_finite (4.0))
    link_error ();
  if (!__builtin_finitef (4.0))
    link_error ();
  if (!__builtin_finitel (4.0))
    link_error ();
}


/* Check that all instances of link_error were subject to DCE.  */
/* { dg-final { scan-tree-dump-times "link_error" 0 "optimized" } } */

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     extern %[[VALUE_x:[0-9]+]] x: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_y:[0-9]+]] y: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_z:[0-9]+]] z: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_link_error:[0-9]+]] @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nan:[0-9]+]] @__builtin_nan(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanf:[0-9]+]] @__builtin_nanf(%[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_nanl:[0-9]+]] @__builtin_nanl(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf:[0-9]+]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff:[0-9]+]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infl:[0-9]+]] @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_finite:[0-9]+]] @finite(%[[VALUE4:[0-9]+]] <unnamed>: f64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_finitef:[0-9]+]] @finitef(%[[VALUE5:[0-9]+]] <unnamed>: f32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_finitel:[0-9]+]] @finitel(%[[VALUE6:[0-9]+]] <unnamed>: f80) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_nan:[0-9]+]] nan: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%[[VALUE___builtin_nan]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]])));
// DEFAULT-NEXT:         let %[[VALUE_nanf:[0-9]+]] nanf: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%[[VALUE___builtin_nanf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]])));
// DEFAULT-NEXT:         let %[[VALUE_nanl:[0-9]+]] nanl: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%[[VALUE___builtin_nanl]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         let %[[VALUE_pinf:[0-9]+]] pinf: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf]]);
// DEFAULT-NEXT:         let %[[VALUE_pinff:[0-9]+]] pinff: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff]]);
// DEFAULT-NEXT:         let %[[VALUE_pinfl:[0-9]+]] pinfl: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%[[VALUE_finite]], read<f64>(%[[VALUE_pinf]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%[[VALUE_finitef]], read<f32>(%[[VALUE_pinff]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f80) -> i32>(%[[VALUE_finitel]], read<f80>(%[[VALUE_pinfl]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%[[VALUE_finite]], read<f64>(%[[VALUE_nan]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%[[VALUE_finitef]], read<f32>(%[[VALUE_nanf]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f80) -> i32>(%[[VALUE_finitel]], read<f80>(%[[VALUE_nanl]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%[[VALUE_finite]], const<f64>(4.0)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f32) -> i32>(%[[VALUE_finitef]], float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(4.0))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f80) -> i32>(%[[VALUE_finitel]], float_widen<f80, reason=arg>(const<f64>(4.0))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_link_error]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
