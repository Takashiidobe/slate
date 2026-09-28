// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-do assemble } */
/* { dg-require-effective-target ptr32plus } */
/* { dg-xfail-if "The array too big" { "h8300-*-*" } { "-mno-h" "-mn" } { "" } } */
/* { dg-require-stack-size "2048*4*4" } */

/* PR c/6957
   This testcase ICEd at -O2 on IA-32, because
   (insn 141 139 142 (set (subreg:SF (reg:QI 72) 0)
	   (plus:SF (reg:SF 73)
	       (reg:SF 76))) 525 {*fop_sf_comm_nosse} (insn_list 134 (nil))
       (expr_list:REG_DEAD (reg:SF 73) (nil)))
   couldn't be reloaded. */

void
foo (unsigned int n, int x, int y, unsigned char *z)
{
  int a, b;
  float c[2048][4];

  switch (x)
    {
    case 0x1906:
      a = b = -1;
      break;
    case 0x190A:
      a = b = -1;
      break;
    case 0x8049:
      a = b = -1;
      break;
    case 0x1907:
      a = 1;
      b = 2;
      break;
    default:
      return;
    }

  if (a >= 0)
    {
      unsigned char *d = z;
      unsigned int i;
      for (i = 0; i < n; i++)
	{
	  do
	    {
	      union
	      {
		float r;
		unsigned int i;
	      }
	      e;
	      e.r = c[i][1];
	      d[a] =
		((e.i >= 0x3f7f0000) ? ((int) e.i <
					    0) ? (unsigned char) 0
		 : (unsigned char) 255 : (e.r =
					  e.r * (255.0F / 256.0F) +
					  32768.0F, (unsigned char) e.i));
	    }
	  while (0);
	  d += y;
	}
    }

  if (b >= 0)
    {
      unsigned char *d = z;
      unsigned int i;
      for (i = 0; i < n; i++)
	{
	  do
	    {
	      union
	      {
		float r;
		unsigned int i;
	      }
	      e;
	      e.r = c[i][2];
	      d[b] =
		((e.i >= 0x3f7f0000) ? ((int) e.i <
					    0) ? (unsigned char) 0
		 : (unsigned char) 255 : (e.r =
					  e.r * (255.0F / 256.0F) +
					  32768.0F, (unsigned char) e.i));
	    }
	  while (0);
	  d += y;
	}
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
// DEFAULT-NEXT:     type @type0 = union {
// DEFAULT-NEXT:         field0 r: f32;
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 r: f32;
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @foo(%1 n: u32, %2 x: i32, %3 y: i32, %4 z: ptr<u8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %7 c: array<array<f32, 4>, 2048> [storage=automatic] [align=16];
// DEFAULT-NEXT:         switch %16 read<i32>(%2)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %16 const<i32>(6406):
// DEFAULT-NEXT:                     write<i32>(%6, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i32>(%5, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 case %16 const<i32>(6410):
// DEFAULT-NEXT:                     write<i32>(%6, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i32>(%5, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 case %16 const<i32>(32841):
// DEFAULT-NEXT:                     write<i32>(%6, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                     write<i32>(%5, neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 case %16 const<i32>(6407):
// DEFAULT-NEXT:                     write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(2));
// DEFAULT-NEXT:                 break %16;
// DEFAULT-NEXT:                 default %16:
// DEFAULT-NEXT:                     return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %8 d: ptr<u8> [storage=automatic] = read<ptr<u8>>(%4);
// DEFAULT-NEXT:                 let %9 i: u32 [storage=automatic];
// DEFAULT-NEXT:                 for %17
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u32>(%9, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     condition: lt<u32>(read<u32>(%9), read<u32>(%1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %21: u32 [synthetic] = read<u32>(%9);
// DEFAULT-NEXT:                         let %22: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%21), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(%9, read<u32>(%22));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %18
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %11 e: @type0 [storage=automatic];
// DEFAULT-NEXT:                                     write<f32>(field0(%11), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(deref(ptr_offset<ptr<array<f32, 4>>, subtract=false, element=array<f32, 4>, overflow=ub>(array_decay<ptr<array<f32, 4>>, length=Some(2048)>(%7), read<u32>(%9)))), const<i32>(1)))));
// DEFAULT-NEXT:                                     let %23: i32 [synthetic];
// DEFAULT-NEXT:                                     if ge<u32>(read<u32>(field1(%11)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1065287680)))
// DEFAULT-NEXT:                                         write<i32>(%23, conditional<i32>(lt<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(field1(%11))), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255)))))));
// DEFAULT-NEXT:                                     else
// DEFAULT-NEXT:                                         write<f32>(field0(%11), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(field0(%11)), div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(const<f32>(255.0), const<f32>(256.0))), const<f32>(32768.0)));
// DEFAULT-NEXT:                                         write<i32>(%23, reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(field1(%11))))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%8), read<i32>(%5))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%23))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             let %24: ptr<u8> [synthetic] = read<ptr<u8>>(%8);
// DEFAULT-NEXT:                             let %25: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%24), read<i32>(%3));
// DEFAULT-NEXT:                             write<ptr<u8>>(%8, read<ptr<u8>>(%25));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %12 d: ptr<u8> [storage=automatic] = read<ptr<u8>>(%4);
// DEFAULT-NEXT:                 let %13 i: u32 [storage=automatic];
// DEFAULT-NEXT:                 for %19
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u32>(%13, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     condition: lt<u32>(read<u32>(%13), read<u32>(%1))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %26: u32 [synthetic] = read<u32>(%13);
// DEFAULT-NEXT:                         let %27: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%26), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(%13, read<u32>(%27));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %20
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %15 e: @type1 [storage=automatic];
// DEFAULT-NEXT:                                     write<f32>(field0(%15), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(deref(ptr_offset<ptr<array<f32, 4>>, subtract=false, element=array<f32, 4>, overflow=ub>(array_decay<ptr<array<f32, 4>>, length=Some(2048)>(%7), read<u32>(%13)))), const<i32>(2)))));
// DEFAULT-NEXT:                                     let %28: i32 [synthetic];
// DEFAULT-NEXT:                                     if ge<u32>(read<u32>(field1(%15)), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1065287680)))
// DEFAULT-NEXT:                                         write<i32>(%28, conditional<i32>(lt<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(field1(%15))), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255)))))));
// DEFAULT-NEXT:                                     else
// DEFAULT-NEXT:                                         write<f32>(field0(%15), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(field0(%15)), div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(const<f32>(255.0), const<f32>(256.0))), const<f32>(32768.0)));
// DEFAULT-NEXT:                                         write<i32>(%28, reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(field1(%15))))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%12), read<i32>(%6))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%28))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             let %29: ptr<u8> [synthetic] = read<ptr<u8>>(%12);
// DEFAULT-NEXT:                             let %30: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%29), read<i32>(%3));
// DEFAULT-NEXT:                             write<ptr<u8>>(%12, read<ptr<u8>>(%30));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
