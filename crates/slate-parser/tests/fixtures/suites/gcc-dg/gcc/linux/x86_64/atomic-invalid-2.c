/* PR c/69104.  Test atomic routines for invalid memory model errors.  This
   only needs to be tested on a single size.  */
/* { dg-do compile } */
/* { dg-require-effective-target sync_int_long } */

#include <stdatomic.h>

/* atomic_store_explicit():
   The order argument shall not be memory_order_acquire,
   memory_order_consume, nor memory_order_acq_rel.  */

void
store (atomic_int *i)
{
  atomic_store_explicit (i, 0, memory_order_consume); /* { dg-warning "invalid memory model" } */
  atomic_store_explicit (i, 0, memory_order_acquire); /* { dg-warning "invalid memory model" } */
  atomic_store_explicit (i, 0, memory_order_acq_rel); /* { dg-warning "invalid memory model" } */
}

/* atomic_load_explicit():
   The order argument shall not be memory_order_release nor
   memory_order_acq_rel.  */

void
load (atomic_int *i)
{
  atomic_int j = atomic_load_explicit (i, memory_order_release); /* { dg-warning "invalid memory model" } */
  atomic_int k = atomic_load_explicit (i, memory_order_acq_rel); /* { dg-warning "invalid memory model" } */
}

/* atomic_compare_exchange():
   The failure argument shall not be memory_order_release nor
   memory_order_acq_rel.  The failure argument shall be no stronger than the
   success argument.  */

void
exchange (atomic_int *i)
{
  int r;

  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_seq_cst, memory_order_release); /* { dg-warning "invalid failure memory model 'memory_order_release'" } */
  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_seq_cst, memory_order_acq_rel); /* { dg-warning "invalid failure memory model 'memory_order_acq_rel'" } */
  atomic_compare_exchange_strong_explicit (i, &r, 0, memory_order_relaxed, memory_order_consume); /* { dg-warning "failure memory model 'memory_order_consume' cannot be stronger than success memory model 'memory_order_relaxed'" } */

  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_seq_cst, memory_order_release); /* { dg-warning "invalid failure memory model 'memory_order_release'" } */
  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_seq_cst, memory_order_acq_rel); /* { dg-warning "invalid failure memory model 'memory_order_acq_rel'" } */
  atomic_compare_exchange_weak_explicit (i, &r, 0, memory_order_relaxed, memory_order_consume); /* { dg-warning "failure memory model 'memory_order_consume' cannot be stronger than success memory model 'memory_order_relaxed'" } */
}

/* atomic_flag_clear():
   The order argument shall not be memory_order_acquire nor
   memory_order_acq_rel.  */

void
clear (atomic_int *i)
{
  atomic_flag_clear_explicit (i, memory_order_acquire); /* { dg-warning "invalid memory model" } */
  atomic_flag_clear_explicit (i, memory_order_acq_rel); /* { dg-warning "invalid memory model" } */
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type[[TYPE_atomic_int:[0-9]+]] atomic_int = i32;
// DEFAULT-NEXT:     fn %[[VALUE_store:[0-9]+]] @store(%[[VALUE_i:[0-9]+]] i: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr:[0-9]+]] __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             write<i32, atomic=consume>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_store_ptr]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_2:[0-9]+]] __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_2:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             write<i32, atomic=acquire>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_store_ptr_2]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_2]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_3:[0-9]+]] __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_3:[0-9]+]] __atomic_store_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             write<i32, atomic=acq_rel>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_store_ptr_3]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_store_tmp_3]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_load:[0-9]+]] @load(%[[VALUE_i_2:[0-9]+]] i: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: atomic i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr:[0-9]+]] __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp:[0-9]+]] __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_load_tmp]])), read<i32, atomic=release>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_load_ptr]]))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], read<i32>(%[[VALUE___atomic_load_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%[[VALUE_j]], read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: atomic i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_2:[0-9]+]] __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_2:[0-9]+]] __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_load_tmp_2]])), read<i32, atomic=acq_rel>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_load_ptr_2]]))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], read<i32>(%[[VALUE___atomic_load_tmp_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%[[VALUE_k]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_exchange:[0-9]+]] @exchange(%[[VALUE_i_3:[0-9]+]] i: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=release>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr]])), addr_of<ptr<i32>>(%[[VALUE_r]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], read<bool>(%[[VALUE3]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_2:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_2:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=acq_rel>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_2]])), addr_of<ptr<i32>>(%[[VALUE_r]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_2]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], read<bool>(%[[VALUE5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_3:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_3:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=relaxed, failure=consume>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_3]])), addr_of<ptr<i32>>(%[[VALUE_r]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_3]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], read<bool>(%[[VALUE7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_4:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_4:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=seq_cst, failure=release>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_4]])), addr_of<ptr<i32>>(%[[VALUE_r]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_4]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], read<bool>(%[[VALUE9]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_5:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_5:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=seq_cst, failure=acq_rel>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_5]])), addr_of<ptr<i32>>(%[[VALUE_r]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_5]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE10]], read<bool>(%[[VALUE11]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_6:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic i32> [storage=automatic] = read<ptr<atomic i32>>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_6:[0-9]+]] __atomic_compare_exchange_tmp: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=relaxed, failure=consume>(deref(read<ptr<atomic i32>>(%[[VALUE___atomic_compare_exchange_ptr_6]])), addr_of<ptr<i32>>(%[[VALUE_r]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE___atomic_compare_exchange_tmp_6]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE12]], read<bool>(%[[VALUE13]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_clear:[0-9]+]] @clear(%[[VALUE_i_4:[0-9]+]] i: ptr<atomic i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, atomic=acquire>(deref(read<ptr<atomic i32>>(%[[VALUE_i_4]])), const<i32>(0));
// DEFAULT-NEXT:         write<i32, atomic=acq_rel>(deref(read<ptr<atomic i32>>(%[[VALUE_i_4]])), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
