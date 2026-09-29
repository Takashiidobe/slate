/* Test we do correct thing for adding to / subtracting from a pointer,
   i.e. that the multiplication by the size of the pointer target type
   still occurs.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */
/* { dg-xfail-run-if "PR97444: stack atomics" { nvptx*-*-* } }*/

#define TEST_POINTER_ADD_SUB(TYPE)			\
  do							\
    {							\
      TYPE a[3][3];					\
      TYPE (*_Atomic q)[3] = &a[0];			\
      ++q;						\
      if (q != &a[1])					\
	__builtin_abort ();				\
      q++;						\
      if (q != &a[2])					\
	__builtin_abort ();				\
      --q;						\
      if (q != &a[1])					\
	__builtin_abort ();				\
      q--;						\
      if (q != &a[0])					\
	__builtin_abort ();				\
      q += 2;						\
      if (q != &a[2])					\
	__builtin_abort ();				\
      q -= 2;						\
      if (q != &a[0])					\
	__builtin_abort ();				\
    }							\
  while (0)

int
main (void)
{
  TEST_POINTER_ADD_SUB (_Bool);
  TEST_POINTER_ADD_SUB (char);
  TEST_POINTER_ADD_SUB (signed char);
  TEST_POINTER_ADD_SUB (unsigned char);
  TEST_POINTER_ADD_SUB (signed short);
  TEST_POINTER_ADD_SUB (unsigned short);
  TEST_POINTER_ADD_SUB (signed int);
  TEST_POINTER_ADD_SUB (unsigned int);
  TEST_POINTER_ADD_SUB (signed long);
  TEST_POINTER_ADD_SUB (unsigned long);
  TEST_POINTER_ADD_SUB (signed long long);
  TEST_POINTER_ADD_SUB (unsigned long long);
  TEST_POINTER_ADD_SUB (float);
  TEST_POINTER_ADD_SUB (double);
  TEST_POINTER_ADD_SUB (long double);
  TEST_POINTER_ADD_SUB (_Complex float);
  TEST_POINTER_ADD_SUB (_Complex double);
  TEST_POINTER_ADD_SUB (_Complex long double);
}

// SLATE-FILECHECK-STD DEFAULT c11
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
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a:[0-9]+]] a: array<array<bool, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_q:[0-9]+]] q: atomic ptr<array<bool, 3>> [storage=automatic] = addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%[[VALUE_a]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q]], ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%[[VALUE_q]]), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%[[VALUE_a]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q]], ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%[[VALUE_q]]), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%[[VALUE_a]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q]], ptr_offset<ptr<array<bool, 3>>, subtract=true, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%[[VALUE_q]]), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%[[VALUE_a]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q]], ptr_offset<ptr<array<bool, 3>>, subtract=true, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%[[VALUE_q]]), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%[[VALUE_a]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q]], ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%[[VALUE_q]]), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%[[VALUE_a]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q]], ptr_offset<ptr<array<bool, 3>>, subtract=true, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%[[VALUE_q]]), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%[[VALUE_a]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_2:[0-9]+]] a: array<array<i8, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_q_2:[0-9]+]] q: atomic ptr<array<i8, 3>> [storage=automatic] = addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_2]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_2]], ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_2]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_2]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_2]], ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_2]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_2]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_2]], ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_2]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_2]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_2]], ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_2]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_2]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_2]], ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_2]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_2]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_2]], ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_2]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_2]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_3:[0-9]+]] a: array<array<i8, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_q_3:[0-9]+]] q: atomic ptr<array<i8, 3>> [storage=automatic] = addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_3]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_3]], ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_3]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_3]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_3]], ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_3]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_3]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_3]], ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_3]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_3]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_3]], ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_3]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_3]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_3]], ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_3]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_3]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_3]], ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%[[VALUE_q_3]]), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%[[VALUE_a_3]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE21:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_4:[0-9]+]] a: array<array<u8, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_q_4:[0-9]+]] q: atomic ptr<array<u8, 3>> [storage=automatic] = addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%[[VALUE_a_4]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_4]], ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%[[VALUE_q_4]]), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%[[VALUE_a_4]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_4]], ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%[[VALUE_q_4]]), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%[[VALUE_a_4]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_4]], ptr_offset<ptr<array<u8, 3>>, subtract=true, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%[[VALUE_q_4]]), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%[[VALUE_a_4]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE25:[0-9]+]]: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_4]], ptr_offset<ptr<array<u8, 3>>, subtract=true, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%[[VALUE_q_4]]), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%[[VALUE_a_4]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_4]], ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%[[VALUE_q_4]]), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%[[VALUE_a_4]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_4]], ptr_offset<ptr<array<u8, 3>>, subtract=true, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%[[VALUE_q_4]]), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%[[VALUE_a_4]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE28:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_5:[0-9]+]] a: array<array<i16, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_5:[0-9]+]] q: atomic ptr<array<i16, 3>> [storage=automatic] = addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%[[VALUE_a_5]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_5]], ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%[[VALUE_q_5]]), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%[[VALUE_a_5]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE30:[0-9]+]]: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_5]], ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%[[VALUE_q_5]]), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%[[VALUE_a_5]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_5]], ptr_offset<ptr<array<i16, 3>>, subtract=true, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%[[VALUE_q_5]]), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%[[VALUE_a_5]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_5]], ptr_offset<ptr<array<i16, 3>>, subtract=true, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%[[VALUE_q_5]]), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%[[VALUE_a_5]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_5]], ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%[[VALUE_q_5]]), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%[[VALUE_a_5]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_5]], ptr_offset<ptr<array<i16, 3>>, subtract=true, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%[[VALUE_q_5]]), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%[[VALUE_a_5]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_6:[0-9]+]] a: array<array<u16, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_6:[0-9]+]] q: atomic ptr<array<u16, 3>> [storage=automatic] = addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%[[VALUE_a_6]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_6]], ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%[[VALUE_q_6]]), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%[[VALUE_a_6]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_6]], ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%[[VALUE_q_6]]), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%[[VALUE_a_6]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE38:[0-9]+]]: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_6]], ptr_offset<ptr<array<u16, 3>>, subtract=true, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%[[VALUE_q_6]]), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%[[VALUE_a_6]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_6]], ptr_offset<ptr<array<u16, 3>>, subtract=true, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%[[VALUE_q_6]]), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%[[VALUE_a_6]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE40:[0-9]+]]: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_6]], ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%[[VALUE_q_6]]), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%[[VALUE_a_6]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE41:[0-9]+]]: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_6]], ptr_offset<ptr<array<u16, 3>>, subtract=true, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%[[VALUE_q_6]]), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%[[VALUE_a_6]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_7:[0-9]+]] a: array<array<i32, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_7:[0-9]+]] q: atomic ptr<array<i32, 3>> [storage=automatic] = addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_a_7]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE43:[0-9]+]]: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_7]], ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%[[VALUE_q_7]]), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_a_7]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_7]], ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%[[VALUE_q_7]]), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_a_7]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE45:[0-9]+]]: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_7]], ptr_offset<ptr<array<i32, 3>>, subtract=true, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%[[VALUE_q_7]]), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_a_7]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE46:[0-9]+]]: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_7]], ptr_offset<ptr<array<i32, 3>>, subtract=true, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%[[VALUE_q_7]]), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_a_7]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE47:[0-9]+]]: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_7]], ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%[[VALUE_q_7]]), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_a_7]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE48:[0-9]+]]: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_7]], ptr_offset<ptr<array<i32, 3>>, subtract=true, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%[[VALUE_q_7]]), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%[[VALUE_a_7]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE49:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_8:[0-9]+]] a: array<array<u32, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_8:[0-9]+]] q: atomic ptr<array<u32, 3>> [storage=automatic] = addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%[[VALUE_a_8]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE50:[0-9]+]]: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_8]], ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%[[VALUE_q_8]]), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%[[VALUE_a_8]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE51:[0-9]+]]: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_8]], ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%[[VALUE_q_8]]), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%[[VALUE_a_8]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE52:[0-9]+]]: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_8]], ptr_offset<ptr<array<u32, 3>>, subtract=true, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%[[VALUE_q_8]]), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%[[VALUE_a_8]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_8]], ptr_offset<ptr<array<u32, 3>>, subtract=true, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%[[VALUE_q_8]]), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%[[VALUE_a_8]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_8]], ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%[[VALUE_q_8]]), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%[[VALUE_a_8]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE55:[0-9]+]]: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_8]], ptr_offset<ptr<array<u32, 3>>, subtract=true, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%[[VALUE_q_8]]), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%[[VALUE_a_8]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE56:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_9:[0-9]+]] a: array<array<i64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_9:[0-9]+]] q: atomic ptr<array<i64, 3>> [storage=automatic] = addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_9]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE57:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_9]], ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_9]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_9]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE58:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_9]], ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_9]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_9]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE59:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_9]], ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_9]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_9]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE60:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_9]], ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_9]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_9]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE61:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_9]], ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_9]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_9]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE62:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_9]], ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_9]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_9]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE63:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_10:[0-9]+]] a: array<array<u64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_10:[0-9]+]] q: atomic ptr<array<u64, 3>> [storage=automatic] = addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_10]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE64:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_10]], ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_10]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_10]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE65:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_10]], ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_10]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_10]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE66:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_10]], ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_10]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_10]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE67:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_10]], ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_10]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_10]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE68:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_10]], ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_10]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_10]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE69:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_10]], ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_10]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_10]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE70:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_11:[0-9]+]] a: array<array<i64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_11:[0-9]+]] q: atomic ptr<array<i64, 3>> [storage=automatic] = addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_11]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE71:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_11]], ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_11]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_11]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE72:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_11]], ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_11]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_11]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE73:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_11]], ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_11]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_11]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE74:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_11]], ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_11]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_11]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE75:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_11]], ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_11]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_11]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE76:[0-9]+]]: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_11]], ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%[[VALUE_q_11]]), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%[[VALUE_a_11]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE77:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_12:[0-9]+]] a: array<array<u64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_12:[0-9]+]] q: atomic ptr<array<u64, 3>> [storage=automatic] = addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_12]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE78:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_12]], ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_12]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_12]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE79:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_12]], ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_12]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_12]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE80:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_12]], ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_12]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_12]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE81:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_12]], ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_12]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_12]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE82:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_12]], ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_12]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_12]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE83:[0-9]+]]: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_12]], ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%[[VALUE_q_12]]), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%[[VALUE_a_12]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE84:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_13:[0-9]+]] a: array<array<f32, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_13:[0-9]+]] q: atomic ptr<array<f32, 3>> [storage=automatic] = addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%[[VALUE_a_13]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE85:[0-9]+]]: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_13]], ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%[[VALUE_q_13]]), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%[[VALUE_a_13]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE86:[0-9]+]]: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_13]], ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%[[VALUE_q_13]]), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%[[VALUE_a_13]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE87:[0-9]+]]: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_13]], ptr_offset<ptr<array<f32, 3>>, subtract=true, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%[[VALUE_q_13]]), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%[[VALUE_a_13]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE88:[0-9]+]]: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_13]], ptr_offset<ptr<array<f32, 3>>, subtract=true, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%[[VALUE_q_13]]), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%[[VALUE_a_13]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE89:[0-9]+]]: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_13]], ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%[[VALUE_q_13]]), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%[[VALUE_a_13]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE90:[0-9]+]]: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_13]], ptr_offset<ptr<array<f32, 3>>, subtract=true, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%[[VALUE_q_13]]), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%[[VALUE_a_13]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE91:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_14:[0-9]+]] a: array<array<f64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_14:[0-9]+]] q: atomic ptr<array<f64, 3>> [storage=automatic] = addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%[[VALUE_a_14]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE92:[0-9]+]]: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_14]], ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%[[VALUE_q_14]]), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%[[VALUE_a_14]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE93:[0-9]+]]: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_14]], ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%[[VALUE_q_14]]), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%[[VALUE_a_14]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE94:[0-9]+]]: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_14]], ptr_offset<ptr<array<f64, 3>>, subtract=true, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%[[VALUE_q_14]]), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%[[VALUE_a_14]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE95:[0-9]+]]: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_14]], ptr_offset<ptr<array<f64, 3>>, subtract=true, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%[[VALUE_q_14]]), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%[[VALUE_a_14]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE96:[0-9]+]]: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_14]], ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%[[VALUE_q_14]]), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%[[VALUE_a_14]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE97:[0-9]+]]: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_14]], ptr_offset<ptr<array<f64, 3>>, subtract=true, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%[[VALUE_q_14]]), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%[[VALUE_a_14]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE98:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_15:[0-9]+]] a: array<array<f80, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_q_15:[0-9]+]] q: atomic ptr<array<f80, 3>> [storage=automatic] = addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%[[VALUE_a_15]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE99:[0-9]+]]: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_15]], ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%[[VALUE_q_15]]), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%[[VALUE_a_15]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE100:[0-9]+]]: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_15]], ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%[[VALUE_q_15]]), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%[[VALUE_a_15]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE101:[0-9]+]]: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_15]], ptr_offset<ptr<array<f80, 3>>, subtract=true, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%[[VALUE_q_15]]), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%[[VALUE_a_15]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE102:[0-9]+]]: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_15]], ptr_offset<ptr<array<f80, 3>>, subtract=true, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%[[VALUE_q_15]]), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%[[VALUE_a_15]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE103:[0-9]+]]: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_15]], ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%[[VALUE_q_15]]), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%[[VALUE_a_15]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE104:[0-9]+]]: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_15]], ptr_offset<ptr<array<f80, 3>>, subtract=true, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%[[VALUE_q_15]]), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%[[VALUE_a_15]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE105:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_16:[0-9]+]] a: array<array<complex<f32>, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_16:[0-9]+]] q: atomic ptr<array<complex<f32>, 3>> [storage=automatic] = addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%[[VALUE_a_16]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE106:[0-9]+]]: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_16]], ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%[[VALUE_q_16]]), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%[[VALUE_a_16]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE107:[0-9]+]]: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_16]], ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%[[VALUE_q_16]]), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%[[VALUE_a_16]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE108:[0-9]+]]: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_16]], ptr_offset<ptr<array<complex<f32>, 3>>, subtract=true, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%[[VALUE_q_16]]), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%[[VALUE_a_16]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE109:[0-9]+]]: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_16]], ptr_offset<ptr<array<complex<f32>, 3>>, subtract=true, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%[[VALUE_q_16]]), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%[[VALUE_a_16]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE110:[0-9]+]]: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_16]], ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%[[VALUE_q_16]]), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%[[VALUE_a_16]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE111:[0-9]+]]: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_16]], ptr_offset<ptr<array<complex<f32>, 3>>, subtract=true, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%[[VALUE_q_16]]), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%[[VALUE_a_16]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE112:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_17:[0-9]+]] a: array<array<complex<f64>, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %[[VALUE_q_17:[0-9]+]] q: atomic ptr<array<complex<f64>, 3>> [storage=automatic] = addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%[[VALUE_a_17]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE113:[0-9]+]]: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_17]], ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%[[VALUE_q_17]]), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%[[VALUE_a_17]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE114:[0-9]+]]: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_17]], ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%[[VALUE_q_17]]), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%[[VALUE_a_17]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE115:[0-9]+]]: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_17]], ptr_offset<ptr<array<complex<f64>, 3>>, subtract=true, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%[[VALUE_q_17]]), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%[[VALUE_a_17]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE116:[0-9]+]]: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_17]], ptr_offset<ptr<array<complex<f64>, 3>>, subtract=true, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%[[VALUE_q_17]]), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%[[VALUE_a_17]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE117:[0-9]+]]: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_17]], ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%[[VALUE_q_17]]), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%[[VALUE_a_17]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE118:[0-9]+]]: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_17]], ptr_offset<ptr<array<complex<f64>, 3>>, subtract=true, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%[[VALUE_q_17]]), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%[[VALUE_a_17]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE119:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_a_18:[0-9]+]] a: array<array<complex<f80>, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_q_18:[0-9]+]] q: atomic ptr<array<complex<f80>, 3>> [storage=automatic] = addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%[[VALUE_a_18]]), const<i32>(0))));
// DEFAULT-NEXT:                 let %[[VALUE120:[0-9]+]]: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_18]], ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%[[VALUE_q_18]]), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%[[VALUE_a_18]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE121:[0-9]+]]: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_18]], ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%[[VALUE_q_18]]), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%[[VALUE_a_18]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE122:[0-9]+]]: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_18]], ptr_offset<ptr<array<complex<f80>, 3>>, subtract=true, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%[[VALUE_q_18]]), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%[[VALUE_a_18]]), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE123:[0-9]+]]: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=old, atomic=seq_cst>(%[[VALUE_q_18]], ptr_offset<ptr<array<complex<f80>, 3>>, subtract=true, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%[[VALUE_q_18]]), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%[[VALUE_a_18]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE124:[0-9]+]]: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_18]], ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%[[VALUE_q_18]]), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%[[VALUE_a_18]]), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE125:[0-9]+]]: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=new, atomic=seq_cst>(%[[VALUE_q_18]], ptr_offset<ptr<array<complex<f80>, 3>>, subtract=true, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%[[VALUE_q_18]]), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%[[VALUE_a_18]]), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
