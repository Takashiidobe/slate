/* Test C23 attribute syntax.  Valid use of fallthrough attribute.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors -Wextra" } */

int
f (int a, int c)
{
  int b = 2;
  switch (a)
    {
    case 1:
      b = 1; /* { dg-warning "may fall through" } */
    case 2:
      b = 2;
      [[fallthrough]];
    case 3:
      b += 7;
      break;
    case 4:
      b = 5;
      [[__fallthrough__]];
    case 5:
      b += 1;
      break;
    case 6:
      if (c == 2)
	{
	  [[fallthrough]];
	}
      else
	{
	  [[fallthrough]];
	}
    case 7:
      b += 3;
      [[fallthrough]];
    default:
      b += 8;
      break;
    }
  return b;
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
// DEFAULT-NEXT:     fn %0 @f(%1 a: i32, %2 c: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 b: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         switch %4 read<i32>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %4 const<i32>(1):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(1));
// DEFAULT-NEXT:                 case %4 const<i32>(2):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(2));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %4 const<i32>(3):
// DEFAULT-NEXT:                     let %5: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %6: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%5), const<i32>(7));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%6));
// DEFAULT-NEXT:                 break %4;
// DEFAULT-NEXT:                 case %4 const<i32>(4):
// DEFAULT-NEXT:                     write<i32>(%3, const<i32>(5));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 case %4 const<i32>(5):
// DEFAULT-NEXT:                     let %7: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %8: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%8));
// DEFAULT-NEXT:                 break %4;
// DEFAULT-NEXT:                 case %4 const<i32>(6):
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%2), const<i32>(2))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 case %4 const<i32>(7):
// DEFAULT-NEXT:                     let %9: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(3));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%10));
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:                 default %4:
// DEFAULT-NEXT:                     let %11: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                     let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%11), const<i32>(8));
// DEFAULT-NEXT:                     write<i32>(%3, read<i32>(%12));
// DEFAULT-NEXT:                 break %4;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
