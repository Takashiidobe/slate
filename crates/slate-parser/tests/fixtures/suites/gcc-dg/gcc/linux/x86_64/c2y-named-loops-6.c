/* N3355 - Named loops.  */
/* { dg-do compile } */
/* { dg-options "-std=c2y -Wall" } */

void
foo (int x)
{
 lab0:
  switch (x)
    {
    case 1:
      ++x;
      /* FALLTHRU */
    lab1:
    case 2:
      /* FALLTHRU */
    case 3:
    lab2:
      for (int i = 0; i < 4; ++i)
	if (i == 0)
	  continue lab2;
	else if (i == 1)
	  continue lab1;
	else if (x == 2)
	  break lab1;
	else
	  break lab0;
      break;
    default:
      break;
    }
}

// SLATE-FILECHECK-STD DEFAULT c2y
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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         label %[[VALUE_lab0:[0-9]+]] lab0:
// DEFAULT-NEXT:             switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_x]])
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     case %[[VALUE0]] const<i32>(1):
// DEFAULT-NEXT:                         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                     label %[[VALUE_lab1:[0-9]+]] lab1:
// DEFAULT-NEXT:                         case %[[VALUE0]] const<i32>(2):
// DEFAULT-NEXT:                             case %[[VALUE0]] const<i32>(3):
// DEFAULT-NEXT:                                 label %[[VALUE_lab2:[0-9]+]] lab2:
// DEFAULT-NEXT:                                     for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                                         init:
// DEFAULT-NEXT:                                             let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                                         condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:                                         increment: {
// DEFAULT-NEXT:                                             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                                             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                                             write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                                             yield void;
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                                         body:
// DEFAULT-NEXT:                                             if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:                                                 continue %[[VALUE3]];
// DEFAULT-NEXT:                                             else
// DEFAULT-NEXT:                                                 if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:                                                     continue %[[VALUE3]];
// DEFAULT-NEXT:                                                 else
// DEFAULT-NEXT:                                                     if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(2))
// DEFAULT-NEXT:                                                         break %[[VALUE3]];
// DEFAULT-NEXT:                                                     else
// DEFAULT-NEXT:                                                         break %[[VALUE0]];
// DEFAULT-NEXT:                     break %[[VALUE0]];
// DEFAULT-NEXT:                     default %[[VALUE0]]:
// DEFAULT-NEXT:                         break %[[VALUE0]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
