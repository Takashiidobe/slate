/* Test atomic_compare_exchange routines for existence and proper
   execution on 2-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic long long v = ATOMIC_VAR_INIT (0);
long long expected = 0;
long long max = ~0LL;
long long desired = ~0LL;
long long zero = 0;

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
// DEFAULT-NEXT:     global %9 v: atomic i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %10 expected: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %11 max: i64 [storage=static] = not<i64>(const<i64>(0)) [linkage=external];
// DEFAULT-NEXT:     global %12 desired: i64 [storage=static] = not<i64>(const<i64>(0)) [linkage=external];
// DEFAULT-NEXT:     global %13 zero: i64 [storage=static] = widen<i64, reason=assign>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %14 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %35: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %15 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %16 __atomic_compare_exchange_tmp: i64 [storage=automatic] = read<i64>(%11);
// DEFAULT-NEXT:             let %36: bool [synthetic] = compare_exchange<i64, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(read<ptr<atomic i64>>(%15)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%16))));
// DEFAULT-NEXT:             write<bool>(%35, read<bool>(%36));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%35))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %37: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %17 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %18 __atomic_compare_exchange_tmp: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:             let %38: bool [synthetic] = compare_exchange<i64, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(read<ptr<atomic i64>>(%17)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%18))));
// DEFAULT-NEXT:             write<bool>(%37, read<bool>(%38));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%37)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), read<i64>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %19 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %20 __atomic_compare_exchange_tmp: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0));
// DEFAULT-NEXT:             let %40: bool [synthetic] = compare_exchange<i64, form=write_back, weak=false, success=release, failure=acquire>(deref(read<ptr<atomic i64>>(%19)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%20))));
// DEFAULT-NEXT:             write<bool>(%39, read<bool>(%40));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%39))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), read<i64>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=seq_cst>(%9), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %41: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %21 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %22 __atomic_compare_exchange_tmp: i64 [storage=automatic] = read<i64>(%12);
// DEFAULT-NEXT:             let %42: bool [synthetic] = compare_exchange<i64, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(read<ptr<atomic i64>>(%21)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%22))));
// DEFAULT-NEXT:             write<bool>(%41, read<bool>(%42));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%41)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %43: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %23 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %24 __atomic_compare_exchange_tmp: i64 [storage=automatic] = read<i64>(%12);
// DEFAULT-NEXT:             let %44: bool [synthetic] = compare_exchange<i64, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i64>>(%23)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%24))));
// DEFAULT-NEXT:             write<bool>(%43, read<bool>(%44));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%43))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=seq_cst>(%9), read<i64>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<i64, atomic=seq_cst>(%9, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %45: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %25 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %26 __atomic_compare_exchange_tmp: i64 [storage=automatic] = read<i64>(%11);
// DEFAULT-NEXT:             let %46: bool [synthetic] = compare_exchange<i64, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i64>>(%25)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%26))));
// DEFAULT-NEXT:             write<bool>(%45, read<bool>(%46));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%45))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %47: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %27 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %28 __atomic_compare_exchange_tmp: i64 [storage=automatic] = read<i64>(%13);
// DEFAULT-NEXT:             let %48: bool [synthetic] = compare_exchange<i64, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i64>>(%27)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%28))));
// DEFAULT-NEXT:             write<bool>(%47, read<bool>(%48));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%47)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), read<i64>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %49: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %29 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %30 __atomic_compare_exchange_tmp: i64 [storage=automatic] = read<i64>(%13);
// DEFAULT-NEXT:             let %50: bool [synthetic] = compare_exchange<i64, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i64>>(%29)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%30))));
// DEFAULT-NEXT:             write<bool>(%49, read<bool>(%50));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%49))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), read<i64>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=seq_cst>(%9), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %51: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %31 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %32 __atomic_compare_exchange_tmp: i64 [storage=automatic] = read<i64>(%12);
// DEFAULT-NEXT:             let %52: bool [synthetic] = compare_exchange<i64, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i64>>(%31)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%32))));
// DEFAULT-NEXT:             write<bool>(%51, read<bool>(%52));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%51)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %53: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %33 __atomic_compare_exchange_ptr: ptr<atomic i64> [storage=automatic] = addr_of<ptr<atomic i64>>(%9);
// DEFAULT-NEXT:             let %34 __atomic_compare_exchange_tmp: i64 [storage=automatic] = read<i64>(%12);
// DEFAULT-NEXT:             let %54: bool [synthetic] = compare_exchange<i64, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i64>>(%33)), addr_of<ptr<i64>>(%10), read<i64>(deref(addr_of<ptr<i64>>(%34))));
// DEFAULT-NEXT:             write<bool>(%53, read<bool>(%54));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%53))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=seq_cst>(%9), read<i64>(%11))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
