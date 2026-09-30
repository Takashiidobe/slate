/* Test atomic_compare_exchange routines for existence and proper
   execution on 2-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic short v = ATOMIC_VAR_INIT (0);
short expected = 0;
short max = ~0;
short desired = ~0;
short zero = 0;

int
main ()
{

  if (!atomic_compare_exchange_strong_explicit (&v, &expected, max, memory_order_relaxed, memory_order_relaxed))
    abort ();
  if (expected != 0)
    abort ();

  if (atomic_compare_exchange_strong_explicit (&v, &expected, 0, memory_order_acquire, memory_order_relaxed))
    abort ();
  if (expected != max)
    abort ();

  if (!atomic_compare_exchange_strong_explicit (&v, &expected, 0, memory_order_release, memory_order_acquire))
    abort ();
  if (expected != max)
    abort ();
  if (v != 0)
    abort ();

  if (atomic_compare_exchange_weak_explicit (&v, &expected, desired, memory_order_acq_rel, memory_order_acquire))
    abort ();
  if (expected != 0)
    abort ();

  if (!atomic_compare_exchange_strong_explicit (&v, &expected, desired, memory_order_seq_cst, memory_order_seq_cst))
    abort ();
  if (expected != 0)
    abort ();
  if (v != max)
    abort ();

  v = 0;

  if (!atomic_compare_exchange_strong (&v, &expected, max))
    abort ();
  if (expected != 0)
    abort ();

  if (atomic_compare_exchange_strong (&v, &expected, zero))
    abort ();
  if (expected != max)
    abort ();

  if (!atomic_compare_exchange_strong (&v, &expected, zero))
    abort ();
  if (expected != max)
    abort ();
  if (v != 0)
    abort ();

  if (atomic_compare_exchange_weak (&v, &expected, desired))
    abort ();
  if (expected != 0)
    abort ();

  if (!atomic_compare_exchange_strong (&v, &expected, desired))
    abort ();
  if (expected != 0)
    abort ();
  if (v != max)
    abort ();

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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: atomic i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_expected:[0-9]+]] expected: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_max:[0-9]+]] max: i16 [storage=static] = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_desired:[0-9]+]] desired: i16 [storage=static] = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zero:[0-9]+]] zero: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), read<i16>(%[[VALUE_max]]));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE0]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), truncate<i16, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE1]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=release, failure=acquire>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), truncate<i16, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), read<i16>(%[[VALUE_desired]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE3]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), read<i16>(%[[VALUE_desired]]));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE4]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), read<i16>(%[[VALUE_max]]));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE5]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), read<i16>(%[[VALUE_zero]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), read<i16>(%[[VALUE_zero]]));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE7]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), read<i16>(%[[VALUE_desired]]));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%[[VALUE_v]])), addr_of<ptr<i16>>(%[[VALUE_expected]]), read<i16>(%[[VALUE_desired]]));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE9]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_expected]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%[[VALUE_v]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_max]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
