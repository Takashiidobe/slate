// SLATE-FILECHECK-DEFINES DEFAULT

extern char bpp;
int inb(int);

void foo()
{
  if (bpp == 32)
    {
      if (2 < 8)
	{
	  do
	    {
	      while (inb(0x9ae8) & (0x0100 >> (2 +1)));
	    }
	  while(0);
	}
      else
	{
	  do
	    {
	      while (inb(0x9ae8) & (0x0100 >> (2)));
	    }
	  while(0);
	}
    }
  else
    do
      { 
	while (inb(0x9ae8) & (0x0100 >> (1)));
      }
    while(0);
  if (8 < 8)
    {
      do
	{
	  while (inb(0x9ae8) & (0x0100 >> (8 +1)));
	}
      while(0);
    }
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
// DEFAULT-NEXT:     extern %[[VALUE_bpp:[0-9]+]] bpp: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_inb:[0-9]+]] @inb(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_bpp]])), const<i32>(32))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if lt<i32>(const<i32>(2), const<i32>(8))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 while %[[VALUE2:[0-9]+]] ne<i32>(and<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_inb]], const<i32>(39656)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(256), add<i32, overflow=ub>(const<i32>(2), const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 while %[[VALUE4:[0-9]+]] ne<i32>(and<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_inb]], const<i32>(39656)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(256), const<i32>(2))), const<i32>(0))
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     while %[[VALUE6:[0-9]+]] ne<i32>(and<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_inb]], const<i32>(39656)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(256), const<i32>(1))), const<i32>(0))
// DEFAULT-NEXT:                         ;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if lt<i32>(const<i32>(8), const<i32>(8))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         while %[[VALUE8:[0-9]+]] ne<i32>(and<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_inb]], const<i32>(39656)), shr<i32, amount_out_of_range=ub, fill=sign_extend>(const<i32>(256), add<i32, overflow=ub>(const<i32>(8), const<i32>(1)))), const<i32>(0))
// DEFAULT-NEXT:                             ;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
