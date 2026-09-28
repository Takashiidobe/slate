/* { dg-do compile } */

/* Test that asm with no outputs are treated as volatile.  */

void f(int x)
{
  __asm__ ("extended asm not discarded" : : "r" (x));
}

void g (void)
{
  __asm__ ("simple asm not discarded");
}
/* { dg-final { scan-assembler "extended asm not discarded" } } */
/* { dg-final { scan-assembler "simple asm not discarded" } } */

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
// DEFAULT-NEXT:     fn %0 @f(%1 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "extended asm not discarded" [dialect=att] [options=nostack] {
// DEFAULT-NEXT:             template: "extended asm not discarded";
// DEFAULT-NEXT:             in 0 "r" [reg] width 32 read<i32>(%1);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm "simple asm not discarded" [dialect=att] [options=nostack];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
