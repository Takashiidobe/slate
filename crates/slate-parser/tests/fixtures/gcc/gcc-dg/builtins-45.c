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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     extern %2 x: f32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %3 y: f64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %4 z: f80 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @f(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @link_error() -> void [linkage=external];
// DEFAULT-NEXT:     fn %14 @__builtin_nan(%13 <unnamed>: ptr<const i8>) -> f64 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %17 @__builtin_nanf(%16 <unnamed>: ptr<const i8>) -> f32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %20 @__builtin_nanl(%19 <unnamed>: ptr<const i8>) -> f80 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %22 @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %23 @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %24 @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %26 @finite(%25 <unnamed>: f64) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %28 @finitef(%27 <unnamed>: f32) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %30 @finitel(%29 <unnamed>: f80) -> i32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 nan: f64 [storage=automatic] = call<f64, signature=fn(ptr<const i8>) -> f64>(%14, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%15)));
// DEFAULT-NEXT:         let %7 nanf: f32 [storage=automatic] = call<f32, signature=fn(ptr<const i8>) -> f32>(%17, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%18)));
// DEFAULT-NEXT:         let %8 nanl: f80 [storage=automatic] = call<f80, signature=fn(ptr<const i8>) -> f80>(%20, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(%21)));
// DEFAULT-NEXT:         let %9 pinf: f64 [storage=automatic] = call<f64, signature=fn() -> f64>(%22);
// DEFAULT-NEXT:         let %10 pinff: f32 [storage=automatic] = call<f32, signature=fn() -> f32>(%23);
// DEFAULT-NEXT:         let %11 pinfl: f80 [storage=automatic] = call<f80, signature=fn() -> f80>(%24);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%26, read<f64>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%28, read<f32>(%10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f80) -> i32>(%30, read<f80>(%11)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f64) -> i32>(%26, read<f64>(%6)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f32) -> i32>(%28, read<f32>(%7)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(f80) -> i32>(%30, read<f80>(%8)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f64) -> i32>(%26, const<f64>(4.0)), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f32) -> i32>(%28, float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(4.0))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         if not<bool>(ne<i32>(call<i32, signature=fn(f80) -> i32>(%30, float_widen<f80, reason=arg>(const<f64>(4.0))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
