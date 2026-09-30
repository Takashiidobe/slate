/* { dg-do compile { target aarch64*-*-* arm*-*-* i?86-*-* powerpc*-*-* riscv*-*-* s390*-*-* x86_64-*-* } } */
/* { dg-additional-options "-msse2" { target i?86-*-* x86_64-*-* } } */

#if defined (__aarch64__)
# define FPR "{d5}"
/* { dg-final { scan-assembler-times "foo\tv5" 4 { target { aarch64*-*-* } } } } */
#elif defined (__arm__)
# define FPR "{d5}"
/* { dg-additional-options "-mcpu=unset -march=armv7-a+fp -mfloat-abi=hard" { target arm*-*-* } } */
/* { dg-final { scan-assembler-times "foo\ts10" 4 { target { arm*-*-* } } } } */
#elif defined (__powerpc__) || defined (__POWERPC__)
# define FPR "{5}"
/* { dg-final { scan-assembler-times "foo\t5" 4 { target { powerpc*-*-* } } } } */
#elif defined (__riscv)
# define FPR "{fa5}"
/* { dg-final { scan-assembler-times "foo\tfa5" 4 { target { riscv*-*-* } } } } */
#elif defined (__s390__)
# define FPR "{f5}"
/* { dg-final { scan-assembler-times "foo\t%f5" 4 { target { s390*-*-* } } } } */
#elif defined (__i386__) || defined (__x86_64__)
# define FPR "{xmm5}"
/* { dg-final { scan-assembler-times "foo\t%xmm5" 4 { target { i?86-*-* x86_64-*-* } } } } */
#endif

float
test_float (float x)
{
  __asm__ ("foo\t%0" : "+"FPR (x));
  return x;
}

float
test_float_from_mem (float *x)
{
  __asm__ ("foo\t%0" : "+"FPR (*x));
  return *x;
}

double
test_double (double x)
{
  __asm__ ("foo\t%0" : "+"FPR (x));
  return x;
}

double
test_double_from_mem (double *x)
{
  __asm__ ("foo\t%0" : "+"FPR (*x));
  return *x;
}

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
// DEFAULT-NEXT:     fn %[[VALUE_test_float:[0-9]+]] @test_float(%[[VALUE_x:[0-9]+]] x: f32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{xmm5}" [{xmm5}] width 32 place<f32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_float_from_mem:[0-9]+]] @test_float_from_mem(%[[VALUE_x_2:[0-9]+]] x: ptr<f32>) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{xmm5}" [{xmm5}] width 32 place<f32>(deref(read<ptr<f32>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f32>(deref(read<ptr<f32>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_double:[0-9]+]] @test_double(%[[VALUE_x_3:[0-9]+]] x: f64) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{xmm5}" [{xmm5}] width 64 place<f64>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f64>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_double_from_mem:[0-9]+]] @test_double_from_mem(%[[VALUE_x_4:[0-9]+]] x: ptr<f64>) -> f64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{xmm5}" [{xmm5}] width 64 place<f64>(deref(read<ptr<f64>>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<f64>(deref(read<ptr<f64>>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
