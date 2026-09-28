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
// DEFAULT-NEXT:     type @type0 memory_order = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     global %9 s: i32 [storage=static] = const<i32>(5) [linkage=external];
// DEFAULT-NEXT:     global %10 count: i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @func() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %29: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%30));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %20: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %21: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %13 vla: vla<vla<i32, %21>, %20> [storage=automatic];
// DEFAULT-NEXT:         let %22: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %14 p: atomic ptr<vla<i32, %22>> [storage=automatic] = pointer_cast<ptr<vla<i32, %22>>, reason=assign>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0)))));
// DEFAULT-NEXT:         let %23: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %15 b: ptr<vla<i32, %23>> [storage=automatic];
// DEFAULT-NEXT:         let %31: ptr<vla<i32, %22>> [synthetic] = update<ptr<vla<i32, %22>>, result=new, atomic=seq_cst>(%14, ptr_offset<ptr<vla<i32, %22>>, subtract=false, element=vla<i32, %22>, overflow=ub>(old<ptr<vla<i32, %22>>>, const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<vla<i32, %23>>>(%15, pointer_cast<ptr<vla<i32, %23>>, reason=assign>(read<ptr<vla<i32, %22>>>(%31)));
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<vla<i32, %23>>>(read<ptr<vla<i32, %23>>>(%15), pointer_cast<ptr<vla<i32, %23>>, reason=usual_arith>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(1)))))), ne<ptr<vla<i32, %22>>>(read<ptr<vla<i32, %22>>, atomic=seq_cst>(%14), pointer_cast<ptr<vla<i32, %22>>, reason=usual_arith>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(1)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %24: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %16 q: ptr<atomic ptr<vla<i32, %24>>> [storage=automatic] = pointer_cast<ptr<atomic ptr<vla<i32, %24>>>, reason=assign>(addr_of<ptr<atomic ptr<vla<i32, %22>>>>(%14));
// DEFAULT-NEXT:         write<ptr<vla<i32, %24>>, atomic=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))), pointer_cast<ptr<vla<i32, %24>>, reason=arg>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         write<ptr<vla<i32, %24>>, atomic=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))), pointer_cast<ptr<vla<i32, %24>>, reason=arg>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         read<ptr<vla<i32, %24>>, atomic=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         read<ptr<vla<i32, %24>>, atomic=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %32: ptr<vla<i32, %24>> [synthetic] = update<ptr<vla<i32, %24>>, result=old, atomic=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))), pointer_cast<ptr<vla<i32, %24>>, reason=arg>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %33: ptr<vla<i32, %24>> [synthetic] = update<ptr<vla<i32, %24>>, result=old, atomic=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))), pointer_cast<ptr<vla<i32, %24>>, reason=arg>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %25: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %26: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %17 vla2: vla<vla<i32, %26>, %25> [storage=automatic];
// DEFAULT-NEXT:         let %27: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %18 p2: ptr<vla<i32, %27>> [storage=automatic] = pointer_cast<ptr<vla<i32, %27>>, reason=assign>(addr_of<ptr<vla<i32, %26>>>(deref(ptr_offset<ptr<vla<i32, %26>>, subtract=false, element=vla<i32, %26>, overflow=ub>(array_decay<ptr<vla<i32, %26>>, length=None>(%17), const<i32>(0)))));
// DEFAULT-NEXT:         let %28: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%9)));
// DEFAULT-NEXT:         let %19 qna: ptr<ptr<vla<i32, %28>>> [storage=automatic] = pointer_cast<ptr<ptr<vla<i32, %28>>>, reason=assign>(addr_of<ptr<ptr<vla<i32, %27>>>>(%18));
// DEFAULT-NEXT:         let %34: bool [synthetic] = compare_exchange<ptr<vla<i32, %24>>, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))), read<ptr<ptr<vla<i32, %28>>>>(%19), pointer_cast<ptr<vla<i32, %24>>, reason=arg>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %35: bool [synthetic] = compare_exchange<ptr<vla<i32, %24>>, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))), read<ptr<ptr<vla<i32, %28>>>>(%19), pointer_cast<ptr<vla<i32, %24>>, reason=arg>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %36: bool [synthetic] = compare_exchange<ptr<vla<i32, %24>>, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))), read<ptr<ptr<vla<i32, %28>>>>(%19), pointer_cast<ptr<vla<i32, %24>>, reason=arg>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %37: bool [synthetic] = compare_exchange<ptr<vla<i32, %24>>, form=write_back, weak=true, success=seq_cst, failure=seq_cst>(deref(ptr_offset<ptr<atomic ptr<vla<i32, %24>>>, subtract=false, element=ptr<vla<i32, %24>>, overflow=ub>(read<ptr<atomic ptr<vla<i32, %24>>>>(%16), call<i32, signature=fn() -> i32>(%11))), read<ptr<ptr<vla<i32, %28>>>>(%19), pointer_cast<ptr<vla<i32, %24>>, reason=arg>(addr_of<ptr<vla<i32, %21>>>(deref(ptr_offset<ptr<vla<i32, %21>>, subtract=false, element=vla<i32, %21>, overflow=ub>(array_decay<ptr<vla<i32, %21>>, length=None>(%13), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%10), const<i32>(10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
