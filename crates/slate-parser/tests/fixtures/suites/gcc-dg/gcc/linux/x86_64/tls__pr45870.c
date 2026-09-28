/* PR target/45870 */
/* { dg-do compile } */
/* { dg-options "-g -O" } */
/* { dg-require-effective-target tls } */

__thread int v[30];
int bar (void);

int
foo (int x, int y, int z)
{
  int a, b = z, c;
  while (b > 0)
    {
      c = (bar () % 3);
      a = v[x];
      if (x < y)
	for (;;);
      b += a;
    }
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
// DEFAULT-NEXT:     global %0 v: array<i32, 30> [storage=thread] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: i32, %4 y: i32, %5 z: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 b: i32 [storage=automatic] = read<i32>(%5);
// DEFAULT-NEXT:         let %8 c: i32 [storage=automatic];
// DEFAULT-NEXT:         while %9 gt<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%8, rem<i32, by_zero=ub, min_by_neg_one=ub>(call<i32, signature=fn() -> i32>(%1), const<i32>(3)));
// DEFAULT-NEXT:                 rem<i32, by_zero=ub, min_by_neg_one=ub>(call<i32, signature=fn() -> i32>(%1), const<i32>(3));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(30)>(%0), read<i32>(%3)))));
// DEFAULT-NEXT:                 if lt<i32>(read<i32>(%3), read<i32>(%4))
// DEFAULT-NEXT:                     for %10
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                         condition: omitted
// DEFAULT-NEXT:                         increment: omitted
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                 let %11: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), read<i32>(%6));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%12));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
