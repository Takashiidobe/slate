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
// IR-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_bits:[0-9]+]] bits: u32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_narrow:[0-9]+]] narrow: i8 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_sync:[0-9]+]] @sync(%[[VALUE_protected:[0-9]+]] protected: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE0]]));
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), sub<i32, overflow=wrap>(old<i32>, truncate<i32, reason=arg, fits=always>(const<i64>(1))));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), read<i32>(%[[VALUE2]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE3]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), or<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), read<i32>(%[[VALUE5]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE6]]));
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), and<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), read<i32>(%[[VALUE8]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE9]]));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), xor<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), read<i32>(%[[VALUE11]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE12]]));
// IR-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), not<i32>(and<i32>(old<i32>, const<i32>(2))));
// IR-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), read<i32>(%[[VALUE14]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE15]]));
// IR-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), read<i32>(%[[VALUE17]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE18]]));
// IR-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), sub<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE19]]), read<i32>(%[[VALUE20]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE21]]));
// IR-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), or<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE22]]), read<i32>(%[[VALUE23]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE24]]));
// IR-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), and<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE25]]), read<i32>(%[[VALUE26]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE27]]));
// IR-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), xor<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), read<i32>(%[[VALUE29]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE30]]));
// IR-NEXT:         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), not<i32>(and<i32>(old<i32>, const<i32>(2))));
// IR-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), read<i32>(%[[VALUE32]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE33]]));
// IR-NEXT:         let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE35:[0-9]+]]: u32 [synthetic] = update<u32, result=old, atomic=seq_cst>(deref(addr_of<ptr<u32>>(%[[VALUE_bits]])), conditional<u32>(lt<i32>(reinterpret<i32, reason=arg, fits=unknown>(old<u32>), reinterpret<i32, reason=arg, fits=unknown>(reinterpret<u32, reason=arg, fits=always>(const<i32>(3)))), old<u32>, reinterpret<u32, reason=arg, fits=always>(const<i32>(3))));
// IR-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE34]])), read<u32>(%[[VALUE35]])));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE36]]));
// IR-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), conditional<i32>(gt<i32>(old<i32>, const<i32>(3)), old<i32>, const<i32>(3)));
// IR-NEXT:         let %[[VALUE39:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE37]]), read<i32>(%[[VALUE38]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE39]]));
// IR-NEXT:         let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE41:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), conditional<i32>(lt<u32>(reinterpret<u32, reason=arg, fits=unknown>(old<i32>), reinterpret<u32, reason=arg, fits=always>(const<i32>(3))), old<i32>, const<i32>(3)));
// IR-NEXT:         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE40]]), read<i32>(%[[VALUE41]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE42]]));
// IR-NEXT:         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE44:[0-9]+]]: u32 [synthetic] = update<u32, result=old, atomic=seq_cst>(deref(addr_of<ptr<u32>>(%[[VALUE_bits]])), conditional<u32>(gt<u32>(old<u32>, reinterpret<u32, reason=arg, fits=always>(const<i32>(3))), old<u32>, reinterpret<u32, reason=arg, fits=always>(const<i32>(3))));
// IR-NEXT:         let %[[VALUE45:[0-9]+]]: i32 [synthetic] = reinterpret<i32, reason=assign, fits=unknown>(add<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE43]])), read<u32>(%[[VALUE44]])));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE45]]));
// IR-NEXT:         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE47:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=success, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(1), const<i32>(2));
// IR-NEXT:         let %[[VALUE48:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE47]])));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE48]]));
// IR-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = compare_exchange<i32, form=old, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(1), const<i32>(2));
// IR-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE49]]), read<i32>(%[[VALUE50]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE51]]));
// IR-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(1));
// IR-NEXT:         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE52]]), read<i32>(%[[VALUE53]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE54]]));
// IR-NEXT:         write<i32, atomic=release>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(0));
// IR-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE56:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(4));
// IR-NEXT:         let %[[VALUE57:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE55]]), read<i32>(%[[VALUE56]]));
// IR-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE57]]));
// IR-NEXT:         fence<scope=thread, order=seq_cst>;
// IR-NEXT:         return read<i32>(%[[VALUE_a]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_sized:[0-9]+]] @sized(%[[VALUE_protected_2:[0-9]+]] protected: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), add<i32, overflow=wrap>(old<i32>, const<i32>(1)));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE58]]));
// IR-NEXT:         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE60:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_narrow]])), not<i8>(and<i8>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(2)))));
// IR-NEXT:         let %[[VALUE61:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE60]])));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE61]]));
// IR-NEXT:         let %[[VALUE62:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE63:[0-9]+]]: i8 [synthetic] = update<i8, result=old, atomic=seq_cst>(deref(addr_of<ptr<i8>>(%[[VALUE_narrow]])), add<i8, overflow=wrap>(old<i8>, truncate<i8, reason=arg, fits=always>(const<i32>(1))));
// IR-NEXT:         let %[[VALUE64:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE62]]), widen<i32, reason=promotion>(read<i8>(%[[VALUE63]])));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE64]]));
// IR-NEXT:         let %[[VALUE65:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE66:[0-9]+]]: i32 [synthetic] = update<i32, result=new, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), or<i32>(old<i32>, const<i32>(2)));
// IR-NEXT:         let %[[VALUE67:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE65]]), read<i32>(%[[VALUE66]]));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE67]]));
// IR-NEXT:         let %[[VALUE68:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE69:[0-9]+]]: i32 [synthetic] = compare_exchange<i32, form=old, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(1), const<i32>(2));
// IR-NEXT:         let %[[VALUE70:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE68]]), read<i32>(%[[VALUE69]]));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE70]]));
// IR-NEXT:         let %[[VALUE71:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE72:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=success, weak=false, success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(1), const<i32>(2));
// IR-NEXT:         let %[[VALUE73:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE71]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE72]])));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE73]]));
// IR-NEXT:         let %[[VALUE74:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE75:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=acquire>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(1));
// IR-NEXT:         let %[[VALUE76:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE74]]), read<i32>(%[[VALUE75]]));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE76]]));
// IR-NEXT:         write<i32, atomic=release>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(0));
// IR-NEXT:         let %[[VALUE77:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE78:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), const<i32>(4));
// IR-NEXT:         let %[[VALUE79:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE77]]), read<i32>(%[[VALUE78]]));
// IR-NEXT:         write<i32>(%[[VALUE_a_2]], read<i32>(%[[VALUE79]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a_2]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
