/* { dg-do compile { target aarch64*-*-* arm*-*-* i?86-*-* powerpc*-*-* riscv*-*-* s390*-*-* x86_64-*-* } } */

#if defined (__aarch64__)
# define GPR "{x4}"
/* { dg-final { scan-assembler-times "foo\tx4" 8 { target { aarch64*-*-* } } } } */
#elif defined (__arm__)
# define GPR "{r4}"
/* { dg-final { scan-assembler-times "foo\tr4" 8 { target { arm*-*-* } } } } */
#elif defined (__i386__)
# define GPR "{ecx}"
#elif defined (__x86_64__)
# define GPR "{rcx}"
/* { dg-final { scan-assembler-times "foo\t%cl" 2 { target x86 } } } */
/* { dg-final { scan-assembler-times "foo\t%cx" 2 { target x86 } } } */
/* { dg-final { scan-assembler-times "foo\t%ecx" 4 { target { x86 && ilp32 } } } } */
/* { dg-final { scan-assembler-times "foo\t%ecx" 2 { target { x86 && lp64 } } } } */
/* { dg-final { scan-assembler-times "foo\t%rcx" 2 { target { x86 && lp64 } } } } */
#elif defined (__powerpc__) || defined (__POWERPC__)
# define GPR "{r5}"
/* { dg-final { scan-assembler-times "foo\t5" 8 { target { powerpc*-*-* } } } } */
#elif defined (__riscv)
# define GPR "{t5}"
/* { dg-final { scan-assembler-times "foo\tt5" 8 { target { riscv*-*-* } } } } */
#elif defined (__s390__)
# define GPR "{r4}"
/* { dg-final { scan-assembler-times "foo\t%r4" 8 { target { s390*-*-* } } } } */
#endif

char
test_char (char x)
{
  __asm__ ("foo\t%0" : "+"GPR (x));
  return x;
}

char
test_char_from_mem (char *x)
{
  __asm__ ("foo\t%0" : "+"GPR (*x));
  return *x;
}

short
test_short (short x)
{
  __asm__ ("foo\t%0" : "+"GPR (x));
  return x;
}

short
test_short_from_mem (short *x)
{
  __asm__ ("foo\t%0" : "+"GPR (*x));
  return *x;
}

int
test_int (int x)
{
  __asm__ ("foo\t%0" : "+"GPR (x));
  return x;
}

int
test_int_from_mem (int *x)
{
  __asm__ ("foo\t%0" : "+"GPR (*x));
  return *x;
}

long
test_long (long x)
{
  __asm__ ("foo\t%0" : "+"GPR (x));
  return x;
}

long
test_long_from_mem (long *x)
{
  __asm__ ("foo\t%0" : "+"GPR (*x));
  return *x;
}

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
// DEFAULT-NEXT:     fn %[[VALUE_test_char:[0-9]+]] @test_char(%[[VALUE_x:[0-9]+]] x: i8) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 8 place<i8>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i8>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_char_from_mem:[0-9]+]] @test_char_from_mem(%[[VALUE_x_2:[0-9]+]] x: ptr<i8>) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 8 place<i8>(deref(read<ptr<i8>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i8>(deref(read<ptr<i8>>(%[[VALUE_x_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_short:[0-9]+]] @test_short(%[[VALUE_x_3:[0-9]+]] x: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 16 place<i16>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i16>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_short_from_mem:[0-9]+]] @test_short_from_mem(%[[VALUE_x_4:[0-9]+]] x: ptr<i16>) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 16 place<i16>(deref(read<ptr<i16>>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i16>(deref(read<ptr<i16>>(%[[VALUE_x_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_int:[0-9]+]] @test_int(%[[VALUE_x_5:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 32 place<i32>(%[[VALUE_x_5]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_int_from_mem:[0-9]+]] @test_int_from_mem(%[[VALUE_x_6:[0-9]+]] x: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 32 place<i32>(deref(read<ptr<i32>>(%[[VALUE_x_6]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%[[VALUE_x_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_long:[0-9]+]] @test_long(%[[VALUE_x_7:[0-9]+]] x: i64) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 64 place<i64>(%[[VALUE_x_7]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_x_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_long_from_mem:[0-9]+]] @test_long_from_mem(%[[VALUE_x_8:[0-9]+]] x: ptr<i64>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{rcx}" [{cx}] width 64 place<i64>(deref(read<ptr<i64>>(%[[VALUE_x_8]])));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i64>(deref(read<ptr<i64>>(%[[VALUE_x_8]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
