/* The same test as 921011-1.c.  It can fails on gcc 4.1 due to a dfs
   traversal of the loops after versioning.  */

/* { dg-do compile } */
/* { dg-options "-Wno-old-style-definition -O2 -fmodulo-sched -fdump-rtl-sms" } */

int th, h, em, nlwm, nlwS, nlw, sy;
void
fun (nb)
     int nb;
{

  while (nb--)
    while (h--)
      {
	nlw = nlwm;
	while (nlw)
	  {
	    if (nlwS == 1)
	      {
	      }
	    else
	      if (nlwS == 1)
		{
		}
	    nlwS--; nlw--;
	  }
	if (em)
	  nlwS--;
	if (++sy == th)
	  sy = 0;
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
// DEFAULT-NEXT:     global %[[VALUE_th:[0-9]+]] th: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_em:[0-9]+]] em: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_nlwm:[0-9]+]] nlwm: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_nlwS:[0-9]+]] nlwS: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_nlw:[0-9]+]] nlw: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_sy:[0-9]+]] sy: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fun:[0-9]+]] @fun(%[[VALUE_nb:[0-9]+]] nb: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_nb]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_nb]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_h]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_h]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield ne<i32>(read<i32>(%[[VALUE4]]), const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_nlw]], read<i32>(%[[VALUE_nlwm]]));
// DEFAULT-NEXT:                     while %[[VALUE6:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_nlw]]), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_nlwS]]), const<i32>(1))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if eq<i32>(read<i32>(%[[VALUE_nlwS]]), const<i32>(1))
// DEFAULT-NEXT:                                     {
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_nlwS]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_nlwS]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_nlw]]);
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_nlw]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_em]]), const<i32>(0))
// DEFAULT-NEXT:                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_nlwS]]);
// DEFAULT-NEXT:                         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_nlwS]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                     let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_sy]]);
// DEFAULT-NEXT:                     let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_sy]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                     if eq<i32>(read<i32>(%[[VALUE14]]), read<i32>(%[[VALUE_th]]))
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_sy]], const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
