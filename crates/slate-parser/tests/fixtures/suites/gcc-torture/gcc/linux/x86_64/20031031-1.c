// SLATE-FILECHECK-DEFINES DEFAULT

/* PR/11640 */

int
internal_insn_latency (int insn_code, int insn2_code)
{
  switch (insn_code)
    {
    case 256:
      switch (insn2_code)
	{
	case 267:
	  return 8;
	case 266:
	  return 8;
	case 265:
	  return 8;
	case 264:
	  return 8;
	case 263:
	  return 8;
	}
      break;
    case 273:
      switch (insn2_code)
	{
	case 267:
	  return 5;
	case 266:
	  return 5;
	case 277:
	  return 3;
	}
      break;
    }
  return 0;
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
// DEFAULT-NEXT:     fn %0 @internal_insn_latency(%1 insn_code: i32, %2 insn2_code: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %3 read<i32>(%1)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %3 const<i32>(256):
// DEFAULT-NEXT:                     switch %4 read<i32>(%2)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %4 const<i32>(267):
// DEFAULT-NEXT:                                 return const<i32>(8);
// DEFAULT-NEXT:                             case %4 const<i32>(266):
// DEFAULT-NEXT:                                 return const<i32>(8);
// DEFAULT-NEXT:                             case %4 const<i32>(265):
// DEFAULT-NEXT:                                 return const<i32>(8);
// DEFAULT-NEXT:                             case %4 const<i32>(264):
// DEFAULT-NEXT:                                 return const<i32>(8);
// DEFAULT-NEXT:                             case %4 const<i32>(263):
// DEFAULT-NEXT:                                 return const<i32>(8);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 break %3;
// DEFAULT-NEXT:                 case %3 const<i32>(273):
// DEFAULT-NEXT:                     switch %5 read<i32>(%2)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             case %5 const<i32>(267):
// DEFAULT-NEXT:                                 return const<i32>(5);
// DEFAULT-NEXT:                             case %5 const<i32>(266):
// DEFAULT-NEXT:                                 return const<i32>(5);
// DEFAULT-NEXT:                             case %5 const<i32>(277):
// DEFAULT-NEXT:                                 return const<i32>(3);
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 break %3;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
