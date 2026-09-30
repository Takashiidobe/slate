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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_test:[0-9]+]] test = struct {
// DEFAULT-NEXT:         field0 array: array<i32, 10>;
// DEFAULT-NEXT:     } [size=40, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_test_struct:[0-9]+]] test_struct = @type[[TYPE_test]];
// DEFAULT-NEXT:     global %[[VALUE_zero:[0-9]+]] zero: @type[[TYPE_test]] [storage=static] = aggregate<@type[[TYPE_test]], zero_fill=false>(field0 = aggregate<array<i32, 10>, zero_fill=false>(index0 = const<i32>(0), index1 = const<i32>(0), index2 = const<i32>(0), index3 = const<i32>(0), index4 = const<i32>(0), index5 = const<i32>(0), index6 = const<i32>(0), index7 = const<i32>(0), index8 = const<i32>(0), index9 = const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ones:[0-9]+]] ones: @type[[TYPE_test]] [storage=static] = aggregate<@type[[TYPE_test]], zero_fill=false>(field0 = aggregate<array<i32, 10>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(1), index2 = const<i32>(1), index3 = const<i32>(1), index4 = const<i32>(1), index5 = const<i32>(1), index6 = const<i32>(1), index7 = const<i32>(1), index8 = const<i32>(1), index9 = const<i32>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: atomic @type[[TYPE_test]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE_test]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_size:[0-9]+]] size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(40))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: @type[[TYPE_test]] [storage=automatic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_ptr:[0-9]+]] __atomic_store_ptr: ptr<atomic @type[[TYPE_test]]> [storage=automatic] = addr_of<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_store_tmp:[0-9]+]] __atomic_store_tmp: @type[[TYPE_test]] [storage=automatic] = copy<@type[[TYPE_test]], reason=assign>(read<@type[[TYPE_test]]>(%[[VALUE_zero]]));
// DEFAULT-NEXT:             write<@type[[TYPE_test]], atomic=relaxed>(deref(read<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE___atomic_store_ptr]])), read<@type[[TYPE_test]]>(deref(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE___atomic_store_tmp]]))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_zero]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_size]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: @type[[TYPE_test]] [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_ptr:[0-9]+]] __atomic_exchange_ptr: ptr<atomic @type[[TYPE_test]]> [storage=automatic] = addr_of<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_val:[0-9]+]] __atomic_exchange_val: @type[[TYPE_test]] [storage=automatic] = copy<@type[[TYPE_test]], reason=assign>(read<@type[[TYPE_test]]>(%[[VALUE_ones]]));
// DEFAULT-NEXT:             let %[[VALUE___atomic_exchange_tmp:[0-9]+]] __atomic_exchange_tmp: @type[[TYPE_test]] [storage=automatic];
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: @type[[TYPE_test]] [synthetic] = update<@type[[TYPE_test]], result=old, atomic=seq_cst>(deref(read<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE___atomic_exchange_ptr]])), read<@type[[TYPE_test]]>(deref(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE___atomic_exchange_val]]))));
// DEFAULT-NEXT:             write<@type[[TYPE_test]]>(deref(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE___atomic_exchange_tmp]])), read<@type[[TYPE_test]]>(%[[VALUE4]]));
// DEFAULT-NEXT:             write<@type[[TYPE_test]]>(%[[VALUE3]], read<@type[[TYPE_test]]>(%[[VALUE___atomic_exchange_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<@type[[TYPE_test]]>(%[[VALUE_c]], copy<@type[[TYPE_test]], reason=assign>(read<@type[[TYPE_test]]>(%[[VALUE3]])));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_c]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_zero]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_size]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_ones]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_size]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: @type[[TYPE_test]] [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_ptr:[0-9]+]] __atomic_load_ptr: ptr<atomic @type[[TYPE_test]]> [storage=automatic] = addr_of<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_load_tmp:[0-9]+]] __atomic_load_tmp: @type[[TYPE_test]] [storage=automatic];
// DEFAULT-NEXT:             write<@type[[TYPE_test]]>(deref(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE___atomic_load_tmp]])), read<@type[[TYPE_test]], atomic=relaxed>(deref(read<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE___atomic_load_ptr]]))));
// DEFAULT-NEXT:             write<@type[[TYPE_test]]>(%[[VALUE5]], read<@type[[TYPE_test]]>(%[[VALUE___atomic_load_tmp]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<@type[[TYPE_test]]>(%[[VALUE_b]], copy<@type[[TYPE_test]], reason=assign>(read<@type[[TYPE_test]]>(%[[VALUE5]])));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_b]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_ones]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_size]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic @type[[TYPE_test]]> [storage=automatic] = addr_of<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp:[0-9]+]] __atomic_compare_exchange_tmp: @type[[TYPE_test]] [storage=automatic] = copy<@type[[TYPE_test]], reason=assign>(read<@type[[TYPE_test]]>(%[[VALUE_zero]]));
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: bool [synthetic] = compare_exchange<@type[[TYPE_test]], form=write_back, weak=false, success=seq_cst, failure=acquire>(deref(read<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE___atomic_compare_exchange_ptr]])), addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_b]]), read<@type[[TYPE_test]]>(deref(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE___atomic_compare_exchange_tmp]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], read<bool>(%[[VALUE7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if not<bool>(read<bool>(%[[VALUE6]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE_a]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_zero]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_size]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_ptr_2:[0-9]+]] __atomic_compare_exchange_ptr: ptr<atomic @type[[TYPE_test]]> [storage=automatic] = addr_of<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE_a]]);
// DEFAULT-NEXT:             let %[[VALUE___atomic_compare_exchange_tmp_2:[0-9]+]] __atomic_compare_exchange_tmp: @type[[TYPE_test]] [storage=automatic] = copy<@type[[TYPE_test]], reason=assign>(read<@type[[TYPE_test]]>(%[[VALUE_ones]]));
// DEFAULT-NEXT:             let %[[VALUE9:[0-9]+]]: bool [synthetic] = compare_exchange<@type[[TYPE_test]], form=write_back, weak=true, success=seq_cst, failure=acquire>(deref(read<ptr<atomic @type[[TYPE_test]]>>(%[[VALUE___atomic_compare_exchange_ptr_2]])), addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_b]]), read<@type[[TYPE_test]]>(deref(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE___atomic_compare_exchange_tmp_2]]))));
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], read<bool>(%[[VALUE9]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_b]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type[[TYPE_test]]>>(%[[VALUE_zero]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_size]])))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
