/* Test __atomic routines for existence and proper execution on 1 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_*OP builtin routines for a char.  */

extern void abort(void);

char v, count, res;
const char init = ~0;

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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_res:[0-9]+]] res: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_init:[0-9]+]] init: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_add:[0-9]+]] @test_fetch_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE0]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE1]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE2]])), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE3]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE4]])), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE5]])), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_sub:[0-9]+]] @test_fetch_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE7]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE8]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE6]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE7]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE10]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE11]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE9]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE10]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE13]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE14]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE12]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE13]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE16]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE17]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE15]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE16]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE19]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE20]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE18]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE19]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE22]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE23]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE21]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE22]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_and:[0-9]+]] @test_fetch_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE24]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, read<i8>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE25]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE26]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, read<i8>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE27]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE28]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE29]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_nand:[0-9]+]] @test_fetch_nand(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE30]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE31]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE32]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE33]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE34]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE35]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_xor:[0-9]+]] @test_fetch_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE36]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE37]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE38]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE39]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE40]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE41]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_or:[0-9]+]] @test_fetch_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE42]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE43]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE44]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE45]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE46]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE48]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE49]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE51]])), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE52]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE53]]));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE54]])), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE55]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE56]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE57]])), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_add_fetch:[0-9]+]] @test_add_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE58]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE59]])), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE60]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE61]])), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE62]])), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE63]])), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_sub_fetch:[0-9]+]] @test_sub_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE65]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE66]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE64]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE66]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE68]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE69]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE67]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE69]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE71]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE72]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE70]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE72]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE74]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE75]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE73]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE75]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE77]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE78]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE76]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE78]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE80]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE81]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE79]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE81]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_and_fetch:[0-9]+]] @test_and_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE82]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, read<i8>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE83]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE84]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, read<i8>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE85]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE86]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))));
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE87]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_nand_fetch:[0-9]+]] @test_nand_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE88]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE89]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE90]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE91]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE92]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE93]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_xor_fetch:[0-9]+]] @test_xor_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE94:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE94]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE95]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE96]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE97]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE98]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE99]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_or_fetch:[0-9]+]] @test_or_fetch(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE100]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE101]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE102]]));
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE103]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE104]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE105]]));
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE106]])), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE107]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE108]]));
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE109]])), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE110]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE111]]));
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE112]])), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE113]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE114]]));
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE115]])), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_add:[0-9]+]] @test_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE118:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE119:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE120:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE121:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), add<i8, overflow=wrap>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_sub:[0-9]+]] @test_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE122:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE123:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE124:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE123]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE124]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE124]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE125:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE126:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE127:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE126]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE127]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE127]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE128:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE129:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE129]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE130]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE130]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE132:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE133:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE132]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE133]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE133]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE134:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE135]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE136]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE136]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE137:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE139:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE138]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_res]], read<i8>(%[[VALUE139]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE139]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_and:[0-9]+]] @test_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, read<i8>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))));
// DEFAULT-NEXT:         let %[[VALUE143:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, read<i8>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])))));
// DEFAULT-NEXT:         let %[[VALUE145:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_nand:[0-9]+]] @test_nand(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE146:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE147:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE150:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, read<i8>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE151:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_xor:[0-9]+]] @test_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], read<i8>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE153:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE155:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE156:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i8>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE157:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_or:[0-9]+]] @test_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%[[VALUE_v]], truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE158:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE159:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE160:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE159]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE160]]));
// DEFAULT-NEXT:         let %[[VALUE161:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE163:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE162]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE163]]));
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE165:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE165]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE166]]));
// DEFAULT-NEXT:         let %[[VALUE167:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE169:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE168]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE169]]));
// DEFAULT-NEXT:         let %[[VALUE170:[0-9]+]]: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE171:[0-9]+]]: i8 [synthetic] = read<i8>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE172:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%[[VALUE171]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%[[VALUE_count]], read<i8>(%[[VALUE172]]));
// DEFAULT-NEXT:         let %[[VALUE173:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_v]])), or<i8>(old<i8>, read<i8>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE_v]])), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_add]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_and]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_nand]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_or]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_add_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_sub_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_and_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_nand_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_xor_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_or_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_add]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_and]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_nand]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_or]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
