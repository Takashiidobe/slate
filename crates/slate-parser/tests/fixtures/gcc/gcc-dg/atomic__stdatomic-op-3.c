/* Test atomic_fetch routines for existence and proper execution on
   4-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic int v;
int count, res;
const int init = ~0;

void
test_fetch_add ()
{
  v = 0;
  count = 1;

  if (atomic_fetch_add_explicit (&v, count, memory_order_relaxed) != 0)
    abort ();

  if (atomic_fetch_add_explicit (&v, 1, memory_order_consume) != 1)
    abort ();

  if (atomic_fetch_add_explicit (&v, count, memory_order_acquire) != 2)
    abort ();

  if (atomic_fetch_add_explicit (&v, 1, memory_order_release) != 3)
    abort ();

  if (atomic_fetch_add_explicit (&v, count, memory_order_acq_rel) != 4)
    abort ();

  if (atomic_fetch_add_explicit (&v, 1, memory_order_seq_cst) != 5)
    abort ();

  if (atomic_fetch_add (&v, 1) != 6)
    abort ();
}

void
test_fetch_sub ()
{
  v = res = 20;
  count = 0;

  if (atomic_fetch_sub_explicit (&v, count + 1, memory_order_relaxed) != res--)
    abort ();

  if (atomic_fetch_sub_explicit (&v, 1, memory_order_consume) != res--)
    abort ();

  if (atomic_fetch_sub_explicit (&v, count + 1, memory_order_acquire) != res--)
    abort ();

  if (atomic_fetch_sub_explicit (&v, 1, memory_order_release) != res--)
    abort ();

  if (atomic_fetch_sub_explicit (&v, count + 1, memory_order_acq_rel) != res--)
    abort ();

  if (atomic_fetch_sub_explicit (&v, 1, memory_order_seq_cst) != res--)
    abort ();

  if (atomic_fetch_sub (&v, 1) != res--)
    abort ();
}

void
test_fetch_and ()
{
  v = init;

  if (atomic_fetch_and_explicit (&v, 0, memory_order_relaxed) != init)
    abort ();

  if (atomic_fetch_and_explicit (&v, init, memory_order_consume) != 0)
    abort ();

  if (atomic_fetch_and_explicit (&v, 0, memory_order_acquire) != 0)
    abort ();

  v = ~v;
  if (atomic_fetch_and_explicit (&v, init, memory_order_release) != init)
    abort ();

  if (atomic_fetch_and_explicit (&v, 0, memory_order_acq_rel) != init)
    abort ();

  if (atomic_fetch_and_explicit (&v, 0, memory_order_seq_cst) != 0)
    abort ();

  if (atomic_fetch_and (&v, 0) != 0)
    abort ();
}

void
test_fetch_xor ()
{
  v = init;
  count = 0;

  if (atomic_fetch_xor_explicit (&v, count, memory_order_relaxed) != init)
    abort ();

  if (atomic_fetch_xor_explicit (&v, ~count, memory_order_consume) != init)
    abort ();

  if (atomic_fetch_xor_explicit (&v, 0, memory_order_acquire) != 0)
    abort ();

  if (atomic_fetch_xor_explicit (&v, ~count, memory_order_release) != 0)
    abort ();

  if (atomic_fetch_xor_explicit (&v, 0, memory_order_acq_rel) != init)
    abort ();

  if (atomic_fetch_xor_explicit (&v, ~count, memory_order_seq_cst) != init)
    abort ();

  if (atomic_fetch_xor (&v, ~count) != 0)
    abort ();
}

void
test_fetch_or ()
{
  v = 0;
  count = 1;

  if (atomic_fetch_or_explicit (&v, count, memory_order_relaxed) != 0)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, 2, memory_order_consume) != 1)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, count, memory_order_acquire) != 3)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, 8, memory_order_release) != 7)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, count, memory_order_acq_rel) != 15)
    abort ();

  count *= 2;
  if (atomic_fetch_or_explicit (&v, count, memory_order_seq_cst) != 31)
    abort ();

  count *= 2;
  if (atomic_fetch_or (&v, count) != 63)
    abort ();
}


/* Test the OP routines with a result which isn't used.  */

void
test_add ()
{
  v = 0;
  count = 1;

  atomic_fetch_add (&v, count);
  if (v != 1)
    abort ();

  atomic_fetch_add_explicit (&v, count, memory_order_consume);
  if (v != 2)
    abort ();

  atomic_fetch_add (&v, 1);
  if (v != 3)
    abort ();

  atomic_fetch_add_explicit (&v, 1, memory_order_release);
  if (v != 4)
    abort ();

  atomic_fetch_add (&v, 1);
  if (v != 5)
    abort ();

  atomic_fetch_add_explicit (&v, count, memory_order_seq_cst);
  if (v != 6)
    abort ();
}

void
test_sub ()
{
  v = res = 20;
  count = 0;

  atomic_fetch_sub (&v, count + 1);
  if (v != --res)
    abort ();

  atomic_fetch_sub_explicit (&v, count + 1, memory_order_consume);
  if (v != --res)
    abort ();

  atomic_fetch_sub (&v, 1);
  if (v != --res)
    abort ();

  atomic_fetch_sub_explicit (&v, 1, memory_order_release);
  if (v != --res)
    abort ();

  atomic_fetch_sub (&v, count + 1);
  if (v != --res)
    abort ();

  atomic_fetch_sub_explicit (&v, count + 1, memory_order_seq_cst);
  if (v != --res)
    abort ();
}

void
test_and ()
{
  v = init;

  atomic_fetch_and (&v, 0);
  if (v != 0)
    abort ();

  v = init;
  atomic_fetch_and_explicit (&v, init, memory_order_consume);
  if (v != init)
    abort ();

  atomic_fetch_and (&v, 0);
  if (v != 0)
    abort ();

  v = ~v;
  atomic_fetch_and_explicit (&v, init, memory_order_release);
  if (v != init)
    abort ();

  atomic_fetch_and (&v, 0);
  if (v != 0)
    abort ();

  v = ~v;
  atomic_fetch_and_explicit (&v, 0, memory_order_seq_cst);
  if (v != 0)
    abort ();
}

void
test_xor ()
{
  v = init;
  count = 0;

  atomic_fetch_xor (&v, count);
  if (v != init)
    abort ();

  atomic_fetch_xor_explicit (&v, ~count, memory_order_consume);
  if (v != 0)
    abort ();

  atomic_fetch_xor (&v, 0);
  if (v != 0)
    abort ();

  atomic_fetch_xor_explicit (&v, ~count, memory_order_release);
  if (v != init)
    abort ();

  atomic_fetch_xor_explicit (&v, 0, memory_order_acq_rel);
  if (v != init)
    abort ();

  atomic_fetch_xor (&v, ~count);
  if (v != 0)
    abort ();
}

void
test_or ()
{
  v = 0;
  count = 1;

  atomic_fetch_or (&v, count);
  if (v != 1)
    abort ();

  count *= 2;
  atomic_fetch_or_explicit (&v, count, memory_order_consume);
  if (v != 3)
    abort ();

  count *= 2;
  atomic_fetch_or (&v, 4);
  if (v != 7)
    abort ();

  count *= 2;
  atomic_fetch_or_explicit (&v, 8, memory_order_release);
  if (v != 15)
    abort ();

  count *= 2;
  atomic_fetch_or (&v, count);
  if (v != 31)
    abort ();

  count *= 2;
  atomic_fetch_or_explicit (&v, count, memory_order_seq_cst);
  if (v != 63)
    abort ();
}

int
main ()
{
  test_fetch_add ();
  test_fetch_sub ();
  test_fetch_and ();
  test_fetch_xor ();
  test_fetch_or ();

  test_add ();
  test_sub ();
  test_and ();
  test_xor ();
  test_or ();

  return 0;
}

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     global %9 v: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 res: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 init: i32 [storage=static] [const] = not<i32>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @test_fetch_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(1));
// DEFAULT-NEXT:         let %24: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%24), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%25), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %26: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%26), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %27: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%27), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %28: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%28), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %29: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%29), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %30: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%30), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_fetch_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%11, const<i32>(20));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:         let %31: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:         let %32: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %33: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%32), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%33));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%31), read<i32>(%32))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %34: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %35: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %36: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%36));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%34), read<i32>(%35))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %37: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:         let %38: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %39: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%39));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%37), read<i32>(%38))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %40: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %41: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %42: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%41), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%42));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%40), read<i32>(%41))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %43: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:         let %44: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %45: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%45));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%43), read<i32>(%44))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %46: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %47: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %48: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%47), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%48));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%46), read<i32>(%47))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %49: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %50: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %51: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%51));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%49), read<i32>(%50))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_fetch_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, read<i32>(%12));
// DEFAULT-NEXT:         let %52: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%52), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %53: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, read<i32>(%12)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%53), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %54: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%54), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, not<i32>(read<i32, atomic=seq_cst>(%9)));
// DEFAULT-NEXT:         let %55: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, read<i32>(%12)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%55), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %56: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%56), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %57: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%57), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %58: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%58), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_fetch_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, read<i32>(%12));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:         let %59: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%59), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %60: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, not<i32>(read<i32>(%10))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%60), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %61: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%61), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %62: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, not<i32>(read<i32>(%10))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%62), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %63: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%63), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %64: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, not<i32>(read<i32>(%10))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%64), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %65: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, not<i32>(read<i32>(%10))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%65), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test_fetch_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(1));
// DEFAULT-NEXT:         let %66: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%66), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %67: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %68: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%67), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%68));
// DEFAULT-NEXT:         let %69: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%69), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %70: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %71: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%70), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%71));
// DEFAULT-NEXT:         let %72: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%72), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %73: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %74: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%73), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%74));
// DEFAULT-NEXT:         let %75: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, const<i32>(8)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%75), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %76: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %77: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%76), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%77));
// DEFAULT-NEXT:         let %78: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%78), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %79: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %80: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%79), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%80));
// DEFAULT-NEXT:         let %81: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%81), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %82: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %83: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%82), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%83));
// DEFAULT-NEXT:         let %84: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%84), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(1));
// DEFAULT-NEXT:         let %85: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %86: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %87: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %88: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %89: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %90: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), add<i32, overflow=wrap>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%11, const<i32>(20));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, const<i32>(20));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:         let %91: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:         let %92: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %93: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%92), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%93));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%93))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %94: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:         let %95: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %96: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%95), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%96));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%96))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %97: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %98: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %99: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%98), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%99));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%99))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %100: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %101: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %102: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%101), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%102));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%102))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %103: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:         let %104: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %105: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%104), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%105));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%105))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %106: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), sub<i32, overflow=wrap>(old<i32>, add<i32, overflow=ub>(read<i32>(%10), const<i32>(1))));
// DEFAULT-NEXT:         let %107: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %108: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%107), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%108));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%108))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, read<i32>(%12));
// DEFAULT-NEXT:         let %109: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, read<i32>(%12));
// DEFAULT-NEXT:         let %110: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, read<i32>(%12)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %111: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, not<i32>(read<i32, atomic=seq_cst>(%9)));
// DEFAULT-NEXT:         let %112: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, read<i32>(%12)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %113: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, not<i32>(read<i32, atomic=seq_cst>(%9)));
// DEFAULT-NEXT:         let %114: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), and<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, read<i32>(%12));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:         let %115: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %116: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, not<i32>(read<i32>(%10))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %117: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %118: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, not<i32>(read<i32>(%10))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %119: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %120: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), xor<i32>(old<i32>, not<i32>(read<i32>(%10))));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(1));
// DEFAULT-NEXT:         let %121: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %122: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %123: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%122), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%123));
// DEFAULT-NEXT:         let %124: i32 [synthetic] = update<i32, result=old, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %125: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %126: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%125), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%126));
// DEFAULT-NEXT:         let %127: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, const<i32>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %128: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %129: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%128), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%129));
// DEFAULT-NEXT:         let %130: i32 [synthetic] = update<i32, result=old, atomic=release>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, const<i32>(8)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %131: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %132: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%131), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%132));
// DEFAULT-NEXT:         let %133: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %134: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %135: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%134), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%135));
// DEFAULT-NEXT:         let %136: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%9)), or<i32>(old<i32>, read<i32>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
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
