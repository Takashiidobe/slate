// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

_Atomic float counter;
float real;
double wide;
int plain;

float floating(void) {
    float a = __c11_atomic_fetch_add(&counter, 1.5f, 5);
    a += __c11_atomic_fetch_sub(&counter, 1.5f, 0);
    a += __atomic_fetch_add(&real, 2.0f, 2);
    a += __atomic_add_fetch(&real, 2.0f, 3);
    a += __atomic_sub_fetch(&wide, 1, 5);
    return a;
}

float extrema(void) {
    float a = __c11_atomic_fetch_min(&counter, 1.5f, 5);
    a += __c11_atomic_fetch_max(&counter, 1.5f, 0);
    a += __atomic_fetch_min(&real, 2.0f, 2);
    a += __atomic_max_fetch(&real, 2.0f, 3);
    a += __atomic_fetch_max(&wide, 1, 5);
    a += __atomic_fetch_min(&plain, 1, 5);
    return a;
}

float propagating(void) {
    float a = __atomic_fetch_fminimum(&real, 1.5f, 5);
    a += __atomic_fetch_fmaximum(&real, 1.5f, 0);
    a += __atomic_fetch_fminimum_num(&wide, 2.0, 2);
    a += __atomic_fetch_fmaximum_num(&wide, 2.0, 3);
    return a;
}

unsigned counted;

unsigned wrapping(unsigned limit) {
    unsigned a = __atomic_fetch_uinc(&counted, limit, 5);
    a += __atomic_fetch_udec(&counted, limit, 2);
    a += __atomic_fetch_uinc(&plain, limit, 0);
    return a;
}

_Bool flag;
_Atomic _Bool atomic_flag;

int flags(int wide) {
    int a = __atomic_fetch_add(&flag, 1, 5);
    a += __atomic_fetch_add(&flag, wide, 5);
    a += __atomic_fetch_and(&flag, 1, 0);
    a += __atomic_fetch_min(&flag, 1, 5);
    a += __atomic_or_fetch(&flag, 1, 5);
    a += __c11_atomic_fetch_add(&atomic_flag, 1, 5);
    a += __sync_fetch_and_xor(&flag, 1);
    return a;
}

int weakness(int weak, int *expected, int *desired) {
    int a = __atomic_compare_exchange_n(&plain, expected, 3, weak, 5, 5);
    a += __atomic_compare_exchange(&plain, expected, desired, weak + 1, 2, 0);
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
// IR-NEXT:     global %[[VALUE_counter:[0-9]+]] counter: atomic f32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_real:[0-9]+]] real: f32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_wide:[0-9]+]] wide: f64 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_plain:[0-9]+]] plain: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_counted:[0-9]+]] counted: u32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_flag:[0-9]+]] flag: bool [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_atomic_flag:[0-9]+]] atomic_flag: atomic bool [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_floating:[0-9]+]] @floating() -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a:[0-9]+]] a: f32 [storage=automatic];
// IR-NEXT:         let %[[VALUE0:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic f32>>(%[[VALUE_counter]])), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         write<f32>(%[[VALUE_a]], read<f32>(%[[VALUE0]]));
// IR-NEXT:         let %[[VALUE1:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE2:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic f32>>(%[[VALUE_counter]])), sub<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         let %[[VALUE3:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE1]]), read<f32>(%[[VALUE2]]));
// IR-NEXT:         write<f32>(%[[VALUE_a]], read<f32>(%[[VALUE3]]));
// IR-NEXT:         let %[[VALUE4:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE5:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=acquire>(deref(addr_of<ptr<f32>>(%[[VALUE_real]])), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %[[VALUE6:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE4]]), read<f32>(%[[VALUE5]]));
// IR-NEXT:         write<f32>(%[[VALUE_a]], read<f32>(%[[VALUE6]]));
// IR-NEXT:         let %[[VALUE7:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE8:[0-9]+]]: f32 [synthetic] = update<f32, result=new, atomic=release>(deref(addr_of<ptr<f32>>(%[[VALUE_real]])), add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %[[VALUE9:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE7]]), read<f32>(%[[VALUE8]]));
// IR-NEXT:         write<f32>(%[[VALUE_a]], read<f32>(%[[VALUE9]]));
// IR-NEXT:         let %[[VALUE10:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a]]);
// IR-NEXT:         let %[[VALUE11:[0-9]+]]: f64 [synthetic] = update<f64, result=new, atomic=seq_cst>(deref(addr_of<ptr<f64>>(%[[VALUE_wide]])), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// IR-NEXT:         let %[[VALUE12:[0-9]+]]: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE10]])), read<f64>(%[[VALUE11]])));
// IR-NEXT:         write<f32>(%[[VALUE_a]], read<f32>(%[[VALUE12]]));
// IR-NEXT:         return read<f32>(%[[VALUE_a]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_extrema:[0-9]+]] @extrema() -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: f32 [storage=automatic];
// IR-NEXT:         let %[[VALUE13:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic f32>>(%[[VALUE_counter]])), minnum<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         write<f32>(%[[VALUE_a_2]], read<f32>(%[[VALUE13]]));
// IR-NEXT:         let %[[VALUE14:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE15:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic f32>>(%[[VALUE_counter]])), maxnum<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         let %[[VALUE16:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE14]]), read<f32>(%[[VALUE15]]));
// IR-NEXT:         write<f32>(%[[VALUE_a_2]], read<f32>(%[[VALUE16]]));
// IR-NEXT:         let %[[VALUE17:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE18:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=acquire>(deref(addr_of<ptr<f32>>(%[[VALUE_real]])), minnum<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %[[VALUE19:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE17]]), read<f32>(%[[VALUE18]]));
// IR-NEXT:         write<f32>(%[[VALUE_a_2]], read<f32>(%[[VALUE19]]));
// IR-NEXT:         let %[[VALUE20:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE21:[0-9]+]]: f32 [synthetic] = update<f32, result=new, atomic=release>(deref(addr_of<ptr<f32>>(%[[VALUE_real]])), maxnum<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %[[VALUE22:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE20]]), read<f32>(%[[VALUE21]]));
// IR-NEXT:         write<f32>(%[[VALUE_a_2]], read<f32>(%[[VALUE22]]));
// IR-NEXT:         let %[[VALUE23:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE24:[0-9]+]]: f64 [synthetic] = update<f64, result=old, atomic=seq_cst>(deref(addr_of<ptr<f64>>(%[[VALUE_wide]])), maxnum<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// IR-NEXT:         let %[[VALUE25:[0-9]+]]: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE23]])), read<f64>(%[[VALUE24]])));
// IR-NEXT:         write<f32>(%[[VALUE_a_2]], read<f32>(%[[VALUE25]]));
// IR-NEXT:         let %[[VALUE26:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a_2]]);
// IR-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), conditional<i32>(lt<i32>(old<i32>, const<i32>(1)), old<i32>, const<i32>(1)));
// IR-NEXT:         let %[[VALUE28:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE26]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%[[VALUE27]])));
// IR-NEXT:         write<f32>(%[[VALUE_a_2]], read<f32>(%[[VALUE28]]));
// IR-NEXT:         return read<f32>(%[[VALUE_a_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_propagating:[0-9]+]] @propagating() -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_3:[0-9]+]] a: f32 [storage=automatic];
// IR-NEXT:         let %[[VALUE29:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(deref(addr_of<ptr<f32>>(%[[VALUE_real]])), minimum<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         write<f32>(%[[VALUE_a_3]], read<f32>(%[[VALUE29]]));
// IR-NEXT:         let %[[VALUE30:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a_3]]);
// IR-NEXT:         let %[[VALUE31:[0-9]+]]: f32 [synthetic] = update<f32, result=old, atomic=relaxed>(deref(addr_of<ptr<f32>>(%[[VALUE_real]])), maximum<f32, rounding=nearest_even, exceptions=ignore, contract=on>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         let %[[VALUE32:[0-9]+]]: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE30]]), read<f32>(%[[VALUE31]]));
// IR-NEXT:         write<f32>(%[[VALUE_a_3]], read<f32>(%[[VALUE32]]));
// IR-NEXT:         let %[[VALUE33:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a_3]]);
// IR-NEXT:         let %[[VALUE34:[0-9]+]]: f64 [synthetic] = update<f64, result=old, atomic=acquire>(deref(addr_of<ptr<f64>>(%[[VALUE_wide]])), minimum_num<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, const<f64>(2.0)));
// IR-NEXT:         let %[[VALUE35:[0-9]+]]: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE33]])), read<f64>(%[[VALUE34]])));
// IR-NEXT:         write<f32>(%[[VALUE_a_3]], read<f32>(%[[VALUE35]]));
// IR-NEXT:         let %[[VALUE36:[0-9]+]]: f32 [synthetic] = read<f32>(%[[VALUE_a_3]]);
// IR-NEXT:         let %[[VALUE37:[0-9]+]]: f64 [synthetic] = update<f64, result=old, atomic=release>(deref(addr_of<ptr<f64>>(%[[VALUE_wide]])), maximum_num<f64, rounding=nearest_even, exceptions=ignore, contract=on>(old<f64>, const<f64>(2.0)));
// IR-NEXT:         let %[[VALUE38:[0-9]+]]: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE36]])), read<f64>(%[[VALUE37]])));
// IR-NEXT:         write<f32>(%[[VALUE_a_3]], read<f32>(%[[VALUE38]]));
// IR-NEXT:         return read<f32>(%[[VALUE_a_3]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_wrapping:[0-9]+]] @wrapping(%[[VALUE_limit:[0-9]+]] limit: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_4:[0-9]+]] a: u32 [storage=automatic];
// IR-NEXT:         let %[[VALUE39:[0-9]+]]: u32 [synthetic] = update<u32, result=old, atomic=seq_cst>(deref(addr_of<ptr<u32>>(%[[VALUE_counted]])), conditional<u32>(ge<u32>(old<u32>, read<u32>(%[[VALUE_limit]])), const<u32>(0), add<u32, overflow=wrap>(old<u32>, const<u32>(1))));
// IR-NEXT:         write<u32>(%[[VALUE_a_4]], read<u32>(%[[VALUE39]]));
// IR-NEXT:         let %[[VALUE40:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_a_4]]);
// IR-NEXT:         let %[[VALUE41:[0-9]+]]: u32 [synthetic] = update<u32, result=old, atomic=acquire>(deref(addr_of<ptr<u32>>(%[[VALUE_counted]])), conditional<u32>(logical_or<bool>(eq<u32>(old<u32>, const<u32>(0)), gt<u32>(old<u32>, read<u32>(%[[VALUE_limit]]))), read<u32>(%[[VALUE_limit]]), sub<u32, overflow=wrap>(old<u32>, const<u32>(1))));
// IR-NEXT:         let %[[VALUE42:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE40]]), read<u32>(%[[VALUE41]]));
// IR-NEXT:         write<u32>(%[[VALUE_a_4]], read<u32>(%[[VALUE42]]));
// IR-NEXT:         let %[[VALUE43:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_a_4]]);
// IR-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), conditional<i32>(ge<u32>(reinterpret<u32, reason=arg, fits=unknown>(old<i32>), reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%[[VALUE_limit]])))), const<i32>(0), add<i32, overflow=wrap>(old<i32>, const<i32>(1))));
// IR-NEXT:         let %[[VALUE45:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE43]]), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%[[VALUE44]])));
// IR-NEXT:         write<u32>(%[[VALUE_a_4]], read<u32>(%[[VALUE45]]));
// IR-NEXT:         return read<u32>(%[[VALUE_a_4]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_flags:[0-9]+]] @flags(%[[VALUE_wide_2:[0-9]+]] wide: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_5:[0-9]+]] a: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE46:[0-9]+]]: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%[[VALUE_flag]])), add<u8, overflow=wrap>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         write<i32>(%[[VALUE_a_5]], from_bool<i32, reason=assign>(ne<u8, reason=arg>(read<u8>(%[[VALUE46]]), const<u8>(0))));
// IR-NEXT:         let %[[VALUE47:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_5]]);
// IR-NEXT:         let %[[VALUE48:[0-9]+]]: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%[[VALUE_flag]])), add<u8, overflow=wrap>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(read<i32>(%[[VALUE_wide_2]]), const<i32>(0)))));
// IR-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE47]]), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%[[VALUE48]]), const<u8>(0))));
// IR-NEXT:         write<i32>(%[[VALUE_a_5]], read<i32>(%[[VALUE49]]));
// IR-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_5]]);
// IR-NEXT:         let %[[VALUE51:[0-9]+]]: u8 [synthetic] = update<u8, result=old, atomic=relaxed>(deref(addr_of<ptr<bool>>(%[[VALUE_flag]])), and<u8>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE50]]), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%[[VALUE51]]), const<u8>(0))));
// IR-NEXT:         write<i32>(%[[VALUE_a_5]], read<i32>(%[[VALUE52]]));
// IR-NEXT:         let %[[VALUE53:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_5]]);
// IR-NEXT:         let %[[VALUE54:[0-9]+]]: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%[[VALUE_flag]])), conditional<u8>(lt<u8>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))), old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE53]]), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%[[VALUE54]]), const<u8>(0))));
// IR-NEXT:         write<i32>(%[[VALUE_a_5]], read<i32>(%[[VALUE55]]));
// IR-NEXT:         let %[[VALUE56:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_5]]);
// IR-NEXT:         let %[[VALUE57:[0-9]+]]: u8 [synthetic] = update<u8, result=new, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%[[VALUE_flag]])), or<u8>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %[[VALUE58:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE56]]), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%[[VALUE57]]), const<u8>(0))));
// IR-NEXT:         write<i32>(%[[VALUE_a_5]], read<i32>(%[[VALUE58]]));
// IR-NEXT:         let %[[VALUE59:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_5]]);
// IR-NEXT:         let %[[VALUE60:[0-9]+]]: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(%[[VALUE_atomic_flag]])), add<u8, overflow=wrap>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %[[VALUE61:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE59]]), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%[[VALUE60]]), const<u8>(0))));
// IR-NEXT:         write<i32>(%[[VALUE_a_5]], read<i32>(%[[VALUE61]]));
// IR-NEXT:         let %[[VALUE62:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_5]]);
// IR-NEXT:         let %[[VALUE63:[0-9]+]]: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%[[VALUE_flag]])), xor<u8>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %[[VALUE64:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE62]]), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%[[VALUE63]]), const<u8>(0))));
// IR-NEXT:         write<i32>(%[[VALUE_a_5]], read<i32>(%[[VALUE64]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a_5]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_weakness:[0-9]+]] @weakness(%[[VALUE_weak:[0-9]+]] weak: i32, %[[VALUE_expected:[0-9]+]] expected: ptr<i32>, %[[VALUE_desired:[0-9]+]] desired: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_a_6:[0-9]+]] a: i32 [storage=automatic];
// IR-NEXT:         let %[[VALUE65:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=dynamic(read<i32>(%[[VALUE_weak]])), success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), read<ptr<i32>>(%[[VALUE_expected]]), const<i32>(3));
// IR-NEXT:         write<i32>(%[[VALUE_a_6]], from_bool<i32, reason=assign>(read<bool>(%[[VALUE65]])));
// IR-NEXT:         let %[[VALUE66:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a_6]]);
// IR-NEXT:         let %[[VALUE67:[0-9]+]]: bool [synthetic] = compare_exchange<i32, form=write_back, weak=dynamic(add<i32, overflow=ub>(read<i32>(%[[VALUE_weak]]), const<i32>(1))), success=acquire, failure=relaxed>(deref(addr_of<ptr<i32>>(%[[VALUE_plain]])), read<ptr<i32>>(%[[VALUE_expected]]), read<i32>(deref(read<ptr<i32>>(%[[VALUE_desired]]))));
// IR-NEXT:         let %[[VALUE68:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE66]]), from_bool<i32, reason=promotion>(read<bool>(%[[VALUE67]])));
// IR-NEXT:         write<i32>(%[[VALUE_a_6]], read<i32>(%[[VALUE68]]));
// IR-NEXT:         return read<i32>(%[[VALUE_a_6]]);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
