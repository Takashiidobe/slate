/* Test __atomic routines for existence and proper execution on 2 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */


/* Test the execution of the __atomic_*OP builtin routines for a short.  */

extern void abort(void);

short v, count, res;
const short init = ~0;

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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %1 v: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 res: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 init: i16 [storage=static] [const] = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @test_fetch_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %24: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%24)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %25: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%25)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%26)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %27: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%27)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %28: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%28)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %29: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%29)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_fetch_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%3, truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %30: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %31: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %32: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%31)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%32));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%30)), widen<i32, reason=promotion>(read<i16>(%31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %33: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %34: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %35: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%34)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%35));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%33)), widen<i32, reason=promotion>(read<i16>(%34)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %36: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %37: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %38: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%37)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%38));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%36)), widen<i32, reason=promotion>(read<i16>(%37)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %39: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %40: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %41: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%40)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%41));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%39)), widen<i32, reason=promotion>(read<i16>(%40)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %42: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %43: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %44: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%43)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%44));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%42)), widen<i32, reason=promotion>(read<i16>(%43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %45: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %46: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %47: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%46)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%47));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%45)), widen<i32, reason=promotion>(read<i16>(%46)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_fetch_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         let %48: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%48)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %49: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, read<i16>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%49)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %50: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%50)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%1)))));
// DEFAULT-NEXT:         let %51: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, read<i16>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%51)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %52: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%52)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %53: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%53)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test_fetch_nand(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         let %54: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%54)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %55: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%55)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %56: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%56)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %57: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%57)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %58: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%58)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %59: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%59)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_fetch_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %60: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%60)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %61: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%61)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %62: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%62)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %63: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%63)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %64: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%64)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %65: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%65)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_fetch_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %66: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%66)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %67: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %68: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%67)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%68));
// DEFAULT-NEXT:         let %69: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%69)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %70: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %71: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%70)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%71));
// DEFAULT-NEXT:         let %72: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%72)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %73: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %74: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%73)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%74));
// DEFAULT-NEXT:         let %75: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%75)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %76: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %77: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%76)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%77));
// DEFAULT-NEXT:         let %78: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%78)), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %79: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %80: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%79)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%80));
// DEFAULT-NEXT:         let %81: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%81)), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_add_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %82: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%82)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %83: i16 [synthetic] = update<i16, result=new, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%83)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %84: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%84)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %85: i16 [synthetic] = update<i16, result=new, atomic=release>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%85)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %86: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%86)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %87: i16 [synthetic] = update<i16, result=new, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%87)), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_sub_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%3, truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %88: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %89: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %90: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%89)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%90));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%88)), widen<i32, reason=promotion>(read<i16>(%90)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %91: i16 [synthetic] = update<i16, result=new, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %92: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %93: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%92)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%93));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%91)), widen<i32, reason=promotion>(read<i16>(%93)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %94: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %95: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %96: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%95)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%96));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%94)), widen<i32, reason=promotion>(read<i16>(%96)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %97: i16 [synthetic] = update<i16, result=new, atomic=release>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %98: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %99: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%98)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%99));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%97)), widen<i32, reason=promotion>(read<i16>(%99)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %100: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %101: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %102: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%101)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%102));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%100)), widen<i32, reason=promotion>(read<i16>(%102)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %103: i16 [synthetic] = update<i16, result=new, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %104: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %105: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%104)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%105));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%103)), widen<i32, reason=promotion>(read<i16>(%105)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test_and_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         let %106: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%106)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         let %107: i16 [synthetic] = update<i16, result=new, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, read<i16>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%107)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %108: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%108)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%1)))));
// DEFAULT-NEXT:         let %109: i16 [synthetic] = update<i16, result=new, atomic=release>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, read<i16>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%109)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %110: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%110)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%1)))));
// DEFAULT-NEXT:         let %111: i16 [synthetic] = update<i16, result=new, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%111)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_nand_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         let %112: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%112)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %113: i16 [synthetic] = update<i16, result=new, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%113)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %114: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%114)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %115: i16 [synthetic] = update<i16, result=new, atomic=release>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%115)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %116: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%116)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %117: i16 [synthetic] = update<i16, result=new, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%117)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_xor_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %118: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%118)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %119: i16 [synthetic] = update<i16, result=new, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%119)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %120: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%120)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %121: i16 [synthetic] = update<i16, result=new, atomic=release>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%121)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %122: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%122)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %123: i16 [synthetic] = update<i16, result=new, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%123)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_or_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %124: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%124)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %125: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %126: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%125)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%126));
// DEFAULT-NEXT:         let %127: i16 [synthetic] = update<i16, result=new, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%127)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %128: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %129: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%128)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%129));
// DEFAULT-NEXT:         let %130: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%130)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %131: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %132: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%131)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%132));
// DEFAULT-NEXT:         let %133: i16 [synthetic] = update<i16, result=new, atomic=release>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%133)), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %134: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %135: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%134)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%135));
// DEFAULT-NEXT:         let %136: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%136)), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %137: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %138: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%137)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%138));
// DEFAULT-NEXT:         let %139: i16 [synthetic] = update<i16, result=new, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%139)), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %140: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %141: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %142: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %143: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %144: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %145: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), add<i16, overflow=wrap>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%3, truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %146: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %147: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %148: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%147)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%148));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%148)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %149: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %150: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %151: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%150)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%151));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%151)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %152: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %153: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %154: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%153)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%154));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%154)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %155: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %156: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %157: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%156)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%157));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%157)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %158: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %159: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %160: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%159)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%160));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%160)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %161: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %162: i16 [synthetic] = read<i16>(%3);
// DEFAULT-NEXT:         let %163: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%162)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%3, read<i16>(%163));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%163)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         let %164: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         let %165: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, read<i16>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %166: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%1)))));
// DEFAULT-NEXT:         let %167: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, read<i16>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %168: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%1)))));
// DEFAULT-NEXT:         let %169: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_nand(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         let %170: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %171: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %172: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %173: i16 [synthetic] = update<i16, result=new, atomic=release>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %174: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, read<i16>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %175: i16 [synthetic] = update<i16, result=new, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), not<i16>(and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, read<i16>(%4));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %176: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %177: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %178: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %179: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %180: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), widen<i32, reason=promotion>(read<i16>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %181: i16 [synthetic] = update<i16, result=new, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%1, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%2, truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %182: i16 [synthetic] = update<i16, result=new, atomic=relaxed>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %183: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %184: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%183)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%184));
// DEFAULT-NEXT:         let %185: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %186: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %187: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%186)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%187));
// DEFAULT-NEXT:         let %188: i16 [synthetic] = update<i16, result=new, atomic=acquire>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %189: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %190: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%189)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%190));
// DEFAULT-NEXT:         let %191: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %192: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %193: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%192)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%193));
// DEFAULT-NEXT:         let %194: i16 [synthetic] = update<i16, result=new, atomic=acq_rel>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %195: i16 [synthetic] = read<i16>(%2);
// DEFAULT-NEXT:         let %196: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%195)), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%2, read<i16>(%196));
// DEFAULT-NEXT:         let %197: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<i16>>(%1)), or<i16>(old<i16>, read<i16>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%1)), const<i32>(63))
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
