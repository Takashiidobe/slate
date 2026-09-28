/* Test __atomic routines for existence and proper execution on 4 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_int_long } */

/* Test the execution of the __atomic_*OP builtin routines for an int.  */

extern void abort(void);

int v, count, res;
const int init = ~0;

/* The fetch_op routines return the original value before the operation.  */

void
test_fetch_add ()
{
  v = 0;
  count = 1;

  if (__atomic_fetch_add (&v, count, __ATOMIC_RELAXED) != 0)
    abort ();

  if (__atomic_fetch_add (&v, 1, __ATOMIC_CONSUME) != 1) 
    abort ();

  if (__atomic_fetch_add (&v, count, __ATOMIC_ACQUIRE) != 2)
    abort ();

  if (__atomic_fetch_add (&v, 1, __ATOMIC_RELEASE) != 3) 
    abort ();

  if (__atomic_fetch_add (&v, count, __ATOMIC_ACQ_REL) != 4) 
    abort ();

  if (__atomic_fetch_add (&v, 1, __ATOMIC_SEQ_CST) != 5) 
    abort ();
}


void
test_fetch_sub()
{
  v = res = 20;
  count = 0;

  if (__atomic_fetch_sub (&v, count + 1, __ATOMIC_RELAXED) !=  res--) 
    abort ();

  if (__atomic_fetch_sub (&v, 1, __ATOMIC_CONSUME) !=  res--) 
    abort ();

  if (__atomic_fetch_sub (&v, count + 1, __ATOMIC_ACQUIRE) !=  res--) 
    abort ();

  if (__atomic_fetch_sub (&v, 1, __ATOMIC_RELEASE) !=  res--) 
    abort ();

  if (__atomic_fetch_sub (&v, count + 1, __ATOMIC_ACQ_REL) !=  res--) 
    abort ();

  if (__atomic_fetch_sub (&v, 1, __ATOMIC_SEQ_CST) !=  res--) 
    abort ();
}

void
test_fetch_and ()
{
  v = init;

  if (__atomic_fetch_and (&v, 0, __ATOMIC_RELAXED) !=  init) 
    abort ();

  if (__atomic_fetch_and (&v, init, __ATOMIC_CONSUME) !=  0) 
    abort ();

  if (__atomic_fetch_and (&v, 0, __ATOMIC_ACQUIRE) !=  0)
    abort ();

  v = ~v;
  if (__atomic_fetch_and (&v, init, __ATOMIC_RELEASE) !=  init)
    abort ();

  if (__atomic_fetch_and (&v, 0, __ATOMIC_ACQ_REL) !=  init) 
    abort ();

  if (__atomic_fetch_and (&v, 0, __ATOMIC_SEQ_CST) !=  0) 
    abort ();
}

void
test_fetch_nand ()
{
  v = init;

  if (__atomic_fetch_nand (&v, 0, __ATOMIC_RELAXED) !=  init) 
    abort ();

  if (__atomic_fetch_nand (&v, init, __ATOMIC_CONSUME) !=  init) 
    abort ();

  if (__atomic_fetch_nand (&v, 0, __ATOMIC_ACQUIRE) !=  0 ) 
    abort ();

  if (__atomic_fetch_nand (&v, init, __ATOMIC_RELEASE) !=  init)
    abort ();

  if (__atomic_fetch_nand (&v, init, __ATOMIC_ACQ_REL) !=  0) 
    abort ();

  if (__atomic_fetch_nand (&v, 0, __ATOMIC_SEQ_CST) !=  init) 
    abort ();
}

void
test_fetch_xor ()
{
  v = init;
  count = 0;

  if (__atomic_fetch_xor (&v, count, __ATOMIC_RELAXED) !=  init) 
    abort ();

  if (__atomic_fetch_xor (&v, ~count, __ATOMIC_CONSUME) !=  init) 
    abort ();

  if (__atomic_fetch_xor (&v, 0, __ATOMIC_ACQUIRE) !=  0) 
    abort ();

  if (__atomic_fetch_xor (&v, ~count, __ATOMIC_RELEASE) !=  0) 
    abort ();

  if (__atomic_fetch_xor (&v, 0, __ATOMIC_ACQ_REL) !=  init) 
    abort ();

  if (__atomic_fetch_xor (&v, ~count, __ATOMIC_SEQ_CST) !=  init) 
    abort ();
}

void
test_fetch_or ()
{
  v = 0;
  count = 1;

  if (__atomic_fetch_or (&v, count, __ATOMIC_RELAXED) !=  0) 
    abort ();

  count *= 2;
  if (__atomic_fetch_or (&v, 2, __ATOMIC_CONSUME) !=  1) 
    abort ();

  count *= 2;
  if (__atomic_fetch_or (&v, count, __ATOMIC_ACQUIRE) !=  3) 
    abort ();

  count *= 2;
  if (__atomic_fetch_or (&v, 8, __ATOMIC_RELEASE) !=  7) 
    abort ();

  count *= 2;
  if (__atomic_fetch_or (&v, count, __ATOMIC_ACQ_REL) !=  15) 
    abort ();

  count *= 2;
  if (__atomic_fetch_or (&v, count, __ATOMIC_SEQ_CST) !=  31) 
    abort ();
}

/* The OP_fetch routines return the new value after the operation.  */

void
test_add_fetch ()
{
  v = 0;
  count = 1;

  if (__atomic_add_fetch (&v, count, __ATOMIC_RELAXED) != 1)
    abort ();

  if (__atomic_add_fetch (&v, 1, __ATOMIC_CONSUME) != 2) 
    abort ();

  if (__atomic_add_fetch (&v, count, __ATOMIC_ACQUIRE) != 3)
    abort ();

  if (__atomic_add_fetch (&v, 1, __ATOMIC_RELEASE) != 4) 
    abort ();

  if (__atomic_add_fetch (&v, count, __ATOMIC_ACQ_REL) != 5) 
    abort ();

  if (__atomic_add_fetch (&v, count, __ATOMIC_SEQ_CST) != 6) 
    abort ();
}


void
test_sub_fetch ()
{
  v = res = 20;
  count = 0;

  if (__atomic_sub_fetch (&v, count + 1, __ATOMIC_RELAXED) !=  --res) 
    abort ();

  if (__atomic_sub_fetch (&v, 1, __ATOMIC_CONSUME) !=  --res) 
    abort ();                                                  
                                                               
  if (__atomic_sub_fetch (&v, count + 1, __ATOMIC_ACQUIRE) !=  --res) 
    abort ();                                                  
                                                               
  if (__atomic_sub_fetch (&v, 1, __ATOMIC_RELEASE) !=  --res) 
    abort ();                                                  
                                                               
  if (__atomic_sub_fetch (&v, count + 1, __ATOMIC_ACQ_REL) !=  --res) 
    abort ();                                                  
                                                               
  if (__atomic_sub_fetch (&v, count + 1, __ATOMIC_SEQ_CST) !=  --res) 
    abort ();
}

void
test_and_fetch ()
{
  v = init;

  if (__atomic_and_fetch (&v, 0, __ATOMIC_RELAXED) !=  0) 
    abort ();

  v = init;
  if (__atomic_and_fetch (&v, init, __ATOMIC_CONSUME) !=  init) 
    abort ();

  if (__atomic_and_fetch (&v, 0, __ATOMIC_ACQUIRE) !=  0) 
    abort ();

  v = ~v;
  if (__atomic_and_fetch (&v, init, __ATOMIC_RELEASE) !=  init)
    abort ();

  if (__atomic_and_fetch (&v, 0, __ATOMIC_ACQ_REL) !=  0) 
    abort ();

  v = ~v;
  if (__atomic_and_fetch (&v, 0, __ATOMIC_SEQ_CST) !=  0) 
    abort ();
}

void
test_nand_fetch ()
{
  v = init;

  if (__atomic_nand_fetch (&v, 0, __ATOMIC_RELAXED) !=  init) 
    abort ();              
                           
  if (__atomic_nand_fetch (&v, init, __ATOMIC_CONSUME) !=  0) 
    abort ();              
                           
  if (__atomic_nand_fetch (&v, 0, __ATOMIC_ACQUIRE) !=  init) 
    abort ();              
                           
  if (__atomic_nand_fetch (&v, init, __ATOMIC_RELEASE) !=  0)
    abort ();              
                           
  if (__atomic_nand_fetch (&v, init, __ATOMIC_ACQ_REL) !=  init) 
    abort ();              
                           
  if (__atomic_nand_fetch (&v, 0, __ATOMIC_SEQ_CST) !=  init) 
    abort ();
}



void
test_xor_fetch ()
{
  v = init;
  count = 0;

  if (__atomic_xor_fetch (&v, count, __ATOMIC_RELAXED) !=  init) 
    abort ();

  if (__atomic_xor_fetch (&v, ~count, __ATOMIC_CONSUME) !=  0) 
    abort ();

  if (__atomic_xor_fetch (&v, 0, __ATOMIC_ACQUIRE) !=  0) 
    abort ();

  if (__atomic_xor_fetch (&v, ~count, __ATOMIC_RELEASE) !=  init) 
    abort ();

  if (__atomic_xor_fetch (&v, 0, __ATOMIC_ACQ_REL) !=  init) 
    abort ();

  if (__atomic_xor_fetch (&v, ~count, __ATOMIC_SEQ_CST) !=  0) 
    abort ();
}

void
test_or_fetch ()
{
  v = 0;
  count = 1;

  if (__atomic_or_fetch (&v, count, __ATOMIC_RELAXED) !=  1) 
    abort ();

  count *= 2;
  if (__atomic_or_fetch (&v, 2, __ATOMIC_CONSUME) !=  3) 
    abort ();

  count *= 2;
  if (__atomic_or_fetch (&v, count, __ATOMIC_ACQUIRE) !=  7) 
    abort ();

  count *= 2;
  if (__atomic_or_fetch (&v, 8, __ATOMIC_RELEASE) !=  15) 
    abort ();

  count *= 2;
  if (__atomic_or_fetch (&v, count, __ATOMIC_ACQ_REL) !=  31) 
    abort ();

  count *= 2;
  if (__atomic_or_fetch (&v, count, __ATOMIC_SEQ_CST) !=  63) 
    abort ();
}


/* Test the OP routines with a result which isn't used. Use both variations
   within each function.  */

void
test_add ()
{
  v = 0;
  count = 1;

  __atomic_add_fetch (&v, count, __ATOMIC_RELAXED);
  if (v != 1)
    abort ();

  __atomic_fetch_add (&v, count, __ATOMIC_CONSUME);
  if (v != 2)
    abort ();

  __atomic_add_fetch (&v, 1 , __ATOMIC_ACQUIRE);
  if (v != 3)
    abort ();

  __atomic_fetch_add (&v, 1, __ATOMIC_RELEASE);
  if (v != 4)
    abort ();

  __atomic_add_fetch (&v, count, __ATOMIC_ACQ_REL);
  if (v != 5)
    abort ();

  __atomic_fetch_add (&v, count, __ATOMIC_SEQ_CST);
  if (v != 6)
    abort ();
}


void
test_sub()
{
  v = res = 20;
  count = 0;

  __atomic_sub_fetch (&v, count + 1, __ATOMIC_RELAXED);
  if (v != --res)
    abort ();

  __atomic_fetch_sub (&v, count + 1, __ATOMIC_CONSUME);
  if (v != --res)
    abort ();                                                  
                                                               
  __atomic_sub_fetch (&v, 1, __ATOMIC_ACQUIRE);
  if (v != --res)
    abort ();                                                  
                                                               
  __atomic_fetch_sub (&v, 1, __ATOMIC_RELEASE);
  if (v != --res)
    abort ();                                                  
                                                               
  __atomic_sub_fetch (&v, count + 1, __ATOMIC_ACQ_REL);
  if (v != --res)
    abort ();                                                  
                                                               
  __atomic_fetch_sub (&v, count + 1, __ATOMIC_SEQ_CST);
  if (v != --res)
    abort ();
}

void
test_and ()
{
  v = init;

  __atomic_and_fetch (&v, 0, __ATOMIC_RELAXED);
  if (v != 0)
    abort ();

  v = init;
  __atomic_fetch_and (&v, init, __ATOMIC_CONSUME);
  if (v != init)
    abort ();

  __atomic_and_fetch (&v, 0, __ATOMIC_ACQUIRE);
  if (v != 0)
    abort ();

  v = ~v;
  __atomic_fetch_and (&v, init, __ATOMIC_RELEASE);
  if (v != init)
    abort ();

  __atomic_and_fetch (&v, 0, __ATOMIC_ACQ_REL);
  if (v != 0)
    abort ();

  v = ~v;
  __atomic_fetch_and (&v, 0, __ATOMIC_SEQ_CST);
  if (v != 0)
    abort ();
}

void
test_nand ()
{
  v = init;

  __atomic_fetch_nand (&v, 0, __ATOMIC_RELAXED);
  if (v != init)
    abort ();

  __atomic_fetch_nand (&v, init, __ATOMIC_CONSUME);
  if (v != 0)
    abort ();

  __atomic_nand_fetch (&v, 0, __ATOMIC_ACQUIRE);
  if (v != init)
    abort ();

  __atomic_nand_fetch (&v, init, __ATOMIC_RELEASE);
  if (v != 0)
    abort ();

  __atomic_fetch_nand (&v, init, __ATOMIC_ACQ_REL);
  if (v != init)
    abort ();

  __atomic_nand_fetch (&v, 0, __ATOMIC_SEQ_CST);
  if (v != init)
    abort ();
}



void
test_xor ()
{
  v = init;
  count = 0;

  __atomic_xor_fetch (&v, count, __ATOMIC_RELAXED);
  if (v != init)
    abort ();

  __atomic_fetch_xor (&v, ~count, __ATOMIC_CONSUME);
  if (v != 0)
    abort ();

  __atomic_xor_fetch (&v, 0, __ATOMIC_ACQUIRE);
  if (v != 0)
    abort ();

  __atomic_fetch_xor (&v, ~count, __ATOMIC_RELEASE);
  if (v != init)
    abort ();

  __atomic_fetch_xor (&v, 0, __ATOMIC_ACQ_REL);
  if (v != init)
    abort ();

  __atomic_xor_fetch (&v, ~count, __ATOMIC_SEQ_CST);
  if (v != 0)
    abort ();
}

void
test_or ()
{
  v = 0;
  count = 1;

  __atomic_or_fetch (&v, count, __ATOMIC_RELAXED);
  if (v != 1)
    abort ();

  count *= 2;
  __atomic_fetch_or (&v, count, __ATOMIC_CONSUME);
  if (v != 3)
    abort ();

  count *= 2;
  __atomic_or_fetch (&v, 4, __ATOMIC_ACQUIRE);
  if (v != 7)
    abort ();

  count *= 2;
  __atomic_fetch_or (&v, 8, __ATOMIC_RELEASE);
  if (v != 15)
    abort ();

  count *= 2;
  __atomic_or_fetch (&v, count, __ATOMIC_ACQ_REL);
  if (v != 31)
    abort ();

  count *= 2;
  __atomic_fetch_or (&v, count, __ATOMIC_SEQ_CST);
  if (v != 63)
    abort ();
}

int
main ()
{
  test_fetch_add ();
  test_fetch_sub ();
  test_fetch_and ();
  test_fetch_nand ();
  test_fetch_xor ();
  test_fetch_or ();

  test_add_fetch ();
  test_sub_fetch ();
  test_and_fetch ();
  test_nand_fetch ();
  test_xor_fetch ();
  test_or_fetch ();

  test_add ();
  test_sub ();
  test_and ();
  test_nand ();
  test_xor ();
  test_or ();

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     global %1 v: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 res: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 init: i32 [storage=static] [const] = not<i32>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @test_fetch_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         let %24: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%24), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%25), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%26), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %27: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%27), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %28: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%28), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %29: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%29), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_fetch_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         let %30: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %31: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %32: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%32));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%30), read<i32>(%31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %34: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %35: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%35));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%33), read<i32>(%34))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %36: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %37: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %38: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%37), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%38));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%36), read<i32>(%37))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %40: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %41: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%41));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%39), read<i32>(%40))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %42: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %43: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %44: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%44));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%42), read<i32>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %45: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %46: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %47: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%47));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%45), read<i32>(%46))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_fetch_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         let %48: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%48), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %49: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, read<i32>(%4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%49), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %50: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%50), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, not<i32>(read<i32>(%1)));
// DEFAULT-NEXT:         let %51: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, read<i32>(%4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%51), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %52: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%52), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %53: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%53), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test_fetch_nand(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         let %54: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%54), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %55: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%55), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %56: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%56), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %57: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%57), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %58: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%58), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %59: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%59), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_fetch_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         let %60: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%60), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %61: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%61), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %62: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%62), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %63: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%63), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %64: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%64), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %65: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%65), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_fetch_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         let %66: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%66), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %67: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %68: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%67), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%68));
// DEFAULT-NEXT:         let %69: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%69), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %70: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %71: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%70), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%71));
// DEFAULT-NEXT:         let %72: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%72), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %73: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %74: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%73), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%74));
// DEFAULT-NEXT:         let %75: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, const<i32>(8)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%75), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %76: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %77: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%76), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%77));
// DEFAULT-NEXT:         let %78: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%78), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %79: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %80: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%79), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%80));
// DEFAULT-NEXT:         let %81: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%81), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_add_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         let %82: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%82), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %83: i32 [synthetic] = update<i32, result=new, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%83), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %84: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%84), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %85: i32 [synthetic] = update<i32, result=new, atomic=release>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%85), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %86: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%86), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %87: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%87), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_sub_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         let %88: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %89: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %90: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%89), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%90));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%88), read<i32>(%90))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %91: i32 [synthetic] = update<i32, result=new, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %92: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %93: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%92), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%93));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%91), read<i32>(%93))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %94: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %95: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %96: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%95), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%96));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%94), read<i32>(%96))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %97: i32 [synthetic] = update<i32, result=new, atomic=release>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %98: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %99: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%98), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%99));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%97), read<i32>(%99))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %100: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %101: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %102: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%101), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%102));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%100), read<i32>(%102))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %103: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %104: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %105: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%104), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%105));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%103), read<i32>(%105))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test_and_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         let %106: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%106), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         let %107: i32 [synthetic] = update<i32, result=new, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, read<i32>(%4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%107), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %108: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%108), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, not<i32>(read<i32>(%1)));
// DEFAULT-NEXT:         let %109: i32 [synthetic] = update<i32, result=new, atomic=release>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, read<i32>(%4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%109), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %110: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%110), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, not<i32>(read<i32>(%1)));
// DEFAULT-NEXT:         let %111: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%111), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_nand_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         let %112: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%112), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %113: i32 [synthetic] = update<i32, result=new, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%113), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %114: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%114), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %115: i32 [synthetic] = update<i32, result=new, atomic=release>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%115), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %116: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%116), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %117: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%117), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_xor_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         let %118: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%118), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %119: i32 [synthetic] = update<i32, result=new, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%119), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %120: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%120), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %121: i32 [synthetic] = update<i32, result=new, atomic=release>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%121), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %122: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%122), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %123: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%123), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_or_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         let %124: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%124), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %125: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %126: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%125), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%126));
// DEFAULT-NEXT:         let %127: i32 [synthetic] = update<i32, result=new, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%127), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %128: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %129: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%128), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%129));
// DEFAULT-NEXT:         let %130: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%130), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %131: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %132: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%131), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%132));
// DEFAULT-NEXT:         let %133: i32 [synthetic] = update<i32, result=new, atomic=release>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, const<i32>(8)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%133), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %134: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %135: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%134), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%135));
// DEFAULT-NEXT:         let %136: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%136), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %137: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %138: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%137), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%138));
// DEFAULT-NEXT:         let %139: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%139), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         let %140: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %141: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %142: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %143: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %144: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %145: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), add<i32, overflow=wrap>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         let %146: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %147: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %148: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%147), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%148));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%148))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %149: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %150: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %151: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%150), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%151));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%151))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %152: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %153: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %154: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%153), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%154));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%154))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %155: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %156: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %157: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%156), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%157));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%157))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %158: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %159: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %160: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%159), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%160));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%160))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %161: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))));
// DEFAULT-NEXT:         let %162: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:         let %163: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%162), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%3, read<i32>(%163));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%163))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         let %164: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         let %165: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, read<i32>(%4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %166: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, not<i32>(read<i32>(%1)));
// DEFAULT-NEXT:         let %167: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, read<i32>(%4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %168: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%1, not<i32>(read<i32>(%1)));
// DEFAULT-NEXT:         let %169: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_nand(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         let %170: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %171: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %172: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %173: i32 [synthetic] = update<i32, result=new, atomic=release>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %174: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, read<i32>(%4))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %175: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), not<i32>(and<i32>(old<i32>, const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, read<i32>(%4));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         let %176: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %177: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %178: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %179: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %180: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), read<i32>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %181: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), xor<i32>(old<i32>, not<i32>(read<i32>(%2))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         let %182: i32 [synthetic] = update<i32, result=new, atomic=relaxed>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %183: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %184: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%183), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%184));
// DEFAULT-NEXT:         let %185: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %186: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %187: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%186), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%187));
// DEFAULT-NEXT:         let %188: i32 [synthetic] = update<i32, result=new, atomic=acquire>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %189: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %190: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%189), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%190));
// DEFAULT-NEXT:         let %191: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, const<i32>(8)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %192: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %193: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%192), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%193));
// DEFAULT-NEXT:         let %194: i32 [synthetic] = update<i32, result=new, atomic=acq_rel>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %195: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:         let %196: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%195), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%2, read<i32>(%196));
// DEFAULT-NEXT:         let %197: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%1)), or<i32>(old<i32>, read<i32>(%2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%7);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%9);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%10);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%11);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%12);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%13);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%14);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%15);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%16);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%17);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%18);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%19);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%20);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%21);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%22);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
