/* Test __atomic routines for existence and proper execution on 1 byte 
   values with each valid memory model.  */
/* { dg-do run } */
/* { dg-require-effective-target sync_char_short } */

/* Test the execution of the __atomic_*OP builtin routines for a char.  */

extern void abort(void);

char       v, count, res;
const char init = ~0;

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
// DEFAULT-NEXT:     global %1 v: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 count: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 res: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 init: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @test_fetch_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %24: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%24)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %25: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%25)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %26: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%26)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %27: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%27)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %28: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%28)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %29: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%29)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_fetch_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%3, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %30: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %31: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %32: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%31)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%32));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%30)), widen<i32, reason=promotion>(read<i8>(%31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %33: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %34: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %35: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%34)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%35));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%33)), widen<i32, reason=promotion>(read<i8>(%34)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %36: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %37: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %38: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%37)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%38));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%36)), widen<i32, reason=promotion>(read<i8>(%37)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %39: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %40: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %41: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%40)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%41));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%39)), widen<i32, reason=promotion>(read<i8>(%40)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %42: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %43: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %44: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%43)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%44));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%42)), widen<i32, reason=promotion>(read<i8>(%43)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %45: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %46: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %47: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%46)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%47));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%45)), widen<i32, reason=promotion>(read<i8>(%46)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_fetch_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         let %48: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%48)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %49: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, read<i8>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%49)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %50: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%1)))));
// DEFAULT-NEXT:         let %51: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, read<i8>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%51)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %52: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%52)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %53: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%53)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @test_fetch_nand() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         let %54: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%54)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %55: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%55)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %56: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%56)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %57: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%57)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %58: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%58)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %59: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%59)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @test_fetch_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %60: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%60)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %61: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%61)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %62: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%62)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %63: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%63)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %64: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%64)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %65: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%65)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test_fetch_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %66: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%66)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %67: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %68: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%67)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%68));
// DEFAULT-NEXT:         let %69: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%69)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %70: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %71: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%70)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%71));
// DEFAULT-NEXT:         let %72: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%72)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %73: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %74: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%73)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%74));
// DEFAULT-NEXT:         let %75: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%75)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %76: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %77: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%76)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%77));
// DEFAULT-NEXT:         let %78: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%78)), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %79: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %80: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%79)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%80));
// DEFAULT-NEXT:         let %81: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%81)), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @test_add_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %82: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%82)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %83: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%83)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %84: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%84)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %85: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%85)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %86: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%86)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %87: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%87)), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @test_sub_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%3, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %88: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %89: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %90: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%89)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%90));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%88)), widen<i32, reason=promotion>(read<i8>(%90)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %91: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %92: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %93: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%92)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%93));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%91)), widen<i32, reason=promotion>(read<i8>(%93)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %94: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %95: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %96: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%95)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%96));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%94)), widen<i32, reason=promotion>(read<i8>(%96)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %97: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %98: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %99: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%98)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%99));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%97)), widen<i32, reason=promotion>(read<i8>(%99)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %100: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %101: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %102: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%101)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%102));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%100)), widen<i32, reason=promotion>(read<i8>(%102)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %103: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %104: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %105: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%104)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%105));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%103)), widen<i32, reason=promotion>(read<i8>(%105)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test_and_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         let %106: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%106)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         let %107: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, read<i8>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%107)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %108: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%108)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%1)))));
// DEFAULT-NEXT:         let %109: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, read<i8>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%109)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %110: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%110)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%1)))));
// DEFAULT-NEXT:         let %111: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%111)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_nand_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         let %112: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%112)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %113: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%113)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %114: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%114)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %115: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%115)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %116: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%116)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %117: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%117)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_xor_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %118: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%118)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %119: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%119)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %120: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%120)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %121: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%121)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %122: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%122)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %123: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%123)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_or_fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %124: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%124)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %125: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %126: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%125)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%126));
// DEFAULT-NEXT:         let %127: i8 [synthetic] = update<i8, result=new, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%127)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %128: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %129: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%128)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%129));
// DEFAULT-NEXT:         let %130: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%130)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %131: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %132: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%131)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%132));
// DEFAULT-NEXT:         let %133: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%133)), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %134: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %135: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%134)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%135));
// DEFAULT-NEXT:         let %136: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%136)), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %137: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %138: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%137)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%138));
// DEFAULT-NEXT:         let %139: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%139)), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test_add() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %140: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %141: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %142: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %143: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %144: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %145: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), add<i8, overflow=wrap>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_sub() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%3, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %146: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %147: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %148: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%147)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%148));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%148)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %149: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %150: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %151: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%150)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%151));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%151)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %152: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %153: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %154: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%153)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%154));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%154)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %155: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %156: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %157: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%156)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%157));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%157)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %158: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %159: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %160: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%159)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%160));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%160)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %161: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%2)), const<i32>(1)))));
// DEFAULT-NEXT:         let %162: i8 [synthetic] = read<i8>(%3);
// DEFAULT-NEXT:         let %163: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%162)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%3, read<i8>(%163));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%163)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_and() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         let %164: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         let %165: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, read<i8>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %166: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%1)))));
// DEFAULT-NEXT:         let %167: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, read<i8>(%4)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %168: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%1)))));
// DEFAULT-NEXT:         let %169: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_nand() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         let %170: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %171: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %172: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %173: i8 [synthetic] = update<i8, result=new, atomic=release>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %174: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, read<i8>(%4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %175: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_xor() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(%4));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %176: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %177: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %178: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %179: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %180: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), widen<i32, reason=promotion>(read<i8>(%4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %181: i8 [synthetic] = update<i8, result=new, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%2))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_or() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%1, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%2, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %182: i8 [synthetic] = update<i8, result=new, atomic=relaxed>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %183: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %184: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%183)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%184));
// DEFAULT-NEXT:         let %185: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %186: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %187: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%186)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%187));
// DEFAULT-NEXT:         let %188: i8 [synthetic] = update<i8, result=new, atomic=acquire>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %189: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %190: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%189)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%190));
// DEFAULT-NEXT:         let %191: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %192: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %193: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%192)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%193));
// DEFAULT-NEXT:         let %194: i8 [synthetic] = update<i8, result=new, atomic=acq_rel>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         let %195: i8 [synthetic] = read<i8>(%2);
// DEFAULT-NEXT:         let %196: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%195)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%2, read<i8>(%196));
// DEFAULT-NEXT:         let %197: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%1)), or<i8>(old<i8>, read<i8>(%2)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%1)), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
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
