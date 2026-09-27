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

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     global %1 v: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 res: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 init: i128 [storage=static] [const] = widen<i128, reason=assign>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @test_fetch_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %24: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%24), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %25: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%25), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%26), widen<i128, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %27: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%27), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %28: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%28), widen<i128, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %29: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%29), widen<i128, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_fetch_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%3, widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %30: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %31: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %32: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%31), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%32));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%30), read<i128>(%31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %33: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %34: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %35: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%34), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%35));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%33), read<i128>(%34))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %36: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %37: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %38: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%37), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%38));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%36), read<i128>(%37))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %39: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %40: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %41: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%40), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%41));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%39), read<i128>(%40))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %42: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %43: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %44: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%43), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%44));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%42), read<i128>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %45: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %46: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %47: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%46), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%47));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%45), read<i128>(%46))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_fetch_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         let %48: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%48), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %49: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, read<i128>(%4)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%49), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %50: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%50), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i128>(%1, not<i128>(read<i128>(%1)));
// DEFAULT-NEXT:         let %51: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, read<i128>(%4)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%51), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %52: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%52), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %53: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%53), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test_fetch_nand() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         let %54: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%54), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %55: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%55), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %56: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%56), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %57: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%57), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %58: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%58), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %59: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%59), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_fetch_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %60: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%60), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %61: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%61), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %62: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%62), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %63: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%63), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %64: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%64), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %65: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%65), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_fetch_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %66: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%66), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %67: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %68: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%67), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%68));
// DEFAULT-NEXT:         let %69: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%69), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %70: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %71: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%70), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%71));
// DEFAULT-NEXT:         let %72: i128 [synthetic] = update<i128, result=old, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%72), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %73: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %74: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%73), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%74));
// DEFAULT-NEXT:         let %75: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%75), widen<i128, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %76: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %77: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%76), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%77));
// DEFAULT-NEXT:         let %78: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%78), widen<i128, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %79: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %80: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%79), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%80));
// DEFAULT-NEXT:         let %81: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%81), widen<i128, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_add_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %82: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%82), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %83: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%83), widen<i128, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %84: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%84), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %85: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%85), widen<i128, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %86: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%86), widen<i128, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %87: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%87), widen<i128, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_sub_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%3, widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %88: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %89: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %90: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%89), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%90));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%88), read<i128>(%90))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %91: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %92: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %93: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%92), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%93));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%91), read<i128>(%93))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %94: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %95: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %96: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%95), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%96));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%94), read<i128>(%96))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %97: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %98: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %99: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%98), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%99));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%97), read<i128>(%99))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %100: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %101: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %102: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%101), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%102));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%100), read<i128>(%102))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %103: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %104: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %105: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%104), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%105));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%103), read<i128>(%105))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test_and_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         let %106: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%106), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         let %107: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, read<i128>(%4)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%107), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %108: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%108), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i128>(%1, not<i128>(read<i128>(%1)));
// DEFAULT-NEXT:         let %109: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, read<i128>(%4)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%109), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %110: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%110), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i128>(%1, not<i128>(read<i128>(%1)));
// DEFAULT-NEXT:         let %111: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%111), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_nand_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         let %112: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%112), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %113: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%113), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %114: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%114), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %115: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%115), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %116: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%116), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %117: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%117), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_xor_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %118: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%118), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %119: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%119), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %120: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%120), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %121: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%121), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %122: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%122), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %123: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%123), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_or_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %124: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%124), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %125: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %126: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%125), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%126));
// DEFAULT-NEXT:         let %127: i128 [synthetic] = update<i128, result=new, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%127), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %128: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %129: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%128), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%129));
// DEFAULT-NEXT:         let %130: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%130), widen<i128, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %131: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %132: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%131), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%132));
// DEFAULT-NEXT:         let %133: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%133), widen<i128, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %134: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %135: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%134), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%135));
// DEFAULT-NEXT:         let %136: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%136), widen<i128, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %137: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %138: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%137), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%138));
// DEFAULT-NEXT:         let %139: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%139), widen<i128, reason=usual_arith>(const<i32>(63)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %140: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %141: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %142: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %143: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %144: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %145: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), add<i128, overflow=wrap>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%3, widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %146: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %147: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %148: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%147), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%148));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%148))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %149: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %150: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %151: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%150), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%151));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%151))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %152: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %153: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %154: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%153), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%154));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%154))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %155: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, widen<i128, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %156: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %157: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%156), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%157));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%157))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %158: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %159: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %160: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%159), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%160));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%160))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %161: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), sub<i128, overflow=wrap>(old<i128>, add<i128, overflow=ub>(read<i128>(%2), widen<i128, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %162: i128 [synthetic] = read<i128>(%3);
// DEFAULT-NEXT:         let %163: i128 [synthetic] = sub<i128, overflow=ub>(read<i128>(%162), widen<i128, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i128>(%3, read<i128>(%163));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%163))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         let %164: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         let %165: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, read<i128>(%4)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %166: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i128>(%1, not<i128>(read<i128>(%1)));
// DEFAULT-NEXT:         let %167: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, read<i128>(%4)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %168: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i128>(%1, not<i128>(read<i128>(%1)));
// DEFAULT-NEXT:         let %169: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_nand() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         let %170: i128 [synthetic] = update<i128, result=old, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %171: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %172: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %173: i128 [synthetic] = update<i128, result=new, atomic=release>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %174: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, read<i128>(%4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %175: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), not<i128>(and<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, read<i128>(%4));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %176: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %177: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %178: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %179: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %180: i128 [synthetic] = update<i128, result=old, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), read<i128>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %181: i128 [synthetic] = update<i128, result=new, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), xor<i128>(old<i128>, not<i128>(read<i128>(%2))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i128>(%1, widen<i128, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i128>(%2, widen<i128, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %182: i128 [synthetic] = update<i128, result=new, atomic=relaxed>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %183: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %184: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%183), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%184));
// DEFAULT-NEXT:         let %185: i128 [synthetic] = update<i128, result=old, atomic=consume>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %186: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %187: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%186), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%187));
// DEFAULT-NEXT:         let %188: i128 [synthetic] = update<i128, result=new, atomic=acquire>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %189: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %190: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%189), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%190));
// DEFAULT-NEXT:         let %191: i128 [synthetic] = update<i128, result=old, atomic=release>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, widen<i128, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %192: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %193: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%192), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%193));
// DEFAULT-NEXT:         let %194: i128 [synthetic] = update<i128, result=new, atomic=acq_rel>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %195: i128 [synthetic] = read<i128>(%2);
// DEFAULT-NEXT:         let %196: i128 [synthetic] = mul<i128, overflow=ub>(read<i128>(%195), widen<i128, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i128>(%2, read<i128>(%196));
// DEFAULT-NEXT:         let %197: i128 [synthetic] = update<i128, result=old, atomic=seq_cst>(deref(addr_of<ptr<i128>>(%1)), or<i128>(old<i128>, read<i128>(%2)));
// DEFAULT-NEXT:         if ne<i128>(read<i128>(%1), widen<i128, reason=usual_arith>(const<i32>(63)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%13);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%17);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%18);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%19);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%20);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%21);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%22);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
