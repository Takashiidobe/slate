// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

int plain;
unsigned bits;

int sync(int *protected) {
    int a = __sync_fetch_and_add(&plain, 1);
    a += __sync_fetch_and_sub(&plain, 1L);
    a += __sync_fetch_and_or(&plain, 2);
    a += __sync_fetch_and_and(&plain, 2);
    a += __sync_fetch_and_xor(&plain, 2);
    a += __sync_fetch_and_nand(&plain, 2);
    a += __sync_add_and_fetch(&plain, 1);
    a += __sync_sub_and_fetch(&plain, 1);
    a += __sync_or_and_fetch(&plain, 2);
    a += __sync_and_and_fetch(&plain, 2);
    a += __sync_xor_and_fetch(&plain, 2);
    a += __sync_nand_and_fetch(&plain, 2);
    a += __sync_fetch_and_min(&bits, 3);
    a += __sync_fetch_and_max(&plain, 3);
    a += __sync_fetch_and_umin(&plain, 3);
    a += __sync_fetch_and_umax(&bits, 3);
    a += __sync_bool_compare_and_swap(&plain, 1, 2);
    a += __sync_val_compare_and_swap(&plain, 1, 2, protected);
    a += __sync_lock_test_and_set(&plain, 1);
    __sync_lock_release(&plain);
    a += __sync_swap(&plain, 4);
    __sync_synchronize();
    return a;
}

char narrow;

int sized(int *protected) {
    int a = __sync_fetch_and_add_4(&plain, 1);
    a += __sync_fetch_and_nand_1(&narrow, 2);
    a += __sync_fetch_and_add_8(&narrow, 1);
    a += __sync_or_and_fetch_16(&plain, 2);
    a += __sync_val_compare_and_swap_4(&plain, 1, 2, protected);
    a += __sync_bool_compare_and_swap_2(&plain, 1, 2);
    a += __sync_lock_test_and_set_1(&plain, 1);
    __sync_lock_release_8(&plain);
    a += __sync_swap_16(&plain, 4);
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
// IR-NEXT:     global %0 plain: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %1 bits: u32 [storage=static] [linkage=external];
// IR-NEXT:     global %5 narrow: i8 [storage=static] [linkage=external];
// IR-NEXT:     fn %2 @sync(%3 protected: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %4 a: i32 [storage=automatic];
// IR-NEXT:         let %9: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         write<i32>(%4, read<i32>(%9));
// IR-NEXT:         let %10: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %11: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), sub<i32, overflow=wrap>(old<i32>, truncate<i32, reason=arg, fits=always>(const<i64>(1))));
// IR-NEXT:         let %12: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%10), read<i32>(%11));
// IR-NEXT:         write<i32>(%4, read<i32>(%12));
// IR-NEXT:         let %13: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %14: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), or<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), read<i32>(%14));
// IR-NEXT:         write<i32>(%4, read<i32>(%15));
// IR-NEXT:         let %16: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %17: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), and<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %18: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), read<i32>(%17));
// IR-NEXT:         write<i32>(%4, read<i32>(%18));
// IR-NEXT:         let %19: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %20: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), xor<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), read<i32>(%20));
// IR-NEXT:         write<i32>(%4, read<i32>(%21));
// IR-NEXT:         let %22: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %23: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), not<i32>(and<i32>(old<i32>, const<i32>(2))));
// IR-NEXT:         let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), read<i32>(%23));
// IR-NEXT:         write<i32>(%4, read<i32>(%24));
// IR-NEXT:         let %25: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %26: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), read<i32>(%26));
// IR-NEXT:         write<i32>(%4, read<i32>(%27));
// IR-NEXT:         let %28: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %29: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), read<i32>(%29));
// IR-NEXT:         write<i32>(%4, read<i32>(%30));
// IR-NEXT:         let %31: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %32: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), or<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %33: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), read<i32>(%32));
// IR-NEXT:         write<i32>(%4, read<i32>(%33));
// IR-NEXT:         let %34: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %35: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), and<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %36: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), read<i32>(%35));
// IR-NEXT:         write<i32>(%4, read<i32>(%36));
// IR-NEXT:         let %37: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %38: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), xor<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%37), read<i32>(%38));
// IR-NEXT:         write<i32>(%4, read<i32>(%39));
// IR-NEXT:         let %40: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %41: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), not<i32>(and<i32>(old<i32>, const<i32>(2))));
// IR-NEXT:         let %42: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), read<i32>(%41));
// IR-NEXT:         write<i32>(%4, read<i32>(%42));
// IR-NEXT:         let %43: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %44: u32 [synthetic] = update<u32, result=old, atomic=seq_cst>(deref(addr_of<ptr<u32>>(%1)), conditional<u32>(lt<i32>(reinterpret<i32, reason=arg, fits=unknown>(old<u32>), reinterpret<i32, reason=arg, fits=unknown>(reinterpret<u32, reason=arg, fits=always>(const<i32>(3)))), old<u32>, reinterpret<u32, reason=arg, fits=always>(const<i32>(3))));
// IR-NEXT:         let %45: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%43)), read<u32>(%44)));
// IR-NEXT:         write<i32>(%4, read<i32>(%45));
// IR-NEXT:         let %46: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %47: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), conditional<i32>(gt<i32>(old<i32>, const<i32>(3)), old<i32>, const<i32>(3)));
// IR-NEXT:         let %48: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), read<i32>(%47));
// IR-NEXT:         write<i32>(%4, read<i32>(%48));
// IR-NEXT:         let %49: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %50: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), conditional<i32>(lt<u32>(reinterpret<u32, reason=arg, fits=unknown>(old<i32>), reinterpret<u32, reason=arg, fits=always>(const<i32>(3))), old<i32>, const<i32>(3)));
// IR-NEXT:         let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%49), read<i32>(%50));
// IR-NEXT:         write<i32>(%4, read<i32>(%51));
// IR-NEXT:         let %52: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %53: u32 [synthetic] = update<u32, result=old, atomic=seq_cst>(deref(addr_of<ptr<u32>>(%1)), conditional<u32>(gt<u32>(old<u32>, reinterpret<u32, reason=arg, fits=always>(const<i32>(3))), old<u32>, reinterpret<u32, reason=arg, fits=always>(const<i32>(3))));
// IR-NEXT:         let %54: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%52)), read<u32>(%53)));
// IR-NEXT:         write<i32>(%4, read<i32>(%54));
// IR-NEXT:         let %55: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %56: bool [synthetic] = compare_exchange<i32, form=success, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%0)), const<i32>(1), const<i32>(2));
// IR-NEXT:         let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%55), from_bool<i32, reason=promotion>(read<bool>(%56)));
// IR-NEXT:         write<i32>(%4, read<i32>(%57));
// IR-NEXT:         let %58: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %59: i32 [synthetic] = compare_exchange<i32, form=old, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%0)), const<i32>(1), const<i32>(2));
// IR-NEXT:         let %60: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%58), read<i32>(%59));
// IR-NEXT:         write<i32>(%4, read<i32>(%60));
// IR-NEXT:         let %61: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %62: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%0)), const<i32>(1));
// IR-NEXT:         let %63: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%61), read<i32>(%62));
// IR-NEXT:         write<i32>(%4, read<i32>(%63));
// IR-NEXT:         write<i32, atomic=release>(deref(addr_of<ptr<i32>>(%0)), const<i32>(0));
// IR-NEXT:         let %64: i32 [synthetic] = read<i32>(%4);
// IR-NEXT:         let %65: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), const<i32>(4));
// IR-NEXT:         let %66: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%64), read<i32>(%65));
// IR-NEXT:         write<i32>(%4, read<i32>(%66));
// IR-NEXT:         fence<scope=thread, order=seq_cst>;
// IR-NEXT:         return read<i32>(%4);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @sized(%7 protected: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %8 a: i32 [storage=automatic];
// IR-NEXT:         let %67: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         write<i32>(%8, read<i32>(%67));
// IR-NEXT:         let %68: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %69: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%5)), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(2)))));
// IR-NEXT:         let %70: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%68), widen<i32, reason=promotion>(read<i8>(%69)));
// IR-NEXT:         write<i32>(%8, read<i32>(%70));
// IR-NEXT:         let %71: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %72: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%5)), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// IR-NEXT:         let %73: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%71), widen<i32, reason=promotion>(read<i8>(%72)));
// IR-NEXT:         write<i32>(%8, read<i32>(%73));
// IR-NEXT:         let %74: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %75: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), or<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %76: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), read<i32>(%75));
// IR-NEXT:         write<i32>(%8, read<i32>(%76));
// IR-NEXT:         let %77: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %78: i32 [synthetic] = compare_exchange<i32, form=old, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%0)), const<i32>(1), const<i32>(2));
// IR-NEXT:         let %79: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%77), read<i32>(%78));
// IR-NEXT:         write<i32>(%8, read<i32>(%79));
// IR-NEXT:         let %80: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %81: bool [synthetic] = compare_exchange<i32, form=success, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%0)), const<i32>(1), const<i32>(2));
// IR-NEXT:         let %82: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%80), from_bool<i32, reason=promotion>(read<bool>(%81)));
// IR-NEXT:         write<i32>(%8, read<i32>(%82));
// IR-NEXT:         let %83: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %84: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%0)), const<i32>(1));
// IR-NEXT:         let %85: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%83), read<i32>(%84));
// IR-NEXT:         write<i32>(%8, read<i32>(%85));
// IR-NEXT:         write<i32, atomic=release>(deref(addr_of<ptr<i32>>(%0)), const<i32>(0));
// IR-NEXT:         let %86: i32 [synthetic] = read<i32>(%8);
// IR-NEXT:         let %87: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%0)), const<i32>(4));
// IR-NEXT:         let %88: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%86), read<i32>(%87));
// IR-NEXT:         write<i32>(%8, read<i32>(%88));
// IR-NEXT:         return read<i32>(%8);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
