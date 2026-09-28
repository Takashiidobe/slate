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
// DEFAULT-NEXT:     type @type0 memory_order = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     global %9 v: atomic i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %10 expected: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %11 max: i16 [storage=static] = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %12 desired: i16 [storage=static] = truncate<i16, reason=assign, fits=unknown>(not<i32>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %13 zero: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %15: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), read<i16>(%11));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %16: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), truncate<i16, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), widen<i32, reason=promotion>(read<i16>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %17: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=release, failure=acquire>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), truncate<i16, reason=arg, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%17))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), widen<i32, reason=promotion>(read<i16>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %18: bool [synthetic] = compare_exchange<i16, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), read<i16>(%12));
// DEFAULT-NEXT:         if read<bool>(%18)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %19: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), read<i16>(%12));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i16>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%9, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %20: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), read<i16>(%11));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%20))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %21: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), read<i16>(%13));
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), widen<i32, reason=promotion>(read<i16>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %22: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), read<i16>(%13));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%22))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), widen<i32, reason=promotion>(read<i16>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %23: bool [synthetic] = compare_exchange<i16, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), read<i16>(%12));
// DEFAULT-NEXT:         if read<bool>(%23)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %24: bool [synthetic] = compare_exchange<i16, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i16>>(%9)), addr_of<ptr<i16>>(%10), read<i16>(%12));
// DEFAULT-NEXT:         if not<bool>(read<bool>(%24))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%10)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i16>(%11)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
