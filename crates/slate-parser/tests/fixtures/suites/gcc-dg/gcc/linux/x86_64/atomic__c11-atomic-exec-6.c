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
// DEFAULT-NEXT:     fn %38 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         do %37
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %1 a: array<array<bool, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %2 q: atomic ptr<array<bool, 3>> [storage=automatic] = addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%1), const<i32>(0))));
// DEFAULT-NEXT:                 let %56: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=new, atomic=seq_cst>(%2, ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%2), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%1), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %57: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=old, atomic=seq_cst>(%2, ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%2), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%1), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %58: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=new, atomic=seq_cst>(%2, ptr_offset<ptr<array<bool, 3>>, subtract=true, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%2), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%1), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %59: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=old, atomic=seq_cst>(%2, ptr_offset<ptr<array<bool, 3>>, subtract=true, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%2), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%1), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %60: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=new, atomic=seq_cst>(%2, ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%2), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%1), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %61: ptr<array<bool, 3>> [synthetic] = update<ptr<array<bool, 3>>, result=new, atomic=seq_cst>(%2, ptr_offset<ptr<array<bool, 3>>, subtract=true, element=array<bool, 3>, overflow=ub>(old<ptr<array<bool, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<bool, 3>>>(read<ptr<array<bool, 3>>, atomic=seq_cst>(%2), addr_of<ptr<array<bool, 3>>>(deref(ptr_offset<ptr<array<bool, 3>>, subtract=false, element=array<bool, 3>, overflow=ub>(array_decay<ptr<array<bool, 3>>, length=Some(3)>(%1), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %39
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %3 a: array<array<i8, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %4 q: atomic ptr<array<i8, 3>> [storage=automatic] = addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%3), const<i32>(0))));
// DEFAULT-NEXT:                 let %62: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%4, ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%4), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%3), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %63: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=old, atomic=seq_cst>(%4, ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%4), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%3), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %64: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%4, ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%4), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%3), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %65: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=old, atomic=seq_cst>(%4, ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%4), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%3), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %66: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%4, ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%4), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%3), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %67: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%4, ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%4), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%3), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %40
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %5 a: array<array<i8, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %6 q: atomic ptr<array<i8, 3>> [storage=automatic] = addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%5), const<i32>(0))));
// DEFAULT-NEXT:                 let %68: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%6, ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%6), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%5), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %69: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=old, atomic=seq_cst>(%6, ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%6), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%5), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %70: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%6, ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%6), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%5), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %71: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=old, atomic=seq_cst>(%6, ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%6), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%5), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %72: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%6, ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%6), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%5), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %73: ptr<array<i8, 3>> [synthetic] = update<ptr<array<i8, 3>>, result=new, atomic=seq_cst>(%6, ptr_offset<ptr<array<i8, 3>>, subtract=true, element=array<i8, 3>, overflow=ub>(old<ptr<array<i8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i8, 3>>>(read<ptr<array<i8, 3>>, atomic=seq_cst>(%6), addr_of<ptr<array<i8, 3>>>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(3)>(%5), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %41
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %7 a: array<array<u8, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %8 q: atomic ptr<array<u8, 3>> [storage=automatic] = addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%7), const<i32>(0))));
// DEFAULT-NEXT:                 let %74: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=new, atomic=seq_cst>(%8, ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%8), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%7), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %75: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=old, atomic=seq_cst>(%8, ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%8), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%7), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %76: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=new, atomic=seq_cst>(%8, ptr_offset<ptr<array<u8, 3>>, subtract=true, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%8), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%7), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %77: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=old, atomic=seq_cst>(%8, ptr_offset<ptr<array<u8, 3>>, subtract=true, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%8), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%7), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %78: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=new, atomic=seq_cst>(%8, ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%8), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%7), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %79: ptr<array<u8, 3>> [synthetic] = update<ptr<array<u8, 3>>, result=new, atomic=seq_cst>(%8, ptr_offset<ptr<array<u8, 3>>, subtract=true, element=array<u8, 3>, overflow=ub>(old<ptr<array<u8, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u8, 3>>>(read<ptr<array<u8, 3>>, atomic=seq_cst>(%8), addr_of<ptr<array<u8, 3>>>(deref(ptr_offset<ptr<array<u8, 3>>, subtract=false, element=array<u8, 3>, overflow=ub>(array_decay<ptr<array<u8, 3>>, length=Some(3)>(%7), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %42
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %9 a: array<array<i16, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %10 q: atomic ptr<array<i16, 3>> [storage=automatic] = addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%9), const<i32>(0))));
// DEFAULT-NEXT:                 let %80: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=new, atomic=seq_cst>(%10, ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%10), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%9), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %81: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=old, atomic=seq_cst>(%10, ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%10), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%9), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %82: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=new, atomic=seq_cst>(%10, ptr_offset<ptr<array<i16, 3>>, subtract=true, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%10), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%9), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %83: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=old, atomic=seq_cst>(%10, ptr_offset<ptr<array<i16, 3>>, subtract=true, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%10), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%9), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %84: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=new, atomic=seq_cst>(%10, ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%10), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%9), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %85: ptr<array<i16, 3>> [synthetic] = update<ptr<array<i16, 3>>, result=new, atomic=seq_cst>(%10, ptr_offset<ptr<array<i16, 3>>, subtract=true, element=array<i16, 3>, overflow=ub>(old<ptr<array<i16, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i16, 3>>>(read<ptr<array<i16, 3>>, atomic=seq_cst>(%10), addr_of<ptr<array<i16, 3>>>(deref(ptr_offset<ptr<array<i16, 3>>, subtract=false, element=array<i16, 3>, overflow=ub>(array_decay<ptr<array<i16, 3>>, length=Some(3)>(%9), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %43
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %11 a: array<array<u16, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %12 q: atomic ptr<array<u16, 3>> [storage=automatic] = addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%11), const<i32>(0))));
// DEFAULT-NEXT:                 let %86: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=new, atomic=seq_cst>(%12, ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%12), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%11), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %87: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=old, atomic=seq_cst>(%12, ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%12), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%11), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %88: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=new, atomic=seq_cst>(%12, ptr_offset<ptr<array<u16, 3>>, subtract=true, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%12), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%11), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %89: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=old, atomic=seq_cst>(%12, ptr_offset<ptr<array<u16, 3>>, subtract=true, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%12), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%11), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %90: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=new, atomic=seq_cst>(%12, ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%12), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%11), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %91: ptr<array<u16, 3>> [synthetic] = update<ptr<array<u16, 3>>, result=new, atomic=seq_cst>(%12, ptr_offset<ptr<array<u16, 3>>, subtract=true, element=array<u16, 3>, overflow=ub>(old<ptr<array<u16, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u16, 3>>>(read<ptr<array<u16, 3>>, atomic=seq_cst>(%12), addr_of<ptr<array<u16, 3>>>(deref(ptr_offset<ptr<array<u16, 3>>, subtract=false, element=array<u16, 3>, overflow=ub>(array_decay<ptr<array<u16, 3>>, length=Some(3)>(%11), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %44
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %13 a: array<array<i32, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %14 q: atomic ptr<array<i32, 3>> [storage=automatic] = addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%13), const<i32>(0))));
// DEFAULT-NEXT:                 let %92: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=new, atomic=seq_cst>(%14, ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%14), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%13), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %93: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=old, atomic=seq_cst>(%14, ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%14), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%13), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %94: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=new, atomic=seq_cst>(%14, ptr_offset<ptr<array<i32, 3>>, subtract=true, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%14), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%13), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %95: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=old, atomic=seq_cst>(%14, ptr_offset<ptr<array<i32, 3>>, subtract=true, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%14), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%13), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %96: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=new, atomic=seq_cst>(%14, ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%14), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%13), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %97: ptr<array<i32, 3>> [synthetic] = update<ptr<array<i32, 3>>, result=new, atomic=seq_cst>(%14, ptr_offset<ptr<array<i32, 3>>, subtract=true, element=array<i32, 3>, overflow=ub>(old<ptr<array<i32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i32, 3>>>(read<ptr<array<i32, 3>>, atomic=seq_cst>(%14), addr_of<ptr<array<i32, 3>>>(deref(ptr_offset<ptr<array<i32, 3>>, subtract=false, element=array<i32, 3>, overflow=ub>(array_decay<ptr<array<i32, 3>>, length=Some(3)>(%13), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %45
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 a: array<array<u32, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %16 q: atomic ptr<array<u32, 3>> [storage=automatic] = addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%15), const<i32>(0))));
// DEFAULT-NEXT:                 let %98: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=new, atomic=seq_cst>(%16, ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%16), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%15), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %99: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=old, atomic=seq_cst>(%16, ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%16), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%15), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %100: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=new, atomic=seq_cst>(%16, ptr_offset<ptr<array<u32, 3>>, subtract=true, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%16), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%15), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %101: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=old, atomic=seq_cst>(%16, ptr_offset<ptr<array<u32, 3>>, subtract=true, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%16), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%15), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %102: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=new, atomic=seq_cst>(%16, ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%16), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%15), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %103: ptr<array<u32, 3>> [synthetic] = update<ptr<array<u32, 3>>, result=new, atomic=seq_cst>(%16, ptr_offset<ptr<array<u32, 3>>, subtract=true, element=array<u32, 3>, overflow=ub>(old<ptr<array<u32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u32, 3>>>(read<ptr<array<u32, 3>>, atomic=seq_cst>(%16), addr_of<ptr<array<u32, 3>>>(deref(ptr_offset<ptr<array<u32, 3>>, subtract=false, element=array<u32, 3>, overflow=ub>(array_decay<ptr<array<u32, 3>>, length=Some(3)>(%15), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %46
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 a: array<array<i64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %18 q: atomic ptr<array<i64, 3>> [storage=automatic] = addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%17), const<i32>(0))));
// DEFAULT-NEXT:                 let %104: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%18, ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%18), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%17), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %105: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=old, atomic=seq_cst>(%18, ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%18), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%17), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %106: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%18, ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%18), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%17), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %107: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=old, atomic=seq_cst>(%18, ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%18), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%17), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %108: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%18, ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%18), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%17), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %109: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%18, ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%18), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%17), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %47
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %19 a: array<array<u64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %20 q: atomic ptr<array<u64, 3>> [storage=automatic] = addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%19), const<i32>(0))));
// DEFAULT-NEXT:                 let %110: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%20, ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%20), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%19), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %111: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=old, atomic=seq_cst>(%20, ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%20), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%19), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %112: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%20, ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%20), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%19), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %113: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=old, atomic=seq_cst>(%20, ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%20), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%19), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %114: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%20, ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%20), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%19), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %115: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%20, ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%20), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%19), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %48
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21 a: array<array<i64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %22 q: atomic ptr<array<i64, 3>> [storage=automatic] = addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%21), const<i32>(0))));
// DEFAULT-NEXT:                 let %116: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%22, ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%22), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%21), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %117: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=old, atomic=seq_cst>(%22, ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%22), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%21), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %118: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%22, ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%22), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%21), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %119: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=old, atomic=seq_cst>(%22, ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%22), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%21), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %120: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%22, ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%22), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%21), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %121: ptr<array<i64, 3>> [synthetic] = update<ptr<array<i64, 3>>, result=new, atomic=seq_cst>(%22, ptr_offset<ptr<array<i64, 3>>, subtract=true, element=array<i64, 3>, overflow=ub>(old<ptr<array<i64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<i64, 3>>>(read<ptr<array<i64, 3>>, atomic=seq_cst>(%22), addr_of<ptr<array<i64, 3>>>(deref(ptr_offset<ptr<array<i64, 3>>, subtract=false, element=array<i64, 3>, overflow=ub>(array_decay<ptr<array<i64, 3>>, length=Some(3)>(%21), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %49
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %23 a: array<array<u64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %24 q: atomic ptr<array<u64, 3>> [storage=automatic] = addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%23), const<i32>(0))));
// DEFAULT-NEXT:                 let %122: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%24, ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%24), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%23), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %123: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=old, atomic=seq_cst>(%24, ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%24), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%23), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %124: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%24, ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%24), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%23), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %125: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=old, atomic=seq_cst>(%24, ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%24), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%23), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %126: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%24, ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%24), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%23), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %127: ptr<array<u64, 3>> [synthetic] = update<ptr<array<u64, 3>>, result=new, atomic=seq_cst>(%24, ptr_offset<ptr<array<u64, 3>>, subtract=true, element=array<u64, 3>, overflow=ub>(old<ptr<array<u64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<u64, 3>>>(read<ptr<array<u64, 3>>, atomic=seq_cst>(%24), addr_of<ptr<array<u64, 3>>>(deref(ptr_offset<ptr<array<u64, 3>>, subtract=false, element=array<u64, 3>, overflow=ub>(array_decay<ptr<array<u64, 3>>, length=Some(3)>(%23), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %50
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %25 a: array<array<f32, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %26 q: atomic ptr<array<f32, 3>> [storage=automatic] = addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%25), const<i32>(0))));
// DEFAULT-NEXT:                 let %128: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=new, atomic=seq_cst>(%26, ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%26), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%25), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %129: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=old, atomic=seq_cst>(%26, ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%26), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%25), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %130: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=new, atomic=seq_cst>(%26, ptr_offset<ptr<array<f32, 3>>, subtract=true, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%26), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%25), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %131: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=old, atomic=seq_cst>(%26, ptr_offset<ptr<array<f32, 3>>, subtract=true, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%26), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%25), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %132: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=new, atomic=seq_cst>(%26, ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%26), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%25), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %133: ptr<array<f32, 3>> [synthetic] = update<ptr<array<f32, 3>>, result=new, atomic=seq_cst>(%26, ptr_offset<ptr<array<f32, 3>>, subtract=true, element=array<f32, 3>, overflow=ub>(old<ptr<array<f32, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f32, 3>>>(read<ptr<array<f32, 3>>, atomic=seq_cst>(%26), addr_of<ptr<array<f32, 3>>>(deref(ptr_offset<ptr<array<f32, 3>>, subtract=false, element=array<f32, 3>, overflow=ub>(array_decay<ptr<array<f32, 3>>, length=Some(3)>(%25), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %51
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %27 a: array<array<f64, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %28 q: atomic ptr<array<f64, 3>> [storage=automatic] = addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%27), const<i32>(0))));
// DEFAULT-NEXT:                 let %134: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=new, atomic=seq_cst>(%28, ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%28), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%27), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %135: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=old, atomic=seq_cst>(%28, ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%28), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%27), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %136: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=new, atomic=seq_cst>(%28, ptr_offset<ptr<array<f64, 3>>, subtract=true, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%28), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%27), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %137: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=old, atomic=seq_cst>(%28, ptr_offset<ptr<array<f64, 3>>, subtract=true, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%28), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%27), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %138: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=new, atomic=seq_cst>(%28, ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%28), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%27), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %139: ptr<array<f64, 3>> [synthetic] = update<ptr<array<f64, 3>>, result=new, atomic=seq_cst>(%28, ptr_offset<ptr<array<f64, 3>>, subtract=true, element=array<f64, 3>, overflow=ub>(old<ptr<array<f64, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f64, 3>>>(read<ptr<array<f64, 3>>, atomic=seq_cst>(%28), addr_of<ptr<array<f64, 3>>>(deref(ptr_offset<ptr<array<f64, 3>>, subtract=false, element=array<f64, 3>, overflow=ub>(array_decay<ptr<array<f64, 3>>, length=Some(3)>(%27), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %52
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %29 a: array<array<f80, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %30 q: atomic ptr<array<f80, 3>> [storage=automatic] = addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%29), const<i32>(0))));
// DEFAULT-NEXT:                 let %140: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=new, atomic=seq_cst>(%30, ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%30), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%29), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %141: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=old, atomic=seq_cst>(%30, ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%30), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%29), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %142: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=new, atomic=seq_cst>(%30, ptr_offset<ptr<array<f80, 3>>, subtract=true, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%30), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%29), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %143: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=old, atomic=seq_cst>(%30, ptr_offset<ptr<array<f80, 3>>, subtract=true, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%30), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%29), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %144: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=new, atomic=seq_cst>(%30, ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%30), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%29), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %145: ptr<array<f80, 3>> [synthetic] = update<ptr<array<f80, 3>>, result=new, atomic=seq_cst>(%30, ptr_offset<ptr<array<f80, 3>>, subtract=true, element=array<f80, 3>, overflow=ub>(old<ptr<array<f80, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<f80, 3>>>(read<ptr<array<f80, 3>>, atomic=seq_cst>(%30), addr_of<ptr<array<f80, 3>>>(deref(ptr_offset<ptr<array<f80, 3>>, subtract=false, element=array<f80, 3>, overflow=ub>(array_decay<ptr<array<f80, 3>>, length=Some(3)>(%29), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %53
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %31 a: array<array<complex<f32>, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %32 q: atomic ptr<array<complex<f32>, 3>> [storage=automatic] = addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%31), const<i32>(0))));
// DEFAULT-NEXT:                 let %146: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=new, atomic=seq_cst>(%32, ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%32), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%31), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %147: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=old, atomic=seq_cst>(%32, ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%32), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%31), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %148: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=new, atomic=seq_cst>(%32, ptr_offset<ptr<array<complex<f32>, 3>>, subtract=true, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%32), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%31), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %149: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=old, atomic=seq_cst>(%32, ptr_offset<ptr<array<complex<f32>, 3>>, subtract=true, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%32), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%31), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %150: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=new, atomic=seq_cst>(%32, ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%32), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%31), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %151: ptr<array<complex<f32>, 3>> [synthetic] = update<ptr<array<complex<f32>, 3>>, result=new, atomic=seq_cst>(%32, ptr_offset<ptr<array<complex<f32>, 3>>, subtract=true, element=array<complex<f32>, 3>, overflow=ub>(old<ptr<array<complex<f32>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f32>, 3>>>(read<ptr<array<complex<f32>, 3>>, atomic=seq_cst>(%32), addr_of<ptr<array<complex<f32>, 3>>>(deref(ptr_offset<ptr<array<complex<f32>, 3>>, subtract=false, element=array<complex<f32>, 3>, overflow=ub>(array_decay<ptr<array<complex<f32>, 3>>, length=Some(3)>(%31), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %54
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %33 a: array<array<complex<f64>, 3>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 let %34 q: atomic ptr<array<complex<f64>, 3>> [storage=automatic] = addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%33), const<i32>(0))));
// DEFAULT-NEXT:                 let %152: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=new, atomic=seq_cst>(%34, ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%34), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%33), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %153: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=old, atomic=seq_cst>(%34, ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%34), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%33), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %154: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=new, atomic=seq_cst>(%34, ptr_offset<ptr<array<complex<f64>, 3>>, subtract=true, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%34), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%33), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %155: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=old, atomic=seq_cst>(%34, ptr_offset<ptr<array<complex<f64>, 3>>, subtract=true, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%34), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%33), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %156: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=new, atomic=seq_cst>(%34, ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%34), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%33), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %157: ptr<array<complex<f64>, 3>> [synthetic] = update<ptr<array<complex<f64>, 3>>, result=new, atomic=seq_cst>(%34, ptr_offset<ptr<array<complex<f64>, 3>>, subtract=true, element=array<complex<f64>, 3>, overflow=ub>(old<ptr<array<complex<f64>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f64>, 3>>>(read<ptr<array<complex<f64>, 3>>, atomic=seq_cst>(%34), addr_of<ptr<array<complex<f64>, 3>>>(deref(ptr_offset<ptr<array<complex<f64>, 3>>, subtract=false, element=array<complex<f64>, 3>, overflow=ub>(array_decay<ptr<array<complex<f64>, 3>>, length=Some(3)>(%33), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %55
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %35 a: array<array<complex<f80>, 3>, 3> [storage=automatic];
// DEFAULT-NEXT:                 let %36 q: atomic ptr<array<complex<f80>, 3>> [storage=automatic] = addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%35), const<i32>(0))));
// DEFAULT-NEXT:                 let %158: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=new, atomic=seq_cst>(%36, ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%36), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%35), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %159: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=old, atomic=seq_cst>(%36, ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%36), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%35), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %160: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=new, atomic=seq_cst>(%36, ptr_offset<ptr<array<complex<f80>, 3>>, subtract=true, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%36), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%35), const<i32>(1)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %161: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=old, atomic=seq_cst>(%36, ptr_offset<ptr<array<complex<f80>, 3>>, subtract=true, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(1)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%36), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%35), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %162: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=new, atomic=seq_cst>(%36, ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%36), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%35), const<i32>(2)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:                 let %163: ptr<array<complex<f80>, 3>> [synthetic] = update<ptr<array<complex<f80>, 3>>, result=new, atomic=seq_cst>(%36, ptr_offset<ptr<array<complex<f80>, 3>>, subtract=true, element=array<complex<f80>, 3>, overflow=ub>(old<ptr<array<complex<f80>, 3>>>, const<i32>(2)));
// DEFAULT-NEXT:                 if ne<ptr<array<complex<f80>, 3>>>(read<ptr<array<complex<f80>, 3>>, atomic=seq_cst>(%36), addr_of<ptr<array<complex<f80>, 3>>>(deref(ptr_offset<ptr<array<complex<f80>, 3>>, subtract=false, element=array<complex<f80>, 3>, overflow=ub>(array_decay<ptr<array<complex<f80>, 3>>, length=Some(3)>(%35), const<i32>(0)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%38);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
