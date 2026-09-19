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
// IR-NEXT:     global %0 counter: f32 [storage=static] [linkage=external];
// IR-NEXT:     global %1 real: f32 [storage=static] [linkage=external];
// IR-NEXT:     global %2 wide: f64 [storage=static] [linkage=external];
// IR-NEXT:     global %3 plain: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %4 @floating() -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %5 a: f32 [storage=automatic];
// IR-NEXT:         let %11: f32 [synthetic] = update<f32, result=old, atomic=seq_cst>(deref(addr_of<ptr<atomic f32>>(%0)), add<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         write<f32>(%5, read<f32>(%11));
// IR-NEXT:         let %12: f32 [synthetic] = read<f32>(%5);
// IR-NEXT:         let %13: f32 [synthetic] = update<f32, result=old, atomic=relaxed>(deref(addr_of<ptr<atomic f32>>(%0)), sub<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(1.5)));
// IR-NEXT:         let %14: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%12), read<f32>(%13));
// IR-NEXT:         write<f32>(%5, read<f32>(%14));
// IR-NEXT:         let %15: f32 [synthetic] = read<f32>(%5);
// IR-NEXT:         let %16: f32 [synthetic] = update<f32, result=old, atomic=acquire>(deref(addr_of<ptr<f32>>(%1)), add<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %17: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%15), read<f32>(%16));
// IR-NEXT:         write<f32>(%5, read<f32>(%17));
// IR-NEXT:         let %18: f32 [synthetic] = read<f32>(%5);
// IR-NEXT:         let %19: f32 [synthetic] = update<f32, result=new, atomic=release>(deref(addr_of<ptr<f32>>(%1)), add<f32, rounding=nearest_even, exceptions=ignore>(old<f32>, const<f32>(2.0)));
// IR-NEXT:         let %20: f32 [synthetic] = add<f32, rounding=nearest_even, exceptions=ignore>(read<f32>(%18), read<f32>(%19));
// IR-NEXT:         write<f32>(%5, read<f32>(%20));
// IR-NEXT:         let %21: f32 [synthetic] = read<f32>(%5);
// IR-NEXT:         let %22: f64 [synthetic] = update<f64, result=new, atomic=seq_cst>(deref(addr_of<ptr<f64>>(%2)), sub<f64, rounding=nearest_even, exceptions=ignore>(old<f64>, int_to_float<f64, reason=arg, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1))));
// IR-NEXT:         let %23: f32 [synthetic] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(add<f64, rounding=nearest_even, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%21)), read<f64>(%22)));
// IR-NEXT:         write<f32>(%5, read<f32>(%23));
// IR-NEXT:         return read<f32>(%5);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @weakness(%7 weak: i32, %8 expected: ptr<i32>, %9 desired: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %10 a: i32 [storage=automatic];
// IR-NEXT:         let %24: bool [synthetic] = compare_exchange<i32, form=write_back, weak=dynamic(read<i32>(%7)), success=seq_cst, failure=seq_cst>(deref(addr_of<ptr<i32>>(%3)), read<ptr<i32>>(%8), const<i32>(3));
// IR-NEXT:         write<i32>(%10, from_bool<i32, reason=assign>(read<bool>(%24)));
// IR-NEXT:         let %25: i32 [synthetic] = read<i32>(%10);
// IR-NEXT:         let %26: bool [synthetic] = compare_exchange<i32, form=write_back, weak=dynamic(add<i32, overflow=ub>(read<i32>(%7), const<i32>(1))), success=acquire, failure=relaxed>(deref(addr_of<ptr<i32>>(%3)), read<ptr<i32>>(%8), read<i32>(deref(read<ptr<i32>>(%9))));
// IR-NEXT:         let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), from_bool<i32, reason=promotion>(read<bool>(%26)));
// IR-NEXT:         write<i32>(%10, read<i32>(%27));
// IR-NEXT:         return read<i32>(%10);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
