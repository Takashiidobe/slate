#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {
  _Atomic(float) f = 1.5f;
  float old_f      = atomic_fetch_add_explicit(&f, 2.25f, memory_order_relaxed);
  float now_f      = atomic_load(&f);

  int            values[4] = {10, 20, 30, 40};
  _Atomic(int *) p         = values;
  int           *old_p = atomic_fetch_add_explicit(&p, 2, memory_order_acq_rel);
  int           *now_p = atomic_load(&p);
  int *old_x = atomic_exchange_explicit(&p, values + 1, memory_order_release);
  int *now_x = atomic_load(&p);

  printf("%.2f %.2f %td %td %d %td %td\n", old_f, now_f, old_p - values,
         now_p - values, *now_p, old_x - values, now_x - values);
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
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 30> [storage=static] = code_units<array<i8, 30>>([37, 46, 50, 102, 32, 37, 46, 50, 102, 32, 37, 116, 100, 32, 37, 116, 100, 32, 37, 100, 32, 37, 116, 100, 32, 37, 116, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: atomic f32 [storage=automatic] = const<f32>(1.5);
// DEFAULT-NEXT:         let %[[VALUE_old_f:[0-9]+]] old_f: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic f32>>(%[[VALUE_f]])), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(2.25)));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_old_f]], read<f32>(%[[VALUE0]]));
// DEFAULT-NEXT:         let %[[VALUE_now_f:[0-9]+]] now_f: f32 [storage=automatic] = read<f32, atomic=seq_cst>(deref(addr_of<ptr<atomic f32>>(%[[VALUE_f]])));
// DEFAULT-NEXT:         let %[[VALUE_values:[0-9]+]] values: array<i32, 4> [storage=automatic] [align=16] = aggregate<array<i32, 4>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(20), index2 = const<i32>(30), index3 = const<i32>(40));
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: atomic ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]]);
// DEFAULT-NEXT:         let %[[VALUE_old_p:[0-9]+]] old_p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic ptr<i32>>>(%[[VALUE_p]])), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=wrap>(old<ptr<i32>>, const<i32>(2)));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_old_p]], read<ptr<i32>>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE_now_p:[0-9]+]] now_p: ptr<i32> [storage=automatic] = read<ptr<i32>, atomic=seq_cst>(deref(addr_of<ptr<atomic ptr<i32>>>(%[[VALUE_p]])));
// DEFAULT-NEXT:         let %[[VALUE_old_x:[0-9]+]] old_x: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i32> [synthetic] = update<ptr<i32>, result=old, atomic=release>(deref(addr_of<ptr<atomic ptr<i32>>>(%[[VALUE_p]])), ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]]), const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_old_x]], read<ptr<i32>>(%[[VALUE2]]));
// DEFAULT-NEXT:         let %[[VALUE_now_x:[0-9]+]] now_x: ptr<i32> [storage=automatic] = read<ptr<i32>, atomic=seq_cst>(deref(addr_of<ptr<atomic ptr<i32>>>(%[[VALUE_p]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%[[VALUE_str]])), float_widen<f64, reason=vararg>(read<f32>(%[[VALUE_old_f]])), float_widen<f64, reason=vararg>(read<f32>(%[[VALUE_now_f]])), ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(%[[VALUE_old_p]]), array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]])), ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(%[[VALUE_now_p]]), array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]])), read<i32>(deref(read<ptr<i32>>(%[[VALUE_now_p]]))), ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(%[[VALUE_old_x]]), array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]])), ptr_diff<i64, element=i32, same_array=required, overflow=ub>(read<ptr<i32>>(%[[VALUE_now_x]]), array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_values]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
