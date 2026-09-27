/* { dg-do compile { target fpic } } */
/* { dg-options "-fpic -O2" } */

void f (char *s)
{
  for (;;)
    {
      int t = 6;
      switch (t)
	{
	case 2:
	  *s = '2';
	case 6: case 4: case 3: case 1:
	  break;
	}
    }
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     fn %0 @f(%1 s: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %3
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %2 t: i32 [storage=automatic] = const<i32>(6);
// DEFAULT-NEXT:                     switch %4 read<i32>(%2)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %4 const<i32>(2):
// DEFAULT-NEXT:                                 write<i8>(deref(read<ptr<i8>>(%1)), truncate<i8, reason=assign, fits=always>(const<i32>(50)));
// DEFAULT-NEXT:                             case %4 const<i32>(6):
// DEFAULT-NEXT:                                 case %4 const<i32>(4):
// DEFAULT-NEXT:                                     case %4 const<i32>(3):
// DEFAULT-NEXT:                                         case %4 const<i32>(1):
// DEFAULT-NEXT:                                             break %4;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
