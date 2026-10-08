/* { dg-do compile { target { { aarch64*-*-* powerpc64*-*-* riscv64-*-* s390*-*-* x86_64-*-* } && int128 } } } */
/* { dg-options "-O2" } get rid of -ansi since we use __int128 */

#if defined (__aarch64__)
# define REG "{x4}"
/* { dg-final { scan-assembler-times "foo\tx4" 1 { target { aarch64*-*-* } } } } */
#elif defined (__powerpc__) || defined (__POWERPC__)
# define REG "{r5}"
/* { dg-final { scan-assembler-times "foo\t5" 1 { target { powerpc*-*-* } } } } */
#elif defined (__riscv)
# define REG "{t5}"
/* { dg-final { scan-assembler-times "foo\tt5" 1 { target { riscv*-*-* } } } } */
#elif defined (__s390__)
# define REG "{r4}"
/* { dg-final { scan-assembler-times "foo\t%r4" 1 { target { s390*-*-* } } } } */
#elif defined (__x86_64__)
# define REG "{xmm0}"
/* { dg-final { scan-assembler-times "foo\t%xmm0" 1 { target { x86_64-*-* } } } } */
#endif

void
test (void)
{
  __asm__ ("foo\t%0" :: REG ((__int128) 42));
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
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "foo\t%0" [dialect=att] [options=nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\t" %0;
// DEFAULT-NEXT:             in 0 "{xmm0}" [{xmm0}] width 128 widen<i128, reason=explicit>(const<i32>(42));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
