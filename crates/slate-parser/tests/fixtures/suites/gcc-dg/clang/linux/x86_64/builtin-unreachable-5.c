/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized" } */

int
foo (int a)
{
  if (a <= 0)
    {
    L1:
      __builtin_unreachable ();
    }

  if (a > 2)
    goto L1;

  return a > 0;
}

/* { dg-final { scan-tree-dump-times "if \\(" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-times "goto" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-times "L1:" 0 "optimized" } } */
/* { dg-final { scan-tree-dump-times "__builtin_unreachable" 0 "optimized" } } */

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
// DEFAULT-NEXT:     fn %3 @__builtin_unreachable() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @foo(%2 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if le<i32>(read<i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %1 L1:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%2), const<i32>(2))
// DEFAULT-NEXT:             goto %1;
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(gt<i32>(read<i32>(%2), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
