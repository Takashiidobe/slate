// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR c23

extern double narrow(float x);
unsigned long sizes(int array[4]) {
    int local[3];
    return sizeof(local) + sizeof(array) + sizeof(narrow(1.0));
}
int nulls(int *p) {
    int *q = 2 - 2;
    int *r = nullptr;
    q = (int *)0;
    return p == (1 - 1) ? !q : p != r;
}
int shadow(int x) {
    { short x = 1; x += 2; }
    return x;
}
int address(int x) { int *p = &x; *p = 'a'; return +*p; }
float casts(double d, unsigned long u) { return (short)d + (float)u; }

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
// IR-NEXT:     fn %0 @narrow(%17 x: f32) -> f64 [linkage=external];
// IR-NEXT:     fn %1 @sizes(%2 array: ptr<i32> [array=4]) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %3 local: array<i32, 3> [storage=automatic];
// IR-NEXT:         return add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(12), const<u64>(8)), const<u64>(8));
// IR-NEXT:     }
// IR-NEXT:     fn %4 @nulls(%5 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %6 q: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// IR-NEXT:         let %7 r: ptr<i32> [storage=automatic] = null<ptr<i32>>;
// IR-NEXT:         write<ptr<i32>>(%6, null<ptr<i32>>);
// IR-NEXT:         return conditional<i32>(eq<ptr<i32>>(read<ptr<i32>>(%5), null<ptr<i32>>), from_bool<i32, reason=promotion>(not<bool>(ne<ptr<i32>>(read<ptr<i32>>(%6), null<ptr<i32>>))), from_bool<i32, reason=promotion>(ne<ptr<i32>>(read<ptr<i32>>(%5), read<ptr<i32>>(%7))));
// IR-NEXT:     }
// IR-NEXT:     fn %8 @shadow(%9 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         {
// IR-NEXT:             let %10 x: i16 [storage=automatic] = truncate<i16, reason=assign, fits=always>(const<i32>(1));
// IR-NEXT:             let %18: i16 [synthetic] = read<i16>(%10);
// IR-NEXT:             let %19: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%18)), const<i32>(2)));
// IR-NEXT:             write<i16>(%10, read<i16>(%19));
// IR-NEXT:         }
// IR-NEXT:         return read<i32>(%9);
// IR-NEXT:     }
// IR-NEXT:     fn %11 @address(%12 x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %13 p: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%12);
// IR-NEXT:         write<i32>(deref(read<ptr<i32>>(%13)), const<i32>(97));
// IR-NEXT:         return read<i32>(deref(read<ptr<i32>>(%13)));
// IR-NEXT:     }
// IR-NEXT:     fn %14 @casts(%15 d: f64, %16 u: u64) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(widen<i32, reason=promotion>(float_to_int<i16, reason=explicit, out_of_range=ub, exceptions=ignore>(read<f64>(%15)))), int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(%16)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
