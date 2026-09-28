/* { dg-do compile { target aarch64*-*-* arm*-*-* i?86-*-* powerpc*-*-* riscv*-*-* s390*-*-* x86_64-*-* } } */

/* Due to hard register constraints, X must be copied.  */

#if defined (__aarch64__)
# define GPR1 "{x1}"
# define GPR2 "{x2}"
#elif defined (__arm__)
# define GPR1 "{r1}"
# define GPR2 "{r2}"
#elif defined (__i386__)
# define GPR1 "{eax}"
# define GPR2 "{ebx}"
#elif defined (__powerpc__) || defined (__POWERPC__)
# define GPR1 "{r4}"
# define GPR2 "{r5}"
#elif defined (__riscv)
# define GPR1 "{t1}"
# define GPR2 "{t2}"
#elif defined (__s390__)
# define GPR1 "{r0}"
# define GPR2 "{r1}"
#elif defined (__x86_64__)
# define GPR1 "{eax}"
# define GPR2 "{ebx}"
#endif

#define TEST(T) \
int \
test_##T (T x) \
{ \
  int out; \
  __asm__ ("foo" : "=r" (out) : GPR1 (x), GPR2 (x)); \
  return out; \
}

TEST(char)
TEST(short)
TEST(int)
TEST(long)

int
test_subreg (long x)
{
  int out;
  short subreg_x = x;
  __asm__ ("foo" : "=r" (out) : GPR1 (x), GPR2 (subreg_x));
  return out;
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
// DEFAULT-NEXT:     fn %0 @test_char(%1 x: i8) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 out: i32 [storage=automatic];
// DEFAULT-NEXT:         asm "foo" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo";
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%2);
// DEFAULT-NEXT:             in 1 "{eax}" [{ax}] width 8 read<i8>(%1);
// DEFAULT-NEXT:             in 2 "{ebx}" [{bx}] width 8 read<i8>(%1);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test_short(%4 x: i16) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 out: i32 [storage=automatic];
// DEFAULT-NEXT:         asm "foo" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo";
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%5);
// DEFAULT-NEXT:             in 1 "{eax}" [{ax}] width 16 read<i16>(%4);
// DEFAULT-NEXT:             in 2 "{ebx}" [{bx}] width 16 read<i16>(%4);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_int(%7 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 out: i32 [storage=automatic];
// DEFAULT-NEXT:         asm "foo" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo";
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%8);
// DEFAULT-NEXT:             in 1 "{eax}" [{ax}] width 32 read<i32>(%7);
// DEFAULT-NEXT:             in 2 "{ebx}" [{bx}] width 32 read<i32>(%7);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_long(%10 x: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 out: i32 [storage=automatic];
// DEFAULT-NEXT:         asm "foo" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo";
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%11);
// DEFAULT-NEXT:             in 1 "{eax}" [{ax}] width 64 read<i64>(%10);
// DEFAULT-NEXT:             in 2 "{ebx}" [{bx}] width 64 read<i64>(%10);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_subreg(%13 x: i64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 out: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 subreg_x: i16 [storage=automatic] = truncate<i16, reason=assign, fits=unknown>(read<i64>(%13));
// DEFAULT-NEXT:         asm "foo" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo";
// DEFAULT-NEXT:             lateout 0 "r" [reg] width 32 place<i32>(%14);
// DEFAULT-NEXT:             in 1 "{eax}" [{ax}] width 64 read<i64>(%13);
// DEFAULT-NEXT:             in 2 "{ebx}" [{bx}] width 16 read<i16>(%15);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
