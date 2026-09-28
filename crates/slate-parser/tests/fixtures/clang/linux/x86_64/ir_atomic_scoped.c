// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int shared;
int expected;
int desired;

int scopes(void) {
    int a = __scoped_atomic_load_n(&shared, __ATOMIC_ACQUIRE, __MEMORY_SCOPE_DEVICE);
    a += __scoped_atomic_fetch_add(&shared, 1, __ATOMIC_RELAXED, __MEMORY_SCOPE_WRKGRP);
    a += __scoped_atomic_sub_fetch(&shared, 1, __ATOMIC_SEQ_CST, __MEMORY_SCOPE_WVFRNT);
    a += __scoped_atomic_exchange_n(&shared, 2, __ATOMIC_SEQ_CST, __MEMORY_SCOPE_SINGLE);
    a += __scoped_atomic_fetch_or(&shared, 4, __ATOMIC_SEQ_CST, __MEMORY_SCOPE_CLUSTR);
    __scoped_atomic_store_n(&shared, a, __ATOMIC_RELEASE, __MEMORY_SCOPE_DEVICE);
    return a;
}

int system_scope(void) {
    return __scoped_atomic_fetch_add(&shared, 1, __ATOMIC_SEQ_CST, __MEMORY_SCOPE_SYSTEM);
}

int dynamic_scope(int scope, int order) {
    int a = __scoped_atomic_fetch_add(&shared, 1, __ATOMIC_SEQ_CST, scope);
    a += __scoped_atomic_fetch_add(&shared, 1, order, scope);
    return a;
}

int exchanges(void) {
    int a = __scoped_atomic_compare_exchange_n(&shared, &expected, 3, 0, __ATOMIC_SEQ_CST,
                                               __ATOMIC_RELAXED, __MEMORY_SCOPE_DEVICE);
    a += __scoped_atomic_compare_exchange(&shared, &expected, &desired, 1, __ATOMIC_ACQ_REL,
                                          __ATOMIC_ACQUIRE, __MEMORY_SCOPE_WRKGRP);
    return a;
}

void generic(void) {
    __scoped_atomic_load(&shared, &desired, __ATOMIC_ACQUIRE, __MEMORY_SCOPE_WVFRNT);
    __scoped_atomic_store(&shared, &desired, __ATOMIC_RELEASE, __MEMORY_SCOPE_WVFRNT);
    __scoped_atomic_exchange(&shared, &desired, &expected, __ATOMIC_SEQ_CST, __MEMORY_SCOPE_SINGLE);
    __scoped_atomic_thread_fence(__ATOMIC_SEQ_CST, __MEMORY_SCOPE_WRKGRP);
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
// IR-NEXT:     global %0 shared: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %1 expected: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %2 desired: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %3 @scopes() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %4 a: i32 [storage=automatic] = read<i32, atomic=acquire, sync_scope=device>(deref(addr_of<ptr<i32>>(%0)));
// IR-NEXT:         let %13: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %14: i32 [synthetic] = update<i32, result=old, atomic=relaxed, sync_scope=workgroup>(deref(addr_of<ptr<i32>>(%0)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), read<i32>(%14));
// IR-NEXT:         write<i32>(%4, read<i32>(%15));
// IR-NEXT:         let %16: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %17: i32 [synthetic] = update<i32, result=new, atomic=seq_cst, sync_scope=wavefront>(deref(addr_of<ptr<i32>>(%0)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), read<i32>(%17));
// IR-NEXT:         write<i32>(%4, read<i32>(%18));
// IR-NEXT:         let %19: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %20: i32 [synthetic] = update<i32, result=old, atomic=seq_cst, sync_scope=single>(deref(addr_of<ptr<i32>>(%0)), const<i32>(2));
// IR-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), read<i32>(%20));
// IR-NEXT:         write<i32>(%4, read<i32>(%21));
// IR-NEXT:         let %22: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %23: i32 [synthetic] = update<i32, result=old, atomic=seq_cst, sync_scope=cluster>(deref(addr_of<ptr<i32>>(%0)), or<i32>(old<i32>, const<i32>(4)));
// IR-NEXT:         let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), read<i32>(%23));
// IR-NEXT:         write<i32>(%4, read<i32>(%24));
// IR-NEXT:         write<i32, atomic=release, sync_scope=device>(deref(addr_of<ptr<i32>>(%0)), read<i32>(%4));
// IR-NEXT:         return read<i32>(%4);
// IR-NEXT:     }
// IR-NEXT:     fn %5 @system_scope() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %25: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         return read<i32>(%25);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @dynamic_scope(%7 scope: i32, %8 order: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %9 a: i32 [storage=automatic];
// IR-NEXT:         let %26: i32 [synthetic] = update<i32, result=old, atomic=seq_cst, sync_scope=dynamic(read<i32>(%7))>(deref(addr_of<ptr<i32>>(%0)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         write<i32>(%9, read<i32>(%26));
// IR-NEXT:         let %27: i32 [synthetic] = read<i32>(%9);
// IR-NEXT:         let %28: i32 [synthetic] = update<i32, result=old, atomic=dynamic(read<i32>(%8)), sync_scope=dynamic(read<i32>(%7))>(deref(addr_of<ptr<i32>>(%0)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), read<i32>(%28));
// IR-NEXT:         write<i32>(%9, read<i32>(%29));
// IR-NEXT:         return read<i32>(%9);
// IR-NEXT:     }
// IR-NEXT:     fn %10 @exchanges() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %11 a: i32 [storage=automatic];
// IR-NEXT:         let %30: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=relaxed, sync_scope=device>(deref(addr_of<ptr<i32>>(%0)), addr_of<ptr<i32>>(%1), const<i32>(3));
// IR-NEXT:         write<i32>(%11, from_bool<i32, reason=assign>(read<bool>(%30)));
// IR-NEXT:         let %31: i32 [synthetic] = read<i32>(%11);
// IR-NEXT:         let %32: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=acq_rel, failure=acquire, sync_scope=workgroup>(deref(addr_of<ptr<i32>>(%0)), addr_of<ptr<i32>>(%1), read<i32>(deref(addr_of<ptr<i32>>(%2))));
// IR-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), from_bool<i32, reason=promotion>(read<bool>(%32)));
// IR-NEXT:         write<i32>(%11, read<i32>(%33));
// IR-NEXT:         return read<i32>(%11);
// IR-NEXT:     }
// IR-NEXT:     fn %12 @generic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%2)), read<i32, atomic=acquire, sync_scope=wavefront>(deref(addr_of<ptr<i32>>(%0))));
// IR-NEXT:         write<i32, atomic=release, sync_scope=wavefront>(deref(addr_of<ptr<i32>>(%0)), read<i32>(deref(addr_of<ptr<i32>>(%2))));
// IR-NEXT:         let %34: i32 [synthetic] = update<i32, result=old, atomic=seq_cst, sync_scope=single>(deref(addr_of<ptr<i32>>(%0)), read<i32>(deref(addr_of<ptr<i32>>(%2))));
// IR-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%1)), read<i32>(%34));
// IR-NEXT:         fence<scope=thread, order=seq_cst, sync_scope=workgroup>;
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
