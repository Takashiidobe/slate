/* { dg-do compile { target aarch64*-*-* arm*-*-* i?86-*-* powerpc*-*-* riscv*-*-* s390*-*-* x86_64-*-* } } */
/* { dg-options "-O2" } */

/* Test multiple alternatives.  */

#if defined (__aarch64__)
# define GPR "{x1}"
/* { dg-final { scan-assembler-times "foo\tx1,x1" 2 { target { aarch64*-*-* } } } } */
#elif defined (__arm__)
# define GPR "{r1}"
/* { dg-final { scan-assembler-times "foo\tr1,r1" 2 { target { arm*-*-* } } } } */
#elif defined (__i386__)
# define GPR "{eax}"
/* { dg-final { scan-assembler-times "foo\t%eax,%eax" 2 { target { i?86-*-* } } } } */
#elif defined (__powerpc__) || defined (__POWERPC__)
# define GPR "{r4}"
/* { dg-final { scan-assembler-times "foo\t4,4" 2 { target { powerpc*-*-* } } } } */
#elif defined (__riscv)
# define GPR "{t1}"
/* { dg-final { scan-assembler-times "foo\tt1,t1" 2 { target { riscv*-*-* } } } } */
#elif defined (__s390__)
# define GPR "{r0}"
/* { dg-final { scan-assembler-times "foo\t%r0,%r0" 2 { target { s390*-*-* } } } } */
#elif defined (__x86_64__)
# define GPR "{eax}"
/* { dg-final { scan-assembler-times "foo\t%eax,%eax" 2 { target { x86_64-*-* } } } } */
#endif

int
test_1 (int x)
{
  __asm__ ("foo\t%0,%0" : "+"GPR (x));
  return x;
}

int
test_2 (int x, int y)
{
  __asm__ ("foo\t%0,%1" : "="GPR (x) : GPR (y));
  return x;
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
// DEFAULT-NEXT:     fn %[[VALUE_test_1:[0-9]+]] @test_1(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\t%0,%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\t" %0 "," %0;
// DEFAULT-NEXT:             inlateout 0 "{eax}" [{ax}] width 32 place<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_2:[0-9]+]] @test_2(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\t%0,%1" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\t" %0 "," %1;
// DEFAULT-NEXT:             lateout 0 "{eax}" [{ax}] width 32 place<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             in 1 "{eax}" [{ax}] width 32 read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
