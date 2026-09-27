/* Test C23 attribute syntax.  Invalid use of fallthrough attribute in
   bad context inside switch.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors -Wextra" } */

int
f (int a)
{
  switch (a)
    {
    case 1:
      a++;
      [[fallthrough]]; /* { dg-error "attribute 'fallthrough' not preceding a case label or default label" } */
      a++;
      [[fallthrough]]; /* { dg-error "attribute 'fallthrough' not preceding a case label or default label" } */
    }
  return a;
}

// SLATE-FILECHECK-FLAVOR gcc
// SLATE-FILECHECK-STD DEFAULT c23
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
// DEFAULT-NEXT:     fn %0 @f(%1 a: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %2 read<i32>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %2 const<i32>(1):
// DEFAULT-NEXT:                     let %3: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                     let %4: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%3), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 let %5: i32 [synthetic] = read<i32>(%1);
// DEFAULT-NEXT:                 let %6: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%5), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%1, read<i32>(%6));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
