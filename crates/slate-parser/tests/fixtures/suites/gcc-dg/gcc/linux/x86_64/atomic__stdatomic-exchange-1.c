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
// DEFAULT-NEXT:     global %11 ret: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %12 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i8, atomic=seq_cst>(%9, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%10, truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %31: i8 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %13 __atomic_exchange_ptr: ptr<atomic i8> [storage=automatic] = addr_of<ptr<atomic i8>>(%9);
// DEFAULT-NEXT:             let %14 __atomic_exchange_val: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)));
// DEFAULT-NEXT:             let %15 __atomic_exchange_tmp: i8 [storage=automatic];
// DEFAULT-NEXT:             let %32: i8 [synthetic] = update<i8, result=old, atomic=relaxed>(deref(read<ptr<atomic i8>>(%13)), read<i8>(deref(addr_of<ptr<i8>>(%14))));
// DEFAULT-NEXT:             write<i8>(deref(addr_of<ptr<i8>>(%15)), read<i8>(%32));
// DEFAULT-NEXT:             write<i8>(%31, read<i8>(%15));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%31)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %33: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %34: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%33)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%34));
// DEFAULT-NEXT:         let %35: i8 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %16 __atomic_exchange_ptr: ptr<atomic i8> [storage=automatic] = addr_of<ptr<atomic i8>>(%9);
// DEFAULT-NEXT:             let %17 __atomic_exchange_val: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)));
// DEFAULT-NEXT:             let %18 __atomic_exchange_tmp: i8 [storage=automatic];
// DEFAULT-NEXT:             let %36: i8 [synthetic] = update<i8, result=old, atomic=acquire>(deref(read<ptr<atomic i8>>(%16)), read<i8>(deref(addr_of<ptr<i8>>(%17))));
// DEFAULT-NEXT:             write<i8>(deref(addr_of<ptr<i8>>(%18)), read<i8>(%36));
// DEFAULT-NEXT:             write<i8>(%35, read<i8>(%18));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%35)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %37: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %38: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%37)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%38));
// DEFAULT-NEXT:         let %39: i8 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %19 __atomic_exchange_ptr: ptr<atomic i8> [storage=automatic] = addr_of<ptr<atomic i8>>(%9);
// DEFAULT-NEXT:             let %20 __atomic_exchange_val: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)));
// DEFAULT-NEXT:             let %21 __atomic_exchange_tmp: i8 [storage=automatic];
// DEFAULT-NEXT:             let %40: i8 [synthetic] = update<i8, result=old, atomic=release>(deref(read<ptr<atomic i8>>(%19)), read<i8>(deref(addr_of<ptr<i8>>(%20))));
// DEFAULT-NEXT:             write<i8>(deref(addr_of<ptr<i8>>(%21)), read<i8>(%40));
// DEFAULT-NEXT:             write<i8>(%39, read<i8>(%21));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%39)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %41: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %42: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%41)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%42));
// DEFAULT-NEXT:         let %43: i8 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %22 __atomic_exchange_ptr: ptr<atomic i8> [storage=automatic] = addr_of<ptr<atomic i8>>(%9);
// DEFAULT-NEXT:             let %23 __atomic_exchange_val: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)));
// DEFAULT-NEXT:             let %24 __atomic_exchange_tmp: i8 [storage=automatic];
// DEFAULT-NEXT:             let %44: i8 [synthetic] = update<i8, result=old, atomic=acq_rel>(deref(read<ptr<atomic i8>>(%22)), read<i8>(deref(addr_of<ptr<i8>>(%23))));
// DEFAULT-NEXT:             write<i8>(deref(addr_of<ptr<i8>>(%24)), read<i8>(%44));
// DEFAULT-NEXT:             write<i8>(%43, read<i8>(%24));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%43)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %45: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %46: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%45)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%46));
// DEFAULT-NEXT:         let %47: i8 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %25 __atomic_exchange_ptr: ptr<atomic i8> [storage=automatic] = addr_of<ptr<atomic i8>>(%9);
// DEFAULT-NEXT:             let %26 __atomic_exchange_val: i8 [storage=automatic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1)));
// DEFAULT-NEXT:             let %27 __atomic_exchange_tmp: i8 [storage=automatic];
// DEFAULT-NEXT:             let %48: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(read<ptr<atomic i8>>(%25)), read<i8>(deref(addr_of<ptr<i8>>(%26))));
// DEFAULT-NEXT:             write<i8>(deref(addr_of<ptr<i8>>(%27)), read<i8>(%48));
// DEFAULT-NEXT:             write<i8>(%47, read<i8>(%27));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(%47)), widen<i32, reason=promotion>(read<i8>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %49: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %50: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%49)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%50));
// DEFAULT-NEXT:         let %51: i8 [synthetic] = read<i8>(%10);
// DEFAULT-NEXT:         let %52: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%51)), const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(%10, read<i8>(%52));
// DEFAULT-NEXT:         let %53: i8 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %28 __atomic_exchange_ptr: ptr<atomic i8> [storage=automatic] = addr_of<ptr<atomic i8>>(%9);
// DEFAULT-NEXT:             let %29 __atomic_exchange_val: i8 [storage=automatic] = read<i8>(%10);
// DEFAULT-NEXT:             let %30 __atomic_exchange_tmp: i8 [storage=automatic];
// DEFAULT-NEXT:             let %54: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(read<ptr<atomic i8>>(%28)), read<i8>(deref(addr_of<ptr<i8>>(%29))));
// DEFAULT-NEXT:             write<i8>(deref(addr_of<ptr<i8>>(%30)), read<i8>(%54));
// DEFAULT-NEXT:             write<i8>(%53, read<i8>(%30));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i8>(%11, read<i8>(%53));
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i8>(%11)), sub<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%10)), const<i32>(1))), ne<i32>(widen<i32, reason=promotion>(read<i8, atomic=seq_cst>(%9)), widen<i32, reason=promotion>(read<i8>(%10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
