/* N3355 - Named loops.  */
/* { dg-do run } */
/* { dg-options "-std=c2y -pedantic-errors" } */

extern void abort (void);

void
foo (int x)
{
  int i, j, k, l, m;
 label1:
  for (i = 0; i < 2; ++i)
    {
      if (i == 1)
	{
	  if (x != 11)
	    abort ();
	  return;
	}
     label2:
      switch (i)
	{
	 label3:
	case 0:
	  for (j = 0; j < 2; ++j)
	    {
	      if (j == 1)
		{
		  if (x != 8)
		    abort ();
		  return;
		}
	     label4:
	      for (k = 0; k < 2; ++k)
		{
		  if (k == 1)
		    {
		      if (x != 6)
			abort ();
		      return;
		    }
		  l = 0;
		 label5:
		  while (l < 2)
		    {
		      if (l == 1)
			{
			  if (x != 4)
			    abort ();
			  return;
			}
		      ++l;
		      m = 0;
		     label6:
		      do
			{
			  if (m == 1)
			    {
			      if (x != 2)
				abort ();
			      return;
			    }
			  ++m;
			 label7:
			  switch (x)
			    {
			    case 0:
			      break label7;
			    case 1:
			      break label6;
			    case 2:
			      continue label6;
			    case 3:
			      break label5;
			    case 4:
			      continue label5;
			    case 5:
			      break label4;
			    case 6:
			      continue label4;
			    case 7:
			      break label3;
			    case 8:
			      continue label3;
			    case 9:
			      break label2;
			    case 10:
			      break label1;
			    case 11:
			      continue label1;
			    default:
			      abort ();
			      break;
			    }
			  if (x)
			    abort ();
			  return;
			}
		      while (m < 2);
		      if (x != 1 || m != 1)
			abort ();
		      return;
		    }
		  if (x != 3 || l != 1 || m != 1)
		    abort ();
		  return;
		}
	      if (x != 5 || k != 0 || l != 1 || m != 1)
		abort ();
	      return;
	    }
	  if (x != 7 || j != 0 || k != 0 || l != 1 || m != 1)
	    abort ();
	  return;
	}
      if (x != 9 || j != 0 || k != 0 || l != 1 || m != 1)
	abort ();
      return;
    }
  if (x != 10 || i != 0 || j != 0 || k != 0 || l != 1 || m != 1)
    abort ();
}

void
bar (int x)
{
  int i, j;
 label1:
  for (i = 0; i < 2; ++i)
    {
      if (i == 1)
	{
	  if (x != 1)
	    abort ();
	  return;
	}
      for (j = 0; j < 2; ++j)
	if (j == 1)
	  abort ();
	else if (x == 0)
	  break label1;
	else if (x == 1)
	  continue label1;
	else
	  abort ();
      abort ();
    }
  if (x != 0)
    abort ();
}

int
main ()
{
  for (int n = 0; n <= 11; ++n)
    foo (n);
  bar (0);
  bar (1);
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: i32 [storage=automatic];
// DEFAULT-NEXT:         label %[[VALUE_label1:[0-9]+]] label1:
// DEFAULT-NEXT:             for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(11))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 return;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         label %[[VALUE_label2:[0-9]+]] label2:
// DEFAULT-NEXT:                             switch %[[VALUE3:[0-9]+]] read<i32>(%[[VALUE_i]])
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     label %[[VALUE_label3:[0-9]+]] label3:
// DEFAULT-NEXT:                                         case %[[VALUE3]] const<i32>(0):
// DEFAULT-NEXT:                                             for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                                                 init:
// DEFAULT-NEXT:                                                     write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:                                                 condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(2))
// DEFAULT-NEXT:                                                 increment: {
// DEFAULT-NEXT:                                                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                                                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                                                     write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                                                     yield void;
// DEFAULT-NEXT:                                                 }
// DEFAULT-NEXT:                                                 body:
// DEFAULT-NEXT:                                                     {
// DEFAULT-NEXT:                                                         if eq<i32>(read<i32>(%[[VALUE_j]]), const<i32>(1))
// DEFAULT-NEXT:                                                             {
// DEFAULT-NEXT:                                                                 if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(8))
// DEFAULT-NEXT:                                                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                                 return;
// DEFAULT-NEXT:                                                             }
// DEFAULT-NEXT:                                                         label %[[VALUE_label4:[0-9]+]] label4:
// DEFAULT-NEXT:                                                             for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                                                                 init:
// DEFAULT-NEXT:                                                                     write<i32>(%[[VALUE_k]], const<i32>(0));
// DEFAULT-NEXT:                                                                 condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(2))
// DEFAULT-NEXT:                                                                 increment: {
// DEFAULT-NEXT:                                                                     let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                                                                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                                                                     write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                                                                     yield void;
// DEFAULT-NEXT:                                                                 }
// DEFAULT-NEXT:                                                                 body:
// DEFAULT-NEXT:                                                                     {
// DEFAULT-NEXT:                                                                         if eq<i32>(read<i32>(%[[VALUE_k]]), const<i32>(1))
// DEFAULT-NEXT:                                                                             {
// DEFAULT-NEXT:                                                                                 if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(6))
// DEFAULT-NEXT:                                                                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                                                 return;
// DEFAULT-NEXT:                                                                             }
// DEFAULT-NEXT:                                                                         write<i32>(%[[VALUE_l]], const<i32>(0));
// DEFAULT-NEXT:                                                                         label %[[VALUE_label5:[0-9]+]] label5:
// DEFAULT-NEXT:                                                                             while %[[VALUE10:[0-9]+]] lt<i32>(read<i32>(%[[VALUE_l]]), const<i32>(2))
// DEFAULT-NEXT:                                                                                 {
// DEFAULT-NEXT:                                                                                     if eq<i32>(read<i32>(%[[VALUE_l]]), const<i32>(1))
// DEFAULT-NEXT:                                                                                         {
// DEFAULT-NEXT:                                                                                             if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(4))
// DEFAULT-NEXT:                                                                                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                                                             return;
// DEFAULT-NEXT:                                                                                         }
// DEFAULT-NEXT:                                                                                     let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_l]]);
// DEFAULT-NEXT:                                                                                     let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_l]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                                                                                     write<i32>(%[[VALUE_m]], const<i32>(0));
// DEFAULT-NEXT:                                                                                     label %[[VALUE_label6:[0-9]+]] label6:
// DEFAULT-NEXT:                                                                                         do %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                                                                                             {
// DEFAULT-NEXT:                                                                                                 if eq<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1))
// DEFAULT-NEXT:                                                                                                     {
// DEFAULT-NEXT:                                                                                                         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(2))
// DEFAULT-NEXT:                                                                                                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                                                                         return;
// DEFAULT-NEXT:                                                                                                     }
// DEFAULT-NEXT:                                                                                                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_m]]);
// DEFAULT-NEXT:                                                                                                 let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                                                                                                 write<i32>(%[[VALUE_m]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                                                                                                 label %[[VALUE_label7:[0-9]+]] label7:
// DEFAULT-NEXT:                                                                                                     switch %[[VALUE16:[0-9]+]] read<i32>(%[[VALUE_x]])
// DEFAULT-NEXT:                                                                                                         {
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(0):
// DEFAULT-NEXT:                                                                                                                 break %[[VALUE16]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(1):
// DEFAULT-NEXT:                                                                                                                 break %[[VALUE13]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(2):
// DEFAULT-NEXT:                                                                                                                 continue %[[VALUE13]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(3):
// DEFAULT-NEXT:                                                                                                                 break %[[VALUE10]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(4):
// DEFAULT-NEXT:                                                                                                                 continue %[[VALUE10]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(5):
// DEFAULT-NEXT:                                                                                                                 break %[[VALUE7]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(6):
// DEFAULT-NEXT:                                                                                                                 continue %[[VALUE7]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(7):
// DEFAULT-NEXT:                                                                                                                 break %[[VALUE4]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(8):
// DEFAULT-NEXT:                                                                                                                 continue %[[VALUE4]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(9):
// DEFAULT-NEXT:                                                                                                                 break %[[VALUE3]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(10):
// DEFAULT-NEXT:                                                                                                                 break %[[VALUE0]];
// DEFAULT-NEXT:                                                                                                             case %[[VALUE16]] const<i32>(11):
// DEFAULT-NEXT:                                                                                                                 continue %[[VALUE0]];
// DEFAULT-NEXT:                                                                                                             default %[[VALUE16]]:
// DEFAULT-NEXT:                                                                                                                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                                                                             break %[[VALUE16]];
// DEFAULT-NEXT:                                                                                                         }
// DEFAULT-NEXT:                                                                                                 if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(0))
// DEFAULT-NEXT:                                                                                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                                                                 return;
// DEFAULT-NEXT:                                                                                             }
// DEFAULT-NEXT:                                                                                         while lt<i32>(read<i32>(%[[VALUE_m]]), const<i32>(2));
// DEFAULT-NEXT:                                                                                     if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(1)), ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1)))
// DEFAULT-NEXT:                                                                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                                                     return;
// DEFAULT-NEXT:                                                                                 }
// DEFAULT-NEXT:                                                                         if logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(3)), ne<i32>(read<i32>(%[[VALUE_l]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1)))
// DEFAULT-NEXT:                                                                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                                         return;
// DEFAULT-NEXT:                                                                     }
// DEFAULT-NEXT:                                                         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(5)), ne<i32>(read<i32>(%[[VALUE_k]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_l]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1)))
// DEFAULT-NEXT:                                                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                                         return;
// DEFAULT-NEXT:                                                     }
// DEFAULT-NEXT:                                     if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(7)), ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_k]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_l]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1)))
// DEFAULT-NEXT:                                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                     return;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(9)), ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_k]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_l]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1)))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         return;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(10)), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_j]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_k]]), const<i32>(0))), ne<i32>(read<i32>(%[[VALUE_l]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_m]]), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         label %[[VALUE_label1_2:[0-9]+]] label1:
// DEFAULT-NEXT:             for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(2))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                     let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         if eq<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(1))
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(1))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 return;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:                             init:
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_j_2]], const<i32>(0));
// DEFAULT-NEXT:                             condition: lt<i32>(read<i32>(%[[VALUE_j_2]]), const<i32>(2))
// DEFAULT-NEXT:                             increment: {
// DEFAULT-NEXT:                                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// DEFAULT-NEXT:                                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                                 yield void;
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                             body:
// DEFAULT-NEXT:                                 if eq<i32>(read<i32>(%[[VALUE_j_2]]), const<i32>(1))
// DEFAULT-NEXT:                                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                                 else
// DEFAULT-NEXT:                                     if eq<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))
// DEFAULT-NEXT:                                         break %[[VALUE17]];
// DEFAULT-NEXT:                                     else
// DEFAULT-NEXT:                                         if eq<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(1))
// DEFAULT-NEXT:                                             continue %[[VALUE17]];
// DEFAULT-NEXT:                                         else
// DEFAULT-NEXT:                                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %[[VALUE23:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_n]]), const<i32>(11))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 call<void, signature=fn(i32) -> void>(%[[VALUE_foo]], read<i32>(%[[VALUE_n]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_bar]], const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
