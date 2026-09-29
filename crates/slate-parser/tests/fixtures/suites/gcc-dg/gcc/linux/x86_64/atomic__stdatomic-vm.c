/* Test atomic operations on expressions of variably modified type
   with side effects.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

int s = 5;

int count = 0;

int
func (void)
{
  count++;
  return 0;
}

int
main (void)
{
  int vla[s][s];
  int (*_Atomic p)[s] = &vla[0];
  int (*b)[s] = kill_dependency (++p);
  if (b != &vla[1] || p != &vla[1])
    abort ();
  int (*_Atomic *q)[s] = &p;
  atomic_store_explicit (q + func (), &vla[0], memory_order_seq_cst);
  if (count != 1)
    abort ();
  atomic_store (q + func (), &vla[0]);
  if (count != 2)
    abort ();
  (void) atomic_load_explicit (q + func (), memory_order_seq_cst);
  if (count != 3)
    abort ();
  (void) atomic_load (q + func ());
  if (count != 4)
    abort ();
  (void) atomic_exchange_explicit (q + func (), &vla[0], memory_order_seq_cst);
  if (count != 5)
    abort ();
  (void) atomic_exchange (q + func (), &vla[0]);
  if (count != 6)
    abort ();
  int vla2[s][s];
  int (*p2)[s] = &vla2[0];
  int (**qna)[s] = &p2;
  (void) atomic_compare_exchange_strong_explicit (q + func (), qna, &vla[0],
						  memory_order_seq_cst,
						  memory_order_seq_cst);
  if (count != 7)
    abort ();
  (void) atomic_compare_exchange_strong (q + func (), qna, &vla[0]);
  if (count != 8)
    abort ();
  (void) atomic_compare_exchange_weak_explicit (q + func (), qna, &vla[0],
						memory_order_seq_cst,
						memory_order_seq_cst);
  if (count != 9)
    abort ();
  (void) atomic_compare_exchange_weak (q + func (), qna, &vla[0]);
  if (count != 10)
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
// DEFAULT-NEXT:     global %[[VALUE_s:[0-9]+]] s: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_func:[0-9]+]] @func() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE_vla:[0-9]+]] vla: vla<vla<i32, %[[VALUE3]]>, %[[VALUE2]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: atomic ptr<vla<i32, %[[VALUE4]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE4]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: ptr<vla<i32, %[[VALUE5]]>> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<vla<i32, %[[VALUE4]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___kill_dependency_tmp:[0-9]+]] __kill_dependency_tmp: ptr<vla<i32, %[[VALUE4]]>> [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: ptr<vla<i32, %[[VALUE4]]>> [synthetic] = update<ptr<vla<i32, %[[VALUE4]]>>, result=new, atomic=seq_cst>(%[[VALUE_p]], ptr_offset<ptr<vla<i32, %[[VALUE4]]>>, subtract=false, element=vla<i32, %[[VALUE4]]>, overflow=ub>(old<ptr<vla<i32, %[[VALUE4]]>>>, const<i32>(1)));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE4]]>>>(%[[VALUE___kill_dependency_tmp]], read<ptr<vla<i32, %[[VALUE4]]>>>(%[[VALUE7]]));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE4]]>>>(%[[VALUE6]], read<ptr<vla<i32, %[[VALUE4]]>>>(%[[VALUE___kill_dependency_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<ptr<vla<i32, %[[VALUE5]]>>>(%[[VALUE_b]], pointer_cast<ptr<vla<i32, %[[VALUE5]]>>, reason=assign>(read<ptr<vla<i32, %[[VALUE4]]>>>(%[[VALUE6]])));
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<vla<i32, %[[VALUE5]]>>>(read<ptr<vla<i32, %[[VALUE5]]>>>(%[[VALUE_b]]), pointer_cast<ptr<vla<i32, %[[VALUE5]]>>, reason=usual_arith>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(1)))))), ne<ptr<vla<i32, %[[VALUE4]]>>>(read<ptr<vla<i32, %[[VALUE4]]>>, atomic=seq_cst>(%[[VALUE_p]]), pointer_cast<ptr<vla<i32, %[[VALUE4]]>>, reason=usual_arith>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = pointer_cast<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, reason=assign>(addr_of<ptr<atomic ptr<vla<i32, %[[VALUE4]]>>>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr:[0-9]+]] __atomic_store_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_store_ptr]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp:[0-9]+]] __atomic_store_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_store_ptr]])), read<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_store_tmp]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr_2:[0-9]+]] __atomic_store_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_store_ptr_2]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp_2:[0-9]+]] __atomic_store_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_store_ptr_2]])), read<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_store_tmp_2]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr:[0-9]+]] __atomic_load_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE12:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_load_ptr]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp:[0-9]+]] __atomic_load_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_load_tmp]])), read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_load_ptr]]))));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE11]], read<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE___atomic_load_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr_2:[0-9]+]] __atomic_load_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE14:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_load_ptr_2]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp_2:[0-9]+]] __atomic_load_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic];
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_load_tmp_2]])), read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_load_ptr_2]]))));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE13]], read<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE___atomic_load_tmp_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_ptr:[0-9]+]] __atomic_exchange_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_ptr]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_val:[0-9]+]] __atomic_exchange_val: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_ptr]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_tmp:[0-9]+]] __atomic_exchange_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = update<ptr<vla<i32, %[[VALUE8]]>>, result=old, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_ptr]])), read<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_val]]))));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_tmp]])), read<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE18]]));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE15]], read<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE___atomic_exchange_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_ptr_2:[0-9]+]] __atomic_exchange_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_ptr_2]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_val_2:[0-9]+]] __atomic_exchange_val: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE21:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_ptr_2]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_tmp_2:[0-9]+]] __atomic_exchange_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = update<ptr<vla<i32, %[[VALUE8]]>>, result=old, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_ptr_2]])), read<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_val_2]]))));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_exchange_tmp_2]])), read<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE22]]));
// DEFAULT-NEXT:             write<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE19]], read<ptr<vla<i32, %[[VALUE8]]>>>(%[[VALUE___atomic_exchange_tmp_2]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE_vla2:[0-9]+]] vla2: vla<vla<i32, %[[VALUE24]]>, %[[VALUE23]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE_p2:[0-9]+]] p2: ptr<vla<i32, %[[VALUE25]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE25]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE24]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE24]]>>, subtract=false, element=vla<i32, %[[VALUE24]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE24]]>>, length=None>(%[[VALUE_vla2]]), const<i32>(0)))));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:         let %[[VALUE_qna:[0-9]+]] qna: ptr<ptr<vla<i32, %[[VALUE26]]>>> [storage=automatic] = pointer_cast<ptr<ptr<vla<i32, %[[VALUE26]]>>>, reason=assign>(addr_of<ptr<ptr<vla<i32, %[[VALUE25]]>>>>(%[[VALUE_p2]]));
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE28:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_ptr]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp:[0-9]+]] __atomic_compare_exchange_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:             let %[[VALUE29:[0-9]+]]: bool [synthetic] = compare_exchange<ptr<vla<i32, %[[VALUE8]]>>, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_ptr]])), read<ptr<ptr<vla<i32, %[[VALUE26]]>>>>(%[[VALUE_qna]]), read<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_tmp]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE27]], read<bool>(%[[VALUE29]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_2:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE31:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_ptr_2]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_2:[0-9]+]] __atomic_compare_exchange_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:             let %[[VALUE32:[0-9]+]]: bool [synthetic] = compare_exchange<ptr<vla<i32, %[[VALUE8]]>>, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_ptr_2]])), read<ptr<ptr<vla<i32, %[[VALUE26]]>>>>(%[[VALUE_qna]]), read<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_tmp_2]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE30]], read<bool>(%[[VALUE32]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_3:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE34:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_ptr_3]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_3:[0-9]+]] __atomic_compare_exchange_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:             let %[[VALUE35:[0-9]+]]: bool [synthetic] = compare_exchange<ptr<vla<i32, %[[VALUE8]]>>, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_ptr_3]])), read<ptr<ptr<vla<i32, %[[VALUE26]]>>>>(%[[VALUE_qna]]), read<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_tmp_3]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE33]], read<bool>(%[[VALUE35]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_4:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic ptr<vla<i32, %[[VALUE8]]>>> [storage=automatic] = ptr_offset<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>, subtract=false, element=ptr<vla<i32, %[[VALUE8]]>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE_q]]), call<i32, signature=fn() -> i32>(%[[VALUE_func]]));
// DEFAULT-NEXT:             const<i32>(0);
// DEFAULT-NEXT:             let %[[VALUE37:[0-9]+]]: ptr<vla<i32, %[[VALUE8]]>> [synthetic] = read<ptr<vla<i32, %[[VALUE8]]>>, atomic=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_ptr_4]])));
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_4:[0-9]+]] __atomic_compare_exchange_tmp: ptr<vla<i32, %[[VALUE8]]>> [storage=automatic] = pointer_cast<ptr<vla<i32, %[[VALUE8]]>>, reason=assign>(addr_of<ptr<vla<i32, %[[VALUE3]]>>>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_vla]]), const<i32>(0)))));
// DEFAULT-NEXT:             let %[[VALUE38:[0-9]+]]: bool [synthetic] = compare_exchange<ptr<vla<i32, %[[VALUE8]]>>, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(read<ptr<atomic ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_ptr_4]])), read<ptr<ptr<vla<i32, %[[VALUE26]]>>>>(%[[VALUE_qna]]), read<ptr<vla<i32, %[[VALUE8]]>>>(deref(addr_of<ptr<ptr<vla<i32, %[[VALUE8]]>>>>(%[[VALUE___atomic_compare_exchange_tmp_4]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE36]], read<bool>(%[[VALUE38]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_count]]), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
