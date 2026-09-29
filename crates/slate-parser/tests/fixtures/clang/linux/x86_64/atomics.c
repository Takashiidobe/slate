#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_int a = 0;

  atomic_store(&a, 100);
  int loaded = atomic_load(&a);

  int fa   = atomic_fetch_add(&a, 5);    // returns 100, a = 105
  int fs   = atomic_fetch_sub(&a, 10);   // returns 105, a = 95
  int fand = atomic_fetch_and(&a, 0x3C); // 95 & 60 = 28
  int forr = atomic_fetch_or(&a, 0x01);  // 28 | 1 = 29
  int fxor = atomic_fetch_xor(&a, 0x0F); // 29 ^ 15 = 18

  int xchg_old = atomic_exchange(&a, 7); // returns 18, a = 7

  int expected = 7;
  int ok = atomic_compare_exchange_strong(&a, &expected, 42); // success, a = 42
  int expected2 = 999;
  int bad =
      atomic_compare_exchange_strong(&a, &expected2, 0); // fail, a stays 42

  atomic_thread_fence(memory_order_seq_cst);

  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", loaded, fa, fs, fand, forr,
         fxor, xchg_old, ok, expected, bad, expected2, (int)a);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 37> [storage=static] = code_units<array<i8, 37>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: atomic i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         write<i32, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), const<i32>(100));
// DEFAULT-NEXT:         let %[[VALUE_loaded:[0-9]+]] loaded: i32 [storage=automatic] = read<i32, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])));
// DEFAULT-NEXT:         let %[[VALUE_fa:[0-9]+]] fa: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), add<i32, overflow=wrap>(old<i32>, const<i32>(5)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_fa]], read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE_fs:[0-9]+]] fs: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), sub<i32, overflow=wrap>(old<i32>, const<i32>(10)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_fs]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE_fand:[0-9]+]] fand: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), and<i32>(old<i32>, const<i32>(60)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_fand]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE_forr:[0-9]+]] forr: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), or<i32>(old<i32>, const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_forr]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE_fxor:[0-9]+]] fxor: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), xor<i32>(old<i32>, const<i32>(15)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_fxor]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         let %[[VALUE_xchg_old:[0-9]+]] xchg_old: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), const<i32>(7));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_xchg_old]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE_expected:[0-9]+]] expected: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         let %[[VALUE_ok:[0-9]+]] ok: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), const<i32>(42));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_ok]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE6]])));
// DEFAULT-NEXT:         let %[[VALUE_expected2:[0-9]+]] expected2: i32 [storage=automatic] = const<i32>(999);
// DEFAULT-NEXT:         let %[[VALUE_bad:[0-9]+]] bad: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_a]])), addr_of<ptr<i32>>(%[[VALUE_expected2]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_bad]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE7]])));
// DEFAULT-NEXT:         fence<scope=thread, order=seq_cst>;
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(37)>(%[[VALUE_str]])), read<i32>(%[[VALUE_loaded]]), read<i32>(%[[VALUE_fa]]), read<i32>(%[[VALUE_fs]]), read<i32>(%[[VALUE_fand]]), read<i32>(%[[VALUE_forr]]), read<i32>(%[[VALUE_fxor]]), read<i32>(%[[VALUE_xchg_old]]), read<i32>(%[[VALUE_ok]]), read<i32>(%[[VALUE_expected]]), read<i32>(%[[VALUE_bad]]), read<i32>(%[[VALUE_expected2]]), read<i32, atomic=seq_cst>(%[[VALUE_a]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
