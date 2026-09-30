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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: atomic i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i16, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr:[0-9]+]] __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp:[0-9]+]] __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_load_tmp]])), read<i16, atomic=relaxed>(deref(read<ptr<atomic i16>>(%[[VALUE___atomic_load_ptr]]))));
// DEFAULT-NEXT:             write<i16>(%[[VALUE0]], read<i16>(%[[VALUE___atomic_load_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE2]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE0]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE1]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_2:[0-9]+]] __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_2:[0-9]+]] __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_load_tmp_2]])), read<i16, atomic=acquire>(deref(read<ptr<atomic i16>>(%[[VALUE___atomic_load_ptr_2]]))));
// DEFAULT-NEXT:             write<i16>(%[[VALUE4]], read<i16>(%[[VALUE___atomic_load_tmp_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE5]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE6]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE4]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE5]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_3:[0-9]+]] __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_3:[0-9]+]] __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_load_tmp_3]])), read<i16, atomic=consume>(deref(read<ptr<atomic i16>>(%[[VALUE___atomic_load_ptr_3]]))));
// DEFAULT-NEXT:             write<i16>(%[[VALUE8]], read<i16>(%[[VALUE___atomic_load_tmp_3]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE9]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE10]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE8]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE9]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_4:[0-9]+]] __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_4:[0-9]+]] __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_load_tmp_4]])), read<i16, atomic=seq_cst>(deref(read<ptr<atomic i16>>(%[[VALUE___atomic_load_ptr_4]]))));
// DEFAULT-NEXT:             write<i16>(%[[VALUE12]], read<i16>(%[[VALUE___atomic_load_tmp_4]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE13]])), const<i32>(1)));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_count]], read<i16>(%[[VALUE14]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE12]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE13]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %[[VALUE15:[0-9]+]]: i16 [synthetic] = update<i16, result=old, atomic=seq_cst>(%[[VALUE_v]], truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(old<i16>), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i16 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_5:[0-9]+]] __atomic_load_ptr: ptr<atomic i16> [storage=automatic] = addr_of<ptr<atomic i16>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_5:[0-9]+]] __atomic_load_tmp: i16 [storage=automatic];
// DEFAULT-NEXT:             write<i16>(deref(addr_of<ptr<i16>>(%[[VALUE___atomic_load_tmp_5]])), read<i16, atomic=seq_cst>(deref(read<ptr<atomic i16>>(%[[VALUE___atomic_load_ptr_5]]))));
// DEFAULT-NEXT:             write<i16>(%[[VALUE16]], read<i16>(%[[VALUE___atomic_load_tmp_5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE16]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_count]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
