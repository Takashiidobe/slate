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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %0 counter: atomic f32 [storage=static] [linkage=external];
// IR-NEXT:     global %1 real: f32 [storage=static] [linkage=external];
// IR-NEXT:     global %2 wide: f64 [storage=static] [linkage=external];
// IR-NEXT:     global %3 plain: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %10 counted: u32 [storage=static] [linkage=external];
// IR-NEXT:     global %14 flag: bool [storage=static] [linkage=external];
// IR-NEXT:     global %15 atomic_flag: atomic bool [storage=static] [linkage=external];
// IR-NEXT:     fn %4 @floating() -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %5 a: f32 [storage=automatic];
// IR-NEXT:         let %24: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic f32>>(%0)), add<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         write<f32>(%5, read<f32>(%24));
// IR-NEXT:         let %25: f32 [synthetic] = read<f32>(%5);
// IR-NEXT:         let %26: f32 [synthetic] = update<f32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic f32>>(%0)), sub<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         let %27: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%25), read<f32>(%26));
// IR-NEXT:         write<f32>(%5, read<f32>(%27));
// IR-NEXT:         let %28: f32 [synthetic] = read<f32>(%5);
// IR-NEXT:         let %29: f32 [synthetic] = update<f32, result=old, atomic=acquire>(deref(addr_of<ptr<f32>>(%1)), add<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %30: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%28), read<f32>(%29));
// IR-NEXT:         write<f32>(%5, read<f32>(%30));
// IR-NEXT:         let %31: f32 [synthetic] = read<f32>(%5);
// IR-NEXT:         let %32: f32 [synthetic] = update<f32, result=new, atomic=release>(deref(addr_of<ptr<f32>>(%1)), add<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %33: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%31), read<f32>(%32));
// IR-NEXT:         write<f32>(%5, read<f32>(%33));
// IR-NEXT:         let %34: f32 [synthetic] = read<f32>(%5);
// IR-NEXT:         let %35: f64 [synthetic] = update<f64, result=new, atomic=seq_cst>(deref(addr_of<ptr<f64>>(%2)), sub<f64, rounding=nearest_even, exceptions=ignore>(old<f64>, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// IR-NEXT:         let %36: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%34)), read<f64>(%35)));
// IR-NEXT:         write<f32>(%5, read<f32>(%36));
// IR-NEXT:         return read<f32>(%5);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @extrema() -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %7 a: f32 [storage=automatic];
// IR-NEXT:         let %37: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic f32>>(%0)), minnum<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         write<f32>(%7, read<f32>(%37));
// IR-NEXT:         let %38: f32 [synthetic] = read<f32>(%7);
// IR-NEXT:         let %39: f32 [synthetic] = update<f32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic f32>>(%0)), maxnum<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         let %40: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%38), read<f32>(%39));
// IR-NEXT:         write<f32>(%7, read<f32>(%40));
// IR-NEXT:         let %41: f32 [synthetic] = read<f32>(%7);
// IR-NEXT:         let %42: f32 [synthetic] = update<f32, result=old, atomic=acquire>(deref(addr_of<ptr<f32>>(%1)), minnum<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %43: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%41), read<f32>(%42));
// IR-NEXT:         write<f32>(%7, read<f32>(%43));
// IR-NEXT:         let %44: f32 [synthetic] = read<f32>(%7);
// IR-NEXT:         let %45: f32 [synthetic] = update<f32, result=new, atomic=release>(deref(addr_of<ptr<f32>>(%1)), maxnum<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %46: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%44), read<f32>(%45));
// IR-NEXT:         write<f32>(%7, read<f32>(%46));
// IR-NEXT:         let %47: f32 [synthetic] = read<f32>(%7);
// IR-NEXT:         let %48: f64 [synthetic] = update<f64, result=old, atomic=seq_cst>(deref(addr_of<ptr<f64>>(%2)), maxnum<f64, rounding=nearest_even, exceptions=ignore>(old<f64>, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// IR-NEXT:         let %49: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%47)), read<f64>(%48)));
// IR-NEXT:         write<f32>(%7, read<f32>(%49));
// IR-NEXT:         let %50: f32 [synthetic] = read<f32>(%7);
// IR-NEXT:         let %51: i32 [synthetic] = update<i32, result=old, atomic=seq_cst>(deref(addr_of<ptr<i32>>(%3)), conditional<i32>(lt<i32>(old<i32>, const<i32>(1)), old<i32>, const<i32>(1)));
// IR-NEXT:         let %52: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%50), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<i32>(%51)));
// IR-NEXT:         write<f32>(%7, read<f32>(%52));
// IR-NEXT:         return read<f32>(%7);
// IR-NEXT:     }
// IR-NEXT:     fn %8 @propagating() -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %9 a: f32 [storage=automatic];
// IR-NEXT:         let %53: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(deref(addr_of<ptr<f32>>(%1)), minimum<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         write<f32>(%9, read<f32>(%53));
// IR-NEXT:         let %54: f32 [synthetic] = read<f32>(%9);
// IR-NEXT:         let %55: f32 [synthetic] = update<f32, result=old, atomic=relaxed>(deref(addr_of<ptr<f32>>(%1)), maximum<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         let %56: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%54), read<f32>(%55));
// IR-NEXT:         write<f32>(%9, read<f32>(%56));
// IR-NEXT:         let %57: f32 [synthetic] = read<f32>(%9);
// IR-NEXT:         let %58: f64 [synthetic] = update<f64, result=old, atomic=acquire>(deref(addr_of<ptr<f64>>(%2)), minimum_num<f64, rounding=nearest_even, exceptions=ignore>(old<f64>, const<f64>(2.0)));
// IR-NEXT:         let %59: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%57)), read<f64>(%58)));
// IR-NEXT:         write<f32>(%9, read<f32>(%59));
// IR-NEXT:         let %60: f32 [synthetic] = read<f32>(%9);
// IR-NEXT:         let %61: f64 [synthetic] = update<f64, result=old, atomic=release>(deref(addr_of<ptr<f64>>(%2)), maximum_num<f64, rounding=nearest_even, exceptions=ignore>(old<f64>, const<f64>(2.0)));
// IR-NEXT:         let %62: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%60)), read<f64>(%61)));
// IR-NEXT:         write<f32>(%9, read<f32>(%62));
// IR-NEXT:         return read<f32>(%9);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @wrapping(%12 limit: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %13 a: u32 [storage=automatic];
// IR-NEXT:         let %63: u32 [synthetic] = update<u32, result=old, atomic=seq_cst>(deref(addr_of<ptr<u32>>(%10)), conditional<u32>(ge<u32>(old<u32>, read<u32>(%12)), const<u32>(0), add<u32, overflow=wrap>(old<u32>, const<u32>(1))));
// IR-NEXT:         write<u32>(%13, read<u32>(%63));
// IR-NEXT:         let %64: u32 [synthetic] = read<u32>(%13);
// IR-NEXT:         let %65: u32 [synthetic] = update<u32, result=old, atomic=acquire>(deref(addr_of<ptr<u32>>(%10)), conditional<u32>(logical_or<bool>(eq<u32>(old<u32>, const<u32>(0)), gt<u32>(old<u32>, read<u32>(%12))), read<u32>(%12), sub<u32, overflow=wrap>(old<u32>, const<u32>(1))));
// IR-NEXT:         let %66: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%64), read<u32>(%65));
// IR-NEXT:         write<u32>(%13, read<u32>(%66));
// IR-NEXT:         let %67: u32 [synthetic] = read<u32>(%13);
// IR-NEXT:         let %68: i32 [synthetic] = update<i32, result=old, atomic=relaxed>(deref(addr_of<ptr<i32>>(%3)), conditional<i32>(ge<u32>(reinterpret<u32, reason=arg, fits=unknown>(old<i32>), reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=arg, fits=unknown>(read<u32>(%12)))), const<i32>(0), add<i32, overflow=wrap>(old<i32>, const<i32>(1))));
// IR-NEXT:         let %69: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%67), reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(%68)));
// IR-NEXT:         write<u32>(%13, read<u32>(%69));
// IR-NEXT:         return read<u32>(%13);
// IR-NEXT:     }
// IR-NEXT:     fn %16 @flags(%17 wide: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %18 a: i32 [storage=automatic];
// IR-NEXT:         let %70: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%14)), add<u8, overflow=wrap>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         write<i32>(%18, from_bool<i32, reason=assign>(ne<u8, reason=arg>(read<u8>(%70), const<u8>(0))));
// IR-NEXT:         let %71: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %72: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%14)), add<u8, overflow=wrap>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(read<i32>(%17), const<i32>(0)))));
// IR-NEXT:         let %73: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%71), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%72), const<u8>(0))));
// IR-NEXT:         write<i32>(%18, read<i32>(%73));
// IR-NEXT:         let %74: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %75: u8 [synthetic] = update<u8, result=old, atomic=relaxed>(deref(addr_of<ptr<bool>>(%14)), and<u8>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %76: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%75), const<u8>(0))));
// IR-NEXT:         write<i32>(%18, read<i32>(%76));
// IR-NEXT:         let %77: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %78: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%14)), conditional<u8>(lt<u8>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))), old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %79: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%77), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%78), const<u8>(0))));
// IR-NEXT:         write<i32>(%18, read<i32>(%79));
// IR-NEXT:         let %80: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %81: u8 [synthetic] = update<u8, result=new, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%14)), or<u8>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %82: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%80), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%81), const<u8>(0))));
// IR-NEXT:         write<i32>(%18, read<i32>(%82));
// IR-NEXT:         let %83: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %84: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic bool>>(%15)), add<u8, overflow=wrap>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %85: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%83), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%84), const<u8>(0))));
// IR-NEXT:         write<i32>(%18, read<i32>(%85));
// IR-NEXT:         let %86: i32 [synthetic] = read<i32>(%18);
// IR-NEXT:         let %87: u8 [synthetic] = update<u8, result=old, atomic=seq_cst>(deref(addr_of<ptr<bool>>(%14)), xor<u8>(old<u8>, from_bool<u8, reason=arg>(ne<i32, reason=arg>(const<i32>(1), const<i32>(0)))));
// IR-NEXT:         let %88: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%86), from_bool<i32, reason=promotion>(ne<u8, reason=arg>(read<u8>(%87), const<u8>(0))));
// IR-NEXT:         write<i32>(%18, read<i32>(%88));
// IR-NEXT:         return read<i32>(%18);
// IR-NEXT:     }
// IR-NEXT:     fn %19 @weakness(%20 weak: i32, %21 expected: ptr<i32>, %22 desired: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %23 a: i32 [storage=automatic];
// IR-NEXT:         let %89: bool [synthetic] = compare_exchange<i32, form=write_back, weak=dynamic(read<i32>(%20)), success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%3)), read<ptr<i32>>(%21), const<i32>(3));
// IR-NEXT:         write<i32>(%23, from_bool<i32, reason=assign>(read<bool>(%89)));
// IR-NEXT:         let %90: i32 [synthetic] = read<i32>(%23);
// IR-NEXT:         let %91: bool [synthetic] = compare_exchange<i32, form=write_back, weak=dynamic(add<i32, overflow=ub>(read<i32>(%20), const<i32>(1))), success=acquire, failure=relaxed>(deref(addr_of<ptr<i32>>(%3)), read<ptr<i32>>(%21), read<i32>(deref(read<ptr<i32>>(%22))));
// IR-NEXT:         let %92: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%90), from_bool<i32, reason=promotion>(read<bool>(%91)));
// IR-NEXT:         write<i32>(%23, read<i32>(%92));
// IR-NEXT:         return read<i32>(%23);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
