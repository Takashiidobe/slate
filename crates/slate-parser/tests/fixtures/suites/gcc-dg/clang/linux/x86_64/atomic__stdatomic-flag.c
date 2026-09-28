/* Test atomic_flag routines for existence and execution.  */
/* The test needs a lockless atomic implementation.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);
atomic_flag a = ATOMIC_FLAG_INIT;

int
main ()
{
  int b;

  if (!atomic_is_lock_free (&a))
    abort ();

  if (atomic_flag_test_and_set (&a))
    abort ();
  atomic_flag_clear_explicit (&a, memory_order_relaxed);
  if (atomic_flag_test_and_set (&a))
    abort ();
  atomic_flag_clear (&a);

  b = atomic_flag_test_and_set_explicit (&a, memory_order_seq_cst);
  if (!atomic_flag_test_and_set (&a) || b != 0)
    abort ();

  b = atomic_flag_test_and_set_explicit (&a, memory_order_acq_rel);
  if (!atomic_flag_test_and_set (&a) || b != 1)
    abort ();

  atomic_flag_clear_explicit (&a, memory_order_seq_cst);
  if (atomic_flag_test_and_set (&a))
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
// DEFAULT-NEXT:     type @type2 atomic_bool = bool;
// DEFAULT-NEXT:     type @type3 atomic_flag = struct {
// DEFAULT-NEXT:         field0 _Value: atomic bool;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type4 atomic_flag = @type3;
// DEFAULT-NEXT:     global %12 a: @type3 [storage=static] = aggregate<@type3, zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(0), const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     fn %11 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %14 b: i32 [storage=automatic];
// DEFAULT-NEXT:         if not<bool>(const<bool>(true))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         let %15: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         write<bool, atomic=relaxed>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %16: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%16)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         write<bool, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %17: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%14, from_bool<i32, reason=assign>(read<bool>(%17)));
// DEFAULT-NEXT:         let %18: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(read<bool>(%18)), ne<i32>(read<i32>(%14), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         let %19: bool [synthetic] = update<bool, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%14, from_bool<i32, reason=assign>(read<bool>(%19)));
// DEFAULT-NEXT:         let %20: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if logical_or<bool>(not<bool>(read<bool>(%20)), ne<i32>(read<i32>(%14), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         write<bool, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %21: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type3>>(%12))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%21)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%11);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
