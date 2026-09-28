/* Test atomic_exchange routines for existence and proper execution on
   1-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic char v;
char count, ret;

int
main ()
{
  v = 0;
  count = 0;

  if (atomic_exchange_explicit (&v, count + 1, memory_order_relaxed) != count)
    abort ();
  count++;

  if (atomic_exchange_explicit (&v, count + 1, memory_order_acquire) != count)
    abort ();
  count++;

  if (atomic_exchange_explicit (&v, count + 1, memory_order_release) != count)
    abort ();
  count++;

  if (atomic_exchange_explicit (&v, count + 1, memory_order_acq_rel) != count)
    abort ();
  count++;

  if (atomic_exchange_explicit (&v, count + 1, memory_order_seq_cst) != count)
    abort ();
  count++;

  count++;

  ret = atomic_exchange (&v, count);
  if (ret != count - 1 || v != count)
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
// DEFAULT-NEXT:     global %9 v: atomic i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 count: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 ret: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %13: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i8>>(%9)), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%13)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %14: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %15: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%14)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%15));
// DEFAULT-NEXT:         let %16: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i8>>(%9)), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%16)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %17: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %18: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%17)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%18));
// DEFAULT-NEXT:         let %19: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(addr_of<ptr<atomic i8>>(%9)), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%19)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %20: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %21: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%20)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%21));
// DEFAULT-NEXT:         let %22: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i8>>(%9)), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%22)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %23: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %24: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%23)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%24));
// DEFAULT-NEXT:         let %25: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), truncate<i8, reason=arg, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%25)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %26: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %27: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%26)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%27));
// DEFAULT-NEXT:         let %28: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %29: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%28)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%29));
// DEFAULT-NEXT:         let %30: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i8>>(%9)), read<i8>(%10));
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%30));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%11)), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
