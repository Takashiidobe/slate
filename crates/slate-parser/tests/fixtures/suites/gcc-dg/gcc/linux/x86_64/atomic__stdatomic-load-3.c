/* Test atomic_load routines for existence and proper execution on
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
// DEFAULT-NEXT:     type @type0 = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     global %9 v: atomic i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 count: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(%9, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:         let %22: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %12 __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %13 __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%13)), read<i32, atomic=relaxed>(deref(read<ptr<atomic i32>>(%12))));
// DEFAULT-NEXT:             write<i32>(%22, read<i32>(%13));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %23: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%24));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%22), read<i32>(%23))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %25: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%9, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %26: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %14 __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %15 __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%15)), read<i32, atomic=acquire>(deref(read<ptr<atomic i32>>(%14))));
// DEFAULT-NEXT:             write<i32>(%26, read<i32>(%15));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %27: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%28));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%26), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %29: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%9, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %30: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %16 __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %17 __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%17)), read<i32, atomic=consume>(deref(read<ptr<atomic i32>>(%16))));
// DEFAULT-NEXT:             write<i32>(%30, read<i32>(%17));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %31: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%32));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%30), read<i32>(%31))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %33: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%9, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %34: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %18 __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %19 __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%19)), read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%18))));
// DEFAULT-NEXT:             write<i32>(%34, read<i32>(%19));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %35: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%36));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%34), read<i32>(%35))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %37: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(%9, add<i32, overflow=ub>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         let %38: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %20 __atomic_load_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %21 __atomic_load_tmp: i32 [storage=automatic];
// DEFAULT-NEXT:             write<i32>(deref(addr_of<ptr<i32>>(%21)), read<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%20))));
// DEFAULT-NEXT:             write<i32>(%38, read<i32>(%21));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%38), read<i32>(%10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
