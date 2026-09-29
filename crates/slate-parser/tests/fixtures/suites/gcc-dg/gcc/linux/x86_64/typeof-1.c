/* Test typeof with __asm redirection. */
/* { dg-do compile } */
/* { dg-options "-O2" } */

extern int foo1;
extern int foo1 __asm ("bar1");
int foo1 = 1;

extern int foo2 (int);
extern int foo2 (int) __asm ("bar2");
int foo2 (int x)
{
  return x;
}

extern int foo3;
extern __typeof (foo3) foo3 __asm ("bar3");
int foo3 = 1;

extern int foo4 (int);
extern __typeof (foo4) foo4 __asm ("bar4");
int foo4 (int x)
{
  return x;
}

// { dg-final { scan-assembler-not "foo" } }

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
// DEFAULT-NEXT:     global %[[VALUE_foo1:[0-9]+]] foo1: i32 [storage=static] = const<i32>(1) [linkage=external] [asm_name="bar1"];
// DEFAULT-NEXT:     global %[[VALUE_foo3:[0-9]+]] foo3: i32 [storage=static] = const<i32>(1) [linkage=external] [asm_name="bar3"];
// DEFAULT-NEXT:     fn %[[VALUE_foo2:[0-9]+]] @foo2(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [asm_name="bar2"] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo4:[0-9]+]] @foo4(%[[VALUE_x_2:[0-9]+]] x: i32) -> i32 [linkage=external] [asm_name="bar4"] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
