/* Test atomic_*_fence routines for existence and execution with each
   valid memory model.  Out-of-line function calls.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

int
main ()
{
  (atomic_thread_fence) (memory_order_relaxed);
  (atomic_thread_fence) (memory_order_consume);
  (atomic_thread_fence) (memory_order_acquire);
  (atomic_thread_fence) (memory_order_release);
  (atomic_thread_fence) (memory_order_acq_rel);
  (atomic_thread_fence) (memory_order_seq_cst);

  (atomic_signal_fence) (memory_order_relaxed);
  (atomic_signal_fence) (memory_order_consume);
  (atomic_signal_fence) (memory_order_acquire);
  (atomic_signal_fence) (memory_order_release);
  (atomic_signal_fence) (memory_order_acq_rel);
  (atomic_signal_fence) (memory_order_seq_cst);

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
// DEFAULT-NEXT:     fn %8 @atomic_thread_fence(%11 <unnamed>: @type0) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @atomic_signal_fence(%12 <unnamed>: @type0) -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%8, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%8, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%8, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%8, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%8, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%8, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%9, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%9, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%9, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%9, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%9, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(4))));
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void>(%9, int_to_enum<@type0, reason=arg>(reinterpret<u32, reason=arg, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
