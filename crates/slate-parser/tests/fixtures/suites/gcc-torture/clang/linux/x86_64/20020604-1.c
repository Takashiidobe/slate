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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 r: f32;
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 r: f32;
// DEFAULT-NEXT:         field1 i: u32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_n:[0-9]+]] n: u32, %[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32, %[[VALUE_z:[0-9]+]] z: ptr<u8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: array<array<f32, 4>, 2048> [storage=automatic] [align=16];
// DEFAULT-NEXT:         switch %[[VALUE0:[0-9]+]] read<i32>(%[[VALUE_x]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(6406):
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(6410):
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(32841):
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 case %[[VALUE0]] const<i32>(6407):
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_a]], const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_b]], const<i32>(2));
// DEFAULT-NEXT:                 break %[[VALUE0]];
// DEFAULT-NEXT:                 default %[[VALUE0]]:
// DEFAULT-NEXT:                     return;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_a]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_d:[0-9]+]] d: ptr<u8> [storage=automatic] = read<ptr<u8>>(%[[VALUE_z]]);
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_i]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     condition: lt<u32>(read<u32>(%[[VALUE_i]]), read<u32>(%[[VALUE_n]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE5]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE6]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_e:[0-9]+]] e: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:                                     write<f32>(field0(%[[VALUE_e]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(deref(ptr_offset<ptr<array<f32, 4>>, subtract=false, element=array<f32, 4>, overflow=ub>(array_decay<ptr<array<f32, 4>>, length=Some(2048)>(%[[VALUE_c]]), read<u32>(%[[VALUE_i]])))), const<i32>(1)))));
// DEFAULT-NEXT:                                     let %[[VALUE8:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:                                     if ge<u32>(read<u32>(field1(%[[VALUE_e]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1065287680)))
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE8]], conditional<i32>(lt<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(field1(%[[VALUE_e]]))), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255)))))));
// DEFAULT-NEXT:                                     else
// DEFAULT-NEXT:                                         write<f32>(field0(%[[VALUE_e]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(field0(%[[VALUE_e]])), div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(const<f32>(255.0), const<f32>(256.0))), const<f32>(32768.0)));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE8]], reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(field1(%[[VALUE_e]]))))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_d]]), read<i32>(%[[VALUE_a]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE8]]))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             let %[[VALUE9:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_d]]);
// DEFAULT-NEXT:                             let %[[VALUE10:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE9]]), read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:                             write<ptr<u8>>(%[[VALUE_d]], read<ptr<u8>>(%[[VALUE10]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ge<i32>(read<i32>(%[[VALUE_b]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_d_2:[0-9]+]] d: ptr<u8> [storage=automatic] = read<ptr<u8>>(%[[VALUE_z]]);
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:                 for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), read<u32>(%[[VALUE_n]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE12:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                         let %[[VALUE13:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE12]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE13]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     let %[[VALUE_e_2:[0-9]+]] e: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:                                     write<f32>(field0(%[[VALUE_e_2]]), read<f32>(deref(ptr_offset<ptr<f32>, subtract=false, element=f32, overflow=ub>(array_decay<ptr<f32>, length=Some(4)>(deref(ptr_offset<ptr<array<f32, 4>>, subtract=false, element=array<f32, 4>, overflow=ub>(array_decay<ptr<array<f32, 4>>, length=Some(2048)>(%[[VALUE_c]]), read<u32>(%[[VALUE_i_2]])))), const<i32>(2)))));
// DEFAULT-NEXT:                                     let %[[VALUE15:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:                                     if ge<u32>(read<u32>(field1(%[[VALUE_e_2]])), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1065287680)))
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE15]], conditional<i32>(lt<i32>(reinterpret<i32, reason=explicit, fits=unknown>(read<u32>(field1(%[[VALUE_e_2]]))), const<i32>(0)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=always>(const<i32>(0))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(const<i32>(255)))))));
// DEFAULT-NEXT:                                     else
// DEFAULT-NEXT:                                         write<f32>(field0(%[[VALUE_e_2]]), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(field0(%[[VALUE_e_2]])), div<f32, rounding=nearest_even, exceptions=ignore, contract=on>(const<f32>(255.0), const<f32>(256.0))), const<f32>(32768.0)));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE15]], reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(field1(%[[VALUE_e_2]]))))));
// DEFAULT-NEXT:                                     write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE_d_2]]), read<i32>(%[[VALUE_b]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(read<i32>(%[[VALUE15]]))));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:                             let %[[VALUE16:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_d_2]]);
// DEFAULT-NEXT:                             let %[[VALUE17:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE16]]), read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:                             write<ptr<u8>>(%[[VALUE_d_2]], read<ptr<u8>>(%[[VALUE17]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
