/* Test atomic_store routines for existence and proper execution on
   4-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic int v;
int count;

int
main ()
{
  v = 0;
  count = 0;

  atomic_init (&v, count + 1);
  if (v != ++count)
    abort ();

  atomic_store_explicit (&v, count + 1, memory_order_relaxed);
  if (v != ++count)
    abort ();

  atomic_store_explicit (&v, count + 1, memory_order_release);
  if (v != ++count)
    abort ();

  atomic_store_explicit (&v, count + 1, memory_order_seq_cst);
  if (v != ++count)
    abort ();

  count++;

  atomic_store (&v, count);
  if (v != count)
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
// DEFAULT-NEXT:     global %[[VALUE_v:[0-9]+]] v: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%[[VALUE_v]], const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], const<i32>(0));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr:[0-9]+]] __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_count]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32, atomic=relaxed>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_store_ptr]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), read<i32>(%[[VALUE1]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_2:[0-9]+]] __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_2:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_count]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32, atomic=relaxed>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_store_ptr_2]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_2]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), read<i32>(%[[VALUE3]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_3:[0-9]+]] __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_3:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_count]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32, atomic=release>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_store_ptr_3]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_3]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), read<i32>(%[[VALUE5]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_4:[0-9]+]] __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_4:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_count]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_store_ptr_4]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_4]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), read<i32>(%[[VALUE7]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_5:[0-9]+]] __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%[[VALUE_v]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_5:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:             write<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_store_ptr_5]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_5]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%[[VALUE_v]]), read<i32>(%[[VALUE_count]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
