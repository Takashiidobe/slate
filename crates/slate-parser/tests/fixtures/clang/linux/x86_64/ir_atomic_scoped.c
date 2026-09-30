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
// IR-NEXT:     global %[[VALUE_shared:[0-9]+]] shared: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_expected:[0-9]+]] expected: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_desired:[0-9]+]] desired: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_scopes:[0-9]+]] @scopes() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = read<i32, atomic=acquire, sync_scope=device>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])));
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=relaxed, sync_scope=workgroup>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE1]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE2]]));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst, sync_scope=wavefront>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), read<i32>(%[[VALUE4]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE5]]));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst, sync_scope=single>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), const<i32>(2));
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), read<i32>(%[[VALUE7]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE8]]));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst, sync_scope=cluster>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), or<i32>(old<i32>, const<i32>(4)));
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), read<i32>(%[[VALUE10]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE11]]));
// IR-NEXT:         write<i32, atomic=release, sync_scope=device>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), read<i32>(%[[VALUE_a]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_system_scope:[0-9]+]] @system_scope() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         return read<i32>(%[[VALUE12]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_dynamic_scope:[0-9]+]] @dynamic_scope(%[[VALUE_scope:[0-9]+]] scope: i32, %[[VALUE_order:[0-9]+]] order: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst, sync_scope=dynamic(read<i32>(%[[VALUE_scope]]))>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE13]]));
// IR-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=dynamic(read<i32>(%[[VALUE_order]])), sync_scope=dynamic(read<i32>(%[[VALUE_scope]]))>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), read<i32>(%[[VALUE15]]));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE16]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_exchanges:[0-9]+]] @exchanges() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE17:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=false, success=seq_cst, failure=relaxed, sync_scope=device>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), const<i32>(3));
// IR-NEXT:         write<i32>(%[[VALUE_a_3]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE17]])));
// IR-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_3]]);
// IR-NEXT:         let %[[VALUE19:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=true, success=acq_rel, failure=acquire, sync_scope=workgroup>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), addr_of<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE_desired]]))));
// IR-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE19]])));
// IR-NEXT:         write<i32>(%[[VALUE_a_3]], read<i32>(%[[VALUE20]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_generic:[0-9]+]] @generic() -> void [linkage=external] [fallthrough=ret_void] {
// IR-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%[[VALUE_desired]])), read<i32, atomic=acquire, sync_scope=wavefront>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]]))));
// IR-NEXT:         write<i32, atomic=release, sync_scope=wavefront>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE_desired]]))));
// IR-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst, sync_scope=single>(deref(addr_of<ptr<i32>>(%[[VALUE_shared]])), read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE_desired]]))));
// IR-NEXT:         write<i32>(deref(addr_of<ptr<i32>>(%[[VALUE_expected]])), read<i32>(%[[VALUE21]]));
// IR-NEXT:         fence<scope=thread, order=seq_cst, sync_scope=workgroup>;
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
