/* Test atomic_compare_exchange routines for existence and proper
   execution on 2-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic int v = ATOMIC_VAR_INIT (0);
int expected = 0;
int max = ~0;
int desired = ~0;
int zero = 0;

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: atomic i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_expected:[0-9]+]] expected: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_max:[0-9]+]] max: i32 [storage=static] = not<i32>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_desired:[0-9]+]] desired: i32 [storage=static] = not<i32>(const<i32>(0)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_zero:[0-9]+]] zero: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_max]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=relaxed, failure=relaxed>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], read<bool>(%[[VALUE1]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE0]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_2:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_2:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=acquire, failure=relaxed>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_2]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_2]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], read<bool>(%[[VALUE3]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%[[VALUE2]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), read<i32>(%[[VALUE_max]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_3:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_3:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=release, failure=acquire>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_3]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_3]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], read<bool>(%[[VALUE5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE4]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), read<i32>(%[[VALUE_max]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_4:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_4:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_desired]]);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=acq_rel, failure=acquire>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_4]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_4]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], read<bool>(%[[VALUE7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_5:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_5:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_desired]]);
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_5]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_5]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], read<bool>(%[[VALUE9]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE8]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), read<i32>(%[[VALUE_max]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%[[VALUE_v]], const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_6:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_6:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_max]]);
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_6]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_6]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], read<bool>(%[[VALUE11]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE10]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_7:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_7:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_zero]]);
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_7]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_7]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], read<bool>(%[[VALUE13]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%[[VALUE12]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), read<i32>(%[[VALUE_max]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_8:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_8:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_zero]]);
// DEFAULT-NEXT:             let %[[VALUE15:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_8]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_8]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE14]], read<bool>(%[[VALUE15]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE14]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), read<i32>(%[[VALUE_max]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_9:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_9:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_desired]]);
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_9]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_9]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE16]], read<bool>(%[[VALUE17]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%[[VALUE16]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_10:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_10:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_desired]]);
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_10]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_10]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE18]], read<bool>(%[[VALUE19]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE18]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_expected]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), read<i32>(%[[VALUE_max]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
