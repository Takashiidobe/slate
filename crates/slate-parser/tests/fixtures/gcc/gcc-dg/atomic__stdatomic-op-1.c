/* Test atomic_fetch routines for existence and proper execution on
   1-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic char v;
char count, res;
const char init = ~0;

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
// DEFAULT-NEXT:     global %9 v: atomic i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 count: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 res: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 init: i8 [storage=static] [const] = truncate<i8, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @test_fetch_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %24: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%24)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %25: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%25)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %26: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%26)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %27: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%27)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %28: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%28)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %29: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%29)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %30: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%30)), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @test_fetch_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%11, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %31: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)))));
// DEFAULT-NEXT:         let %32: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %33: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%32)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%33));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%31)), widen<i32, reason=promotion>(read<i8>(%32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %34: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %35: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %36: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%35)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%36));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%34)), widen<i32, reason=promotion>(read<i8>(%35)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %37: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)))));
// DEFAULT-NEXT:         let %38: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %39: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%38)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%39));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%37)), widen<i32, reason=promotion>(read<i8>(%38)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %40: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %41: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %42: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%41)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%42));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%40)), widen<i32, reason=promotion>(read<i8>(%41)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %43: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)))));
// DEFAULT-NEXT:         let %44: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %45: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%44)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%45));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%43)), widen<i32, reason=promotion>(read<i8>(%44)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %46: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %47: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %48: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%47)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%48));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%46)), widen<i32, reason=promotion>(read<i8>(%47)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %49: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %50: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %51: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%50)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%51));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%49)), widen<i32, reason=promotion>(read<i8>(%50)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @test_fetch_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, read<i8>(%12));
// DEFAULT-NEXT:         let %52: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%52)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %53: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, read<i8>(%12)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%53)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %54: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%54)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)))));
// DEFAULT-NEXT:         let %55: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, read<i8>(%12)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%55)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %56: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%56)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %57: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%57)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %58: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%58)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test_fetch_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, read<i8>(%12));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %59: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%59)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %60: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%10))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%60)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %61: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%61)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %62: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%10))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%62)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %63: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%63)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %64: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%10))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%64)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %65: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%10))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%65)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @test_fetch_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %66: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%66)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %67: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %68: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%67)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%68));
// DEFAULT-NEXT:         let %69: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%69)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %70: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %71: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%70)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%71));
// DEFAULT-NEXT:         let %72: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%72)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %73: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %74: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%73)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%74));
// DEFAULT-NEXT:         let %75: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%75)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %76: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %77: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%76)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%77));
// DEFAULT-NEXT:         let %78: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%78)), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %79: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %80: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%79)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%80));
// DEFAULT-NEXT:         let %81: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%81)), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %82: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %83: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%82)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%83));
// DEFAULT-NEXT:         let %84: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%84)), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @test_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %85: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %86: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %87: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %88: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %89: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %90: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), add<i8, overflow=wrap>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(%11, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %91: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)))));
// DEFAULT-NEXT:         let %92: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %93: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%92)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%93));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%93)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %94: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)))));
// DEFAULT-NEXT:         let %95: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %96: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%95)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%96));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%96)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %97: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %98: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %99: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%98)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%99));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%99)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %100: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %101: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %102: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%101)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%102));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%102)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %103: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)))));
// DEFAULT-NEXT:         let %104: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %105: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%104)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%105));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%105)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %106: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), sub<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)))));
// DEFAULT-NEXT:         let %107: i8 [synthetic] = read<i8>(%11);
// DEFAULT-NEXT:         let %108: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%107)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%108));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%108)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @test_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, read<i8>(%12));
// DEFAULT-NEXT:         let %109: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, read<i8>(%12));
// DEFAULT-NEXT:         let %110: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, read<i8>(%12)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %111: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)))));
// DEFAULT-NEXT:         let %112: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, read<i8>(%12)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %113: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)))));
// DEFAULT-NEXT:         let %114: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @test_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, read<i8>(%12));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %115: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %116: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%10))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %117: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %118: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%10))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %119: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%12)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %120: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), xor<i8>(old<i8>, truncate<i8, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i8>(%10))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %121: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %122: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %123: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%122)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%123));
// DEFAULT-NEXT:         let %124: i8 [synthetic] = update<i8, result=old, atomic=consume>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %125: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %126: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%125)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%126));
// DEFAULT-NEXT:         let %127: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %128: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %129: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%128)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%129));
// DEFAULT-NEXT:         let %130: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %131: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %132: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%131)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%132));
// DEFAULT-NEXT:         let %133: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %134: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %135: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%134)), const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%135));
// DEFAULT-NEXT:         let %136: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), or<i8>(old<i8>, read<i8>(%10)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), const<i32>(63))
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
