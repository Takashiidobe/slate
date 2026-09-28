/* Test atomic_load routines for existence and proper execution on
   8-byte values with each valid memory model.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);

_Atomic long long v;
long long count;

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
// DEFAULT-NEXT:     type @type0 memory_order = enum : u32 {
// DEFAULT-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %1 memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %2 memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %3 memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 memory_order = @type0;
// DEFAULT-NEXT:     global %9 v: atomic i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 count: i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %11 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i64, atomic=seq_cst>(%9, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         write<i64>(%10, widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %12: i64 [synthetic] = read<i64>(%10);
// DEFAULT-NEXT:         let %13: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%12), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%10, read<i64>(%13));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=relaxed>(deref(addr_of<ptr<atomic i64>>(%9))), read<i64>(%12))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %14: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(%9, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %15: i64 [synthetic] = read<i64>(%10);
// DEFAULT-NEXT:         let %16: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%15), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%10, read<i64>(%16));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=acquire>(deref(addr_of<ptr<atomic i64>>(%9))), read<i64>(%15))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %17: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(%9, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %18: i64 [synthetic] = read<i64>(%10);
// DEFAULT-NEXT:         let %19: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%18), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%10, read<i64>(%19));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=consume>(deref(addr_of<ptr<atomic i64>>(%9))), read<i64>(%18))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %20: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(%9, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %21: i64 [synthetic] = read<i64>(%10);
// DEFAULT-NEXT:         let %22: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%21), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:         write<i64>(%10, read<i64>(%22));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=seq_cst>(deref(addr_of<ptr<atomic i64>>(%9))), read<i64>(%21))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             let %23: i64 [synthetic] = update<i64, result=old, atomic=seq_cst>(%9, add<i64, overflow=ub>(old<i64>, widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         if ne<i64>(read<i64, atomic=seq_cst>(deref(addr_of<ptr<atomic i64>>(%9))), read<i64>(%10))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
