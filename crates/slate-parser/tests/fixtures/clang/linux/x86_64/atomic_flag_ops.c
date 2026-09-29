#include <stdatomic.h>
#include <stdio.h>

int main(void) {
  atomic_flag flag = ATOMIC_FLAG_INIT;

  int first = atomic_flag_test_and_set(&flag);
  atomic_flag_clear_explicit(&flag, memory_order_release);
  int second = atomic_flag_test_and_set_explicit(&flag, memory_order_acquire);
  int third  = atomic_flag_test_and_set_explicit(&flag, memory_order_relaxed);
  atomic_flag_clear(&flag);
  int fourth = atomic_flag_test_and_set(&flag);

  printf("%d %d %d %d\n", first, second, third, fourth);
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
// DEFAULT-NEXT:     type @type[[TYPE_atomic_bool:[0-9]+]] atomic_bool = bool;
// DEFAULT-NEXT:     type @type[[TYPE_atomic_flag:[0-9]+]] atomic_flag = struct {
// DEFAULT-NEXT:         field0 _Value: atomic bool;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_atomic_flag_2:[0-9]+]] atomic_flag = @type[[TYPE_atomic_flag]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([37, 100, 32, 37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_flag:[0-9]+]] flag: @type[[TYPE_atomic_flag]] [storage=automatic] = aggregate<@type[[TYPE_atomic_flag]], zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_first:[0-9]+]] first: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type[[TYPE_atomic_flag]]>>(%[[VALUE_flag]]))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_first]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE0]])));
// DEFAULT-NEXT:         write<bool, atomic=release>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type[[TYPE_atomic_flag]]>>(%[[VALUE_flag]]))))), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_second:[0-9]+]] second: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: bool [synthetic] = update<bool, result=old, atomic=acquire>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type[[TYPE_atomic_flag]]>>(%[[VALUE_flag]]))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_second]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE1]])));
// DEFAULT-NEXT:         let %[[VALUE_third:[0-9]+]] third: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic] = update<bool, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type[[TYPE_atomic_flag]]>>(%[[VALUE_flag]]))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_third]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE2]])));
// DEFAULT-NEXT:         write<bool, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type[[TYPE_atomic_flag]]>>(%[[VALUE_flag]]))))), ne<i32, reason=arg>(const<i32>(0), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_fourth:[0-9]+]] fourth: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: bool [synthetic] = update<bool, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type[[TYPE_atomic_flag]]>>(%[[VALUE_flag]]))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_fourth]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE3]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])), read<i32>(%[[VALUE_first]]), read<i32>(%[[VALUE_second]]), read<i32>(%[[VALUE_third]]), read<i32>(%[[VALUE_fourth]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
