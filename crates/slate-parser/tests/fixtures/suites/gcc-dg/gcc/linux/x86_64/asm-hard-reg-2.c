/* { dg-do compile { target { { aarch64*-*-* powerpc64*-*-* riscv64-*-* s390*-*-* } || { { i?86-*-* x86_64-*-* } && { ! ia32 } } } } } */
/* { dg-options "-std=c99" } we need long long */

#if defined (__aarch64__)
# define GPR "{x4}"
/* { dg-final { scan-assembler-times "foo\tx4" 2 { target { aarch64*-*-* } } } } */
#elif defined (__powerpc__) || defined (__POWERPC__)
# define GPR "{r5}"
/* { dg-final { scan-assembler-times "foo\t5" 2 { target { powerpc64*-*-* } } } } */
#elif defined (__riscv)
# define GPR "{t5}"
/* { dg-final { scan-assembler-times "foo\tt5" 2 { target { riscv64-*-* } } } } */
#elif defined (__s390__)
# define GPR "{r4}"
/* { dg-final { scan-assembler-times "foo\t%r4" 2 { target { s390*-*-* } } } } */
#elif defined (__x86_64__)
# define GPR "{rcx}"
/* { dg-final { scan-assembler-times "foo\t%rcx" 2 { target { i?86-*-* x86_64-*-* } } } } */
#endif

long long
test_longlong (long long x)
{
  __asm__ ("foo\t%0" : "+"GPR (x));
  return x;
}

long long
test_longlong_from_mem (long long *x)
{
  __asm__ ("foo\t%0" : "+"GPR (*x));
  return *x;
}

// SLATE-FILECHECK-STD DEFAULT c99
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
// DEFAULT-NEXT:     fn %[[VALUE_test_longlong:[0-9]+]] @test_longlong(%[[VALUE_x:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 64 place<i64>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_longlong_from_mem:[0-9]+]] @test_longlong_from_mem(%[[VALUE_x_2:[0-9]+]] x: ptr<i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 64 place<i64>(deref(read<ptr<i64>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i64>(deref(read<ptr<i64>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
