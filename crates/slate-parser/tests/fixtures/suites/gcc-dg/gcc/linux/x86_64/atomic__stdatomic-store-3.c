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
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %12 __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %13 __atomic_store_tmp: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:             write<i32, atomic=relaxed>(deref(read<ptr<atomic i32>>(%12)), read<i32>(deref(addr_of<ptr<i32>>(%13))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %22: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%23));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%23))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %14 __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %15 __atomic_store_tmp: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:             write<i32, atomic=relaxed>(deref(read<ptr<atomic i32>>(%14)), read<i32>(deref(addr_of<ptr<i32>>(%15))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %24: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%25));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%25))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %16 __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %17 __atomic_store_tmp: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:             write<i32, atomic=release>(deref(read<ptr<atomic i32>>(%16)), read<i32>(deref(addr_of<ptr<i32>>(%17))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %26: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%27));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%27))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %18 __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %19 __atomic_store_tmp: i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%10), const<i32>(1));
// DEFAULT-NEXT:             write<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%18)), read<i32>(deref(addr_of<ptr<i32>>(%19))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %28: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%29));
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%29))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %30: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%10, read<i32>(%31));
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %20 __atomic_store_ptr: ptr<atomic i32> [storage=automatic] = addr_of<ptr<atomic i32>>(%9);
// DEFAULT-NEXT:             let %21 __atomic_store_tmp: i32 [storage=automatic] = read<i32>(%10);
// DEFAULT-NEXT:             write<i32, atomic=seq_cst>(deref(read<ptr<atomic i32>>(%20)), read<i32>(deref(addr_of<ptr<i32>>(%21))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(read<i32, atomic=seq_cst>(%9), read<i32>(%10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
