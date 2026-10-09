/* N3355 - Named loops.  */
/* { dg-do compile } */
/* { dg-options "-std=c23 -pedantic-errors" } */

void
foo (int w)
{
  d: e: f:;
  a: b: c:
  for (int x = 0; x < 32; ++x)
    {
      if (x == 0)
	continue a;	/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
      else if (x == 1)
	continue b;	/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
      else if (x == 2)
	continue c;	/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
      else if (x == 31)
	break b;	/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    }
  int y = 0;
  g: h:
  #pragma GCC unroll 2
  while (y < 16)
    {
      ++y;
      if (y == 12)
	continue g;	/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
      else if (y == 13)
	continue h;	/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
      else if (y == 14)
	break g;	/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    }
  i: j:;
  k: l:
  switch (y)
    {
    case 6:
      break;
    case 7:
      break k;		/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    case 8:
      break l;		/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    }
  m: n: o: p:
  for (int x = 0; x < 2; ++x)
    q: r: s: t:
    switch (x)
      {
      case 0:
	u: v:
      case 3:
	w: x:
	for (int y = 0; y < 2; ++y)
	  y: z:
	  for (int z = 0; z < 2; ++z)
	    aa: ab: ac:
	    for (int a = 0; a < 2; ++a)
	      ad: ae: af:
	      switch (a)
		{
		case 0:
		  if (w == 0)
		    break ae;		/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
		  else if (w == 1)
		    break ab;		/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
		  else if (w == 2)
		    break z;		/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
		  else if (w == 3)
		    break v;		/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
		  else if (w == 4)
		    break s;		/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
		  else if (w == 5)
		    break p;		/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
		  else if (w == 6)
		    break;
		  else if (w == 7)
		    continue aa;	/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
		  else if (w == 8)
		    continue y;		/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
		  else if (w == 9)
		    continue x;		/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
		  else if (w == 10)
		    continue m;		/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
		  ag: ah:
		  do
		    {
		      if (w == 11)
			break ag;	/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
		      else
			continue ah;	/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
		    }
		  while (0);
		  break;
		default:
		  break;
		}
	break;
      default:
	break;
      }
  [[]] [[]] ai:
  [[]] [[]] aj:
  [[]] [[]] ak:
  [[]] [[]] [[]]
  for (int x = 0; x < 32; ++x)
    if (x == 31)
      break ak;				/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    else if (x == 30)
      break aj;				/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    else if (x == 29)
      continue ai;			/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
  al:
  [[]] am:
  [[]]
  do
    {
      if (w == 42)
	continue am;			/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
      else if (w == 41)
	break al;			/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    }
  while (1);
  an:
  [[]] ao:
  [[]] [[]]
  while (w)
    {
      if (w == 40)
	break ao;			/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
      else if (w == 39)
	continue an;			/* { dg-error "ISO C does not support 'continue' statement with an identifier operand before" } */
    }
  [[]] ap:
  [[]] aq:
  [[]]
  switch (w)
    {
    case 42:
      break ap;				/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    default:
      break aq;				/* { dg-error "ISO C does not support 'break' statement with an identifier operand before" } */
    }
}

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
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_w:[0-9]+]] w: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         label %[[VALUE_d:[0-9]+]] d:
// DEFAULT-NEXT:             label %[[VALUE_e:[0-9]+]] e:
// DEFAULT-NEXT:                 label %[[VALUE_f:[0-9]+]] f:
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:         label %[[VALUE_a:[0-9]+]] a:
// DEFAULT-NEXT:             label %[[VALUE_b:[0-9]+]] b:
// DEFAULT-NEXT:                 label %[[VALUE_c:[0-9]+]] c:
// DEFAULT-NEXT:                     for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(32))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// DEFAULT-NEXT:                                     continue %[[VALUE0]];
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(1))
// DEFAULT-NEXT:                                         continue %[[VALUE0]];
// DEFAULT-NEXT:                                     else
// DEFAULT-NEXT:                                         if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(2))
// DEFAULT-NEXT:                                             continue %[[VALUE0]];
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             if eq<i32>(read<i32>(%[[VALUE_x]]), const<i32>(31))
// DEFAULT-NEXT:                                                 break %[[VALUE0]];
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         label %[[VALUE_g:[0-9]+]] g:
// DEFAULT-NEXT:             label %[[VALUE_h:[0-9]+]] h:
// DEFAULT-NEXT:                 while %[[VALUE3:[0-9]+]] lt<i32>(read<i32>(%[[VALUE_y]]), const<i32>(16))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_y]]), const<i32>(12))
// DEFAULT-NEXT:                             continue %[[VALUE3]];
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_y]]), const<i32>(13))
// DEFAULT-NEXT:                                 continue %[[VALUE3]];
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if eq<i32>(read<i32>(%[[VALUE_y]]), const<i32>(14))
// DEFAULT-NEXT:                                     break %[[VALUE3]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         label %[[VALUE_i:[0-9]+]] i:
// DEFAULT-NEXT:             label %[[VALUE_j:[0-9]+]] j:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         label %[[VALUE_k:[0-9]+]] k:
// DEFAULT-NEXT:             label %[[VALUE_l:[0-9]+]] l:
// DEFAULT-NEXT:                 switch %[[VALUE6:[0-9]+]] read<i32>(%[[VALUE_y]])
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE6]] const<i32>(6):
// DEFAULT-NEXT:                             break %[[VALUE6]];
// DEFAULT-NEXT:                         case %[[VALUE6]] const<i32>(7):
// DEFAULT-NEXT:                             break %[[VALUE6]];
// DEFAULT-NEXT:                         case %[[VALUE6]] const<i32>(8):
// DEFAULT-NEXT:                             break %[[VALUE6]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         label %[[VALUE_m:[0-9]+]] m:
// DEFAULT-NEXT:             label %[[VALUE_n:[0-9]+]] n:
// DEFAULT-NEXT:                 label %[[VALUE_o:[0-9]+]] o:
// DEFAULT-NEXT:                     label %[[VALUE_p:[0-9]+]] p:
// DEFAULT-NEXT:                         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(2))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:                                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 label %[[VALUE_q:[0-9]+]] q:
// DEFAULT-NEXT:                                     label %[[VALUE_r:[0-9]+]] r:
// DEFAULT-NEXT:                                         label %[[VALUE_s:[0-9]+]] s:
// DEFAULT-NEXT:                                             label %[[VALUE_t:[0-9]+]] t:
// DEFAULT-NEXT:                                                 switch %[[VALUE10:[0-9]+]] read<i32>(%[[VALUE_x_2]])
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         case %[[VALUE10]] const<i32>(0):
// DEFAULT-NEXT:                                                             label %[[VALUE_u:[0-9]+]] u:
// DEFAULT-NEXT:                                                                 label %[[VALUE_v:[0-9]+]] v:
// DEFAULT-NEXT:                                                                     case %[[VALUE10]] const<i32>(3):
// DEFAULT-NEXT:                                                                         label %[[VALUE_w_2:[0-9]+]] w:
// DEFAULT-NEXT:                                                                             label %[[VALUE_x_3:[0-9]+]] x:
// DEFAULT-NEXT:                                                                                 for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                                                                                     init:
// DEFAULT-NEXT:                                                                                         let %[[VALUE_y_2:[0-9]+]] y: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                                                                                     condition: lt<i32>(read<i32>(%[[VALUE_y_2]]), const<i32>(2))
// DEFAULT-NEXT:                                                                                     increment: {
// DEFAULT-NEXT:                                                                                         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y_2]]);
// DEFAULT-NEXT:                                                                                         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                                                                                         write<i32>(%[[VALUE_y_2]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                                                                                         yield void;
// DEFAULT-NEXT:                                                                                     }
// DEFAULT-NEXT:                                                                                     body:
// DEFAULT-NEXT:                                                                                         label %[[VALUE_y_3:[0-9]+]] y:
// DEFAULT-NEXT:                                                                                             label %[[VALUE_z:[0-9]+]] z:
// DEFAULT-NEXT:                                                                                                 for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                                                                                                     init:
// DEFAULT-NEXT:                                                                                                         let %[[VALUE_z_2:[0-9]+]] z: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                                                                                                     condition: lt<i32>(read<i32>(%[[VALUE_z_2]]), const<i32>(2))
// DEFAULT-NEXT:                                                                                                     increment: {
// DEFAULT-NEXT:                                                                                                         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_z_2]]);
// DEFAULT-NEXT:                                                                                                         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                                                                                                         write<i32>(%[[VALUE_z_2]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                                                                                                         yield void;
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                     body:
// DEFAULT-NEXT:                                                                                                         label %[[VALUE_aa:[0-9]+]] aa:
// DEFAULT-NEXT:                                                                                                             label %[[VALUE_ab:[0-9]+]] ab:
// DEFAULT-NEXT:                                                                                                                 label %[[VALUE_ac:[0-9]+]] ac:
// DEFAULT-NEXT:                                                                                                                     for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                                                                                                                         init:
// DEFAULT-NEXT:                                                                                                                             let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                                                                                                                         condition: lt<i32>(read<i32>(%[[VALUE_a_2]]), const<i32>(2))
// DEFAULT-NEXT:                                                                                                                         increment: {
// DEFAULT-NEXT:                                                                                                                             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:                                                                                                                             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                                                                                                                             write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                                                                                                                             yield void;
// DEFAULT-NEXT:                                                                                                                         }
// DEFAULT-NEXT:                                                                                                                         body:
// DEFAULT-NEXT:                                                                                                                             label %[[VALUE_ad:[0-9]+]] ad:
// DEFAULT-NEXT:                                                                                                                                 label %[[VALUE_ae:[0-9]+]] ae:
// DEFAULT-NEXT:                                                                                                                                     label %[[VALUE_af:[0-9]+]] af:
// DEFAULT-NEXT:                                                                                                                                         switch %[[VALUE20:[0-9]+]] read<i32>(%[[VALUE_a_2]])
// DEFAULT-NEXT:                                                                                                                                             {
// DEFAULT-NEXT:                                                                                                                                                 case %[[VALUE20]] const<i32>(0):
// DEFAULT-NEXT:                                                                                                                                                     if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(0))
// DEFAULT-NEXT:                                                                                                                                                         break %[[VALUE20]];
// DEFAULT-NEXT:                                                                                                                                                     else
// DEFAULT-NEXT:                                                                                                                                                         if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(1))
// DEFAULT-NEXT:                                                                                                                                                             break %[[VALUE17]];
// DEFAULT-NEXT:                                                                                                                                                         else
// DEFAULT-NEXT:                                                                                                                                                             if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(2))
// DEFAULT-NEXT:                                                                                                                                                                 break %[[VALUE14]];
// DEFAULT-NEXT:                                                                                                                                                             else
// DEFAULT-NEXT:                                                                                                                                                                 if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(3))
// DEFAULT-NEXT:                                                                                                                                                                     break %[[VALUE11]];
// DEFAULT-NEXT:                                                                                                                                                                 else
// DEFAULT-NEXT:                                                                                                                                                                     if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(4))
// DEFAULT-NEXT:                                                                                                                                                                         break %[[VALUE10]];
// DEFAULT-NEXT:                                                                                                                                                                     else
// DEFAULT-NEXT:                                                                                                                                                                         if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(5))
// DEFAULT-NEXT:                                                                                                                                                                             break %[[VALUE7]];
// DEFAULT-NEXT:                                                                                                                                                                         else
// DEFAULT-NEXT:                                                                                                                                                                             if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(6))
// DEFAULT-NEXT:                                                                                                                                                                                 break %[[VALUE20]];
// DEFAULT-NEXT:                                                                                                                                                                             else
// DEFAULT-NEXT:                                                                                                                                                                                 if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(7))
// DEFAULT-NEXT:                                                                                                                                                                                     continue %[[VALUE17]];
// DEFAULT-NEXT:                                                                                                                                                                                 else
// DEFAULT-NEXT:                                                                                                                                                                                     if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(8))
// DEFAULT-NEXT:                                                                                                                                                                                         continue %[[VALUE14]];
// DEFAULT-NEXT:                                                                                                                                                                                     else
// DEFAULT-NEXT:                                                                                                                                                                                         if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(9))
// DEFAULT-NEXT:                                                                                                                                                                                             continue %[[VALUE11]];
// DEFAULT-NEXT:                                                                                                                                                                                         else
// DEFAULT-NEXT:                                                                                                                                                                                             if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(10))
// DEFAULT-NEXT:                                                                                                                                                                                                 continue %[[VALUE7]];
// DEFAULT-NEXT:                                                                                                                                                 label %[[VALUE_ag:[0-9]+]] ag:
// DEFAULT-NEXT:                                                                                                                                                     label %[[VALUE_ah:[0-9]+]] ah:
// DEFAULT-NEXT:                                                                                                                                                         do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:                                                                                                                                                             {
// DEFAULT-NEXT:                                                                                                                                                                 if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(11))
// DEFAULT-NEXT:                                                                                                                                                                     break %[[VALUE21]];
// DEFAULT-NEXT:                                                                                                                                                                 else
// DEFAULT-NEXT:                                                                                                                                                                     continue %[[VALUE21]];
// DEFAULT-NEXT:                                                                                                                                                             }
// DEFAULT-NEXT:                                                                                                                                                         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                                                                                                                                                 break %[[VALUE20]];
// DEFAULT-NEXT:                                                                                                                                                 default %[[VALUE20]]:
// DEFAULT-NEXT:                                                                                                                                                     break %[[VALUE20]];
// DEFAULT-NEXT:                                                                                                                                             }
// DEFAULT-NEXT:                                                         break %[[VALUE10]];
// DEFAULT-NEXT:                                                         default %[[VALUE10]]:
// DEFAULT-NEXT:                                                             break %[[VALUE10]];
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:         label %[[VALUE_ai:[0-9]+]] ai:
// DEFAULT-NEXT:             label %[[VALUE_aj:[0-9]+]] aj:
// DEFAULT-NEXT:                 label %[[VALUE_ak:[0-9]+]] ak:
// DEFAULT-NEXT:                     for %[[VALUE22:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_x_4:[0-9]+]] x: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(32))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:                             let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_x_4]], read<i32>(%[[VALUE24]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(31))
// DEFAULT-NEXT:                                 break %[[VALUE22]];
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 if eq<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(30))
// DEFAULT-NEXT:                                     break %[[VALUE22]];
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_x_4]]), const<i32>(29))
// DEFAULT-NEXT:                                         continue %[[VALUE22]];
// DEFAULT-NEXT:         label %[[VALUE_al:[0-9]+]] al:
// DEFAULT-NEXT:             label %[[VALUE_am:[0-9]+]] am:
// DEFAULT-NEXT:                 do %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(42))
// DEFAULT-NEXT:                             continue %[[VALUE25]];
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(41))
// DEFAULT-NEXT:                                 break %[[VALUE25]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 while ne<i32>(const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         label %[[VALUE_an:[0-9]+]] an:
// DEFAULT-NEXT:             label %[[VALUE_ao:[0-9]+]] ao:
// DEFAULT-NEXT:                 while %[[VALUE26:[0-9]+]] ne<i32>(read<i32>(%[[VALUE_w]]), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(40))
// DEFAULT-NEXT:                             break %[[VALUE26]];
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_w]]), const<i32>(39))
// DEFAULT-NEXT:                                 continue %[[VALUE26]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         label %[[VALUE_ap:[0-9]+]] ap:
// DEFAULT-NEXT:             label %[[VALUE_aq:[0-9]+]] aq:
// DEFAULT-NEXT:                 switch %[[VALUE27:[0-9]+]] read<i32>(%[[VALUE_w]])
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         case %[[VALUE27]] const<i32>(42):
// DEFAULT-NEXT:                             break %[[VALUE27]];
// DEFAULT-NEXT:                         default %[[VALUE27]]:
// DEFAULT-NEXT:                             break %[[VALUE27]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
