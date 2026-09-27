/* Test atomic_load routines for existence and proper execution on
   2-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic short v;
short count;

int
main ()
{
  v = 0;
  count = 0;

  if (atomic_load_explicit (&v, memory_order_relaxed) != count++)
    abort ();
  else
    v++;

  if (atomic_load_explicit (&v, memory_order_acquire) != count++)
    abort ();
  else
    v++;

  if (atomic_load_explicit (&v, memory_order_consume) != count++)
    abort ();
  else
    v++;

  if (atomic_load_explicit (&v, memory_order_seq_cst) != count++)
    abort ();
  else
    v++;

  if (atomic_load (&v) != count)
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
// DEFAULT-NEXT:     global %9 v: atomic i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 count: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%9, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%10, truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %22: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %12 __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%9);
// DEFAULT-NEXT:             let %13 __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%13)), read<i16, atomic=relaxed>(deref(read<ptr<atomic i16>>(%12))));
// DEFAULT-NEXT:             write<i16>(%22, read<i16>(%13));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %23: i16 [synthetic] = read<i16>(%10);
// DEFAULT-NEXT:         let %24: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%23)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%10, read<i16>(%24));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%22)), widen<i32, reason=promotion>(read<i16>(%23)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %25: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(%9, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:         let %26: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %14 __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%9);
// DEFAULT-NEXT:             let %15 __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%15)), read<i16, atomic=acquire>(deref(read<ptr<atomic i16>>(%14))));
// DEFAULT-NEXT:             write<i16>(%26, read<i16>(%15));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %27: i16 [synthetic] = read<i16>(%10);
// DEFAULT-NEXT:         let %28: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%27)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%10, read<i16>(%28));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%26)), widen<i32, reason=promotion>(read<i16>(%27)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %29: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(%9, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:         let %30: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %16 __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%9);
// DEFAULT-NEXT:             let %17 __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%17)), read<i16, atomic=consume>(deref(read<ptr<atomic i16>>(%16))));
// DEFAULT-NEXT:             write<i16>(%30, read<i16>(%17));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %31: i16 [synthetic] = read<i16>(%10);
// DEFAULT-NEXT:         let %32: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%31)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%10, read<i16>(%32));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%30)), widen<i32, reason=promotion>(read<i16>(%31)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %33: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(%9, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:         let %34: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %18 __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%9);
// DEFAULT-NEXT:             let %19 __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%19)), read<i16, atomic=seq_cst>(deref(read<ptr<atomic i16>>(%18))));
// DEFAULT-NEXT:             write<i16>(%34, read<i16>(%19));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %35: i16 [synthetic] = read<i16>(%10);
// DEFAULT-NEXT:         let %36: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%35)), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%10, read<i16>(%36));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%34)), widen<i32, reason=promotion>(read<i16>(%35)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %37: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(%9, truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:         let %38: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %20 __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%9);
// DEFAULT-NEXT:             let %21 __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%21)), read<i16, atomic=seq_cst>(deref(read<ptr<atomic i16>>(%20))));
// DEFAULT-NEXT:             write<i16>(%38, read<i16>(%21));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%38)), widen<i32, reason=promotion>(read<i16>(%10)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
