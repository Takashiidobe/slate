/* { dg-do compile } */
/* { dg-options "-fpermissive -O2 -w" } */

extern void sincos(double x, double *sinx, double *cosx);
void apply(void (*f)(double, double *, double *),
	   double x, double *sinx, double *cosx)
{
  f(x, sinx, cosx);
  return;
}
void apply_sincos(double x, double **sinx, double **cosx)
{
  apply(sincos, x, sinx, cosx);
  return;
}

// SLATE-FILECHECK-ARGS -fpermissive
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
// DEFAULT-NEXT:     fn %[[VALUE_sincos:[0-9]+]] @sincos(%[[VALUE_x:[0-9]+]] x: f64, %[[VALUE_sinx:[0-9]+]] sinx: ptr<f64>, %[[VALUE_cosx:[0-9]+]] cosx: ptr<f64>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_apply:[0-9]+]] @apply(%[[VALUE_f:[0-9]+]] f: ptr<fn(f64, ptr<f64>, ptr<f64>) -> void>, %[[VALUE_x_2:[0-9]+]] x: f64, %[[VALUE_sinx_2:[0-9]+]] sinx: ptr<f64>, %[[VALUE_cosx_2:[0-9]+]] cosx: ptr<f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(f64, ptr<f64>, ptr<f64>) -> void>(read<ptr<fn(f64, ptr<f64>, ptr<f64>) -> void>>(%[[VALUE_f]]), read<f64>(%[[VALUE_x_2]]), read<ptr<f64>>(%[[VALUE_sinx_2]]), read<ptr<f64>>(%[[VALUE_cosx_2]]));
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_apply_sincos:[0-9]+]] @apply_sincos(%[[VALUE_x_3:[0-9]+]] x: f64, %[[VALUE_sinx_3:[0-9]+]] sinx: ptr<ptr<f64>>, %[[VALUE_cosx_3:[0-9]+]] cosx: ptr<ptr<f64>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<fn(f64, ptr<f64>, ptr<f64>) -> void>, f64, ptr<f64>, ptr<f64>) -> void>(%[[VALUE_apply]], function_decay<ptr<fn(f64, ptr<f64>, ptr<f64>) -> void>>(%[[VALUE_sincos]]), read<f64>(%[[VALUE_x_3]]), pointer_cast<ptr<f64>, reason=arg>(read<ptr<ptr<f64>>>(%[[VALUE_sinx_3]])), pointer_cast<ptr<f64>, reason=arg>(read<ptr<ptr<f64>>>(%[[VALUE_cosx_3]])));
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
