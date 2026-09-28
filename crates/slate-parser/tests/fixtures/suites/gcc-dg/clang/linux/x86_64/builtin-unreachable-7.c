/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-optimized -fno-tree-dominator-opts -fno-tree-vrp" } */
/* { dg-require-effective-target label_values } */

void foo (int b, int c)
{
  __label__ lab;
  __label__ lab2;
  static void *x[2] = {&&lab, &&lab2};
  if (b == c)
    {
lab:
      __builtin_unreachable ();
    }
lab2:
  goto *x[c!=0];
}

/* Fab should still able to remove the conditional but leave the bb there. */

/* { dg-final { scan-tree-dump-times "lab:" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-times "__builtin_unreachable" 1 "optimized" } } */
/* { dg-final { scan-tree-dump-not "if " "optimized" } } */

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
// DEFAULT-NEXT:     global %5 x: array<ptr<void>, 2> [storage=static] [align=16] = aggregate<array<ptr<void>, 2>, zero_fill=false>(index0 = label_addr<ptr<void>>(%1), index1 = label_addr<ptr<void>>(%2)) [linkage=internal];
// DEFAULT-NEXT:     fn %6 @__builtin_unreachable() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @foo(%3 b: i32, %4 c: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%3), read<i32>(%4))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 label %1 lab:
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         label %2 lab2:
// DEFAULT-NEXT:             goto *read<ptr<void>>(deref(ptr_offset<ptr<ptr<void>>, subtract=false, element=ptr<void>, overflow=ub>(array_decay<ptr<ptr<void>>, length=Some(2)>(%5), from_bool<i32, reason=promotion>(ne<i32>(read<i32>(%4), const<i32>(0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
