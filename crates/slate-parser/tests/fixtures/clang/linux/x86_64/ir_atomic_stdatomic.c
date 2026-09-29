// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata
#include <stdatomic.h>

atomic_int counter;
_Atomic(long *) cursor;
atomic_flag gate = ATOMIC_FLAG_INIT;
#define BUMP(object) atomic_fetch_add(object, 1)

int f(memory_order order, int *expected) {
    atomic_init(&counter, 0);
    atomic_store_explicit(&counter, 1, memory_order_relaxed);
    int a = atomic_load_explicit(&counter, memory_order_consume);
    a += atomic_load_explicit(&counter, memory_order_acquire);
    atomic_store_explicit(&counter, 2, memory_order_release);
    a += atomic_exchange_explicit(&counter, 3, memory_order_acq_rel);
    a += atomic_fetch_add_explicit(&counter, 1, memory_order_seq_cst);
    a += atomic_fetch_sub_explicit(&counter, 1, memory_order_relaxed);
    a += atomic_fetch_and_explicit(&counter, 1, memory_order_relaxed);
    a += atomic_fetch_or_explicit(&counter, 1, memory_order_relaxed);
    a += atomic_fetch_xor_explicit(&counter, 1, memory_order_relaxed);
    a += atomic_compare_exchange_strong_explicit(&counter, expected, 4, memory_order_acq_rel, memory_order_acquire);
    a += atomic_compare_exchange_weak_explicit(&counter, expected, 4, memory_order_release, memory_order_relaxed);
    a += atomic_fetch_add(&counter, 1);
    atomic_fetch_add_explicit(&cursor, 2, memory_order_relaxed);
    a += atomic_flag_test_and_set_explicit(&gate, memory_order_acquire);
    atomic_flag_clear_explicit(&gate, memory_order_release);
    atomic_thread_fence(memory_order_seq_cst);
    atomic_signal_fence(memory_order_acquire);
    a += atomic_load_explicit(&counter, order);
    counter += 5;
    a += BUMP(&counter);
    a += atomic_is_lock_free(&counter);
    return a;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_memory_order:[0-9]+]] memory_order = enum : u32 {
// IR-NEXT:         %[[VALUE_memory_order_relaxed:[0-9]+]] memory_order_relaxed = const<i32>(0);
// IR-NEXT:         %[[VALUE_memory_order_consume:[0-9]+]] memory_order_consume = const<i32>(1);
// IR-NEXT:         %[[VALUE_memory_order_acquire:[0-9]+]] memory_order_acquire = const<i32>(2);
// IR-NEXT:         %[[VALUE_memory_order_release:[0-9]+]] memory_order_release = const<i32>(3);
// IR-NEXT:         %[[VALUE_memory_order_acq_rel:[0-9]+]] memory_order_acq_rel = const<i32>(4);
// IR-NEXT:         %[[VALUE_memory_order_seq_cst:[0-9]+]] memory_order_seq_cst = const<i32>(5);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_memory_order_2:[0-9]+]] memory_order = @type[[TYPE_memory_order]] [c="enum memory_order"];
// IR-NEXT:     type @type[[TYPE_atomic_bool:[0-9]+]] atomic_bool = bool [c="_Atomic(_Bool)"] [c_atomic="true"];
// IR-NEXT:     type @type[[TYPE_atomic_int:[0-9]+]] atomic_int = i32 [c="_Atomic(int)"] [c_atomic="true"];
// IR-NEXT:     type @type[[TYPE_atomic_flag:[0-9]+]] atomic_flag = struct {
// IR-NEXT:         field0 _Value: atomic bool;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_atomic_flag_2:[0-9]+]] atomic_flag = @type[[TYPE_atomic_flag]] [c="struct atomic_flag"];
// IR-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: atomic i32 [storage=static] [linkage=external] [c="atomic_int"] [c_canon="_Atomic(int)"] [typedef_chain="atomic_int"] [c_atomic="true"];
// IR-NEXT:     global %[[VALUE_cursor:[0-9]+]] cursor: atomic ptr<i64> [storage=static] [linkage=external] [c="_Atomic(long *)"] [c_atomic="true"];
// IR-NEXT:     global %[[VALUE_gate:[0-9]+]] gate: @type[[TYPE_atomic_flag]] [storage=static] = aggregate<@type[[TYPE_atomic_flag]], zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(0), const<i32>(0))) [linkage=external] [c="atomic_flag"] [c_canon="struct atomic_flag"] [typedef_chain="atomic_flag"];
// IR-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_order:[0-9]+]] order: @type[[TYPE_memory_order]] [c="memory_order"] [c_canon="enum memory_order"] [typedef_chain="memory_order"], %[[VALUE_expected:[0-9]+]] expected: ptr<i32> [c="int *"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(memory_order, int *)"] [c_canon="int(enum memory_order, int *)"] {
// IR-NEXT:         write<i32>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), const<i32>(0)) [c_builtin="__c11_atomic_init"] [c_macro="atomic_init"];
// IR-NEXT:         write<i32, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), const<i32>(1)) [c_builtin="__c11_atomic_store"] [c_macro="atomic_store_explicit"];
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = read<i32, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]]))) [c_builtin="__c11_atomic_load"] [c_macro="atomic_load_explicit"] [c="int"];
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), read<i32, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]]))) [c_builtin="__c11_atomic_load"] [c_macro="atomic_load_explicit"]);
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE1]]));
// IR-NEXT:         write<i32, atomic=release>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), const<i32>(2)) [c_builtin="__c11_atomic_store"] [c_macro="atomic_store_explicit"];
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), const<i32>(3)) [c_builtin="__c11_atomic_exchange"] [c_macro="atomic_exchange_explicit"];
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), read<i32>(%[[VALUE3]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE4]]));
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_add"] [c_macro="atomic_fetch_add_explicit"];
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), read<i32>(%[[VALUE6]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE7]]));
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), sub<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_sub"] [c_macro="atomic_fetch_sub_explicit"];
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), read<i32>(%[[VALUE9]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE10]]));
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), and<i32>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_and"] [c_macro="atomic_fetch_and_explicit"];
// IR-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), read<i32>(%[[VALUE12]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE13]]));
// IR-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), or<i32>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_or"] [c_macro="atomic_fetch_or_explicit"];
// IR-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), read<i32>(%[[VALUE15]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE16]]));
// IR-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), xor<i32>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_xor"] [c_macro="atomic_fetch_xor_explicit"];
// IR-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE17]]), read<i32>(%[[VALUE18]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE19]]));
// IR-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE21:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=acq_rel, failure=acquire>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), read<ptr<i32>>(%[[VALUE_expected]]), const<i32>(4)) [c_builtin="__c11_atomic_compare_exchange_strong"] [c_macro="atomic_compare_exchange_strong_explicit"];
// IR-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE21]])));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE22]]));
// IR-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE24:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=release, failure=relaxed>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), read<ptr<i32>>(%[[VALUE_expected]]), const<i32>(4)) [c_builtin="__c11_atomic_compare_exchange_weak"] [c_macro="atomic_compare_exchange_weak_explicit"];
// IR-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE23]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE24]])));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE25]]));
// IR-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_add"] [c_macro="atomic_fetch_add"];
// IR-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), read<i32>(%[[VALUE27]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE28]]));
// IR-NEXT:         let %[[VALUE29:[0-9]+]]: ptr<i64> [synthetic] = update<ptr<i64>, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic ptr<i64>>>(%[[VALUE_cursor]])), ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=wrap>(old<ptr<i64>>, const<i32>(2))) [c_builtin="__c11_atomic_fetch_add"] [c_macro="atomic_fetch_add_explicit"];
// IR-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE31:[0-9]+]]: bool [synthetic] = update<bool, result=old, atomic=acquire>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type[[TYPE_atomic_flag]]>>(%[[VALUE_gate]]))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0))) [c_builtin="__c11_atomic_exchange"] [c_macro="atomic_flag_test_and_set_explicit"];
// IR-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE30]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE31]])));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE32]]));
// IR-NEXT:         write<bool, atomic=release>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type[[TYPE_atomic_flag]]>>(%[[VALUE_gate]]))))), ne<i32, reason=arg>(const<i32>(0), const<i32>(0))) [c_builtin="__c11_atomic_store"] [c_macro="atomic_flag_clear_explicit"];
// IR-NEXT:         fence<scope=thread, order=seq_cst> [c_builtin="__c11_atomic_thread_fence"] [c_macro="atomic_thread_fence"];
// IR-NEXT:         fence<scope=signal, order=acquire> [c_builtin="__c11_atomic_signal_fence"] [c_macro="atomic_signal_fence"];
// IR-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE33]]), read<i32, atomic=dynamic(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_memory_order]]>(%[[VALUE_order]])))>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]]))) [c_builtin="__c11_atomic_load"] [c_macro="atomic_load_explicit"]);
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE34]]));
// IR-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%[[VALUE_counter]], add<i32, overflow=ub>(old<i32>, const<i32>(5)));
// IR-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%[[VALUE_counter]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_add"] [c_macro="atomic_fetch_add"];
// IR-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), read<i32>(%[[VALUE37]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE38]]));
// IR-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE39]]), from_bool<i32, reason=promotion>(const<bool>(true) [c_builtin="__c11_atomic_is_lock_free"] [c_macro="atomic_is_lock_free"]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE40]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
