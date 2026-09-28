/* { dg-do compile { target { { aarch64*-*-* powerpc64*-*-* riscv64-*-* s390*-*-* } || { { i?86-*-* x86_64-*-* } && { ! ia32 } } } } } */

typedef int V __attribute__ ((vector_size (4 * sizeof (int))));

#if defined (__aarch64__)
# define VR "{v20}"
/* { dg-final { scan-assembler-times "foo\tv20" 2 { target { aarch64*-*-* } } } } */
#elif defined (__powerpc__) || defined (__POWERPC__)
# define VR "{v5}"
/* { dg-final { scan-assembler-times "foo\t5" 2 { target { powerpc64*-*-* } } } } */
#elif defined (__riscv)
# define VR "{v5}"
/* { dg-additional-options "-march=rv64imv" { target riscv64-*-* } } */
/* { dg-final { scan-assembler-times "foo\tv5" 2 { target { riscv*-*-* } } } } */
#elif defined (__s390__)
# define VR "{v5}"
/* { dg-require-effective-target s390_mvx { target s390*-*-* } } */
/* { dg-final { scan-assembler-times "foo\t%v5" 2 { target s390*-*-* } } } */
#elif defined (__x86_64__)
# define VR "{xmm9}"
/* { dg-final { scan-assembler-times "foo\t%xmm9" 2 { target { x86_64-*-* } } } } */
#endif

V
test (V x)
{
  __asm__ ("foo\t%0" : "+"VR (x));
  return x;
}

V
test_from_mem (V *x)
{
  __asm__ ("foo\t%0" : "+"VR (*x));
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
// DEFAULT-NEXT:     type @type0 V = vector<i32, 4>;
// DEFAULT-NEXT:     fn %1 @test(%2 x: vector<i32, 4>) -> vector<i32, 4> [linkage=external] [abi=sysv64(direct) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{xmm9}" [{xmm9}] width 128 place<vector<i32, 4>>(%2);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<vector<i32, 4>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @test_from_mem(%4 x: ptr<vector<i32, 4>>) -> vector<i32, 4> [linkage=external] [abi=sysv64(scalar) -> direct] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         asm "foo\\t%0" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             template: "foo\\t" %0;
// DEFAULT-NEXT:             inlateout 0 "{xmm9}" [{xmm9}] width 128 place<vector<i32, 4>>(deref(read<ptr<vector<i32, 4>>>(%4)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<vector<i32, 4>>(deref(read<ptr<vector<i32, 4>>>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
