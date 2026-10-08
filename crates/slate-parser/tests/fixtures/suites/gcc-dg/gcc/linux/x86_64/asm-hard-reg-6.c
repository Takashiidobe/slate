/* { dg-do compile { target aarch64*-*-* arm*-*-* i?86-*-* powerpc*-*-* riscv*-*-* s390*-*-* x86_64-*-* } } */
/* { dg-options "-O2" } */

/* Test multiple alternatives.  */

#if defined (__aarch64__)
# define GPR1 "{x1}"
# define GPR2 "{x2}"
# define GPR3 "{x3}"
/* { dg-final { scan-assembler-times "foo\tx1,x3" 1 { target { aarch64*-*-* } } } } */
/* { dg-final { scan-assembler-times "bar\tx2,\\\[x1\\\]" 1 { target { aarch64*-*-* } } } } */
#elif defined (__arm__)
# define GPR1 "{r1}"
# define GPR2 "{r2}"
# define GPR3 "{r3}"
/* { dg-final { scan-assembler-times "foo\tr1,r3" 1 { target { arm*-*-* } } } } */
/* { dg-final { scan-assembler-times "bar\tr2,\\\[r1\\\]" 1 { target { arm*-*-* } } } } */
#elif defined (__i386__)
# define GPR1 "{eax}"
# define GPR2 "{ebx}"
# define GPR3 "{ecx}"
/* { dg-final { scan-assembler-times "foo\t4\\(%esp\\),%ecx" 1 { target { { i?86-*-* x86_64-*-* } && { ia32 } } } } } */
/* { dg-final { scan-assembler-times "bar\t%ebx,\\(%eax\\)" 1 { target { { i?86-*-* x86_64-*-* } && { ia32 } } } } } */
#elif defined (__powerpc__) || defined (__POWERPC__)
# define GPR1 "{r4}"
# define GPR2 "{r5}"
# define GPR3 "{r6}"
/* { dg-final { scan-assembler-times "foo\t4,6" 1 { target { powerpc*-*-* } } } } */
/* { dg-final { scan-assembler-times "bar\t5,0\\(4\\)" 1 { target { powerpc*-*-* } } } } */
#elif defined (__riscv)
# define GPR1 "{t1}"
# define GPR2 "{t2}"
# define GPR3 "{t3}"
/* { dg-final { scan-assembler-times "foo\tt1,t3" 1 { target { riscv*-*-* } } } } */
/* { dg-final { scan-assembler-times "bar\tt2,0\\(a1\\)" 1 { target { riscv*-*-* } } } } */
#elif defined (__s390__)
# define GPR1 "{r0}"
# define GPR2 "{r1}"
# define GPR3 "{r2}"
/* { dg-final { scan-assembler-times "foo\t%r0,%r2" 1 { target { s390*-*-* } } } } */
/* { dg-final { scan-assembler-times "bar\t%r1,0\\(%r3\\)" 1 { target { s390*-*-* } } } } */
#elif defined (__x86_64__)
# define GPR1 "{eax}"
# define GPR2 "{ebx}"
# define GPR3 "{rcx}"
/* { dg-final { scan-assembler-times "foo\t%eax,%rcx" 1 { target { { i?86-*-* x86_64-*-* } && lp64 } } } } */
/* { dg-final { scan-assembler-times "bar\t%ebx,\\(%rsi\\)" 1 { target { { i?86-*-* x86_64-*-* } && lp64 } } } } */
/* { dg-final { scan-assembler-times "foo\t%eax,%ecx" 1 { target { { i?86-*-* x86_64-*-* } && x32 } } } } */
/* { dg-final { scan-assembler-times "bar\t%ebx,\\(%esi\\)" 1 { target { { i?86-*-* x86_64-*-* } && x32 } } } } */
#endif

void
test_reg_reg (int x, long long *y)
{
  __asm__ ("foo\t%0,%1" :: GPR1"m,"GPR2 (x), GPR3",m" (y));
}

void
test_reg_mem (int x, long long *y)
{
  __asm__ ("bar\t%0,%1" :: GPR1"m,"GPR2 (x), GPR3",m" (*y));
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
// DEFAULT-NEXT:     fn %[[VALUE_test_reg_reg:[0-9]+]] @test_reg_reg(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "foo\t%0,%1" [dialect=att] [options=nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             template: "foo\t" %0 "," %1;
// DEFAULT-NEXT:             in 0 "{eax}m,{ebx}" [unresolved("{") | imm | {ax} | xmm_reg | unresolved("}") | mem, {bx}] -> xmm_reg width 32 read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             in 1 "{rcx},m" [{cx}, mem] width 64 read<ptr<i64>>(%[[VALUE_y]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_reg_mem:[0-9]+]] @test_reg_mem(%[[VALUE_x_2:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: ptr<i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "bar\t%0,%1" [dialect=att] [options=nomem,nostack] [alternative=0] {
// DEFAULT-NEXT:             template: "bar\t" %0 "," %1;
// DEFAULT-NEXT:             in 0 "{eax}m,{ebx}" [unresolved("{") | imm | {ax} | xmm_reg | unresolved("}") | mem, {bx}] -> xmm_reg width 32 read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             in 1 "{rcx},m" [{cx}, mem] width 64 read<i64>(deref(read<ptr<i64>>(%[[VALUE_y_2]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
