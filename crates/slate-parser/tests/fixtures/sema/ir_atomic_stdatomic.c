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
// IR-NEXT:     type @type0 memory_order = enum : u32 {
// IR-NEXT:         %0 memory_order_relaxed = const<i32>(0);
// IR-NEXT:         %1 memory_order_consume = const<i32>(1);
// IR-NEXT:         %2 memory_order_acquire = const<i32>(2);
// IR-NEXT:         %3 memory_order_release = const<i32>(3);
// IR-NEXT:         %4 memory_order_acq_rel = const<i32>(4);
// IR-NEXT:         %5 memory_order_seq_cst = const<i32>(5);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type1 memory_order = @type0 [c="enum memory_order"];
// IR-NEXT:     type @type2 atomic_bool = bool [c="_Atomic(_Bool)"] [c_atomic="true"];
// IR-NEXT:     type @type3 atomic_int = i32 [c="_Atomic(int)"] [c_atomic="true"];
// IR-NEXT:     type @type4 atomic_flag = struct {
// IR-NEXT:         field0 _Value: atomic bool;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     type @type5 atomic_flag = @type4 [c="struct atomic_flag"];
// IR-NEXT:     global %12 counter: atomic i32 [storage=static] [linkage=external] [c="atomic_int"] [c_canon="_Atomic(int)"] [typedef_chain="atomic_int"] [c_atomic="true"];
// IR-NEXT:     global %13 cursor: atomic ptr<i64> [storage=static] [linkage=external] [c="_Atomic(long *)"] [c_atomic="true"];
// IR-NEXT:     global %14 gate: @type4 [storage=static] = aggregate<@type4, zero_fill=false>(field0 = ne<i32, reason=assign>(const<i32>(0), const<i32>(0))) [linkage=external] [c="atomic_flag"] [c_canon="struct atomic_flag"] [typedef_chain="atomic_flag"];
// IR-NEXT:     fn %15 @f(%16 order: @type0 [c="memory_order"] [c_canon="enum memory_order"] [typedef_chain="memory_order"], %17 expected: ptr<i32> [c="int *"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(memory_order, int *)"] [c_canon="int(enum memory_order, int *)"] {
// IR-NEXT:         write<i32>(deref(addr_of<ptr<atomic i32>>(%12)), const<i32>(0)) [c_builtin="__c11_atomic_init"] [c_macro="atomic_init"];
// IR-NEXT:         write<i32, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%12)), const<i32>(1)) [c_builtin="__c11_atomic_store"] [c_macro="atomic_store_explicit"];
// IR-NEXT:         let %18 a: i32 [storage=automatic] = read<i32, atomic=consume>(deref(addr_of<ptr<atomic i32>>(%12))) [c_builtin="__c11_atomic_load"] [c_macro="atomic_load_explicit"] [c="int"];
// IR-NEXT:         let %19: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), read<i32, atomic=acquire>(deref(addr_of<ptr<atomic i32>>(%12))) [c_builtin="__c11_atomic_load"] [c_macro="atomic_load_explicit"]);
// IR-NEXT:         write<i32>(%18, read<i32>(%20));
// IR-NEXT:         write<i32, atomic=release>(deref(addr_of<ptr<atomic i32>>(%12)), const<i32>(2)) [c_builtin="__c11_atomic_store"] [c_macro="atomic_store_explicit"];
// IR-NEXT:         let %21: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %22: i32 [synthetic] = update<i32, result=old, atomic=acq_rel>(deref(addr_of<ptr<atomic i32>>(%12)), const<i32>(3)) [c_builtin="__c11_atomic_exchange"] [c_macro="atomic_exchange_explicit"];
// IR-NEXT:         let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), read<i32>(%22));
// IR-NEXT:         write<i32>(%18, read<i32>(%23));
// IR-NEXT:         let %24: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %25: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%12)), add<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_add"] [c_macro="atomic_fetch_add_explicit"];
// IR-NEXT:         let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), read<i32>(%25));
// IR-NEXT:         write<i32>(%18, read<i32>(%26));
// IR-NEXT:         let %27: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %28: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%12)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_sub"] [c_macro="atomic_fetch_sub_explicit"];
// IR-NEXT:         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), read<i32>(%28));
// IR-NEXT:         write<i32>(%18, read<i32>(%29));
// IR-NEXT:         let %30: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %31: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%12)), and<i32>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_and"] [c_macro="atomic_fetch_and_explicit"];
// IR-NEXT:         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), read<i32>(%31));
// IR-NEXT:         write<i32>(%18, read<i32>(%32));
// IR-NEXT:         let %33: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %34: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%12)), or<i32>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_or"] [c_macro="atomic_fetch_or_explicit"];
// IR-NEXT:         let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), read<i32>(%34));
// IR-NEXT:         write<i32>(%18, read<i32>(%35));
// IR-NEXT:         let %36: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %37: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic i32>>(%12)), xor<i32>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_xor"] [c_macro="atomic_fetch_xor_explicit"];
// IR-NEXT:         let %38: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), read<i32>(%37));
// IR-NEXT:         write<i32>(%18, read<i32>(%38));
// IR-NEXT:         let %39: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %40: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=acq_rel, failure=acquire>(deref(addr_of<ptr<atomic i32>>(%12)), read<ptr<i32>>(%17), const<i32>(4)) [c_builtin="__c11_atomic_compare_exchange_strong"] [c_macro="atomic_compare_exchange_strong_explicit"];
// IR-NEXT:         let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), from_bool<i32, reason=promotion>(read<bool>(%40)));
// IR-NEXT:         write<i32>(%18, read<i32>(%41));
// IR-NEXT:         let %42: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %43: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=release, failure=relaxed>(deref(addr_of<ptr<atomic i32>>(%12)), read<ptr<i32>>(%17), const<i32>(4)) [c_builtin="__c11_atomic_compare_exchange_weak"] [c_macro="atomic_compare_exchange_weak_explicit"];
// IR-NEXT:         let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), from_bool<i32, reason=promotion>(read<bool>(%43)));
// IR-NEXT:         write<i32>(%18, read<i32>(%44));
// IR-NEXT:         let %45: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %46: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%12)), add<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_add"] [c_macro="atomic_fetch_add"];
// IR-NEXT:         let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%45), read<i32>(%46));
// IR-NEXT:         write<i32>(%18, read<i32>(%47));
// IR-NEXT:         let %48: ptr<i64> [synthetic] = update<ptr<i64>, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic ptr<i64>>>(%13)), ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=wrap>(old<ptr<i64>>, const<i32>(2))) [c_builtin="__c11_atomic_fetch_add"] [c_macro="atomic_fetch_add_explicit"];
// IR-NEXT:         let %49: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %50: bool [synthetic] = update<bool, result=old, atomic=acquire>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type4>>(%14))))), ne<i32, reason=arg>(const<i32>(1), const<i32>(0))) [c_builtin="__c11_atomic_exchange"] [c_macro="atomic_flag_test_and_set_explicit"];
// IR-NEXT:         let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%49), from_bool<i32, reason=promotion>(read<bool>(%50)));
// IR-NEXT:         write<i32>(%18, read<i32>(%51));
// IR-NEXT:         write<bool, atomic=release>(deref(addr_of<ptr<atomic bool>>(field0(deref(addr_of<ptr<@type4>>(%14))))), ne<i32, reason=arg>(const<i32>(0), const<i32>(0))) [c_builtin="__c11_atomic_store"] [c_macro="atomic_flag_clear_explicit"];
// IR-NEXT:         fence<scope=thread, order=seq_cst> [c_builtin="__c11_atomic_thread_fence"] [c_macro="atomic_thread_fence"];
// IR-NEXT:         fence<scope=signal, order=acquire> [c_builtin="__c11_atomic_signal_fence"] [c_macro="atomic_signal_fence"];
// IR-NEXT:         let %52: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), read<i32, atomic=dynamic(enum_to_int<u32, reason=promotion>(read<@type0>(%16)))>(deref(addr_of<ptr<atomic i32>>(%12))) [c_builtin="__c11_atomic_load"] [c_macro="atomic_load_explicit"]);
// IR-NEXT:         write<i32>(%18, read<i32>(%53));
// IR-NEXT:         let %54: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(%12, add<i32, overflow=ub>(old<i32>, const<i32>(5)));
// IR-NEXT:         let %55: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %56: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic i32>>(%12)), add<i32, overflow=wrap>(old<i32>, const<i32>(1))) [c_builtin="__c11_atomic_fetch_add"] [c_macro="atomic_fetch_add"];
// IR-NEXT:         let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%55), read<i32>(%56));
// IR-NEXT:         write<i32>(%18, read<i32>(%57));
// IR-NEXT:         let %58: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %59: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), from_bool<i32, reason=promotion>(const<bool>(true) [c_builtin="__c11_atomic_is_lock_free"] [c_macro="atomic_is_lock_free"]));
// IR-NEXT:         write<i32>(%18, read<i32>(%59));
// IR-NEXT:         return read<i32>(%18);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
