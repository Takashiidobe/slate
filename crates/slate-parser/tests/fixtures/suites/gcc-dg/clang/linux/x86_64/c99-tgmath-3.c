/* Test for <tgmath.h> in C99. */
/* Origin: Matt Austern <austern@apple.com>
/* { dg-do compile { target c99_runtime } } */
/* { dg-options "-std=iso9899:1999" } */
/* { dg-require-effective-target tgmath_h } */

/* Test that invoking type-generic exp on a complex invokes cexp. */
#include <tgmath.h>

complex double foo(complex double x)
{
  return exp(x);
}

/* { dg-final { scan-assembler "cexp" } } */

// SLATE-FILECHECK-STD DEFAULT iso9899:1999
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
// DEFAULT-NEXT:     type @type[[TYPE__Argument_type_is_not_arithmetic:[0-9]+]] _Argument_type_is_not_arithmetic = void;
// DEFAULT-NEXT:     fn %[[VALUE_exp:[0-9]+]] @exp(%[[VALUE___x:[0-9]+]] __x: f64) -> f64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expf:[0-9]+]] @expf(%[[VALUE___x_2:[0-9]+]] __x: f32) -> f32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_expl:[0-9]+]] @expl(%[[VALUE___x_3:[0-9]+]] __x: f80) -> f80 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cexp:[0-9]+]] @cexp(%[[VALUE___z:[0-9]+]] __z: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cexpf:[0-9]+]] @cexpf(%[[VALUE___z_2:[0-9]+]] __z: complex<f32>) -> complex<f32> [linkage=external] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE_cexpl:[0-9]+]] @cexpl(%[[VALUE___z_3:[0-9]+]] __z: complex<f80>) -> complex<f80> [linkage=external] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote:[0-9]+]] @__tg_promote(...) -> void [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_2:[0-9]+]] @__tg_promote(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> f64 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_3:[0-9]+]] @__tg_promote(%[[VALUE1:[0-9]+]] <unnamed>: u32) -> f64 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_4:[0-9]+]] @__tg_promote(%[[VALUE2:[0-9]+]] <unnamed>: i64) -> f64 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_5:[0-9]+]] @__tg_promote(%[[VALUE3:[0-9]+]] <unnamed>: u64) -> f64 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_6:[0-9]+]] @__tg_promote(%[[VALUE4:[0-9]+]] <unnamed>: i64) -> f64 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_7:[0-9]+]] @__tg_promote(%[[VALUE5:[0-9]+]] <unnamed>: u64) -> f64 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_8:[0-9]+]] @__tg_promote(%[[VALUE6:[0-9]+]] <unnamed>: f32) -> f32 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_9:[0-9]+]] @__tg_promote(%[[VALUE7:[0-9]+]] <unnamed>: f64) -> f64 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_10:[0-9]+]] @__tg_promote(%[[VALUE8:[0-9]+]] <unnamed>: f80) -> f80 [linkage=internal] [overloadable];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_11:[0-9]+]] @__tg_promote(%[[VALUE9:[0-9]+]] <unnamed>: complex<f32>) -> complex<f32> [linkage=internal] [overloadable] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_12:[0-9]+]] @__tg_promote(%[[VALUE10:[0-9]+]] <unnamed>: complex<f64>) -> complex<f64> [linkage=internal] [overloadable] [abi=sysv64(native_c) -> native_c];
// DEFAULT-NEXT:     fn %[[VALUE___tg_promote_13:[0-9]+]] @__tg_promote(%[[VALUE11:[0-9]+]] <unnamed>: complex<f80>) -> complex<f80> [linkage=internal] [overloadable] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>];
// DEFAULT-NEXT:     fn %[[VALUE___tg_exp:[0-9]+]] @__tg_exp(%[[VALUE___x_4:[0-9]+]] __x: f32) -> f32 [linkage=internal] [inline=always] [definition=emitted] [overloadable] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f32, signature=fn(f32) -> f32>(%[[VALUE_expf]], read<f32>(%[[VALUE___x_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___tg_exp_2:[0-9]+]] @__tg_exp(%[[VALUE___x_5:[0-9]+]] __x: f64) -> f64 [linkage=internal] [inline=always] [definition=emitted] [overloadable] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f64, signature=fn(f64) -> f64>(%[[VALUE_exp]], read<f64>(%[[VALUE___x_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___tg_exp_3:[0-9]+]] @__tg_exp(%[[VALUE___x_6:[0-9]+]] __x: f80) -> f80 [linkage=internal] [inline=always] [definition=emitted] [overloadable] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<f80, signature=fn(f80) -> f80>(%[[VALUE_expl]], read<f80>(%[[VALUE___x_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___tg_exp_4:[0-9]+]] @__tg_exp(%[[VALUE___x_7:[0-9]+]] __x: complex<f32>) -> complex<f32> [linkage=internal] [inline=always] [definition=emitted] [overloadable] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<complex<f32>, signature=fn(complex<f32>) -> complex<f32>, abi=sysv64(native_c) -> native_c>(%[[VALUE_cexpf]], read<complex<f32>>(%[[VALUE___x_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___tg_exp_5:[0-9]+]] @__tg_exp(%[[VALUE___x_8:[0-9]+]] __x: complex<f64>) -> complex<f64> [linkage=internal] [inline=always] [definition=emitted] [overloadable] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE_cexp]], read<complex<f64>>(%[[VALUE___x_8]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___tg_exp_6:[0-9]+]] @__tg_exp(%[[VALUE___x_9:[0-9]+]] __x: complex<f80>) -> complex<f80> [linkage=internal] [inline=always] [definition=emitted] [overloadable] [abi=sysv64(byval<align=16>) -> coerce<f80, f80>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<complex<f80>, signature=fn(complex<f80>) -> complex<f80>, abi=sysv64(byval<align=16>) -> coerce<f80, f80>>(%[[VALUE_cexpl]], read<complex<f80>>(%[[VALUE___x_9]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: complex<f64>) -> complex<f64> [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<complex<f64>, signature=fn(complex<f64>) -> complex<f64>, abi=sysv64(native_c) -> native_c>(%[[VALUE___tg_exp_5]], read<complex<f64>>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
