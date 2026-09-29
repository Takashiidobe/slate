/* { dg-do compile } */

float fi = __builtin_inff();
/* { dg-error "does not support infinity" "INF unsupported" { target pdp11*-*-* vax-*-* } .-1 } */
double di = __builtin_inf();
/* { dg-error "does not support infinity" "INF unsupported" { target pdp11*-*-* vax-*-* } .-1 } */
long double li = __builtin_infl();
/* { dg-error "does not support infinity" "INF unsupported" { target pdp11*-*-* vax-*-* } .-1 } */

float fh = __builtin_huge_valf();
double dh = __builtin_huge_val();
long double lh = __builtin_huge_vall();


// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %[[VALUE_fi:[0-9]+]] fi: f32 [storage=static] = call<f32, signature=fn() -> f32>(%[[VALUE___builtin_inff:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_di:[0-9]+]] di: f64 [storage=static] = call<f64, signature=fn() -> f64>(%[[VALUE___builtin_inf:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_li:[0-9]+]] li: f80 [storage=static] = call<f80, signature=fn() -> f80>(%[[VALUE___builtin_infl:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fh:[0-9]+]] fh: f32 [storage=static] = call<f32, signature=fn() -> f32>(%[[VALUE___builtin_huge_valf:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_dh:[0-9]+]] dh: f64 [storage=static] = call<f64, signature=fn() -> f64>(%[[VALUE___builtin_huge_val:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_lh:[0-9]+]] lh: f80 [storage=static] = call<f80, signature=fn() -> f80>(%[[VALUE___builtin_huge_vall:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inff]] @__builtin_inff() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_inf]] @__builtin_inf() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_infl]] @__builtin_infl() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_valf]] @__builtin_huge_valf() -> f32 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_val]] @__builtin_huge_val() -> f64 [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_huge_vall]] @__builtin_huge_vall() -> f80 [linkage=external] [memory=none];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
