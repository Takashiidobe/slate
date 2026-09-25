/* Test __atomic routines for existence and proper execution on 8 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_long_long_runtime } */
/* { dg-options "" } */
/* { dg-options "-march=pentium" { target { { i?86-*-* x86_64-*-* } && ia32 } } } */

/* Test the execution of the __atomic_*OP builtin routines for long long.  */

extern void abort(void);

long long       v, count, res;
const long long init = ~0;

/* The fetch_op routines return the original value before the operation.  */

void test_fetch_add() {
  v     = 0;
  count = 1;

  if (__atomic_fetch_add(&v, count, __ATOMIC_RELAXED) != 0)
    abort();

  if (__atomic_fetch_add(&v, 1, __ATOMIC_CONSUME) != 1)
    abort();

  if (__atomic_fetch_add(&v, count, __ATOMIC_ACQUIRE) != 2)
    abort();

  if (__atomic_fetch_add(&v, 1, __ATOMIC_RELEASE) != 3)
    abort();

  if (__atomic_fetch_add(&v, count, __ATOMIC_ACQ_REL) != 4)
    abort();

  if (__atomic_fetch_add(&v, 1, __ATOMIC_SEQ_CST) != 5)
    abort();
}

void test_fetch_sub() {
  v = res = 20;
  count   = 0;

  if (__atomic_fetch_sub(&v, count + 1, __ATOMIC_RELAXED) != res--)
    abort();

  if (__atomic_fetch_sub(&v, 1, __ATOMIC_CONSUME) != res--)
    abort();

  if (__atomic_fetch_sub(&v, count + 1, __ATOMIC_ACQUIRE) != res--)
    abort();

  if (__atomic_fetch_sub(&v, 1, __ATOMIC_RELEASE) != res--)
    abort();

  if (__atomic_fetch_sub(&v, count + 1, __ATOMIC_ACQ_REL) != res--)
    abort();

  if (__atomic_fetch_sub(&v, 1, __ATOMIC_SEQ_CST) != res--)
    abort();
}

void test_fetch_and() {
  v = init;

  if (__atomic_fetch_and(&v, 0, __ATOMIC_RELAXED) != init)
    abort();

  if (__atomic_fetch_and(&v, init, __ATOMIC_CONSUME) != 0)
    abort();

  if (__atomic_fetch_and(&v, 0, __ATOMIC_ACQUIRE) != 0)
    abort();

  v = ~v;
  if (__atomic_fetch_and(&v, init, __ATOMIC_RELEASE) != init)
    abort();

  if (__atomic_fetch_and(&v, 0, __ATOMIC_ACQ_REL) != init)
    abort();

  if (__atomic_fetch_and(&v, 0, __ATOMIC_SEQ_CST) != 0)
    abort();
}

void test_fetch_nand() {
  v = init;

  if (__atomic_fetch_nand(&v, 0, __ATOMIC_RELAXED) != init)
    abort();

  if (__atomic_fetch_nand(&v, init, __ATOMIC_CONSUME) != init)
    abort();

  if (__atomic_fetch_nand(&v, 0, __ATOMIC_ACQUIRE) != 0)
    abort();

  if (__atomic_fetch_nand(&v, init, __ATOMIC_RELEASE) != init)
    abort();

  if (__atomic_fetch_nand(&v, init, __ATOMIC_ACQ_REL) != 0)
    abort();

  if (__atomic_fetch_nand(&v, 0, __ATOMIC_SEQ_CST) != init)
    abort();
}

void test_fetch_xor() {
  v     = init;
  count = 0;

  if (__atomic_fetch_xor(&v, count, __ATOMIC_RELAXED) != init)
    abort();

  if (__atomic_fetch_xor(&v, ~count, __ATOMIC_CONSUME) != init)
    abort();

  if (__atomic_fetch_xor(&v, 0, __ATOMIC_ACQUIRE) != 0)
    abort();

  if (__atomic_fetch_xor(&v, ~count, __ATOMIC_RELEASE) != 0)
    abort();

  if (__atomic_fetch_xor(&v, 0, __ATOMIC_ACQ_REL) != init)
    abort();

  if (__atomic_fetch_xor(&v, ~count, __ATOMIC_SEQ_CST) != init)
    abort();
}

void test_fetch_or() {
  v     = 0;
  count = 1;

  if (__atomic_fetch_or(&v, count, __ATOMIC_RELAXED) != 0)
    abort();

  count *= 2;
  if (__atomic_fetch_or(&v, 2, __ATOMIC_CONSUME) != 1)
    abort();

  count *= 2;
  if (__atomic_fetch_or(&v, count, __ATOMIC_ACQUIRE) != 3)
    abort();

  count *= 2;
  if (__atomic_fetch_or(&v, 8, __ATOMIC_RELEASE) != 7)
    abort();

  count *= 2;
  if (__atomic_fetch_or(&v, count, __ATOMIC_ACQ_REL) != 15)
    abort();

  count *= 2;
  if (__atomic_fetch_or(&v, count, __ATOMIC_SEQ_CST) != 31)
    abort();
}

/* The OP_fetch routines return the new value after the operation.  */

void test_add_fetch() {
  v     = 0;
  count = 1;

  if (__atomic_add_fetch(&v, count, __ATOMIC_RELAXED) != 1)
    abort();

  if (__atomic_add_fetch(&v, 1, __ATOMIC_CONSUME) != 2)
    abort();

  if (__atomic_add_fetch(&v, count, __ATOMIC_ACQUIRE) != 3)
    abort();

  if (__atomic_add_fetch(&v, 1, __ATOMIC_RELEASE) != 4)
    abort();

  if (__atomic_add_fetch(&v, count, __ATOMIC_ACQ_REL) != 5)
    abort();

  if (__atomic_add_fetch(&v, count, __ATOMIC_SEQ_CST) != 6)
    abort();
}

void test_sub_fetch() {
  v = res = 20;
  count   = 0;

  if (__atomic_sub_fetch(&v, count + 1, __ATOMIC_RELAXED) != --res)
    abort();

  if (__atomic_sub_fetch(&v, 1, __ATOMIC_CONSUME) != --res)
    abort();

  if (__atomic_sub_fetch(&v, count + 1, __ATOMIC_ACQUIRE) != --res)
    abort();

  if (__atomic_sub_fetch(&v, 1, __ATOMIC_RELEASE) != --res)
    abort();

  if (__atomic_sub_fetch(&v, count + 1, __ATOMIC_ACQ_REL) != --res)
    abort();

  if (__atomic_sub_fetch(&v, count + 1, __ATOMIC_SEQ_CST) != --res)
    abort();
}

void test_and_fetch() {
  v = init;

  if (__atomic_and_fetch(&v, 0, __ATOMIC_RELAXED) != 0)
    abort();

  v = init;
  if (__atomic_and_fetch(&v, init, __ATOMIC_CONSUME) != init)
    abort();

  if (__atomic_and_fetch(&v, 0, __ATOMIC_ACQUIRE) != 0)
    abort();

  v = ~v;
  if (__atomic_and_fetch(&v, init, __ATOMIC_RELEASE) != init)
    abort();

  if (__atomic_and_fetch(&v, 0, __ATOMIC_ACQ_REL) != 0)
    abort();

  v = ~v;
  if (__atomic_and_fetch(&v, 0, __ATOMIC_SEQ_CST) != 0)
    abort();
}

void test_nand_fetch() {
  v = init;

  if (__atomic_nand_fetch(&v, 0, __ATOMIC_RELAXED) != init)
    abort();

  if (__atomic_nand_fetch(&v, init, __ATOMIC_CONSUME) != 0)
    abort();

  if (__atomic_nand_fetch(&v, 0, __ATOMIC_ACQUIRE) != init)
    abort();

  if (__atomic_nand_fetch(&v, init, __ATOMIC_RELEASE) != 0)
    abort();

  if (__atomic_nand_fetch(&v, init, __ATOMIC_ACQ_REL) != init)
    abort();

  if (__atomic_nand_fetch(&v, 0, __ATOMIC_SEQ_CST) != init)
    abort();
}

void test_xor_fetch() {
  v     = init;
  count = 0;

  if (__atomic_xor_fetch(&v, count, __ATOMIC_RELAXED) != init)
    abort();

  if (__atomic_xor_fetch(&v, ~count, __ATOMIC_CONSUME) != 0)
    abort();

  if (__atomic_xor_fetch(&v, 0, __ATOMIC_ACQUIRE) != 0)
    abort();

  if (__atomic_xor_fetch(&v, ~count, __ATOMIC_RELEASE) != init)
    abort();

  if (__atomic_xor_fetch(&v, 0, __ATOMIC_ACQ_REL) != init)
    abort();

  if (__atomic_xor_fetch(&v, ~count, __ATOMIC_SEQ_CST) != 0)
    abort();
}

void test_or_fetch() {
  v     = 0;
  count = 1;

  if (__atomic_or_fetch(&v, count, __ATOMIC_RELAXED) != 1)
    abort();

  count *= 2;
  if (__atomic_or_fetch(&v, 2, __ATOMIC_CONSUME) != 3)
    abort();

  count *= 2;
  if (__atomic_or_fetch(&v, count, __ATOMIC_ACQUIRE) != 7)
    abort();

  count *= 2;
  if (__atomic_or_fetch(&v, 8, __ATOMIC_RELEASE) != 15)
    abort();

  count *= 2;
  if (__atomic_or_fetch(&v, count, __ATOMIC_ACQ_REL) != 31)
    abort();

  count *= 2;
  if (__atomic_or_fetch(&v, count, __ATOMIC_SEQ_CST) != 63)
    abort();
}

/* Test the OP routines with a result which isn't used. Use both variations
   within each function.  */

void test_add() {
  v     = 0;
  count = 1;

  __atomic_add_fetch(&v, count, __ATOMIC_RELAXED);
  if (v != 1)
    abort();

  __atomic_fetch_add(&v, count, __ATOMIC_CONSUME);
  if (v != 2)
    abort();

  __atomic_add_fetch(&v, 1, __ATOMIC_ACQUIRE);
  if (v != 3)
    abort();

  __atomic_fetch_add(&v, 1, __ATOMIC_RELEASE);
  if (v != 4)
    abort();

  __atomic_add_fetch(&v, count, __ATOMIC_ACQ_REL);
  if (v != 5)
    abort();

  __atomic_fetch_add(&v, count, __ATOMIC_SEQ_CST);
  if (v != 6)
    abort();
}

void test_sub() {
  v = res = 20;
  count   = 0;

  __atomic_sub_fetch(&v, count + 1, __ATOMIC_RELAXED);
  if (v != --res)
    abort();

  __atomic_fetch_sub(&v, count + 1, __ATOMIC_CONSUME);
  if (v != --res)
    abort();

  __atomic_sub_fetch(&v, 1, __ATOMIC_ACQUIRE);
  if (v != --res)
    abort();

  __atomic_fetch_sub(&v, 1, __ATOMIC_RELEASE);
  if (v != --res)
    abort();

  __atomic_sub_fetch(&v, count + 1, __ATOMIC_ACQ_REL);
  if (v != --res)
    abort();

  __atomic_fetch_sub(&v, count + 1, __ATOMIC_SEQ_CST);
  if (v != --res)
    abort();
}

void test_and() {
  v = init;

  __atomic_and_fetch(&v, 0, __ATOMIC_RELAXED);
  if (v != 0)
    abort();

  v = init;
  __atomic_fetch_and(&v, init, __ATOMIC_CONSUME);
  if (v != init)
    abort();

  __atomic_and_fetch(&v, 0, __ATOMIC_ACQUIRE);
  if (v != 0)
    abort();

  v = ~v;
  __atomic_fetch_and(&v, init, __ATOMIC_RELEASE);
  if (v != init)
    abort();

  __atomic_and_fetch(&v, 0, __ATOMIC_ACQ_REL);
  if (v != 0)
    abort();

  v = ~v;
  __atomic_fetch_and(&v, 0, __ATOMIC_SEQ_CST);
  if (v != 0)
    abort();
}

void test_nand() {
  v = init;

  __atomic_fetch_nand(&v, 0, __ATOMIC_RELAXED);
  if (v != init)
    abort();

  __atomic_fetch_nand(&v, init, __ATOMIC_CONSUME);
  if (v != 0)
    abort();

  __atomic_nand_fetch(&v, 0, __ATOMIC_ACQUIRE);
  if (v != init)
    abort();

  __atomic_nand_fetch(&v, init, __ATOMIC_RELEASE);
  if (v != 0)
    abort();

  __atomic_fetch_nand(&v, init, __ATOMIC_ACQ_REL);
  if (v != init)
    abort();

  __atomic_nand_fetch(&v, 0, __ATOMIC_SEQ_CST);
  if (v != init)
    abort();
}

void test_xor() {
  v     = init;
  count = 0;

  __atomic_xor_fetch(&v, count, __ATOMIC_RELAXED);
  if (v != init)
    abort();

  __atomic_fetch_xor(&v, ~count, __ATOMIC_CONSUME);
  if (v != 0)
    abort();

  __atomic_xor_fetch(&v, 0, __ATOMIC_ACQUIRE);
  if (v != 0)
    abort();

  __atomic_fetch_xor(&v, ~count, __ATOMIC_RELEASE);
  if (v != init)
    abort();

  __atomic_fetch_xor(&v, 0, __ATOMIC_ACQ_REL);
  if (v != init)
    abort();

  __atomic_xor_fetch(&v, ~count, __ATOMIC_SEQ_CST);
  if (v != 0)
    abort();
}

void test_or() {
  v     = 0;
  count = 1;

  __atomic_or_fetch(&v, count, __ATOMIC_RELAXED);
  if (v != 1)
    abort();

  count *= 2;
  __atomic_fetch_or(&v, count, __ATOMIC_CONSUME);
  if (v != 3)
    abort();

  count *= 2;
  __atomic_or_fetch(&v, 4, __ATOMIC_ACQUIRE);
  if (v != 7)
    abort();

  count *= 2;
  __atomic_fetch_or(&v, 8, __ATOMIC_RELEASE);
  if (v != 15)
    abort();

  count *= 2;
  __atomic_or_fetch(&v, count, __ATOMIC_ACQ_REL);
  if (v != 31)
    abort();

  count *= 2;
  __atomic_fetch_or(&v, count, __ATOMIC_SEQ_CST);
  if (v != 63)
    abort();
}

int main() {
  test_fetch_add();
  test_fetch_sub();
  test_fetch_and();
  test_fetch_nand();
  test_fetch_xor();
  test_fetch_or();

  test_add_fetch();
  test_sub_fetch();
  test_and_fetch();
  test_nand_fetch();
  test_xor_fetch();
  test_or_fetch();

  test_add();
  test_sub();
  test_and();
  test_nand();
  test_xor();
  test_or();

  return 0;
}



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
// DEFAULT-NEXT:     global %1 v: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 res: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 init: i64 [storage=static] [const] = widen<i64, reason=assign>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @test_fetch_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %24: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%24), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %25: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%25), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %26: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%26), widen<i64, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %27: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%27), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %28: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%28), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %29: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%29), widen<i64, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_fetch_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%3, widen<i64, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %30: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %31: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %32: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%31), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%32));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%30), read<i64>(%31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %33: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %34: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %35: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%34), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%35));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%33), read<i64>(%34))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %36: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %37: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %38: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%37), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%38));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%36), read<i64>(%37))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %39: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %40: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %41: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%40), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%41));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%39), read<i64>(%40))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %42: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %43: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %44: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%43), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%44));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%42), read<i64>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %45: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %46: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %47: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%46), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%47));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%45), read<i64>(%46))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_fetch_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         let %48: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%48), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %49: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, read<i64>(%4)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%49), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %50: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%50), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64>(%1, not<i64>(read<i64>(%1)));
// DEFAULT-NEXT:         let %51: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, read<i64>(%4)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%51), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %52: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%52), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %53: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%53), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test_fetch_nand() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         let %54: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%54), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %55: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%55), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %56: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%56), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %57: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%57), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %58: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%58), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %59: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%59), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_fetch_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %60: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%60), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %61: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%61), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %62: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%62), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %63: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%63), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %64: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%64), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %65: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%65), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_fetch_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %66: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%66), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %67: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %68: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%67), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%68));
// DEFAULT-NEXT:         let %69: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%69), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %70: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %71: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%70), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%71));
// DEFAULT-NEXT:         let %72: i64 [synthetic] = update<i64, result=old, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%72), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %73: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %74: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%73), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%74));
// DEFAULT-NEXT:         let %75: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%75), widen<i64, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %76: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %77: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%76), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%77));
// DEFAULT-NEXT:         let %78: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%78), widen<i64, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %79: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %80: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%79), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%80));
// DEFAULT-NEXT:         let %81: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%81), widen<i64, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_add_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %82: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%82), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %83: i64 [synthetic] = update<i64, result=new, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%83), widen<i64, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %84: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%84), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %85: i64 [synthetic] = update<i64, result=new, atomic=release>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%85), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %86: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%86), widen<i64, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %87: i64 [synthetic] = update<i64, result=new, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%87), widen<i64, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_sub_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%3, widen<i64, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %88: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %89: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %90: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%89), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%90));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%88), read<i64>(%90))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %91: i64 [synthetic] = update<i64, result=new, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %92: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %93: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%92), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%93));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%91), read<i64>(%93))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %94: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %95: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %96: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%95), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%96));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%94), read<i64>(%96))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %97: i64 [synthetic] = update<i64, result=new, atomic=release>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %98: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %99: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%98), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%99));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%97), read<i64>(%99))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %100: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %101: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %102: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%101), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%102));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%100), read<i64>(%102))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %103: i64 [synthetic] = update<i64, result=new, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %104: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %105: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%104), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%105));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%103), read<i64>(%105))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test_and_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         let %106: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%106), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         let %107: i64 [synthetic] = update<i64, result=new, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, read<i64>(%4)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%107), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %108: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%108), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64>(%1, not<i64>(read<i64>(%1)));
// DEFAULT-NEXT:         let %109: i64 [synthetic] = update<i64, result=new, atomic=release>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, read<i64>(%4)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%109), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %110: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%110), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64>(%1, not<i64>(read<i64>(%1)));
// DEFAULT-NEXT:         let %111: i64 [synthetic] = update<i64, result=new, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%111), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_nand_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         let %112: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%112), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %113: i64 [synthetic] = update<i64, result=new, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%113), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %114: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%114), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %115: i64 [synthetic] = update<i64, result=new, atomic=release>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%115), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %116: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%116), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %117: i64 [synthetic] = update<i64, result=new, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%117), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_xor_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %118: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%118), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %119: i64 [synthetic] = update<i64, result=new, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%119), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %120: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%120), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %121: i64 [synthetic] = update<i64, result=new, atomic=release>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%121), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %122: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%122), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %123: i64 [synthetic] = update<i64, result=new, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%123), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_or_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %124: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%124), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %125: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %126: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%125), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%126));
// DEFAULT-NEXT:         let %127: i64 [synthetic] = update<i64, result=new, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%127), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %128: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %129: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%128), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%129));
// DEFAULT-NEXT:         let %130: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%130), widen<i64, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %131: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %132: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%131), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%132));
// DEFAULT-NEXT:         let %133: i64 [synthetic] = update<i64, result=new, atomic=release>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%133), widen<i64, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %134: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %135: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%134), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%135));
// DEFAULT-NEXT:         let %136: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%136), widen<i64, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %137: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %138: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%137), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%138));
// DEFAULT-NEXT:         let %139: i64 [synthetic] = update<i64, result=new, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%139), widen<i64, reason=usual_arith>(const<i32>(63)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %140: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %141: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %142: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %143: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %144: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(5)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %145: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), add<i64, overflow=wrap>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(6)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%3, widen<i64, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(20)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %146: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %147: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %148: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%147), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%148));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%148))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %149: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %150: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %151: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%150), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%151));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%151))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %152: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %153: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %154: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%153), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%154));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%154))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %155: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:         let %156: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %157: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%156), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%157));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%157))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %158: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %159: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %160: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%159), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%160));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%160))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %161: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), sub<i64, overflow=wrap>(old<i64>, add<i64, overflow=ub>(read<i64>(%2), widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         let %162: i64 [synthetic] = read<i64>(%3);
// DEFAULT-NEXT:         let %163: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%162), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%3, read<i64>(%163));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%163))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         let %164: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         let %165: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, read<i64>(%4)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %166: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64>(%1, not<i64>(read<i64>(%1)));
// DEFAULT-NEXT:         let %167: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, read<i64>(%4)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %168: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i64>(%1, not<i64>(read<i64>(%1)));
// DEFAULT-NEXT:         let %169: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_nand() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         let %170: i64 [synthetic] = update<i64, result=old, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %171: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %172: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %173: i64 [synthetic] = update<i64, result=new, atomic=release>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %174: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, read<i64>(%4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %175: i64 [synthetic] = update<i64, result=new, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), not<i64>(and<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, read<i64>(%4));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %176: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %177: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %178: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %179: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %180: i64 [synthetic] = update<i64, result=old, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), read<i64>(%4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %181: i64 [synthetic] = update<i64, result=new, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), xor<i64>(old<i64>, not<i64>(read<i64>(%2))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64>(%1, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%2, widen<i64, reason=assign>(const<i32>(1)));
// DEFAULT-NEXT:         let %182: i64 [synthetic] = update<i64, result=new, atomic=relaxed>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %183: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %184: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%183), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%184));
// DEFAULT-NEXT:         let %185: i64 [synthetic] = update<i64, result=old, atomic=consume>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(3)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %186: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %187: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%186), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%187));
// DEFAULT-NEXT:         let %188: i64 [synthetic] = update<i64, result=new, atomic=acquire>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(7)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %189: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %190: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%189), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%190));
// DEFAULT-NEXT:         let %191: i64 [synthetic] = update<i64, result=old, atomic=release>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %192: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %193: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%192), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%193));
// DEFAULT-NEXT:         let %194: i64 [synthetic] = update<i64, result=new, atomic=acq_rel>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %195: i64 [synthetic] = read<i64>(%2);
// DEFAULT-NEXT:         let %196: i64 [synthetic] = mul<i64, overflow=ub>(read<i64>(%195), widen<i64, reason=usual_arith>(const<i32>(2)));
// DEFAULT-NEXT:         write<i64>(%2, read<i64>(%196));
// DEFAULT-NEXT:         let %197: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(deref(addr_of<ptr<i64>>(%1)), or<i64>(old<i64>, read<i64>(%2)));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%1), widen<i64, reason=usual_arith>(const<i32>(63)))
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
