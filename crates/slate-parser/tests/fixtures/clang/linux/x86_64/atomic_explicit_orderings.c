#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_int a = 0;

  atomic_store_explicit(&a, 10, memory_order_relaxed);
  int relaxed_load = atomic_load_explicit(&a, memory_order_relaxed);

  atomic_store_explicit(&a, 20, memory_order_release);
  int acquire_load = atomic_load_explicit(&a, memory_order_acquire);

  atomic_store_explicit(&a, 30, memory_order_seq_cst);
  int consume_load = atomic_load_explicit(&a, memory_order_consume);

  int old_add  = atomic_fetch_add_explicit(&a, 2, memory_order_acq_rel);
  int old_or   = atomic_fetch_or_explicit(&a, 1, memory_order_relaxed);
  int old_xchg = atomic_exchange_explicit(&a, 5, memory_order_acquire);

  int expected = 5;
  int ok       = atomic_compare_exchange_strong_explicit(
      &a, &expected, 8, memory_order_acq_rel, memory_order_acquire);

  atomic_thread_fence(memory_order_release);
  atomic_thread_fence(memory_order_acquire);
  atomic_thread_fence(memory_order_relaxed);

  printf("%d %d %d %d %d %d %d %d %d\n", relaxed_load, acquire_load,
         consume_load, old_add, old_or, old_xchg, ok, expected, (int)a);
  return 0;
}



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
// DEFAULT-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// DEFAULT-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// DEFAULT-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_memory_order_2:[0-9]+]] memory_order = @type[[TYPE_memory_order]];
// DEFAULT-NEXT:     type @type[[TYPE_atomic_int:[0-9]+]] atomic_int = i32;
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 28> [storage=static] = code_units<array<i8, 28>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: atomic i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), const<i32>(10));
// DEFAULT-NEXT:         let %[[VALUE_relaxed_load:[0-9]+]] relaxed_load: i32 [storage=automatic] = read<i32, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<i32, atomic=release>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), const<i32>(20));
// DEFAULT-NEXT:         let %[[VALUE_acquire_load:[0-9]+]] acquire_load: i32 [storage=automatic] = read<i32, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), const<i32>(30));
// DEFAULT-NEXT:         let %[[VALUE_consume_load:[0-9]+]] consume_load: i32 [storage=automatic] = read<i32, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         let %[[VALUE_old_add:[0-9]+]] old_add: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), add<i32, overflow=wrap>(old<i32>, const<i32>(2)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_old_add]], read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE_old_or:[0-9]+]] old_or: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), or<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_old_or]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE_old_xchg:[0-9]+]] old_xchg: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), const<i32>(5));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_old_xchg]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE_expected:[0-9]+]] expected: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %[[VALUE_ok:[0-9]+]] ok: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=acq_rel, failure=acquire>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), const<i32>(8));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_ok]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE3]])));
// DEFAULT-NEXT:         fence<scope=thread, order=release>;
// DEFAULT-NEXT:         fence<scope=thread, order=acquire>;
// DEFAULT-NEXT:         fence<scope=thread, order=relaxed>;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(28)>(%[[VALUE_str]])), read<i32>(%[[VALUE_relaxed_load]]), read<i32>(%[[VALUE_acquire_load]]), read<i32>(%[[VALUE_consume_load]]), read<i32>(%[[VALUE_old_add]]), read<i32>(%[[VALUE_old_or]]), read<i32>(%[[VALUE_old_xchg]]), read<i32>(%[[VALUE_ok]]), read<i32>(%[[VALUE_expected]]), read<i32, atomic=seq_cst>(%[[VALUE_a]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
