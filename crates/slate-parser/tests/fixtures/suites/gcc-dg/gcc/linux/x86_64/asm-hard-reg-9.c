/* { dg-do compile { target { s390*-*-* || { x86_64-*-* && lp64 } } } } */
/* { dg-options "-O2" } */

/* Ensure that if the reload register for operand 2 is resued for operand 3,
   that exclude start hard regs coming from operand 3 are taken into account.
   Otherwise a different register than r8 may be chosen rendering the insn
   after LRA unsatisfiable.  */

long
test ()
{
  long x;
  __asm__ ("" : "=r" (x) : "0" (1000), "r" (0l), "{r8}" (0l));
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
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i64 [storage=automatic];
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<i64>(%[[VALUE_x]]) from const<i32>(1000);
// DEFAULT-NEXT:             in 1 "r" [reg] width 64 const<i64>(0);
// DEFAULT-NEXT:             in 2 "{r8}" [{r8}] width 64 const<i64>(0);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i64>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
