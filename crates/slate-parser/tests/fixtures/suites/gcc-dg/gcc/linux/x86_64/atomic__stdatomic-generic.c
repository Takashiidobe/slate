/* Test generic atomic routines for proper function calling.  */
/* { dg-do run } */
/* { dg-options "-std=c11 -pedantic-errors" } */

#include <stdatomic.h>

extern void abort (void);
extern int memcmp (const void *, const void *, __SIZE_TYPE__);

typedef struct test {
  int array[10];
} test_struct;

test_struct zero = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
test_struct ones = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
_Atomic test_struct a;
test_struct b;

int size = sizeof (test_struct);
/* Test for consistency on sizes 1, 2, 4, 8, 16 and 32.  */
int
main ()
{
  test_struct c;

  atomic_store_explicit (&a, zero, memory_order_relaxed);
  if (memcmp (&a, &zero, size))
    abort ();

  c = atomic_exchange_explicit (&a, ones, memory_order_seq_cst);
  if (memcmp (&c, &zero, size))
    abort ();
  if (memcmp (&a, &ones, size))
    abort ();

  b = atomic_load_explicit (&a, memory_order_relaxed);
  if (memcmp (&b, &ones, size))
    abort ();

  if (!atomic_compare_exchange_strong_explicit (&a, &b, zero, memory_order_seq_cst, memory_order_acquire))
    abort ();
  if (memcmp (&a, &zero, size))
    abort ();

  if (atomic_compare_exchange_weak_explicit (&a, &b, ones, memory_order_seq_cst, memory_order_acquire))
    abort ();
  if (memcmp (&b, &zero, size))
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
// DEFAULT-NEXT:     type @type2 test = struct {
// DEFAULT-NEXT:         field0 array: array<i32, 10>;
// DEFAULT-NEXT:     } [size=40, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type3 test_struct = @type2;
// DEFAULT-NEXT:     global %12 zero: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = aggregate<array<i32, 10>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0), index2 = const<i32>(0), index3 = const<i32>(0), index4 = const<i32>(0), index5 = const<i32>(0), index6 = const<i32>(0), index7 = const<i32>(0), index8 = const<i32>(0), index9 = const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %13 ones: @type2 [storage=static] = aggregate<@type2, zero_fill=false>(field0 = aggregate<array<i32, 10>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(1), index2 = const<i32>(1), index3 = const<i32>(1), index4 = const<i32>(1), index5 = const<i32>(1), index6 = const<i32>(1), index7 = const<i32>(1), index8 = const<i32>(1), index9 = const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %14 a: atomic @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 b: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(40))) [linkage=external];
// DEFAULT-NEXT:     fn %8 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @memcmp(%30 <unnamed>: ptr<const void>, %31 <unnamed>: ptr<const void>, %32 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %18 c: @type2 [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %19 __atomic_store_ptr: ptr<atomic @type2> [storage=automatic] = addr_of<ptr<atomic @type2>>(%14);
// DEFAULT-NEXT:             let %20 __atomic_store_tmp: @type2 [storage=automatic] = copy<@type2, reason=assign>(read<@type2>(%12));
// DEFAULT-NEXT:             write<@type2, atomic=relaxed>(deref(read<ptr<atomic @type2>>(%19)), read<@type2>(deref(addr_of<ptr<@type2>>(%20))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%9, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<atomic @type2>>(%14)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%12)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %33: @type2 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %21 __atomic_exchange_ptr: ptr<atomic @type2> [storage=automatic] = addr_of<ptr<atomic @type2>>(%14);
// DEFAULT-NEXT:             let %22 __atomic_exchange_val: @type2 [storage=automatic] = copy<@type2, reason=assign>(read<@type2>(%13));
// DEFAULT-NEXT:             let %23 __atomic_exchange_tmp: @type2 [storage=automatic];
// DEFAULT-NEXT:             let %34: @type2 [synthetic] = update<@type2, result=old, atomic=seq_cst>(deref(read<ptr<atomic @type2>>(%21)), read<@type2>(deref(addr_of<ptr<@type2>>(%22))));
// DEFAULT-NEXT:             write<@type2>(deref(addr_of<ptr<@type2>>(%23)), read<@type2>(%34));
// DEFAULT-NEXT:             write<@type2>(%33, read<@type2>(%23));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<@type2>(%18, copy<@type2, reason=assign>(read<@type2>(%33)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%9, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%18)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%12)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%9, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<atomic @type2>>(%14)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%13)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %35: @type2 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %24 __atomic_load_ptr: ptr<atomic @type2> [storage=automatic] = addr_of<ptr<atomic @type2>>(%14);
// DEFAULT-NEXT:             let %25 __atomic_load_tmp: @type2 [storage=automatic];
// DEFAULT-NEXT:             write<@type2>(deref(addr_of<ptr<@type2>>(%25)), read<@type2, atomic=relaxed>(deref(read<ptr<atomic @type2>>(%24))));
// DEFAULT-NEXT:             write<@type2>(%35, read<@type2>(%25));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<@type2>(%15, copy<@type2, reason=assign>(read<@type2>(%35)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%9, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%13)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %36: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %26 __atomic_compare_exchange_ptr: ptr<atomic @type2> [storage=automatic] = addr_of<ptr<atomic @type2>>(%14);
// DEFAULT-NEXT:             let %27 __atomic_compare_exchange_tmp: @type2 [storage=automatic] = copy<@type2, reason=assign>(read<@type2>(%12));
// DEFAULT-NEXT:             let %37: bool [synthetic] = compare_exchange<@type2, form=write_back, weak=false, success=seq_cst, failure=acquire>(deref(read<ptr<atomic @type2>>(%26)), addr_of<ptr<@type2>>(%15), read<@type2>(deref(addr_of<ptr<@type2>>(%27))));
// DEFAULT-NEXT:             write<bool>(%36, read<bool>(%37));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%36))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%9, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<atomic @type2>>(%14)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%12)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         let %38: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %28 __atomic_compare_exchange_ptr: ptr<atomic @type2> [storage=automatic] = addr_of<ptr<atomic @type2>>(%14);
// DEFAULT-NEXT:             let %29 __atomic_compare_exchange_tmp: @type2 [storage=automatic] = copy<@type2, reason=assign>(read<@type2>(%13));
// DEFAULT-NEXT:             let %39: bool [synthetic] = compare_exchange<@type2, form=write_back, weak=true, success=seq_cst, failure=acquire>(deref(read<ptr<atomic @type2>>(%28)), addr_of<ptr<@type2>>(%15), read<@type2>(deref(addr_of<ptr<@type2>>(%29))));
// DEFAULT-NEXT:             write<bool>(%38, read<bool>(%39));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%38)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%9, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%15)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type2>>(%12)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%16)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%8);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
