/* Test __atomic routines for existence and proper execution on 16 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_int_128_runtime } */
/* { dg-options "-mcx16" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-options "-mlsx -mscq" { target { loongarch64-*-* } } } */

/* Test the execution of the __atomic_*OP builtin routines for an int_128.  */

extern void abort(void);

__int128_t v, count, res;
const __int128_t init = ~0;

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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_res:[0-9]+]] res: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_init:[0-9]+]] init: i128 [storage=static] [const] = widen<i128, reason=assign>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_add:[0-9]+]] @test_fetch_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE0]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE1]]), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE2]]), widen<i128, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE3]]), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE4]]), widen<i128, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE5]]), widen<i128, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_sub:[0-9]+]] @test_fetch_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE7]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE8]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE6]]), read<i128>(%[[VALUE7]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE10]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE11]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE9]]), read<i128>(%[[VALUE10]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE13]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE14]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE12]]), read<i128>(%[[VALUE13]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE16]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE17]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE15]]), read<i128>(%[[VALUE16]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE19]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE20]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE18]]), read<i128>(%[[VALUE19]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE22]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE23]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE21]]), read<i128>(%[[VALUE22]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_and:[0-9]+]] @test_fetch_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE24]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, read<i128>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE25]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE26]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], not<i128>(read<i128>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, read<i128>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE27]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE28]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE29]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_nand:[0-9]+]] @test_fetch_nand() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE30]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE31]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE32]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE33]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE34]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE35]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_xor:[0-9]+]] @test_fetch_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE36]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE37]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE38]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE39]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE40]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE41]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_or:[0-9]+]] @test_fetch_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE42]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE43]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE44]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE45]]), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE46]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE48]]), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE49]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE51]]), widen<i128, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE52]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE53]]));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE54]]), widen<i128, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE55]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE56]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE57]]), widen<i128, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_add_fetch:[0-9]+]] @test_add_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE58]]), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE59]]), widen<i128, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE60]]), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE61]]), widen<i128, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE62]]), widen<i128, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE63]]), widen<i128, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_sub_fetch:[0-9]+]] @test_sub_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE65]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE66]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE64]]), read<i128>(%[[VALUE66]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE68]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE69]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE67]]), read<i128>(%[[VALUE69]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE71]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE72]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE70]]), read<i128>(%[[VALUE72]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE74]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE75]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE73]]), read<i128>(%[[VALUE75]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE77]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE78]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE76]]), read<i128>(%[[VALUE78]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE80]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE81]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE79]]), read<i128>(%[[VALUE81]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_and_fetch:[0-9]+]] @test_and_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE82]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, read<i128>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE83]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE84]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], not<i128>(read<i128>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, read<i128>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE85]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE86]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], not<i128>(read<i128>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE87]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_nand_fetch:[0-9]+]] @test_nand_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE88]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE89]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE90]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE91]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE92]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE93]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_xor_fetch:[0-9]+]] @test_xor_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE94:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE94]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE95]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE96]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE97]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE98]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE99]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_or_fetch:[0-9]+]] @test_or_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE100]]), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE101]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE102]]));
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE103]]), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE104]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE105]]));
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE106]]), widen<i128, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE107]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE108]]));
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE109]]), widen<i128, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE110]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE111]]));
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE112]]), widen<i128, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE113:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE114:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE113]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE114]]));
// DEFAULT-NEXT:         let %[[VALUE115:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE115]]), widen<i128, reason=usual_arith>(const<i32>(63)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_add:[0-9]+]] @test_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE116:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE117:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE118:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE119:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE120:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE121:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), add<i128, overflow=wrap>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_sub:[0-9]+]] @test_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE122:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE123:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE124:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE123]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE124]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE124]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE125:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE126:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE127:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE126]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE127]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE127]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE128:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE129:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE130:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE129]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE130]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE130]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE131:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE132:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE133:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE132]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE133]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE133]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE134:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE135:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE136:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE135]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE136]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE136]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE137:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%[[VALUE_count]]), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE138:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE139:[0-9]+]]: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%[[VALUE138]]), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_res]], read<i128>(%[[VALUE139]]));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE139]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_and:[0-9]+]] @test_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE140:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE141:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, read<i128>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE142:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], not<i128>(read<i128>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE143:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, read<i128>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE144:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], not<i128>(read<i128>(%[[VALUE_v]])));
// DEFAULT-NEXT:         let %[[VALUE145:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_nand:[0-9]+]] @test_nand() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE146:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE147:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE148:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE149:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE150:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, read<i128>(%[[VALUE_init]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE151:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_xor:[0-9]+]] @test_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], read<i128>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE152:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE153:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE154:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE155:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE156:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), read<i128>(%[[VALUE_init]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE157:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), xor<i128>(old<i128>, not<i128>(read<i128>(%[[VALUE_count]]))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_or:[0-9]+]] @test_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%[[VALUE_v]], widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE158:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE159:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE160:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE159]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE160]]));
// DEFAULT-NEXT:         let %[[VALUE161:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE162:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE163:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE162]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE163]]));
// DEFAULT-NEXT:         let %[[VALUE164:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE165:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE166:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE165]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE166]]));
// DEFAULT-NEXT:         let %[[VALUE167:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE168:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE169:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE168]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE169]]));
// DEFAULT-NEXT:         let %[[VALUE170:[0-9]+]]: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE171:[0-9]+]]: i128 [synthetic] = read<i128>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE172:[0-9]+]]: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%[[VALUE171]]), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%[[VALUE_count]], read<i128>(%[[VALUE172]]));
// DEFAULT-NEXT:         let %[[VALUE173:[0-9]+]]: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%[[VALUE_v]])), or<i128>(old<i128>, read<i128>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%[[VALUE_v]]), widen<i128, reason=usual_arith>(const<i32>(63)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_add]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_and]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_nand]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_fetch_or]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_add_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_sub_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_and_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_nand_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_xor_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_or_fetch]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_add]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_and]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_nand]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_test_or]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
