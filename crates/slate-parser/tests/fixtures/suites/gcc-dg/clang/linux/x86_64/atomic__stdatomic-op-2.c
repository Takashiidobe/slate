/* Test atomic_fetch routines for existence and proper execution on
   2-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic short v;
short count, res;
const short init = ~0;

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
// DEFAULT-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_memory_order_2:[0-9]+]] memory_order = @type[[TYPE_memory_order]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: atomic i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_res:[0-9]+]] res: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_init:[0-9]+]] init: i16 [storage=static] [const] = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_add:[0-9]+]] @test_fetch_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE0]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE2]])), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE3]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE4]])), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE5]])), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE6]])), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_sub:[0-9]+]] @test_fetch_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE8]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE9]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE7]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE8]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE11]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE12]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE10]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE11]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE14]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE15]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE13]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE14]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE17]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE18]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE16]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE17]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE20]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE21]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE19]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE20]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE23]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE24]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE22]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE23]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE26]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE27]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE25]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE26]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_and:[0-9]+]] @test_fetch_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], read<i16>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE28]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, read<i16>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE29]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE30]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])))));
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, read<i16>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE31]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE32]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE33]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE34]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_xor:[0-9]+]] @test_fetch_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], read<i16>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE35]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE36]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE37]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE38]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE39]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE40]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE41]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_fetch_or:[0-9]+]] @test_fetch_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE42]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE43]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE44]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE45]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE46]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE48]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE49]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE51]])), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE52]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE53]]));
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE54]])), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE55]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE56]]));
// DEFAULT-NEXT:         let %[[VALUE57:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE57]])), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE58:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE59:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE58]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE59]]));
// DEFAULT-NEXT:         let %[[VALUE60:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE60]])), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_add:[0-9]+]] @test_add(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE61:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE62:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE63:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE64:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE65:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE66:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), add<i16, overflow=wrap>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_sub:[0-9]+]] @test_sub(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE67:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE68:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE69:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE68]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE69]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE69]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE70:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE71:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE72:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE71]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE72]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE72]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE73:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE74:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE75:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE74]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE75]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE75]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE76:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE77:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE78:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE77]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE78]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE78]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE79:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE80:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE81:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE80]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE81]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE81]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE82:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), sub<i16, overflow=wrap>(old<i16>, truncate<i16, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])), const<i32>(1)))));
// DEFAULT-NEXT:         let %[[VALUE83:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_res]]);
// DEFAULT-NEXT:         let %[[VALUE84:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE83]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], read<i16>(%[[VALUE84]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE84]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_and:[0-9]+]] @test_and(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], read<i16>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE85:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], read<i16>(%[[VALUE_init]]));
// DEFAULT-NEXT:         let %[[VALUE86:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, read<i16>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE87:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])))));
// DEFAULT-NEXT:         let %[[VALUE88:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, read<i16>(%[[VALUE_init]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE89:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])))));
// DEFAULT-NEXT:         let %[[VALUE90:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), and<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_xor:[0-9]+]] @test_xor(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], read<i16>(%[[VALUE_init]]));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE91:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE92:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE93:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE94:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE95:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_init]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE96:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), xor<i16>(old<i16>, truncate<i16, reason=arg, fits=unknown>(not<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]]))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_or:[0-9]+]] @test_or(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE97:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE98:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE99:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE98]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE99]]));
// DEFAULT-NEXT:         let %[[VALUE100:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=consume>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE101:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE102:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE101]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE102]]));
// DEFAULT-NEXT:         let %[[VALUE103:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE104:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE105:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE104]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE105]]));
// DEFAULT-NEXT:         let %[[VALUE106:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=release>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, truncate<i16, reason=arg, fits=always>(const<i32>(8))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE107:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE108:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE107]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE108]]));
// DEFAULT-NEXT:         let %[[VALUE109:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE110:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE111:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE110]])), const<i32>(2)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE111]]));
// DEFAULT-NEXT:         let %[[VALUE112:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), or<i16>(old<i16>, read<i16>(%[[VALUE_count]])));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(63))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_add]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_and]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_fetch_or]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_add]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_sub]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_and]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_xor]]);
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%[[VALUE_test_or]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
