// SLATE-FILECHECK-DEFINES DEFAULT

/* PR target/7370.  */
/* { dg-require-stack-size "4000 + 8" } */

int g (int *x, int *y);

void f ()
{
  int x, y;
  char a[4000];

  g (&x, &y);
  x = x/y + x;
}

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
// DEFAULT-NEXT:     fn %0 @g(%5 x: ptr<i32>, %6 y: ptr<i32>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %3 y: i32 [storage=automatic];
// DEFAULT-NEXT:         let %4 a: array<i8, 4000> [storage=automatic];
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i32>, ptr<i32>) -> i32>(%0, addr_of<ptr<i32>>(%2), addr_of<ptr<i32>>(%3));
// DEFAULT-NEXT:         write<i32>(%2, add<i32, overflow=ub>(div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%2), read<i32>(%3)), read<i32>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
